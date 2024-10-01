#include<stdio.h>
#include "boolean.h"
#include "file_handler.h"
#include "file_reader.h"
#include "word_delimit.h"


void test_file_reader(char* file_name) {
    ReaderConfig* rc = init_reader_config(file_name, False, True, True, True, 128);

    char* next_line;

    while((next_line=get_next_line(rc)) != NULL) {
        printf("$$$%s\n", next_line);
    }
} 


void test_file_handler(char* file_name) {
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
    char* str = " A test   str";

    Delimiter* dl = init_word_delimiter(str);

    char dest[16];
    while(next_word(dl, dest)) {
        printf("Next word:%s:\n", dest);
    }
}


int main(int argc, char* argv[]) {
    // vm file is provided as argv[1]

    char* file_name = argv[1];

    test_file_reader(file_name);
    test_file_handler(file_name);
    test_delim();
}

