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
#include "parser.h"
#include "config.h"


// function declarations
int handle_statements(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc));

int handle_subroutine_call(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc));

int handle_expression(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc));


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


int handle_keyword(TokenizerConfig* tc,  char* val, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    // advance and handle keyword to be same as val

    if(!adv(tc, KEYWORD))
        return false;

    char* kw = get_keyword(tc);

    if(strcmp(kw, val) != 0)
        return false;

    genFunc(kw, KEYWORD, genConfig);

    return true;
}


int expect_char(TokenizerConfig* tc, char c, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    // just expecting this character to be present as next token

    if(!adv(tc, SYMBOL))
        return false;
    
    if( (get_token_type(tc) != SYMBOL) || ( get_symbol(tc) != c ) )
        // not required symbol
        return false;

    genFunc(get_raw_token(tc), SYMBOL, genConfig);

    return true;
}


int handle_identifier(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    // identifier cannot be last token

    if(!adv(tc, IDENTIFIER))
        return false;

    genFunc(get_identifier(tc), IDENTIFIER, genConfig);

    return true;
}


int handle_int_const(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    // integer constant cannot be last token

    if(!adv(tc, INT_CONST))
        return false;

    genFunc(get_raw_token(tc), INT_CONST, genConfig);

    return true;
}


int handle_string_const(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    // integer constant cannot be last token

    if(!adv(tc, STRING_CONST))
        return false;

    genFunc(get_string_val(tc), STRING_CONST, genConfig);

    return true;
}


int handle_keyword_const(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
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

    return handle_keyword(tc, kw, genConfig, genFunc);
}


int handle_classname(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    // class name is just an identifier

    genFunc("className", EXPECT, genConfig);

    return handle_identifier(tc, genConfig, genFunc);
}


int handle_var_name(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    // variable name

    // genFunc("varName", EXPECT, genConfig);

    return handle_identifier(tc, genConfig, genFunc);
}


int handle_dec_var_name(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    // declared variable name

    genFunc("decVarName", EXPECT, genConfig);

    return handle_identifier(tc, genConfig, genFunc);
}


int handle_type(TokenizerConfig* tc, int include_void, GenConfig* genConfig, GenFuncPtr(genFunc)) {
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
        return handle_identifier(tc, genConfig, genFunc);
    }

    // printf("handle_type with kw=%s\n", kw);

    return handle_keyword(tc, kw, genConfig, genFunc);
}


int handle_class_var_dec(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
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

    genFunc("classVarDec", BEGIN, genConfig);

    genFunc("varKind", EXPECT, genConfig);
    // it is a class variable declaration
    if(!handle_keyword(tc, kw, genConfig, genFunc))
        return false;

    genFunc("varType", EXPECT, genConfig);
    if(!handle_type(tc, false, genConfig, genFunc))
        return false;

    if(!handle_dec_var_name(tc, genConfig, genFunc))
        return false;

    // (',' varName)*
    while( lookahead_symbol(tc, ',') ) {
        if(!expect_char(tc, ',', genConfig, genFunc))
            return false;

        if(!handle_dec_var_name(tc, genConfig, genFunc))
            return false;
    }

    if( !expect_char(tc, ';', genConfig, genFunc))
        return false;

    genFunc("classVarDec", END, genConfig);

    return handle_class_var_dec(tc, genConfig, genFunc);
}


int handle_subroutine_name(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    // identifying name

    genFunc("subroutineName", EXPECT, genConfig);

    return handle_identifier(tc, genConfig, genFunc);
}


int handle_param_list(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    // ((type varName) (',' type varName)*)?
    // assume param list is not empty, caller needs to check

    while(true) {
        genFunc("varType", EXPECT, genConfig);
        if(!handle_type(tc, false, genConfig, genFunc))
            return false;

        if(!handle_dec_var_name(tc, genConfig, genFunc))
            return false;

        if(!lookahead_symbol(tc, ',')) {
            break;
        }

        if(!expect_char(tc, ',', genConfig, genFunc))
            return false;
    }

    return true;
}


