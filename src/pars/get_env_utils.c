#include "minishell.h"
#include "pars.h"

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
