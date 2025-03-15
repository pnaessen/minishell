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

	if (is_cmd_invalid(cmd_node))
		return ;
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
		child_process(cmd_node, env_array);
	else
		parent_process(pid, cmd_node, env_array);
}

void	execute_cmd_node(t_ast *node, t_env *env)
{

	check_builtin(node, env);
	if (node->error_code == -1)
		execute_cmd(node, env);
}

void	execute_ast(t_ast *node, t_env *env)
{
	static int	heredocs_processed = 0;

	if (!node)
		return ;
	if (!heredocs_processed)
	{
		process_all_heredocs(node);
		heredocs_processed = 1;
	}
	if (node->token == CMD)
		execute_cmd_node(node, env);
	else if (node->token == PIPE)
		execute_pipe(node, env);
	else if (node->token == REDIR_IN || node->token == REDIR_OUT
		|| node->token == APPEND)
		exec_with_redirects(node, env);
	if (node->head != node && node->head)
		node->head->error_code = node->error_code;
	if (node->head == node)
	{
		heredocs_processed = 0;
		cleanup_heredoc_files(node);
	}
}
