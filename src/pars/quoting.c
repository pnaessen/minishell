#include "pars.h"

// int	check_num_of_quotes(char *args)
// {
// 	int		i;
// 	t_data	*data;

// 	i = 0;
// 	data->quotes = ERROR;
// 	data->quote_num = 0;
// 	data->quote_type = '\0';
// 	while (args[i])
// 	{
// 		is_in_quotes(args[i], data);
// 		if (data->quotes == SUCCESS)
// 		{
// 			if (data->quote_type == args[i])
// 				data->quote_num++;
// 		}
// 		i++;
// 	}
// 	// if (data->quotes == SUCCESS)
// 	// 	return (ERROR);
// 	if (data->quote_num % 2 != 0)
// 		return (ERROR);
// 	return (SUCCESS);
// }

int	is_closed(char *args, int i, char quote)
{
	i++;
	while (args[i])
	{
		if (args[i] == quote)
		{
			args[i] = '#';
			return (SUCCESS);
		}
		else if (args[i] == '#')
			return (SUCCESS);
		i++;
	}
	return (ERROR);
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
	while (args[i])
	{
		handle_quotes(args[i], &data);
		if (data.quotes == SUCCESS)
		{
			if (ft_is_quotes(args[i]) == SUCCESS)
			{
				if ((is_closed(args, i, data.quote_type) == SUCCESS)
					|| args[i] == '#')
					i++;
				if (data.quote_type != args[i])
				{
					len++;
					i++;
				}
			}
		}
		len++;
		i++;
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
	str = malloc(size * sizeof(char));
	if (!str)
		return (NULL);
	if (ft_is_quotes(args[i]) == SUCCESS)
	{
		handle_quotes(args[i], &data);
		if ((is_closed(args, i, data.quote_type) == SUCCESS))
			i++;
	}
	while (args[i])
	{
		handle_quotes(args[i], &data);
		if (data.quotes == SUCCESS && ft_is_quotes(args[i]) == SUCCESS)
		{
			if ((is_closed(args, i, data.quote_type) == SUCCESS))
				i++;
			else if (data.quote_type != args[i])
			{
				str[j] = args[i];
				j++;
				i++;
			}
		}
		if (args[i] == '#')
			i++;
		str[j] = args[i];
		j++;
		i++;
	}
	str[j] = '\0';
	printf("str : %s\n", str);
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
			printf("size : %d\n", size);
			new_cmd = handling_quotes(temp->cmd[i], size);
			printf("temp->cmd[%d] : %s || new_cmd : %s\n", i, temp->cmd[i],
				new_cmd);
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
