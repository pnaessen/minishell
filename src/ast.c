#include "minishell.h"

t_ast	*parse_and_build_ast(char *input)
{
	t_stack	*parsed_stack;
	t_ast	*ast_result;

	if (!input || !*input)
		return (NULL);
	parsed_stack = parsing_input(input);
	if (!parsed_stack)
		return (NULL);
	ast_result = build_tree(parsed_stack);
	free_stack(parsed_stack);
	return (ast_result);
}

t_ast	*build_tree(t_stack *stack)
{
	t_stack	*current;
	t_ast	*root;
	t_ast	*current_node;
	t_stack	*end;

	end = stack->prev;
	current = stack;
	root = init_first_cmd(stack, end, &current_node);
	if (!root)
		return (NULL);
	while (current != end)
	{
		if (current->token == PIPE)
		{
			handle_pipe(&current_node, &current, stack, &root);
			if (current_node->token == PIPE && !current_node->right)
				return (NULL);
		}
		else if (is_redirection(current->token))
			handle_redirection(&current_node, &current, &root);
		current = current->next;
	}
	return (root);
}

void	handle_redirection(t_ast **current_node, t_stack **current,
		t_ast **root)
{
	t_ast	*redir_node;

	redir_node = create_ast_redirection((*current)->next->cmd,
			(*current)->token);
	if (!redir_node)
	{
		free_ast(*root);
		return ;
	}
	redir_node->left = *root;
	if (*current_node == *root)
		*root = redir_node;
	*current_node = redir_node;
	*current = (*current)->next;
}

t_ast	*create_ast_redirection(char **args, t_node_type token)
{
	t_ast	*node;
	int		args_count;

	if (!args)
		return (NULL);
	args_count = count_args(args);
	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node = init_cmd_node(node, args, args_count);
	if (!node)
		return (NULL);
	return (init_redi_struct(node, token));
}

t_ast	*init_redi_struct(t_ast *node, t_node_type token)
{
	node->token = token;
	node->left = NULL;
	node->right = NULL;
	node->head = node;
	node->error_code = 0;
	return (node);
}
