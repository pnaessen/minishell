/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   here_doc.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 15:10:00 by pnaessen          #+#    #+#             */
/*   Updated: 2025/03/04 16:40:12 by pnaessen         ###   ########lyon.fr   */
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
	//filename = ft_strjoin_free(filename, pid_str); need to join and free filename
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
		if (!line || ft_strcmp(line, delimiter) == 0)
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

void	execute_with_heredoc(t_ast *cmd, t_env *env, char *filename)
{
	int	saved_fd;
	int	temp_fd;

	temp_fd = open(filename, O_RDONLY);
	if (temp_fd == -1)
	{
		cmd->error_code = 1;
		return ;
	}
	saved_fd = dup(STDIN_FILENO);
	dup2(temp_fd, STDIN_FILENO);
	close(temp_fd);
	execute_ast(cmd->left, env);
	dup2(saved_fd, STDIN_FILENO);
	close(saved_fd);
}

void	handle_heredoc(t_ast *cmd, t_env *env)
{
	char	*temp_filename;
	char	*delimiter;

	delimiter = cmd->right->cmd->args[0];
	temp_filename = create_temp_filename();
	if (!temp_filename)
	{
		cmd->error_code = 1;
		return ;
	}
	if (write_to_temp_file(delimiter, temp_filename) == -1)
	{
		perror("minishell: heredoc");
		cmd->error_code = 1;
		free(temp_filename);
		return ;
	}
	execute_with_heredoc(cmd->left, env, temp_filename);
	unlink(temp_filename);
	free(temp_filename);
}
