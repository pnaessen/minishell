/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:10:59 by vicperri          #+#    #+#             */
/*   Updated: 2024/11/15 14:17:45 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_intlen(int n)
{
	size_t	i;

	i = 0;
	if (n <= 0)
	{
		i = 1;
	}
	while (n != 0)
	{
		n /= 10;
		i++;
	}
	return (i);
}

char	*ft_itoa(int n)
{
	char	*c;
	int		size;
	long	num;

	num = n;
	size = ft_intlen(n);
	c = (char *)malloc(size + 1 * sizeof(char));
	if (!(c))
		return (0);
	c[size--] = '\0';
	if (num < 0)
	{
		c[0] = '-';
		num *= -1;
	}
	while (num > 0)
	{
		c[size--] = num % 10 + '0';
		num /= 10;
	}
	if (n == 0)
		c[0] = '0';
	return (c);
}
