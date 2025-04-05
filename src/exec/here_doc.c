
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

int	write_to_temp_file(char *delimiter, char *filename)
{
	int		temp_fd;
	char	*line;
	int		status;

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
	return (check_signal_status(status));
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
	{
		free(s1);
		return (NULL);
	}
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
		return (handle_temp_file_error(delimiter, temp_filename));
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
		return (0);
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
