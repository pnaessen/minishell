/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_tools.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:04:06 by vicperri          #+#    #+#             */
/*   Updated: 2025/04/11 15:04:22 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_signals(void)
{
	struct sigaction	sa;

	sa.sa_handler = handle_sig;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sa.sa_handler = SIG_IGN;
	sigaction(SIGQUIT, &sa, NULL);
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

int	check_signal_status(int status)
{
	rl_event_hook = NULL;
	if (g_signal_status == 131)
	{
		g_signal_status = 130;
		signal(SIGINT, handle_sig);
		signal(SIGQUIT, SIG_IGN);
		return (-2);
	}
	handle_signals();
	if (status != 0)
		g_signal_status = status;
	return (0);
}
