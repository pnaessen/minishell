/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_init.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 09:00:03 by pnaessen          #+#    #+#             */
/*   Updated: 2025/04/10 09:02:07 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_cmd_true(t_ast **root)
{
	*root = malloc(sizeof(t_ast));
	if (!*root)
		return ;
	(*root)->token = CMD;
	(*root)->cmd = malloc(sizeof(t_cmd));
	if (!(*root)->cmd)
	{
		free_cmd_true(root);
		return ;
	}
	(*root)->cmd->args = malloc(sizeof(char *) * 2);
	if (!(*root)->cmd->args)
	{
		free_cmd_true(root);
		return ;
	}
	(*root)->cmd->args[0] = ft_strdup("true");
	if (!(*root)->cmd->args[0])
	{
		free_cmd_true(root);
		return ;
	}
	(*root)->cmd->args[1] = NULL;
	(*root)->cmd->path = NULL;
	(*root)->cmd->has_heredoc = 0;
}

void	init_cmd_node_base(t_ast *root)
{
	root->left = NULL;
	root->right = NULL;
	root->head = root;
	root->error_code = 0;
}

t_ast	*init_process_result(t_stack *cmd_token, t_stack **redir_tokens)
{
	t_ast	*cmd_node;

	if (!cmd_token || cmd_token->token != CMD || !cmd_token->cmd)
		cmd_node = create_cmd_node(NULL);
	else
		cmd_node = create_cmd_node(cmd_token);
	if (!cmd_node)
	{
		free(redir_tokens);
		return (NULL);
	}
	return (cmd_node);
}

void	init_redir_properties(t_ast *redir_node, t_ast *cmd_node)
{
	redir_node->cmd->path = NULL;
	redir_node->cmd->has_heredoc = 0;
	redir_node->left = cmd_node;
	redir_node->right = NULL;
	redir_node->head = redir_node;
	redir_node->error_code = 0;
	redir_node->root = NULL;
}

void	replace_cmd_with_redir(t_ast **result, t_ast *target_cmd,
		t_ast *redir_node)
{
	t_ast	*parent;

	if (*result == target_cmd)
		*result = redir_node;
	else
	{
		parent = *result;
		while (parent->left && parent->left != target_cmd)
			parent = parent->left;
		if (parent->left == target_cmd)
			parent->left = redir_node;
	}
}
