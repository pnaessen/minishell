/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/04 12:58:59 by vicperri          #+#    #+#             */
/*   Updated: 2024/11/15 12:36:54 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	lend;
	size_t	lens;
	size_t	i;
	size_t	nb;

	i = 0;
	lend = ft_strlen(dst);
	lens = ft_strlen(src);
	nb = lend;
	if (dstsize == 0)
		return (lens + dstsize);
	while (nb < dstsize - 1 && src[i])
	{
		dst[nb] = src[i];
		i++;
		nb++;
	}
	dst[nb] = '\0';
	if (lend > dstsize)
		return (lens + dstsize);
	return (lend + lens);
}
