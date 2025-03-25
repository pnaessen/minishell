
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

int	setup_heredoc_file(t_ast *node, char *delimiter)
{
	char	*temp_filename;
	int		fd;

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
	fd = open(temp_filename, O_RDONLY);
	if (fd == -1)
	{
		free(delimiter);
		free(temp_filename);
		return (0);
	}
	close(fd);
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
			return (0);
		if (!setup_heredoc_file(node, delimiter))
			return (0);
	}
	if (node->left && !process_all_heredocs(node->left))
		return (0);
	if (node->right && !process_all_heredocs(node->right))
		return (0);
	return (1);
}
