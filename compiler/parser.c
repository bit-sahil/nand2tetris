#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "../common/boolean.h"
#include "../common/file_handler.h"
#include "../common/file_reader.h"
#include "../common/code_reader.h"
#include "../common/code_delimit.h"
#include "tokenizer.h"
#include "tokenizer_api.h"
#include "lookahead_tokenizer.h"
#include "xml_writer.h"


// function declarations
int handle_statements(TokenizerConfig* tc, FILE* outfp);


typedef struct ParserConfig {
    TokenizerConfig* tc;
} ParserConfig;


ParserConfig* init_parser(char* file_name, char* out_ext, int max_lookahead) {
    // initialize parser config 

    ParserConfig* pc = (ParserConfig*) malloc(sizeof(ParserConfig));

    pc->tc = init_lookahead_tokenizer(file_name, out_ext, max_lookahead);

    return pc;
}


void dealloc_parser(ParserConfig* pc) {
    // deallocate space taken by DirConfig and opened write file
    dealloc_tokenizer(pc->tc);

    free(pc);
}


int lookahead_keyword(TokenizerConfig* tc, char* expected) {
    // lookahead by 1 place to check whether it's expected keyword

    if(!advance_lookahead(tc, 1)) {
        printf("ParserError: advance_lookahead failed\n");
        return False;
    }

    if(lookahead_token_type(tc, 1) != KEYWORD) {
        // printf("ParserError: lookahead_token_type\n");
        return False;
    }

    if( strcmp( lookahead_raw_token(tc, 1), expected) != 0 ) {
        // printf("ParserError: lookahead_raw_token=%s; expected=%s\n", lookahead_raw_token(tc, 1), expected);
        return False;
    }

    return True;
}


int lookahead_k(TokenizerConfig* tc, TokenType t, int k) {
    // lookahead by k place and check whether it's expected type

    if(!advance_lookahead(tc, k)) {
        printf("ParserError: lookahead_k advance_lookahead failed\n");
        return False;
    }

    if(lookahead_token_type(tc, k) != t) {
        // printf("ParserError: lookahead_k lookahead_token_type\n");
        return False;
    }

    return True;
}


int lookahead_symbol(TokenizerConfig* tc, char expected) {
    // lookahead by 1 place to check whether it's expected symbol

    if(!advance_lookahead(tc, 1))
        return False;

    if(lookahead_token_type(tc, 1) != SYMBOL)
        return False;

    if(get_symbol_token(lookahead_raw_token(tc, 1)) != expected)
        return False;

    return True;
}


int adv(TokenizerConfig* tc, TokenType expected) {
    // advance token, checking that we do have more tokens
    // also match expected token type with current token type and return True/False

    if(!has_more_tokens(tc)) {
        printf("ParserError: No more tokens, but we're expecting token type = %d\n", expected);
        return False;
    }

    advance_token(tc);
    TokenType t = get_token_type(tc);

    if(t != expected) {
        // printf("ParserError: Expected token type=%d; returned token type=%d for token=%s\n", expected, t, get_raw_token(tc));
        return False;
    }

    // printf("Emitted Token: %s; token type=%d\n", get_raw_token(tc), t);

    return True;
}


int handle_keyword(TokenizerConfig* tc,  char* val, FILE* outfp) {
    // advance and handle keyword to be same as val

    if(!adv(tc, KEYWORD))
        return False;

    char* kw = get_keyword(tc);

    if(strcmp(kw, val) != 0)
        return False;

    out_str(kw, "keyword", outfp);

    return True;
}


int expect_char(TokenizerConfig* tc, char c, FILE* outfp) {
    // just expecting this character to be present as next token

    if(!adv(tc, SYMBOL))
        return False;
    
    if( (get_token_type(tc) != SYMBOL) || ( get_symbol(tc) != c ) )
        // not required symbol
        return False;

    out_symbol(c, "symbol", outfp);

    return True;
}


int handle_identifier(TokenizerConfig* tc, FILE* outfp) {
    // identifier cannot be last token

    if(!adv(tc, IDENTIFIER))
        return False;

    out_str(get_identifier(tc), "identifier", outfp);

    return True;
}


