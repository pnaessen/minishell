#include "minishell.h"

t_ast	*process_input_redir(t_ast *result, t_stack *redir)
{
	t_ast	*target_cmd;
	t_ast	*redir_node;

	target_cmd = find_cmd_for_input_redir(result);
	if (!target_cmd || target_cmd->token != CMD)
		return (result);
	redir_node = create_redir_node(redir->token, redir->next->cmd[0],
			target_cmd);
	if (!redir_node)
		return (NULL);
	replace_cmd_with_redir(&result, target_cmd, redir_node);
	return (result);
}

t_ast	*process_output_redir(t_ast *result, t_stack *redir)
{
	t_ast	*redir_node;

	redir_node = create_redir_node(redir->token, redir->next->cmd[0], result);
	if (!redir_node)
		return (NULL);
	return (redir_node);
}

t_ast	*process_redirections_ast(t_ast *result, t_stack **redir_tokens,
		int redir_count)
{
	int		i;
	t_stack	*redir;

	if (!redir_tokens)
		return (result);
	i = 0;
	while (i < redir_count)
	{
		redir = redir_tokens[i];
		if (redir->token == REDIR_IN || redir->token == REDIR_HEREDOC)
		{
			result = process_input_redir(result, redir);
			if (!result)
				return (NULL);
		}
		else if (redir->token == REDIR_OUT || redir->token == APPEND)
		{
			result = process_output_redir(result, redir);
			if (!result)
				return (NULL);
		}
		i++;
	}
	return (result);
}

void	update_current_position(t_stack **current, t_stack *cmd_token,
		t_stack *start, t_stack *end)
{
	if (cmd_token)
		*current = cmd_token;
	else
		*current = start;
	while (*current != end && (*current)->next != end)
		*current = (*current)->next;
}

t_ast	*process_right_side_of_pipe(t_stack *start, t_stack *end,
		t_stack **current)
{
	t_stack	*cmd_token;
	t_ast	*cmd_node;
	t_ast	*result;
	t_stack	**redir_tokens;
	int		redir_count;

	redir_tokens = collect_redirections(start, end, &cmd_token, &redir_count);
	cmd_node = create_cmd_node(cmd_token);
	if (!cmd_node)
	{
		free(redir_tokens);
		return (NULL);
	}
	result = cmd_node;
	result = process_redirections_ast(result, redir_tokens, redir_count);
	free(redir_tokens);
	if (!result)
	{
		free_ast(cmd_node);
		return (NULL);
	}
	update_current_position(current, cmd_token, start, end);
	return (result);
}
