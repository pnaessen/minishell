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
	{
		perror("minishell: malloc create_ast_operator");
		return (NULL);
	}
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
