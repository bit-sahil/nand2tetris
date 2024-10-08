#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include "../common/file_handler.h"
#include "../common/file_reader.h"
#include "../common/code_reader.h"
#include "../common/code_delimit.h"
#include "tokenizer.h"
#include "tokenizer_api.h"
#include "lookahead_tokenizer.h"
#include "xml_writer.h"

// function signature to be passed around
#define GenFuncPtr(F) void (*F) (char*, int, FILE*)

// function declarations
int handle_statements(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc));

int handle_subroutine_call(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc));

int handle_expression(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc));


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
        return false;
    }

    if(lookahead_token_type(tc, 1) != KEYWORD) {
        // printf("ParserError: lookahead_token_type\n");
        return false;
    }

    if( strcmp( lookahead_raw_token(tc, 1), expected) != 0 ) {
        // printf("ParserError: lookahead_raw_token=%s; expected=%s\n", lookahead_raw_token(tc, 1), expected);
        return false;
    }

    return true;
}


int lookahead_k(TokenizerConfig* tc, TokenType t, int k) {
    // lookahead by k place and check whether it's expected type

    if(!advance_lookahead(tc, k)) {
        printf("ParserError: lookahead_k advance_lookahead failed\n");
        return false;
    }

    if(lookahead_token_type(tc, k) != t) {
        // printf("ParserError: lookahead_k lookahead_token_type\n");
        return false;
    }

    return true;
}


int lookahead_symbol(TokenizerConfig* tc, char expected) {
    // lookahead by 1 place to check whether it's expected symbol

    if(!advance_lookahead(tc, 1))
        return false;

    if(lookahead_token_type(tc, 1) != SYMBOL)
        return false;

    if(get_symbol_token(lookahead_raw_token(tc, 1)) != expected)
        return false;

    return true;
}


int adv(TokenizerConfig* tc, TokenType expected) {
    // advance token, checking that we do have more tokens
    // also match expected token type with current token type and return true/false

    if(!has_more_tokens(tc)) {
        printf("ParserError: No more tokens, but we're expecting token type = %d\n", expected);
        return false;
    }

    advance_token(tc);
    TokenType t = get_token_type(tc);

    if(t != expected) {
        // printf("ParserError: Expected token type=%d; returned token type=%d for token=%s\n", expected, t, get_raw_token(tc));
        return false;
    }

    // printf("Emitted Token: %s; token type=%d\n", get_raw_token(tc), t);

    return true;
}


int handle_keyword(TokenizerConfig* tc,  char* val, FILE* outfp, GenFuncPtr(genFunc)) {
    // advance and handle keyword to be same as val

    if(!adv(tc, KEYWORD))
        return false;

    char* kw = get_keyword(tc);

    if(strcmp(kw, val) != 0)
        return false;

    genFunc(kw, KEYWORD, outfp);

    return true;
}


int expect_char(TokenizerConfig* tc, char c, FILE* outfp, GenFuncPtr(genFunc)) {
    // just expecting this character to be present as next token

    if(!adv(tc, SYMBOL))
        return false;
    
    if( (get_token_type(tc) != SYMBOL) || ( get_symbol(tc) != c ) )
        // not required symbol
        return false;

    genFunc(get_raw_token(tc), SYMBOL, outfp);

    return true;
}


int handle_identifier(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    // identifier cannot be last token

    if(!adv(tc, IDENTIFIER))
        return false;

    genFunc(get_identifier(tc), IDENTIFIER, outfp);

    return true;
}


int handle_int_const(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    // integer constant cannot be last token

    if(!adv(tc, INT_CONST))
        return false;

    genFunc(get_raw_token(tc), INT_CONST, outfp);

    return true;
}


int handle_string_const(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    // integer constant cannot be last token

    if(!adv(tc, STRING_CONST))
        return false;

    genFunc(get_string_val(tc), STRING_CONST, outfp);

    return true;
}


int handle_keyword_const(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
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
        return false;
    }

    return handle_keyword(tc, kw, outfp, genFunc);
}


int handle_classname(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    // class name is just an identifier

    return handle_identifier(tc, outfp, genFunc);
}


int handle_var_name(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    // variable name

    return handle_identifier(tc, outfp, genFunc);
}


