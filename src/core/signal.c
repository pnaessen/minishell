/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:11:14 by pnaessen          #+#    #+#             */
/*   Updated: 2025/03/28 15:27:59 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		g_signal_status = 0;

void	handle_sig(int sig)
{
	if (sig == SIGINT)
	{
		g_signal_status = 130;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

int	check_heredoc_signals(void)
{
	if (g_signal_status == 131)
	{
		rl_done = 1;
		return (1);
	}
	return (0);
}

void	heredoc_sig_handler(int sig)
{
	static int	handling_signal = 0;

	if (handling_signal)
		return ;
	handling_signal = 1;
	if (sig == SIGINT)
		g_signal_status = 131;
	handling_signal = 0;
}

void	handle_signals_heredoc(void)
{
	g_signal_status = 0;
	rl_event_hook = check_heredoc_signals;
	signal(SIGINT, heredoc_sig_handler);
	signal(SIGQUIT, SIG_IGN);
}
