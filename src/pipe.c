#include "minishell.h"

void	pipe_child_left(t_ast *cmd, t_env *env, int *pipefd)
{
	t_env	*env_copy;

	close(pipefd[0]);
	if (dup2(pipefd[1], STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(pipefd[1]);
		exit(1);
	}
	close(pipefd[1]);
	if (cmd->left->token == CMD && cmd->left->cmd && cmd->left->cmd->redirs)
	{
		//if (apply_all_redirections(cmd->left->cmd)) // modif
		//	exit(1);
	}
	env_copy = env;
	execute_ast(cmd->left, env_copy);
	exit(cmd->error_code);
}

void	pipe_child_right(t_ast *cmd, t_env *env, int *pipefd)
{
	t_env	*env_copy;
	int exit_code;

	close(pipefd[1]);
	if (cmd->right->token == CMD && cmd->right->cmd
		&& !cmd->right->cmd->has_heredoc)
	{
		if (dup2(pipefd[0], STDIN_FILENO) == -1)
		{
			perror("minishell: dup2");
			close(pipefd[0]);
			exit(1);
		}
	}
	close(pipefd[0]);
	env_copy = env;
	execute_ast(cmd->right, env_copy);
	exit_code = cmd->right->error_code;
	if(exit_code == -1)
		exit_code = cmd->error_code;
	exit(cmd->error_code);
}

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