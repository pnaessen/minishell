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
	if (c == '\'' || c == '"')
		return (SUCCESS);
	return (ERROR);
}

void	check_quotes(char argv, t_data *data)
{
	if (ft_is_quotes(argv) == SUCCESS)
	{
		if (data->quotes == SUCCESS)
			data->quotes = ERROR;
		else
			data->quotes = SUCCESS;
	}
}

void	handle_quotes(char argv, t_data *data)
{
	if (ft_is_quotes(argv) == SUCCESS)
	{
		if (data->quotes == SUCCESS && data->quote_type == argv)
		{
			data->quotes = ERROR;
			data->quote_type = '\0';
			data->quote_num++;
		}
		else
		{
			data->quotes = SUCCESS;
			if (data->quote_type == '\0')
			{
				data->quote_type = argv;
				data->quote_num++;
			}
		}
	}
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
