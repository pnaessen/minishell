/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pn <pn@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 12:23:40 by pnaessen          #+#    #+#             */
/*   Updated: 2025/03/25 19:38:24 by pn               ###   ########lyon.fr   */
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

void	exit_with_message(t_ast *cmd, t_env *env, char *arg, int code)
{
	ft_putstr_fd("minishell: exit: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": numeric argument required\n", 2);
	free_ast(cmd->root);
	free_env_list(env);
	exit(code);
}

int	handle_numeric_exit(t_ast *cmd, t_env *env, char *arg)
{
	int	exit_code;
	int	is_invalid;

	exit_code = ft_atoi(arg);
	is_invalid = (exit_code == -1 && arg[0] != '-');
	is_invalid = is_invalid || (exit_code == 0 && arg[0] == '-'
			&& arg[1] != '0');
	if (is_invalid)
		exit_with_message(cmd, env, arg, 2);
	if (exit_code < 0)
		exit_code = 256 + (exit_code % 256);
	else
		exit_code = exit_code % 256;
	return (exit_code);
}

void	exit_without_args(t_ast *cmd, t_env *env)
{
	cmd->error_code = 0;
	free_ast(cmd->root);
	free_env_list(env);
	exit(0);
}

void	ft_exit(t_ast *cmd, t_env *env)
{
	int	exit_code;

	printf("exit\n");
	if (!cmd->cmd->args[1])
		exit_without_args(cmd, env);
	if (!ft_is_valid_number(cmd->cmd->args[1]))
		exit_with_message(cmd, env, cmd->cmd->args[1], 2);
	exit_code = handle_numeric_exit(cmd, env, cmd->cmd->args[1]);
	if (cmd->cmd->args[2])
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		cmd->error_code = 1;
		return ;
	}
	free_ast(cmd->root);
	free_env_list(env);
	exit(exit_code);
}
