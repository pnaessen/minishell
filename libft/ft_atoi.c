/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/06 12:47:15 by pnaessen          #+#    #+#             */
/*   Updated: 2025/01/10 11:58:23 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_atoi(const char *nptr)
{
	size_t			i;
	int				sign;
	unsigned long	nb;

	i = 0;
	sign = 1;
	nb = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '-' || nptr[i] == '+')
		if (nptr[i++] == '-')
			sign *= -1;
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		if (((nb * 10) + (nptr[i] - '0')) > 9223372036854775807 && sign == 1)
			return (-1);
		else if (((nb * 10) + (nptr[i] - '0')) > 9223372036854775807
			&& sign == -1)
			return (0);
		nb = nb * 10 + (nptr[i++] - '0');
	}
	return (nb * sign);
}
