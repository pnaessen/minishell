/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 13:31:37 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/20 11:25:32 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_path_var(char **env)
{
	int	i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "PATH=", 5) == 0)
			return (env[i] + 5);
		i++;
	}
	return (NULL);
}

char	*get_path(char *cmd, char **env)
{
	char	**paths;
	char	*path_var;
	char	*full_path;
	int		i;

	path_var = get_path_var(env);
	if (!path_var)
		return (NULL);
	paths = ft_split(path_var, ':');
	if (!path_var || !paths)
		return (NULL);
	i = 0;
	while (paths[i])
	{
		full_path = find_command_path(paths, cmd);
		if (full_path && access(full_path, F_OK) == 0)
		{
			ft_free(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	ft_free(paths);
	return (NULL);
}

char	*find_command_path(char **paths, char *cmd)
{
	char	*path;
	char	*full_path;
	int		i;

	if (!cmd || !paths)
		return (NULL);
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, F_OK) == 0)
			return (cmd);
	}
	i = 0;
	while (paths[i])
	{
		path = ft_strjoin(paths[i], "/");
		full_path = ft_strjoin(path, cmd);
		free(path);
		if (access(full_path, X_OK) == 0)
			return (full_path);
		free(full_path);
		i++;
	}
	return (NULL);
}

void	env_to_tab(t_env **env)
{
	t_env *tmp;
	char **envp;
	int i;

	tmp = *env;
	i = 0;
	while (tmp)
	{
		i++;
		tmp = tmp->next;
	}
	if (*env)
		free(env);
	envp = malloc(sizeof(char *) * (i + 1));
	if (!envp)
		return ;
}