#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <assert.h>
#include <ctype.h>

#define ARENA_IMPLEMENTATION
#include "arena.h"
 
enum Token_kind {
  TOKEN_SYMBOL = 0,
  TOKEN_KEYWORD,
  TOKEN_NUM,
  TOKEN_SEP,
  TOKEN_OP,
};
typedef enum Token_kind Token_kind;

struct Token {
  Token_kind kind;
  const char* lexme;
};
typedef struct Token Token;

const char* token_str(Token_kind kind)
{
    switch (kind) {
        case TOKEN_SYMBOL:    return "SYMBOL";
        case TOKEN_KEYWORD:   return "KEYWORD";
        case TOKEN_NUM:       return "NUMBER";
        case TOKEN_SEP:       return "SEPERATOR";
        case TOKEN_OP:        return "OPERATOR";
    }
}
void trim_left(const char* src, size_t* pos)
{
	while (*pos < strlen(src) && isspace(src[*pos])) 
		(*pos)++;
}

char* split_str(Arena* arena, const char* src, size_t pos, size_t len)
{
	assert(pos >= 0 && (pos + len) <= strlen(src));
	char* dst = arena_reserve(arena, (len + 1) * sizeof(char));
	for(size_t i = pos; i < (pos + len); i++)
		dst[i - pos] = src[i];
	dst[len] = '\0';
	return dst;
}

Token* lexer(Arena* arena, const char* source, size_t* tcount)
{
	size_t current_pos = 0;
	size_t source_length = strlen(source);
	Token* tokens = arena_reserve(arena, source_length * sizeof(Token));
	size_t tokens_count = 0;
	
	while(current_pos < source_length) {
		trim_left(source, &current_pos);

		if(current_pos > source_length) 
			break;
		
		Token token;
		char ch = source[current_pos];
		size_t start = current_pos;
		if(isdigit(ch)) {
			while(current_pos < source_length && isdigit(source[current_pos]) || source[current_pos] == '.')
				current_pos++;
			token.kind = TOKEN_NUM;
		} else if(ispunct(ch)) {
			while (current_pos < source_length && ispunct(source[current_pos]))
				current_pos++;
			char* operators = "=<>*+-/";
			if(strchr(operators, ch) != NULL)
				token.kind = TOKEN_OP;
			else
				token.kind = TOKEN_SEP;
			
		} else if(isalpha(ch)) {
			while(current_pos < source_length && isalnum(source[current_pos]) || source[current_pos] == '_')
				current_pos++;
			token.kind = TOKEN_SYMBOL;
		}

		if(current_pos > source_length)
			break;
		token.lexme = split_str(arena, source, start, current_pos - start);
		if(token.kind == TOKEN_SYMBOL) {
			char* keywords[] = { 
				"while", "auto", "break", "case", "char", "const", "continue", "default", "do",
				"double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return",
				"short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile"
			};
			size_t kw_length = sizeof(keywords) / sizeof(keywords[0]); 
			for(int i = 0 ; i < kw_length; i++) {
				char* keyword = keywords[i];
				if(strcmp(token.lexme, keyword) == 0) {
					token.kind = TOKEN_KEYWORD; 
					break;
				} 
			}
		}

		tokens[tokens_count++] = token;
		(*tcount)++;
	}
	return tokens;
}

char* shift_args(int* argc, char*** argv)
{
	char* ret_str = (*(*argv)++); 
	(*argc)--;
	return ret_str;
}
int main(int argc, char** argv) {
	char* program = shift_args(&argc, &argv);
	if(argc <= 0) {
		fprintf(stderr, "ERROR: did not provide input file\n");
		fprintf(stderr, "Usage: %s <input.c file> <output.txt file>\n", program);
		return 1;
	}
	char* input_file_name = shift_args(&argc, &argv);
	if(argc <= 0) {
		fprintf(stderr, "ERROR: did not provide output file\n");
		fprintf(stderr, "Usage: %s <input.c file> <output.txt file>\n", program);
		return 1;
	}
	char* output_file_name = shift_args(&argc, &argv);
	Arena string_pool = arena_make();

    FILE* input_file = fopen(input_file_name, "r");
    if(input_file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", input_file_name);
        return 1;
    }
    fseek(input_file, 0, SEEK_END);
	long file_length = ftell(input_file);
	char* source_code = arena_reserve(&string_pool, (file_length + 1)*sizeof(char));
	fseek(input_file, 0, SEEK_SET);
	fread(source_code, file_length, 1, input_file);
	source_code[file_length] = '\0';
    fclose(input_file);
    
	size_t tokens_count = 0;
    Token* tokens = lexer(&string_pool, source_code, &tokens_count);

    FILE* output_file = fopen(output_file_name, "w");
    if(output_file == NULL) {
        fprintf(stderr, "Error: Could not open file %s\n", output_file_name);
        return 1;
    }

    fprintf(output_file, "+------------+---------------------+\n");
    fprintf(output_file, "|   Token    |       Lexeme        |\n");
    fprintf(output_file, "+------------+---------------------+\n");

    for (int i = 0; i < tokens_count; i++) {
		Token* t = &tokens[i];
        fprintf(output_file, "|  %-10s|       %-14s|\n", token_str(t->kind), t->lexme);
		fprintf(output_file, "+------------+---------------------+\n");
    }
    fclose(output_file);
	arena_free(&string_pool);
	printf("Generated %s from %s!\n", output_file_name, input_file_name);
    return 0;
}