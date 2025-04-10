/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc_tools.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 09:03:22 by pnaessen          #+#    #+#             */
/*   Updated: 2025/04/10 09:08:16 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_temp_filename(void)
{
	char		*filename;
	char		*counter_str;
	static int	counter = 0;

	filename = ft_strdup("/tmp/minishell_heredoc_");
	if (!filename)
		return (NULL);
	counter_str = ft_itoa(counter++);
	if (!counter_str)
	{
		free(filename);
		return (NULL);
	}
	filename = ft_strjoin_free(filename, counter_str);
	free(counter_str);
	return (filename);
}

int	handle_heredoc_line(char *delimiter, char *line, int temp_fd)
{
	if (!line || g_signal_status == 131 || ft_strcmp(line, delimiter) == 0)
	{
		free(line);
		return (1);
	}
	write(temp_fd, line, ft_strlen(line));
	write(temp_fd, "\n", 1);
	free(line);
	return (0);
}

int	write_to_temp_file(char *delimiter, char *filename)
{
	int		temp_fd;
	char	*line;
	int		status;
	int		tmp;

	status = g_signal_status;
	g_signal_status = 0;
	handle_signals_heredoc();
	temp_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (temp_fd == -1)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (handle_heredoc_line(delimiter, line, temp_fd))
			break ;
	}
	if (close(temp_fd) == -1)
		perror("minishell: close");
	tmp = check_signal_status(status);
	return (tmp);
}
