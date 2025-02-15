/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tools2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/17 16:31:14 by pnaessen          #+#    #+#             */
/*   Updated: 2024/12/07 11:04:01 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	parse_hex(unsigned long int n)
{
	int		len;
	char	c;

	len = 0;
	if (n > 15)
		len += parse_hex(n / 16);
	if (n % 16 < 10)
	{
		c = (n % 16) + '0';
		write(1, &c, 1);
	}
	else
	{
		c = (n % 16) - 10 + 'a';
		write(1, &c, 1);
	}
	return (len + 1);
}

int	parse_hex_upper(unsigned long int n)
{
	int		len;
	char	c;

	len = 0;
	if (n > 15)
		len += parse_hex_upper(n / 16);
	if (n % 16 < 10)
	{
		c = (n % 16) + '0';
		write(1, &c, 1);
	}
	else
	{
		c = (n % 16) - 10 + 'A';
		write(1, &c, 1);
	}
	return (len + 1);
}

int	parse_pointer(void *ptr)
{
	if (ptr == NULL)
	{
		write(1, "(nil)", 5);
		return (5);
	}
	write(1, "0x", 2);
	return (parse_hex((unsigned long int)ptr) + 2);
}
