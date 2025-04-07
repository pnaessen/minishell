#include "minishell.h"

int	handle_first_fork(t_ast *cmd, t_env **env, int pipefd[2], pid_t *pid1)
{
	if (!can_create_process(env))
	{
		process_finished(env);
		cleanup_pipe(pipefd);
		cmd->error_code = 1;
		return (0);
	}
	*pid1 = fork();
	if (*pid1 == -1)
	{
		fork_fail(&cmd, pipefd);
		return (0);
	}
	if (*pid1 == 0)
		pipe_child_left(cmd, env, pipefd);
	return (1);
}

int	check_second_process(t_ast *cmd, t_env **env, int pipefd[2], pid_t pid1)
{
	if (!can_create_process(env))
	{
		cleanup_pipe(pipefd);
		waitpid(pid1, NULL, 0);
		process_finished(env);
		cmd->error_code = 1;
		return (0);
	}
	return (1);
}

int	create_second_fork(t_ast *cmd, t_env **env, int pipefd[2], pid_t pid1)
{
	pid_t	pid2;

	pid2 = fork();
	if (pid2 == -1)
	{
		process_finished(env);
		fork_fail(&cmd, pipefd);
		waitpid(pid1, NULL, 0);
		process_finished(env);
		return (0);
	}
	if (pid2 == 0)
		pipe_child_right(cmd, env, pipefd);
	handle_pipe_parent(cmd, pid1, pid2, pipefd);
	return (1);
}

void	execute_pipe(t_ast *cmd, t_env **env)
{
	int		pipefd[2];
	pid_t	pid1;

	if (!setup_pipe(cmd, pipefd))
		return ;
	if (!handle_first_fork(cmd, env, pipefd, &pid1))
		return ;
	if (!check_second_process(cmd, env, pipefd, pid1))
		return ;
	if (!create_second_fork(cmd, env, pipefd, pid1))
		return ;
	process_finished(env);
	process_finished(env);
}

void	handle_pipe_parent(t_ast *cmd, pid_t pid1, pid_t pid2, int *pipefd)
{
	int	status;
	int	status2;

	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	if (close(pipefd[0]) == -1)
		perror("minishell: close");
	if (close(pipefd[1]) == -1)
		perror("minishell: close");
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
