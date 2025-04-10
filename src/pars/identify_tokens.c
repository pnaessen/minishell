/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identify_tokens.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:02:38 by vicperri          #+#    #+#             */
/*   Updated: 2025/04/10 15:02:40 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pars.h"

int	identify_token_type(t_stack **stack)
{
	t_stack	*temp;
	t_data	data;
	int		i;

	temp = *stack;
	while (1)
	{
		i = 0;
		data.quotes = ERROR;
		while (temp->cmd[i])
		{
			check_quotes(temp->cmd[i][0], &data);
			if (ft_is_operator(temp->cmd[i][0]) == ERROR
				|| data.quotes == SUCCESS)
				temp->token = CMD;
			else
				define_type(temp, temp->cmd[i], data.quotes);
			i++;
		}
		temp = temp->next;
		if (temp == *stack)
			break ;
	}
	return (SUCCESS);
}

int	loop_through_stack(t_stack *tmp, t_stack *frst)
{
	while (1)
	{
		if (is_redirection(tmp->token) && (tmp->next == frst
				|| tmp->next->token != CMD))
		{
			ft_putstr_fd("minishell: syntax error  unexpected token `", 2);
			ft_putstr_fd(tmp->cmd[0], 2);
			ft_putstr_fd("'\n", 2);
			return (ERROR);
		}
		if (tmp->token == PIPE && tmp->next != frst && tmp->next->token == PIPE)
		{
			ft_putstr_fd("minishell: syntax error  unexpected token `|'\n", 2);
			return (ERROR);
		}
		if (check_redirection_syntax(tmp) == ERROR)
			return (ERROR);
		tmp = tmp->next;
		if (tmp == frst)
			break ;
	}
	return (SUCCESS);
}

int	check_errors(t_stack **stack)
{
	t_stack	*tmp;
	t_stack	*frst;
	int		result;

	if (!stack || !*stack)
		return (ERROR);
	tmp = *stack;
	frst = *stack;
	if (check_initial_errors(tmp, frst) == ERROR)
		return (ERROR);
	result = loop_through_stack(tmp, frst);
	return (result);
}
