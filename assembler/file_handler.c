#include<stdio.h>
#include<stdlib.h>
#include "cstring.h"
#include "parser.h"
#include "binary.h"
#include "map.h"
#include "symbol_table.h"


/* 
I think instead of trying to write most readable code, it might be good to also try most optimized code 
cos assemblers back in the day did not have luxury of large memories.
So instead of modular functionality/data processing pipeline type behavior, I could try to optimize wrt resources (memory and time)
But not everywhere, at some places I'd still relax under luxury of too many resources.

I would not, though, try to work without stdio library. Might get off libraries like stdbool and use simple integers (less optimized now!!)
But, assembly language I just worked with, for which we just made a CPU chip, memory chip and a working computer wouldn't have luxury of boolean bits,
so perhaps not too bad
*/


#define MAX_LINE_SIZE 128
#define MAX_LINES_HANDLED 64
#define Empty 0
#define Label 1
#define Machine 2
#define Macro 3


int instruction_type_with_line_number(char* line, int* current_line_number, int print) {
    // print line with line number, ignoring empty lines if print=1, print=0 for skip printing
    // increments line number for non-empty lines excluding Label symbols
    // returns whether line is empty, label, or normal machine instruction

    if(line[0] == '\0') {
        //empty line
        return Empty;
    }
    
    if(line[0] == '(')
        return Label;


    if(line[0] == '$') {
        //this is a macro
        return Macro;
    }

    *current_line_number = *current_line_number + 1;

    if(print) {
        printf("%d: ", *current_line_number);
        print_str(line);
        printf("\n");
    }

    return Machine;
}


void line_without_space(char* line, char* cleaned_line) {
    /* Input a line from assembly program, and remove all whitespaces, comments, and tab
    tab is converted to whitespace, wherever delimiter is required
    Converted line is stored in cleaned_line variable
    */

    int idx = 0;
    char curr_char;
    
    for(int i=0;i<MAX_LINE_SIZE;i++) {
        curr_char = line[i];
        
        if((curr_char == '\n') || (curr_char == '\0') || (curr_char == '\r')) {
            // end of line
            // '\0' is perhaps not required for reading file, cos '\n' should always terminate all lines, except possibly last
            // added this to avoid unexpected failured in future
            break;

        } else if(curr_char == '/' && line[i+1] == '/') {
            // if a slash is encountered, and next char is also a slash, then it's a comment. Ignore rest of line
            break;

        } else if((curr_char == ' ') || (curr_char == '\t')) {
            // no space is required in assembly language
            continue;

        } else {
            // else it's a character relevant to assembly language
            // printf("Relevant char: %c | %x \n", curr_char, curr_char);
            cleaned_line[idx] = curr_char;
            idx++;
        }
    }
    
    // mark end of string to avoid reading garbage
    cleaned_line[idx] = '\0';
}


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


void get_dir_path(char* file_name, char* dir_path) {
    // find last '/' in file_name and remove off everything after it
    
    copy_str(dir_path, file_name);

    int idx = -1, last_idx = 0;
    while(1) {
        idx = search_char(&dir_path[last_idx], '/');
        // printf("%d, file_name: %s\n", idx, file_name);
        if(idx == -1) break;
        last_idx = idx;
    }

    if(last_idx == 0) {
        // not found
        last_idx = -1;  // start from 0th index
    }

    // string from 0 to last_idx is the substring
    // terminate string after last '/' index
    replace_substr_end(dir_path, "\0", last_idx+1);
}
    

void get_c_file_name(char* file_name, char* c_file_name) {
    // asmc file
    copy_str(c_file_name, file_name);
    replace_substr_end(c_file_name, "c", -1);
}


