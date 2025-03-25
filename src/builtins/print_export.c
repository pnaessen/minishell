/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_export.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/01 14:47:14 by pn                #+#    #+#             */
/*   Updated: 2025/03/25 09:19:04 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	sort_env_array(t_env **sorted, int env_size)
{
	int		i;
	int		j;
	t_env	*tmp;

	i = 0;
	while (i < env_size - 1)
	{
		j = 0;
		while (j < env_size - i - 1)
		{
			if (ft_strrcmp(sorted[j]->str, sorted[j + 1]->str) > 0)
			{
				tmp = sorted[j];
				sorted[j] = sorted[j + 1];
				sorted[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

void	print_env_var(t_env *var)
{
	char	*equal_pos;
	char	*name;
	char	*value;

	equal_pos = ft_strchr(var->str, '=');
	if (equal_pos)
	{
		name = ft_substr(var->str, 0, equal_pos - var->str);
		value = ft_strdup(equal_pos + 1);
		if (name && value)
			printf("declare -x %s=\"%s\"\n", name, value);
		free(name);
		free(value);
	}
	else
		printf("declare -x %s\n", var->str);
}

int	count_env_vars(t_env *env_list)
{
	int	count;

	count = 0;
	while (env_list)
	{
		count++;
		env_list = env_list->next;
	}
	return (count);
}

void	populate_env_array(t_env **sorted, t_env *env_list)
{
	int	i;

	i = 0;
	while (env_list)
	{
		sorted[i++] = env_list;
		env_list = env_list->next;
	}
}

void	print_env_array(t_env **sorted, int size)
{
	int	i;

	i = 0;
	while (i < size)
		print_env_var(sorted[i++]);
}


