#include "minishell.h"
#include "pars.h"

char	*handle_variable_replacement(char *args, t_data *data, t_env **env)
{
	char	*var_name;
	char	*value;
	char	*new_args;

	if (!*env)
	{
		new_args = handle_no_env_case(args, data);
		return (new_args);
	}
	var_name = extract_variable_name(args, (data->j + 1));
	value = get_env_value(var_name, env);
	if (args[data->j + 1] == '?')
	{
		free(value);
		value = ft_itoa(data->error_code);
	}
	new_args = handle_value_replacement(args, data, var_name, value);
	return (new_args);
}

char	*handle_no_env_case(char *args, t_data *data)
{
	char	*var_name;
	char	*value;
	char	*new_args;

	if (args[data->j + 1] == '?')
	{
		var_name = extract_variable_name(args, (data->j + 1));
		value = ft_itoa(data->error_code);
		new_args = replace_value(args, data->j, value, var_name);
		free(value);
		free(var_name);
	}
	else if (data->quote_type == '"')
		new_args = replace_with_empty(args, data->j - 1);
	else
		new_args = replace_with_empty(args, data->j);
	return (new_args);
}

char	*handle_value_replacement(char *args, t_data *data, char *var_name,
		char *value)
{
	char	*new_args;

	if (!value)
	{
		if (data->quote_type == '"')
			new_args = replace_with_empty(args, (data->j - 1));
		else
			new_args = replace_with_empty(args, data->j);
	}
	else
		new_args = replace_value(args, data->j, value, var_name);
	free(value);
	free(var_name);
	return (new_args);
}

char	*handle_invalid_variable(char *args, int i, char quote)
{
	if (quote != '\'' && (args[i + 1] >= '0' && args[i + 1] <= '9'))
	{
		if (quote == '"')
			return (replace_without_dollar(args, i - 1, SUCCESS));
		else
			return (replace_without_dollar(args, i, ERROR));
	}
	return (ft_strdup(args));
}

char	*process_variable_replacement(char **tab, t_data *data, t_env **env)
{
	while (tab[data->i][data->j])
	{
		handle_quotes(tab[data->i][data->j], data);
		if (tab[data->i][data->j] == '$')
		{
			data->temp = tab[data->i];
			if (is_valid_var_char(tab[data->i], data->j) == SUCCESS
				&& data->quote_type != '\'')
			{
				tab[data->i] = handle_variable_replacement(tab[data->i], data,
						env);
			}
			else
			{
				tab[data->i] = handle_invalid_variable(tab[data->i], data->j,
						data->quote_type);
			}
			free(data->temp);
			return (tab[data->i]);
		}
		else
			data->j++;
	}
	return (tab[data->i]);
}
