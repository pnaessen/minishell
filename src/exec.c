
#include "minishell.h"

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
void	execute_cmd(t_ast *cmd_node, t_env *env)
{
	pid_t	pid;
	char	**env_array;

	if (!cmd_node || !cmd_node->cmd || !cmd_node->cmd->args
		|| !cmd_node->cmd->args[0])
	{
		cmd_node->error_code = 1;
		return ;
	}
	env_array = env_to_tab(&env);
	if (!env_array)
	{
		cmd_node->error_code = 1;
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		ft_free_ta(env_array);
		cmd_node->error_code = 1;
		return ;
	}
	if (pid == 0)
	{
		if (cmd_node->cmd->redirs)
		{
			if (apply_all_redirections(cmd_node->cmd))
			{
				ft_free_ta(env_array);
				exit(1);
			}
		}
		child_process(cmd_node, env_array);
	}
	else
		parent_process(pid, cmd_node, env_array);
}

void	execute_ast(t_ast *node, t_env *env)
{
	int	saved_stdin;
	int	saved_stdout;

	if (!node)
		return ;
	if (node->token == CMD)
	{
		if (node->cmd->redirs)
		{
			saved_stdin = dup(STDIN_FILENO);
			saved_stdout = dup(STDOUT_FILENO);
			if (saved_stdin == -1 || saved_stdout == -1)
			{
				perror("minishell: dup");
				node->error_code = 1;
				if (saved_stdin != -1)
					close(saved_stdin);
				if (saved_stdout != -1)
					close(saved_stdout);
				return ;
			}
			if (apply_all_redirections(node->cmd))
			{
				node->error_code = 1;
				dup2(saved_stdin, STDIN_FILENO);
				dup2(saved_stdout, STDOUT_FILENO);
				close(saved_stdin);
				close(saved_stdout);
				return ;
			}
			check_builtin(node, env);
			if (node->error_code == -1)
			{
				dup2(saved_stdin, STDIN_FILENO);
				dup2(saved_stdout, STDOUT_FILENO);
				execute_cmd(node, env);
			}
			dup2(saved_stdin, STDIN_FILENO);
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdin);
			close(saved_stdout);
		}
		else
		{
			check_builtin(node, env);
			if (node->error_code == -1)
				execute_cmd(node, env);
		}
	}
	else if (node->token == PIPE)
		execute_pipe(node, env);
	else if (node->token == REDIR_IN)
		handle_redir_in(node, env);
	else if (node->token == REDIR_OUT)
		handle_redir_out(node, env);
	else if (node->token == APPEND)
		handle_redir_append(node, env);
	else if (node->token == REDIR_HEREDOC)
		handle_heredoc(node, env);
}
