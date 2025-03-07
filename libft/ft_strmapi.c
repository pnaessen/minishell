/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/07 14:10:59 by vicperri          #+#    #+#             */
/*   Updated: 2024/11/15 14:18:31 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char			*str;
	char			*s1;
	unsigned int	i;

	i = 0;
	str = (char *)malloc(ft_strlen(s) + 1 * sizeof(char));
	if (!(str))
		return (0);
	s1 = (char *)s;
	while (s1[i])
	{
		str[i] = s1[i];
		str[i] = f(i, str[i]);
		i++;
	}
	str[i] = '\0';
	return (str);
}
