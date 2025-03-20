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

void	handle_redirection(t_ast **current_node, t_stack **current,
		t_ast **root)
{
	t_ast	*redir_node;
	char	*filename;

	filename = (*current)->next->cmd[0];
	if (!filename)
		return ;
	redir_node = malloc(sizeof(t_ast)); // need check a NULL
	if (!redir_node)
	{
		free_ast(*root);
		*root = NULL;
		return ;
	}
	redir_node->token = (*current)->token;
	redir_node->cmd = malloc(sizeof(t_cmd)); // need to check a NULL
	if (!redir_node->cmd)
	{
		free(redir_node);
		// return (free_ast(*root));
		return ;
	}
	init_redir_node(redir_node, filename, current_node, root);
	if ((*current)->token == REDIR_HEREDOC && (*current_node)->token != CMD)
		handle_heredoc_case(current_node, root, redir_node);
	else
		handle_standard_case(current_node, root, redir_node);
	*current = (*current)->next;
}

void	init_redir_node(t_ast *redir_node, char *filename, t_ast **current_node,
		t_ast **root)
{
	redir_node->cmd->args = malloc(sizeof(char *) * 2); // hereee
	if (!redir_node->cmd->args)
	{
		free(redir_node->cmd);
		free(redir_node);
		free_ast(*root);
		*root = NULL;
		return ;
	}
	redir_node->cmd->args[0] = ft_strdup(filename);
	if (!redir_node->cmd->args[0])
	{
		free(redir_node->cmd->args);
		free(redir_node->cmd);
		free(redir_node);
		return (free_ast(*root));
	}
	redir_node->cmd->args[1] = NULL;
	redir_node->cmd->path = NULL;
	redir_node->cmd->redirs = NULL;
	redir_node->cmd->has_heredoc = 0;
	redir_node->left = *current_node;
	redir_node->right = NULL;
	redir_node->head = redir_node;
	redir_node->error_code = 0;
}

t_ast	*handle_pipe(t_ast **current_node, t_stack **current, t_stack *stack,
		t_ast **root)
{
	t_ast	*new_cmd;
	t_ast	*pipe_node;
	t_stack	*next_cmd;
	t_ast	*right_side;
	t_stack	*last_token;

	next_cmd = find_next_cmd((*current)->next, stack);
	if (next_cmd == stack)
		return (NULL);
	new_cmd = create_ast_command(next_cmd->cmd);
	if (!new_cmd)
		return (NULL);
	right_side = build_right_side(next_cmd, stack, new_cmd, &last_token);
	if (!right_side)
	{
		free_ast(new_cmd);
		return (NULL);
	}
	pipe_node = create_pipe_node(*current_node, right_side);
	if (!pipe_node)
	{
		free_ast(right_side);
		return (NULL);
	}
	if (*current_node == *root)
		*root = pipe_node;
	*current_node = pipe_node;
	*current = last_token;
	return (pipe_node);
}
