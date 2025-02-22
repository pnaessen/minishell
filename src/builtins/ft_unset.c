/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 13:31:29 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/22 16:28:00 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	valid_var(char *str)
{
	int	i;

	i = 0;
	if (!str || !*str || ft_isdigit(*str))
		return (0);
	while (str[i] && str[i] != '=')
	{
		if (!ft_isalnum(str[i] && str[i] != '_'))
			return (0);
		i++;
	}
	return (1);
}

void	ft_unset(t_ast *input, t_env **env)
{
	int i;
	t_env *current;
	t_env *prev;

	i = 1;
	while (input->cmd->args[i])
	{
		if (!valid_var(input->cmd->args[i]))
		{
			ft_putstr_fd("minishell: unset: '", 2);
			ft_putstr_fd(input->cmd->args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			input->error_code = 1;
			return ;
		}
		prev = NULL;
		current = *env;
		while (current)
		{
			if (ft_strncmp(current->str, input->cmd->args[i],
					ft_strlen(input->cmd->args[i])) == 0)
			{
				if (prev)
					prev->next = current->next;
				else
					*env = current->next;
				free(current->str);
				free(current);
				break ;
			}
			prev = current;
			current = current->next;
		}
		i++;
	}
	input->error_code = 0;
}