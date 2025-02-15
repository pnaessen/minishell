/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:21:52 by pnaessen          #+#    #+#             */
/*   Updated: 2025/01/21 10:42:18 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	parse_char(int c)
{
	write(1, &c, 1);
	return (1);
}

int	parse_string(char *str)
{
	int	i;

	if (str == NULL)
	{
		write(1, "(null)", 6);
		return (6);
	}
	i = 0;
	while (str[i])
		i++;
	write(1, str, i);
	return (i);
}

int	parse_int(int n)
{
	int		len;
	char	digit;

	len = 0;
	if (n == -2147483648)
	{
		write(1, "-2147483648", 11);
		return (11);
	}
	if (n < 0)
	{
		write(1, "-", 1);
		len++;
		n = -n;
	}
	if (n > 9)
		len += parse_int(n / 10);
	digit = n % 10 + '0';
	write(1, &digit, 1);
	return (len + 1);
}

int	parse_unsigned(unsigned int n)
{
	int		len;
	char	c;

	len = 0;
	if (n > 9)
		len += parse_unsigned(n / 10);
	c = (n % 10) + '0';
	write(1, &c, 1);
	return (len + 1);
}
