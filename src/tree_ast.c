#include "minishell.h"
#include "pars.h"

t_ast	*init_first_cmd(t_stack *stack, t_stack *end, t_ast **current_node)
{
	t_stack	*current;
	t_ast	*root;

	current = find_next_cmd(stack, end);
	if (current->token != CMD)
		return (NULL);
	root = create_ast_command(current->cmd);
	if (!root)
		return (NULL);
	*current_node = root;
	return (root);
}

