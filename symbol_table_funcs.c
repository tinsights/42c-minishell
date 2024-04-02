/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   symbol_table_funcs.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 19:02:10 by achak             #+#    #+#             */
/*   Updated: 2024/03/31 19:10:59 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	remove_head_ptr_or_not(t_env **head, t_env *remove_node,
		t_env *prev_node, t_env *next_node)
{
	if (!prev_node)
	{
		*head = remove_node->next;
		free(remove_node->key);
		free(remove_node->value);
		free(remove_node);
	}
	else
	{
		next_node = remove_node->next;
		free(remove_node->key);
		free(remove_node->value);
		free(remove_node);
		prev_node->next = next_node;
	}
}

void	remove_table_entry(t_env **head, char *key)
{
	t_env	*remove_node;
	t_env	*prev_node;
	t_env	*next_node;

	remove_node = *head;
	prev_node = NULL;
	next_node = NULL;
	while (remove_node)
	{
		if (!my_strncmp(remove_node->key, key,
				my_strlen(remove_node->key)))
			break ;
		prev_node = remove_node;
		remove_node = remove_node->next;
	}
	if (!remove_node)
		return ;
	remove_head_ptr_or_not(head, remove_node, prev_node, next_node);
}

char	*get_key_from_var(char *var)
{
	char	*key;
	int		i;
	int		j;

	i = -1;
	j = -1;
	while (var[++i])
		if (var[i] == '=')
			break ;
	key = (char *)malloc(sizeof(char) * (i + 1));
	if (key)
	{
		key[i] = '\0';
		while (++j < i)
			key[j] = var[j];
	}
	return (key);
}
