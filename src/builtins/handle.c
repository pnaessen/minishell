/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/18 14:43:00 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/18 15:32:45 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_build_in(char *str, )
{
	if (ft_strcmp(str, "pwd") == 0)
		pwd();
	else if (ft_strcmp(str, "echo") == 0)
		fonction;
	else if (ft_strcmp(str, "cd") == 0)
		fonction;
	else if (ft_strcmp(str, "export") == 0)
		fonction;
	else if (ft_strcmp(str, "unset") == 0)
		fonction;
	else if (ft_strcmp(str, "env") == 0)
		fonction;
	else if (ft_strcmp(str, "exit") == 0)
		fonction;
	else
		return (1);
	return (0);
}
