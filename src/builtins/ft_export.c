/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/28 10:32:39 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/28 12:01:30 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_export(t_ast *cmd, t_env **env)
{
	t_env *current;
	t_env *export;
	char *tmp;

	(void)cmd;
	current = NULL;
	export = *env;
	tmp = ft_strdup(export->str);
	while (export != NULL)
	{
		current = export;
		while (current != NULL)
		{
			if (ft_strncmp(tmp, current->str, ft_strlen(tmp)) >= 0)
				tmp = ft_strdup(current->str);
			current = current->next;
		}
		printf("declare -x %s\n", tmp);
		export = export->next;		
	}
}

// void update_export(t_env *export)
// {
//	 del la node que je viens de print
// }