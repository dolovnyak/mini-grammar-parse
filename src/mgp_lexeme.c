#include "mgp_internal.h"

static int	template_usage_error(const char *template)
{
	return (mgp_error((const char *[]) {
		"Template \"", template, "\" is incorrect.\n",
		"Template could be a word with simple rules inside it: \n",
		"{'symbol'}, {'symbol'+}, {'symbol'-'symbol'}, {'symbol'-'symbol'+}\n",
		"For example template {N+}{o}{0-9+}{A}{a-c} - any word like NNNo72345Ac\n",
		"Also in construction A-Z second symbol (Z) should be >= first symbol (A)\n",
		"For example: constructions {Z-A} {3-0} aren't correct", NULL}));
}

static int	is_template_part_correct(const char template_part[MAX_TEMPLATE_PART_LEN])
{
	size_t	part_len;

	part_len = mgp_strlen(template_part);
	if (part_len < 1 || part_len > 4)
		return (MGP_ERROR);
	if (part_len == 2 && template_part[1] != '+')
		return (MGP_ERROR);
	if (part_len == 3 && template_part[1] != '-')
		return (MGP_ERROR);
	if (part_len == 4 && template_part[1] != '-' && template_part[3] != '+')
		return (MGP_ERROR);
	if ((part_len == 3 || part_len == 4) && (template_part[2] - template_part[0] < 0))
		return (MGP_ERROR);
	return (MGP_SUCCESS);
}

static int	is_template_correct(const char *template)
{
	char	template_part[MAX_TEMPLATE_PART_LEN];

	bzero(template_part, sizeof(template_part));
	while (mgp_get_next_template_part(&template, template_part))
	{
		if (!is_template_part_correct(template_part))
			return (MGP_ERROR);
		bzero(template_part, sizeof(template_part));
	}
	if (*template)
		return (MGP_ERROR);
	return (MGP_SUCCESS);
}

int			mgp_lexeme_add(t_mgp *mgp, const char *template, const char *token,
				int (*parse_func)(const char *body, t_lex_data *data))
{
	t_lexeme	tmp_lexeme;

	if (mgp->lexemes_num >= MAX_LEXEMES_NUM)
		return (mgp_error((const char *[]){"Overflow MAX_LEXEMES_NUM: ", template, NULL}));
	if (!template || !token)
		return (mgp_error((const char *[]){"Empty template or token", NULL}));
	if (mgp_strlen(template) > MAX_TEMPLATE_LEN)
		return (mgp_error((const char *[]){"Overflow MAX_TEMPLATE_LEN: ", template, NULL}));
	if (mgp_strlen(token) > MAX_LEXEME_TOKEN_LEN)
		return (mgp_error((const char *[]){"Overflow MAX_LEXEME_TOKEN_LEN: ", token, NULL}));
	if (mgp_lexeme_find_by_token(mgp->lexemes, token) != NULL)
		return (mgp_error((const char *[]){"Lexeme with token \"", token, "\" already exist", NULL}));
	if (!is_template_correct(template))
		return (template_usage_error(template));
	mgp_strcpy(tmp_lexeme.token, token);
	mgp_strcpy(tmp_lexeme.template, template);
	tmp_lexeme.parse_func = parse_func;
	mgp->lexemes[mgp->lexemes_num] = tmp_lexeme;
	++mgp->lexemes_num;
	return (MGP_SUCCESS);
}

t_lexeme	*mgp_lexeme_find_by_token(t_lexeme *lexemes, const char *token)
{
	int i = 0;

	while (i < MAX_LEXEMES_NUM)
	{
		if (mgp_strcmp(lexemes[i].token, token) == 0)
			return &(lexemes[i]);
		++i;
	}
	return (NULL);
}

t_lexeme	*mgp_lexeme_find_by_template(t_lexeme *lexemes, const char *template) //TODO maybe del
{
	int i = 0;

	while (i < MAX_LEXEMES_NUM)
	{
		if (mgp_strcmp(lexemes[i].template, template) == 0)
			return &(lexemes[i]);
		++i;
	}
	return (NULL);
}
