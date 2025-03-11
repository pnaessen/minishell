#include "minishell.h"
#include "pars.h"

t_ast	*free_cmd_node(t_ast *node, int i)
{
	while (i > 0)
	{
		i--;
		free(node->cmd->redirs);
	}
	free(node->cmd->args);
	if (node->cmd->redirs)
		free_redirections(node->cmd->redirs);
	free(node->cmd);
	free(node);
	return(NULL);
}

void free_stack_node(t_stack *node)
{
	if(!node)
		return ;
	if (node->cmd)
		free_env_array(node->cmd);
	free(node);
}

void free_stack(t_stack *stack)
{
	t_stack *current;
	t_stack *next;
	t_stack *start;

	if(!stack)
		return ;
	start = stack;
	current = stack;
	if(current->prev)
		current->prev = NULL;
	while(current)
	{
		next = current->next;
		free_stack_node(current);
		if(next == start)
			break;
		current = current->next;
	}
}
