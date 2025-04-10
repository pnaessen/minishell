/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:02:45 by pnaessen          #+#    #+#             */
/*   Updated: 2025/04/10 15:02:46 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

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
