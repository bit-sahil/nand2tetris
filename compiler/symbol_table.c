#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "../common/abstract_map.h"
#include "symbol_table.h"


SymbolTable* init_symbol_table(int size) {

    SymbolTable* symbol_table = (SymbolTable*) malloc(sizeof(SymbolTable));

    // using key size = 256 chars
    symbol_table->map = init_abstract_map(size, 256);

    // initialize running_kind_num array to 0
    for(int k = Argument; k <= Static; k++) {
        symbol_table->running_kind_num[k] = 0;
    }
    return symbol_table;
}


void dealloc_symbol_table(SymbolTable* symbol_table) {
    dealloc_map(symbol_table->map);
}


void print_kind_nums(SymbolTable* symbol_table) {
    printf("kind=num;");
    for(int k = Argument; k <= Static; k++) {
        printf("%d=%d;", k, symbol_table->running_kind_num[k]);
    }
    printf("\n");
}


void _print_value(void* value) {
    SymbolTableData* data = (SymbolTableData*) value;
    printf("type=%s, kind=%d, num=%d", data->type, data->kind, data->num);
}


void print_symbol_table(SymbolTable* symbol_table) {
    print_kind_nums(symbol_table);
    print_abstract_map(symbol_table->map, _print_value);
}


void _add_key(SymbolTable* symbol_table, char* key, SymbolTableData* data) {
    // printf("key=%s;value=%s;val=%s\n", key, data->type);
    add_abstract_key(symbol_table->map, key, (void*) data);
}


void add_key_symbol_table(SymbolTable* symbol_table, char* name, char* type, Kind kind) {
    SymbolTableData* data = (SymbolTableData*) malloc(sizeof(SymbolTableData));

    strcpy(data->type, type);
    data->kind = kind;
    data->num = symbol_table->running_kind_num[kind]++;

    // printf("key=%s;value=%s;val=%s\n", key, data->type);
    _add_key(symbol_table, name, data);
}


SymbolTableData* get_value_symbol_table(SymbolTable* symbol_table, char* key) {
    void *val = get_abstract_value(symbol_table->map, key);
    
    return (SymbolTableData*) val;
}
