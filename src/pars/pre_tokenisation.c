#include "pars.h"

int	num_of_lines(const char *s1)
{
	t_data	data;

	data.i = 0;
	data.count = 0;
	data.quotes = ERROR;
	data.quote_num = 0;
	data.quote_type = '\0';
	while (s1[data.i])
	{
		handle_quotes(s1[data.i], &data);
		if (is_redirection_operator(s1, data.i) && data.quotes == ERROR)
			data.count++;
		else if (is_end_of_word(s1, data.i, &data))
			data.count++;
		else if (is_operator_sequence(s1, data.i, &data))
		{
			while (s1[data.i] && ft_is_operator(s1[data.i]) == SUCCESS)
				data.i++;
			data.count++;
		}
		if (s1[data.i] != '\0')
			data.i++;
	}
	return (data.count);
}

void	init_data(t_data *data)
{
	data->count = 0;
	data->quotes = ERROR;
	data->quote_num = 0;
	data->quote_type = '\0';
}

int	num_of_words(const char *s1, int i)
{
	t_data	data;

	init_data(&data);
	data.count = handle_multi_operators(s1, i);
	if (data.count != 0)
		return (data.count);
	while (s1[i])
	{
		handle_quotes(s1[i], &data);
		if (is_redirection_operator(s1, i) && data.quotes == ERROR)
		{
			while (s1[i] && s1[i] != ' ')
			{
				i++;
				data.count++;
			}
			return (data.count);
		}
		if (ft_is_operator(s1[i + 1]) == ERROR || data.quotes == SUCCESS)
			data.count++;
		else
			return (data.count);
		i++;
	}
	return (data.count);
}

static char	*ft_newtab(const char *s1, int size, int i)
{
	char	*dup;
	int		j;

	dup = malloc((size + 1) * sizeof(char));
	j = 0;
	if (!(dup))
		return (0);
	while (s1[i] && j < size)
	{
		dup[j] = s1[i];
		j++;
		i++;
	}
	dup[j] = '\0';
	return (dup);
}

char	**pre_tokenisation(char const *s)
{
	char	**res;
	int		i;
	int		j;
	int		tmp;

	i = 0;
	j = 0;
	tmp = num_of_lines(s);
	res = malloc((tmp + 1) * sizeof(char *));
	if (!(res))
		return (0);
	while (s[i] && j < tmp)
	{
		res[j] = ft_newtab(s, num_of_words(s, i), i);
		if (!res[j])
			return (ft_free_all(res));
		i += num_of_words(s, i);
		if (s[i])
			i++;
		j++;
	}
	res[j] = NULL;
	return (res);
}
