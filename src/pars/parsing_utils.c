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

int	ft_is_quotes(char c)
{
	if (c == 39 || c == '"')
		return (SUCCESS);
	return (ERROR);
}
int	handle_quotes(char argv, int quotes)
{
	if (ft_is_quotes(argv) == SUCCESS)
	{
		if (quotes == SUCCESS)
			quotes = ERROR;
		else
			quotes = SUCCESS;
	}
	return (quotes);
}

char	**ft_free_all(char **args)
{
	int	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
	return (SUCCESS);
}
