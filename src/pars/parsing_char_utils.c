#include "pars.h"

int	ft_iswhitespace(int c)
{
	if ((c == ' ') || (c >= 9 && c <= 13))
		return (SUCCESS);
	else
		return (ERROR);
}

int	ft_is_operator(int c)
{
	if (c == 0)
		return (ERROR);
	if (ft_isalnum(c) == ERROR && ft_iswhitespace(c) == ERROR)
	{
		if (c != '.' && c != 39 && c != '"' && c != '$')
			return (SUCCESS);
	}
	return (ERROR);
}

int	ft_is_quotes(char c)
{
	if (c == 39 || c == '"')
		return (SUCCESS);
	return (ERROR);
}
