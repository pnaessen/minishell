/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split_var.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:03:37 by vicperri          #+#    #+#             */
/*   Updated: 2025/04/10 15:03:38 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pars.h"

void	init_data_split_var(t_data *data)
{
	data->i = 0;
	data->count = 0;
	data->quotes = ERROR;
	data->quote_type = '\0';
	data->quote_num = 0;
}

static int	lines_in_node(const char *s1)
{
	t_data	data;

	init_data_split_var(&data);
	while (s1[data.i])
	{
		handle_quotes(s1[data.i], &data);
		if (s1[data.i + 1] == '\0')
			break ;
		if ((s1[data.i] != ' ' && s1[data.i] != '$') && (s1[data.i + 1]
				&& (s1[data.i + 1] == ' ' || (s1[data.i + 1] == '\0'
						&& data.quotes == ERROR))))
		{
			if (data.quotes == ERROR)
				data.count++;
		}
		if (s1[data.i] == '$' && (data.i == 0 || s1[data.i - 1] != '$'))
			data.count++;
		data.i++;
	}
	return (data.count + 1);
}

static int	cnt_words(const char *s1, int i)
{
	t_data	data;

	init_data_split_var(&data);
	while (s1[i] == ' ')
		i++;
	while (s1[i])
	{
		handle_quotes(s1[i], &data);
		if (s1[i] == ' ' && data.quotes == ERROR)
			return (data.count);
		else if ((data.quotes == ERROR || data.quote_type != 39) && s1[i]
			&& s1[i + 1] && s1[i + 1] == '$' && data.count > 0)
		{
			if (s1[i + 1] == '\0')
				break ;
			data.count++;
			return (data.count);
		}
		else
			data.count++;
		i++;
	}
	return (data.count);
}

static char	*create_tab(const char *s1, int size, int i)
{
	char	*dup;
	int		j;

	j = 0;
	if (size <= 0)
		return (NULL);
	dup = malloc((size + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	while (s1[i] && j < size)
	{
		if (s1[i] == '\0')
			break ;
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
	int		word_len;
	int		lines;

	init_data_split_var(&data);
	lines = lines_in_node(s);
	res = malloc((lines + 1) * sizeof(char *));
	if (!res)
		return (NULL);
	while (s[data.i] && data.count <= lines)
	{
		word_len = cnt_words(s, data.i);
		check_quotes(s[data.i], &data);
		res[data.count] = create_tab(s, word_len, data.i);
		if (!res[data.count])
			return (ft_free_all(res));
		data.i += word_len;
		data.count++;
		if (s[data.i] && s[data.i] != '$')
			data.i++;
	}
	res[data.count] = NULL;
	return (res);
}