int handle_var_dec(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    // 'var' type varName (',' varName)* ';'

    if(!lookahead_keyword(tc, "var"))
        return true;

    genFunc("varDec", BEGIN, genConfig);

    genFunc("varKind", EXPECT, genConfig);
    if(!handle_keyword(tc, "var", genConfig, genFunc))
        return false;

    genFunc("varType", EXPECT, genConfig);
    if(!handle_type(tc, false, genConfig, genFunc))
        return false;

    if(!handle_dec_var_name(tc, genConfig, genFunc))
        return false;

    while(lookahead_symbol(tc, ',')) {
        if(!expect_char(tc, ',', genConfig, genFunc))
            return false;

        if(!handle_dec_var_name(tc, genConfig, genFunc))
            return false;
    }

    if(!expect_char(tc, ';', genConfig, genFunc))
        return false;

    genFunc("varDec", END, genConfig);

    return handle_var_dec(tc, genConfig, genFunc);
}


int handle_unaryop(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    // '-'|'~'

    if(lookahead_symbol(tc, '-')) {
        genFunc("unaryOp", EXPECT, genConfig);
        if(!expect_char(tc, '-', genConfig, genFunc))
            return false;
    
    } else if(lookahead_symbol(tc, '~')) {
        genFunc("unaryOp", EXPECT, genConfig);
        if(!expect_char(tc, '~', genConfig, genFunc))
            return false;
    
    } else {
        printf("ParserError: handle_unaryop, expected unary op not found\n");
        return false;
    }

    return true;
}