int handle_int_const(TokenizerConfig* tc, FILE* outfp) {
    // integer constant cannot be last token

    if(!adv(tc, INT_CONST))
        return False;

    out_str(get_raw_token(tc), "integerConstant", outfp);

    return True;
}


int handle_string_const(TokenizerConfig* tc, FILE* outfp) {
    // integer constant cannot be last token

    if(!adv(tc, STRING_CONST))
        return False;

    out_sym_str(get_string_val(tc), "stringConstant", outfp);

    return True;
}


int handle_keyword_const(TokenizerConfig* tc, FILE* outfp) {
    // 'true'|'false'|'null'|'this'

    char* kw;
    if(lookahead_keyword(tc, "true")) {
        kw = "true";
    } else if(lookahead_keyword(tc, "false")) {
        kw = "false";
    } else if(lookahead_keyword(tc, "null")) {
        kw = "null";
    } else if(lookahead_keyword(tc, "this")) {
        kw = "this";
    } else {
        printf("ParserError: Failed to handle keyword constant\n");
        return False;
    }

    return handle_keyword(tc, kw, outfp);
}


int handle_classname(TokenizerConfig* tc, FILE* outfp) {
    // class name is just an identifier

    return handle_identifier(tc, outfp);
}


int handle_var_name(TokenizerConfig* tc, FILE* outfp) {
    // variable name

    return handle_identifier(tc, outfp);
}


int handle_type(TokenizerConfig* tc, FILE* outfp, int include_void) {
    // 'int'|'char'|'boolean' | className
    // include_void: include void as well for type checking

    char* kw;
    if(lookahead_keyword(tc, "int")) {
        kw = "int";
    } else if(lookahead_keyword(tc, "char")) {
        kw = "char";
    } else if(lookahead_keyword(tc, "boolean")) {
        kw = "boolean";
    } else if(include_void && lookahead_keyword(tc, "void")) {
        kw = "void";
    } else {
        return handle_identifier(tc, outfp);
    }

    // printf("handle_type with kw=%s\n", kw);

    return handle_keyword(tc, kw, outfp);
}


int handle_class_var_dec(TokenizerConfig* tc, FILE* outfp) {
    //  ('static'|'field') type varName (',' varName)* ';'
    
    // 0 or more
    char* kw;
    if(lookahead_keyword(tc, "static")) {
        kw = "static";
    } else if(lookahead_keyword(tc, "field")) {
        kw = "field";
    } else {
        return True;
    }

    //printf("handle_class_var_dec: kw=%s;\n", kw);

    fprintf(outfp, "<classVarDec>\n");

    // it is a class variable declaration
    if(!handle_keyword(tc, kw, outfp))
        return False;

    if(!handle_type(tc, outfp, False))
        return False;

    if(!handle_var_name(tc, outfp))
        return False;

    // (',' varName)*
    while( lookahead_symbol(tc, ',') ) {
        if(!expect_char(tc, ',', outfp))
            return False;

        if(!handle_var_name(tc, outfp))
            return False;
    }

    if( !expect_char(tc, ';', outfp) )
        return False;

    fprintf(outfp, "</classVarDec>\n");

    return handle_class_var_dec(tc, outfp);
}


int handle_subroutine_name(TokenizerConfig* tc, FILE* outfp) {
    // identifying name

    return handle_identifier(tc, outfp);
}


int handle_param_list(TokenizerConfig* tc, FILE* outfp) {
    // ((type varName) (',' type varName)*)?
    // assume param list is not empty, caller needs to check

    while(True) {
        if(!handle_type(tc, outfp, False))
            return False;

        if(!handle_var_name(tc, outfp))
            return False;

        if(!lookahead_symbol(tc, ',')) {
            break;
        }

        if(!expect_char(tc, ',', outfp))
            return False;
    }

    return True;
}


