#include "mgp_internal.h"

size_t		mgp_strlen(const char *str)
{
	size_t a;

	a = 0;
	while (str[a] != '\0')
		a++;
	return (a);
}

char		*mgp_strcpy(char *dst, const char *src)
{
	size_t	i;

	i = 0;
	while (src[i] != '\0')
	{
		dst[i] = src[i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

int			mgp_strcmp(const char *s1, const char *s2)
{
	if (s1 == s2)
		return (0);
	while (*s1 == *s2 && *s1 && *s2)
	{
		s1++;
		s2++;
	}
	return ((unsigned char)*s1 - (unsigned char)*s2);
}

size_t		mgp_strlcpy(char *dest, const char *source, size_t size)
{
	size_t a;
	size_t r;

	a = 0;
	r = 0;
	if (!dest || !source)
		return (0);
	while (source[r] != '\0')
		++r;
	if (size == 0)
		return (r);
	while (a < size - 1 && source[a] != '\0')
	{
		((unsigned char *)dest)[a] = ((unsigned char *)source)[a];
		a++;
	}
	dest[a] = '\0';
	return (r);
}

int			mgp_get_next_template_part(const char **template, char *template_part)
{
	int	i;

	if (!*template || !**template)
		return (MGP_ERROR);
	if (**template != '{')
		return (MGP_ERROR);
	++*template;
	i = 0;
	while (i < MAX_TEMPLATE_PART_LEN && **template)
	{
		if (**template == '}')
		{
			++*template;
			return (MGP_SUCCESS);
		}
		if (**template == '{')
			return (MGP_ERROR);
		template_part[i] = **template;
		++*template;
		++i;
	}
	return (MGP_ERROR);
}
