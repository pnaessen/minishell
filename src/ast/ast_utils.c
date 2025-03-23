#include "minishell.h"
#include "pars.h"

t_stack	*find_next_cmd(t_stack *current, t_stack *end)
{
	while (current != end && current->token != CMD
		&& is_redirection(current->prev->token))
		current = current->next;
	return (current);
}

t_ast	*init_cmd_node_alloc(t_ast *node, int args_count)
{
	node->cmd = malloc(sizeof(t_cmd));
	if (!node->cmd)
	{
		free(node);
		return (NULL);
	}
	node->cmd->args = malloc(sizeof(char *) * (args_count + 1));
	if (!node->cmd->args)
	{
		free(node->cmd);
		free(node);
		return (NULL);
	}
	node->cmd->redirs = NULL;
	return (node);
}

t_ast	*init_cmd_struct(t_ast *node)
{
	node->token = CMD;
	node->left = NULL;
	node->right = NULL;
	node->head = node;
	node->error_code = 0;
	node->root = NULL;
	return (node);
}

t_ast	*init_cmd_node(t_ast *node, char **args, int args_count)
{
	int	i;

	node = init_cmd_node_alloc(node, args_count);
	if (!node)
		return (NULL);
	i = 0;
	while (i < args_count)
	{
		node->cmd->args[i] = ft_strdup(args[i]);
		if (!node->cmd->args[i])
			return (free_cmd_node(node, i));
		i++;
	}
	node->cmd->args[args_count] = NULL;
	node->cmd->path = NULL;
	node->cmd->has_heredoc = 0;
	return (node);
}

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
