/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   replace_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:03:33 by vicperri          #+#    #+#             */
/*   Updated: 2025/04/10 15:03:34 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pars.h"

void	init_data_in_empty(t_data *data)
{
	data->i = 0;
	data->count = 0;
}

char	*replace_with_empty(char *args, int pos)
{
	char	*new_args;
	t_data	data;

	init_data_in_empty(&data);
	size_of_empty_args(args, pos, &data);
	new_args = malloc((data.count + 1) * sizeof(char));
	if (!new_args)
		return (NULL);
	init_data_in_empty(&data);
	while (args[data.i])
	{
		if (data.i == pos)
		{
			new_args[data.count++] = '"';
			new_args[data.count++] = '"';
			while (args[data.i] && (args[data.i] != ' '
					|| ft_is_quotes(args[data.i]) == ERROR))
				data.i++;
		}
		if (args[data.i])
			new_args[data.count++] = args[data.i++];
	}
	new_args[data.count] = '\0';
	return (new_args);
}

void	init_data_in_env(t_data *data, char *var_name)
{
	data->i = 0;
	data->count = 0;
	data->start = ft_strlen(var_name);
}

char	*replace_value(char *args, int pos, char *value, char *var_name)
{
	t_data	data;

	init_data_in_env(&data, var_name);
	size_of_args(args, pos, value, &data);
	data.temp = malloc((data.count + 1) * sizeof(char));
	if (!data.temp)
		return (NULL);
	init_data_in_env(&data, var_name);
	while (args[data.i])
	{
		if (data.i == pos)
		{
			while (*value)
				data.temp[data.count++] = *value++;
			while (args[data.i] && data.start >= 0)
			{
				data.start--;
				data.i++;
			}
		}
		if (args[data.i] != '\0')
			data.temp[data.count++] = args[data.i++];
	}
	data.temp[data.count] = '\0';
	return (data.temp);
}

char	*replace_without_dollar(char *args, int pos, int quote)
{
	char	*new_args;
	t_data	data;

	data.i = 0;
	data.j = 0;
	if (quote == SUCCESS)
		data.count = ft_strlen(args) - 4;
	else
		data.count = ft_strlen(args) - 2;
	new_args = malloc((data.count + 1) * sizeof(char));
	if (!new_args)
		return (NULL);
	while (args[data.i] && data.j < data.count)
	{
		if (data.i == pos)
		{
			if (quote == SUCCESS)
				data.i += 3;
			else
				data.i += 2;
		}
		new_args[data.j++] = args[data.i++];
	}
	new_args[data.j] = '\0';
	return (new_args);
}
