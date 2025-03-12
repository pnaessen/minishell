/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:10:00 by pnaessen          #+#    #+#             */
/*   Updated: 2025/03/12 11:02:54 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*create_temp_filename(void)
{
	char	*filename;
	char	*pid_str;

	filename = ft_strdup("/tmp/minishell_heredoc_");
	if (!filename)
		return (NULL);
	pid_str = ft_itoa(getpid());
	if (!pid_str)
	{
		free(filename);
		return (NULL);
	}
	filename = ft_strjoin_free(filename, pid_str);
	free(pid_str);
	return (filename);
}

int	write_to_temp_file(char *delimiter, char *filename)
{
	int		temp_fd;
	char	*line;

	temp_fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (temp_fd == -1)
		return (-1);
	while (1)
	{
		line = readline("> ");
		if (!line || ft_strrcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		write(temp_fd, line, ft_strlen(line));
		write(temp_fd, "\n", 1);
		free(line);
	}
	close(temp_fd);
	return (0);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*str;

	j = 0;
	i = 0;
	str = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!str)
		return (NULL);
	while (s1[i] != '\0')
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[j] != '\0')
	{
		str[i + j] = s2[j];
		j++;
	}
	free(s1);
	str[i + j] = '\0';
	return (str);
}

void	cleanup_heredoc_files(t_ast *node)
{
	t_redir	*redir;

	if (!node)
		return ;
	if (node->token == CMD && node->cmd && node->cmd->redirs)
	{
		redir = node->cmd->redirs;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
				unlink(redir->file);
			redir = redir->next;
		}
	}
	cleanup_heredoc_files(node->left);
	cleanup_heredoc_files(node->right);
}

int	process_all_heredocs(t_ast *node)
{
	char	*temp_filename;
	char	*delimiter;
	t_redir	*redir;

	if (!node)
		return (1);
	if (node->token == CMD && node->cmd && node->cmd->redirs)
	{
		redir = node->cmd->redirs;
		while (redir)
		{
			if (redir->type == REDIR_HEREDOC)
			{
				delimiter = ft_strdup(redir->file);
				if (!delimiter)
					return (0);
				temp_filename = create_temp_filename();
				if (!temp_filename)
				{
					free(delimiter);
					return (0);
				}
				if (write_to_temp_file(delimiter, temp_filename) == -1)
				{
					free(delimiter);
					free(temp_filename);
					return (0);
				}
				free(delimiter);
				free(redir->file);
				redir->file = temp_filename;
			}
			redir = redir->next;
		}
	}
	if (node->left && !process_all_heredocs(node->left))
		return (0);
	if (node->right && !process_all_heredocs(node->right))
		return (0);
	return (1);
}
