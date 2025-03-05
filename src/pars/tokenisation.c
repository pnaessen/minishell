#include "pars.h"

int	lines_in_node(const char *s1)
{
	int	i;
	int	count;
	int	quotes;

	i = 0;
	count = 0;
	quotes = ERROR;
	while (s1[i])
	{
		quotes = handle_quotes(s1[i], quotes);
		if (s1[i] != ' ' && (s1[i + 1] == ' ' || s1[i + 1] == '\0'))
		{
			if (quotes == ERROR)
				count++;
		}
		i++;
	}
	return (count);
}

int	words_in_lines(const char *s1, int i)
{
	int	count;
	int	quotes;

	count = 0;
	quotes = ERROR;
	while (s1[i] == ' ')
		i++;
	while (s1[i])
	{
		quotes = handle_quotes(s1[i], quotes);
		if (s1[i] == ' ' && quotes == ERROR)
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
	int		quotes;
	int		i;
	int		j;

	quotes = ERROR;
	i = 0;
	j = 0;
	if (!(s))
		return (0);
	res = malloc((lines_in_node(s) + 1) * sizeof(char *));
	if (!(res))
		return (0);
	res[lines_in_node(s)] = NULL;
	while (s[i] && j < lines_in_node(s))
	{
		quotes = handle_quotes(s[i], quotes);
		if (s[i] != ' ' || quotes == SUCCESS)
		{
			res[j] = new_line_in_tab(s, words_in_lines(s, i), i);
			if (!(res[j]))
				return (ft_free_all(res));
			i += words_in_lines(s, i);
			j++;
		}
		i++;
	}
	return (res);
}