void handle_assign(char* macro, char* f_contents, char* intermediate_file) {
    // special handling of ASSIGN macro

    FILE *iFile = fopen(intermediate_file, "w");

    char statement[128] = {0};

    // assignment starts at index 6
    copy_str(statement, &macro[6]);

    // now we have statement of the form x=a
    // we parse x and a, where x will always be a variable, 
    // and b could be a variable or number

    char x[16], a[16];  // not handling long variable names

    int idx = find_substr(statement, "=");
    copy_str(x, statement);
    replace_substr_end(x, "", idx);  // x stores what's before '='

    copy_str(a, &statement[idx+1]);  // a stores everything after '='

    // numeric if a begins with a number
    int isNum = ((a[0]>='0') && (a[0]<='9'));

    if(isNum) {
        // it's a number we want to assign
        fprintf(iFile, f_contents, a, "A", x);
    } else {
        // read from another variables memory and assign the value
        fprintf(iFile, f_contents, a, "M", x);
    }   

    fclose(iFile);
}


void get_macro_file_name(char* macro, char* macro_file) {
    // return .m file path

    replace_substr_end(macro_file, "macros/", 0);
    replace_substr_end(macro_file, macro, -1);
    replace_substr_end(macro_file, ".m", -1);
}


void get_intermediate_file_name(char* macro, char* intermediate_file) {
    // return .intm file path
    // todo: make generic, hardcoding to handle ASSIGN
    replace_substr_end(intermediate_file, "macros/", 0);
    replace_substr_end(intermediate_file, "ASSIGN.intm", -1);
}


void file_to_str(char* file_name, char* f_contents) {
    // open file and store it's contents into f_contents string
    // assume f_contents has sufficient space and there is no overflow

    FILE* fp = fopen(file_name, "r");
    f_contents[0] = '\0'; // start with an empty string

    char line[MAX_LINE_SIZE] = {0};
    while(fgets(line, MAX_LINE_SIZE, fp)) {
        replace_substr_end(f_contents, line, -1);
    }

    fclose(fp);
}   


void generate_intermediate_file(char* macro, char* intermediate_file) {
    // generate intermediate file by substituting relevant %s placeholders from macro file and 

    char macro_file[128];
    char f_contents[MAX_LINE_SIZE*MAX_LINES_HANDLED];
    get_macro_file_name("ASSIGN", macro_file); // todo: make it generic

    file_to_str(macro_file, f_contents);

    // handle ASSIGN macro and update f_contents, that's the only macro we have to begin with
    // also adds updated contents after handling macro with substitution
    handle_assign(macro, f_contents, intermediate_file);
}


FILE* get_macro_file(char* macro) {
    // return pointer in read mode to macro file based on whether it's a simple macro file 
    // vs macro file with substitution
    // in case of macro with substitution, we generate an intermediate file to be read by replace_with_macro routine

    if(macro[0] != '$') {
        // simple macro
        char macro_file[128];
        get_macro_file_name(macro, macro_file);
        printf("w/o: %s; %s\n", macro, macro_file);
        return fopen(macro_file, "r");
    } else {
        // macro with substitution
        char intermediate_file[128];
        get_intermediate_file_name(macro, intermediate_file);
        printf("w/: %s; %s\n", macro, intermediate_file);
        generate_intermediate_file(&macro[1], intermediate_file);
        return fopen(intermediate_file, "r");
    }
}
    

void replace_with_macro(char* macro, FILE* cFile) {
    /* look for macro in macros/ directory
    copy instructions to cFile (after leaving a space maybe? -- not required it seems)
    if there's a macro present in file, then replace it with code of that macro (recursively call this function)
    
    To simplify, again we assume no cycles
    */

    FILE* macroFile = get_macro_file(macro);
    
    char line[MAX_LINE_SIZE] = {0};
    char cleaned_line[MAX_LINE_SIZE] = {0};
    int current_line_number = -1;
    int iType = 0;

    while(fgets(line, MAX_LINE_SIZE, macroFile)) {
        line_without_space(line, cleaned_line);
        // printf("ORIGINAL: %s; Cleaned: %s\n", line, cleaned_line);
        iType = instruction_type_with_line_number(cleaned_line, &current_line_number, 0);

        if (iType == Macro) {
            // replace recursively with macro instructions
            replace_with_macro(&cleaned_line[1], cFile);
            continue;
        }

        // write instruction in cFile as it is
        fputs(line, cFile);
    }

    fclose(macroFile);
}


