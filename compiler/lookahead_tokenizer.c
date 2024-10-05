#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "../common/boolean.h"
#include "../common/file_handler.h"
#include "../common/file_reader.h"
#include "../common/code_reader.h"
#include "../common/code_delimit.h"
#include "tokenizer.h"


int is_la_tokenizer(TokenizerConfig* tc) {
	return tc->n != 0;
}


int add_lookahead(TokenizerConfig* tc) {
	// adds a value from token to lookahead
	// return False if all tokens are consumed

	if(!has_more_tokens_nla(tc))
		return False;

	advance_token_nla(tc);

	// printf("Adding lookahead: n=%d;token=%s;type=%d;curr=%d;idx=%d\n", tc->n, get_raw_token_nla(tc), get_token_type_nla(tc), tc->curr, tc->idx);

	strcpy((tc->mem + (tc->idx % tc->n)*WORD_SIZE), get_raw_token_nla(tc));
	tc->ttype[tc->idx % tc->n] =  get_token_type_nla(tc);

	tc->idx = ( tc->idx + 1 );
	return True;
}


int advance_lookahead(TokenizerConfig* tc, int k) {
	// fill lookahead table mem by nktokens, if k < tc->n
	
	if(k > tc->n) {
		printf("LookaheadError: Received %d tokens to lookahead with capacity %d\n", k, tc->n);
		return False;
	}

	while(tc->idx - tc->curr - 1 < k) {
		if(!add_lookahead(tc)) {
			printf("LookaheadError: Not enough tokens\n");
			return False;
		}
	}

	return True;
}


TokenType lookahead_token_type(TokenizerConfig* tc, int k) {
	/* lookahead k tokens, k=1 mean lookahead next token
	k=0th token is currently being pointed at

	Use this method only when advance lookahead by k is called
	*/

	if(k > tc->n) {
		printf("LookaheadError: Received %d tokens to lookahead with capacity %d\n", k, tc->n);
	}

	// if advanced already by k
	int curr = (tc->curr + k) % tc->n;
	return tc->ttype[curr];
}


const char* lookahead_raw_token(TokenizerConfig* tc, int k) {
    /* lookahead k tokens, k=1 mean lookahead next token
	k=0th token is currently being pointed at

	Use this method only when advance lookahead by k is called
	*/

	if(k > tc->n) {
		printf("LookaheadError: Received %d tokens to lookahead with capacity %d\n", k, tc->n);
	}

	// if advanced already by k
	int curr = (tc->curr + k) % tc->n;

	// printf("lookahead_raw_token curr=%d; token=%s\n", curr, (tc->mem + curr*WORD_SIZE));

	return tc->mem + curr*WORD_SIZE;
}


int has_more_tokens_la(TokenizerConfig* tc) {
	// has more tokens in lookahead

	if( (tc->curr + 1) < tc->idx)
		return True;

	return add_lookahead(tc);
}


void advance_token_la(TokenizerConfig* tc) {
    // advance lookahead token
    // means previous token value is consumed, and move curr pointer one step ahead, if present

    tc->curr = tc->curr + 1;
	
	if(tc->curr == tc->idx)
		// add lookahead if not already present
		add_lookahead(tc);
}


TokenType get_token_type_la(TokenizerConfig* tc){
    // return current token type

    return tc->ttype[tc->curr % tc->n];
}


const char* get_raw_token_la(TokenizerConfig* tc){
    // returns raw token value as a string

	return tc->mem + (tc->curr % tc->n)*WORD_SIZE;
}
