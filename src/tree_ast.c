/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:52:29 by pnaessen          #+#    #+#             */
/*   Updated: 2025/03/06 15:06:47 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"
#include "pars.h"

t_ast	*parse_and_build_ast(char *input)
{
	t_stack	*pars_input;
	t_ast	*tree;

	pars_input = parsing_input(input);
	if (!pars_input)
		return (NULL);
	tree = convert_stack_to_ast(pars_input);
	if (!tree)
		return (NULL);
	// free_stack(pars_input);
	return (tree);
}

t_ast	*convert_stack_to_ast(t_stack *stack)
{
	t_stack	*current;
	t_stack	*end;
	t_ast	*tree;
	t_ast	*prev_cmd;

	if (!stack)
		return (NULL);
	current = stack;
	end = stack->prev;
	tree = NULL;
	prev_cmd = NULL;
	while (1)
	{
		if (current->token == CMD)
			process_cmd_node(current, &tree, &prev_cmd);
		else if (current->token == PIPE)
			tree = process_pipe_node(current, stack, tree);
		// else if (current->token == REDIR_IN || current->token == REDIR_OUT
		// 	|| current->token == APPEND || current->token == REDIR_HEREDOC)
		// 	tree = process_redir_node(current, stack, tree);
		if (current == end)
			break ;
		current = current->next;
	}
	return (tree);
}

t_ast	*process_cmd_node(t_stack *node, t_ast **tree, t_ast **prev_cmd)
{
	t_ast	*cmd_node;

	cmd_node = create_ast_command(node->cmd);
	if (!cmd_node)
		return (NULL);
	if (!(*tree))
		*tree = cmd_node;
	else if (*prev_cmd && (*tree)->token == PIPE)
		*tree = create_ast_operator(PIPE, *prev_cmd, cmd_node);
	*prev_cmd = cmd_node;
	return (cmd_node);
}

t_ast	*process_pipe_node(t_stack *node, t_stack *stack, t_ast *tree)
{
	t_stack	*next;
	t_ast	*right;

	next = node->next;
	if (next != stack && next->token == CMD)
	{
		right = create_ast_command(next->cmd);
		if (!right)
			return (NULL);
		return (create_ast_operator(PIPE, tree, right));
	}
	return (tree);
}

t_ast	*create_ast_command(char **args)
{
	t_ast	*node;
	int		i;
	int		args_count;

	args_count = 0;
	i = 0;
	if (!args)
		return (NULL);
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
	node->cmd->args = malloc(sizeof(char *) * (args_count + 1));
	if (!node->cmd->args)
	{
		free(node->cmd);
		free(node);
		return (NULL);
	}
	while (i < args_count)
	{
		node->cmd->args[i] = ft_strdup(args[i]);
		if (!node->cmd->args[i])
		{
			while (i > 0)
			{
				free(node->cmd->args[i]);
				i--;
			}
			free(node->cmd->args);
			free(node->cmd);
			free(node);
			return (NULL);
		}
		i++;
	}
	node->cmd->args = args;
	node->cmd->args[args_count] = NULL;
	node->cmd->path = NULL;
	node->token = CMD;
	node->left = NULL;
	node->right = NULL;
	node->head = node;
	node->error_code = 0;
	return (node);
}

t_ast	*create_ast_operator(t_node_type token, t_ast *left, t_ast *right)
{
	t_ast *node;

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
