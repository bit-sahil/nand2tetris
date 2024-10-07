#include<stdio.h>
#include<dirent.h>
#include "../common/file_handler.h"
#include "../common/code_reader.h"
#include "helper.h"
#include "writer.h"


/* This is entry point into virtual machine translator
Takes in a f.vm file or a directory f/ containing multiple vm files and outputs f.asm file post translation

*/


void process_file_and_translate(ReaderConfig* rc, FILE* asmFile) {
    /* This function uses reader config to read input file line by line, parse vm commands
    to figure out type of command, and based on that subtype and arg, if applicable

    Finally handle command and generate assembly code

    Generated code is written into asmFile
    */
    
    int line_num = 0;

    char fvar[128];
    get_file_variable_name(rc->file_name, fvar);

    char *cleaned_line;

    while((cleaned_line = get_next_line(rc)) != NULL) {
        line_num++;
        parse_and_generate_asm(cleaned_line, asmFile, line_num, rc->file_name, fvar);
    }
}


void handle_file_or_directory(char* file_name) {

    DirConfig* dc = init_dir_config(file_name, ".vm", ".asm");
    FILE* asmFile = get_out_file(dc, file_name);


    // add bootstrap code if it's a directory
    if(dc->is_dir)
        add_bootstrap_code(asmFile);

    char f_name[128];

    while(get_next_file(dc, f_name)) {
        printf("Generating output for f_name=%s\n", f_name);

        fprintf(asmFile, "//Processing file: %s\n", f_name);

        ReaderConfig* rc = init_code_reader(f_name);
        process_file_and_translate(rc, asmFile);
        dealloc_reader_config(rc);
    }

    // takes care of closing asmFile as well
    dealloc_dir_config(dc, asmFile);
}


int main(int argc, char* argv[]) {
    // vm file is provided as argv[1]

    char* file_name = argv[1];

    handle_file_or_directory(file_name);
}

