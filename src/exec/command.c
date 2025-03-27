#include "minishell.h"

char	**create_args_copy(t_ast *cmd, char **env_array)
{
	char	**args_copy;
	int		i;

	i = 0;
	while (cmd->cmd->args[i])
		i++;
	args_copy = malloc(sizeof(char *) * (i + 1));
	if (!args_copy)
	{
		perror("minishell: malloc args_copy");
		free_ast(cmd->root);
		free_env_array(env_array);
		exit(1);
	}
	return (args_copy);
}

void	copy_args(t_ast *cmd, char **args_copy, char **env_array)
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
			free_ast(cmd->root);
			free_env_array(env_array);
			perror("minishell: strdup copy_args");
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
	if (cmd->root && cmd->root->garbage)
		clean_fd_garbage(&cmd->root->garbage);
	ft_putstr_fd("minishell: command not found: ", 2);
	ft_putstr_fd(cmd->cmd->args[0], 2);
	ft_putstr_fd("\n", 2);
	free_ast(cmd->root);
	free_env_array(env_array);
	exit(127);
}

void	child_process(t_ast *cmd, t_env *env)
{
	char	**env_array;
	char	**args_copy;
	char	*path_copy;

	handle_signals_child();
	env_array = env_to_tab(&env);
	if (!env_array)
	{
		free_env_list(env);
		free_ast(cmd->root);
		exit(1);
	}
	free_env_list(env);
	cmd->cmd->path = get_path(cmd->cmd->args[0], env_array);
	if (!cmd->cmd->path)
		handle_command_not_found(cmd, env_array);
	args_copy = create_args_copy(cmd, env_array);
	copy_args(cmd, args_copy, env_array);
	path_copy = ft_strdup(cmd->cmd->path);
	if (!path_copy)
	{
		free_args_array(args_copy);
		free_ast(cmd->root);
		free_env_array(env_array);
		exit(1);
	}
	// for (int i = 3; i < 1024; i++)
	// 	close(i);
	if (cmd->root && cmd->root->garbage)
		clean_fd_garbage(&cmd->root->garbage);
	free_ast(cmd->root);
	execute_command(path_copy, args_copy, env_array);
}
