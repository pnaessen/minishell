#include "minishell.h"

int	init_redir_node_for_pipe(t_ast *node, char *filename)
{
	node->cmd->args = malloc(sizeof(char *) * 2);
	if (!node->cmd->args)
	{
		free(node->cmd);
		free(node);
		return (0);
	}
	node->cmd->args[0] = ft_strdup(filename);
	if (!node->cmd->args[0])
	{
		free(node->cmd->args);
		free(node->cmd);
		free(node);
		return (0);
	}
	node->cmd->args[1] = NULL;
	node->cmd->path = NULL;
	node->cmd->has_heredoc = 0;
	node->right = NULL;
	node->head = node;
	node->error_code = 0;
	return (1);
}

t_ast	*create_new_redir_node(t_stack *redir_current, t_ast *current_node)
{
	t_ast	*temp_node;
	char	*filename;

	filename = redir_current->next->cmd[0];
	if (!filename)
		return (NULL);
	temp_node = malloc(sizeof(t_ast));
	if (!temp_node)
		return (NULL);
	temp_node->token = redir_current->token;
	temp_node->cmd = malloc(sizeof(t_cmd));
	if (!temp_node->cmd)
	{
		free(temp_node);
		return (NULL);
	}
	if (!init_redir_node_for_pipe(temp_node, filename))
		return (NULL);
	temp_node->left = current_node;
	return (temp_node);
}

t_ast	*process_redirections(t_stack *current, t_stack *stack, t_ast *cmd_node,
		t_stack **last_token)
{
	t_ast	*current_node;
	t_ast	*temp_node;

	current_node = cmd_node;
	while (current != stack && is_redirection(current->token)
		&& current->next != stack && current->next->token == CMD)
	{
		temp_node = create_new_redir_node(current, current_node);
		if (!temp_node)
			return (NULL);
		current_node = temp_node;
		current = current->next->next;
	}
	*last_token = current->prev;
	return (current_node);
}

t_ast	*build_right_side(t_stack *next_cmd, t_stack *stack, t_ast *new_cmd,
		t_stack **last_token)
{
	t_ast	*cmd_node;
	t_stack	*redir_current;

	cmd_node = new_cmd;
	redir_current = next_cmd->next;
	return (process_redirections(redir_current, stack, cmd_node, last_token));
}
