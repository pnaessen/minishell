#include "minishell.h"
#include "pars.h"

char	*handle_variable_replacement(char *args, int i, t_data *data,
		t_env **env)
{
	char	*var_name;
	char	*value;
	char	*new_args;

	var_name = extract_variable_name(args, i + 1);
	value = get_env_value(var_name, env);
	if (!value)
	{
		if (data->quote_type == '"')
			new_args = replace_with_empty(args, i - 1);
		else
			new_args = replace_with_empty(args, i);
	}
	else
	{
		if (data->quote_type == '"')
			new_args = replace_with_value(args, i - 1, value);
		else
			new_args = replace_with_value(args, i, value);
	}
	free(value);
	return (new_args);
}

char	*handle_invalid_variable(char *args, int i)
{
	if (ft_is_quotes(args[i + 1]) == SUCCESS || (args[i + 1] >= '0' && args[i
			+ 1] <= '9'))
		return (replace_without_dollar(args, i, ft_is_quotes(args[i + 1])));
	return (ft_strdup(args));
}

char	*find_and_replace_var(char *args, t_env **env)
{
	char	*new_args;
	int		i;
	t_data	data;

	new_args = ft_strdup(args);
	i = 0;
	data.quote_type = '\0';
	while (args[i])
	{
		handle_quotes(args[i], &data);
		if (args[i] == '$')
		{
			if (is_valid_var_char(args, i) == SUCCESS)
			{
				if (data.quote_type != 39)
					new_args = handle_variable_replacement(args, i, &data, env);
			}
			else
				new_args = handle_invalid_variable(args, i);
		}
		i++;
	}
	return (new_args);
}
