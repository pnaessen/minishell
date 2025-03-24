/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 12:23:40 by pnaessen          #+#    #+#             */
/*   Updated: 2025/03/21 17:46:31 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (0);
		i++;
	}
	return (1);
}

void	handle_numeric_exit(t_ast *cmd, t_env *env, char *arg)
{
	int	exit_code;

	exit_code = ft_atoi(arg);
	if ((exit_code == -1 && arg[0] != '-') || (exit_code == 0 && arg[0] == '-'
			&& arg[1] != '0'))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		free_ast(cmd->root);
		free_env_list(env);
		exit(2);
	}
	if (exit_code < 0)
		exit_code = 256 + (exit_code % 256);
	else
		exit_code = exit_code % 256;
	cmd->error_code = exit_code;
	free_ast(cmd->root);
	free_env_list(env);
	exit(exit_code);
}

void	ft_exit(t_ast *cmd, t_env *env)
{
	printf("exit\n");
	if (cmd->cmd->args[1] && cmd->cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		cmd->error_code = 1;
		return ;
	}
	if (cmd->cmd->args[1])
	{
		if (!ft_is_valid_number(cmd->cmd->args[1]))
		{
			ft_putstr_fd("minishell: exit: ", 2);
			ft_putstr_fd(cmd->cmd->args[1], 2);
			ft_putstr_fd(": numeric argument required\n", 2);
			free_ast(cmd->root);
			free_env_list(env);
			exit(2);
		}
		handle_numeric_exit(cmd, env, cmd->cmd->args[1]);
	}
	else
	{
		cmd->error_code = 0;
		free_ast(cmd->root);
		free_env_list(env);
		exit(0);
	}
}
