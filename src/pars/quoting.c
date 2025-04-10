/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quoting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:03:25 by vicperri          #+#    #+#             */
/*   Updated: 2025/04/10 15:03:26 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "pars.h"

int	check_num_of_quotes(char *args)
{
	int		i;
	t_data	data;

	i = 0;
	data.quotes = ERROR;
	data.quote_num = 0;
	data.quote_type = '\0';
	while (args[i])
	{
		handle_quotes(args[i], &data);
		i++;
	}
	if (data.quote_num % 2 != 0)
		return (ERROR);
	return (SUCCESS);
}

int	final_len(char *args)
{
	t_data	data;

	data.i = 0;
	data.count = 0;
	data.quotes = ERROR;
	data.quote_type = '\0';
	data.quote_num = 0;
	while (args[data.i])
	{
		handle_quotes(args[data.i], &data);
		if (calculate_len(args, &data) == ERROR)
		{
			if (args[data.i])
			{
				data.i++;
				data.count++;
			}
		}
	}
	return (data.count);
}

void	handle_quote_transitions(char *args, t_data *data)
{
	if (args[data->i + 1] && data->quote_type == args[data->i + 1]
		&& data->quote_type != args[data->i])
	{
		data->temp[data->j++] = args[data->i++];
		if (args[data->i] && ft_is_quotes(args[data->i]) == SUCCESS
			&& data->quote_type == args[data->i])
		{
			data->quote_type = '\0';
			data->i++;
		}
	}
	else if (args[data->i] && ft_is_quotes(args[data->i]) == SUCCESS
		&& data->quote_type == args[data->i])
	{
		data->i++;
		if (args[data->i] && ft_is_quotes(args[data->i]) == SUCCESS
			&& data->quote_type == args[data->i])
		{
			data->quote_type = '\0';
			data->i++;
		}
	}
	else if (args[data->i])
		data->temp[data->j++] = args[data->i++];
}

char	*handling_quotes(char *args, int size)
{
	t_data	data;

	data.i = 0;
	data.j = 0;
	data.quotes = 0;
	data.quote_type = '\0';
	data.quote_num = 0;
	data.temp = malloc(size * sizeof(char));
	if (!data.temp)
		return (NULL);
	while (args[data.i])
	{
		handle_quotes(args[data.i], &data);
		handle_quote_transitions(args, &data);
	}
	data.temp[data.j] = '\0';
	return (data.temp);
}

int	quoting(t_stack **stack)
{
	t_stack	*temp;
	char	*new_cmd;
	int		size;
	int		i;

	temp = *stack;
	while (1)
	{
		i = 0;
		while (temp->cmd[i])
		{
			size = final_len(temp->cmd[i]) + 1;
			new_cmd = handling_quotes(temp->cmd[i], size);
			if (!new_cmd)
				return (ERROR);
			free(temp->cmd[i]);
			temp->cmd[i] = new_cmd;
			i++;
		}
		temp = temp->next;
		if (temp == *stack)
			break ;
	}
	return (SUCCESS);
}
