#include "minishell.h"

int	parent_process(pid_t pid, t_ast *cmd)
{
	int	status;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, &status, 0);
	handle_signals();
	if (WIFEXITED(status))
		cmd->error_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		cmd->error_code = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			ft_putchar_fd('\n', STDOUT_FILENO);
		else if (WTERMSIG(status) == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
	}
	return (cmd->error_code);
}

void	execute_cmd(t_ast *cmd_node, t_env *env)
{
	pid_t	pid;

	if (is_cmd_invalid(cmd_node))
		return ;
	if (!can_create_process(env))
	{
		cmd_node->error_code = 1;
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		process_finished(env);
		cmd_node->error_code = 1;
		return ;
	}
	if (pid == 0)
		child_process(cmd_node, env);
	else
	{
		parent_process(pid, cmd_node);
		process_finished(env);
	}
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
	int			tmp;

	if (!node)
		return ;
	if (!heredocs_processed)
	{
		tmp = process_all_heredocs(node);
		heredocs_processed = 1;
		if (tmp == 0)
		{
			env->error_code = 130;
			heredocs_processed = 0;
			cleanup_heredoc_files(node);
			clean_fd_garbage(&node->garbage);
			return ;
		}
	}
	if (node->token == CMD)
		execute_cmd_node(node, env);
	else if (node->token == PIPE)
		execute_pipe(node, env);
	else if (node->token == REDIR_IN || node->token == REDIR_OUT
		|| node->token == APPEND || node->token == REDIR_HEREDOC)
		exec_with_redirects(node, env);
	if (node->head != node && node->head)
		node->head->error_code = node->error_code;
	env->error_code = node->error_code;
	if (node->head == node)
	{
		heredocs_processed = 0;
		cleanup_heredoc_files(node);
		clean_fd_garbage(&node->garbage);
	}
}
