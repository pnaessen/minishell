/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/18 09:22:21 by vicperri          #+#    #+#             */
/*   Updated: 2024/12/11 14:49:09 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	conversion_format(const char *format, int i, va_list args)
{
	int	count;

	count = 0;
	if (format[i] == 'c')
		count = ft_putchar_printf(va_arg(args, int));
	if (format[i] == 's')
		count = ft_putstr_printf(va_arg(args, char *));
	if (format[i] == 'd' || format[i] == 'i')
		count = ft_putnbr_printf(va_arg(args, int));
	if (format[i] == 'u')
		count = ft_unsign_nbr(va_arg(args, unsigned int));
	if (format[i] == 'p')
		count = ft_hexa_void(va_arg(args, unsigned long int));
	if (format[i] == 'x')
		count = ft_hexa_nbr(va_arg(args, unsigned int), format[i]);
	if (format[i] == 'X')
		count = ft_hexa_nbr(va_arg(args, unsigned int), format[i]);
	if (format[i] == '%')
		count = ft_putchar_printf('%');
	return (count);
}

int	ft_printf(const char *format, ...)
{
	int		i;
	int		len;
	va_list	args;

	i = 0;
	len = 0;
	va_start(args, format);
	if (format == NULL)
		return (-1);
	while (format[i])
	{
		if (format[i] == '%')
		{
			i++;
			len += conversion_format(format, i, args);
		}
		else
		{
			write(1, &format[i], 1);
			len++;
		}
		i++;
	}
	va_end(args);
	return (len);
}
