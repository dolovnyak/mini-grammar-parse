#ifndef MINI_GRAMMAR_PARSE_H
# define MINI_GRAMMAR_PARSE_H

# define MAX_LEXEME_TOKEN_LEN 64 + 1
# define MAX_TEMPLATE_LEN 128 + 1
# define MAX_LEXEMES_NUM 128
# define MAX_LEXEME_STRING 256

# define MAX_GRAMMAR_RULE_LEN 1024 + 1
# define MAX_POSSIBLE_LEXEMES_IN_GRAMMAR_RULE 32
# define MAX_LEXEMES_IN_POSSIBLE_LEXEME 5
# define MAX_GRAMMAR_RULES_NUM 32
# define MAX_POSSIBLE_LEXEME_TOKEN_LEN MAX_LEXEME_TOKEN_LEN * MAX_LEXEMES_IN_POSSIBLE_LEXEME + MAX_LEXEMES_IN_POSSIBLE_LEXEME + 1

/**
 * You should change enum e_lex_data_type and union u_lex_data if you want to add new types
 */

typedef enum				e_lex_data_type
{
	None = 0,
	IntegerNum,
	FloatNum,
	DoubleNum
}							t_lex_data_type;

typedef union				u_lex_raw_data
{
	int						num_int;
	float					num_float;
	double					num_double;
}							t_lex_raw_data;

typedef struct				s_lex_data
{
	t_lex_data_type			data_type;
	t_lex_raw_data			raw_data;
}							t_lex_data;

typedef struct				s_lexeme
{
	char					token[MAX_LEXEME_TOKEN_LEN];
	char					template[MAX_TEMPLATE_LEN];
	int						(*parse_func)(const char *body, t_lex_data *data);
}							t_lexeme;

typedef struct				s_possible_lexeme
{
	t_lexeme				*lexemes[MAX_LEXEMES_IN_POSSIBLE_LEXEME];
	int						number;
}							t_possible_lexeme;

typedef struct				s_grammar_rule
{
	t_possible_lexeme 		possible_lexemes[MAX_POSSIBLE_LEXEMES_IN_GRAMMAR_RULE];
	int						possible_lexemes_num;
	int						(*parse_func)(t_lex_data lexemes_data[MAX_POSSIBLE_LEXEMES_IN_GRAMMAR_RULE], void *data);
}							t_grammar_rule;

typedef struct				s_mgp
{
	t_lexeme				lexemes[MAX_LEXEMES_NUM];
	t_grammar_rule			grammar_rules[MAX_GRAMMAR_RULES_NUM];
	int						lexemes_num;
	int						grammar_rules_num;
}							t_mgp;

t_mgp						mgp_init();
int							mgp_lexeme_add(t_mgp *mgp, const char *template,
		const char *token, int (*parse_func)(const char *body, t_lex_data *data));
int							mgp_grammar_rule_add(t_mgp *mgp, const char *body,
		int (*parse_func)(t_lex_data *lexemes_data, void *data));
int							mgp_parse(const t_mgp *mgp, const char *line, void *data);

#endif
