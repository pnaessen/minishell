#include "minishell.h"

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

int	apply_all_redirections(t_ast *node)
{
	if (!node)
		return (0);
	if (apply_redirection(node))
		return (1);
	return (0);
}
