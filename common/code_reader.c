#include "file_reader.h"
#include<stdbool.h>


ReaderConfig* init_code_reader(char* f_name) {
    // in code, we want to preserve spaces in between, but not trailing spaces (unless it's a language like python)
    // we also don't want comments

    return init_reader_config(f_name, false, true, true, true, 1024);
}

