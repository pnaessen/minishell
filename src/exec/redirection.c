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
	//add_fd_to_garbage(&redir->root->garbage, fd);
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
	//add_fd_to_garbage(&redir->root->garbage, fd);
	if (dup2(fd, STDOUT_FILENO) == -1)
	{
		perror("minishell: dup2\n");
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
	//add_fd_to_garbage(&redir->root->garbage, fd);
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
		
	if (node->left && node->left->token != CMD)
	{
		if (apply_all_redirections(node->left))
			return (1);
	}
	if (is_redirection(node->token))
	{
		if (apply_redirection(node))
			return (1);
	}
	
	return (0);
}

void	cleanup_heredoc_files(t_ast *node)
{
	if (!node)
		return ;
	if (node->token == REDIR_HEREDOC && node->cmd && node->cmd->args)
	{
		if (node->cmd->args[0])
			unlink(node->cmd->args[0]);
	}
	if (node->left)
		cleanup_heredoc_files(node->left);
	if (node->right)
		cleanup_heredoc_files(node->right);
}
