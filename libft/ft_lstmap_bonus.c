/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 08:53:49 by pnaessen          #+#    #+#             */
/*   Updated: 2024/11/15 11:58:09 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*current;

	new_list = NULL;
	current = malloc(sizeof(t_list));
	if (!current || !lst)
		return (NULL);
	current->content = f(lst->content);
	new_list = current;
	while (lst->next)
	{
		new_list->next = malloc(sizeof(t_list));
		if (!new_list->next)
			return (ft_lstclear(&current, del), NULL);
		new_list = new_list->next;
		lst = lst->next;
		new_list->content = f(lst->content);
	}
	new_list->next = NULL;
	return (current);
}
