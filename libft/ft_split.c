/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:10:59 by vicperri          #+#    #+#             */
/*   Updated: 2025/03/10 10:19:14 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_free(char **res)
{
	size_t	i;

	i = 0;
	while (res[i])
	{
		free(res[i]);
		res[i] = NULL;
		i++;
	}
	free(res);
	return (0);
}

size_t	ft_reslen(const char *s1, char c)
{
	size_t	i;
	size_t	count;

	i = 0;
	count = 0;
	while (s1[i])
	{
		if (s1[i] != c && (s1[i + 1] == c || s1[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

size_t	ft_countword(const char *s1, size_t i, char c)
{
	size_t	count;

	count = 0;
	while (s1[i] == c)
		i++;
	while (s1[i])
	{
		if (s1[i] == c)
			return (count);
		i++;
		count++;
	}
	return (count);
}

char	*ft_newtabb(const char *s1, size_t size, char c, size_t i)
{
	char	*dup;
	size_t	j;

	dup = malloc((size + 1) * sizeof(char));
	j = 0;
	if (!(dup))
		return (0);
	while (s1[i] != c && s1[i])
	{
		dup[j] = s1[i];
		i++;
		j++;
	}
	dup[j] = '\0';
	return (dup);
}

char	**ft_split(char const *s, char c)
{
	char	**res;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (!(s))
		return (0);
	res = malloc((ft_reslen(s, c) + 1) * sizeof(char *));
	if (!(res))
		return (0);
	res[ft_reslen(s, c)] = NULL;
	while (s[i] && j < ft_reslen(s, c))
	{
		if (s[i] != c)
		{
			res[j] = ft_newtabb(s, ft_countword(s, i, c), c, i);
			if (!(res[j]))
				return (ft_free(res));
			i += ft_countword(s, i, c);
			j++;
		}
		i++;
	}
	return (res);
}
