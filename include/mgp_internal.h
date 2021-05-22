#include "mini_grammar_parse.h"

#ifndef MGP_INTERNAL_H
#define MGP_INTERNAL_H

#include <unistd.h>
#include <string.h>

#define MGP_SUCCESS 1
#define MGP_ERROR 0

#define MGP_TRUE 1
#define MGP_FALSE 0

# define MAX_TEMPLATE_PART_LEN 4 + 1

t_lexeme			*mgp_lexeme_find_by_token(t_lexeme *lexemes, const char *token);
t_lexeme			*mgp_lexeme_find_by_template(t_lexeme *lexemes, const char *template);

int					mgp_error(const char **error_strings);

size_t				mgp_strlen(const char *str);
char				*mgp_strcpy(char *dst, const char *src);
int					mgp_strcmp(const char *s1, const char *s2);
size_t				mgp_strlcpy(char *dest, const char *source, size_t size);
int					mgp_get_next_template_part(const char **template, char *template_part);

#endif
