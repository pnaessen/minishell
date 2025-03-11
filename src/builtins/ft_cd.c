/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 11:37:35 by pnaessen          #+#    #+#             */
/*   Updated: 2025/03/10 11:47:17 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*get_home_var(t_env *env)
{
	t_env	*temp;

	temp = env;
	while (temp)
	{
		if (ft_strncmp(temp->str, "HOME=", 5) == 0)
			return (temp->str + 5);
		temp = temp->next;
	}
	return (NULL);
}

int	env_var_exists(t_env *env, char *name)
{
	t_env	*temp;
	int		name_len;

	temp = env;
	name_len = ft_strlen(name);
	while (temp)
	{
		if (ft_strncmp(temp->str, name, name_len) == 0
			&& temp->str[name_len] == '=')
			return (1);
		temp = temp->next;
	}
	return (0);
}

void	add_to_env(t_env **env, char *new_str)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->str = new_str;
	new_node->next = NULL;
	lstadd_back(env, new_node);
}

void	set_env_var(t_env **env, char *name, char *value)
{
	t_env	*temp;
	char	*new_str;
	char	*name_equal;

	if (!name || !value || !env)
		return ;
	name_equal = ft_strjoin(name, "=");
	if (!name_equal)
		return ;
	temp = *env;
	while (temp)
	{
		if (ft_strncmp(temp->str, name_equal, ft_strlen(name_equal)) == 0)
		{
			free(temp->str);
			new_str = ft_strjoin(name_equal, value);
			temp->str = new_str;
			free(name_equal);
			if (!new_str)
				return ;
			temp->str = new_str;
			return ;
		}
		temp = temp->next;
	}
	new_str = ft_strjoin(name_equal, value);
	free(name_equal);
	if (!new_str)
		return ;
	add_to_env(env, new_str);
}

void	update_env_cd(t_ast *cmd, t_env **env, char *old_dir)
{
	char	*current_dir;

	current_dir = getcwd(NULL, PATH_MAX);
	if (!current_dir)
	{
		perror("minishell: getcwd");
		cmd->error_code = 1;
		free(old_dir);
		return ;
	}
	set_env_var(env, "PWD", current_dir);
	set_env_var(env, "OLDPWD", old_dir);
	free(current_dir);
	free(old_dir);
}

void	ft_cd(t_ast *cmd, t_env **env)
{
	char	*path;
	char	*old_dir;

	old_dir = getcwd(NULL, PATH_MAX);
	// if (!old_dir)
	// {
	// 	perror("minishell: getcwd");
	// 	cmd->error_code = 1;
	// 	return ;
	// }
	if (!cmd->cmd->args[1])
	{
		path = get_home_var(*env);
		if (!path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			cmd->error_code = 1;
			free(old_dir);
			return ;
		}
	}
	else
		path = cmd->cmd->args[1];
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		cmd->error_code = 1;
		free(old_dir);
		return ;
	}
	update_env_cd(cmd, env, old_dir);
	cmd->error_code = 0;
}
