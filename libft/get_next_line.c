/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 16:00:27 by vicperri          #+#    #+#             */
/*   Updated: 2024/12/11 14:58:23 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*join(char *line, char *readbuffer)
{
	char	*str;
	size_t	size;
	size_t	i;
	size_t	j;

	i = 0;
	if (!line || !readbuffer)
		return (NULL);
	j = ft_strlen(line);
	while (readbuffer[i] && readbuffer[i] != '\n')
		i++;
	if (readbuffer[i] == '\n')
		i++;
	size = i + j;
	str = malloc((size + 1) * sizeof(char));
	if (!str)
		return (free(line), NULL);
	ft_memcpy(str, line, j);
	i = 0;
	while (readbuffer[i] && readbuffer[i] != '\n')
		str[j++] = readbuffer[i++];
	if (readbuffer[i] == '\n')
		str[j++] = readbuffer[i++];
	str[j] = '\0';
	return (free(line), str);
}

void	ft_update(char *readbuffer)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	while (readbuffer[i] && readbuffer[i] != '\n')
		i++;
	if (readbuffer[i] == '\n')
		i++;
	while (readbuffer[i])
	{
		readbuffer[j] = readbuffer[i];
		i++;
		j++;
	}
	readbuffer[j] = '\0';
}

char	*dup_gnl(char *s1)
{
	size_t	len;
	char	*dup;
	size_t	i;

	len = 0;
	i = 0;
	while (s1[len] && s1[len] != '\n')
		len++;
	if (s1[len] == '\n')
		len++;
	dup = malloc((len + 1) * sizeof(char));
	if (!dup)
		return (NULL);
	while (i < len)
	{
		dup[i] = s1[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}

char	*get_next_line(int fd)
{
	static char	readbuffer[BUFFER_SIZE + 1] = {0};
	char		*line;
	int			bytes;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	bytes = 1;
	line = dup_gnl(readbuffer);
	if (!line)
		return (NULL);
	while (bytes > 0 && !ft_strchr(line, '\n'))
	{
		bytes = read(fd, readbuffer, BUFFER_SIZE);
		if (bytes == -1)
			return (ft_bzero(readbuffer, BUFFER_SIZE), free(line), NULL);
		readbuffer[bytes] = '\0';
		line = join(line, readbuffer);
		if (!line)
			return (NULL);
	}
	if (!line[0] && bytes == 0)
		return (free(line), NULL);
	ft_update(readbuffer);
	return (line);
}

// #include <fcntl.h>

// int	main(void)
// {
// 	int fd = open("hello.txt", O_RDONLY);
// 	char *line;

// 	if (fd < 0)
// 		return (1);

// 	while ((line = get_next_line(fd)) != NULL)
// 	{
// 		printf("%s", line);
// 		free(line);
// 	}
// 	close(fd);
// 	return (0);
// }