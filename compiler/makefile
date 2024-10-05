
tokenizer: tokenizer_main.c
	gcc tokenizer_main.c tokenizer_api.c tokenizer.c token_handler.c lookahead_tokenizer.c xml_writer.c ../common/{code_reader.c,delimiter.c,file_handler.c,file_reader.c,word_delimit.c,code_delimit.c}


parser: parser.c
	gcc parser.c tokenizer_api.c tokenizer.c token_handler.c lookahead_tokenizer.c xml_writer.c ../common/{code_reader.c,delimiter.c,file_handler.c,file_reader.c,word_delimit.c,code_delimit.c}

