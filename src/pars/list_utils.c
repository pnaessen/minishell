
#include "pars.h"

void	rot_lstadd_back(t_stack **stack, t_stack *new_node)
{
	t_stack	*last;

	if (!new_node)
		return ;
	if (!*stack)
	{
		*stack = new_node;
		new_node->next = new_node;
		new_node->prev = new_node;
	}
	else
	{
		last = (*stack)->prev;
		last->next = new_node;
		new_node->prev = last;
		new_node->next = *stack;
		(*stack)->prev = new_node;
	}
}

t_stack	*node_init(char **cmd)
{
	t_stack	*new_node;

	new_node = (t_stack *)malloc(sizeof(t_stack)); //Leaks
	if (!(new_node))
		return (NULL);
	new_node->cmd = cmd;
	new_node->next = NULL;
	new_node->prev = NULL;
	return (new_node);
}

int	fill_the_list(char **cmd, t_stack **stack)
{
	t_stack	*new_node;

	if (!cmd)
		return (ERROR);
	new_node = node_init(cmd);
	if (!new_node)
		return (ERROR);
	if (!*stack)
	{
		*stack = new_node;
		new_node->next = new_node;
		new_node->prev = new_node;
	}
	else
		rot_lstadd_back(stack, new_node);
	return (SUCCESS);
}

const char	*get_token_name(t_node_type token)
{
	switch (token)
	{
	case CMD:
		return ("CMD");
	case PIPE:
		return ("PIPE");
	case AND:
		return ("AND");
	case OR:
		return ("OR");
	case REDIR_IN:
		return ("REDIR_IN");
	case REDIR_OUT:
		return ("REDIR_OUT");
	case REDIR_HEREDOC:
		return ("REDIR_HEREDOC");
	case APPEND:
		return ("APPEND");
	default:
		return ("UNKNOWN");
	}
}

void	print_stack(t_stack **stack)
{
	t_stack	*temp;
	int		i;

	temp = *stack;
	printf("\n");
	while (1)
	{
		i = 0;
		printf("-----------------------------\n");
		while (temp->cmd[i])
		{
			printf("cmd[%d] : [%s] || type : %s\n", i, temp->cmd[i],
				get_token_name(temp->token));
			i++;
		}
		printf("-----------------------------\n");
		temp = temp->next;
		if (temp == *stack)
			break ;
	}
	printf("\n");
}