int handle_type(TokenizerConfig* tc, int include_void, FILE* outfp, GenFuncPtr(genFunc)) {
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
        return handle_identifier(tc, outfp, genFunc);
    }

    // printf("handle_type with kw=%s\n", kw);

    return handle_keyword(tc, kw, outfp, genFunc);
}


int handle_class_var_dec(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    //  ('static'|'field') type varName (',' varName)* ';'
    
    // 0 or more
    char* kw;
    if(lookahead_keyword(tc, "static")) {
        kw = "static";
    } else if(lookahead_keyword(tc, "field")) {
        kw = "field";
    } else {
        return true;
    }

    //printf("handle_class_var_dec: kw=%s;\n", kw);

    genFunc("classVarDec", BEGIN, outfp);

    // it is a class variable declaration
    if(!handle_keyword(tc, kw, outfp, genFunc))
        return false;

    if(!handle_type(tc, false, outfp, genFunc))
        return false;

    if(!handle_var_name(tc, outfp, genFunc))
        return false;

    // (',' varName)*
    while( lookahead_symbol(tc, ',') ) {
        if(!expect_char(tc, ',', outfp, genFunc))
            return false;

        if(!handle_var_name(tc, outfp, genFunc))
            return false;
    }

    if( !expect_char(tc, ';', outfp, genFunc))
        return false;

    genFunc("classVarDec", END, outfp);

    return handle_class_var_dec(tc, outfp, genFunc);
}


int handle_subroutine_name(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    // identifying name

    return handle_identifier(tc, outfp, genFunc);
}


int handle_param_list(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    // ((type varName) (',' type varName)*)?
    // assume param list is not empty, caller needs to check

    while(true) {
        if(!handle_type(tc, false, outfp, genFunc))
            return false;

        if(!handle_var_name(tc, outfp, genFunc))
            return false;

        if(!lookahead_symbol(tc, ',')) {
            break;
        }

        if(!expect_char(tc, ',', outfp, genFunc))
            return false;
    }

    return true;
}


int handle_var_dec(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    // 'var' type varName (',' varName)* ';'

    if(!lookahead_keyword(tc, "var"))
        return true;

    genFunc("varDec", BEGIN, outfp);

    if(!handle_keyword(tc, "var", outfp, genFunc))
        return false;

    if(!handle_type(tc, false, outfp, genFunc))
        return false;

    if(!handle_var_name(tc, outfp, genFunc))
        return false;

    while(lookahead_symbol(tc, ',')) {
        if(!expect_char(tc, ',', outfp, genFunc))
            return false;

        if(!handle_var_name(tc, outfp, genFunc))
            return false;
    }

    if(!expect_char(tc, ';', outfp, genFunc))
        return false;

    genFunc("varDec", END, outfp);

    return handle_var_dec(tc, outfp, genFunc);
}


int handle_unaryop(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    // '-'|'~'

    if(lookahead_symbol(tc, '-')) {
        if(!expect_char(tc, '-', outfp, genFunc))
            return false;
    
    } else if(lookahead_symbol(tc, '~')) {
        if(!expect_char(tc, '~', outfp, genFunc))
            return false;
    
    } else {
        printf("ParserError: handle_unaryop, expected unary op not found\n");
        return false;
    }

    return true;
}


