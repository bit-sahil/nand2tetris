#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "config.h"
#include "symbol_table.h"


GenConfig* init_gen_config(FILE* outfp) {
	GenConfig* genConfig = (GenConfig*) malloc(sizeof(GenConfig));

	genConfig->outfp = outfp;
	
	genConfig->tableStack = NULL;

	genConfig->state = NULL;
	genConfig->expected = NULL;
	
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

	Kind kind;
	if(strcmp(genConfig->varKind, "static") == 0) {
		kind = Static;
	} else if(strcmp(genConfig->varKind, "field") == 0) {
		kind = Field;
	} else if(strcmp(genConfig->varKind, "local") == 0) {
		kind = Local;
	} else if(strcmp(genConfig->varKind, "argument") == 0) {
		kind = Argument;
	} else {
		printf("SymbolTableError: unhandled varKind=%s\n", genConfig->varKind);
		kind = Local;
	}

	add_key_symbol_table(genConfig->tableStack->table, varName, genConfig->varType, kind);
}


CodeStateStack* _push_state(char* codeState, CodeStateStack* prev) {
	CodeStateStack* state = (CodeStateStack*) malloc(sizeof(CodeStateStack));
	strcpy(state->val, codeState);
	state->prev = prev;
	return state;
}


CodeStateStack* _pop_state(CodeStateStack* state) {
	if(state == NULL) {
		printf("CodeStateStackError: popping from null state\n");
		return NULL;
	}

	CodeStateStack* prev = state->prev;
	free(state);
	return prev;
}


int is_non_empty(CodeStateStack* state) {
	return state != NULL;
}


char* _top_state(CodeStateStack* state) {
	// already checked state to be non-empty
	return state->val;
}


int _top_state_cmp(CodeStateStack* state, char* curr) {
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
