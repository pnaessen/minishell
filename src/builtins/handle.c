/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:43:00 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/18 16:53:49 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	check_build_in(t_cmd *cmd, t_shell *shell)
{
	int	ret;

	if (strcmp(cmd->args[0], "pwd") == 0)
		ret = ft_pwd(shell, cmd);
	else if (strcmp(cmd->args[0], "echo") == 0)
		ret = ft_echo(shell, cmd);
	else if (strcmp(cmd->args[0], "cd") == 0)
		ret = ft_cd(shell, cmd);
	else if (strcmp(cmd->args[0], "env") == 0)
		ret = ft_env(shell, cmd);
	else if (strcmp(cmd->args[0], "exit") == 0)
		ret = ft_exit(shell, cmd);
	else if (strcmp(cmd->args[0], "export") == 0)
		ret = ft_export(shell, cmd);
	else if (strcmp(cmd->args[0], "unset") == 0)
		ret = ft_unset(shell, cmd);
	else
		return (-1);
	return (ret);
}