int handle_term(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    //  integerConstant | stringConstant | keywordConstant | varName | varName '[' expression ']' | subroutineCall |
    // '(' expression ')' | unaryOp term

    genFunc("term", BEGIN, outfp);

    if(lookahead_k(tc, INT_CONST, 1)) {
        if(!handle_int_const(tc, outfp, genFunc))
            return false;
    
    } else if(lookahead_k(tc, STRING_CONST, 1)) {
        if(!handle_string_const(tc, outfp, genFunc))
            return false;
    
    } else if(lookahead_k(tc, KEYWORD, 1)) {        
        if(!handle_keyword_const(tc, outfp, genFunc))
            return false;
        
    
    } else if(lookahead_k(tc, SYMBOL, 1)) {
        // '(' expression ')' | unaryOp term

        if(lookahead_symbol(tc, '(')) {

            if(!expect_char(tc, '(', outfp, genFunc))
                return false;

            if(!handle_expression(tc, outfp, genFunc))
                return false;

            if(!expect_char(tc, ')', outfp, genFunc))
                return false;
        
        } else {

            if(!handle_unaryop(tc, outfp, genFunc))
                return false;

            if(!handle_term(tc, outfp, genFunc))
                return false;
        }
    
    } else  {
        // varName | varName '[' expression ']' 
        // | subroutineCall = subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName '(' expressionList ')'

        if(lookahead_k(tc, SYMBOL, 2)) {
            // varName '[' expression ']' | subroutineCall
            
            char* raw = lookahead_raw_token(tc, 2);
            char c = get_symbol_token(raw);

            if( c == '[') {
                if(!handle_var_name(tc, outfp, genFunc))
                    return false;

                if(!expect_char(tc, '[', outfp, genFunc))
                    return false;

                if(!handle_expression(tc, outfp, genFunc))
                    return false;

                if(!expect_char(tc, ']', outfp, genFunc))
                    return false;
            
            } else if( c == '(' || c == '.') {
                if(!handle_subroutine_call(tc, outfp, genFunc))
                    return false;
            
            } else {
                // just handle variable name
                if(!handle_var_name(tc, outfp, genFunc))
                    return false;
            }
        }
    }

    genFunc("term", END, outfp);

    return true;
}


int lookahead_op(TokenizerConfig* tc) {
    // '+'|'-'|'*'|'/'|'&'|'|'|'<'|'>'|'='

    if(!lookahead_k(tc, SYMBOL, 1))
        return false;

    char* raw = lookahead_raw_token(tc, 1);
    char c = get_symbol_token(raw);

    switch (c) {
        case '+':
            return '+';
        case '-':
            return '-';
        case '*':
            return '*';
        case '/':
            return '/';
        case '&':
            return '&';
        case '|':
            return '|';
        case '<':
            return '<';
        case '>':
            return '>';
        case '=':
            return '=';
        default:
            // printf("ParserError: lookahead_op, binary op not found:%c\n", c);
            return false;
    }
}


int handle_expression(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    // term (op term)*

    genFunc("expression", BEGIN, outfp);

    if(!handle_term(tc, outfp, genFunc))
        return false;

    // todo: handle (op term)*

    char c;

    while( (c = lookahead_op(tc)) ) {
        // op is just a char
        if(!expect_char(tc, c, outfp, genFunc))
            return false;

        if(!handle_term(tc, outfp, genFunc))
            return false;
    }

    genFunc("expression", END, outfp);

    return true;
}


int handle_expression_list(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    //  (expression (',' expression)* )?

    if(!handle_expression(tc, outfp, genFunc))
        return false;

    while(lookahead_symbol(tc, ',')) {
        if(!expect_char(tc, ',', outfp, genFunc))
            return false;

        if(!handle_expression(tc, outfp, genFunc))
            return false;
    }

    return true;
}


int handle_let_statement(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    // 'let' varName ('[' expression ']')? '=' expression ';'
    // already checked that it's a let statement

    genFunc("letStatement", BEGIN, outfp);

    if(!handle_keyword(tc, "let", outfp, genFunc))
        return false;

    if(!handle_var_name(tc, outfp, genFunc))
        return false;

    if(lookahead_symbol(tc, '[')) {
        if(!expect_char(tc, '[', outfp, genFunc))
            return false;

        if(!handle_expression(tc, outfp, genFunc))
            return false;

        if(!expect_char(tc, ']', outfp, genFunc))
            return false;
    }

    if(!expect_char(tc, '=', outfp, genFunc))
        return false;

    if(!handle_expression(tc, outfp, genFunc))
        return false;

    if(!expect_char(tc, ';', outfp, genFunc))
        return false;

    genFunc("letStatement", END, outfp);

    return true;
}


int handle_if_statement(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    // 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?
    // already checked that it's an if statement

    genFunc("ifStatement", BEGIN, outfp);

    if(!handle_keyword(tc, "if", outfp, genFunc))
        return false;

    // expression
    if(!expect_char(tc, '(', outfp, genFunc))
        return false;

    if(!handle_expression(tc, outfp, genFunc))
        return false;

    if(!expect_char(tc, ')', outfp, genFunc))
        return false;

    // statements
    if(!expect_char(tc, '{', outfp, genFunc))
        return false;

    if(!handle_statements(tc, outfp, genFunc))
        return false;

    if(!expect_char(tc, '}', outfp, genFunc))
        return false;

    // else
    if(lookahead_keyword(tc, "else")) {
        // statements
        if(!handle_keyword(tc, "else", outfp, genFunc))
            return false;

        if(!expect_char(tc, '{', outfp, genFunc))
            return false;

        if(!handle_statements(tc, outfp, genFunc))
            return false;

        if(!expect_char(tc, '}', outfp, genFunc))
            return false;
    }

    genFunc("ifStatement", END, outfp);

    return true;
}


