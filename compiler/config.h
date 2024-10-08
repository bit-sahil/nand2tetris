
#ifndef CONFIG_H
#define CONFIG_H

#include "symbol_table.h"

#define GenFuncPtr(F) void (*F) (char*, int, GenConfig*)


#define MAX_TOKEN_LEN 128


typedef struct CodeStateStack {
	char val[MAX_TOKEN_LEN];
	struct CodeStateStack* prev;
} CodeStateStack;


typedef struct SymbolTableStack {
	SymbolTable* table;
	struct SymbolTableStack* prev;
} SymbolTableStack;


typedef struct GenConfig {
	FILE* outfp;
	SymbolTableStack* tableStack;

	CodeStateStack* state;
	CodeStateStack* expected;

	char className[MAX_TOKEN_LEN];
	char varKind[MAX_TOKEN_LEN];
	char varType[MAX_TOKEN_LEN];
} GenConfig;


GenConfig* init_gen_config(FILE* outfp);

void dealloc_gen_config(GenConfig* genConfig);


void push_new_symbol_table(GenConfig* genConfig);

void pop_symbol_table(GenConfig* genConfig);

void add_var_symbol_table(GenConfig* genConfig, char* varName);


int is_non_empty(CodeStateStack* state);


void push_state(GenConfig* genConfig, char* codeState);

char* top_state(GenConfig* genConfig);

int top_state_cmp(GenConfig* genConfig, char* curr);

void pop_state(GenConfig* genConfig);


void push_expected(GenConfig* genConfig, char* codeState);

char* top_expected(GenConfig* genConfig);

int top_expected_cmp(GenConfig* genConfig, char* curr);

void pop_expected(GenConfig* genConfig);


#endif
