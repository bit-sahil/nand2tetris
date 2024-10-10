#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "tokenizer.h"
#include "tokenizer_api.h"
#include "parser.h"
#include "config.h"
#include "symbol_table.h"
#include "../common/map.h"



void out_statement(GenConfig* genConfig, char* statement) {
	fprintf(genConfig->outfp, "\t%s\n", statement);
}


void out_func_call(GenConfig* genConfig) {
	// call <class_name>.<function_name> nArgs

	fprintf(genConfig->outfp, "\tcall %s.%s %d\n", context_value(genConfig, "callerClassName"), context_value(genConfig, "subroutineName"), genConfig->nArg);

}


void out_func_signature(GenConfig* genConfig) {
	// function <class_name>.<function_name> nLocal
	// push local 0  }- nLocal times

	int nLocal = symbol_table_num_var_type(genConfig, Local);

	fprintf(genConfig->outfp, "function %s.%s %d\n", context_value(genConfig, "className"), context_value(genConfig, "subroutineName"), nLocal);

	// for(int i=0; i < nLocal; i++)
	// 	fprintf(genConfig->outfp, "\tpush local %d\n", i);
}


void out_push_vm_var(GenConfig* genConfig, char* vm_var_name, char* local_var_name) {
	// push argument to current stack

	fprintf(genConfig->outfp, "\tpush %s  // %s\n", vm_var_name, local_var_name);
}


void out_push_var(GenConfig* genConfig, char* var) {
	char vm_var_name[128];
	vm_var_name_symbol_table(genConfig, var, vm_var_name);
	out_push_vm_var(genConfig, vm_var_name, var);
}



void out_pop_vm_var(GenConfig* genConfig, char* vm_var_name, char* local_var_name) {
	fprintf(genConfig->outfp, "\tpop %s  // %s\n", vm_var_name, local_var_name);
}


void out_pop_var(GenConfig* genConfig, char* var) {
	char vm_var_name[128];
	vm_var_name_symbol_table(genConfig, var, vm_var_name);
	out_pop_vm_var(genConfig, vm_var_name, var);
}


void _push_and_update_label(GenConfig* genConfig) {
	// update running label to stack and update label for next
	char label[4];
	sprintf(label, "%d", genConfig->nLabel++);
	push_curr_label(genConfig, label);
}


void out_keyword_equivalent(GenConfig* genConfig, char* var) {
	if(strcmp(var, "true") == 0) {
		out_push_vm_var(genConfig, "constant 0", var);
		out_statement(genConfig, "not  // true");
	
	} else if(strcmp(var, "false") == 0) {
		out_push_vm_var(genConfig, "constant 0", var);

	} else if(strcmp(var, "null") == 0) {
		out_push_vm_var(genConfig, "constant 0", var);

	} else {
		printf("CodeGeneratorError: unhandled keyword:%s\n", var);
		out_statement(genConfig, "<unhandled keyword");
	}
}


void begin(char* token, GenConfig* genConfig) {
	push_state(genConfig, token);

	if(strcmp(token, "class") == 0) {
		// entering class
		// time for adding symbol table
		push_new_symbol_table(genConfig);
		printf("Adding new symbol_table for class\n");

		// reset running label count in class
		genConfig->nLabel = 0;
	
	} else if(strcmp(token, "parameterList") == 0) {
		// variable kind should be argument
		store_context(genConfig, "varKind", "argument");

	} else if(strcmp(token, "subroutineDec") == 0) {
		// variable type should be argument
		push_new_symbol_table(genConfig);
		printf("Adding new symbol_table for subroutineDec\n");

	} else if(strcmp(token, "subroutineCall") == 0) {
		// next is a function call, so by default callerClassName should be current class name
		store_context(genConfig, "callerClassName", context_value(genConfig, "className"));
		genConfig->nArg = 0;

	} else if(strcmp(token, "whileStatement") == 0) {
		// beginning of while statement
		_push_and_update_label(genConfig);
		fprintf(genConfig->outfp, "label LOOP%s\n", top_curr_label(genConfig));

	} else if(strcmp(token, "ifStatement") == 0) {
		// beginning of if statement
		_push_and_update_label(genConfig);

	}
}


