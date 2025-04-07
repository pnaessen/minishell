#include "pars.h"

static int	lines_in_node(const char *s1)
{
	int		i;
	int		count;
	t_data	data;

	i = 0;
	count = 0;
	data.quotes = ERROR;
	data.quote_type = '\0';
	data.quote_num = 0;
	while (s1[i])
	{
		handle_quotes(s1[i], &data);
		if ((s1[i] != ' ' && s1[i] != '$') && (s1[i + 1] && (s1[i + 1] == ' '
					|| (s1[i + 1] == '\0' && data.quotes == ERROR))))
		{
			if (data.quotes == ERROR)
				count++;
		}
		if (s1[i] == '$' && (i == 0 || s1[i - 1] != '$'))
			count++;
		i++;
	}
	return (count);
}

static int	cnt_words(const char *s1, int i)
{
	int		count;
	t_data	data;

	count = 0;
	data.quotes = ERROR;
	data.quote_type = '\0';
	data.quote_num = 0;
	while (s1[i] == ' ')
		i++;
	while (s1[i])
	{
		handle_quotes(s1[i], &data);
		if (s1[i] == ' ' && data.quotes == ERROR)
			return (count);
		// else if (s1[i] == 39 && data.quote_type != 39)
		//  	return (count);
		else if ((data.quotes == ERROR || data.quote_type != 39) && (s1[i] && s1[i
				+ 1] == '$' && count > 0))
		{
			count++;
			return (count);
		}

		else
			count++;
		i++;
	}
	return (count);
}

static char	*create_tab(const char *s1, int size, int i)
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

char	**split_var(char const *s)
{
	char	**res;
	t_data	data;

	data.quotes = ERROR;
	data.i = 0;
	data.count = 0;
	res = malloc((lines_in_node(s) + 1) * sizeof(char *));
	if (!(res))
		return (0);
	while (s[data.i] && data.count <= lines_in_node(s))
	{
		check_quotes(s[data.i], &data);
		res[data.count] = create_tab(s, cnt_words(s, data.i), data.i);
		printf("split var res[%d] : |%s|\n", data.count, res[data.count]);
		if (!(res[data.count]))
			return (ft_free_all(res));
		data.i += cnt_words(s, data.i);
		data.count++;
		if (s[data.i] && s[data.i] != '$')
			data.i++;
	}
	res[data.count] = NULL;
	printf("split var res[%d] : %s\n", data.count, res[data.count]);
	return (res);
}

// add