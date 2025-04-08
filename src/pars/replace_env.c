#include "minishell.h"
#include "pars.h"

void	size_of_empty_args(char *args, int pos, t_data *data)
{
	while (args[data->i])
	{
		if (data->i == pos)
		{
			data->count++;
			data->count++;
			while (args[data->i] && (args[data->i] != ' '
					|| ft_is_quotes(args[data->i]) == ERROR))
				data->i++;
		}
		if (args[data->i])
		{
			data->count++;
			data->i++;
		}
	}
}

void	size_of_args(char *args, int pos, char *value, t_data *data)
{
	int	j;

	j = 0;
	while (args[data->i])
	{
		if (data->i == pos)
		{
			while (value[j])
			{
				data->count++;
				j++;
			}
			while (args[data->i] && data->start >= 0)
			{
				data->start--;
				data->i++;
			}
		}
		if (args[data->i] != '\0')
		{
			data->count++;
			data->i++;
		}
	}
}

char	*replace_with_empty(char *args, int pos)
{
	char	*new_args;
	t_data	data;

	data.i = 0;
	data.count = 0;
	size_of_empty_args(args, pos, &data);
	new_args = malloc((data.count + 1) * sizeof(char));
	if (!new_args)
		return (NULL);
	data.i = 0;
	data.count = 0;
	while (args[data.i])
	{
		if (data.i == pos)
		{
			new_args[data.count++] = '"';
			new_args[data.count++] = '"';
			while (args[data.i] && (args[data.i] != ' '
					|| ft_is_quotes(args[data.i]) == ERROR))
				data.i++;
		}
		if (args[data.i])
			new_args[data.count++] = args[data.i++];
	}
	new_args[data.count] = '\0';
	return (new_args);
}

char	*replace_value(char *args, int pos, char *value, char *var_name)
{
	char	*new_args;
	t_data	data;

	data.i = 0;
	data.count = 0;
	data.start = ft_strlen(var_name);
	size_of_args(args, pos, value, &data);
	new_args = malloc((data.count + 1) * sizeof(char));
	if (!new_args)
		return (NULL);
	data.i = 0;
	data.count = 0;
	data.start = ft_strlen(var_name);
	while (args[data.i])
	{
		if (data.i == pos)
		{
			while (*value)
				new_args[data.count++] = *value++;
			while (args[data.i] && data.start >= 0)
			{
				data.start--;
				data.i++;
			}
		}
		if (args[data.i] != '\0')
			new_args[data.count++] = args[data.i++];
	}
	new_args[data.count] = '\0';
	return (new_args);
}

char	*replace_without_dollar(char *args, int pos, int quote)
{
	char	*new_args;
	t_data	data;

	data.i = 0;
	data.j = 0;
	if (quote == SUCCESS)
		data.count = ft_strlen(args) - 4;
	else
		data.count = ft_strlen(args) - 2;
	new_args = malloc((data.count + 1) * sizeof(char));
	if (!new_args)
		return (NULL);
	while (args[data.i] && data.j < data.count)
	{
		if (data.i == pos)
		{
			if (quote == SUCCESS)
				data.i += 3;
			else
				data.i += 2;
		}
		new_args[data.j++] = args[data.i++];
	}
	new_args[data.j] = '\0';
	return (new_args);
}
