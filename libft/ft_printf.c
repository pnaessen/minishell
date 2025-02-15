/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/15 20:19:38 by aviscogl          #+#    #+#             */
/*   Updated: 2024/12/07 11:03:44 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		count;

	count = 0;
	if (format == NULL)
		return (-1);
	va_start(args, format);
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			count += parse_format(&format, args);
		}
		else
		{
			parse_char(*format);
			count++;
		}
		format++;
	}
	va_end(args);
	return (count);
}

int	parse_format(const char **format, va_list args)
{
	int	count;

	count = 0;
	if (**format == 'c')
		count = parse_char(va_arg(args, int));
	else if (**format == 's')
		count = parse_string(va_arg(args, char *));
	else if (**format == 'd' || **format == 'i')
		count = parse_int(va_arg(args, int));
	else if (**format == 'u')
		count = parse_unsigned(va_arg(args, unsigned int));
	else if (**format == 'x')
		count = parse_hex(va_arg(args, unsigned int));
	else if (**format == 'X')
		count = parse_hex_upper(va_arg(args, unsigned int));
	else if (**format == 'p')
		count = parse_pointer(va_arg(args, void *));
	else if (**format == '%')
		count = parse_char('%');
	else
		return (0);
	return (count);
}