void compile(char* file_name, FILE* cFile, char* dir_path, struct Map* fMap) {
    /* This function opens file_name and starts reading it's contents to write relevant instructions in cFile
    If there's an include statement, that means we need to add contents of included file first in cFile
    For that, we recursively call compile with new file that we need to deal with

    To avoid redundancy of code when multiple files has included some file, we keep a sumbol for each file in a map
    If map contains that symbol as a key, that means system already started reading that file
    Looks like we could just do well with a list, but value corresponding to key could later help in storing state
    To catch errors due to circular dependency, we'd like to know if a file just started getting processed and we 
    reached another file while including child files, or whether we're fully done compiling this file
    To begin with, we'd not handle circular dependencies and just work with whether key is present in map
    */

    fputs("\n", cFile);  // leaving a space for readability

    char* value = "1";  // just a random value
    if(get_value(fMap, file_name, value)) return;  // file already present in map

    add_key(fMap, file_name, value);  // add to map 
    // at this point, we've started including this file and assuming no circular dependencies, we should be done including
    // this file fully before another file try to include it again

    FILE* currFile = fopen(file_name, "r");
    
    int idx;
    char line[MAX_LINE_SIZE] = {0};

    char cleaned_line[MAX_LINE_SIZE] = {0};
    int current_line_number = -1;
    int iType = 0;

    while(fgets(line, MAX_LINE_SIZE, currFile)) {
        // printf("line: %s\n", line);

        idx = find_substr(line, "#include \"");
        if(idx==0) {
            // this is a #include statement
            // compare it with idx=0 since we're not planning to allow for additional spaces in this line
            // also, file name should begin at index 10
            char inc_file_name[MAX_LINE_SIZE];
            copy_str(inc_file_name, dir_path);  // starting with directory name
            replace_substr_end(inc_file_name, &line[10], -1); // append file name
            
            // printf("inc file: %s, dir path: %s\n", inc_file_name, dir_path);            
            idx = search_char(inc_file_name, '"');  // index of " where file_name should end
            replace_substr_end(inc_file_name, "\0", idx);
            compile(inc_file_name, cFile, dir_path, fMap);

            continue;  // don't add include line to file
        }

        line_without_space(line, cleaned_line);
        // printf("ORIGINAL: %s; Cleaned: %s\n", line, cleaned_line);
        iType = instruction_type_with_line_number(cleaned_line, &current_line_number, 0);

        if (iType == Macro) {
            // replace with instructions present in macro file
            replace_with_macro(&cleaned_line[1], cFile);
            continue;
        }

        // it's an instruction line to be copied over as it is
        fputs(line, cFile);
    }

    fputs("\n", cFile);  // leaving a space for readability
    fclose(currFile);
}
 

void generate_compiled_file(char* file_name, char* c_file_name) {
    /* I want to keep it as simple as possible to begin with, and would like to skip recursively listing files in a directory
    taking files from user seems like a good option, but then #include would not include same relative path
    so making some simplifying assumptions for now - all these files should be present in same folder
    we take a dir_path as param and look for files in that directory -- we can easily find directory name as such but let's just pass it for now
    It turns out, we don't need to list files in directory, we can just add concatenate file name with directory path and we're done    

    This function takes in .asm as main file and generate .asmc file (pretty random, but call c for compilation)
    Compilation is done from files inside dir_path directory and assume those are present
    And even for simplest of cases, this function has to include asm code recursively, cos other files could also be using some #include statement
    Also, we need to skip loading code multiple times to avoid conflicts, code duplication and accuracy, hence #ifndef and #define too
    We can let go of #endif and skip entire file to begin with
    Also have #include only at beginning of files
    */

    char dir_path[128];  // don't get into super long paths
    get_dir_path(file_name, dir_path);

    FILE *cFile = fopen(c_file_name, "w"); //.asmc file created in same path as .asm file
    struct Map* fMap = new_map(32);  // not expecting too many files to begin with

    compile(file_name, cFile, dir_path, fMap);

    fclose(cFile);
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

