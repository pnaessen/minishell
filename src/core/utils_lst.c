/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:44:37 by pnaessen          #+#    #+#             */
/*   Updated: 2025/03/12 10:58:07 by pnaessen         ###   ########lyon.fr   */
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
