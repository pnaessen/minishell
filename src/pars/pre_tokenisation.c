#include "pars.h"

int	num_of_lines(const char *s1, t_data *data)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	data->quotes = ERROR;
	while (s1[i])
	{
		handle_quotes(s1[i], data);
		if ((ft_is_operator(s1[i]) == ERROR && ft_is_operator(s1[i
					+ 1]) == SUCCESS) || s1[i + 1] == '\0')
		{
			if (data->quotes == ERROR)
				count++;
		}
		else if (ft_is_operator(s1[i]) == SUCCESS && data->quotes == ERROR)
		{
			while (ft_is_operator(s1[i]) == SUCCESS)
				i++;
			count++;
		}
		i++;
	}
	return (count);
}

int	num_of_words(const char *s1, t_data *data, int i)
{
	int	count;

	count = 0;
	data->quotes = ERROR;
	if (ft_is_operator(s1[i]) == SUCCESS)
	{
		while (ft_is_operator(s1[i]) == SUCCESS)
		{
			i++;
			count++;
		}
		return (count);
	}
	while (s1[i])
	{
		handle_quotes(s1[i], data);
		if (ft_is_operator(s1[i + 1]) == ERROR || data->quotes == SUCCESS)
			count++;
		else
			return (count);
		i++;
	}
	return (count);
}

static char	*ft_newtab(const char *s1, int size, int i)
{
	char	*dup;
	int		j;

	dup = malloc((size + 1) * sizeof(char));
	j = 0;
	if (!(dup))
		return (0);
	while (j <= size && s1[i])
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
	t_data	*data;
	int		i;
	int		j;

	i = 0;
	j = 0;
	data = malloc(sizeof(t_data));
	if (!data)
		return (NULL);
	if (!(s))
		return (0);
	res = malloc((num_of_lines(s, data) + 1) * sizeof(char *));
	if (!(res))
		return (0);
	res[num_of_lines(s, data)] = NULL;
	while (s[i] && j < num_of_lines(s, data))
	{
		res[j] = ft_newtab(s, num_of_words(s, data, i), i);
		if (!(res[j]))
			return (ft_free_all(res));
		i += num_of_words(s, data, i);
		j++;
		i++;
	}
	return (res);
}
