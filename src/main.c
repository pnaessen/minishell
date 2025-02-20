/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 15:38:59 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/20 11:26:44 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	execute_command(t_ast *cmd, t_env *env)
{
	int	ret;

	ret = check_builtin(cmd, env);
	if (ret == -1)
	{
		ret = execute_ast(cmd, env);
		return (ret);
	}
	return (ret);
}

int	main(int argc, char **env)
{
	char	*input;
	t_env	*head;
	t_ast	*cmd;

	head = NULL;
	cmd = NULL;
	if (argc != 1)
	{
		printf("Usage : minishell doesn't take argument\n");
		return (0);
	}
	handle_signals();
	handle_env(env, &head);
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
		// cmd = parsing(input);
		if (cmd)
		{
			execute_command(cmd, head);
		}
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
