/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hexa_void.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 11:45:40 by vicperri          #+#    #+#             */
/*   Updated: 2025/02/03 11:14:55 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_hexa_recursive(unsigned long int p, char *base_min)
{
	int	count;

	count = 0;
	if (p >= 16)
		count += ft_hexa_recursive(p / 16, base_min);
	count += ft_putchar_printf(base_min[p % 16]);
	return (count);
}

int	ft_hexa_void(unsigned long int p)
{
	char	*base_min;
	int		count;

	base_min = "0123456789abcdef";
	count = 0;
	if (p == 0)
	{
		write(1, "(nil)", 5);
		return (5);
	}
	count += write(1, "0x", 2);
	count += ft_hexa_recursive(p, base_min);
	return (count);
}
