
#ifndef CODE_READER_H
#define CODE_READER_H

#include "file_reader.h"


ReaderConfig* init_code_reader(char* f_name);
    // in code, we want to preserve spaces in between, but not trailing spaces (unless it's a language like python)
    // we also don't want comments

#endif

