
#include "minishell.h"

int	is_valid_identifier(char *str)
{
	int		i;
	char	*name;
	char	*equal_pos;

	if (!str || !*str || ft_isdigiter(str[0]))
		return (0);
	equal_pos = ft_strchr(str, '=');
	if (equal_pos)
		name = ft_substr(str, 0, equal_pos - str);
	else
		name = ft_strdup(str);
	if (!name)
		return (0);
	i = 0;
	while (name[i])
	{
		if (ft_isalnum(name[i]) && name[i] != '_')
		{
			free(name);
			return (0);
		}
		i++;
	}
	free(name);
	return (1);
}

void	update_env_var(t_env **env, char *var_str)
{
	t_env	*current;
	char	*equal_pos;
	char	*name;
	int		name_len;

	equal_pos = ft_strchr(var_str, '=');
	if (!equal_pos)
		return ;
	name_len = equal_pos - var_str;
	name = ft_substr(var_str, 0, name_len);
	if (!name)
		return ;
	current = *env;
	while (current)
	{
		if (ft_strncmp(current->str, name, name_len) == 0
			&& (current->str[name_len] == '='
				|| current->str[name_len] == '\0'))
		{
			free(current->str);
			current->str = ft_strdup(var_str);
			free(name);
			return ;
		}
		current = current->next;
	}
	free(name);
	add_to_env(env, ft_strdup(var_str));
}

void	ft_export(t_ast *cmd, t_env **env)
{
	int	i;

	if (!cmd->cmd->args[1])
	{
		print_sorted_env(env);
		cmd->error_code = 0;
		return ;
	}
	i = 1;
	cmd->error_code = 0;
	while (cmd->cmd->args[i])
	{
		if (is_valid_identifier(cmd->cmd->args[i]))
		{
			if (ft_strchr(cmd->cmd->args[i], '='))
				update_env_var(env, cmd->cmd->args[i]);
			else if (!env_var_exists(*env, cmd->cmd->args[i]))
				add_to_env(env, ft_strdup(cmd->cmd->args[i]));
		}
		else
		{
			ft_putstr_fd("minishell: export: '", 2);
			ft_putstr_fd(cmd->cmd->args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			cmd->error_code = 1;
		}
		i++;
	}
}
