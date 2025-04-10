/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenisation.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:03:42 by vicperri          #+#    #+#             */
/*   Updated: 2025/04/10 15:03:43 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pars.h"

int	lines_in_node(const char *s1)
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
		if (s1[i] != ' ' && (s1[i + 1] == ' ' || s1[i + 1] == '\0'))
		{
			if (data.quotes == ERROR)
				count++;
		}
		i++;
	}
	return (count);
}

int	cnt_words(const char *s1, int i)
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
		else
			count++;
		i++;
	}
	return (count);
}

char	*create_tab(const char *s1, int size, int i)
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

char	**tokenisation(char const *s)
{
	char	**res;
	t_data	data;

	init_data(&data);
	data.i = 0;
	res = malloc((lines_in_node(s) + 1) * sizeof(char *));
	if (!(res))
		return (0);
	while (s[data.i] && data.count < lines_in_node(s))
	{
		handle_quotes(s[data.i], &data);
		if (s[data.i] != ' ' || data.quotes == SUCCESS)
		{
			res[data.count] = create_tab(s, cnt_words(s, data.i), data.i);
			if (!(res[data.count]))
				return (ft_free_all(res));
			data.i += cnt_words(s, data.i);
			data.count++;
		}
		if (s[data.i])
			data.i++;
	}
	res[data.count] = NULL;
	return (res);
}
