#include "minishell.h"
#include "pars.h"

char	*handle_variable_replacement(char *args, int i, t_data *data,
		t_env **env)
{
	char	*var_name;
	char	*value;
	char	*new_args;

	if (!*env)
	{
		if (args[i + 1] == '?')
		{
			var_name = extract_variable_name(args, i + 1);
			value = ft_itoa(data->error_code);
			new_args = replace_value(args, i, value, var_name);
			free(value);
			free(var_name);
		}
		else if (data->quote_type == '"')
			new_args = replace_with_empty(args, i - 1);
		else
			new_args = replace_with_empty(args, i);
		return (new_args);
	}
	var_name = extract_variable_name(args, i + 1);
	value = get_env_value(var_name, env);
	if (args[i + 1] == '?')
		value = ft_itoa(data->error_code);
	if (!value)
	{
		if (data->quote_type == '"')
			new_args = replace_with_empty(args, i - 1);
		else
			new_args = replace_with_empty(args, i);
	}
	else
		new_args = replace_value(args, i, value, var_name);
	free(value);
	free(var_name);
	return (new_args);
}

char	*handle_invalid_variable(char *args, int i, char quote)
{
	write(1, &args[i + 1], 1);
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
				tab[data->i] = handle_variable_replacement(tab[data->i],
						data->j, data, env);
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

char	*find_and_replace_var(char *args, t_env **env, int tmp_error)
{
	char	**tab;
	char	*res;
	t_data	data;

	if (!env)
		*env = NULL;
	tab = split_var(args);
	if (!tab)
		return (NULL);
	data.i = 0;
	data.error_code = tmp_error;
	while (tab[data.i])
	{
		data.quote_type = '\0';
		data.quote_num = 0;
		data.quotes = ERROR;
		data.j = 0;
		tab[data.i] = process_variable_replacement(tab, &data, env);
		data.i++;
	}
	res = return_env(args, tab);
	return (res);
}

char	*return_env(char *args, char **tab_args)
{
	int		space;
	char	*new_args;

	space = spaces_to_add(args);
	new_args = join_tabs(tab_args, space);
	ft_free_all(tab_args);
	return (new_args);
}

int	spaces_to_add(const char *s1)
{
	int		i;
	int		count;
	t_data	data;

	i = 0;
	count = 0;
	data.quotes = ERROR;
	while (s1[i])
	{
		check_quotes(s1[i], &data);
		if (s1[i + 1] == '\0')
			return (count);
		if (s1[i] != ' ' && s1[i + 1] == ' ')
		{
			if (data.quotes == ERROR)
				count++;
		}
		i++;
	}
	return (count);
}

int	len_tab_of_tab(char **tab_args)
{
	int	i;
	int	size;

	i = 0;
	size = 0;
	while (tab_args[i])
	{
		size += ft_strlen(tab_args[i]);
		i++;
	}
	return (size + (i - 1));
}

char	*join_tabs(char **tab_args, int space)
{
	t_data	data;
	int		len;
	char	*new_args;

	data.i = 0;
	data.count = 0;
	len = len_tab_of_tab(tab_args);
	new_args = malloc((len + 1) * sizeof(char));
	if (!new_args)
		return (NULL);
	len += 1;
	while (tab_args[data.i])
	{
		data.j = 0;
		while (tab_args[data.i][data.j])
		{
			new_args[data.count] = tab_args[data.i][data.j];
			data.count++;
			data.j++;
		}
		if (space > 0)
		{
			new_args[data.count] = ' ';
			data.count++;
			space--;
		}
		data.i++;
	}
	new_args[data.count] = '\0';
	return (new_args);
}
// unset PWD SHLVL _