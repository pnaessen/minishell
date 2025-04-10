/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:01:04 by pnaessen          #+#    #+#             */
/*   Updated: 2025/04/10 15:01:05 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	apply_redirection(t_ast *redir)
{
	if (redir->token == REDIR_IN || redir->token == REDIR_HEREDOC)
		return (apply_input_redirection(redir));
	else if (redir->token == REDIR_OUT)
		return (apply_output_redirection(redir));
	else if (redir->token == APPEND)
		return (apply_append_redirection(redir));
	return (0);
}

int	save_std_fds(int *saved_stdin, int *saved_stdout, t_ast *node)
{
	*saved_stdin = dup(STDIN_FILENO);
	*saved_stdout = dup(STDOUT_FILENO);
	if (*saved_stdin == -1 || *saved_stdout == -1)
	{
		perror("minishell: dup");
		node->error_code = 1;
		if (*saved_stdin != -1)
			close(*saved_stdin);
		if (*saved_stdout != -1)
			close(*saved_stdout);
		return (1);
	}
	if (node->root)
	{
		add_fd_to_garbage(&node->root->garbage, *saved_stdin);
		add_fd_to_garbage(&node->root->garbage, *saved_stdout);
	}
	return (0);
}

void	restore_std_fds(int saved_stdin, int saved_stdout)
{
	if (dup2(saved_stdin, STDIN_FILENO) < 0)
		perror("minishell: dup2");
	if (dup2(saved_stdout, STDOUT_FILENO) < 0)
		perror("minishell: dup2");
	if (close(saved_stdin) == -1)
		perror("minishell: close");
	if (close(saved_stdout) == -1)
		perror("minishell: close");
}
