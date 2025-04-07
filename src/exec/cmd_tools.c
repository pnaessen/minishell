#include "minishell.h"

void	handle_path_error(t_ast *cmd, char **env_array)
{
	free_args_array(cmd->cmd->args);
	free_ast(cmd->root);
	free_env_array(env_array);
	perror("minishell: strdup path");
	exit(1);
}

char	*prepare_command_path(t_ast *cmd, char **env_array)
{
	char	*path_copy;

	cmd->cmd->path = get_path(cmd->cmd->args[0], env_array);
	if (!cmd->cmd->path)
		handle_command_not_found(cmd, env_array);
	path_copy = ft_strdup(cmd->cmd->path);
	if (!path_copy)
		handle_path_error(cmd, env_array);
	return (path_copy);
}

char	**prepare_command_args(t_ast *cmd, char **env_array)
{
	char	**args_copy;

	args_copy = create_args_copy(cmd, env_array);
	copy_args(cmd, args_copy, env_array);
	return (args_copy);
}

void	prepare_execution(t_ast *cmd, char **env_array, char **args_copy,
		char *path_copy)
{
	if (cmd->root && cmd->root->garbage)
		clean_fd_garbage(&cmd->root->garbage);
	free_ast(cmd->root);
	execute_command(path_copy, args_copy, env_array);
}

void	child_process(t_ast *cmd, t_env **env)
{
	char	**env_array;
	char	**args_copy;
	char	*path_copy;

	handle_signals_child();
	env_array = env_to_tab(env);
	if (!env_array)
	{
		free_env_list(*env);
		free_ast(cmd->root);
		exit(1);
	}
	free_env_list(*env);
	path_copy = prepare_command_path(cmd, env_array);
	args_copy = prepare_command_args(cmd, env_array);
	prepare_execution(cmd, env_array, args_copy, path_copy);
}
