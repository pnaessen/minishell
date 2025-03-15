#include "minishell.h"
#include "pars.h"

void	exec_with_redirects(t_ast *node, t_env *env)
{
	int	saved_stdin;
	int	saved_stdout;

	if (save_std_fds(&saved_stdin, &saved_stdout, node))
		return ;
	if (apply_all_redirections(node))
	{
		node->error_code = 1;
		restore_std_fds(saved_stdin, saved_stdout);
		return ;
	}
	execute_ast(node->left, env);
	// check_builtin(node, env);
	restore_std_fds(saved_stdin, saved_stdout); // not sure
}

int	apply_all_redirections(t_ast *node)
{
	if (!node)
		return (0);
	if (apply_redirection(node))
		return (1);
	return (0);
}

int	apply_redirection(t_ast *redir)
{
	if (redir->token == REDIR_IN || redir->token == REDIR_HEREDOC)
		return (apply_input_redirection(redir));
	else if (redir->token == REDIR_OUT)
		return (apply_output_redirection(redir));
	else if (redir->token == APPEND)
		return (apply_append_redirection(redir));
	return (0);
}

int	apply_input_redirection(t_ast *redir)
{
	int	fd;

	fd = open(redir->cmd->args[0], O_RDONLY);
	if (fd == -1)
	{
		perror("minishell: open");
		return (1);
	}
	if (dup2(fd, STDIN_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_output_redirection(t_ast *redir)
{
	int	fd;

	fd = open(redir->cmd->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("minishell: open");
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}

int	apply_append_redirection(t_ast *redir)
{
	int	fd;

	fd = open(redir->cmd->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (fd == -1)
	{
		perror("minishell: open");
		return (1);
	}
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2");
		close(fd);
		return (1);
	}
	close(fd);
	return (0);
}
