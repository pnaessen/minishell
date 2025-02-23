/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pn <pn@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 11:37:35 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/23 13:28:10 by pn               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

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

void	ft_cd(t_ast *cmd, t_env *env)
{
	char	*path;

	if (!cmd->cmd->args[1])
	{
		path = get_home_var(env);
		if (!path)
		{
			printf("cd: HOME not set\n");
			cmd->error_code = 1;
			return ;
		}
	}
	else
		path = cmd->cmd->args[1];
	if (chdir(path) != 0)
	{
		perror("cd");
		cmd->error_code = 1;
		return ;
	}
	cmd->error_code = 0;
}