int handle_var_dec(TokenizerConfig* tc, FILE* outfp) {
    // 'var' type varName (',' varName)* ';'

    if(!lookahead_keyword(tc, "var"))
        return True;

    fprintf(outfp, "<varDec>\n");

    if(!handle_keyword(tc, "var", outfp))
        return False;

    if(!handle_type(tc, outfp, False))
        return False;

    if(!handle_var_name(tc, outfp))
        return False;

    while(lookahead_symbol(tc, ',')) {
        if(!expect_char(tc, ',', outfp))
            return False;

        if(!handle_var_name(tc, outfp))
            return False;
    }

    if(!expect_char(tc, ';', outfp))
        return False;

    fprintf(outfp, "</varDec>\n");

    return handle_var_dec(tc, outfp);
}


int handle_term(TokenizerConfig* tc, FILE* outfp) {
    //  integerConstant | stringConstant | keywordConstant | varName | varName '[' expression ']' | subroutineCall |
    // '(' expression ')' | unaryOp term

    fprintf(outfp, "<term>\n");

    if(lookahead_k(tc, INT_CONST, 1)) {
        if(!handle_int_const(tc, outfp))
            return False;
    
    } else if(lookahead_k(tc, STRING_CONST, 1)) {
        if(!handle_string_const(tc, outfp))
            return False;
    
    } else if(lookahead_k(tc, KEYWORD, 1)) {
        if(!handle_keyword_const(tc, outfp))
            return False;
    
    } else {
        if(!handle_var_name(tc, outfp))
            return False;

        // todo: other terms to be handled...
        // printf("Some possible unhandled term here :p\n");
        // return True;
    }

    fprintf(outfp, "</term>\n");

    return True;
}


int handle_expression(TokenizerConfig* tc, FILE* outfp) {
    // term (op term)*

    fprintf(outfp, "<expression>\n");

    if(!handle_term(tc, outfp))
        return False;

    // todo: handle (op term)*

    fprintf(outfp, "</expression>\n");

    return True;
}


int handle_expression_list(TokenizerConfig* tc, FILE* outfp) {
    //  (expression (',' expression)* )?

    if(!handle_expression(tc, outfp))
        return False;

    while(lookahead_symbol(tc, ',')) {
        if(!expect_char(tc, ',', outfp))
            return False;

        if(!handle_expression(tc, outfp))
            return False;
    }

    return True;
}


int handle_let_statement(TokenizerConfig* tc, FILE* outfp) {
    // 'let' varName ('[' expression ']')? '=' expression ';'
    // already checked that it's a let statement

    fprintf(outfp, "<letStatement>\n");

    if(!handle_keyword(tc, "let", outfp))
        return False;

    if(!handle_var_name(tc, outfp))
        return False;

    if(lookahead_symbol(tc, '[')) {
        if(!expect_char(tc, '[', outfp))
            return False;

        if(!handle_expression(tc, outfp))
            return False;

        if(!expect_char(tc, ']', outfp))
            return False;
    }

    if(!expect_char(tc, '=', outfp))
        return False;

    if(!handle_expression(tc, outfp))
        return False;

    if(!expect_char(tc, ';', outfp))
        return False;

    fprintf(outfp, "</letStatement>\n");

    return True;
}


int handle_if_statement(TokenizerConfig* tc, FILE* outfp) {
    // 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?
    // already checked that it's an if statement

    fprintf(outfp, "<ifStatement>\n");

    if(!handle_keyword(tc, "if", outfp))
        return False;

    // expression
    if(!expect_char(tc, '(', outfp))
        return False;

    if(!handle_expression(tc, outfp))
        return False;

    if(!expect_char(tc, ')', outfp))
        return False;

    // statements
    if(!expect_char(tc, '{', outfp))
        return False;

    if(!handle_statements(tc, outfp))
        return False;

    if(!expect_char(tc, '}', outfp))
        return False;

    // else
    if(lookahead_keyword(tc, "else")) {
        // statements
        if(!handle_keyword(tc, "else", outfp))
            return False;

        if(!expect_char(tc, '{', outfp))
            return False;

        if(!handle_statements(tc, outfp))
            return False;

        if(!expect_char(tc, '}', outfp))
            return False;
    }

    fprintf(outfp, "</ifStatement>\n");

    return True;
}


