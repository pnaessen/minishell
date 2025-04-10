/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_tools.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:02:41 by pnaessen          #+#    #+#             */
/*   Updated: 2025/04/10 15:02:42 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

int	check_env_match(t_env *current, char *name, int name_len)
{
	return (ft_strncmp(current->str, name, name_len) == 0
		&& (current->str[name_len] == '=' || current->str[name_len] == '\0'));
}
