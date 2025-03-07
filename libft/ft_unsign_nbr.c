/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unsign_nbr.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 11:45:40 by vicperri          #+#    #+#             */
/*   Updated: 2024/12/11 14:55:57 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_unsign_nbr(unsigned int n)
{
	int	count;

	count = 0;
	if (n > 9)
	{
		count += ft_unsign_nbr((n / 10));
		count += ft_unsign_nbr((n % 10));
	}
	else
		count += ft_putchar_printf(n + 48);
	return (count);
}
