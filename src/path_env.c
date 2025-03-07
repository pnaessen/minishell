/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pn <pn@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:31:37 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/27 21:40:25 by pn               ###   ########lyon.fr   */
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

char	*search_command_in_path(char *cmd, char **path_dirs)
{
	int		i;
	char	*temp;
	char	*full_path;

	i = 0;
	while (path_dirs[i])
	{
		temp = ft_strjoin(path_dirs[i], "/");
		if (!temp)
		{
			ft_free_ta(path_dirs);
			return (NULL);
		}
		full_path = ft_strjoin(temp, cmd);
		free(temp);
		if (!full_path)
		{
			ft_free_ta(path_dirs);
			return (NULL);
		}
		if (access(full_path, X_OK) == 0)
		{
			ft_free_ta(path_dirs);
			return (full_path);
		}
		free(full_path);
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

	i = 0;
	current = *env;
	while (current)
	{
		i++;
		current = current->next;
	}
	env_array = malloc(sizeof(char *) * (i + 1));
	if (!env_array)
		return (NULL);
	i = 0;
	current = *env;
	while (current)
	{
		env_array[i] = ft_strdup(current->str);
		if (!env_array[i])
		{
			while (i > 0)
				free(env_array[--i]);
			free(env_array);
			return (NULL);
		}
		i++;
		current = current->next;
	}
	env_array[i] = NULL;
	return (env_array);
}
