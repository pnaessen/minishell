/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:43:00 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/19 14:16:47 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_builtin(t_ast *input, t_env *env)
{
	int	ret;

	if (strcmp(input->cmd->args[0], "pwd") == 0)
		ret = ft_pwd();
	else if (strcmp(input->cmd->args[0], "echo") == 0)
		ret = ft_echo();
	else if (strcmp(input->cmd->args[0], "cd") == 0)
		ret = ft_cd();
	else if (strcmp(input->cmd->args[0], "env") == 0)
		ret = print_stack(&env);
	else if (strcmp(input->cmd->args[0], "exit") == 0)
		ret = ft_exit();
	else if (strcmp(input->cmd->args[0], "export") == 0)
		ret = ft_export();
	else if (strcmp(input->cmd->args[0], "unset") == 0)
		ret = ft_unset();
	else
		return (-1);
	return (ret);
}
