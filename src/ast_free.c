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
	node->cmd = NULL;
	free(node->cmd);
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
	node->cmd->args = NULL;
	free(node->cmd->args);
}

void	free_stack_node(t_stack *node)
{
	if (!node)
		return;
	if (node->cmd)
		free_env_array(node->cmd);
	free(node);
}

void	free_stack(t_stack *stack)
{
	t_stack	*current;
	t_stack	*next;
	t_stack	*start;

	if (!stack)
		return;
	start = stack;
	current = stack;
	if (current->prev)
		current->prev->next = NULL;	
	while (current)
	{
		next = current->next;
		free_stack_node(current);
		if (next == start)
			break;
		current = next;
	}
}
