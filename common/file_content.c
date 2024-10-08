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
