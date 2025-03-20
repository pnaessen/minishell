#include "minishell.h"
#include "pars.h"

char	**create_default_cmd(void)
{
	char	**default_cmd;

	default_cmd = malloc(sizeof(char *) * 2);
	if (!default_cmd)
		return (NULL);
	default_cmd[0] = ft_strdup("true");
	if (!default_cmd[0])
	{
		free(default_cmd);
		return (NULL);
	}
	default_cmd[1] = NULL;
	return (default_cmd);
}

t_ast	*process_heredoc_no_cmd(t_stack *next_token, t_stack **current)
{
	t_ast	*right_side;
	t_ast	*redir_node;
	char	**default_cmd;

	default_cmd = create_default_cmd();
	if (!default_cmd)
		return (NULL);
	right_side = create_ast_command(default_cmd);
	free(default_cmd[0]);
	free(default_cmd);
	if (!right_side)
		return (NULL);
	redir_node = create_redir_node(next_token->token, next_token->next->cmd[0],
			right_side);
	if (!redir_node)
	{
		free_ast(right_side);
		return (NULL);
	}
	*current = next_token->next;
	return (redir_node);
}

t_ast	*process_normal_pipe(t_stack *next_token, t_stack *stack,
		t_stack **current)
{
	t_ast	*right_side;
	t_stack	*next_cmd;
	t_stack	*last_token;

	next_cmd = find_next_cmd(next_token, stack);
	if (next_cmd == stack)
		return (NULL);
	right_side = create_ast_command(next_cmd->cmd);
	if (!right_side)
		return (NULL);
	right_side = build_right_side(next_cmd, stack, right_side, &last_token);
	if (!right_side)
		return (NULL);
	*current = last_token;
	return (right_side);
}

t_ast	*handle_pipe_redir(t_stack **current, t_stack *stack,
		t_stack *next_token)
{
	t_stack	*cmd_after_redir;

	cmd_after_redir = next_token->next->next;
	while (cmd_after_redir != stack && cmd_after_redir->token != CMD)
		cmd_after_redir = cmd_after_redir->next;
	if (cmd_after_redir != stack && cmd_after_redir->token == CMD)
		return (process_redir_cmd(next_token, cmd_after_redir, current));
	else if (next_token->token == REDIR_HEREDOC)
		return (process_heredoc_no_cmd(next_token, current));
	return (NULL);
}

t_ast	*handle_pipe(t_ast **current_node, t_stack **current, t_stack *stack,
		t_ast **root)
{
	t_ast	*pipe_node;
	t_ast	*right_side;
	t_stack	*next_token;

	right_side = NULL;
	next_token = (*current)->next;
	if (next_token != stack && is_redirection(next_token->token))
		right_side = handle_pipe_redir(current, stack, next_token);
	else
		right_side = process_normal_pipe(next_token, stack, current);
	if (!right_side)
		return (NULL);
	pipe_node = create_pipe_node(*current_node, right_side);
	if (!pipe_node)
	{
		free_ast(right_side);
		return (NULL);
	}
	if (*current_node == *root)
		*root = pipe_node;
	*current_node = pipe_node;
	return (pipe_node);
}
