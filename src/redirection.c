/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pn <pn@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 12:13:47 by pn                #+#    #+#             */
/*   Updated: 2025/02/23 17:54:43 by pn               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int handle_redir_in(t_ast *cmd, t_env *env)
{
	int fd;
	int saved_fd;

	saved_fd = dup(STDIN_FILENO);
	fd = open(cmd->right->cmd->args[0], O_RDONLY);
	if (fd == -1)
	{
		perror("open");
		return (1);
	}
	dup2(fd, STDIN_FILENO);
	close(fd);
	cmd->error_code = execute_ast(cmd->left, env);
	dup2(saved_fd, STDIN_FILENO);
	close(saved_fd);
	return (cmd->error_code);
}

int handle_redir_out(t_ast *cmd, t_env *env)
{
	int fd;
	int saved_fd;

	saved_fd = dup(STDOUT_FILENO);
	fd = open(cmd->right->cmd->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		return (1);
	}
	dup2(fd, STDOUT_FILENO);
	close(fd);
	cmd->error_code = execute_ast(cmd->left, env);
	dup2(saved_fd, STDOUT_FILENO);
	close(saved_fd);
	return (cmd->error_code);
}