/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pn <pn@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 12:04:20 by pnaessen          #+#    #+#             */
/*   Updated: 2025/04/06 18:32:19 by pn               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count_word(char const *s, char c);
static int	ft_len(char const *s, int i, char c);
static char	*ft_strndup(char const *s, int i, int size, char c);
char		**ft_free(char **tab);

char	**ft_split(char const *s, char c)
{
	char	**result;
	int		i;
	int		j;

	if (*s == '\0')
		return (NULL);
	result = (char **)malloc(sizeof(char *) * (ft_count_word(s, c) + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (j < ft_count_word(s, c))
	{
		while (s[i] == c)
			i++;
		result[j] = ft_strndup(s, i, ft_len(s, i, c), c);
		if (!result[j])
			return (ft_free(result));
		while (s[i] && s[i] != c)
			i++;
		j++;
	}
	result[j] = NULL;
	return (result);
}

static int	ft_count_word(char const *s, char c)
{
	int		count;
	int		in_word;
	size_t	i;

	count = 0;
	in_word = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && in_word == 0)
		{
			in_word = 1;
			count++;
		}
		else if (s[i] == c)
			in_word = 0;
		i++;
	}
	return (count);
}

static int	ft_len(char const *s, int i, char c)
{
	int	len;

	len = 0;
	while (s[i] && s[i] != c)
	{
		len++;
		i++;
	}
	return (len);
}

static char	*ft_strndup(char const *s, int i, int size, char c)
{
	char	*dup;
	int		j;

	dup = (char *)malloc(sizeof(char) * (size + 1));
	if (!dup)
		return (NULL);
	j = 0;
	while (j < size && s[i] && s[i] != c)
	{
		dup[j] = s[i];
		i++;
		j++;
	}
	dup[j] = '\0';
	return (dup);
}

char	**ft_free(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}
