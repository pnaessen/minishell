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

int	len_without_whitespaces(char *args, t_data *data)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	data->quotes = ERROR;
	while (args[i])
	{
		handle_quotes(args[i], data);
		if (add_whitespace(args[i], args[i - 1], data->quotes) == SUCCESS)
			len++;
		else if (ft_iswhitespace(args[i]) == ERROR || data->quotes == SUCCESS)
			len++;
		i++;
	}
	return (len);
}

char	*rm_whitespaces(char *args, t_data *data, int size)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	data->quotes = ERROR;
	str = malloc(size * sizeof(char));
	if (!str)
		return (NULL);
	while (args[i])
	{
		handle_quotes(args[i], data);
		if (add_whitespace(args[i], args[i - 1], data->quotes) == SUCCESS)
			str[j++] = ' ';
		else if (ft_iswhitespace(args[i]) == ERROR || data->quotes == SUCCESS)
			str[j++] = args[i];
		i++;
	}
	str[j] = '\0';
	return (str);
}
char	*handle_whitespaces(char *args)
{
	t_data	*data;
	int		size;
	char	*args_cleaned;
	char	*args_trim;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	size = len_without_whitespaces(args, data) + 1;
	args_cleaned = rm_whitespaces(args, data, size);
	args_trim = ft_strtrim(args_cleaned, " ");
	free(args_cleaned);
	return (args_trim);
}
