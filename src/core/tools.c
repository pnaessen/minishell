/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:04:19 by vicperri          #+#    #+#             */
/*   Updated: 2025/04/10 15:04:20 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	can_create_process(t_env **env)
{
	if (!env || !*env)
		return (1);
	if ((*env)->process_count >= PROCESS_LIMIT)
	{
		ft_putstr_fd("minishell: fork: Resource temporarily unavailable\n", 2);
		return (0);
	}
	(*env)->process_count++;
	return (1);
}

void	process_finished(t_env **env)
{
	if (!env || !*env)
		return ;
	if ((*env)->process_count > 0)
		(*env)->process_count--;
}

void	free_env_fail(char **env_array, int count)
{
	while (count > 0)
		free(env_array[--count]);
	free(env_array);
}
