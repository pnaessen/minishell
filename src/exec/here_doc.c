/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:01:43 by pnaessen          #+#    #+#             */
/*   Updated: 2025/04/11 14:38:43 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin_free(char *s1, char *s2)
{
	int		i;
	int		j;
	char	*str;

	j = -1;
	i = 0;
	str = (char *)malloc((ft_strlen(s1) + ft_strlen(s2) + 1) * sizeof(char));
	if (!str)
	{
		free(s1);
		return (NULL);
	}
	while (s1[i] != '\0')
	{
		str[i] = s1[i];
		i++;
	}
	while (s2[++j] != '\0')
		str[i + j] = s2[j];
	free(s1);
	free(s2);
	str[i + j] = '\0';
	return (str);
}

int	handle_temp_file_error(char *delimiter, char *temp_filename)
{
	if (!delimiter)
		free(delimiter);
	if (!temp_filename)
		free(temp_filename);
	return (0);
}

int	setup_temp_file(char *delimiter, char *temp_filename)
{
	int	fd;
	int	write_result;

	write_result = write_to_temp_file(delimiter, temp_filename);
	if (write_result <= -1)
	{
		return (handle_temp_file_error(delimiter, temp_filename));
	}
	fd = open(temp_filename, O_RDONLY);
	if (fd == -1)
	{
		perror("minishell: open setup_heredoc_file");
		return (handle_temp_file_error(delimiter, temp_filename));
	}
	if (close(fd) == -1)
		perror("minishell: close");
	return (1);
}

int	setup_heredoc_file(t_ast *node, char *delimiter)
{
	char	*temp_filename;

	temp_filename = create_temp_filename();
	if (!temp_filename)
	{
		free(delimiter);
		return (0);
	}
	if (!setup_temp_file(delimiter, temp_filename))
	{
		free(temp_filename);
		free(delimiter);
		return (0);
	}
	free(delimiter);
	free(node->cmd->args[0]);
	node->cmd->args[0] = temp_filename;
	node->cmd->has_heredoc = 1;
	return (1);
}

int	process_all_heredocs(t_ast *node)
{
	char	*delimiter;

	if (!node)
		return (1);
	if (node->token == REDIR_HEREDOC && node->cmd && node->cmd->args)
	{
		delimiter = ft_strdup(node->cmd->args[0]);
		if (!delimiter)
		{
			perror("minishell: strdup process_all_heredocs");
			return (0);
		}
		if (!setup_heredoc_file(node, delimiter))
			return (0);
	}
	if (node->left && !process_all_heredocs(node->left))
		return (0);
	if (node->right && !process_all_heredocs(node->right))
		return (0);
	return (1);
}
