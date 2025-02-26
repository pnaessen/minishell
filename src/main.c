/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pn <pn@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 15:38:59 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/26 22:07:54 by pn               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	execute_command(t_ast *cmd, t_env *env)
{
		execute_ast(cmd, env);
}

int	main(int argc, char **argv, char **env)
{
	char	*input;
	t_env	*head;
	//t_ast	*cmd;
	//t_ast	*pipe_cmd;

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
			exit(0);
		}
		if (*input)
			add_history(input);
		// cmd = create_test_command("pwd");
		// if (cmd)
		// {
		// 	execute_command(cmd, head);
		// }
		t_ast *pipe_cmd = create_test_pipe("ls", "rev");
		if (pipe_cmd)
		{
   			 execute_ast(pipe_cmd, head);
		}
		free(input);
		break ;
	}
	return (0);
}

t_ast	*create_test_command(char *cmd_str)
{
	t_ast	*node;
	char	**args;

	node = malloc(sizeof(t_ast));
	args = ft_split(cmd_str, ' ');
	if (!node || !args)
		return (NULL);
	node->cmd = malloc(sizeof(t_cmd));
	if (!node->cmd)
	{
		write(2, "malloc error\n", 13);
		free(node);
		ft_free(args);
		return (NULL);
	}
	node->cmd->args = args;
	node->cmd->path = NULL;
	node->token = CMD;
	node->left = NULL;
	node->right = NULL;
	node->head = node;
	node->error_code = 0;
	return (node);
}

t_ast *create_test_pipe(char *left_cmd, char *right_cmd)
{
	t_ast *pipe_node;
	t_ast *left;
	t_ast *right;

	pipe_node = malloc(sizeof(t_ast));
	left = create_test_command(left_cmd);
	right = create_test_command(right_cmd);
	if (!pipe_node || !left || !right)
	{
		if (pipe_node)
			free(pipe_node);
		if (left)
			free_ast(left);
		if (right)
			free_ast(right);
		return (NULL);
	}
	pipe_node->cmd = NULL;
	pipe_node->token = PIPE;
	pipe_node->left = left;
	pipe_node->right = right;
	pipe_node->head = pipe_node;
	left->head = pipe_node;
	right->head = pipe_node;
	
	pipe_node->error_code = 0;
	return (pipe_node);
}

void	free_ast_cmd(t_ast *node)
{
	int	i;

	if (!node->cmd)
		return ;
	if (node->cmd->path)
	{
		free(node->cmd->path);
		node->cmd->path = NULL;
	}
	if (node->cmd->args)
	{
		i = 0;
		while (node->cmd->args[i])
		{
			free(node->cmd->args[i]);
			i++;
		}
		free(node->cmd->args);
		node->cmd->args = NULL;
	}
	free(node->cmd);
	node->cmd = NULL;
}

void	free_ast(t_ast *node)
{
	if (!node)
		return ;
	if (node->left)
	{
		free_ast(node->left);
		node->left = NULL;
	}
	if (node->right)
	{
		free_ast(node->right);
		node->right = NULL;
	}	
	if (node->cmd)
		free_ast_cmd(node);
	free(node);
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
