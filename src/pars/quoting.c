#include "pars.h"

int	check_num_of_quotes(char *args)
{
	int		i;
	t_data	data;

	i = 0;
	data.quotes = ERROR;
	data.quote_num = 0;
	data.quote_type = '\0';
	while (args[i])
	{
		handle_quotes(args[i], &data);
		i++;
	}
	if (data.quote_num % 2 != 0)
		return (ERROR);
	return (SUCCESS);
}

int	final_len(char *args)
{
	int		i;
	int		len;
	t_data	data;

	i = 0;
	len = 0;
	data.quotes = ERROR;
	data.quote_type = '\0';
	data.quote_num = 0;
	while (args[i])
	{
		handle_quotes(args[i], &data);
		if (args[i + 1] && data.quote_type == args[i + 1]
			&& data.quote_type != args[i])
		{
			i++;
			len++;
			if (args[i] && ft_is_quotes(args[i]) == SUCCESS
				&& data.quote_type == args[i])
			{
				data.quote_type = '\0';
				i++;
			}
		}
		else if (args[i] && ft_is_quotes(args[i]) == SUCCESS
			&& data.quote_type == args[i])
		{
			i++;
			if (args[i] && ft_is_quotes(args[i]) == SUCCESS
				&& data.quote_type == args[i])
			{
				data.quote_type = '\0';
				i++;
			}
		}
		else if (args[i])
		{
			i++;
			len++;
		}
	}
	return (len);
}

char	*handling_quotes(char *args, int size)
{
	char	*str;
	int		i;
	int		j;
	t_data	data;

	i = 0;
	j = 0;
	data.quotes = 0;
	data.quote_type = '\0';
	data.quote_num = 0;
	str = malloc(size * sizeof(char)); //leaks
	if (!str)
		return (NULL);
	while (args[i])
	{
		handle_quotes(args[i], &data);
		if (args[i + 1] && data.quote_type == args[i + 1]
			&& data.quote_type != args[i])
		{
			str[j] = args[i];
			i++;
			j++;
			if (args[i] && ft_is_quotes(args[i]) == SUCCESS
				&& data.quote_type == args[i])
			{
				data.quote_type = '\0';
				i++;
			}
		}
		else if (args[i] && ft_is_quotes(args[i]) == SUCCESS
			&& data.quote_type == args[i])
		{
			i++;
			if (args[i] && ft_is_quotes(args[i]) == SUCCESS
				&& data.quote_type == args[i])
			{
				data.quote_type = '\0';
				i++;
			}
		}
		else if (args[i])
		{
			str[j] = args[i];
			i++;
			j++;
		}
	}
	str[j] = '\0';
	return (str);
}

int	quoting(t_stack **stack)
{
	t_stack	*temp;
	char	*new_cmd;
	int		size;
	int		i;

	temp = *stack;
	while (1)
	{
		i = 0;
		while (temp->cmd[i])
		{
			size = final_len(temp->cmd[i]) + 1;
			new_cmd = handling_quotes(temp->cmd[i], size);
			if (!new_cmd)
				return (ERROR);
			free(temp->cmd[i]);
			temp->cmd[i] = new_cmd;
			i++;
		}
		temp = temp->next;
		if (temp == *stack)
			break ;
	}
	return (0);
}
