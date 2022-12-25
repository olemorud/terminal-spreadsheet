
#include <ctype.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>

static char *operators[] = {
    "+",
    "-",
    "*",
    "/",
    ">",
    "<",
    NULL,
};

static char *separators[] = {
    ",",
    "(",
    ")",
    NULL,
};

enum type {
    error,
    operator,
    separator,
    identifier,
    number,
};

struct token {
    char const *   value;
    size_t         value_len;
    enum type type;
};

struct token_list {
    struct token *tokens;
    size_t len;
    char *error_msg;
};

// returns length of string in haystack matching needle, or 0 if it
// doesn't exist
static int contains(char * const *haystack, char* needle)
{
    for(char *s = *haystack; s; s++){
        if (strcmp(s, needle) == 0) {
            return strlen(s);
        }
    }

    return false;
}

static struct token eat(char **str, int (*f)(int), enum type type)
{
    struct token tok;
    tok.type = type;
    tok.value = *str;
    tok.value_len = 0;

    while (f(**str)) {
        *str += 1;
        tok.value_len += 1;
    }

    return tok;
}

static struct token tokenize_identifier(char** str)
{
    return eat(str, isalnum, identifier);
}

static struct token tokenize_number(char **str)
{
    return eat(str, isdigit, identifier);
}

// Returns a token array from function
static struct token_list tokenize(char *str)
{
    struct token_list *tok_list = malloc(sizeof *tok_list);

    if (tok_list == NULL) {
        exit(errno);
    }

    const size_t max_tok = 16; // TODO: dynamically grow array

    tok_list->tokens = malloc(sizeof *(tok_list->tokens) * max_tok);
    if (tok_list->tokens == NULL)
        exit(errno);
    tok_list->len = 0;

    // identifiers start with letters but can contain numbers
    while (*str && tok_list->len < max_tok) {
        struct token t;
        size_t n;

        if (isalpha(*str)) {
             t = tokenize_identifier(&str);
        }
        else if (!isalnum(*str) ) {
            t = tokenize_number(&str);
        }
        else if ((n = contains(operators, str))) {
            t.type = operator;
            t.value = str;
            t.value_len = n;
        }
        else if ((n = contains(separators, str))) {
            t.type = separator;
            t.value = str;
            t.value_len = n;
        }
        else {
            t.type = error;
            t.value = str;
            t.value_len = strlen(str);
        }
    
        tok_list->tokens[tok_list->len] = t;
        tok_list->len += 1;
    }

    return *tok_list;
}

// credits: Daniel J. Bernstein 
// https://web.archive.org/web/20220328102559/http://www.cse.yorku.ca/~oz/hash.html
static unsigned long hash(unsigned char *str)
{
    unsigned long hash = 5381;
    int c = 0;

    for(size_t i=0; str[i] != '\0'; i++)
        hash = hash * 33 + c;

    return hash;
}