int handle_while_statement(TokenizerConfig* tc, FILE* outfp) {
    // 'while' '(' expression ')' '{' statements '}'
    // already checked that it's a 'while' statement
    
    fprintf(outfp, "<whileStatement>\n");

    if(!handle_keyword(tc, "while", outfp))
        return False;

    // expression
    if(!expect_char(tc, '(', outfp))
        return False;

    if(!handle_expression(tc, outfp))
            return False;

    if(!expect_char(tc, ')', outfp))
        return False;

    // statements
    if(!expect_char(tc, '{', outfp))
        return False;

    if(!handle_statements(tc, outfp))
        return False;

    if(!expect_char(tc, '}', outfp))
        return False;

    fprintf(outfp, "</whileStatement>\n");

    return True;
}


int handle_subroutine_call(TokenizerConfig* tc, FILE* outfp) {
    //  subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName '(' expressionList ')'

    if(!lookahead_k(tc, SYMBOL, 2)) {
        printf("ParserError: expecting lookahead 2 token to be symbol\n");
        return False;
    }

    char* raw = lookahead_raw_token(tc, 2);
    char c = get_symbol_token(raw);

    if( c == '(') {
        // subroutineName '(' expressionList ')'

        if(!handle_subroutine_name(tc, outfp))
            return False;

        if(!expect_char(tc, '(', outfp))
            return False;

        fprintf(outfp, "<expressionList>\n");
        if(!lookahead_symbol(tc, ')')) {
            if(!handle_expression_list(tc, outfp))
                return False;
        }
        fprintf(outfp, "</expressionList>\n");

        if(!expect_char(tc, ')', outfp))
            return False;

    } else if( c == '.') {
        // (className | varName) '.' subroutineName '(' expressionList ')'

        if(!handle_var_name(tc, outfp))
            return False;

        if(!expect_char(tc, '.', outfp))
            return False;

        if(!handle_subroutine_name(tc, outfp))
            return False;

        if(!expect_char(tc, '(', outfp))
            return False;

        fprintf(outfp, "<expressionList>\n");
        if(!lookahead_symbol(tc, ')')) {
            if(!handle_expression_list(tc, outfp))
                return False;
        }
        fprintf(outfp, "</expressionList>\n");

        if(!expect_char(tc, ')', outfp))
            return False;

    } else {
        // expected either variations of subroutine call
        printf("ParserError: handle_subroutine_call, got c=%c; raw=%s\n", c, raw);
        return False;
    }

    return True;
}


int handle_do_statement(TokenizerConfig* tc, FILE* outfp) {
    // 'do' subroutineCall ';'
    // already checked that it's a 'do' statement

    fprintf(outfp, "<doStatement>\n");
    
    if(!handle_keyword(tc, "do", outfp))
        return False;

    if(!handle_subroutine_call(tc, outfp))
        return False;

    if(!expect_char(tc, ';', outfp))
        return False;

    fprintf(outfp, "</doStatement>\n");

    return True;
}


int handle_return_statement(TokenizerConfig* tc, FILE* outfp) {
    // 'return' expression? ';'
    // already checked that it's a 'return' statement

    fprintf(outfp, "<returnStatement>\n");
    
    if(!handle_keyword(tc, "return", outfp))
        return False;

    if(!lookahead_symbol(tc, ';')) {
        if(!handle_expression(tc, outfp))
            return False;
    }

    if(!expect_char(tc, ';', outfp))
        return False;

    fprintf(outfp, "</returnStatement>\n");

    return True;
}


int handle_statement(TokenizerConfig* tc, FILE* outfp) {
    //  letStatement | ifStatement | whileStatement | doStatement | returnStatement

    if(lookahead_keyword(tc, "let")) {
        if(!handle_let_statement(tc, outfp))
            return False;
    
    } else if(lookahead_keyword(tc, "if")) {
        if(!handle_if_statement(tc, outfp))
            return False;

    } else if(lookahead_keyword(tc, "do")) {
        if(!handle_do_statement(tc, outfp))
            return False;

    } else if(lookahead_keyword(tc, "while")) {
        if(!handle_while_statement(tc, outfp))
            return False;

    } else if(lookahead_keyword(tc, "return")) {
        if(!handle_return_statement(tc, outfp))
            return False;

    } else {
        // return true if it's not a statements, that is, we've handled all statements already
        return True;
    }

    return handle_statement(tc, outfp);
}


