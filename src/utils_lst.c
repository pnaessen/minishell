/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pn <pn@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:44:37 by pnaessen          #+#    #+#             */
/*   Updated: 2025/03/11 20:52:19 by pn               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_env(char **env, t_env **head)
{
	t_env	*new_node;
	int		i;

	i = 0;
	if (!*env)
		create_mini_env(head);
	while (env[i])
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return ;
		new_node->str = ft_strdup(env[i]);
		if (!new_node->str)
		{
			free(new_node);
			return ;
		}
		new_node->next = NULL;
		lstadd_back(head, new_node);
		i++;
	}
}

void	lstadd_back(t_env **head, t_env *new_node)
{
	t_env	*tmp;

	if (!head || !new_node)
		return ;
	if (!(*head))
		*head = new_node;
	else
	{
		tmp = *head;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new_node;
	}
}

void	free_env_list(t_env *env)
{
	t_env	*temp;

	while (env)
	{
		temp = env;
		env = env->next;
		if (temp->str)
			free(temp->str);
		free(temp);
	}
}

t_env	*create_env_var(char *var_string)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return (NULL);
	new_node->str = ft_strdup(var_string);
	if (!new_node->str)
	{
		free(new_node);
		return (NULL);
	}
	new_node->next = NULL;
	return (new_node);
}

void	add_pwd_to_env(t_env **head)
{
	t_env	*new_node;
	char	*path;
	char	*pwd_var;

	path = getcwd(NULL, PATH_MAX);
	if (!path)
		return ;
	pwd_var = ft_strjoin("PWD=", path);
	free(path);
	if (!pwd_var)
		return ;
	new_node = create_env_var(pwd_var);
	free(pwd_var);
	if (!new_node)
		return ;
	lstadd_back(head, new_node);
}

void	add_shlvl_to_env(t_env **head)
{
	t_env	*new_node;

	new_node = create_env_var("SHLVL=1");
	if (!new_node)
		return ;
	lstadd_back(head, new_node);
}

void	add_underscore_to_env(t_env **head)
{
	t_env	*new_node;

	new_node = create_env_var("_=/usr/bin/env");
	if (!new_node)
		return ;
	lstadd_back(head, new_node);
}

void	create_mini_env(t_env **head)
{
	add_pwd_to_env(head);
	add_shlvl_to_env(head);
	add_underscore_to_env(head);
}
