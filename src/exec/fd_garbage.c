#include "minishell.h"

void	add_fd_to_garbage(t_fd_garbage **head, int fd)
{
	t_fd_garbage	*new_node;
	t_fd_garbage	*tmp;

	if (fd < 0)
		return ;
	new_node = malloc(sizeof(t_fd_garbage));
	if (!new_node)
	{
		close(fd);
		return ;
	}
	new_node->fd = fd;
	new_node->next = NULL;
	if (!(*head))
		*head = new_node;
	else
	{
		tmp = *head;
		while (tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new_node;
	}
}

void	free_fd_node(t_fd_garbage *node)
{
	if (!node)
		return ;
	if (node->fd >= 0)
		close(node->fd);
	free(node);
}

void	clean_fd_garbage(t_fd_garbage **head)
{
	t_fd_garbage	*current;
	t_fd_garbage	*next;

	if (!head || !*head)
		return ;
	current = *head;
	while (current)
	{
		next = current->next;
		free_fd_node(current);
		current = next;
	}
	*head = NULL;
}
