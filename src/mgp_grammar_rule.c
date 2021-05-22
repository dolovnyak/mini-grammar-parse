#include "mgp_internal.h"

static int	add_possible_lexeme_to_grammar_rule(t_grammar_rule *grammar_rule, t_lexeme lexemes[MAX_LEXEMES_NUM], const char token[MAX_LEXEME_TOKEN_LEN])
{
	t_lexeme	*tmp_lexeme;

	if ((tmp_lexeme = mgp_lexeme_find_by_token(lexemes, token)) == NULL)
		return (mgp_error((const char *[]){"There is no token with name \"", token, "\"", NULL}));
	if (grammar_rule->possible_lexemes[grammar_rule->possible_lexemes_num].number >= MAX_LEXEMES_IN_POSSIBLE_LEXEME)
		return (mgp_error((const char *[]){"Overflow MAX_LEXEMES_IN_POSSIBLE_LEXEME in token \"", token, "\"", NULL}));

	grammar_rule->possible_lexemes[grammar_rule->possible_lexemes_num].lexemes[grammar_rule->possible_lexemes[grammar_rule->possible_lexemes_num].number] = tmp_lexeme;
	++grammar_rule->possible_lexemes[grammar_rule->possible_lexemes_num].number;
	return (MGP_SUCCESS);
}

static int	add_lexeme_to_grammar_rule(t_grammar_rule *grammar_rule,
				t_lexeme lexemes[MAX_LEXEMES_NUM], const char possible_lexemes_tokens[MAX_POSSIBLE_LEXEME_TOKEN_LEN])
{
	char		token[MAX_LEXEME_TOKEN_LEN];
	int			i;
	int			j;

	if (grammar_rule->possible_lexemes_num >= MAX_POSSIBLE_LEXEMES_IN_GRAMMAR_RULE)
		return (mgp_error((const char *[]){"Overflow MAX_POSSIBLE_LEXEMES_IN_GRAMMAR_RULE in token \"", possible_lexemes_tokens, "\"", NULL}));
	bzero(token, sizeof(token));
	i = 0;
	j = 0;
	while (possible_lexemes_tokens[i])
	{
		if (possible_lexemes_tokens[i] == '/')
		{
			if (!add_possible_lexeme_to_grammar_rule(grammar_rule, lexemes, token))
				return (mgp_error((const char *[]){"Add lexeme in token \"", token, "\"", NULL}));
			j = 0;
			++i;
			bzero(token, sizeof(token));
		}
		token[j] = possible_lexemes_tokens[i];
		i++;
		j++;
	}
	if (!add_possible_lexeme_to_grammar_rule(grammar_rule, lexemes, token))
		return (mgp_error((const char *[]){"Add lexeme in token \"", token, "\"", NULL}));
	++grammar_rule->possible_lexemes_num;
	return (MGP_SUCCESS);
}

static int	parse_token(t_lexeme lexemes[MAX_LEXEMES_NUM], const char *body, int *index, t_grammar_rule *grammar_rule)
{
	char		possible_lexemes_tokens[MAX_POSSIBLE_LEXEME_TOKEN_LEN];
	int			i;

	i = 0;
	bzero(possible_lexemes_tokens, sizeof(possible_lexemes_tokens));
	while (body[*index] && body[*index] != ' ')
	{
		if (i >= MAX_POSSIBLE_LEXEME_TOKEN_LEN)
			return (mgp_error((const char *[]){"Overflow MAX_POSSIBLE_LEXEME_TOKEN_LEN in token \"", possible_lexemes_tokens, "\"", NULL}));
		possible_lexemes_tokens[i] = body[*index];
		++i;
		++(*index);
	}
	if (!add_lexeme_to_grammar_rule(grammar_rule, lexemes, possible_lexemes_tokens))
		return (mgp_error((const char *[]){"Add lexeme to grammar rule in token \"", possible_lexemes_tokens, "\"", NULL}));
	return (MGP_SUCCESS);
}

static int	parse_body(t_lexeme lexemes[MAX_LEXEMES_NUM], const char *body, t_grammar_rule *grammar_rule)
{
	int	token_num;
	int	index;

	token_num = 0;
	index = 0;
	while (body[index])
	{
		if (body[index] == ' ')
		{
			++index;
			continue;
		}
		if (!parse_token(lexemes, body, &index, grammar_rule))
			return (mgp_error((const char *[]){"Parse token", NULL}));
		++token_num;
	}
	if (token_num == 0)
		return (mgp_error((const char *[]){"Empty body", NULL}));
	return (MGP_SUCCESS);
}

int mgp_grammar_rule_add(t_mgp *mgp, const char *body,
						int (*parse_func)(t_lex_data *lexemes_data, void *data))
{
	t_grammar_rule	tmp_grammar_rule;

	bzero(&tmp_grammar_rule, sizeof(tmp_grammar_rule));
	if (mgp->grammar_rules_num >= MAX_GRAMMAR_RULES_NUM)
		return (mgp_error((const char *[]){"Overflow MAX_GRAMMAR_RULES_NUM on rule: ", body, NULL}));
	if (mgp_strlen(body) >= MAX_GRAMMAR_RULE_LEN)
		return (mgp_error((const char *[]){"Overflow MAX_GRAMMAR_RULE_LEN on rule: ", body, NULL}));
	tmp_grammar_rule.parse_func = parse_func;
	if (!parse_body(mgp->lexemes, body, &tmp_grammar_rule))
		return (mgp_error((const char *[]){"Grammar rule body error: ", body, NULL}));
	mgp->grammar_rules[mgp->grammar_rules_num] = tmp_grammar_rule;
	++mgp->grammar_rules_num;
	return (MGP_SUCCESS);
}
