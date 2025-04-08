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

int	ft_strlen_c(char *str, char c)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}

int	size_of_exp(t_env **env, char *var_name)
{
	int		len;
	int		count;
	t_env	*temp;

	count = 0;
	temp = *env;
	while (1)
	{
		len = ft_strlen_c(temp->str, '=');
		if (ft_strncmp(var_name, temp->str, len) == SUCCESS)
		{
			count = ft_strlen(temp->str);
			return (count);
		}
		temp = temp->next;
		if (temp == NULL)
			break ;
	}
	return (0);
}

char	*get_env_value(char *var_name, t_env **env)
{
	char	*value;
	t_env	*temp;
	int		len;
	int		len_str;

	if (!var_name)
		return (NULL);
	temp = *env;
	len_str = size_of_exp(env, var_name);
	if (len_str == 0)
		return (NULL);
	while (1)
	{
		len = ft_strlen_c(temp->str, '=');
		if (ft_strncmp(var_name, temp->str, len) == SUCCESS)
		{
			value = ft_substr(temp->str, len + 1, len_str);
			return (value);
		}
		temp = temp->next;
		if (temp == NULL)
			break ;
	}
	free(var_name);
	return (NULL);
}
