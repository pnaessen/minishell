/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pn <pn@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 12:13:47 by pn                #+#    #+#             */
/*   Updated: 2025/03/11 21:01:43 by pn               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_redir_in(t_ast *cmd, t_env *env)
{
	int	fd;
	int	saved_fd;

	saved_fd = dup(STDIN_FILENO);
	if (saved_fd == -1)
	{
		perror("minishell: dup");
		cmd->error_code = 1;
		return ;
	}
	fd = open(cmd->right->cmd->args[0], O_RDONLY);
	if (fd == -1)
	{
		perror("minishell: open");
		cmd->error_code = 1;
		close(saved_fd);
		return ;
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		cmd->error_code = 1;
		close(fd);
		close(saved_fd);
		return ;
	}
	close(fd);
	execute_ast(cmd->left, env);
	if (dup2(saved_fd, STDIN_FILENO) == -1)
		perror("minishell: dup2");
	close(saved_fd);
}

void	handle_redir_out(t_ast *cmd, t_env *env)
{
	int	fd;
	int	saved_fd;

	saved_fd = dup(STDOUT_FILENO);
	if (saved_fd == -1)
	{
		perror("minishell: dup");
		cmd->error_code = 1;
		return ;
	}
	fd = open(cmd->right->cmd->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell: open");
		cmd->error_code = 1;
		close(saved_fd);
		return ;
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		cmd->error_code = 1;
		close(fd);
		close(saved_fd);
		return ;
	}
	close(fd);
	execute_ast(cmd->left, env);
	if (dup2(saved_fd, STDOUT_FILENO) == -1)
		perror("minishell: dup2");
	close(saved_fd);
}

void	handle_redir_append(t_ast *cmd, t_env *env)
{
	int	fd;
	int	saved_fd;

	saved_fd = dup(STDOUT_FILENO);
	if (saved_fd == -1)
	{
		perror("minishell: dup");
		cmd->error_code = 1;
		return ;
	}
	fd = open(cmd->right->cmd->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("minishell: open");
		cmd->error_code = 1;
		close(saved_fd);
		return ;
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		cmd->error_code = 1;
		close(fd);
		close(saved_fd);
		return ;
	}
	close(fd);
	execute_ast(cmd->left, env);
	if (dup2(saved_fd, STDOUT_FILENO) == -1)
		perror("minishell: dup2");
	close(saved_fd);
}

int	apply_input_redirection(t_redir *redir)
{
	int	fd;

	fd = open(redir->file, O_RDONLY);
	if (fd == -1)
	{
		perror("minishell: open");
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_output_redirection(t_redir *redir)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell: open");
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_append_redirection(t_redir *redir)
{
	int	fd;

	fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("minishell: open");
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_redirection(t_redir *redir)
{
	if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
		return (apply_input_redirection(redir));
	else if (redir->type == REDIR_OUT)
		return (apply_output_redirection(redir));
	else if (redir->type == APPEND)
		return (apply_append_redirection(redir));
	return (0);
}

int	apply_all_redirections(t_cmd *cmd)
{
	t_redir	*current;

	if (!cmd || !cmd->redirs)
		return (0);
	current = cmd->redirs;
	while (current)
	{
		if (apply_redirection(current))
			return (1);
		current = current->next;
	}
	return (0);
}
