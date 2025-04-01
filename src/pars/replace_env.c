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
	new_args = malloc((len_args + 3) * sizeof(char));
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
char	*replace_value(char *args, int pos, char *value)
{
	char	*new_args;
	t_data	data;
	int		len_args;
	int		len_value;

	data.i = 0;
	data.count = 0;
	len_value = ft_strlen(value);
	len_args = size_of_args(args);
	new_args = malloc((len_args + len_value + 1) * sizeof(char));
	if (!new_args)
		return (NULL);
	while (args[data.i])
	{
		if (data.i == pos)
		{
			while (*value)
				new_args[data.count++] = *value++;
			while (args[data.i] && args[data.i] != ' ')
			{
				if (args[data.i] == ' '
					|| (ft_is_quotes(args[data.i]) == SUCCESS && data.i > 0))
					break ;
				data.i++;
			}
		}
		if (args[data.i] != '\0')
			new_args[data.count++] = args[data.i++];
	}
	new_args[data.count] = '\0';
	return (new_args);
}

char	*replace_value_quotes(char *args, int pos, char *value)
{
	char	*new_args;
	t_data	data;
	int		len_args;
	int		len_value;

	data.i = 0;
	data.count = 0;
	len_value = ft_strlen(value);
	len_args = size_of_args(args);
	new_args = malloc((len_args + len_value + 3) * sizeof(char));
	if (!new_args)
		return (NULL);
	while (args[data.i])
	{
		if (data.i == pos)
		{
			new_args[data.count++] = '"';
			while (*value)
				new_args[data.count++] = *value++;
			new_args[data.count++] = '"';
			while (args[data.i] && args[data.i] != ' ')
			{
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
	int		len;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (quote == SUCCESS)
		len = size_of_args(args) - 4;
	else
		len = size_of_args(args) - 2;
	new_args = malloc((len + 1) * sizeof(char));
	if (!new_args)
		return (NULL);
	write(1, "in without dollar\n", 19);
	while (args[i])
	{
		if (i == pos)
		{
			if (quote == SUCCESS)
				i += 3;
			else
				i += 2;
		}
		new_args[j] = args[i];
		printf("args[%d] = %c\n", i, args[i]);
		i++;
		j++;
	}
	if (quote == SUCCESS)
		new_args[j - 1] = '\0';
	else
		new_args[j] = '\0';
	return (new_args);
}
