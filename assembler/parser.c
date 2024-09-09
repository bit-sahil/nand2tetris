#include<stdio.h>
#include<stdlib.h>
#include "cstring.h"
#include "symbol_table.h"


void populate_alu_op(struct Map* ALU_OP) {
    if(ALU_OP->cnt > 0) return; //map is already populated
    
    // adding a=0 operations (Address register, Data register and constants)
    add_key(ALU_OP, "0",   "0101010");
    add_key(ALU_OP, "1",   "0111111");
    add_key(ALU_OP, "-1",  "0111010");
    add_key(ALU_OP, "D",   "0001100");
    add_key(ALU_OP, "A",   "0110000");
    add_key(ALU_OP, "!D",  "0001101");
    add_key(ALU_OP, "!A",  "0110001");
    add_key(ALU_OP, "-D",  "0001111");
    add_key(ALU_OP, "-A",  "0110011");
    add_key(ALU_OP, "D+1", "0011111");
    add_key(ALU_OP, "A+1", "0110111");
    add_key(ALU_OP, "D-1", "0001110");
    add_key(ALU_OP, "A-1", "0110010");
    add_key(ALU_OP, "D+A", "0000010");
    add_key(ALU_OP, "D-A", "0010011");
    add_key(ALU_OP, "A-D", "0000111");
    add_key(ALU_OP, "D&A", "0000000");
    add_key(ALU_OP, "D|A", "0010101");
    
    // a=1 operations (Memory and Data register)
    add_key(ALU_OP, "M",   "1110000");
    add_key(ALU_OP, "!M",  "1110001");
    add_key(ALU_OP, "-M",  "1110011");
    add_key(ALU_OP, "M+1", "1110111");
    add_key(ALU_OP, "M-1", "1110010");
    add_key(ALU_OP, "D+M", "1000010");
    add_key(ALU_OP, "D-M", "1010011");
    add_key(ALU_OP, "M-D", "1000111");
    add_key(ALU_OP, "D&M", "1000000");
    add_key(ALU_OP, "D|M", "1010101");
}


void get_op(char* inst, char* op) {
    // we need to find instruction between '=' and ';' and copy it in op

    int idx = search_char(inst, '=') + 1; //start after '='
    // search_char returns -1 if there is no equality, so we start correctly at index 0

    char c = inst[idx];
    int op_idx = 0;

    while(c!='\0' && c!=';') {
        // we stop either at end of string or ';' post which jump instruction begins
        op[op_idx++] = c;
        c = inst[++idx];
    }
    
    op[op_idx] = '\0';
}


void update_binary_instruction(char* bval, int* binary) {
    // bval is 7-character binary string (containing 0's and 1's)
    // This function convert these to int and store in corresponding binary array

    for(int i=0;i<7;i++)
        binary[12-i] = bval[i] - '0';
}


void parse_alu_operation(char* inst, int* binary) {
    /* c-instruction has structure of 1 1 1 a | c1 c2 c3 c4 | c5 c6 d1 d2 | d3 j1 j2 j3
    bits a, c1 ... c6 corresponds to binary[12], binary[11] ... binary[6]

    Here's the plan
    Make a map with alu operations as key, and string of 7 binary digits as value
    Find string between = and ;, and copy it in key
    Find value from map, and it should exist!
    Copy value in binary map - can do '1' - char to get integer -- should we convert everything in char right away? - let's not
    */
    
    for(int i=12; i>=6;i--) binary[i] = 0; //initialize
    
    struct Map* ALU_OP = new_map(28); //todo: make it global variable for efficient reuse
    // we have a total of 28 operations
    populate_alu_op(ALU_OP);
    
    char op[4]; // 3 char and one string termination
    get_op(inst, op);
    
    char bval[7]; // no string termination required
    get_value(ALU_OP, op, bval);

    update_binary_instruction(bval, binary);
}


