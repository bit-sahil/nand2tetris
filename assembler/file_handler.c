#include<stdio.h>
#include<stdlib.h>
#include "cstring.h"
#include "parser.h"
#include "binary.h"
#include "map.h"
#include "symbol_table.h"
#include "inst_helper.h"
#include "compiler.h"


/* 
I think instead of trying to write most readable code, it might be good to also try most optimized code 
cos assemblers back in the day did not have luxury of large memories.
So instead of modular functionality/data processing pipeline type behavior, I could try to optimize wrt resources (memory and time)
But not everywhere, at some places I'd still relax under luxury of too many resources.

I would not, though, try to work without stdio library. Might get off libraries like stdbool and use simple integers (less optimized now!!)
But, assembly language I just worked with, for which we just made a CPU chip, memory chip and a working computer wouldn't have luxury of boolean bits,
so perhaps not too bad
*/


void get_hack_file_name(char* file_name, char* hack_file_name) {
    // replace .asm or .asmc with .hack and store value in hack_file_name
    
    copy_str(hack_file_name, file_name);
    int idx = find_substr(hack_file_name, ".asm");
    replace_substr_end(hack_file_name, ".hack", idx);
}


void populate_labels_from_file(char* file_name, struct Map* symbolTable) {
    /* This function opens a file, and process it's line one by one, ignoring comments and whitespaces
    Then we want line numbers (as addresses of given instruction), for jump instruction to jump to
    Whenever we find a label, we add it's jump address to symbol table to be used later in Jump instructions
    But we're behind the address where we need to jump by 1, so add 1 in current line number before adding label in symbol table
    */

    FILE *asmFile = fopen(file_name, "r"); //.asm or .asmc file
    
    char line[MAX_LINE_SIZE] = {0};
    char cleaned_line[MAX_LINE_SIZE] = {0};
    int current_line_number = -1;
    int iType = Empty;

    while(fgets(line, MAX_LINE_SIZE, asmFile)) {
        line_without_space(line, cleaned_line);
        // printf("ORIGINAL: %s; Cleaned: %s\n", line, cleaned_line);
        iType = instruction_type_with_line_number(cleaned_line, &current_line_number, 0);
        
        //printf("LJ: %d: %s : %d\n", current_line_number, cleaned_line, iType);
        if(iType == Label)
            add_label(symbolTable, cleaned_line, current_line_number+1);
    }
    fclose(asmFile);
}


void parse_and_add_to_file(char* inst, struct Map* symbolTable, FILE* hackFile) {
    char binary_str[17];

    parse_instruction(inst, binary_str, symbolTable);
    //printf("Instruction: %s ; Binary: %s \n", inst, binary_str);
    //printf("%s\n", binary_str);
    fputs(binary_str, hackFile);
    fputs("\n", hackFile);
}


void read_file_and_generate_machine_code(char* file_name, struct Map* symbolTable) {
    /* This function opens a file, and process it's line one by one, ignoring comments and whitespaces
    Then we want line numbers (as addresses of given instruction, for jump instruction to be functional, and that's what we do
    
    We've already assigned addresses to all symbols, so we use those from symbol table and generate binary machine code
    Machine code output is written in a newly created .hack file with same path and name as .asm file
    */

    FILE *asmFile = fopen(file_name, "r"); //.asm or .asmc file
    
    char hack_file_name[128];
    get_hack_file_name(file_name, hack_file_name);
    FILE *hackFile = fopen(hack_file_name, "w"); //.hack file created in same path as .asm file

    char line[MAX_LINE_SIZE] = {0};
    char cleaned_line[MAX_LINE_SIZE] = {0};
    int current_line_number = -1;
    int iType = 0;

    char binary_str[17]; //stores binary instruction in string format

    while(fgets(line, MAX_LINE_SIZE, asmFile)) {
        line_without_space(line, cleaned_line);
        // printf("ORIGINAL: %s", line);
        iType = instruction_type_with_line_number(cleaned_line, &current_line_number, 0);
        
        // printf("MC: %d: %s : %d\n", current_line_number, cleaned_line, iType);
        if(iType == Machine) {
            parse_and_add_to_file(cleaned_line, symbolTable, hackFile);
        }
        
    }
    fclose(asmFile);
    fclose(hackFile);
}


void read_file_and_process_instructions(char* file_name) {
    /* This function opens given file, and process it line by line, in 2 passes
    In 1st pass, we deal with only symbols and populate symbol table
    In 2nd pass, we've already assigned addresses to all symbols, so we use those from symbol table and generate binary machine code
    
    A separate function could very well return a list of instructions to be used in both passes,
    but that seems to assume more luxury of RAM space than I want to allow.
    So, I try to process files line by line.
    This would have to duplicate the effort required to read, clean, and number lines in both functions, and lead to some duplication. 
    But currently (I might be wrong), this effort seems lesser compared to having to store instructions.
    */

    struct Map* symbolTable = new_symbol_table();
    
    // 1st pass - populate symbol table
    populate_labels_from_file(file_name, symbolTable);

    // 2nd pass - generate machine code
    read_file_and_generate_machine_code(file_name, symbolTable);
}


void compile_and_process_file(char* file_name) {
    char c_file_name[128];
    get_c_file_name(file_name, c_file_name);

    generate_compiled_file(file_name, c_file_name);
    read_file_and_process_instructions(c_file_name);
}


int main(){
    char file_name[128];
    scanf("%s", file_name); //read input from prompt in terminal
    compile_and_process_file(file_name);
    return 0;
}

