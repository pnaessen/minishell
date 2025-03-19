#include "minishell.h"
#include "pars.h"

t_ast	*free_cmd_node(t_ast *node, int i)
{
	while (i > 0)
	{
		i--;
		free(node->cmd->args);
	}
	free(node->cmd->args);
	if (node->cmd->redirs)
		free_redirections(node->cmd->redirs);
	free(node->cmd);
	free(node);
	return (NULL);
}

void	free_stack_node(t_stack *node)
{
	if (!node)
		return ;
	if (node->cmd)
	{
		free_env_array(node->cmd);
		node->cmd = NULL;
	}
	free(node);
	node = NULL;
}

void	free_stack(t_stack *stack)
{
	t_stack	*current;
	t_stack	*next;

	if (!stack)
		return ;
	current = stack;
	if (current->prev)
		current->prev->next = NULL;
	while (current)
	{
		next = current->next;
		free_stack_node(current);
		current = next;
	}
}

void	free_args_array(char **args)
{
	int	i;

	i = 0;
	while (args[i])
		free(args[i++]);
	free(args);
}
