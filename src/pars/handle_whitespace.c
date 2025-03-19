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

int	len_without_whitespaces(char *args)
{
	int		i;
	int		len;
	t_data	data;

	i = 0;
	len = 0;
	data.quotes = ERROR;
	while (args[i])
	{
		check_quotes(args[i], &data);
		if ((i - 1 > 0) && (add_whitespace(args[i], args[i - 1], data.quotes) == SUCCESS))
			len++;
		else if (ft_iswhitespace(args[i]) == ERROR || data.quotes == SUCCESS)
			len++;
		i++;
	}
	return (len);
}

char	*rm_whitespaces(char *args, int size)
{
	char	*str;
	t_data	data;
	int		i;
	int		j;

	i = 0;
	j = 0;
	data.quotes = ERROR;
	str = malloc(size * sizeof(char));
	if (!str)
		return (NULL);
	while (args[i])
	{
		check_quotes(args[i], &data);
		if ((i - 1 > 0) && (add_whitespace(args[i], args[i - 1], data.quotes) == SUCCESS))
			str[j++] = ' ';
		else if (ft_iswhitespace(args[i]) == ERROR || data.quotes == SUCCESS)
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

	size = len_without_whitespaces(args) + 1;
	args_cleaned = rm_whitespaces(args, size);
	args_trim = ft_strtrim(args_cleaned, " ");
	free(args_cleaned);
	return (args_trim);
}
