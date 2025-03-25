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

int	check_env_match(t_env *current, char *name, int name_len)
{
	return (ft_strncmp(current->str, name, name_len) == 0
		&& (current->str[name_len] == '=' || current->str[name_len] == '\0'));
}

void	update_var_export(t_env *env, char *name, int name_len, char *var_str)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (check_env_match(current, name, name_len))
		{
			free(current->str);
			current->str = ft_strdup(var_str);
			return ;
		}
		current = current->next;
	}
}

void	update_env_var(t_env **env, char *var_str)
{
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
	update_var_export(*env, name, name_len, var_str);
	if (!env_var_exists(*env, name))
		add_to_env(env, ft_strdup(var_str));
	free(name);
}

void	handle_export_arg(t_env **env, char *arg, int *error_code)
{
	if (is_valid_identifier(arg))
	{
		if (ft_strchr(arg, '='))
			update_env_var(env, arg);
		else if (!env_var_exists(*env, arg))
			add_to_env(env, ft_strdup(arg));
	}
	else
	{
		ft_putstr_fd("minishell: export: '", 2);
		ft_putstr_fd(arg, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		*error_code = 1;
	}
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
		handle_export_arg(env, cmd->cmd->args[i], &cmd->error_code);
		i++;
	}
}

void	print_sorted_env(t_env **env)
{
	t_env	**sorted;
	int		env_size;

	if (!env || !*env)
		return ;
	env_size = count_env_vars(*env);
	sorted = (t_env **)malloc(sizeof(t_env *) * env_size);
	if (!sorted)
		return ;
	populate_env_array(sorted, *env);
	sort_env_array(sorted, env_size);
	print_env_array(sorted, env_size);
	free(sorted);
}
