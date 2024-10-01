#include "file_reader.h"
#include "boolean.h"


ReaderConfig* init_assembly_reader(char* f_name) {
    // in assembly, we don't want to preserve any spaces in between
    // we also don't want comments

    return init_reader_config(f_name, True, True, True, True, 1024);
}

