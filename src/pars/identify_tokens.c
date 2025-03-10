#include "pars.h"

void	define_type(t_stack *temp, char *cmd, int quotes)
{
	if (quotes == ERROR)
	{
		if (cmd[0] == '|' && cmd[1] != '|')
			temp->token = PIPE;
		if (cmd[0] == '|' && cmd[1] == '|')
			temp->token = OR;
		if (cmd[0] == '<' && cmd[1] != '<')
			temp->token = REDIR_IN;
		if (cmd[0] == '<' && cmd[1] == '<')
			temp->token = REDIR_HEREDOC;
		if (cmd[0] == '>' && cmd[1] != '>')
			temp->token = REDIR_OUT;
		if (cmd[0] == '>' && cmd[1] == '>')
			temp->token = APPEND;
		if (cmd[0] == '&' && cmd[1] == '&')
			temp->token = AND;
	}
}

int	identify_token_type(t_stack **stack)
{
	t_stack	*temp;
	t_data	*data;
	int		i;

	temp = *stack;
	data = malloc(sizeof(t_data));
	if (!data)
		return (ERROR);
	while (1)
	{
		//i = 0;
		//data->quotes = ERROR;
		// while (temp->cmd[i])
		// {
		// 	handle_quotes(temp->cmd[i][0], data);
		// 	if (ft_is_operator(temp->cmd[i][0]) == ERROR
		// 		|| data->quotes == SUCCESS)
		// 		temp->token = CMD;
		// 	else
		// 		define_type(temp, temp->cmd[i], data->quotes);
		// 	i++;
		// }
		temp = temp->next;
		if (temp == *stack)
			break ;
	}
	return (SUCCESS);
}