int handle_statements(TokenizerConfig* tc, FILE* outfp) {
    
    fprintf(outfp, "<statements>\n");

    if(!handle_statement(tc, outfp))
        return False;

    fprintf(outfp, "</statements>\n");
    
}


int handle_subroutine_body(TokenizerConfig* tc, FILE* outfp) {
    // '{' varDec* statements '}'

    fprintf(outfp, "<subroutineBody>\n");

    if(!expect_char(tc, '{', outfp))
        return False;

    if(!handle_var_dec(tc, outfp))
        return False;

    if(!handle_statements(tc, outfp))
        return False;

    if(!expect_char(tc, '}', outfp))
        return False;

    fprintf(outfp, "</subroutineBody>\n");

    return True;
}


int handle_subroutine_dec(TokenizerConfig* tc, FILE* outfp) {
    //  ('constructor'|'function'|'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody
    // 0 or more
    char* kw;
    if(lookahead_keyword(tc, "constructor")) {
        kw = "constructor";
    } else if(lookahead_keyword(tc, "function")) {
        kw = "function";
    } else if(lookahead_keyword(tc, "method")) {
        kw = "method";
    } else {
        return True;
    }

    fprintf(outfp, "<subroutineDec>\n");

    // it is a subroutine declaration
    if(!handle_keyword(tc, kw, outfp))
        return False;

    if(!handle_type(tc, outfp, True))
        return False;

    if(!handle_subroutine_name(tc, outfp))
        return False;

    if(!expect_char(tc, '(', outfp))
        return False;

    fprintf(outfp, "<parameterList>\n");
    if(!lookahead_symbol(tc, ')')) {
        // if next token is ')', then there are no params to function

        if(!handle_param_list(tc, outfp))
            return False;
    }
    fprintf(outfp, "</parameterList>\n");

    if(!expect_char(tc, ')', outfp))
        return False;

    if(!handle_subroutine_body(tc, outfp))
        return False;

    fprintf(outfp, "</subroutineDec>\n");

    return handle_subroutine_dec(tc, outfp);
}


int handle_class(TokenizerConfig* tc, FILE* outfp) {
    // handle class
    // 'class' className '{' classVarDec* subroutineDec* '}'

    if(!handle_keyword(tc, "class", outfp))
        return False;

    if(!handle_classname(tc, outfp))
        return False;

    if(!expect_char(tc, '{', outfp))
        return False;

    if(!handle_class_var_dec(tc, outfp))
        return False;

    if(!handle_subroutine_dec(tc, outfp))
        return False;

    if(!expect_char(tc, '}', outfp))
        return False;

    return True;
}


void parse_and_generate_output(TokenizerConfig* tc, FILE* outfp) {
    // create output file corresponding to each input file, one at a time
    // file should begin with a class, and each file has only one class
    fprintf(outfp, "<class>\n");

    if(!handle_class(tc, outfp)) {
        // error during class handling
        printf("Error handling class with current token:%s; token type = %d\n", get_raw_token(tc), get_token_type(tc));
    }

    fprintf(outfp, "</class>\n");
}


void parse_file_or_directory(char* file_name) {
    ParserConfig* pc = init_parser(file_name, "out.xml", 2);

    FILE* outfp;
    TokenizerConfig* tc = pc->tc;

    while(adv_next_file(tc)) {
        outfp = get_out_file(tc->dc, tc->rc->file_name);

        printf("Generating output for file_name=%s\n", tc->rc->file_name);
        parse_and_generate_output(tc, outfp);

        fclose(outfp);
    }

    dealloc_parser(pc);
}


int main(int argc, char* argv[]) {
    // .jack file or directory name is provided as argv[1]

    char* file_name = argv[1];

    parse_file_or_directory(file_name);
}

