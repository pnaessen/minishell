/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:00:56 by pnaessen          #+#    #+#             */
/*   Updated: 2025/04/10 15:00:57 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pars.h"

t_ast	*init_first_cmd(t_stack *stack, t_stack *end, t_ast **current_node)
{
	t_stack	*current;
	t_ast	*root;

	current = find_next_cmd(stack, end);
	if (current->token != CMD)
		return (NULL);
	root = create_ast_command(current->cmd);
	if (!root)
		return (NULL);
	*current_node = root;
	return (root);
}

void	set_root_pointers(t_ast *node, t_ast *root)
{
	if (!node || !root)
		return ;
	node->root = root;
	set_root_pointers(node->left, root);
	set_root_pointers(node->right, root);
}

void	handle_heredoc_case(t_ast **current_node, t_ast **root,
		t_ast *redir_node)
{
	t_ast	*cmd_node;
	t_ast	*temp;

	cmd_node = NULL;
	temp = *current_node;
	while (temp && temp->token != CMD)
		temp = temp->left;
	cmd_node = temp;
	if (cmd_node)
	{
		redir_node->left = cmd_node;
		temp = *current_node;
		while (temp->left && temp->left->token != CMD)
			temp = temp->left;
		temp->left = redir_node;
	}
	else
	{
		if (*current_node == *root)
			*root = redir_node;
		*current_node = redir_node;
	}
}

void	handle_standard_case(t_ast **current_node, t_ast **root,
		t_ast *redir_node)
{
	if (*current_node == *root)
		*root = redir_node;
	*current_node = redir_node;
}

t_ast	*create_redir_args(t_ast *redir_node, char *filename)
{
	redir_node->cmd->args = malloc(sizeof(char *) * 2);
	if (!redir_node->cmd->args)
	{
		perror("minishell: malloc create_redir_args");
		free(redir_node->cmd);
		free(redir_node);
		return (NULL);
	}
	redir_node->cmd->args[0] = ft_strdup(filename);
	if (!redir_node->cmd->args[0])
	{
		perror("minishell: strdup create_redir_args");
		free(redir_node->cmd->args);
		free(redir_node->cmd);
		free(redir_node);
		return (NULL);
	}
	filename = NULL;
	redir_node->cmd->args[1] = NULL;
	return (redir_node);
}
