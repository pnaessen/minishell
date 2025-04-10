/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_redi.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:00:32 by pnaessen          #+#    #+#             */
/*   Updated: 2025/04/10 15:00:34 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_stack	*find_right_side_end(t_stack *next_token, t_stack *stack)
{
	t_stack	*temp;
	t_stack	*end_of_right;

	end_of_right = stack;
	temp = next_token;
	while (temp != stack)
	{
		if (temp->token == PIPE)
		{
			end_of_right = temp;
			break ;
		}
		temp = temp->next;
	}
	return (end_of_right);
}

t_ast	*create_and_link_pipe(t_ast **current_node, t_ast *right_side,
		t_ast **root)
{
	t_ast	*pipe_node;

	pipe_node = create_pipe_node(*current_node, right_side);
	if (!pipe_node)
	{
		free_ast(right_side);
		return (NULL);
	}
	if (*current_node == *root)
		*root = pipe_node;
	*current_node = pipe_node;
	return (pipe_node);
}

t_ast	*handle_pipe(t_ast **current_node, t_stack **current, t_stack *stack,
		t_ast **root)
{
	t_ast	*right_side;
	t_stack	*next_token;
	t_stack	*end_of_right;

	next_token = (*current)->next;
	end_of_right = find_right_side_end(next_token, stack);
	right_side = process_right_side(next_token, end_of_right, current);
	if (!right_side)
		return (NULL);
	return (create_and_link_pipe(current_node, right_side, root));
}
