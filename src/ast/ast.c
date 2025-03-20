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

t_ast	*create_redir_args(t_ast *redir_node, char *filename)
{
	redir_node->cmd->args = malloc(sizeof(char *) * 2);
	if (!redir_node->cmd->args)
	{
		free(redir_node->cmd);
		free(redir_node);
		return (NULL);
	}
	redir_node->cmd->args[0] = ft_strdup(filename);
	if (!redir_node->cmd->args[0])
	{
		free(redir_node->cmd->args);
		free(redir_node->cmd);
		free(redir_node);
		return (NULL);
	}
	redir_node->cmd->args[1] = NULL;
	return (redir_node);
}

void	init_redir_properties(t_ast *redir_node, t_ast *cmd_node)
{
	redir_node->cmd->path = NULL;
	redir_node->cmd->redirs = NULL;
	redir_node->cmd->has_heredoc = 0;
	redir_node->left = cmd_node;
	redir_node->right = NULL;
	redir_node->head = redir_node;
	redir_node->error_code = 0;
}

t_ast	*create_redir_node(t_node_type token, char *filename, t_ast *cmd_node)
{
	t_ast	*redir_node;

	redir_node = malloc(sizeof(t_ast));
	if (!redir_node)
		return (NULL);
	redir_node->token = token;
	redir_node->cmd = malloc(sizeof(t_cmd));
	if (!redir_node->cmd)
	{
		free(redir_node);
		return (NULL);
	}
	redir_node = create_redir_args(redir_node, filename);
	if (!redir_node)
		return (NULL);
	init_redir_properties(redir_node, cmd_node);
	return (redir_node);
}

t_ast	*process_redir_cmd(t_stack *next_token, t_stack *cmd_after_redir,
		t_stack **current)
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
	*current = cmd_after_redir;
	return (redir_node);
}
