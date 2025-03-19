#include "minishell.h"

char	*get_home_var(t_env *env)
{
	t_env	*temp;

	temp = env;
	while (temp)
	{
		if (ft_strncmp(temp->str, "HOME=", 5) == 0)
			return (temp->str + 5);
		temp = temp->next;
	}
	return (NULL);
}

int	env_var_exists(t_env *env, char *name)
{
	t_env	*temp;
	int		name_len;

	temp = env;
	name_len = ft_strlen(name);
	while (temp)
	{
		if (ft_strncmp(temp->str, name, name_len) == 0
			&& temp->str[name_len] == '=')
			return (1);
		temp = temp->next;
	}
	return (0);
}

void	add_to_env(t_env **env, char *new_str)
{
	t_env	*new_node;

	new_node = malloc(sizeof(t_env));
	if (!new_node)
		return ;
	new_node->str = new_str;
	new_node->next = NULL;
	lstadd_back(env, new_node);
}