void end(char* token, GenConfig* genConfig) {
	if(top_state_cmp(genConfig, token))
    	pop_state(genConfig);
    else if(is_non_empty(genConfig->state))
    	printf("CodeGeneratorError: End error. Mismatch token=%s;state=%s\n", token, top_state(genConfig));

    if(strcmp(token, "class") == 0) {
		// entering class
		// time for removing class level symbol table
		pop_symbol_table(genConfig);
		printf("pop_symbol_table for class\n");
	
	} else if(strcmp(token, "classVarDec") == 0) {
		print_symbol_table(genConfig->tableStack->table);

	} else if(strcmp(token, "subroutineDec") == 0) {
		// variable type should be argument
		print_symbol_table(genConfig->tableStack->table);
	
		pop_symbol_table(genConfig);
		printf("pop_symbol_table for subroutineDec\n");

	} else if(strcmp(token, "subroutineCall") == 0) {
		// output function call statement
		// all arguments are pushed by now

		out_func_call(genConfig);

	} else if(strcmp(token, "returnStatement") == 0) {
		// end of return statement

		out_statement(genConfig, "return");

	} else if(strcmp(token, "term") == 0) {
		// end of term
		// handle postfix binary op, if any

		if(has_term_op(genConfig) && !top_term_op_cmp(genConfig, "delayOp")) {
			out_statement(genConfig, top_term_op(genConfig));
			pop_term_op(genConfig);
		}

	} else if(strcmp(token, "doStatement") == 0) {
		// additionally handle returned 0
		out_statement(genConfig, "pop temp 7");  // last unused temp register to be safe

	} else if(strcmp(token, "letStatement") == 0) {
		// end of let statement
		// put value in variable which is being set, let x = ...;

		out_pop_var(genConfig, context_value(genConfig, "letVarName"));
	
	} else if(strcmp(token, "whileStatement") == 0) {
		// end of while statement

		fprintf(genConfig->outfp, "\tgoto LOOP%s\n", top_curr_label(genConfig));
		fprintf(genConfig->outfp, "label ENDLOOP%s\n", top_curr_label(genConfig));
		pop_curr_label(genConfig);
	
	} else if(strcmp(token, "ifStatement") == 0) {
		// end of if-else statement block

		fprintf(genConfig->outfp, "label ELSE_END%s\n", top_curr_label(genConfig));
		pop_curr_label(genConfig);
	
	}

}


void expect(char* token, GenConfig* genConfig) {
	// printf("expecting: %s\n", token);
	push_expected(genConfig, token);
}


void end_do(char* token, GenConfig* genConfig) {
	printf("end_do: %s\n", token);
	
	if(strcmp(token, "subroutineVarDec") == 0) {
		// done with subroutine variable declaration
		// we can output function signature now

		out_func_signature(genConfig);
		
	} else if(strcmp(token, "noReturnExpression") == 0) {
		// no expression in return statement
		// default return value should 0
		// push 0 on stack as return value

		out_statement(genConfig, "push constant 0");
		
	} else if(strcmp(token, "callArg") == 0) {
		// callArg for a function call
		// add 1 to nArg
		// arguments would be automatically pushed to stack while handling expressions

		genConfig->nArg++;
		
	} else if(strcmp(token, "delayOp") == 0) {
		// delay adding operation until expression is fully resolved

		push_term_op(genConfig, "delayOp");
		
	} else if(strcmp(token, "endDelayOp") == 0) {
		// end delay since expression is fully resolved

		if(!top_term_op_cmp(genConfig, "delayOp"))
			printf("CodeGeneratorError: unexpected delayOp\n");
		else
			pop_term_op(genConfig);
		
	} else if(strcmp(token, "whileConditionEnd") == 0) {
		// end delay since expression is fully resolved

		out_statement(genConfig, "not");
		fprintf(genConfig->outfp, "\tif-goto ENDLOOP%s\n", top_curr_label(genConfig));
		
	} else if(strcmp(token, "ifConditionEnd") == 0) {
		// end of if condition, if condition is fully resolved

		out_statement(genConfig, "not");
		fprintf(genConfig->outfp, "\tif-goto ELSE%s\n", top_curr_label(genConfig));

	} else if(strcmp(token, "ifStatementsEnd") == 0) {
		// end of if inner statements and beginning of else block

		fprintf(genConfig->outfp, "\tgoto ELSE_END%s\n", top_curr_label(genConfig));
		fprintf(genConfig->outfp, "label ELSE%s\n", top_curr_label(genConfig));
		
	}

}


