/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 10:15:41 by achak             #+#    #+#             */
/*   Updated: 2024/02/08 15:57:50 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

int	new_line_check(t_list *list)
{
	int	i;

	if (list == NULL)
		return (0);
	while (list != NULL)
	{
		i = 0;
		while (list->buffer[i] != '\0')
		{
			if (list->buffer[i] == '\n')
				return (1);
			i++;
		}
		list = list->next;
	}
	return (0);
}

t_list	*find_last_node(t_list *list)
{
	if (list == NULL)
		return (NULL);
	while (list->next != NULL)
		list = list->next;
	return (list);
}

int	len_til_newline(t_list *list)
{
	int	counter;
	int	i;

	counter = 0;
	while (list != NULL)
	{
		i = 0;
		while (list->buffer[i] != '\0')
		{
			if (list->buffer[i] == '\n')
				return (++counter);
			i++;
			counter++;
		}
		list = list->next;
	}
	return (counter);
}

void	copy_next_line(t_list *list, char *next_line)
{
	int	i;
	int	j;

	j = 0;
	while (list != NULL)
	{
		i = 0;
		while (list->buffer[i] != '\0')
		{
			if (list->buffer[i] == '\n')
			{
				next_line[j++] = '\n';
				next_line[j] = '\0';
				return ;
			}
			next_line[j++] = list->buffer[i++];
		}
		list = list->next;
	}
	next_line[j] = '\0';
}

void	ft_free(t_list **list, t_list *clean_node, char *buffer, int c)
{
	t_list	*temp;

	while (*list != NULL)
	{
		temp = (*list)->next;
		free((*list)->buffer);
		free(*list);
		*list = temp;
	}
	*list = NULL;
	if (c == -1)
		return ;
	if (clean_node->buffer[0] != '\0')
		*list = clean_node;
	else
	{
		free(buffer);
		free(clean_node);
	}
}
