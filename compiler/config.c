#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

#include "config.h"
#include "symbol_table.h"
#include "../common/map.h"


GenConfig* init_gen_config(FILE* outfp) {
	GenConfig* genConfig = (GenConfig*) malloc(sizeof(GenConfig));

	genConfig->outfp = outfp;
	
	genConfig->tableStack = NULL;

	genConfig->state = NULL;
	genConfig->expected = NULL;
	
	genConfig->term_op = NULL;
	genConfig->curr_label = NULL;

	genConfig->context = NULL;
	
	return genConfig;
}


void dealloc_gen_config(GenConfig* genConfig) {
	free(genConfig);
}


void push_new_symbol_table(GenConfig* genConfig) {
	SymbolTableStack* tableStack = (SymbolTableStack*) malloc(sizeof(SymbolTableStack));

	tableStack->table = init_symbol_table(256);
	tableStack->prev = genConfig->tableStack;

	genConfig->tableStack = tableStack;
}


void pop_symbol_table(GenConfig* genConfig) {
	if(genConfig->tableStack == NULL) {
		printf("SymbolTableStackError: popping from null state\n");
		return;
	}

	SymbolTableStack* prev = genConfig->tableStack->prev;
	
	dealloc_symbol_table(genConfig->tableStack->table);
	free(genConfig->tableStack);

	genConfig->tableStack = prev;
}


void add_var_symbol_table(GenConfig* genConfig, char* varName) {
	// printf("add_var_symbol_table, varName=%s; varKind=%s; varType=%s\n", varName, genConfig->varKind, genConfig->varType);

	char* varKind = context_value(genConfig, "varKind");

	add_key_symbol_table(genConfig->tableStack->table, varName, context_value(genConfig, "varType"), str_to_kind(varKind));
}


SymbolTableData* _get_value_symbol_tables(GenConfig* genConfig, char* key) {
	SymbolTableStack* tableStack = genConfig->tableStack;

	SymbolTableData* data;

	while(tableStack != NULL) {
		if( (data=get_value_symbol_table(tableStack->table, key)) != NULL)
			return data;

		tableStack = tableStack->prev;
	}

	return NULL;
}


int has_value_symbol_table(GenConfig* genConfig, char* key) {
	return _get_value_symbol_tables(genConfig, key) != NULL;
}


char* var_type_symbol_table(GenConfig* genConfig, char* key) {
	SymbolTableData* data = _get_value_symbol_tables(genConfig, key);

	return data->type;
}


void vm_var_name_symbol_table(GenConfig* genConfig, char* key, char* val) {
	SymbolTableData* data = _get_value_symbol_tables(genConfig, key);

	sprintf(val, "%s %d", kind_to_str(data->kind), data->num);
}


int symbol_table_size(GenConfig* genConfig) {
	// returns number of elements in top symbol table
	// usually that's the table for subroutine

	return genConfig->tableStack->table->map->cnt;
}


int symbol_table_num_var_type(GenConfig* genConfig, Kind kind) {
	// returns number of variables of given type in top symbol table
	return genConfig->tableStack->table->running_kind_num[kind];
}


void store_context(GenConfig* genConfig, char* key, char* value) {
	if(genConfig->context == NULL)
		printf("ContextError: context map not found\n");

	add_key(genConfig->context, key, value);
}


char* context_value(GenConfig* genConfig, char* key) {
	return get_str_value(genConfig->context, key);
}


StrValStack* _push_state(char* codeState, StrValStack* prev) {
	StrValStack* state = (StrValStack*) malloc(sizeof(StrValStack));
	strcpy(state->val, codeState);
	state->prev = prev;
	return state;
}


StrValStack* _pop_state(StrValStack* state) {
	if(state == NULL) {
		printf("StrValStackError: popping from null state\n");
		return NULL;
	}

	StrValStack* prev = state->prev;
	free(state);
	return prev;
}


int is_non_empty(StrValStack* state) {
	return state != NULL;
}


char* _top_state(StrValStack* state) {
	// already checked state to be non-empty
	return state->val;
}


int _top_state_cmp(StrValStack* state, char* curr) {
	if(!is_non_empty(state))
		return false;
	
	return strcmp(_top_state(state), curr) == 0;
}


void push_state(GenConfig* genConfig, char* codeState) {
	genConfig->state = _push_state(codeState, genConfig->state);
}


char* top_state(GenConfig* genConfig) {
	return _top_state(genConfig->state);
}


int top_state_cmp(GenConfig* genConfig, char* curr) {
	return _top_state_cmp(genConfig->state, curr);
}


void pop_state(GenConfig* genConfig) {
	genConfig->state = _pop_state(genConfig->state);
}


void push_expected(GenConfig* genConfig, char* codeState) {
	genConfig->expected = _push_state(codeState, genConfig->expected);
}


char* top_expected(GenConfig* genConfig) {
	return _top_state(genConfig->expected);
}


int top_expected_cmp(GenConfig* genConfig, char* curr) {
	return _top_state_cmp(genConfig->expected, curr);
}


void pop_expected(GenConfig* genConfig) {
	genConfig->expected = _pop_state(genConfig->expected);
}


int has_term_op(GenConfig* genConfig) {
	return is_non_empty(genConfig->term_op);
}


void push_term_op(GenConfig* genConfig, char* termOp) {
	genConfig->term_op = _push_state(termOp, genConfig->term_op);
}


char* top_term_op(GenConfig* genConfig) {
	return _top_state(genConfig->term_op);
}


int top_term_op_cmp(GenConfig* genConfig, char* curr) {
	return _top_state_cmp(genConfig->term_op, curr);
}


void pop_term_op(GenConfig* genConfig) {
	genConfig->term_op = _pop_state(genConfig->term_op);
}


int has_curr_label(GenConfig* genConfig) {
	return is_non_empty(genConfig->curr_label);
}


void push_curr_label(GenConfig* genConfig, char* termOp) {
	genConfig->curr_label = _push_state(termOp, genConfig->curr_label);
}


char* top_curr_label(GenConfig* genConfig) {
	return _top_state(genConfig->curr_label);
}


int top_curr_label_cmp(GenConfig* genConfig, char* curr) {
	return _top_state_cmp(genConfig->curr_label, curr);
}


void pop_curr_label(GenConfig* genConfig) {
	genConfig->curr_label = _pop_state(genConfig->curr_label);
}
