// #include "minishell.h"

// void	handle_redir_in(t_ast *cmd, t_env *env)
// {
// 	int	fd;
// 	int	saved_fd;

// 	saved_fd = dup(STDIN_FILENO);
// 	if (saved_fd == -1)
// 	{
// 		perror("minishell: dup");
// 		cmd->error_code = 1;
// 		return ;
// 	}
// 	fd = open(cmd->right->cmd->args[0], O_RDONLY);
// 	if (fd == -1)
// 	{
// 		perror("minishell: open");
// 		cmd->error_code = 1;
// 		close(saved_fd);
// 		return ;
// 	}
// 	if (dup2(fd, STDIN_FILENO) == -1)
// 	{
// 		perror("minishell: dup2");
// 		cmd->error_code = 1;
// 		close(fd);
// 		close(saved_fd);
// 		return ;
// 	}
// 	close(fd);
// 	execute_ast(cmd->left, env);
// 	if (dup2(saved_fd, STDIN_FILENO) == -1)
// 		perror("minishell: dup2");
// 	close(saved_fd);
// }

// void	handle_redir_out(t_ast *cmd, t_env *env)
// {
// 	int	fd;
// 	int	saved_fd;

// 	saved_fd = dup(STDOUT_FILENO);
// 	if (saved_fd == -1)
// 	{
// 		perror("minishell: dup");
// 		cmd->error_code = 1;
// 		return ;
// 	}
// 	fd = open(cmd->right->cmd->args[0], O_WRONLY | O_CREAT | O_TRUNC, 0644);
// 	if (fd == -1)
// 	{
// 		perror("minishell: open");
// 		cmd->error_code = 1;
// 		close(saved_fd);
// 		return ;
// 	}
// 	if (dup2(fd, STDOUT_FILENO) == -1)
// 	{
// 		perror("minishell: dup2");
// 		cmd->error_code = 1;
// 		close(fd);
// 		close(saved_fd);
// 		return ;
// 	}
// 	close(fd);
// 	execute_ast(cmd->left, env);
// 	if (dup2(saved_fd, STDOUT_FILENO) == -1)
// 		perror("minishell: dup2");
// 	close(saved_fd);
// }

// void	handle_redir_append(t_ast *cmd, t_env *env)
// {
// 	int	fd;
// 	int	saved_fd;

// 	saved_fd = dup(STDOUT_FILENO);
// 	if (saved_fd == -1)
// 	{
// 		perror("minishell: dup");
// 		cmd->error_code = 1;
// 		return ;
// 	}
// 	fd = open(cmd->right->cmd->args[0], O_WRONLY | O_CREAT | O_APPEND, 0644);
// 	if (fd == -1)
// 	{
// 		perror("minishell: open");
// 		cmd->error_code = 1;
// 		close(saved_fd);
// 		return ;
// 	}
// 	if (dup2(fd, STDOUT_FILENO) == -1)
// 	{
// 		perror("minishell: dup2");
// 		cmd->error_code = 1;
// 		close(fd);
// 		close(saved_fd);
// 		return ;
// 	}
// 	close(fd);
// 	execute_ast(cmd->left, env);
// 	if (dup2(saved_fd, STDOUT_FILENO) == -1)
// 		perror("minishell: dup2");
// 	close(saved_fd);
// }
