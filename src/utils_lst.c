/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_lst.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:44:37 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/18 14:45:04 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_env(char **env, t_env **head)
{
	t_env	*new_node;
	int		i;

	i = 0;
	while (env[i])
	{
		new_node = malloc(sizeof(t_env));
		if (!new_node)
			return ;
		new_node->str = env[i];
		new_node->next = NULL;
		lstadd_back(head, new_node);
		i++;
	}
}

void	lstadd_back(t_env **head, t_env *new_node)
{
	t_env	*tmp;

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

void	print_stack(t_env **head)
{
	t_env	*current;

	if (*head == NULL)
		return ;
	current = *head;
	while (current != NULL)
	{
		printf("node = %s\n", current->str);
		current = current->next;
	}
}
