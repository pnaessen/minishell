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
		//kill(pid1, SIGTERM);
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

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	close(pipefd[0]);
	close(pipefd[1]);
	waitpid(pid1, &status, 0);
	waitpid(pid2, &status2, 0);
	handle_signals();
	if (WIFEXITED(status2))
		cmd->error_code = WEXITSTATUS(status2);
	else if (WIFSIGNALED(status2))
	{
		cmd->error_code = 128 + WTERMSIG(status2);
		if (WTERMSIG(status2) == SIGINT)
			ft_putchar_fd('\n', STDOUT_FILENO);
		else if (WTERMSIG(status2) == SIGQUIT)
			ft_putstr_fd("Quit (core dumped)\n", STDOUT_FILENO);
	}
}

void	fork_fail(t_ast **cmd, int *pipefd)
{
	perror("minishell: fork");
	close(pipefd[0]);
	close(pipefd[1]);
	(*cmd)->error_code = 1;
}
