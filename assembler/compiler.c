#include<stdio.h>
#include<stdlib.h>
#include "cstring.h"
#include "map.h"
#include "inst_helper.h"
#include "compiler.h"


#define Intermediate_T 0
#define Macro_T 1


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
    // $$ASSIGN,x=a

    FILE *iFile = fopen(intermediate_file, "w");

    char statement[128] = {0};

    // assignment starts at index 6
    copy_str(statement, &macro[7]);

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


void handle_push(char* macro, char* f_contents, char* intermediate_file) {
    // special handling of PUSH macro
    // $$PUSH,x

    FILE *iFile = fopen(intermediate_file, "w");

    char x[128] = {0};

    // everything after ',' in macro is x
    copy_str(x, &macro[5]);

    // x is only variable to be passed
    fprintf(iFile, f_contents, x);

    fclose(iFile);
}


void handle_goto(char* macro, char* f_contents, char* intermediate_file) {
    // special handling of GOTO macro
    // $$GOTO,x

    FILE *iFile = fopen(intermediate_file, "w");

    char x[128] = {0};

    // everything after ',' in macro is x
    copy_str(x, &macro[5]);

    // x is only variable to be passed
    fprintf(iFile, f_contents, x);

    fclose(iFile);
}


void handle_pop_c(char* macro, char* f_contents, char* intermediate_file) {
    // special handling of POP_C macro
    // $$POP_C,x

    FILE *iFile = fopen(intermediate_file, "w");

    char x[128] = {0};

    // everything after ',' in macro is x
    copy_str(x, &macro[6]);

    // x is only variable to be passed
    fprintf(iFile, f_contents, x);

    fclose(iFile);
}


void handle_call(char* macro, char* intermediate_file) {
    // special handling of CALL macro, slightly more special cos it's not simple string substitution in CALL.m macro file
    // form: CALL, retAddr, val1, val2, ..., funcAddr

    FILE *iFile = fopen(intermediate_file, "w");

    char x[128] = {0};
    char statement[128] = {0};

    int idx = 5;  //that's where first variable to put on stack starts
    int next_idx = search_char(&macro[idx], ',');  // search index of first ',' in substring

    // first term is retAddr
    char retAddr[128];
    copy_str_until(retAddr, &macro[idx], next_idx);

    while(next_idx != -1) {
        // this is a variable to be pushed on stack
        copy_str_until(x, &macro[idx], next_idx);

        // replace with PUSH macro in intermediate file
        replace_substr_end(statement, "$PUSH,", 0);  //statement="$PUSH,"
        replace_substr_end(statement, x, 6);  //statement="$PUSH,x"
        replace_with_macro(statement, iFile);  // write statements corresponding to macro in intermediate file

        idx = idx+next_idx+1;
        next_idx = search_char(&macro[idx], ',');  // search index of first ',' in substring
    }

    // we're at last term in CALL macro line, which is function name to be called
    // replace with GOTO macro in intermediate file
    replace_substr_end(statement, "$GOTO,", 0);  //statement="$GOTO,"
    replace_substr_end(statement, &macro[idx], 6);  //statement="$GOTO,x"
    //printf("handle_call: %s\n", statement);
    replace_with_macro(statement, iFile);  // write statements corresponding to macro in intermediate file

    // now add another line for return address
    //(retAddr)
    fprintf(iFile, "(%s)\n", retAddr);

    fclose(iFile);
}


void handle_func(char* macro, char* intermediate_file) {
    // special handling of FUNC macro, slightly more special cos it's not simple string substitution in CALL.m macro file
    // form: FUNC, fName, var1, var2, ...

    FILE *iFile = fopen(intermediate_file, "w");

    // treat as array of 16 char* each 64 bit long
    char vars[16*64] = {0};
    int i = 0, vars_idx=0;

    char statement[128] = {0};

    int idx = 5;  //that's where first variable to put on stack starts
    int next_idx = search_char(&macro[idx], ',');  // search index of first ',' in substring

    // first term is fName
    char fName[128];
    copy_str_until(fName, &macro[idx], next_idx);
    // add line for func address: (fName)
    fprintf(iFile, "(%s)\n", fName);

    // first pass to store all variables, if any, in vars array
    while(next_idx != -1) {
        idx = idx+next_idx+1;
        next_idx = search_char(&macro[idx], ',');  // search index of first ',' in substring

        // store i-th variable to be popped from stack and stored value into
        vars_idx = 64*i;
        copy_str_until(&vars[vars_idx], &macro[idx], next_idx);
        //printf("func:%d,  %s\n", i, &vars[vars_idx]);
        i++;
    }

    // second pass to pop vars in reverse order and assigning variable values in function from stack
    while(i>0) {
        i--;  // position of variable
        vars_idx = 64*i;

        // replace with POP_C macro in intermediate file
        replace_substr_end(statement, "$POP_C,", 0);  //statement="$POP_C,"
        replace_substr_end(statement, &vars[vars_idx], 7);  //statement="$POP_C,var"
        //printf("func: %d, %s\n", i, &vars[vars_idx]);
        replace_with_macro(statement, iFile);  // write statements corresponding to macro in intermediate file
    }

    fclose(iFile);
}


