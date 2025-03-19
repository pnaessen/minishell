#include "minishell.h"

void	setup_pipe_right(int *pipefd)
{
	close(pipefd[1]);
	if (dup2(pipefd[0], STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(pipefd[0]);
		exit(1);
	}
	close(pipefd[0]);
}

int	get_right_exit_code(t_ast *cmd)
{
	int	exit_code;

	exit_code = cmd->right->error_code;
	if (exit_code == -1)
		exit_code = cmd->error_code;
	return (exit_code);
}

void	pipe_child_right(t_ast *cmd, t_env *env, int *pipefd)
{
	int	exit_code;

	setup_pipe_right(pipefd);
	execute_ast(cmd->right, env);
	exit_code = get_right_exit_code(cmd);
	clean_and_exit(cmd, env, exit_code);
}
