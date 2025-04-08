#include "minishell.h"
#include "pars.h"

char	*replace_with_empty(char *args, int pos)
{
	char	*new_args;
	int		i;
	int		j;
	int		len_args;

	i = 0;
	j = 0;
	len_args = size_of_args(args);
	new_args = malloc((len_args + 3) * sizeof(char)); // invalid read
	if (!new_args)
		return (NULL);
	while (args[i])
	{
		if (i == pos)
		{
			new_args[j++] = '"';
			new_args[j++] = '"';
			while (args[i] && (args[i] != ' '
					|| ft_is_quotes(args[i]) == ERROR))
				i++;
		}
		if (args[i])
			new_args[j++] = args[i++];
	}
	new_args[j] = '\0';
	return (new_args);
}

char	*replace_value(char *args, int pos, char *value, char *var_name)
{
	char	*new_args;
	t_data	data;
	int		start;
	int		len_args;
	int		len_value;

	data.i = 0;
	data.count = 0;
	len_value = ft_strlen(value);
	len_args = size_of_args(args);
	start = ft_strlen(var_name);
	new_args = malloc((len_args + len_value + 2) * sizeof(char));
		// leaks avec echo $USER "$PATH" invalid read
	if (!new_args)
		return (NULL);
	while (args[data.i])
	{
		if (data.i == pos)
		{
			if (data.i > 0 && args[data.i - 1] != '"')
				new_args[data.count++] = '"';
			while (*value)
				new_args[data.count++] = *value++;
			while (args[data.i] && start >= 0)
			{
				start--;
				data.i++;
			}
		}
		if (args[data.i] != '\0')
			new_args[data.count++] = args[data.i++];
	}
	new_args[data.count] = '\0';
	return (new_args);
}

char	*replace_value_quotes(char *args, int pos, char *value, char *var_name)
{
	char	*new_args;
	t_data	data;
	int		start;
	int		len_args;
	int		len_value;

	data.i = 0;
	data.count = 0;
	len_value = ft_strlen(value);
	len_args = size_of_args(args);
	start = ft_strlen(var_name);
	new_args = malloc((len_args + len_value + 3) * sizeof(char)); // leaks
	if (!new_args)
		return (NULL);
	while (args[data.i])
	{
		if (data.i == pos)
		{
			while (*value)
				new_args[data.count++] = *value++;
			while (args[data.i] && start >= 0)
			{
				start--;
				data.i++;
			}
		}
		if (args[data.i] != '\0')
			new_args[data.count++] = args[data.i++];
	}
	if (pos != 0)
	{
		new_args[data.count] = '"';
		data.count++;
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
