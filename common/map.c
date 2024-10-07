#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include"map.h"


/* Using map instead of symbol_table cos map is equally intuitive and less typing, 
avoids bulky names like new_symbol_table and struct SymbolTable
Also, I think reserving SymbolTable for map pre-populated with reserved keywords

*/


Map* init_map(int size) {
    // using key size = 256 chars

    return init_abstract_map(size, 256);
}


void _print_value(void* value) {
    printf("value=%s", (char*) value);
}


void print_map(Map* map) {
    print_abstract_map(map, _print_value);
}


void add_key(Map* map, char* key, char* value) {
    char* val = (char*) malloc(1+strlen(value));
    strcpy(val, value);

    // printf("key=%s;value=%s;val=%s\n", key, value, val);
    add_abstract_key(map, key, (void*) val);
}


int get_value(Map* map, char* key, char* value) {
    void *val = get_abstract_value(map, key);
    
    if(val == NULL)
        return 0;

    strcpy(value, (char*) val);
    return 1;
}
