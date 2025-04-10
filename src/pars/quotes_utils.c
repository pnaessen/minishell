#include "pars.h"

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

