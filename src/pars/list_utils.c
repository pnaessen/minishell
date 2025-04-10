#include "pars.h"

void	rot_lstadd_back(t_stack **stack, t_stack *new_node)
{
	t_stack	*last;

	if (!new_node)
		return ;
	if (!*stack)
	{
		*stack = new_node;
		new_node->next = new_node;
		new_node->prev = new_node;
	}
	else
	{
		last = (*stack)->prev;
		last->next = new_node;
		new_node->prev = last;
		new_node->next = *stack;
		(*stack)->prev = new_node;
	}
}

t_stack	*node_init(char **cmd)
{
	t_stack	*new_node;

	new_node = (t_stack *)malloc(sizeof(t_stack));
	if (!(new_node))
		return (NULL);
	new_node->cmd = cmd;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

int	fill_the_list(char **cmd, t_stack **stack)
{
	t_stack	*new_node;

	if (!cmd)
		return (ERROR);
	new_node = node_init(cmd);
	if (!new_node)
		return (ERROR);
	if (!*stack)
	{
		*stack = new_node;
		new_node->next = new_node;
		new_node->prev = new_node;
	}
	else
		rot_lstadd_back(stack, new_node);
	return (SUCCESS);
}
