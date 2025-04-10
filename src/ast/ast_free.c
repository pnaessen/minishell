/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ast_free.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 14:59:35 by pnaessen          #+#    #+#             */
/*   Updated: 2025/04/10 14:59:36 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pars.h"

void	free_ast_cmd(t_ast *node)
{
	if (!node || !node->cmd)
		return ;
	if (node->cmd->args)
		free_ast_cmd_args(node);
	if (node->cmd->path)
		free(node->cmd->path);
	if (node->cmd)
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
	free_ast_cmd(node);
	free(node);
}

void	free_ast_cmd_args(t_ast *node)
{
	int	i;

	if (!node || !node->cmd || !node->cmd->args)
		return ;
	i = 0;
	while (node->cmd->args[i])
	{
		free(node->cmd->args[i]);
		i++;
	}
	free(node->cmd->args);
	node->cmd->args = NULL;
}
