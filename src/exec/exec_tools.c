/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_tools.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:01:25 by pnaessen          #+#    #+#             */
/*   Updated: 2025/04/10 15:01:26 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	setup_pipe(t_ast *cmd, int pipefd[2])
{
	if (pipe(pipefd) == -1)
	{
		perror("minishell: pipe");
		cmd->error_code = 1;
		return (0);
	}
	return (1);
}

void	cleanup_pipe(int pipefd[2])
{
	if (close(pipefd[0]) == -1)
		perror("minishell: close");
	if (close(pipefd[1]) == -1)
		perror("minishell: close");
}

void	handle_ast_node(t_ast *node, t_env **env)
{
	if (node->token == CMD)
		execute_cmd_node(node, env);
	else if (node->token == PIPE)
		execute_pipe(node, env);
	else if (node->token == REDIR_IN || node->token == REDIR_OUT
		|| node->token == APPEND || node->token == REDIR_HEREDOC)
		exec_with_redirects(node, env);
}

void	update_error_codes(t_ast *node, t_env **env)
{
	if (node->head != node && node->head)
		node->head->error_code = node->error_code;
	if (!env || !*env)
		return ;
	(*env)->error_code = node->error_code;
}

void	fork_fail(t_ast **cmd, int *pipefd)
{
	perror("minishell: fork");
	if (close(pipefd[0]) == -1)
		perror("minishell: close");
	if (close(pipefd[1]) == -1)
		perror("minishell: close");
	(*cmd)->error_code = 1;
}
