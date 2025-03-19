#include "minishell.h"

void	free_args_array(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}

char	**create_args_copy(t_ast *cmd, char **env_array, char *path)
{
	char	**args_copy;
	int		i;

	i = 0;
	while (cmd->cmd->args[i])
		i++;
	args_copy = malloc(sizeof(char *) * (i + 1));
	if (!args_copy)
	{
		free(path);
		free_env_array(env_array);
		exit(1);
	}
	return (args_copy);
}

void	copy_args(t_ast *cmd, char **args_copy, char **env_array, char *path)
{
	int	i;

	i = 0;
	while (cmd->cmd->args[i])
	{
		args_copy[i] = ft_strdup(cmd->cmd->args[i]);
		if (!args_copy[i])
		{
			while (--i >= 0)
				free(args_copy[i]);
			free(args_copy);
			free(path);
			free_env_array(env_array);
			exit(1);
		}
		i++;
	}
	args_copy[i] = NULL;
}

void	execute_command(char *path, char **args, char **env_array)
{
	int	i;

	if (execve(path, args, env_array) == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(args[0]);
		i = 0;
		while (args[i])
			free(args[i++]);
		free(args);
		free(path);
		free_env_array(env_array);
		exit(126);
	}
}

void	handle_command_not_found(t_ast *cmd, char **env_array)
{
	ft_putstr_fd("minishell: command not found: ", 2);
	ft_putstr_fd(cmd->cmd->args[0], 2);
	ft_putstr_fd("\n", 2);
	free_env_array(env_array);
	exit(127);
}

void	child_process(t_ast *cmd, t_env *env)
{
	char	**env_array;
	char	**args_copy;
	char	*path_copy;

	env_array = env_to_tab(&env);
	if (!env_array)
		exit(1);
	cmd->cmd->path = get_path(cmd->cmd->args[0], env_array);
	if (!cmd->cmd->path)
		handle_command_not_found(cmd, env_array);
	args_copy = create_args_copy(cmd, env_array, cmd->cmd->path);
	copy_args(cmd, args_copy, env_array, cmd->cmd->path);
	path_copy = ft_strdup(cmd->cmd->path);
	if (!path_copy)
	{
		free_args_array(args_copy);
		free(cmd->cmd->path);
		free_env_array(env_array);
		exit(1);
	}
	free_ast(cmd->root);
	free_env_list(env);
	execute_command(path_copy, args_copy, env_array);
}

int	parent_process(pid_t pid, t_ast *cmd)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
		cmd->error_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		cmd->error_code = 128 + WTERMSIG(status);
	return (cmd->error_code);
}

void	execute_cmd(t_ast *cmd_node, t_env *env)
{
	pid_t	pid;

	if (is_cmd_invalid(cmd_node))
		return ;
	pid = fork();
	if (pid == -1)
	{
		cmd_node->error_code = 1;
		return ;
	}
	if (pid == 0)
		child_process(cmd_node, env);
	else
		parent_process(pid, cmd_node);
}

void	execute_cmd_node(t_ast *node, t_env *env)
{
	check_builtin(node, env);
	if (node->error_code == -1)
		execute_cmd(node, env);
}

void	execute_ast(t_ast *node, t_env *env)
{
	static int	heredocs_processed = 0;

	if (!node)
		return ;
	if (!heredocs_processed)
	{
		process_all_heredocs(node);
		heredocs_processed = 1;
	}
	if (node->token == CMD)
		execute_cmd_node(node, env);
	else if (node->token == PIPE)
		execute_pipe(node, env);
	else if (node->token == REDIR_IN || node->token == REDIR_OUT
		|| node->token == APPEND || node->token == REDIR_HEREDOC)
		exec_with_redirects(node, env);
	if (node->head != node && node->head)
		node->head->error_code = node->error_code;
	if (node->head == node)
	{
		heredocs_processed = 0;
		cleanup_heredoc_files(node);
	}
}