int handle_while_statement(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    // 'while' '(' expression ')' '{' statements '}'
    // already checked that it's a 'while' statement
    
    genFunc("whileStatement", BEGIN, outfp);

    if(!handle_keyword(tc, "while", outfp, genFunc))
        return false;

    // expression
    if(!expect_char(tc, '(', outfp, genFunc))
        return false;

    if(!handle_expression(tc, outfp, genFunc))
            return false;

    if(!expect_char(tc, ')', outfp, genFunc))
        return false;

    // statements
    if(!expect_char(tc, '{', outfp, genFunc))
        return false;

    if(!handle_statements(tc, outfp, genFunc))
        return false;

    if(!expect_char(tc, '}', outfp, genFunc))
        return false;

    genFunc("whileStatement", END, outfp);

    return true;
}


int handle_subroutine_call(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    //  subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName '(' expressionList ')'

    if(!lookahead_k(tc, SYMBOL, 2)) {
        printf("ParserError: expecting lookahead 2 token to be symbol\n");
        return false;
    }

    char* raw = lookahead_raw_token(tc, 2);
    char c = get_symbol_token(raw);

    if( c == '(') {
        // subroutineName '(' expressionList ')'

        if(!handle_subroutine_name(tc, outfp, genFunc))
            return false;

        if(!expect_char(tc, '(', outfp, genFunc))
            return false;

        genFunc("expressionList", BEGIN, outfp);
        if(!lookahead_symbol(tc, ')')) {
            if(!handle_expression_list(tc, outfp, genFunc))
                return false;
        }
        genFunc("expressionList", END, outfp);

        if(!expect_char(tc, ')', outfp, genFunc))
            return false;

    } else if( c == '.') {
        // (className | varName) '.' subroutineName '(' expressionList ')'

        if(!handle_var_name(tc, outfp, genFunc))
            return false;

        if(!expect_char(tc, '.', outfp, genFunc))
            return false;

        if(!handle_subroutine_name(tc, outfp, genFunc))
            return false;

        if(!expect_char(tc, '(', outfp, genFunc))
            return false;

        genFunc("expressionList", BEGIN, outfp);
        if(!lookahead_symbol(tc, ')')) {
            if(!handle_expression_list(tc, outfp, genFunc))
                return false;
        }
        genFunc("expressionList", END, outfp);

        if(!expect_char(tc, ')', outfp, genFunc))
            return false;

    } else {
        // expected either variations of subroutine call
        printf("ParserError: handle_subroutine_call, got c=%c; raw=%s\n", c, raw);
        return false;
    }

    return true;
}


int handle_do_statement(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    // 'do' subroutineCall ';'
    // already checked that it's a 'do' statement

    genFunc("doStatement", BEGIN, outfp);
    
    if(!handle_keyword(tc, "do", outfp, genFunc))
        return false;

    if(!handle_subroutine_call(tc, outfp, genFunc))
        return false;

    if(!expect_char(tc, ';', outfp, genFunc))
        return false;

    genFunc("doStatement", END, outfp);

    return true;
}


int handle_return_statement(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    // 'return' expression? ';'
    // already checked that it's a 'return' statement

    genFunc("returnStatement", BEGIN, outfp);
    
    if(!handle_keyword(tc, "return", outfp, genFunc))
        return false;

    if(!lookahead_symbol(tc, ';')) {
        if(!handle_expression(tc, outfp, genFunc))
            return false;
    }

    if(!expect_char(tc, ';', outfp, genFunc))
        return false;

    genFunc("returnStatement", END, outfp);

    return true;
}


