#include "minishell.h"
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
	t_data	data;
	int		i;

	temp = *stack;
	while (1)
	{
		i = 0;
		data.quotes = ERROR;
		while (temp->cmd[i])
		{
			check_quotes(temp->cmd[i][0], &data);
			if (ft_is_operator(temp->cmd[i][0]) == ERROR
				|| data.quotes == SUCCESS)
				temp->token = CMD;
			else
				define_type(temp, temp->cmd[i], data.quotes);
			i++;
		}
		temp = temp->next;
		if (temp == *stack)
			break ;
	}
	return (SUCCESS);
}

int	check_initial_errors(t_stack *temp, t_stack *first)
{
	if (temp->token == PIPE)
	{
		ft_putstr_fd("minishell: syntax error  unexpected token `|'\n", 2);
		return (ERROR);
	}
	if (first->prev->token != CMD)
	{
		ft_putstr_fd("minishell: syntax error  unexpected token `", 2);
		ft_putstr_fd(first->prev->cmd[0], 2);
		ft_putstr_fd("'\n", 2);
		return (ERROR);
	}
	return (SUCCESS);
}

int	check_redirection_syntax(t_stack *temp)
{
	int	i;

	i = 0;
	while (temp->cmd && temp->cmd[i])
	{
		if ((temp->cmd[i][0] == '>' && temp->cmd[i][1] == '<')
			|| (temp->cmd[i][0] == '<' && temp->cmd[i][1] == '>'))
		{
			ft_putstr_fd("minishell: syntax error  unexpected token `", 2);
			ft_putstr_fd(temp->cmd[i], 2);
			ft_putstr_fd("'\n", 2);
			return (ERROR);
		}
		i++;
	}
	return (SUCCESS);
}

int	check_errors(t_stack **stack)
{
	t_stack	*temp;
	t_stack	*first;

	if (!stack || !*stack)
		return (ERROR);
	temp = *stack;
	first = *stack;
	if (check_initial_errors(temp, first) == ERROR)
		return (ERROR);
	while (1)
	{
		if (is_redirection(temp->token) && (temp->next == first
				|| temp->next->token != CMD))
		{
			ft_putstr_fd("minishell: syntax error  unexpected token `", 2);
			ft_putstr_fd(temp->cmd[0], 2);
			ft_putstr_fd("'\n", 2);
			return (ERROR);
		}
		if (temp->token == PIPE && temp->next != first
			&& temp->next->token == PIPE)
		{
			ft_putstr_fd("minishell: syntax error  unexpected token `|'\n", 2);
			return (ERROR);
		}
		if (check_redirection_syntax(temp) == ERROR)
			return (ERROR);
		temp = temp->next;
		if (temp == first)
			break ;
	}
	return (SUCCESS);
}
