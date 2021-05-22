#include "mgp_internal.h"

int	template_part_matches_line(const char template_part[MAX_TEMPLATE_PART_LEN], const char *line, int *line_index)
{
	size_t part_len;

	part_len = mgp_strlen(template_part);
	if (part_len == 1)
	{
		if (line[*line_index] != template_part[0])
			return (MGP_ERROR);
		++*line_index;
		return (MGP_SUCCESS);
	}
	if (part_len == 2)
	{
		if (line[*line_index] != template_part[0])
			return (MGP_ERROR);
		++*line_index;
		while (line[*line_index] == template_part[0])
			++*line_index;
		return (MGP_SUCCESS);
	}
	if (part_len == 3)
	{
		if (!(line[*line_index] >= template_part[0] && line[*line_index] <= template_part[2]))
			return (MGP_ERROR);
		++*line_index;
		return (MGP_SUCCESS);
	}
	if (part_len == 4)
	{
		if (!(line[*line_index] >= template_part[0] && line[*line_index] <= template_part[2]))
			return (MGP_ERROR);
		++*line_index;
		while (line[*line_index] >= template_part[0] && line[*line_index] <= template_part[2])
			++*line_index;
		return (MGP_SUCCESS);
	}
	return (mgp_error((const char *[]){"Parse lib code error in template_part_matches_line, ",
							"cur line state \"", line, "\", cur template_part \"", template_part, "\"", NULL}));
}

int	template_matches_line(const char template[MAX_TEMPLATE_LEN], const char *line, int *line_index)
{
	char	template_part[MAX_TEMPLATE_PART_LEN];

	*line_index = 0;
	bzero(template_part, sizeof(template_part));
	while (mgp_get_next_template_part(&template, template_part))
	{
		if (!template_part_matches_line(template_part, line, line_index))
			return (MGP_ERROR);
		bzero(template_part, sizeof(template_part));
	}
	if (*template)
		return (mgp_error((const char *[]){"Parse lib code error in template_matches_line", NULL}));
	return (MGP_SUCCESS);
}

int	possible_lexeme_matches_line(const t_possible_lexeme *possible_lexeme, const char *line, int *line_index, t_lex_data *lex_data)
{
	int	i;
	char	lexeme_string[MAX_LEXEME_STRING];

	i = 0;
	bzero(lexeme_string, sizeof(lexeme_string));
	while (i < possible_lexeme->number)
	{
		if (template_matches_line(possible_lexeme->lexemes[i]->template, line, line_index))
		{
			if (*line_index >= MAX_LEXEME_STRING)
				return (mgp_error((const char *[]){"Overflow MAX_LEXEME_STRING, cur line processing \"", line, "\"", NULL}));
			if (possible_lexeme->lexemes[i]->parse_func != NULL)
			{
				mgp_strlcpy(lexeme_string, line, *line_index + 1);
				if (!possible_lexeme->lexemes[i]->parse_func(line, lex_data))
					return (mgp_error((const char *[]){"parse_func on LEXEME when parse lexeme \"", lexeme_string, "\"", NULL}));
			}
			return (MGP_SUCCESS);
		}
		++i;
	}
	return (MGP_ERROR);
}

int	grammar_rule_matches_line(const t_grammar_rule *grammar_rule, const char *line, t_lex_data lexemes_data[MAX_POSSIBLE_LEXEMES_IN_GRAMMAR_RULE])
{
	int		i;
	int		line_index;

	i = 0;
	line_index = 0;
	while (i < grammar_rule->possible_lexemes_num)
	{
		if (!possible_lexeme_matches_line(&grammar_rule->possible_lexemes[i], line, &line_index, &lexemes_data[i]))
			return (MGP_ERROR);
		line += line_index;
		++i;
	}
	if (*line)
		return (MGP_ERROR);
	return (MGP_SUCCESS);
}

int	mgp_parse(const t_mgp *mgp, const char *line, void *data)
{
	int			i;
	t_lex_data	lexemes_data[MAX_POSSIBLE_LEXEMES_IN_GRAMMAR_RULE];

	if (!line || !*line)
		return (mgp_error((const char *[]){"mgp_parse line is empty", NULL}));
	i = 0;
	while (i < mgp->grammar_rules_num)
	{
		bzero(lexemes_data, sizeof(lexemes_data));
		if (grammar_rule_matches_line(&mgp->grammar_rules[i], line, lexemes_data))
		{
			if (mgp->grammar_rules[i].parse_func != NULL)
				if (!mgp->grammar_rules[i].parse_func(lexemes_data, data))
					return (mgp_error((const char *[]){"parse_func on GRAMMAR_RULE when parse line \"", line, "\"", NULL}));
			return (MGP_SUCCESS);
		}
		++i;
	}
	return (mgp_error((const char *[]){"There is no rule or there is errors when parse line \"", line, "\"", NULL}));
}
