#include "cstring.h"
#include "map.h"
#include "binary.h"


int curr_address = 16;


void add_numeric_value(struct Map* symbolTable, char* key, int num) {
    /* Add numeric value for specified key in map
    First numeric value is converted to binary
    Then we convert it to 16-bit binary string + 1 bit for '/0'
    Addresses are 15-bit instead of 16 bit, but those work just as well in 16-bit char
    
    todo: should probably test for key being already present in map 
    */
    
    char binary_str[17];
    int_to_binary_str(num, binary_str);
    
    add_key(symbolTable, key, binary_str);
}


void resolve_var(struct Map* symbolTable, char* var, char* binary_str) {
    /* resolve variable from symbol table
    Check for variable in symbol table and return address in binary_str
    If variable is not present, then allocate memory space in Ram, starting address 16
    and return address in binary_str
    */
    
    if(get_value(symbolTable, var, binary_str))
        return;
    
    extern int curr_address;
    add_numeric_value(symbolTable, var, curr_address);
    int_to_binary_str(curr_address, binary_str);
    curr_address++;
}


void add_label(struct Map* symbolTable, char* label, int address) {
    /* Add label = (Symbol) to symbol table
    We need to remove closing brackets from label for the key
    No changes with address (address refers to ROM address where instruction is stored)
    */

    char key[128];
    copy_str(key, &label[1]); // copy string after '('
    
    int i = 0;
    while(key[i] != ')')
        i++;
    
    key[i] = '\0';
    
    add_numeric_value(symbolTable, key, address);
}


void ram_key(int num, char* key) {
    // we already know that num ranges from 0-15 and simply populate key

    key[0] = 'R';
    if(num<10) {
        key[1] = '0' + num;
        key[2] = '\0';
    } else {
        key[1] = '1';
        key[2] = '0' + num%10;
        key[3] = '\0';
    }
}


void add_ram(struct Map* symbolTable) {
    // Add R0-R15 to symbol table

    char key[4];

    for(int i=0;i<16;i++) {
        ram_key(i, key);
        add_numeric_value(symbolTable, key, i);
    }
}


struct Map* new_symbol_table() {
    /* Allocate space for new table to be used as symbol table
    We pre-populate symbol table with pre-defined symbols
    Can make it dynamic to use optimum space, but starting with static 1024 elements in map
    */

    struct Map* symbolTable = new_map(1024);
    
    // special symbols
    add_numeric_value(symbolTable, "SP", 0);
    add_numeric_value(symbolTable, "LCL", 1);
    add_numeric_value(symbolTable, "ARG", 2);
    add_numeric_value(symbolTable, "THIS", 3);
    add_numeric_value(symbolTable, "THAT", 4);
    // add_numeric_value(symbolTable, "SPC", 16383); // Initializing Stack Pointer Custom would be ideal, but then cpu emulator is not going to function properly

    // R0-R15
    add_ram(symbolTable);

    // Screen and Keyboard
    add_numeric_value(symbolTable, "SCREEN", 16384);
    add_numeric_value(symbolTable, "KBD", 24576);

    return symbolTable;
}
