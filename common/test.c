#include<stdio.h>
#include<stdbool.h>
#include "file_handler.h"
#include "file_reader.h"
#include "word_delimit.h"
#include "code_delimit.h"


void test_file_reader(char* file_name) {
    printf("********* Testing File Reader **********\n");

    ReaderConfig* rc = init_reader_config(file_name, false, true, true, true, 128);

    char* next_line;

    while((next_line=get_next_line(rc)) != NULL) {
        printf("$$$%s\n", next_line);
    }
} 


void test_file_handler(char* file_name) {
    printf("********* Testing File Handler **********\n");

    DirConfig* dc = init_dir_config(file_name, ".h", ".tmp");

    char f_name[128];

    FILE* out_file = get_out_file(dc, file_name);
    // returns pointer to output file in "w" mode

    while(get_next_file(dc, f_name)) 
        printf("fName:%s\n", f_name);
    // in case of file, return file_name only once

    dealloc_dir_config(dc, out_file);
} 


void test_delim() {
    printf("********* Testing Line Delimiter **********\n");

    char* str = " A test   str";

    Delimiter* dl = init_word_delimiter(str);

    char dest[16];
    while(next_word(dl, dest)) {
        printf("Next word:%s:\n", dest);
    }
}


void test_code_delim() {
    printf("********* Testing Code Line Delimiter **********\n");

    char* str = "char* lms =&func(\"This is my string\n end string\", &p2);";

    Delimiter* dl = init_code_delimiter(str);
    int n=20;
    char dest[64];
    while(n-- && next_code_token(dl, dest)) {
        printf("Next code token:%s:\n", dest);
    }
}


int main(int argc, char* argv[]) {
    // vm file is provided as argv[1]

    char* file_name = argv[1];

    test_file_reader(file_name);
    test_file_handler(file_name);
    test_delim();
    test_code_delim();
}

