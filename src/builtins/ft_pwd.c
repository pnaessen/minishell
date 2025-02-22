/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 16:56:01 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/22 13:18:11 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(t_ast *cmd)
{
	char	*path;

	path = getcwd(NULL, PATH_MAX);
	if (!path)
	{
		perror("minishell: pwd");
		cmd->error_code = 1;
		return ;
	}
	printf("%s\n", path);
	free(path);
	cmd->error_code = 0;
}