void handle_spcl_macro(char* macro, char* f_contents, char* intermediate_file) {
    // figure out which special macro type it is and handle it

    if(find_substr(macro, "ASSIGN,") != -1)
        handle_assign(macro, f_contents, intermediate_file);
    else if(find_substr(macro, "PUSH,") != -1)
        handle_push(macro, f_contents, intermediate_file);
    else if(find_substr(macro, "GOTO,") != -1)
        handle_goto(macro, f_contents, intermediate_file);
    else if(find_substr(macro, "POP_C,") != -1)
        handle_pop_c(macro, f_contents, intermediate_file);

}


void get_macro_file_name(char* macro, char* macro_file) {
    // return .m file path

    replace_substr_end(macro_file, "macros/", 0);
    replace_substr_end(macro_file, macro, -1);
    replace_substr_end(macro_file, ".m", -1);
}


void get_spcl_macro_file_name(char* macro, char* macro_file, int f_type) {
    // return .intm or .m file path based on f_type
    // handling special macro with $$, but passed to function without $
    //form: MACRO,...

    replace_substr_end(macro_file, "macros/", 0);
    replace_substr_end(macro_file, macro, -1);

    int idx = search_char(macro_file, ',');
    replace_substr_end(macro_file, "", idx); // remove everything post first ','

    if(f_type == Intermediate_T)
        replace_substr_end(macro_file, ".intm", -1);  // add .intm in the end
    else
        replace_substr_end(macro_file, ".m", -1);  // add .m in the end

    //printf("gsmfn: %s; %d; %s\n", macro, f_type, macro_file);
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
    // form: MACRO,...

    if(find_substr(macro, "CALL,") != -1) {
        // special handling of $$CALL (does not have a macro file of it's own)
        handle_call(macro, intermediate_file);
        return;
    } else if(find_substr(macro, "FUNC,") != -1) {
        // special handling of $$FUNC (does not have a macro file of it's own)
        handle_func(macro, intermediate_file);
        return;
    }

    char macro_file[128];
    char f_contents[MAX_LINE_SIZE*MAX_LINES_HANDLED];
    get_spcl_macro_file_name(macro, macro_file, Macro_T);

    file_to_str(macro_file, f_contents);

    // handle macro and update f_contents
    // also adds updated contents after handling macro with substitution
    handle_spcl_macro(macro, f_contents, intermediate_file);
}


FILE* get_macro_file(char* macro) {
    // return pointer in read mode to macro file based on whether it's a simple macro file 
    // vs macro file with substitution
    // in case of macro with substitution, we generate an intermediate file to be read by replace_with_macro routine

    if(macro[0] != '$') {
        // simple macro
        char macro_file[128];
        get_macro_file_name(macro, macro_file);
        //printf("w/o: %s; %s\n", macro, macro_file);
        return fopen(macro_file, "r");
    } else {
        // macro with substitution
        // form: $MACRO,...
        char intermediate_file[128];
        get_spcl_macro_file_name(&macro[1], intermediate_file, Intermediate_T);
        //printf("w/: %s; %s\n", macro, intermediate_file);
        generate_intermediate_file(&macro[1], intermediate_file);
        return fopen(intermediate_file, "r");
    }
}
    

void replace_with_macro(char* macro, FILE* cFile) {
    /* look for macro in macros/ directory
    copy instructions to cFile (after leaving a space maybe? -- not required it seems)
    if there's a macro present in file, then replace it with code of that macro (recursively call this function)
    
    To simplify, again we assume no cycles

    form: MACRO or $MACRO,...
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

