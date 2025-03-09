#include "minishell.h"
#include "pars.h"

void	free_ast_children(t_ast *node)
{
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
}

void	free_ast(t_ast *node)
{
	if (!node)
		return ;
	free_ast_children(node);
	if (node->cmd)
		free_ast_cmd(node);
	free(node);
}

t_ast	*free_cmd_node(t_ast *node, int i)
{
	while (i > 0)
	{
		i--;
		free(node->cmd->args[i]);
	}
	free(node->cmd->args);
	free(node->cmd);
	free(node);
	return (NULL);
}

void	free_ast_cmd(t_ast *node)
{
	if (!node->cmd)
		return ;
	if (node->cmd->path)
	{
		free(node->cmd->path);
		node->cmd->path = NULL;
	}
	if (node->cmd->args)
		free_ast_cmd_args(node);
	free(node->cmd);
	node->cmd = NULL;
}

void	free_ast_cmd_args(t_ast *node)
{
	int	i;

	i = 0;
	while (node->cmd->args[i])
	{
		free(node->cmd->args[i]);
		i++;
	}
	free(node->cmd->args);
	node->cmd->args = NULL;
}
