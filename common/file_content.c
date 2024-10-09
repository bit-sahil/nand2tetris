#include<stdio.h>
#include<string.h>
#include<stdlib.h>


char* file_to_str(char* file_name, int line_size, int max_lines) {
    // open file and store it's contents into f_contents string
    // assume f_contents has sufficient space and there is no overflow

    FILE* fp = fopen(file_name, "r");
    
    char* f_contents = (char*) malloc(line_size*max_lines*sizeof(char));
    f_contents[0] = '\0'; // start with an empty string

    char line[line_size];

    while(fgets(line, line_size, fp)) {
        strcat(f_contents, line);
    }

    fclose(fp);

    return f_contents;
}


void op_file_name(char* command, char* folder_name, char* file_ext, char* file_name) {
    // file_name = folder_name/command.file_ext
    // '/' and '.' are part of folder_name and file_ext already

    strcpy(file_name, folder_name);
    strcat(file_name, command);
    strcat(file_name, file_ext);
}
