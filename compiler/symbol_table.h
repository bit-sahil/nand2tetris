
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "../common/abstract_map.h"


typedef enum {
	Argument,
	Local,
	Field,
	Static,
} Kind;


typedef struct SymbolTable {
	AbstractMap* map;
	int running_kind_num[4];
} SymbolTable;


typedef struct SymbolTableData {
	char type[128];
	Kind kind;
	int num;
} SymbolTableData;


SymbolTable* init_symbol_table(int size);

void dealloc_symbol_table(SymbolTable* symbol_table);

void print_symbol_table(SymbolTable* symbol_table);

void add_key_symbol_table(SymbolTable* symbol_table, char* name, char* type, Kind kind);

SymbolTableData* get_value_symbol_table(SymbolTable* symbol_table, char* key);

char* kind_to_str(Kind kind);

Kind str_to_kind(char* varKind);

#endif
