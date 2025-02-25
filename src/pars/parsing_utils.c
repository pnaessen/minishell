#include "pars.h"

int	len_without_whitespaces(char *argv)
{
	int	i;
	int	len;
	int	quotes;

	i = 0;
	len = 0;
	quotes = ERROR;
	while (argv[i])
	{
		quotes = handle_quotes(argv[i], quotes);
		if (add_whitespace(argv[i], argv[i - 1], quotes) == SUCCESS)
			len++;
		else if (ft_iswhitespace(argv[i]) == ERROR || quotes == SUCCESS)
			len++;
		i++;
	}
	return (len);
}

int	check_num_of_quotes(char *argv)
{
	int	single_quote;
	int	double_quote;
	int	i;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (argv[i])
	{
		if (ft_is_quotes(argv[i]) == SUCCESS)
		{
			if (argv[i] == 39)
				single_quote++;
			else
				double_quote++;
		}
		i++;
	}
	if (single_quote % 2 != 0 || double_quote % 2 != 0)
	{
		ft_printf("ERROR : quotes aren't in pairs");
		return (ERROR);
	}
	return (SUCCESS);
}

int	len_for_cleaned_args(char *args)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (args[i])
	{
		if (ft_is_operator(args[i]) == SUCCESS)
		{
			if (ft_isalnum(args[i - 1]) == SUCCESS || ft_isalnum(args[i
					+ 1]) == SUCCESS)
			{
				if (ft_isalnum(args[i - 1]) == SUCCESS)
					len++;
				i++;
				if (args[i] != '-')
					len++;
			}
		}
		i++;
	}
	return (len + i);
}
void ft_free_all(char **args)
{
	size_t	i;

	i = 0;
	while (args[i])
	{
		free(args[i]);
		args[i] = NULL;
		i++;
	}
	free(args);
}