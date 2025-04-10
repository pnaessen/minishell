#include "minishell.h"
#include "pars.h"

t_ast	*create_cmd_node(t_stack *cmd_token)
{
	t_ast	*cmd_node;
	char	**default_cmd;

	if (cmd_token && cmd_token->token == CMD && cmd_token->cmd)
		return (create_ast_command(cmd_token->cmd));
	default_cmd = create_default_cmd();
	if (!default_cmd)
		return (NULL);
	cmd_node = create_ast_command(default_cmd);
	free(default_cmd[0]);
	free(default_cmd);
	return (cmd_node);
}

int	count_redirections(t_stack *start, t_stack *end)
{
	t_stack	*temp;
	int		count;

	temp = start;
	count = 0;
	while (temp != end)
	{
		if (is_redirection(temp->token) && temp->next != end)
		{
			count++;
			temp = temp->next;
		}
		temp = temp->next;
	}
	return (count);
}

t_stack	**init_redir_tokens(int *redir_count, t_stack *start, t_stack *end)
{
	t_stack	**redir_tokens;

	*redir_count = count_redirections(start, end);
	if (*redir_count == 0)
		return (NULL);
	redir_tokens = malloc(sizeof(t_stack *) * (*redir_count));
	if (!redir_tokens)
		return (NULL);
	return (redir_tokens);
}

t_stack	**collect_redirections(t_stack *start, t_stack *end,
		t_stack **cmd_token, int *redir_count)
{
	t_stack	**redir_tokens;
	t_stack	*temp;
	int		i;

	redir_tokens = init_redir_tokens(redir_count, start, end);
	if (!redir_tokens)
		return (NULL);
	temp = start;
	*cmd_token = NULL;
	i = 0;
	while (temp != end)
	{
		if (temp->token == CMD && !*cmd_token)
			*cmd_token = temp;
		else if (is_redirection(temp->token) && temp->next != end
			&& i < *redir_count)
		{
			redir_tokens[i++] = temp;
			temp = temp->next;
		}
		temp = temp->next;
	}
	return (redir_tokens);
}

t_ast	*find_cmd_for_input(t_ast *result)
{
	t_ast	*target_cmd;

	target_cmd = result;
	while (target_cmd && target_cmd->token != CMD && target_cmd->left)
		target_cmd = target_cmd->left;
	return (target_cmd);
}
