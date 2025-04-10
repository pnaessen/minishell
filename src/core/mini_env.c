/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:03:55 by vicperri          #+#    #+#             */
/*   Updated: 2025/04/10 15:03:56 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	new_node->process_count = 0;
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
	if (*head)
		(*head)->process_count = 0;
}