void _identifier(char* token_value, GenConfig* genConfig) {
	printf("_identifier token_value=%s\n", token_value);
	if(is_non_empty(genConfig->expected))
    	printf("_keyword token=%s;expected=%s\n", token_value, top_expected(genConfig));

	if(top_expected_cmp(genConfig, "className")) {
		store_context(genConfig, "className", token_value);
		pop_expected(genConfig);
	
	} else if(top_expected_cmp(genConfig, "varType")) {
		store_context(genConfig, "varType", token_value);
		pop_expected(genConfig);
	
	} else if(top_expected_cmp(genConfig, "decVarName")) {
		// add variable in symbol table
		add_var_symbol_table(genConfig, token_value);
		pop_expected(genConfig);
	
	} else if(top_expected_cmp(genConfig, "subroutineName")) {
		store_context(genConfig, "subroutineName", token_value);
		pop_expected(genConfig);
	
	} else if(top_expected_cmp(genConfig, "callerClassName")) {
		// it could be a variable of a particular class, or class name
		// find token_value in function symbol table and match type to a given class
		// find token_value in class level symbol table and match type to a given class
		// if no match, then it should be callerClassName itself
		// if match, then get type as callerClassName

		if(has_value_symbol_table(genConfig, token_value)) {
			store_context(genConfig, "callerClassName", var_type_symbol_table(genConfig, token_value));
			
			// also mean that first argument is this, and to be passed in function as arg 0
			out_push_var(genConfig, token_value);

		} else {
			printf("top_expected_cmp callerClassName matched %s\n", token_value);
			store_context(genConfig, "callerClassName", token_value);
			printf("top_expected_cmp callerClassName matched %s\n", context_value(genConfig, "callerClassName"));
		}

		pop_expected(genConfig);
	
	} else if(top_expected_cmp(genConfig, "letVarName")) {
		store_context(genConfig, "letVarName", token_value);
		pop_expected(genConfig);
	
	} else if(top_expected_cmp(genConfig, "termSimpleVarName")) {
		// just push variable on stack if it's part of term
		out_push_var(genConfig, token_value);

		pop_expected(genConfig);
	
	}


}


void _keyword(char* token_value, GenConfig* genConfig) {
	printf("_keyword token_value=%s\n", token_value);
	if(is_non_empty(genConfig->expected))
    	printf("_keyword token=%s;expected=%s\n", token_value, top_expected(genConfig));

	if(top_expected_cmp(genConfig, "varKind")) {
		if(strcmp(token_value, "var") == 0) {
			store_context(genConfig, "varKind", "local");
		} else {
			store_context(genConfig, "varKind", token_value);
		}
		pop_expected(genConfig);
	
	} else if(top_expected_cmp(genConfig, "varType")) {
		store_context(genConfig, "varType", token_value);
		pop_expected(genConfig);
	
	} else if(top_expected_cmp(genConfig, "subroutineType")) {
		store_context(genConfig, "subroutineType", token_value);

		if(strcmp(token_value, "method") == 0) {
			store_context(genConfig, "varKind", "argument");
			store_context(genConfig, "varType", context_value(genConfig, "className"));
			add_var_symbol_table(genConfig, "this");
		}

		pop_expected(genConfig);
	
	} else if(top_expected_cmp(genConfig, "termKeywordConstant")) {
		// just push keyword constant on stack if it's part of term
		out_keyword_equivalent(genConfig, token_value);
		
		pop_expected(genConfig);
	
	}
}


