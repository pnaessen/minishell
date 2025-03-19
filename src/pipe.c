#include "minishell.h"

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
		fork_fail(&cmd, pipefd);
		return ;
	}
	if (pid1 == 0)
		pipe_child_left(cmd, env, pipefd);
	pid2 = fork();
	if (pid2 == -1)
	{
		fork_fail(&cmd, pipefd);
		kill(pid1, SIGTERM);
		return ;
	}
	if (pid2 == 0)
		pipe_child_right(cmd, env, pipefd);
	handle_pipe_parent(cmd, pid1, pid2, pipefd);
}

void	handle_pipe_parent(t_ast *cmd, pid_t pid1, pid_t pid2, int *pipefd)
{
	int	status;
	int	status2;

	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status2, 0);
	if (WIFEXITED(status2))
		cmd->error_code = WEXITSTATUS(status2);
	else if (WIFSIGNALED(status2))
		cmd->error_code = 128 + WTERMSIG(status2);
}

void	fork_fail(t_ast **cmd, int *pipefd)
{
	perror("minishell: fork");
	close(pipefd[0]);
	close(pipefd[1]);
	(*cmd)->error_code = 1;
}