int handle_statement(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    //  letStatement | ifStatement | whileStatement | doStatement | returnStatement

    if(lookahead_keyword(tc, "let")) {
        if(!handle_let_statement(tc, outfp, genFunc))
            return false;
    
    } else if(lookahead_keyword(tc, "if")) {
        if(!handle_if_statement(tc, outfp, genFunc))
            return false;

    } else if(lookahead_keyword(tc, "do")) {
        if(!handle_do_statement(tc, outfp, genFunc))
            return false;

    } else if(lookahead_keyword(tc, "while")) {
        if(!handle_while_statement(tc, outfp, genFunc))
            return false;

    } else if(lookahead_keyword(tc, "return")) {
        if(!handle_return_statement(tc, outfp, genFunc))
            return false;

    } else {
        // return true if it's not a statements, that is, we've handled all statements already
        return true;
    }

    return handle_statement(tc, outfp, genFunc);
}


int handle_statements(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    
    genFunc("statements", BEGIN, outfp);

    if(!handle_statement(tc, outfp, genFunc))
        return false;

    genFunc("statements", END, outfp);
    
}


int handle_subroutine_body(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    // '{' varDec* statements '}'

    genFunc("subroutineBody", BEGIN, outfp);

    if(!expect_char(tc, '{', outfp, genFunc))
        return false;

    if(!handle_var_dec(tc, outfp, genFunc))
        return false;

    if(!handle_statements(tc, outfp, genFunc))
        return false;

    if(!expect_char(tc, '}', outfp, genFunc))
        return false;

    genFunc("subroutineBody", END, outfp);

    return true;
}


int handle_subroutine_dec(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
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
        return true;
    }

    genFunc("subroutineDec", BEGIN, outfp);

    // it is a subroutine declaration
    if(!handle_keyword(tc, kw, outfp, genFunc))
        return false;

    if(!handle_type(tc, true, outfp, genFunc))
        return false;

    if(!handle_subroutine_name(tc, outfp, genFunc))
        return false;

    if(!expect_char(tc, '(', outfp, genFunc))
        return false;

    genFunc("parameterList", BEGIN, outfp);
    if(!lookahead_symbol(tc, ')')) {
        // if next token is ')', then there are no params to function

        if(!handle_param_list(tc, outfp, genFunc))
            return false;
    }
    genFunc("parameterList", END, outfp);

    if(!expect_char(tc, ')', outfp, genFunc))
        return false;

    if(!handle_subroutine_body(tc, outfp, genFunc))
        return false;

    genFunc("subroutineDec", END, outfp);

    return handle_subroutine_dec(tc, outfp, genFunc);
}


int handle_class(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    // handle class
    // 'class' className '{' classVarDec* subroutineDec* '}'

    if(!handle_keyword(tc, "class", outfp, genFunc))
        return false;

    if(!handle_classname(tc, outfp, genFunc))
        return false;

    if(!expect_char(tc, '{', outfp, genFunc))
        return false;

    if(!handle_class_var_dec(tc, outfp, genFunc))
        return false;

    if(!handle_subroutine_dec(tc, outfp, genFunc))
        return false;

    if(!expect_char(tc, '}', outfp, genFunc))
        return false;

    return true;
}


void parse_and_generate_output(TokenizerConfig* tc, FILE* outfp, GenFuncPtr(genFunc)) {
    // create output file corresponding to each input file, one at a time
    // file should begin with a class, and each file has only one class
    genFunc("class", BEGIN, outfp);

    if(!handle_class(tc, outfp, genFunc)) {
        // error during class handling
        printf("Error handling class with current token:%s; token type = %d\n", get_raw_token(tc), get_token_type(tc));
    }

    genFunc("class", END, outfp);
}


void parse_file_or_directory(char* file_name) {
    ParserConfig* pc = init_parser(file_name, "out.xml", 2);

    FILE* outfp;
    TokenizerConfig* tc = pc->tc;

    while(adv_next_file(tc)) {
        outfp = get_out_file(tc->dc, tc->rc->file_name);

        printf("Generating output for file_name=%s\n", tc->rc->file_name);
        parse_and_generate_output(tc, outfp, out_xml);

        fclose(outfp);
    }

    dealloc_parser(pc);
}


int main(int argc, char* argv[]) {
    // .jack file or directory name is provided as argv[1]

    char* file_name = argv[1];

    parse_file_or_directory(file_name);
}

