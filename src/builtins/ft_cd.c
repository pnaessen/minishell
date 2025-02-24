/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 11:37:35 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/24 16:07:09 by pnaessen         ###   ########lyon.fr   */
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

void	update_env_cd(t_ast *cmd, t_env *env)
{
	t_env	*tmp;
	char	*pwd;
	char	*old_pwd;

	tmp = env;
	old_pwd = getcwd(NULL, PATH_MAX);
	if (!old_pwd)
	{
		perror("minishell: pwd");
		cmd->error_code = 1;
		return ;
	}
	while (tmp)
	{
		if (ft_strncmp(tmp->str, "PWD=", 4) == 0)
		{
			pwd = getcwd(NULL, PATH_MAX);
			if (!pwd)
			{
				perror("minishell: pwd");
				cmd->error_code = 1;
				free(old_pwd);
				return ;
			}
			if (tmp->str)
				free(tmp->str);
			tmp->str = ft_strdup(pwd);
			free(pwd);
		}
		if (ft_strncmp(tmp->str, "OLDPWD=", 7) == 0)
		{
			if (tmp->str)
				free(tmp->str);
			tmp->str = ft_strdup(old_pwd);
		}
		tmp = tmp->next;
	}
	free(old_pwd);
}
