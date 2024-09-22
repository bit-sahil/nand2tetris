
#ifndef COMPILER_H
#define COMPILER_H


#define MAX_LINES_HANDLED 64


void get_dir_path(char* file_name, char* dir_path);
    // find last '/' in file_name and remove off everything after it
    

void get_c_file_name(char* file_name, char* c_file_name);
    // asmc file


void handle_assign(char* macro, char* f_contents, char* intermediate_file);
    // special handling of ASSIGN macro


void get_macro_file_name(char* macro, char* macro_file);
    // return .m file path


void get_intermediate_file_name(char* macro, char* intermediate_file);
    // return .intm file path
    // todo: make generic, hardcoding to handle ASSIGN


void file_to_str(char* file_name, char* f_contents);
    // open file and store it's contents into f_contents string
    // assume f_contents has sufficient space and there is no overflow


void generate_intermediate_file(char* macro, char* intermediate_file);
    // generate intermediate file by substituting relevant %s placeholders from macro file and 


FILE* get_macro_file(char* macro);
    // return pointer in read mode to macro file based on whether it's a simple macro file 
    // vs macro file with substitution
    // in case of macro with substitution, we generate an intermediate file to be read by replace_with_macro routine


void replace_with_macro(char* macro, FILE* cFile);
    /* look for macro in macros/ directory
    copy instructions to cFile (after leaving a space maybe? -- not required it seems)
    if there's a macro present in file, then replace it with code of that macro (recursively call this function)
    
    To simplify, again we assume no cycles
    */


void compile(char* file_name, FILE* cFile, char* dir_path, struct Map* fMap);
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

 

void generate_compiled_file(char* file_name, char* c_file_name);
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


#endif
