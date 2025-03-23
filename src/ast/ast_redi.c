#include "minishell.h"

int	can_process_prev_redirs(t_stack *temp, t_stack *stack)
{
	return (temp->prev && temp->prev->prev && temp->prev->prev != stack->prev
		&& is_redirection(temp->prev->prev->token));
}

t_ast	*build_redir_chain(t_stack *cmd_after_redir, t_stack *stack,
		t_ast *right_side)
{
	t_stack	*temp;
	t_ast	*redir_node;

	temp = cmd_after_redir;
	if (temp->prev && temp->prev->prev)
	{
		temp = temp->prev->prev;
		while (temp != stack->prev && is_redirection(temp->token))
		{
			redir_node = create_redir_node(temp->token, temp->next->cmd[0],
					right_side);
			if (!redir_node)
			{
				free_ast(right_side);
				return (NULL);
			}
			right_side = redir_node;
			if (can_process_prev_redirs(temp, stack))
				temp = temp->prev->prev;
			else
				break ;
		}
	}
	return (right_side);
}

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
	right_side = process_right_side_of_pipe(next_token, end_of_right, current);
	if (!right_side)
		return (NULL);
	return (create_and_link_pipe(current_node, right_side, root));
}
