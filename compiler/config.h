
#ifndef CONFIG_H
#define CONFIG_H

#include "symbol_table.h"
#include "../common/map.h"


#define GenFuncPtr(F) void (*F) (char*, int, GenConfig*)

#define MAX_TOKEN_LEN 128


typedef struct StrValStack {
	char val[MAX_TOKEN_LEN];
	struct StrValStack* prev;
} StrValStack;


typedef struct SymbolTableStack {
	SymbolTable* table;
	struct SymbolTableStack* prev;
} SymbolTableStack;


typedef struct GenConfig {
	FILE* outfp;
	SymbolTableStack* tableStack;

	StrValStack* state;
	StrValStack* expected;

	Map* context;
	int nArg;
	int nLabel;
	StrValStack* curr_label;
	StrValStack* term_op;
} GenConfig;


GenConfig* init_gen_config(FILE* outfp);

void dealloc_gen_config(GenConfig* genConfig);


void push_new_symbol_table(GenConfig* genConfig);

void pop_symbol_table(GenConfig* genConfig);

void add_var_symbol_table(GenConfig* genConfig, char* varName);

int symbol_table_size(GenConfig* genConfig);

int class_symbol_table_size(GenConfig* genConfig);

int has_value_symbol_table(GenConfig* genConfig, char* key);

char* var_type_symbol_table(GenConfig* genConfig, char* key);

void vm_var_name_symbol_table(GenConfig* genConfig, char* key, char* val);

int symbol_table_num_var_type(GenConfig* genConfig, Kind kind);


void store_context(GenConfig* genConfig, char* key, char* value);

char* context_value(GenConfig* genConfig, char* key);


int is_non_empty(StrValStack* state);

void push_state(GenConfig* genConfig, char* codeState);

char* top_state(GenConfig* genConfig);

int top_state_cmp(GenConfig* genConfig, char* curr);

void pop_state(GenConfig* genConfig);


void push_expected(GenConfig* genConfig, char* codeState);

char* top_expected(GenConfig* genConfig);

int top_expected_cmp(GenConfig* genConfig, char* curr);

void pop_expected(GenConfig* genConfig);


int has_term_op(GenConfig* genConfig);

void push_term_op(GenConfig* genConfig, char* termOp);

char* top_term_op(GenConfig* genConfig);

int top_term_op_cmp(GenConfig* genConfig, char* curr);

void pop_term_op(GenConfig* genConfig);


int has_curr_label(GenConfig* genConfig);

void push_curr_label(GenConfig* genConfig, char* termOp);

char* top_curr_label(GenConfig* genConfig);

int top_curr_label_cmp(GenConfig* genConfig, char* curr);

void pop_curr_label(GenConfig* genConfig);


#endif
