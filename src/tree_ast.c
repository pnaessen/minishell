/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:52:29 by pnaessen          #+#    #+#             */
/*   Updated: 2025/03/07 13:43:47 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pars.h"

t_ast	*create_ast_command(char **args)
{
	t_ast	*node;
	int		args_count;

	if (!args)
		return (NULL);
	args_count = 0;
	while (args[args_count])
		args_count++;
	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->cmd = malloc(sizeof(t_cmd));
	if (!node->cmd)
	{
		free(node);
		return (NULL);
	}
	return (init_cmd_node(node, args, args_count));
}

t_ast	*init_cmd_node(t_ast *node, char **args, int args_count)
{
	int	i;

	node->cmd->args = malloc(sizeof(char *) * (args_count + 1));
	if (!node->cmd->args)
	{
		free(node->cmd);
		free(node);
		return (NULL);
	}
	i = 0;
	while (i < args_count)
	{
		node->cmd->args[i] = ft_strdup(args[i]);
		if (!node->cmd->args[i])
			return (free_cmd_node(node, i));
		i++;
	}
	node->cmd->args[args_count] = NULL;
	node->cmd->path = NULL;
	node->token = CMD;
	node->left = NULL;
	node->right = NULL;
	node->head = node;
	node->error_code = 0;
	return (node);
}

t_ast	*free_cmd_node(t_ast *node, int i)
{
	while (i > 0)
	{
		i--;
		free(node->cmd->args[i]);
	}
	free(node->cmd->args);
	free(node->cmd);
	free(node);
	return (NULL);
}

t_ast	*create_ast_operator(t_node_type token, t_ast *left, t_ast *right)
{
	t_ast	*node;

	if (!left || !right)
		return (NULL);
	node = malloc(sizeof(t_ast));
	if (!node)
		return (NULL);
	node->token = token;
	node->cmd = NULL;
	node->left = left;
	node->right = right;
	node->head = node;
	node->error_code = 0;
	if (left)
		left->head = node;
	if (right)
		right->head = node;
	return (node);
}

t_stack	*find_next_cmd(t_stack *current)
{
	t_stack	*start;

	if (!current)
		return (NULL);
	start = current;
	while (1)
	{
		if (current->token == CMD)
			return (current);
		current = current->next;
		if (current == start)
			break ;
	}
	return (NULL);
}

t_ast	*parse_and_build_ast(char *input)
{
	t_stack	*parsed_stack;
	t_ast	*ast_result;

	if (!input || !*input)
		return (NULL);
	parsed_stack = parsing_input(input);
	if (!parsed_stack)
		return (NULL);
	ast_result = build_pipe_tree(parsed_stack);
	return (ast_result);
}

t_ast	*create_command_and_continue(t_stack *cmd_token, t_ast *left)
{
	t_ast	*right;
	t_ast	*pipe_node;

	right = create_ast_command(cmd_token->cmd);
	if (!right)
		return (left);
	pipe_node = create_ast_operator(PIPE, left, right);
	// si PIPE = cmd_token->token = segfault
	if (!pipe_node)
	{
		free_ast(right);
		return (left);
	}
	return (pipe_node);
}

t_ast	*process_pipe_node(t_stack *current, t_ast *left_ast)
{
	t_stack	*next_cmd;
	t_ast	*pipe_node;

	next_cmd = find_next_cmd(current->next);
	if (!next_cmd)
		return (left_ast);
	pipe_node = create_command_and_continue(next_cmd, left_ast);
	return (pipe_node);
}

// t_ast	*create_continue(t_stack *cmd_token, t_ast *left)
// {
// 	t_ast	*right;
// 	t_ast	*pipe_node;

// 	right = create_ast_command(cmd_token->cmd);
// 	if (!right)
// 		return (left);
// 	pipe_node = create_ast_operator(REDIR_OUT, left, right);
// 	// si PIPE = cmd_token->token = segfault
// 	if (!pipe_node)
// 	{
// 		free_ast(right);
// 		return (left);
// 	}
// 	return (pipe_node);
// }

// t_ast	*create_in(t_stack *cmd_token, t_ast *left)
// {
// 	t_ast	*right;
// 	t_ast	*pipe_node;

// 	right = create_ast_command(cmd_token->cmd);
// 	if (!right)
// 		return (left);
// 	pipe_node = create_ast_operator(REDIR_IN, left, right);
// 		// si PIPE = cmd_token->token = segfault
// 	if (!pipe_node)
// 	{
// 		free_ast(right);
// 		return (left);
// 	}
// 	return (pipe_node);
// }

// t_ast	*process_redi_node(t_stack *current, t_ast *left_ast)
// {
// 	t_stack	*next_cmd;
// 	t_ast	*pipe_node;

// 	next_cmd = find_next_cmd(current->next);
// 	if (!next_cmd)
// 		return (left_ast);
// 	pipe_node = create_continue(next_cmd, left_ast);
// 	return (pipe_node);
// }

t_ast	*build_pipe_tree(t_stack *stack)
{
	t_stack	*current;
	t_stack	*end;
	t_ast	*left_ast;

	if (!stack)
		return (NULL);
	current = stack;
	end = stack->prev;
	current = find_next_cmd(current);
	if (!current)
		return (NULL);
	left_ast = create_ast_command(current->cmd);
	if (!left_ast)
		return (NULL);
	current = current->next;
	while (current != end->next)
	{
		if (current->token == PIPE)
			left_ast = process_pipe_node(current, left_ast);
		// left_ast devien la head
		// if (current->token == REDIR_IN)
		// 	left_ast = process_redi_node(current, left_ast);
		// if (current->token == REDIR_OUT)
		// 	left_ast = process_redi_node(current, left_ast);
		current = current->next;
	}
	return (left_ast);
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
