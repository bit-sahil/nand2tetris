#include "file_reader.h"
#include "boolean.h"


ReaderConfig* init_code_reader(char* f_name) {
    // in code, we want to preserve spaces in between, but not trailing spaces (unless it's a language like python)
    // we also don't want comments

    return init_reader_config(f_name, False, True, True, True, 1024);
}

