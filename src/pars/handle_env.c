/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:00:43 by vicperri          #+#    #+#             */
/*   Updated: 2025/04/10 15:00:46 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pars.h"

char	*find_and_replace_var(char *args, t_env **env, int tmp_error)
{
	char	**tab;
	char	*res;
	t_data	data;

	if (!env)
		*env = NULL;
	tab = split_var(args);
	if (!tab)
		return (NULL);
	data.i = 0;
	data.error_code = tmp_error;
	while (tab[data.i])
	{
		data.quote_type = '\0';
		data.quote_num = 0;
		data.quotes = ERROR;
		data.j = 0;
		tab[data.i] = process_variable_replacement(tab, &data, env);
		data.i++;
	}
	res = return_env(args, tab);
	return (res);
}

char	*return_env(char *args, char **tab_args)
{
	int		space;
	char	*new_args;

	space = spaces_to_add(args);
	new_args = join_tabs(tab_args, space);
	ft_free_all(tab_args);
	return (new_args);
}

int	spaces_to_add(const char *s1)
{
	int		i;
	int		count;
	t_data	data;

	i = 0;
	count = 0;
	data.quotes = ERROR;
	while (s1[i])
	{
		check_quotes(s1[i], &data);
		if (s1[i + 1] == '\0')
			return (count);
		if (s1[i] != ' ' && s1[i + 1] == ' ')
		{
			if (data.quotes == ERROR)
				count++;
		}
		i++;
	}
	return (count);
}

int	len_tab_of_tab(char **tab_args)
{
	int	i;
	int	size;

	i = 0;
	size = 0;
	while (tab_args[i])
	{
		size += ft_strlen(tab_args[i]);
		i++;
	}
	return (size + (i - 1));
}

char	*join_tabs(char **tab_args, int space)
{
	t_data	data;
	int		len;
	char	*new_args;

	data.i = 0;
	data.count = 0;
	len = len_tab_of_tab(tab_args);
	new_args = malloc((len + 1) * sizeof(char));
	if (!new_args)
		return (NULL);
	while (tab_args[data.i])
	{
		data.j = 0;
		while (tab_args[data.i][data.j])
			new_args[data.count++] = tab_args[data.i][data.j++];
		if (space > 0)
		{
			new_args[data.count++] = ' ';
			space--;
		}
		data.i++;
	}
	new_args[data.count] = '\0';
	return (new_args);
}
