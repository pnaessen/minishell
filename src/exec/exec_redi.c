#include "minishell.h"
#include "pars.h"

void	exec_with_redirects(t_ast *node, t_env *env)
{
	int		saved_stdin;
	int		saved_stdout;
	t_ast	*cmd_node;
	int		error;

	if (save_std_fds(&saved_stdin, &saved_stdout, node))
		return ;
	if (apply_all_redirections(node))
	{
		node->error_code = 1;
		restore_std_fds(saved_stdin, saved_stdout);
		return ;
	}
	cmd_node = find_cmd_node(node, &error);
	if (error || !cmd_node)
	{
		node->error_code = 1;
		restore_std_fds(saved_stdin, saved_stdout);
		return ;
	}
	execute_cmd_with_redir(cmd_node, node, env);
	 restore_std_fds(saved_stdin, saved_stdout);
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

t_ast	*find_cmd_node(t_ast *node, int *has_error)
{
	*has_error = 0;
	if (!node)
	{
		*has_error = 1;
		return (NULL);
	}
	if (node->token == CMD)
		return (node);
	if (node->left)
	{
		if (node->left->token == CMD)
			return (node->left);
		else
			return (find_cmd_node(node->left, has_error));
	}
	*has_error = 1;
	return (NULL);
}

void	execute_cmd_with_redir(t_ast *cmd_node, t_ast *node, t_env *env)
{
	if (!cmd_node || cmd_node->token != CMD)
		return ;
	check_builtin(cmd_node, env);
	if (cmd_node->error_code == -1)
		execute_cmd(cmd_node, env);
	node->error_code = cmd_node->error_code;
}
