/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:04:00 by vicperri          #+#    #+#             */
/*   Updated: 2025/04/10 15:04:01 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path(char *cmd, char **env_array)
{
	int	i;

	if (!cmd || !*cmd)
		return (NULL);
	i = 0;
	while (cmd[i] && cmd[i] == ' ')
		i++;
	if (!cmd[i])
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == -1)
			return (NULL);
		if (access(cmd, X_OK) == -1)
			return (NULL);
		return (ft_strdup(cmd));
	}
	return (find_in_path(cmd, env_array));
}

char	*find_in_path(char *cmd, char **env_array)
{
	char	*path_var;
	char	**path_dirs;
	int		i;

	i = 0;
	path_var = NULL;
	while (env_array && env_array[i])
	{
		if (ft_strncmp(env_array[i], "PATH=", 5) == 0)
		{
			path_var = env_array[i] + 5;
			break ;
		}
		i++;
	}
	if (!path_var)
		return (NULL);
	path_dirs = ft_split(path_var, ':');
	if (!path_dirs)
		return (NULL);
	return (search_command_in_path(cmd, path_dirs));
}

char	*try_command_path(char *dir_path, char *cmd)
{
	char	*temp;
	char	*full_path;

	temp = ft_strjoin(dir_path, "/");
	if (!temp)
		return (NULL);
	full_path = ft_strjoin(temp, cmd);
	free(temp);
	if (!full_path)
		return (NULL);
	if (access(full_path, F_OK | X_OK) == 0)
		return (full_path);
	free(full_path);
	return (NULL);
}

char	*search_command_in_path(char *cmd, char **path_dirs)
{
	int		i;
	char	*result;

	i = 0;
	while (path_dirs[i])
	{
		result = try_command_path(path_dirs[i], cmd);
		if (!result && path_dirs[i + 1])
		{
			i++;
			continue ;
		}
		if (result)
		{
			ft_free_ta(path_dirs);
			return (result);
		}
		i++;
	}
	ft_free_ta(path_dirs);
	return (NULL);
}

char	**env_to_tab(t_env **env)
{
	t_env	*current;
	char	**env_array;
	int		i;
	int		count;

	count = count_env_nodes(*env);
	env_array = malloc(sizeof(char *) * (count + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	current = *env;
	while (current)
	{
		env_array[i] = ft_strdup(current->str);
		if (!env_array[i])
		{
			free_env_fail(env_array, i);
			return (NULL);
		}
		i++;
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
