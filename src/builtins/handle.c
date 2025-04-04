/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:43:00 by pnaessen          #+#    #+#             */
/*   Updated: 2025/03/25 10:11:45 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_builtin(t_ast *input, t_env *env)
{
	if (!input->cmd || !input->cmd->args)
		return ;
	if (ft_strcmp(input->cmd->args[0], "pwd") == 0)
		ft_pwd(input);
	else if (ft_strcmp(input->cmd->args[0], "echo") == 0)
		ft_echo(input);
	else if (ft_strcmp(input->cmd->args[0], "cd") == 0)
		ft_cd(input, &env);
	else if (ft_strcmp(input->cmd->args[0], "env") == 0)
		print_env(&env, input);
	else if (ft_strcmp(input->cmd->args[0], "exit") == 0)
		ft_exit(input, env);
	else if (ft_strcmp(input->cmd->args[0], "export") == 0)
		ft_export(input, &env);
	else if (ft_strcmp(input->cmd->args[0], "unset") == 0)
		ft_unset(input, &env);
	else
		input->error_code = -1;
}
