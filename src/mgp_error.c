#include "mgp_internal.h"

int	mgp_error(const char **error_strings)
{
	int i = 0;

	write(STDERR_FILENO, "MGP_ERROR: ", 11);
	while (error_strings[i])
	{
		write(STDERR_FILENO, error_strings[i], mgp_strlen(error_strings[i]));
		++i;
	}
	write(STDERR_FILENO, "\n", 1);
	return (MGP_ERROR);
}
