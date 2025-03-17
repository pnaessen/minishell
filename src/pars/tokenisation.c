#include "pars.h"

int	lines_in_node(const char *s1, t_data *data)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	data->quotes = ERROR;
	while (s1[i])
	{
		handle_quotes(s1[i], data);
		if (s1[i] != ' ' && (s1[i + 1] == ' ' || s1[i + 1] == '\0'))
		{
			if (data->quotes == ERROR)
				count++;
		}
		i++;
	}
	return (count);
}

int	words_in_lines(const char *s1, t_data *data, int i)
{
	int	count;

	count = 0;
	data->quotes = ERROR;
	while (s1[i] == ' ')
		i++;
	while (s1[i])
	{
		handle_quotes(s1[i], data);
		if (s1[i] == ' ' && data->quotes == ERROR)
			return (count);
		else
			count++;
		i++;
	}
	return (count);
}

char	*new_line_in_tab(const char *s1, int size, int i)
{
	char	*dup;
	int		j;

	dup = malloc((size + 1) * sizeof(char));
	j = 0;
	if (!(dup))
		return (0);
	while (j < size && s1[i])
	{
		dup[j] = s1[i];
		j++;
		i++;
	}
	dup[j] = '\0';
	return (dup);
}

char	**tokenisation(char const *s)
{
	char	**res;
	t_data	*data;
	int		i;
	int		j;

	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	data->quotes = ERROR;
	i = 0;
	j = 0;
	if (!(s))
		return (0);
	res = malloc((lines_in_node(s, data) + 1) * sizeof(char *));
	if (!(res))
		return (0);
	res[lines_in_node(s, data)] = NULL;
	while (s[i] && j < lines_in_node(s, data))
	{
		handle_quotes(s[i], data);
		if (s[i] != ' ' || data->quotes == SUCCESS)
		{
			res[j] = new_line_in_tab(s, words_in_lines(s, data, i), i);
			if (!(res[j]))
				return (ft_free_all(res));
			i += words_in_lines(s, data, i);
			j++;
		}
		i++;
	}
	res[j] = NULL;
	return (res);
}
