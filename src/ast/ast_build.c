#include "minishell.h"
#include "pars.h"

void	set_current_position(t_stack *stack, t_stack *end, t_stack **current)
{
	if (is_redirection(stack->token) && stack->next != end->next
		&& stack->next->token == CMD)
	{
		if (stack->next->next != end->next && stack->next->next->token == CMD)
			*current = stack->next->next->next;
		else
			*current = stack->next->next;
		stack->token = REDIR_IN;
	}
	else if (stack->token == CMD)
		*current = stack->next;
	else
	{
		*current = find_next_cmd(stack, end);
		if (*current != end->next)
			*current = (*current)->next;
	}
}

int	handle_after_pipe(t_stack **current, t_stack *stack)
{
	if ((*current)->token == CMD)
		return (0);
	if ((*current)->next != stack && (*current)->next->token == CMD)
		*current = (*current)->next;
	return (1);
}

int	handle_regular_redirection(t_ast **current_node, t_stack **current,
		t_stack *stack, t_ast **root)
{
	if ((*current)->next != stack && (*current)->next->token == CMD)
	{
		handle_redirection(current_node, current, root);
		if (!*root)
			return (0);
	}
	return (1);
}

int	process_current_token(t_ast **current_node, t_stack **current,
		t_stack *stack, t_ast **root)
{
	static int	after_pipe = 0;

	if ((*current)->token == PIPE)
	{
		after_pipe = 1;
		if (!handle_pipe(current_node, current, stack, root))
			return (0);
	}
	else if (is_redirection((*current)->token))
	{
		if (after_pipe)
		{
			if (!handle_after_pipe(current, stack))
				after_pipe = 0;
		}
		else if (!handle_regular_redirection(current_node, current, stack,
				root))
			return (0);
	}
	else if ((*current)->token == CMD)
		after_pipe = 0;
	return (1);
}

t_ast	*build_tree(t_stack *stack)
{
	t_stack	*current;
	t_ast	*root;
	t_ast	*current_node;
	t_stack	*end;

	root = NULL;
	end = stack->prev;
	current = stack;
	root = init_first_node(stack, end, &current_node);
	if (!root)
		return (NULL);
	set_current_position(stack, end, &current);
	while (current != stack)
	{
		if (!process_current_token(&current_node, &current, stack, &root))
		{
			free_ast(root);
			return (NULL);
		}
		current = current->next;
	}
	if (root)
		set_root_pointers(root, root);
	return (root);
}
