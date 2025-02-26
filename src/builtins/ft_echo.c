/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/26 09:21:08 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/26 09:41:23 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_valid_flag(char *flag)
{
	int	i;

	if (!flag || flag[0] != '-')
		return (0);
	i = 1;
	while (flag[i])
	{
		if (flag[i] != 'n')
			return (0);
		i++;
	}
	if (i == 1)
		return (0);
	return (1);
}

void	ft_echo(t_ast *cmd)
{
	int	i;
	int	flag;

	if (!cmd || !cmd->cmd || !cmd->cmd->args)
		return ;
	i = 1;
	flag = 0;
	while (cmd->cmd->args[i] && is_valid_flag(cmd->cmd->args[i]))
	{
		flag = 1;
		i++;
	}
	while (cmd->cmd->args[i])
	{
		printf("%s", cmd->cmd->args[i]);
		if (cmd->cmd->args[i + 1])
			printf(" ");
		i++;
	}
	if (flag == 0)
		printf("\n");
	cmd->error_code = 0;
}
