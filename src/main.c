/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 15:38:59 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/28 10:43:48 by pnaessen         ###   ########lyon.fr   */
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
			cmd = create_test_pipeline(input);
			if (cmd)
			{
				//print_ast(cmd, 0);
				execute_ast(cmd, head);
				free_ast(cmd);
			}
		}
		free(input);
	}
	free_env_list(head);
	return (0);
}

t_ast	*create_test_command(char *cmd_str)
{
	t_ast	*node;
	char	**args;
	int		i;
	int		only_spaces_or_slash;

	if (cmd_str == NULL)
		return (NULL);
	only_spaces_or_slash = 1;
	i = 0;
	while (cmd_str[i])
	{
		if (cmd_str[i] != ' ' && cmd_str[i] != '\\')
		{
			only_spaces_or_slash = 0;
			break ;
		}
		i++;
	}
	if (cmd_str[0] == '\0' || only_spaces_or_slash)
		return (NULL);
	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	args = ft_split(cmd_str, ' ');
	if (!args)
	{
		free(node);
		return (NULL);
	}
	if (!args[0])
	{
		free(node);
		ft_free(args);
		return (NULL);
	}
	node->cmd = malloc(sizeof(t_cmd));
	if (!node->cmd)
	{
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

t_ast	*create_command_pipeline(char **cmds, int count)
{
	t_ast	*pipe_node;
	t_ast	*root;
	t_ast	*current;
	t_ast	*next_cmd;
	t_ast	*cmd;
	int		i;

	if (!cmds || count <= 0)
		return (NULL);
	if (count == 1)
		return (create_test_command(cmds[0]));
	cmd = create_test_command(cmds[0]);
	if (!cmd)
		return (NULL);
	root = cmd;
	i = 1;
	while (i < count)
	{
		next_cmd = create_test_command(cmds[i]);
		if (!next_cmd)
		{
			free_ast(root);
			return (NULL);
		}
		pipe_node = malloc(sizeof(t_ast));
		if (!pipe_node)
		{
			free_ast(root);
			free_ast(next_cmd);
			return (NULL);
		}
		pipe_node->cmd = NULL;
		pipe_node->token = PIPE;
		pipe_node->left = root;
		pipe_node->right = next_cmd;
		pipe_node->head = pipe_node;
		pipe_node->error_code = 0;
		current = root;
		current->head = pipe_node;
		next_cmd->head = pipe_node;
		root = pipe_node;
		i++;
	}
	return (root);
}

t_ast	*create_test_pipeline(char *cmds)
{
	int		count;
	char	*temp;
	char	**split_commands;
	int		valid_count;
	t_ast	*result;

	if (!cmds || !*cmds)
		return (NULL);
	count = 1;
	temp = cmds;
	while (*temp)
	{
		if (*temp == '|')
			count++;
		temp++;
	}
	split_commands = ft_split(cmds, '|');
	if (!split_commands)
		return (NULL);
	valid_count = 0;
	while (split_commands[valid_count])
		valid_count++;
	result = create_command_pipeline(split_commands, valid_count);
	ft_free(split_commands);
	return (result);
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
