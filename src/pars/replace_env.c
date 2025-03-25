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
	new_args = malloc((len_args + 4) * sizeof(char));
	if (!new_args)
		return (NULL);
	while (args[i])
	{
		if (i == pos)
		{
			new_args[j++] = 39;
			new_args[j++] = ' ';
			new_args[j++] = 39;
			while (args[i] && (args[i] != ' ' || args[i] != '"'))
				i++;
		}
		new_args[j++] = args[i++];
	}
	return (new_args);
}

char	*replace_with_value(char *args, int pos, char *value)
{
	char	*new_args;
	t_data	data;
	int		len_args;
	int		len_value;

	data.i = 0;
	data.count = 0;
	len_value = ft_strlen(value);
	printf("len value : %d | pos : %d\n", len_value, pos);
	len_args = size_of_args(args);
	new_args = malloc((len_args + len_value + 3) * sizeof(char));
	if (!new_args)
		return (NULL);
	while (args[data.i])
	{
		if (data.i == pos)
		{
			new_args[data.count++] = 39;
			while (*value)
				new_args[data.count++] = *value++;
			new_args[data.count++] = 39;
			while (args[data.i] && args[data.i] != ' ')
				data.i++;
		}
		new_args[data.count++] = args[data.i++];
	}
	new_args[data.count] = '\0';
	return (new_args);
}

char	*replace_without_dollar(char *args, int pos, int quote)
{
	char *new_args;
	int len;
	int i;
	int j;

	i = 0;
	j = 0;
	if (quote == SUCCESS)
		len = size_of_args(args) - 3;
	else
		len = size_of_args(args) - 2;
	new_args = malloc((len + 1) * sizeof(char));
	if (!new_args)
		return (NULL);
	while (args[i])
	{
		if (i == pos)
			i += 2;
		new_args[j] = args[i];
		i++;
		j++;
	}
	if (quote == SUCCESS)
		new_args[j - 1] = '\0';
	else
		new_args[j] = '\0';
	return (new_args);
}
