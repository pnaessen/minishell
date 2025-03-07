/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 12:58:59 by vicperri          #+#    #+#             */
/*   Updated: 2024/11/15 12:32:36 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_comp_strnstr(const char *big, size_t i, const char *little, size_t len)
{
	size_t	j;

	j = 0;
	while (little[j])
	{
		if (big[i] != little[j] || i >= len)
			return (1);
		else
			j++;
		i++;
	}
	return (0);
}

char	*ft_strnstr(const char *big, const char *little, size_t len)
{
	size_t	i;
	char	*big1;

	big1 = (char *)big;
	i = 0;
	if (ft_strlen(little) == 0)
		return (big1);
	if (ft_strlen(little) > len)
		return (0);
	while (big[i] && i < len)
	{
		if (big[i] == little[0])
		{
			if (ft_comp_strnstr(big, i, little, len) == 0)
				return (&big1[i]);
		}
		i++;
	}
	return (0);
}