void parse_storage_operation(char* inst, int* binary) {
    /* d1 d2 d3 corresponds to index 5, 4 and 3 respectively
    And in terms of storage location symbols, and their binary index altogether to avoid confusion 
    d1 = A = binary[5], 
    d2 = D = binary[4], 
    d3 = M = binary[3]

    Since storage is always LHS of equality relation, we search for '=' char and if it's found,
    parse for symbols upto it's index (excluding equal_idx) and update binary register
    This approach makes parsing less strict and allow for all permutations of A, D and M
    */

    binary[3] = binary[4] = binary[5] = 0;

    int equal_idx = search_char(inst, '=');

    for(int i=0;i<equal_idx;i++) {
        char out = inst[i];
        if(out == 'M') {
            binary[3] = 1;
        } else if(out == 'D') {
            binary[4] = 1;
        } else if(out == 'A') {
            binary[5] = 1;
        }
    }
}


void parse_jump_operation(char* inst, int* binary) {
    /* Jump operation uses j1, j2, and j3 bits which corresponds to output of ALU being
    less than, equal to, and greater than
    j1 = out < 0  = binary[2]
    j2 = out == 0 = binary[1]
    j1 = out > 0  = binary[0]
    
    ALU returns zr, and ng bits specifying is_zero and is_negative, which are combined with 
    j1, j2 and j3 instructions to figure out if either of these conditions is true

    In our assembly language, jump statement occurs after ';' and is mapped to 'J' followed by
    GT: greater than => binary[0]
    EQ: equal => binary[1]
    GE: greater than or equal => binary[0] | binary[1]
    LT: less than => binary[2]
    NE: not equal to (either > or <) => binary[0] | binary[2]
    LE: less than or equal to => binary[2] | binary[1]
    JMP: jump regardless of value => binary[0] | binary[1] | binary[2]
    */

    binary[0] = binary[1] = binary[2] = 0;

    int semicolon_idx = search_char(inst, ';');
    
    if(semicolon_idx == -1) 
        return;
    
    char* addr = &inst[semicolon_idx+1];

    if(compare_str(addr, "JGT")) {
        binary[0] = 1;
    } else if(compare_str(addr, "JEQ")) {
        binary[1] = 1;
    } else if(compare_str(addr, "JGE")) {
        binary[0] = binary[1] = 1;
    } else if(compare_str(addr, "JLT")) {
        binary[2] = 1;
    } else if(compare_str(addr, "JNE")) {
        binary[0] = binary[2] = 1;
    } else if(compare_str(addr, "JLE")) {
        binary[1] = binary[2] = 1;
    } else if(compare_str(addr, "JGT")) {
        binary[0] = binary[1] = binary[2] = 1;
    }
}


void parse_c_instruction(char* inst, int* binary) {
    /* c-instruction has structure of 1 1 1 a | c1 c2 c3 c4 | c5 c6 d1 d2 | d3 j1 j2 j3
    a ... c6 are bits used in ALU, and defines computation on values stored in A, D and M register
    d1, d2, d3 specify where result of computation has to be stored, if any
    j1, j2, j3 control jump operations
    */
    
    binary[15] = binary[14] = binary[13] = 1;
    
    parse_alu_operation(inst, binary);
    parse_storage_operation(inst, binary);
    parse_jump_operation(inst, binary);
}


void int_to_binary(int num, int* binary) {
    for(int i=0;i<16;i++) {
        binary[i] = num%2;
        num = num/2;
    }
}


void print_binary(int* binary) {
    for(int i=15;i>=0;i--) {
        printf("%d", binary[i]);
    }
    printf("\n");
}


void parse_a_instruction(char* inst, int* binary) {
    // parsing a-instruction of type @address
    // address is a numeric string, so first converting it to int, and then integer to 16-bit binary
    // 16-bit works as it is because in a-instruction, first binary digit is 0

    int number = atoi(&inst[1]);
    int_to_binary(number, binary);
}


void parse_instruction(char* inst, int inst_number) {
    int binary[16]; // cast it in char later

    if(inst[0] == '@')
        parse_a_instruction(inst, binary);
    else
        parse_c_instruction(inst, binary);
    
    printf("%s: ", inst);
    print_binary(binary);
}


int main(){
    parse_instruction("@7", 0);
    parse_instruction("DA=D+1", 1);
    parse_instruction("M;JGE", 2);
    return 0;
}

