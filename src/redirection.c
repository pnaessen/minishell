#include "minishell.h"

int	save_std_fds(int *saved_stdin, int *saved_stdout, t_ast *node)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
	{
		perror("minishell: dup");
		node->error_code = 1;
		if (*saved_stdin != -1)
			close(*saved_stdin);
		if (*saved_stdout != -1)
			close(*saved_stdout);
		return (1);
	}
	return (0);
}

void	restore_std_fds(int saved_stdin, int saved_stdout)
{
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
}

void	exec_with_redirects(t_ast *node, t_env *env)
{
	int	saved_stdin;
	int	saved_stdout;

	if (save_std_fds(&saved_stdin, &saved_stdout, node))
		return ;
	if (apply_all_redirections(node->cmd))
	{
		node->error_code = 1;
		restore_std_fds(saved_stdin, saved_stdout);
		return ;
	}
	check_builtin(node, env);
	restore_std_fds(saved_stdin, saved_stdout);
	if (node->error_code == -1)
		execute_cmd(node, env);
}

int	is_cmd_invalid(t_ast *cmd_node)
{
	if (!cmd_node || !cmd_node->cmd || !cmd_node->cmd->args
		|| !cmd_node->cmd->args[0])
	{
		cmd_node->error_code = 1;
		return (1);
	}
	return (0);
}

int	handle_child_redirects(t_ast *cmd_node, char **env_array)
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
	return (0);
}
