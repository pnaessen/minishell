/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 11:25:46 by pnaessen          #+#    #+#             */
/*   Updated: 2025/03/06 11:43:11 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_cmd(t_ast *cmd, t_env *env)
{
	pid_t	pid;
	char	**env_array;

	env_array = env_to_tab(&env);
	if (!env_array)
	{
		cmd->error_code = 1;
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		ft_free_ta(env_array);
		cmd->error_code = 1;
		return ;
	}
	if (pid == 0)
		child_process(cmd, env_array);
	parent_process(pid, cmd, env_array);
}

void	child_process(t_ast *cmd, char **env_array)
{
	cmd->cmd->path = get_path(cmd->cmd->args[0], env_array);
	if (!cmd->cmd->path)
	{
		ft_putstr_fd("minishell: command not found: ", 2);
		ft_putstr_fd(cmd->cmd->args[0], 2);
		ft_putstr_fd("\n", 2);
		free_env_array(env_array);
		exit(127);
	}
	if (execve(cmd->cmd->path, cmd->cmd->args, env_array) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(cmd->cmd->args[0]);
		free(cmd->cmd->path);
		cmd->cmd->path = NULL;
		free_env_array(env_array);
		exit(126);
	}
}

int	parent_process(pid_t pid, t_ast *cmd, char **env_array)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		cmd->error_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		cmd->error_code = 128 + WTERMSIG(status);
	free_env_array(env_array);
	return (cmd->error_code);
}

void	execute_ast(t_ast *cmd, t_env *env)
{
	if (!cmd) // add un if only space
	return ;
	if (cmd->token == CMD)
	{
		check_builtin(cmd, env);
		if (cmd->error_code == -1)
			execute_cmd(cmd, env);
	}
	else if (cmd->token == PIPE)
		execute_pipe(cmd, env);
	else if (cmd->token == REDIR_IN)
		handle_redir_in(cmd, env);
	else if (cmd->token == REDIR_OUT)
		handle_redir_out(cmd, env);
	else if (cmd->token == APPEND)
		handle_redir_append(cmd, env);
	else if (cmd->token == REDIR_HEREDOC)
		handle_heredoc(cmd, env);
}
