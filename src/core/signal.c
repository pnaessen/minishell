/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pn <pn@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/19 14:11:14 by pnaessen          #+#    #+#             */
/*   Updated: 2025/03/26 20:54:15 by pn               ###   ########lyon.fr   */
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

void	handle_signals(void)
{
	signal(SIGINT, handle_sig); // CTRL+C
	signal(SIGQUIT, SIG_IGN);   // CTRL+ backslash
}

void	handle_signals_child(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
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
