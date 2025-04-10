/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   identify_tokens_utils.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:00:58 by vicperri          #+#    #+#             */
/*   Updated: 2025/04/10 15:00:59 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
#include "pars.h"

void	define_type(t_stack *temp, char *cmd, int quotes)
{
	if (quotes == ERROR)
	{
		if (cmd[0] == '|' && cmd[1] != '|')
			temp->token = PIPE;
		if (cmd[0] == '|' && cmd[1] == '|')
			temp->token = OR;
		if (cmd[0] == '<' && cmd[1] != '<')
			temp->token = REDIR_IN;
		if (cmd[0] == '<' && cmd[1] == '<')
			temp->token = REDIR_HEREDOC;
		if (cmd[0] == '>' && cmd[1] != '>')
			temp->token = REDIR_OUT;
		if (cmd[0] == '>' && cmd[1] == '>')
			temp->token = APPEND;
		if (cmd[0] == '&' && cmd[1] == '&')
			temp->token = AND;
	}
}

int	check_initial_errors(t_stack *temp, t_stack *first)
{
	if (temp->token == PIPE)
	{
		ft_putstr_fd("minishell: syntax error  unexpected token `|'\n", 2);
		return (ERROR);
	}
	if (first->prev->token != CMD)
	{
		ft_putstr_fd("minishell: syntax error  unexpected token `", 2);
		ft_putstr_fd(first->prev->cmd[0], 2);
		ft_putstr_fd("'\n", 2);
		return (ERROR);
	}
	return (SUCCESS);
}

int	check_redirection_syntax(t_stack *temp)
{
	int	i;

	i = 0;
	while (temp->cmd && temp->cmd[i])
	{
		if ((temp->cmd[i][0] == '>' && temp->cmd[i][1] == '<')
			|| (temp->cmd[i][0] == '<' && temp->cmd[i][1] == '>'))
		{
			ft_putstr_fd("minishell: syntax error  unexpected token `", 2);
			ft_putstr_fd(temp->cmd[i], 2);
			ft_putstr_fd("'\n", 2);
			return (ERROR);
		}
		i++;
	}
	return (SUCCESS);
}
