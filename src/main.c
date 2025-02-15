/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 15:38:59 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/15 17:04:31 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	main(int argc, char **argv, char **env)
{
	(void)env;
	(void)argv;
	char *imput;
	if (argc != 1)
	{
		printf("Usage : minishell doesn't take argument\n");
		return (0);
	}
	while (1)
	{
		imput = readline("minishell$ ");
		if (!imput)
		{
			printf("exit\n");
			exit(0);
		}
		if (*imput)
			add_history(imput);
		// send to parser
		// send to exec
		free(imput);
	}
	return (0);
}