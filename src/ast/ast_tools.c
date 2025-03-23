#include "minishell.h"
#include "pars.h"

t_ast	*create_ast_command(char **args)
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
	return (init_cmd_struct(node));
}

t_ast	*create_ast_operator(t_node_type token, t_ast *left, t_ast *right)
{
	t_ast	*node;

	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->token = token;
	node->cmd = NULL;
	node->left = left;
	node->right = right;
	node->head = node;
	node->error_code = 0;
	node->root = node;
	if (left)
		left->head = node;
	if (right)
		right->head = node;
	return (node);
}

t_ast	*create_pipe_node(t_ast *left_cmd, t_ast *right_cmd)
{
	t_ast	*pipe_node;

	pipe_node = create_ast_operator(PIPE, left_cmd, right_cmd);
	if (!pipe_node)
		return (NULL);
	return (pipe_node);
}

t_ast	*setup_initial_nodes(t_stack *next_token, t_stack *cmd_after_redir)
{
	t_ast	*right_side;
	t_ast	*redir_node;

	right_side = create_ast_command(cmd_after_redir->cmd);
	if (!right_side)
		return (NULL);
	redir_node = create_redir_node(next_token->token, next_token->next->cmd[0],
			right_side);
	if (!redir_node)
	{
		free_ast(right_side);
		return (NULL);
	}
	return (redir_node);
}

t_ast	*process_redir_cmd(t_stack *next_token, t_stack *cmd_after_redir,
		t_stack **current)
{
	t_ast	*redir_node;
	t_ast	*temp_node;
	t_stack	*temp_stack;

	redir_node = setup_initial_nodes(next_token, cmd_after_redir);
	if (!redir_node)
		return (NULL);
	temp_node = redir_node;
	temp_stack = next_token->next->next;
	while (temp_stack != cmd_after_redir && is_redirection(temp_stack->token)
		&& temp_stack->next != cmd_after_redir)
	{
		if (!create_and_link_redir(&temp_node, temp_stack))
		{
			free_ast(redir_node);
			return (NULL);
		}
		temp_stack = temp_stack->next->next;
	}
	*current = cmd_after_redir;
	return (redir_node);
}
