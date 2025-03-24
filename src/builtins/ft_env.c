
#include "minishell.h"

void	print_env(t_env **head, t_ast *cmd)
{
	t_env	*current;

	if (*head == NULL)
	{
		printf("env: environment not set\n");
		cmd->error_code = 1;
		return ;
	}
	current = *head;
	while (current != NULL)
	{
		if (ft_strchr(current->str, '='))
			printf("%s\n", current->str);
		current = current->next;
		cmd->error_code = 0;
	}
}
