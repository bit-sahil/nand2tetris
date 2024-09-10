#include "map.h"


#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H


void add_numeric_value(struct Map* symbolTable, char* key, int num);

void resolve_var(struct Map* symbolTable, char* var, char* binary_str);

void add_label(struct Map* symbolTable, char* label, int address);

struct Map* new_symbol_table();

#endif
