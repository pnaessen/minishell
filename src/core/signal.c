/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:11:14 by pnaessen          #+#    #+#             */
/*   Updated: 2025/03/21 17:31:00 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sig(int sig)
{
	if (sig == SIGINT)
	{
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	handle_signals(void)
{
	signal(SIGINT, &handle_sig);
	signal(SIGQUIT, SIG_IGN);
}

void	handle_sig_subshell(int sig)
{
	if (sig == SIGINT)
	{
		//write(STDOUT_FILENO, "\n", 1);
		exit(130);
	}
}

void	handle_signals_subshell(void)
{
	signal(SIGINT, &handle_sig_subshell);
	signal(SIGQUIT, SIG_DFL);
}

