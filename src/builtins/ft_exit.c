/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 12:23:40 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/21 13:15:08 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_exit(t_ast *cmd, t_env *env)
{
	int	exit_code;

	printf("exit\n");
	if (cmd->cmd->args[2])
	{
		perror("exit: too many arguments\n");
		cmd->error_code = 1;
		return (1);
	}
	if (cmd->cmd->args[1])
	{
		if (!ft_isdigit(cmd->cmd->args[1]))
		{
			perror("bash exit");
			cmd->error_code = 2;
			return (2);
		}
		exit_code = ft_atoi(cmd->cmd->args[1]);
		if (exit_code < 0)
			exit_code = 256 + (exit_code % 256);
		else
			exit_code = exit_code % 256;
	}
	else
		exit_code = 0;
	cmd->error_code = exit_code;
	return (exit_code);
}

int	ft_isdigit(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	return (1);
}
