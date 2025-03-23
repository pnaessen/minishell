#include "minishell.h"
#include "pars.h"

void	free_redirections(t_redir *redirs)
{
	t_redir	*current;
	t_redir	*next;

	current = redirs;
	while (current)
	{
		next = current->next;
		if (current->file)
			free(current->file);
		free(current);
		current = next;
	}
}

void	free_ast_cmd(t_ast *node)
{
	if (!node || !node->cmd)
		return ;
	free_ast_cmd_args(node);
	if (node->cmd->path)
		free(node->cmd->path);
	free(node->cmd);
	node->cmd = NULL;
}

void	free_ast(t_ast *node)
{
	if (!node)
		return ;
	if (node->left)
	{
		free_ast(node->left);
		node->left = NULL;
	}
	if (node->right)
	{
		free_ast(node->right);
		node->right = NULL;
	}
	free_ast_cmd(node);
	free(node);
}

void	free_ast_cmd_args(t_ast *node)
{
	int	i;

	if (!node || !node->cmd || !node->cmd->args)
		return ;
	i = 0;
	while (node->cmd->args[i])
	{
		free(node->cmd->args[i]);
		i++;
	}
	free(node->cmd->args);
	node->cmd->args = NULL;
}

int	create_and_link_redir(t_ast **temp_node, t_stack *temp_stack)
{
	t_ast	*new_redir;

	new_redir = create_redir_node(temp_stack->token, temp_stack->next->cmd[0],
			(*temp_node)->left);
	if (!new_redir)
		return (0);
	(*temp_node)->left = new_redir;
	return (1);
}
