/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 11:15:06 by vicperri          #+#    #+#             */
/*   Updated: 2024/11/15 12:13:46 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_lst;
	t_list	*head;

	new_lst = NULL;
	head = (t_list *)malloc(sizeof(t_list));
	if (!head || !lst)
		return (0);
	head->content = f(lst->content);
	new_lst = head;
	while (lst->next != NULL)
	{
		new_lst->next = (t_list *)malloc(sizeof(t_list));
		if (!new_lst->next)
		{
			ft_lstclear(&head, del);
			return (0);
		}
		new_lst = new_lst->next;
		lst = lst->next;
		new_lst->content = f(lst->content);
	}
	new_lst->next = NULL;
	return (head);
}
