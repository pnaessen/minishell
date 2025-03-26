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
	char **tab_args;
	char *new_args;
	int i;
	int j;
	t_data data;

	tab_args = pre_tokenisation(args);
	i = 0;
	data.quote_type = '\0';
	data.quote_num = 0;
	data.quotes = ERROR;
	while (tab_args[i])
	{
		j = 0;
		while (tab_args[i][j])
		{
			handle_quotes(tab_args[i][j], &data);
			if (tab_args[i][j] == '$')
			{
				if (is_valid_var_char(tab_args[i], j) == SUCCESS)
				{
					if (data.quote_type != 39)
						tab_args[i] = handle_variable_replacement(tab_args[i], j, &data,
								env);
				}
				else
					tab_args[i] = handle_invalid_variable(tab_args[i], j);
			}
			j++;
		}
		if (i > 0)

		i++;
	}
	new_args = join_tabs(tab_args);
	return (new_args);
}
int len_tab_of_tab(char **tab_args)
{
	int i;
	int j;

	i = 0;
	while (tab_args[i])
	{
		j = 0;
		while (tab_args[i][j])
			j++;
		i++;

	}
	return (i + j);
}
char *join_tabs(char **tab_args)
{
	int i;
	int j;
	int k;
	int size;
	char *new_args;

	i = 0;
	k = 0;
	size = len_tab_of_tab(tab_args);
	new_args = malloc((size) * sizeof(char));
	if (!new_args)
		return (NULL);
	while (tab_args[i])
	{
		j = 0;
		while (tab_args[i][j])
			new_args[k++] = tab_args[i][j++];
		new_args[k++] = ' ';
		i++;
	}
	return (new_args);
}
