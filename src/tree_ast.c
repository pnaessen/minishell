#include "minishell.h"
#include "pars.h"

t_ast	*build_tree_compat(t_stack *parsed_stack)
{
	t_stack	*current;
	t_stack	*end;
	t_ast	*root;
	t_ast	*current_node;
	t_ast	*last_cmd_node;

	current = parsed_stack;
	end = parsed_stack->prev;
	root = init_first_cmd(parsed_stack, end, &current_node);
	if (!root)
		return (NULL);
	last_cmd_node = current_node;
	current = current->next;
	while (current != parsed_stack)
	{
		if (current->token == PIPE)
		{
			handle_pipe(&current_node, &current, parsed_stack, &root);
			if (current_node->token == PIPE && current_node->right)
				last_cmd_node = current_node->right;
		}
		else if (current->token == REDIR_IN || current->token == REDIR_OUT
			|| current->token == APPEND || current->token == REDIR_HEREDOC)
		{
			if (current->next != parsed_stack && current->next->token == CMD)
			{
				if (!add_redirection_to_cmd(last_cmd_node, current->token,
						current->next->cmd[0]))
				{
					free_ast(root);
					return (NULL);
				}
				current = current->next;
			}
		}
		current = current->next;
	}
	return (root);
}

t_ast	*parse_and_build_ast(char *input)
{
	t_stack	*parsed_stack;
	t_ast	*ast_result;

	if (!input || !*input)
		return (NULL);
	parsed_stack = parsing_input(input);
	if (!parsed_stack)
		return (NULL);
	ast_result = build_tree_compat(parsed_stack);
	free_stack(parsed_stack);
	return (ast_result);
}

t_ast	*handle_pipe(t_ast **current_node, t_stack **current, t_stack *stack,
		t_ast **root)
{
	t_ast	*new_cmd;
	t_ast	*pipe_node;
	t_stack	*next_cmd;

	next_cmd = find_next_cmd((*current)->next, stack);
	if (next_cmd == stack)
		return (NULL);
	new_cmd = create_ast_command(next_cmd->cmd);
	if (!new_cmd)
		return (NULL);
	pipe_node = create_pipe_node(*current_node, new_cmd);
	if (!pipe_node)
		return (NULL);
	if (*current_node == *root) //si pas la full bug
		*root = pipe_node;
	*current_node = pipe_node;
	*current = next_cmd;
	return (pipe_node);
}

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

int	add_redirection_to_cmd(t_ast *cmd_node, t_node_type type, char *file)
{
	t_redir	*new_redir;
	t_redir	*current;

	// if cmd_node comme << eof | << eof cree la node cmd pour mettre une redi 
	if (!cmd_node || !file)
		return (0);
	if (!cmd_node->cmd)
	{
		cmd_node->cmd = malloc(sizeof(t_cmd));
		if (!cmd_node->cmd)
			return (0);
		cmd_node->cmd->args = NULL;
		cmd_node->cmd->redirs = NULL;
		cmd_node->cmd->path = NULL;
	}
	new_redir = create_redirection(type, file);
	if (!new_redir)
		return (0);
	if (!cmd_node->cmd->redirs)
		cmd_node->cmd->redirs = new_redir;
	else
	{
		current = cmd_node->cmd->redirs;
		while (current->next)
			current = current->next;
		current->next = new_redir;
	}
	return (1);
}

