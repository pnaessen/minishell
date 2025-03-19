// #include "minishell.h"

// int	apply_input_redirection(t_redir *redir)
// {
// 	int	fd;

// 	fd = open(redir->file, O_RDONLY);
// 	if (fd == -1)
// 	{
// 		perror("minishell: open");
// 		return (1);
// 	}
// 	if (dup2(fd, STDIN_FILENO) == -1)
// 	{
// 		perror("minishell: dup2");
// 		close(fd);
// 		return (1);
// 	}
// 	close(fd);
// 	return (0);
// }

// int	apply_output_redirection(t_redir *redir)
// {
// 	int	fd;

// 	fd = open(redir->file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd == -1)
// 	{
// 		perror("minishell: open");
// 		return (1);
// 	}
// 	if (dup2(fd, STDOUT_FILENO) == -1)
// 	{
// 		perror("minishell: dup2");
// 		close(fd);
// 		return (1);
// 	}
// 	close(fd);
// 	return (0);
// }

// int	apply_append_redirection(t_redir *redir)
// {
// 	int	fd;

// 	fd = open(redir->file, O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	if (fd == -1)
// 	{
// 		perror("minishell: open");
// 		return (1);
// 	}
// 	if (dup2(fd, STDOUT_FILENO) == -1)
// 	{
// 		perror("minishell: dup2");
// 		close(fd);
// 		return (1);
// 	}
// 	close(fd);
// 	return (0);
// }

// int	apply_redirection(t_redir *redir)
// {
// 	if (redir->type == REDIR_IN || redir->type == REDIR_HEREDOC)
// 		return (apply_input_redirection(redir));
// 	else if (redir->type == REDIR_OUT)
// 		return (apply_output_redirection(redir));
// 	else if (redir->type == APPEND)
// 		return (apply_append_redirection(redir));
// 	return (0);
// }

// int	apply_all_redirections(t_cmd *cmd)
// {
// 	t_redir	*current;

// 	if (!cmd || !cmd->redirs)
// 		return (0);
// 	current = cmd->redirs;
// 	while (current)
// 	{
// 		if (apply_redirection(current))
// 			return (1);
// 		current = current->next;
// 	}
// 	return (0);
// }
