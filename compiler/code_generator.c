#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include "symbol_table.h"
#include "tokenizer.h"
#include "tokenizer_api.h"
#include "parser.h"
#include "config.h"


void begin(char* token, GenConfig* genConfig) {
	push_state(genConfig, token);

	if(strcmp(token, "class") == 0) {
		// entering class
		// time for adding symbol table
		push_new_symbol_table(genConfig);
		printf("Adding new symbol_table for class\n");
	
	} else if(strcmp(token, "parameterList") == 0) {
		// variable type should be argument
		strcpy(genConfig->varKind, "argument");

	} else if(strcmp(token, "subroutineDec") == 0) {
		// variable type should be argument
		push_new_symbol_table(genConfig);
		printf("Adding new symbol_table for subroutineDec\n");

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

	}

}


void expect(char* token, GenConfig* genConfig) {
	// printf("expecting: %s\n", token);
	push_expected(genConfig, token);
}


void _symbol(char c, GenConfig* genConfig) {
	// do nothing with symbols unless those are operators
}


void _keyword(char* token_value, GenConfig* genConfig) {
	// printf("_keyword token_value=%s\n", token_value);
	// if(is_non_empty(genConfig->expected))
    // 	printf("_keyword token=%s;expected=%s\n", token_value, top_expected(genConfig));

	if(top_expected_cmp(genConfig, "varKind")) {
		if(strcmp(token_value, "var") == 0) {
			strcpy(genConfig->varKind, "local");

		} else {
			strcpy(genConfig->varKind, token_value);
		}
		pop_expected(genConfig);
	
	} else if(top_expected_cmp(genConfig, "varType")) {
		strcpy(genConfig->varType, token_value);
		pop_expected(genConfig);
	
	} else if(top_expected_cmp(genConfig, "subroutineType")) {
		if(strcmp(token_value, "method") == 0) {
			strcpy(genConfig->varKind, "argument");
			strcpy(genConfig->varType, genConfig->className);
			add_var_symbol_table(genConfig, "this");
		}

		pop_expected(genConfig);
	
	}

}


void _identifier(char* token_value, GenConfig* genConfig) {
	// printf("_identifier token_value=%s\n", token_value);
	// if(is_non_empty(genConfig->expected))
    // 	printf("_keyword token=%s;expected=%s\n", token_value, top_expected(genConfig));

	if(top_expected_cmp(genConfig, "className")) {
		strcpy(genConfig->className, token_value);
		pop_expected(genConfig);
	
	} else if(top_expected_cmp(genConfig, "varType")) {
		strcpy(genConfig->varType, token_value);
		pop_expected(genConfig);
	
	} else if(top_expected_cmp(genConfig, "decVarName")) {
		// add variable in symbol table
		add_var_symbol_table(genConfig, token_value);
		pop_expected(genConfig);
	
	}

}


void _integerConstant(char* token_value, GenConfig* genConfig) {

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
    
    if(action == BEGIN) {
        begin(token, genConfig);
    } else if (action == END) {
        end(token, genConfig);
    } else if (action == EXPECT) {
        expect(token, genConfig);
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
