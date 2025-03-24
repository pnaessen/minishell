/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 15:38:59 by pnaessen          #+#    #+#             */
/*   Updated: 2025/03/24 10:01:19 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	char	*input;
	t_env	*head;
	t_ast	*cmd;

	head = NULL;
	(void)argv;
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
			free_env_list(head);
			exit(0);
		}
		if (*input)
		{
			add_history(input);
			cmd = parse_and_build_ast(input, &head);
			if (cmd)
			{
				print_ast(cmd, 0);
				execute_ast(cmd, head);
				printf("%d\n", cmd->error_code);
				free_ast(cmd);
			}
		}
		free(input);
	}
	return (0);
}

void	print_ast(t_ast *node, int level)
{
	int	i;

	if (!node)
		return ;
	for (int i = 0; i < level; i++)
		printf("  ");
	if (node->token == CMD)
	{
		printf("CMD: ");
		if (node->cmd && node->cmd->args)
		{
			i = 0;
			while (node->cmd->args[i])
			{
				printf("%s ", node->cmd->args[i]);
				i++;
			}
		}
		printf("\n");
	}
	else if (node->token == PIPE)
	{
		printf("PIPE\n");
		print_ast(node->left, level + 1);
		print_ast(node->right, level + 1);
	}
	else if (node->token == REDIR_IN)
	{
		printf("REDIR_IN\n");
		print_ast(node->left, level + 1);
		print_ast(node->right, level + 1);
	}
	else if (node->token == REDIR_OUT)
	{
		printf("REDIR_OUT\n");
		print_ast(node->left, level + 1);
		print_ast(node->right, level + 1);
	}
}
