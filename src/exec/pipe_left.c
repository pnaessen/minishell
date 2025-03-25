#include "minishell.h"

void	setup_pipe_left(int *pipefd)
{
	close(pipefd[0]);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(pipefd[1]);
		exit(1);
	}
	close(pipefd[1]);
}

int	get_exit_code(t_ast *cmd)
{
	if (cmd->left->error_code != -1)
		return (cmd->left->error_code);
	return (cmd->error_code);
}

void	clean_and_exit(t_ast *cmd, t_env *env, int exit_code)
{
	if (cmd->root && cmd->root->garbage)
		clean_fd_garbage(&cmd->root->garbage);
	free_ast(cmd->root);
	free_env_list(env);
	exit(exit_code);
}

void	pipe_child_left(t_ast *cmd, t_env *env, int *pipefd)
{
	int	exit_code;

	handle_signals_child();
	setup_pipe_left(pipefd);
	if (cmd->left->token == CMD)
	{
		check_builtin(cmd->left, env);
		if (cmd->left->error_code == -1)
			execute_ast(cmd->left, env);
	}
	else
		execute_ast(cmd->left, env);
	exit_code = get_exit_code(cmd);
	clean_and_exit(cmd, env, exit_code);
}
