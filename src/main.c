/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 15:38:59 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/18 16:13:46 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	char	*input;
	t_env	*head;

	(void)argv;
	(void)env;
	head = NULL;
	if (argc != 1)
	{
		printf("Usage : minishell doesn't take argument\n");
		return (0);
	}
	handle_env(env, &head);
	print_stack(&head);
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			exit(0);
		}
		if (*input)
			add_history(input);
		check_build_in(input);
		// send to parser
		// send to exec
		free(input);
	}
	return (0);
}

// < in > out < in1 < in2 < in3 > out1 < out2 cat
// < in > out < (chmod 000) in1 < in2 < in3 > out1 < out2 cat

// export PATH="./"$PATH
// touch cat
// cat Makefile;

// env -i ./minishell
// env
// unset (tout ce qui reste dans l'env)
// + unset une fois que l'environnement est vide
// export

[echo -n test] | [echo yes]