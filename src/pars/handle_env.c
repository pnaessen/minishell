#include "minishell.h"
#include "pars.h"

int	is_valid_var_char(char args)
{
	if (args == '_')
		return (SUCCESS);
	if ((args >= 'a' && args <= 'z') || (args >= 'A' && args <= 'Z'))
		return (SUCCESS);
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
		if (args[i] == ' ' || args[i] == '"')
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
	while (args[i])
	{
		var_name[j] = args[i];
		j++;
		i++;
		if (args[i] == ' ' || args[i] == '"')
			break ;
	}
	var_name[j] = '\0';
	return (var_name);
}

char	*get_env_value(char *var_name, t_env **env)
{
	char	*value;
	t_env	*temp;
	int		len;
	int		len_str;

	temp = *env;
	len = ft_strlen(var_name);
	len_str = ft_strlen(temp->str);
	while (1)
	{
		if (ft_strncmp(temp->str, var_name, len) == SUCCESS)
		{
			value = ft_substr(temp->str, len + 1, len_str);
			return (value);
		}
		temp = temp->next;
		if (temp == NULL)
			break ;
	}
	return (NULL);
}

int	size_of_args(char *args)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while (args[i])
	{
		if (args[i] == '$')
		{
			while (args[i] && args[i] != '"')
				i++;
		}
		i++;
		len++;
	}
	return (len);
}

char	*replace_with_empty(char *args, int pos)
{
	char	*new_args;
	int		i;
	int		j;
	int		len_args;

	i = 0;
	j = 0;
	printf("in replace empty\n");
	len_args = size_of_args(args);
	new_args = malloc((len_args + 4) * sizeof(char)); // espace
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
}//handle when it is an operator

char	*replace_with_value(char *args, int pos, char *value)
{
	char	*new_args;
	int		i;
	int		j;
	int		len_args;
	int		len_value;

	i = 0;
	j = 0;
	len_value = ft_strlen(value);
	len_args = size_of_args(args);
	new_args = malloc((len_args + len_value + 3) * sizeof(char));
	while (args[i])
	{
		if (i == pos)
		{
			new_args[j++] = 39;
			while (*value)
				new_args[j++] = *value++;
			new_args[j++] = 39;
			while (args[i] && (args[i] != ' ' || args[i] != '"'))
				i++;
		}
		new_args[j++] = args[i++];
	}
	new_args[j] = '\0';
	return (new_args);
} // handle when it is an operator 

char	*replace_without_dollar(char *args, int pos, int quote)
{
	char	*new_args;
	int		len;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (quote == SUCCESS)
		len = size_of_args(args) - 3;
	else
		len = size_of_args(args) - 2;
	new_args = malloc((len + 1) * sizeof(char));
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

char	*find_and_replace_var(char *args, t_env **env)
{
	char	*var_name;
	char	*value;
	char	*new_args;
	int		i;
	t_data	data;

	data.quote_type = '\0';
	i = 0;
	while (args[i])
	{
		handle_quotes(args[i], &data);
		if (args[i] == '$' && is_valid_var_char(args[i + 1]) == SUCCESS)
		{
			if (data.quote_type != 39)
			{
				var_name = extract_variable_name(args, i + 1);
				printf("var_name = %s\n", var_name);
				value = get_env_value(var_name, env);
				printf("value = %s\n", value);
				if (!value)
				{
					if (data.quote_type == '"')
						new_args = replace_with_empty(args, i - 1);
					else
						new_args = replace_with_empty(args, i);
				}
				else
				{
					if (data.quote_type == '"')
						new_args = replace_with_value(args, i - 1, value);
					else
						new_args = replace_with_value(args, i, value);
				}
				printf("new_args = %s\n", new_args);
			}
			else
				new_args = ft_strdup(args);
		}
		else if (args[i] == '$' && is_valid_var_char(args[i + 1]) == ERROR)
		{
			if (ft_is_quotes(args[i + 1]) == SUCCESS || (args[i + 1] >= '0'
					&& args[i + 1] <= '9'))
				new_args = replace_without_dollar(args, i, ft_is_quotes(args[i
							+ 1]));
			else
				new_args = ft_strdup(args);
		}
		i++;
	}
	return (new_args);
}
