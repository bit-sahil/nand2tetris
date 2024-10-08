#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<stdbool.h>
#include "../common/file_handler.h"
#include "../common/file_reader.h"
#include "../common/code_reader.h"
#include "../common/code_delimit.h"
#include "tokenizer.h"
#include "tokenizer_api.h"
#include "lookahead_tokenizer.h"
#include "xml_writer.h"
#include "parser.h"
#include "config.h"


void parse_file_or_directory(char* file_name) {
    ParserConfig* pc = init_parser(file_name, "out.xml", 2);

    TokenizerConfig* tc = pc->tc;

    while(adv_next_file(tc)) {
        FILE* outfp = get_out_file(tc->dc, tc->rc->file_name);
        GenConfig* genConfig = init_gen_config(outfp);

        printf("Generating output for file_name=%s\n", tc->rc->file_name);
        parse_and_generate_output(tc, genConfig, out_xml);

        dealloc_gen_config(genConfig);
        fclose(outfp);
    }

    dealloc_parser(pc);
}


int main(int argc, char* argv[]) {
    // .jack file or directory name is provided as argv[1]

    char* file_name = argv[1];

    parse_file_or_directory(file_name);
}