int handle_term(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    //  integerConstant | stringConstant | keywordConstant | varName | varName '[' expression ']' | subroutineCall |
    // '(' expression ')' | unaryOp term

    genFunc("term", BEGIN, genConfig);

    if(lookahead_k(tc, INT_CONST, 1)) {
        if(!handle_int_const(tc, genConfig, genFunc))
            return false;
    
    } else if(lookahead_k(tc, STRING_CONST, 1)) {
        genFunc("stringConstant", EXPECT, genConfig);
        if(!handle_string_const(tc, genConfig, genFunc))
            return false;
    
    } else if(lookahead_k(tc, KEYWORD, 1)) {
        genFunc("termKeywordConstant", EXPECT, genConfig);
        if(!handle_keyword_const(tc, genConfig, genFunc))
            return false;
        
    
    } else if(lookahead_k(tc, SYMBOL, 1)) {
        // '(' expression ')' | unaryOp term

        if(lookahead_symbol(tc, '(')) {

            genFunc("delayOp", END_DO, genConfig);
            if(!expect_char(tc, '(', genConfig, genFunc))
                return false;

            if(!handle_expression(tc, genConfig, genFunc))
                return false;

            if(!expect_char(tc, ')', genConfig, genFunc))
                return false;
            genFunc("endDelayOp", END_DO, genConfig);
        
        } else {

            if(!handle_unaryop(tc, genConfig, genFunc))
                return false;

            if(!handle_term(tc, genConfig, genFunc))
                return false;
        }
    
    } else  {
        // varName | varName '[' expression ']' 
        // | subroutineCall: subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName '(' expressionList ')'

        if(lookahead_k(tc, SYMBOL, 2)) {
            // varName '[' expression ']' | subroutineCall
            
            char* raw = lookahead_raw_token(tc, 2);
            char c = get_symbol_token(raw);

            if( c == '[') {
                genFunc("termArrVarName", EXPECT, genConfig);
                if(!handle_var_name(tc, genConfig, genFunc))
                    return false;

                if(!expect_char(tc, '[', genConfig, genFunc))
                    return false;

                genFunc("delayOp", END_DO, genConfig);
                if(!handle_expression(tc, genConfig, genFunc))
                    return false;
                genFunc("endDelayOp", END_DO, genConfig);

                if(!expect_char(tc, ']', genConfig, genFunc))
                    return false;

                genFunc("termArrEnd", END_DO, genConfig);
            
            } else if( c == '(' || c == '.') {
                if(!handle_subroutine_call(tc, genConfig, genFunc))
                    return false;
            
            } else {
                genFunc("termSimpleVarName", EXPECT, genConfig);
                // just handle variable name
                if(!handle_var_name(tc, genConfig, genFunc))
                    return false;
            }
        }
    }

    genFunc("term", END, genConfig);

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


int handle_expression(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    // term (op term)*

    genFunc("expression", BEGIN, genConfig);

    if(!handle_term(tc, genConfig, genFunc))
        return false;

    // todo: handle (op term)*

    char c;

    while( (c = lookahead_op(tc)) ) {
        // op is just a char

        genFunc("binaryOp", EXPECT, genConfig);
        if(!expect_char(tc, c, genConfig, genFunc))
            return false;

        if(!handle_term(tc, genConfig, genFunc))
            return false;
    }

    genFunc("expression", END, genConfig);

    return true;
}


int handle_expression_list(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    //  (expression (',' expression)* )?

    genFunc("callArg", END_DO, genConfig);
    if(!handle_expression(tc, genConfig, genFunc))
        return false;

    while(lookahead_symbol(tc, ',')) {
        if(!expect_char(tc, ',', genConfig, genFunc))
            return false;

        genFunc("callArg", END_DO, genConfig);
        if(!handle_expression(tc, genConfig, genFunc))
            return false;
    }

    return true;
}


int handle_let_statement(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    // 'let' varName ('[' expression ']')? '=' expression ';'
    // already checked that it's a let statement

    genFunc("letStatement", BEGIN, genConfig);

    if(!handle_keyword(tc, "let", genConfig, genFunc))
        return false;

    genFunc("letVarName", EXPECT, genConfig);
    if(!handle_var_name(tc, genConfig, genFunc))
        return false;

    if(lookahead_symbol(tc, '[')) {
        genFunc("letArrayBegin", END_DO, genConfig);
        if(!expect_char(tc, '[', genConfig, genFunc))
            return false;

        if(!handle_expression(tc, genConfig, genFunc))
            return false;

        if(!expect_char(tc, ']', genConfig, genFunc))
            return false;
        genFunc("letArrayEnd", END_DO, genConfig);
    }

    if(!expect_char(tc, '=', genConfig, genFunc))
        return false;

    if(!handle_expression(tc, genConfig, genFunc))
        return false;

    if(!expect_char(tc, ';', genConfig, genFunc))
        return false;

    genFunc("letStatement", END, genConfig);

    return true;
}


int handle_if_statement(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    // 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?
    // already checked that it's an if statement

    genFunc("ifStatement", BEGIN, genConfig);

    if(!handle_keyword(tc, "if", genConfig, genFunc))
        return false;

    // expression
    if(!expect_char(tc, '(', genConfig, genFunc))
        return false;

    if(!handle_expression(tc, genConfig, genFunc))
        return false;

    if(!expect_char(tc, ')', genConfig, genFunc))
        return false;
    genFunc("ifConditionEnd", END_DO, genConfig);

    // statements
    if(!expect_char(tc, '{', genConfig, genFunc))
        return false;

    if(!handle_statements(tc, genConfig, genFunc))
        return false;

    if(!expect_char(tc, '}', genConfig, genFunc))
        return false;
    genFunc("ifStatementsEnd", END_DO, genConfig);

    // else
    if(lookahead_keyword(tc, "else")) {
        // statements
        if(!handle_keyword(tc, "else", genConfig, genFunc))
            return false;

        if(!expect_char(tc, '{', genConfig, genFunc))
            return false;

        if(!handle_statements(tc, genConfig, genFunc))
            return false;

        if(!expect_char(tc, '}', genConfig, genFunc))
            return false;
    }

    genFunc("ifStatement", END, genConfig);

    return true;
}


int handle_while_statement(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    // 'while' '(' expression ')' '{' statements '}'
    // already checked that it's a 'while' statement
    
    genFunc("whileStatement", BEGIN, genConfig);

    if(!handle_keyword(tc, "while", genConfig, genFunc))
        return false;

    // expression
    if(!expect_char(tc, '(', genConfig, genFunc))
        return false;

    if(!handle_expression(tc, genConfig, genFunc))
            return false;

    if(!expect_char(tc, ')', genConfig, genFunc))
        return false;
    genFunc("whileConditionEnd", END_DO, genConfig);

    // statements
    if(!expect_char(tc, '{', genConfig, genFunc))
        return false;

    if(!handle_statements(tc, genConfig, genFunc))
        return false;

    if(!expect_char(tc, '}', genConfig, genFunc))
        return false;

    genFunc("whileStatement", END, genConfig);

    return true;
}


int handle_subroutine_call(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    //  subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName '(' expressionList ')'

    if(!lookahead_k(tc, SYMBOL, 2)) {
        printf("ParserError: expecting lookahead 2 token to be symbol\n");
        return false;
    }

    genFunc("subroutineCall", BEGIN_SP, genConfig);

    char* raw = lookahead_raw_token(tc, 2);
    char c = get_symbol_token(raw);

    if( c == '(') {
        // subroutineName '(' expressionList ')'

        genFunc("methodCall", END_DO, genConfig);

        if(!handle_subroutine_name(tc, genConfig, genFunc))
            return false;

        if(!expect_char(tc, '(', genConfig, genFunc))
            return false;

        genFunc("expressionList", BEGIN, genConfig);
        if(!lookahead_symbol(tc, ')')) {
            if(!handle_expression_list(tc, genConfig, genFunc))
                return false;
        }
        genFunc("expressionList", END, genConfig);

        if(!expect_char(tc, ')', genConfig, genFunc))
            return false;

    } else if( c == '.') {
        // (className | varName) '.' subroutineName '(' expressionList ')'

        genFunc("callerClassName", EXPECT, genConfig);
        if(!handle_var_name(tc, genConfig, genFunc))
            return false;

        if(!expect_char(tc, '.', genConfig, genFunc))
            return false;

        if(!handle_subroutine_name(tc, genConfig, genFunc))
            return false;

        if(!expect_char(tc, '(', genConfig, genFunc))
            return false;

        genFunc("expressionList", BEGIN, genConfig);
        if(!lookahead_symbol(tc, ')')) {
            if(!handle_expression_list(tc, genConfig, genFunc))
                return false;
        }
        genFunc("expressionList", END, genConfig);

        if(!expect_char(tc, ')', genConfig, genFunc))
            return false;

    } else {
        // expected either variations of subroutine call
        printf("ParserError: handle_subroutine_call, got c=%c; raw=%s\n", c, raw);
        return false;
    }

    genFunc("subroutineCall", END_SP, genConfig);

    return true;
}


int handle_do_statement(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    // 'do' subroutineCall ';'
    // already checked that it's a 'do' statement

    genFunc("doStatement", BEGIN, genConfig);
    
    if(!handle_keyword(tc, "do", genConfig, genFunc))
        return false;

    if(!handle_subroutine_call(tc, genConfig, genFunc))
        return false;

    if(!expect_char(tc, ';', genConfig, genFunc))
        return false;

    genFunc("doStatement", END, genConfig);

    return true;
}


int handle_return_statement(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    // 'return' expression? ';'
    // already checked that it's a 'return' statement

    genFunc("returnStatement", BEGIN, genConfig);
    
    if(!handle_keyword(tc, "return", genConfig, genFunc))
        return false;

    if(!lookahead_symbol(tc, ';')) {
        if(!handle_expression(tc, genConfig, genFunc))
            return false;
    } else {
        genFunc("noReturnExpression", END_DO, genConfig);
    }

    if(!expect_char(tc, ';', genConfig, genFunc))
        return false;

    genFunc("returnStatement", END, genConfig);

    return true;
}


int handle_statement(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    //  letStatement | ifStatement | whileStatement | doStatement | returnStatement

    if(lookahead_keyword(tc, "let")) {
        if(!handle_let_statement(tc, genConfig, genFunc))
            return false;
    
    } else if(lookahead_keyword(tc, "if")) {
        if(!handle_if_statement(tc, genConfig, genFunc))
            return false;

    } else if(lookahead_keyword(tc, "do")) {
        if(!handle_do_statement(tc, genConfig, genFunc))
            return false;

    } else if(lookahead_keyword(tc, "while")) {
        if(!handle_while_statement(tc, genConfig, genFunc))
            return false;

    } else if(lookahead_keyword(tc, "return")) {
        if(!handle_return_statement(tc, genConfig, genFunc))
            return false;

    } else {
        // return true if it's not a statements, that is, we've handled all statements already
        return true;
    }

    return handle_statement(tc, genConfig, genFunc);
}


int handle_statements(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    
    genFunc("statements", BEGIN, genConfig);

    if(!handle_statement(tc, genConfig, genFunc))
        return false;

    genFunc("statements", END, genConfig);
    
}


int handle_subroutine_body(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    // '{' varDec* statements '}'

    genFunc("subroutineBody", BEGIN, genConfig);

    if(!expect_char(tc, '{', genConfig, genFunc))
        return false;

    if(!handle_var_dec(tc, genConfig, genFunc))
        return false;

    genFunc("subroutineVarDec", END_DO, genConfig);

    if(!handle_statements(tc, genConfig, genFunc))
        return false;

    if(!expect_char(tc, '}', genConfig, genFunc))
        return false;

    genFunc("subroutineBody", END, genConfig);

    return true;
}


int handle_subroutine_dec(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
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

    genFunc("subroutineDec", BEGIN, genConfig);

    genFunc("subroutineType", EXPECT, genConfig);
    // it is a subroutine declaration
    if(!handle_keyword(tc, kw, genConfig, genFunc))
        return false;

    if(!handle_type(tc, true, genConfig, genFunc))
        return false;

    if(!handle_subroutine_name(tc, genConfig, genFunc))
        return false;

    if(!expect_char(tc, '(', genConfig, genFunc))
        return false;

    genFunc("parameterList", BEGIN, genConfig);
    if(!lookahead_symbol(tc, ')')) {
        // if next token is ')', then there are no params to function

        if(!handle_param_list(tc, genConfig, genFunc))
            return false;
    }
    genFunc("parameterList", END, genConfig);

    if(!expect_char(tc, ')', genConfig, genFunc))
        return false;

    if(!handle_subroutine_body(tc, genConfig, genFunc))
        return false;

    genFunc("subroutineDec", END, genConfig);

    return handle_subroutine_dec(tc, genConfig, genFunc);
}


int handle_class(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    // handle class
    // 'class' className '{' classVarDec* subroutineDec* '}'

    if(!handle_keyword(tc, "class", genConfig, genFunc))
        return false;

    if(!handle_classname(tc, genConfig, genFunc))
        return false;

    if(!expect_char(tc, '{', genConfig, genFunc))
        return false;

    if(!handle_class_var_dec(tc, genConfig, genFunc))
        return false;

    if(!handle_subroutine_dec(tc, genConfig, genFunc))
        return false;

    if(!expect_char(tc, '}', genConfig, genFunc))
        return false;

    return true;
}


void parse_and_generate_output(TokenizerConfig* tc, GenConfig* genConfig, GenFuncPtr(genFunc)) {
    // create output file corresponding to each input file, one at a time
    // file should begin with a class, and each file has only one class
    genFunc("class", BEGIN, genConfig);

    if(!handle_class(tc, genConfig, genFunc)) {
        // error during class handling
        printf("Error handling class with current token:%s; token type = %d\n", get_raw_token(tc), get_token_type(tc));
    }

    genFunc("class", END, genConfig);
}
