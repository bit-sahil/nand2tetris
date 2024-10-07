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
#include "lookahead_tokenizer.h"
#include "tokenizer_api.h"
#include "xml_writer.h"


void out_token(TokenizerConfig* tc, FILE* outfp) {
    // add token into xml file

    switch(get_token_type(tc)) {

        case SYMBOL:
            out_sym_str(get_raw_token(tc), "symbol", outfp);
            break;

        case KEYWORD:
            out_str(get_raw_token(tc), "keyword", outfp);
            break;

        case IDENTIFIER:
            out_str(get_raw_token(tc), "identifier", outfp);
            break;

        case INT_CONST:
            out_str(get_raw_token(tc), "integerConstant", outfp);
            break;

        case STRING_CONST:
            out_sym_str(get_raw_token(tc), "stringConstant", outfp);
            break;
    }
}


void generate_output(TokenizerConfig* tc, FILE* outfp) {
    // create output file corresponding to each input file

    fprintf(outfp, "<tokens>\n");

    int n=10;

    while(has_more_tokens(tc)) {
        advance_token(tc);
        // printf("word:%s\n", tc->word);
        // printf("token:%s;type=%d\n", get_raw_token(tc), get_token_type(tc));

        // if(advance_lookahead(tc, 2))
        //     printf("lookahead token:%s;type=%d\n", lookahead_raw_token(tc, 2), lookahead_token_type(tc, 2));
        
        out_token(tc, outfp);
    }

    fprintf(outfp, "</tokens>\n");
}


void handle_file_or_directory(char* file_name) {
    // TokenizerConfig* tc = init_tokenizer(file_name, "Tout.xml");
    TokenizerConfig* tc = init_lookahead_tokenizer(file_name, "Tout.xml", 7);

    FILE* outfp;

    while(adv_next_file(tc)) {
        outfp = get_out_file(tc->dc, tc->rc->file_name);

        printf("Generating output for file_name=%s\n", tc->rc->file_name);
        generate_output(tc, outfp);

        fclose(outfp);
    }

    dealloc_tokenizer(tc);
}


int main(int argc, char* argv[]) {
    // .jack file or directory name is provided as argv[1]

    char* file_name = argv[1];

    handle_file_or_directory(file_name);
}
