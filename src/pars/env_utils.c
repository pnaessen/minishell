#include "minishell.h"
#include "pars.h"

int	is_valid_var_char(char *args, int i)
{
	if ((i > 2 && args[i - 2] == '<') && (i > 3 && args[i - 3] == '<'))
		return (ERROR);
	if (args[i + 1] != '\0')
	{
		if (args[i + 1] == '?')
			return (SUCCESS);
		if (args[i + 1] == '_')
			return (SUCCESS);
		if ((args[i + 1] >= 'a' && args[i + 1] <= 'z') || (args[i + 1] >= 'A'
				&& args[i + 1] <= 'Z'))
			return (SUCCESS);
	}
	return (ERROR);
}

int	size_of_var(char *args, int i)
{
	int	len;

	len = 0;
	while (args[i])
	{
		len++;
		i++;
		if (args[i] == ' ' || ft_is_quotes(args[i]) == SUCCESS)
			break ;
	}
	return (len);
}

char	*extract_variable_name(char *args, int i)
{
	char	*var_name;
	int		j;
	int		len;

	j = 0;
	len = size_of_var(args, i);
	var_name = malloc((len + 1) * sizeof(char));
	if (!var_name)
		return (NULL);
	while (args[i])
	{
		var_name[j] = args[i];
		j++;
		i++;
		if (args[i] == ' ' || ft_is_quotes(args[i]) == SUCCESS)
			break ;
	}
	var_name[j] = '\0';
	return (var_name);
}

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
