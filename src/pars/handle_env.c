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
	if (args[i + 1] == '?')
		value = ft_itoa((*env)->error_code);
	if (!value)
	{
		if (data->quote_type == '"')
			new_args = replace_with_empty(args, i - 1);
		else
			new_args = replace_with_empty(args, i);
	}
	else
	{
		new_args = replace_with_value(args, i, value);
	}
	free(value);
	return (new_args);
}

char	*handle_invalid_variable(char *args, int i)
{
	if (ft_is_quotes(args[i + 1]) == SUCCESS && args[i] == '$')
		return (ft_strdup(args));
	if (ft_is_quotes(args[i + 1]) == SUCCESS || (args[i + 1] >= '0' && args[i
			+ 1] <= '9'))
		return (replace_without_dollar(args, i, ft_is_quotes(args[i + 1])));
	return (ft_strdup(args));
}

void	process_variable_replacement(char **tab, t_data *data, t_env **env)
{
	data->j = 0;
	while (tab[data->i][data->j])
	{
		handle_quotes(tab[data->i][data->j], data);
		if (tab[data->i][data->j] == '$')
		{
			data->temp = tab[data->i];
			if (is_valid_var_char(tab[data->i], data->j) == SUCCESS
				&& data->quote_type != 39)
				tab[data->i] = handle_variable_replacement(tab[data->i],
						data->j, data, env);
			else
				tab[data->i] = handle_invalid_variable(tab[data->i], data->j);
			free(data->temp);
		}
		data->j++;
	}
}

char	*find_and_replace_var(char *args, t_env **env)
{
	char	**tab;
	t_data	data;

	tab = split_var(args);
	data.i = 0;
	while (tab[data.i])
	{
		data.quote_type = '\0';
		data.quote_num = 0;
		data.quotes = ERROR;
		process_variable_replacement(tab, &data, env);
		data.i++;
	}
	data.temp = return_env(args, tab);
	return (data.temp);
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
	while (tab_args[data.i])
	{
		data.j = 0;
		while (tab_args[data.i][data.j])
			new_args[data.count++] = tab_args[data.i][data.j++];
		if (space > 0)
		{
			new_args[data.count++] = ' ';
			space--;
		}
		data.i++;
	}
	new_args[data.count] = '\0';
	return (new_args);
}
