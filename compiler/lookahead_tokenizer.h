
#ifndef LOOKAHEAD_TOKENIZER_H
#define LOOKAHEAD_TOKENIZER_H


#include "tokenizer.h"


int is_la_tokenizer(TokenizerConfig* tc);
	// is tokenizer lookahead type?


int has_more_tokens_la(TokenizerConfig* tc);
	// has more tokens in lookahead


void advance_token_la(TokenizerConfig* tc);
    // advance lookahead token
    // means previous token value is consumed, and move curr pointer one step ahead, if present
	

TokenType get_token_type_la(TokenizerConfig* tc);
    // return current token type


char* get_raw_token_la(TokenizerConfig* tc);
    // returns raw token value as a string


int advance_lookahead(TokenizerConfig* tc, int k);
	// fill lookahead table mem by nktokens, if k < tc->n


TokenType lookahead_token_type(TokenizerConfig* tc, int k);
	/* lookahead k tokens, k=1 mean lookahead next token
	k=0th token is currently being pointed at

	Use this method only when advance lookahead by k is called
	*/


char* lookahead_raw_token(TokenizerConfig* tc, int k);
    /* lookahead k tokens, k=1 mean lookahead next token
	k=0th token is currently being pointed at

	Use this method only when advance lookahead by k is called
	*/

#endif