char* binary_op(char c) {
	switch (c) {
        case '+':
            return "add";
        case '-':
            return "sub";
        case '*':
            return "call Math.multiply 2";
        case '/':
            return "call Math.divide 2";
        case '&':
            return "and";
        case '|':
            return "or";
        case '<':
            return "lt";
        case '>':
            return "gt";
        case '=':
            return "eq";
        default:
            printf("CodeGeneratorError: unhandled binary op:%c\n", c);
            return "<unhandled op>";
    }
}


char* unary_op(char c) {
	switch (c) {
        case '-':
            return "neg";
        case '~':
            return "not";

        default:
            printf("CodeGeneratorError: unhandled unary op:%c\n", c);
            return "<unhandled op>";
    }
}


void _symbol(char c, GenConfig* genConfig) {
	// do nothing with symbols unless those are operators
	if(top_expected_cmp(genConfig, "binaryOp")) {
		push_term_op(genConfig, binary_op(c));
		pop_expected(genConfig);
	
	} else if(top_expected_cmp(genConfig, "unaryOp")) {
		push_term_op(genConfig, unary_op(c));
		pop_expected(genConfig);
	
	}
}


void _integerConstant(char* token_value, GenConfig* genConfig) {
	// integerConstant is only part of term
	// we simply like to push constant integer value on stack

	fprintf(genConfig->outfp, "\tpush constant %s\n", token_value);
}


void _stringConstant(char* token_value, GenConfig* genConfig) {

}


void terminal(char* token_value, int action, GenConfig* genConfig) {
	// printf("terminal token_value=%s;action=%d\n", token_value, action);

    if(action == KEYWORD) {
        _keyword(token_value, genConfig);
    } else if (action == SYMBOL) {
        char c = get_symbol_token(token_value);
        _symbol(c, genConfig);
    } else if (action == IDENTIFIER) {
        _identifier(token_value, genConfig);
    } else if (action == INT_CONST) {
        _integerConstant(token_value, genConfig);
    } else if (action == STRING_CONST) {
        _stringConstant(token_value, genConfig);
    }
}


void generate_vm_code(char* token, int action, GenConfig* genConfig) {
	// printf("token=%s;action=%d\n", token, action);
    
    if(action == BEGIN || action == BEGIN_SP) {
        begin(token, genConfig);
    } else if (action == END || action == END_SP) {
        end(token, genConfig);
    } else if (action == EXPECT) {
        expect(token, genConfig);
    } else if (action == END_DO) {
        end_do(token, genConfig);
    } else {
        // code to output terminals
        // token contains value and action is among TokenType
        terminal(token, action, genConfig);
    }
}


void parse_file_or_directory(char* file_name) {
    ParserConfig* pc = init_parser(file_name, ".vm", 2);

    FILE* outfp;
    TokenizerConfig* tc = pc->tc;

    while(adv_next_file(tc)) {
        outfp = get_out_file(tc->dc, tc->rc->file_name);
        GenConfig* genConfig = init_gen_config(outfp);
        genConfig->context = init_map(256);

        printf("Generating output for file_name=%s\n", tc->rc->file_name);
        parse_and_generate_output(tc, genConfig, generate_vm_code);

        fclose(outfp);
        dealloc_gen_config(genConfig);
    }

    dealloc_parser(pc);
}


int main(int argc, char* argv[]) {
    // .jack file or directory name is provided as argv[1]

    char* file_name = argv[1];

    parse_file_or_directory(file_name);
}
