/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/23 10:12:14 by pn                #+#    #+#             */
/*   Updated: 2025/03/04 14:35:07 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	pipe_child_left(t_ast *cmd, t_env *env, int *pipefd)
{
	t_env	*env_copy;

	close(pipefd[0]);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(pipefd[1]);
		exit(1);
	}
	close(pipefd[1]);
	env_copy = env;
	execute_ast(cmd->left, env_copy);
	exit(cmd->error_code);
}

void	pipe_child_right(t_ast *cmd, t_env *env, int *pipefd)
{
	t_env	*env_copy;

	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(pipefd[0]);
		exit(1);
	}
	close(pipefd[0]);
	env_copy = env;
	execute_ast(cmd->right, env_copy);
	exit(cmd->error_code);
}

void	execute_pipe(t_ast *cmd, t_env *env)
{
	int		pipefd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (pipe(pipefd) == -1)
	{
		perror("minishell: pipe");
		cmd->error_code = 1;
		return ;
	}
	pid1 = fork();
	if (pid1 == -1)
	{
		perror("minishell: fork");
		close(pipefd[0]);
		close(pipefd[1]);
		cmd->error_code = 1;
		return ;
	}
	if (pid1 == 0)
		pipe_child_left(cmd, env, pipefd);
	pid2 = fork();
	if (pid2 == -1)
	{
		perror("minishell: fork");
		kill(pid1, SIGTERM);
		close(pipefd[0]);
		close(pipefd[1]);
		cmd->error_code = 1;
		return ;
	}
	if (pid2 == 0)
		pipe_child_right(cmd, env, pipefd);
	handle_pipe_parent(cmd, pid1, pid2, pipefd);
}

void	handle_pipe_parent(t_ast *cmd, pid_t pid1, pid_t pid2, int *pipefd)
{
	int	status;

	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status, 0);
	if (WIFEXITED(status))
		cmd->error_code = WEXITSTATUS(status);
}
