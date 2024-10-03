#include<stdio.h>


void out_str(char* token, char* tokenType, FILE* outfp) {
        fprintf(outfp, "<%s> %s </%s>\n", tokenType, token, tokenType);
}


void out_sym_str(char* token, char* tokenType, FILE* outfp) {
    // output to file, also handling symbols for xm
    fprintf(outfp, "<%s> ", tokenType);

    char curr;
    for(int i=0; (token[i] != '\0'); i++) {

        switch(token[i]) {
            case '<':
                fprintf(outfp, "&lt;");
                break;
            case '>':
                fprintf(outfp, "&gt;");
                break;
            case '"':
                fprintf(outfp, "&quot;");
                break;
            case '&':
                fprintf(outfp, "&amp;");
                break;
            default:
                fprintf(outfp, "%c", token[i]);
        }
    }

    fprintf(outfp, " </%s>\n", tokenType);
}

