#include "pars.h"

int	add_whitespace(char b, char a, int quotes)
{
	if (ft_iswhitespace(b) == SUCCESS && quotes == ERROR)
	{
		if (ft_iswhitespace(a) == ERROR)
			return (SUCCESS);
	}
	return (ERROR);
}
int	check_num_of_quotes(char *args)
{
	int	single_quote;
	int	double_quote;
	int	i;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	while (args[i])
	{
		if (ft_is_quotes(args[i]) == SUCCESS)
		{
			if (args[i] == 39)
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

int	len_without_whitespaces(char *args)
{
	int	i;
	int	len;
	int	quotes;

	i = 0;
	len = 0;
	quotes = ERROR;
	while (args[i])
	{
		quotes = handle_quotes(args[i], quotes);
		if (add_whitespace(args[i], args[i - 1], quotes) == SUCCESS)
			len++;
		else if (ft_iswhitespace(args[i]) == ERROR || quotes == SUCCESS)
			len++;
		i++;
	}
	return (len);
}

char	*rm_whitespaces(char *args, int size)
{
	char	*str;
	int		quotes;
	int		i;
	int		j;

	i = 0;
	j = 0;
	quotes = ERROR;
	str = malloc(size * sizeof(char));
	if (!str)
		return (NULL);
	while (args[i])
	{
		quotes = handle_quotes(args[i], quotes);
		if (add_whitespace(args[i], args[i - 1], quotes) == SUCCESS)
			str[j++] = ' ';
		else if (ft_iswhitespace(args[i]) == ERROR || quotes == SUCCESS)
			str[j++] = args[i];
		i++;
	}
	str[j] = '\0';
	return (str);
}
char	*handle_whitespaces(char *args)
{
	int		size;
	char	*args_cleaned;
	char	*args_trim;

	if (check_num_of_quotes(args) == ERROR)
		return (NULL);
	size = len_without_whitespaces(args) + 1;
	args_cleaned = rm_whitespaces(args, size);
	args_trim = ft_strtrim(args_cleaned, " ");
	free(args_cleaned);
	return (args_trim);
}
