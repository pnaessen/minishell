/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/21 19:47:55 by aviscogl          #+#    #+#             */
/*   Updated: 2025/02/05 17:11:44 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static char	*strdup(const char *s)
{
	size_t	len;
	char	*str;
	size_t	i;

	i = 0;
	len = 0;
	while (s[len] && s[len] != '\n')
		len++;
	if (s[len] == '\n')
		len++;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	while (i < len)
	{
		str[i] = s[i];
		i++;
	}
	str[i] = '\0';
	return (str);
}

static char	*strjoin(char *s1, char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*str;

	len2 = 0;
	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	while (s2[len2] != '\n' && s2[len2] != '\0')
		len2++;
	if (s2[len2] == '\n')
		len2++;
	str = malloc(len1 + len2 + 1);
	if (!str)
	{
		free(s1);
		return (NULL);
	}
	ft_memcpy(str, s1, len1);
	ft_memcpy(str + len1, s2, len2);
	str[len1 + len2] = '\0';
	free(s1);
	return (str);
}

static void	ft_update(char *str)
{
	int	i;
	int	j;

	j = 0;
	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n')
		i++;
	while (str[i])
		str[j++] = str[i++];
	str[j] = '\0';
}

char	*get_next_line(int fd)
{
	char		*line;
	static char	str[BUFFER_SIZE + 1] = {0};
	int			index;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	index = 1;
	line = strdup(str);
	if (!line)
		return (NULL);
	while (index > 0 && !ft_strchr(line, '\n'))
	{
		index = read(fd, str, BUFFER_SIZE);
		if (index == -1)
			return (ft_bzero(str, BUFFER_SIZE), free(line), NULL);
		str[index] = '\0';
		line = strjoin(line, str);
		if (!line)
			return (NULL);
	}
	if (index == 0 && !line[0])
		return (free(line), NULL);
	ft_update(str);
	return (line);
}
