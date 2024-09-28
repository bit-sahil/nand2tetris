#include<stdio.h>
#include<stdlib.h>
#include "parser.h"
#include "cstring.h"
#include "helper.h"


void handle_return(FILE* asmFile) {
    // form: return

    char* f_contents = get_f_contents("return");

    fprintf(asmFile, f_contents);

    free(f_contents);
}

