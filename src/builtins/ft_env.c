/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 12:25:47 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/20 12:27:01 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	print_env(t_env **head, t_ast *cmd)
{
	t_env *current;

	if (*head == NULL)
	{
		printf("env: environment not set\n");
		cmd->error_code = 1;
		return (1);
	}
	current = *head;
	while (current != NULL)
	{
		printf("%s\n", current->str);
		current = current->next;
		cmd->error_code = 0;
	}
	return (0);
}