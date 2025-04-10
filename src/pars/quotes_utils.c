/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quotes_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:03:19 by vicperri          #+#    #+#             */
/*   Updated: 2025/04/10 15:03:20 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pars.h"

int	ft_is_quotes(char c)
{
	if (c == '\'' || c == '"')
		return (SUCCESS);
	return (ERROR);
}

void	check_quotes(char argv, t_data *data)
{
	if (ft_is_quotes(argv) == SUCCESS)
	{
		if (data->quotes == SUCCESS)
			data->quotes = ERROR;
		else
			data->quotes = SUCCESS;
	}
}

void	handle_quotes(char argv, t_data *data)
{
	if (ft_is_quotes(argv) == SUCCESS)
	{
		if (data->quotes == SUCCESS && data->quote_type == argv)
		{
			data->quotes = ERROR;
			data->quote_type = '\0';
			data->quote_num++;
		}
		else
		{
			data->quotes = SUCCESS;
			if (data->quote_type == '\0')
			{
				data->quote_type = argv;
				data->quote_num++;
			}
		}
	}
}

void	define_data_quoting(t_data *data)
{
	data->quote_type = '\0';
	data->i++;
}

int	calculate_len(char *args, t_data *data)
{
	if (args[data->i + 1] && data->quote_type == args[data->i + 1]
		&& data->quote_type != args[data->i])
	{
		data->i++;
		data->count++;
		if (args[data->i] && ft_is_quotes(args[data->i]) == SUCCESS
			&& data->quote_type == args[data->i])
			define_data_quoting(data);
		return (SUCCESS);
	}
	else if (args[data->i] && ft_is_quotes(args[data->i]) == SUCCESS
		&& data->quote_type == args[data->i])
	{
		data->i++;
		if (args[data->i] && ft_is_quotes(args[data->i]) == SUCCESS
			&& data->quote_type == args[data->i])
			define_data_quoting(data);
		return (SUCCESS);
	}
	return (ERROR);
}
