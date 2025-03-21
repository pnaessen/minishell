#include "minishell.h"
#include "pars.h"

void	init_cmd_true(t_ast **root)
{
	*root = malloc(sizeof(t_ast));
	if (!*root)
		return ;
	(*root)->token = CMD;
	(*root)->cmd = malloc(sizeof(t_cmd));
	if (!(*root)->cmd)
	{
		free(*root);
		*root = NULL;
		return ;
	}
	(*root)->cmd->args = malloc(sizeof(char *) * 2);
	if (!(*root)->cmd->args)
	{
		free((*root)->cmd);
		free(*root);
		*root = NULL;
		return ;
	}
	(*root)->cmd->args[0] = ft_strdup("true");
	if (!(*root)->cmd->args[0])
	{
		free((*root)->cmd->args);
		free((*root)->cmd);
		free(*root);
		*root = NULL;
		return ;
	}
	(*root)->cmd->args[1] = NULL;
	(*root)->cmd->path = NULL;
	(*root)->cmd->redirs = NULL;
	(*root)->cmd->has_heredoc = 0;
}

void	init_cmd_node_base(t_ast *root)
{
	root->left = NULL;
	root->right = NULL;
	root->head = root;
	root->error_code = 0;
}

t_ast	*check_cmd_after_redir(t_stack *stack, t_stack *end)
{
	t_ast	*root;
	t_stack	*current;

	root = NULL;
	current = stack;
	while (current != end->next && is_redirection(current->token))
		current = current->next->next;
	if (current != end->next && current->token == CMD)
		root = create_ast_command(current->cmd);
	else
	{
		init_cmd_true(&root);
		if (!root)
			return (NULL);
	}
	init_cmd_node_base(root);
	return (root);
}

t_ast	*init_first_node(t_stack *stack, t_stack *end, t_ast **current_node)
{
	t_ast	*root;
	t_stack	*current;
	t_stack	*cmd_node;

	root = NULL;
	current = stack;
	if (is_redirection(current->token) && current->next != end->next
		&& current->next->token == CMD)
	{
		cmd_node = current;
		while (cmd_node != end && is_redirection(cmd_node->token))
		{
			cmd_node = cmd_node->next->next;
			if (cmd_node == end->next)
				break ;
		}
		if (cmd_node != end->next && cmd_node->token == CMD)
			root = create_ast_command(cmd_node->cmd);
		else
		{
			root = check_cmd_after_redir(stack, end);
			if (!root)
				return (NULL);
			*current_node = root;
			handle_redirection(current_node, &current, &root);
			current->token = CMD;
			return (root);
		}
		*current_node = root;
		current = stack;
		while (current != cmd_node && is_redirection(current->token))
		{
			handle_redirection(current_node, &current, &root);
			if (!root)
				return (NULL);
			current = current->next;
		}
	}
	else
		root = init_first_cmd(stack, end, current_node);
	return (root);
}
