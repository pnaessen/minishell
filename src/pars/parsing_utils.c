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
	if (c == '|' || c == '&' || c == '<' || c == '>')
	{
		return (SUCCESS);
	}
	return (ERROR);
}

char	**ft_free_all(char **args)
{
	int	i;

	i = 0;
	if (args)
	{
		while (args[i])
		{
			free(args[i]);
			i++;
		}
		free(args);
	}
	return (SUCCESS);
}
