
#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "../common/map.h"


void add_numeric_value(Map* symbolTable, char* key, int num);

void resolve_var(Map* symbolTable, char* var, char* binary_str);

void add_label(Map* symbolTable, char* label, int address);

Map* new_symbol_table();

#endif
