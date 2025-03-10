#include "pars.h"

int	check_num_of_quotes(char *args)
{
	int		i;
	t_data	*data;

	i = 0;
	data->quotes = ERROR;
	data->quote_num = 0;
	data->quote_type = '\0';
	while (args[i])
	{
		handle_quotes(args[i], data);
		if (data->quotes == SUCCESS)
		{
			if (data->quote_type == args[i])
				data->quote_num++;
		}
		i++;
	}
	// if (data->quotes == SUCCESS)
	// 	return (ERROR);
	if (data->quote_num % 2 != 0)
		return (ERROR);
	return (SUCCESS);
}

int	is_closed(char *args, int i, char quote)
{
	i += 1;
	while (args[i])
	{
		if (args[i] == quote)
			return (SUCCESS);
		i++;
	}
	return (ERROR);
}
int	final_len(char *args, t_data *data)
{
	int		i;
	int		len;
	t_data	*data;

	i = 0;
	len = 0;
	data->quotes = ERROR;
	data->quote_type = '\0';
	while (args[i])
	{
		handle_quotes(args[i], data);
		if (ft_is_quote(args[i]) && is_closed(args, i, args[i])
			&& data->quote_type != args[i])
			i++;
		else
		{
			len++;
			i++;
		}
	}
	return (len);
}

char	*handling_quotes(char *args, int size, t_data *data)
{
	char	*str;
	int		i;
	int		j;
	int		is_expansion;

	i = 0;
	j = 0;
	data->quotes = ERROR;
	data->quote_type = '\0';
	is_expansion = 0;
	str = malloc(size * sizeof(char));
	if (!str)
		return (NULL);
	while (args[i])
	{
		handle_quotes(args[i], &data);
		if (data->quote_type == '"' && j == 0 && args[i + 1] == '$')
			is_expansion = 1;
		if (ft_is_quote(args[i]) && is_closed(args, i, args[i])
			&& !(is_expansion && args[i] == '"'))
			i += 1;
		else
		{
			str[j] = args[i];
			j += 1;
			i += 1;
		}
	}
	str[j] = '\0';
	return (str);
}

int	quoting(t_stack **stack)
{
	t_stack	*temp;
	char	*new_cmd;
	t_data	*data;
	int		size;
	int		i;

	data = malloc(sizeof(t_data));
	if (!data)
		return (ERROR);
	temp = *stack;
	while (1)
	{
		i = 0;
		while (temp->cmd[i])
		{
			size = final_len(temp->cmd[i], data) + 1;
			new_cmd = handling_quotes(temp->cmd[i], size, data);
			free(temp->cmd[i]);
			temp->cmd[i] = new_cmd;
			i++;
		}
		temp = temp->next;
		if (temp == *stack)
			break ;
	}
	return (SUCCESS);
}
