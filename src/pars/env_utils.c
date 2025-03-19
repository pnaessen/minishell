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
