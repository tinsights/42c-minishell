/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_symbol_table.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 10:14:56 by achak             #+#    #+#             */
/*   Updated: 2024/03/29 14:31:47 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
void	remove_table_entry(t_env *head, char *key)
{
	t_env	*temp1;
	t_env	*temp2;

	temp1 = head;
	temp2 = NULL;
	while (head)
	{
		if (!my_strncmp(head->key, key, my_strlen(head->key)))
			break ;
		temp1 = head;
		head = head->next;
	}
	if (!head)
		return ;
	temp2 = head->next;
	free(head->key);
	free(head->value);
	free(head);
	temp1->next = temp2;
}
*/

void	remove_table_entry(t_env **head, char *key)
{
	t_env	*remove_node;
	t_env	*prev_node;
	t_env	*next_node;
	int		flag;

	remove_node = *head;
	prev_node = NULL;
	next_node = NULL;
	flag = 0;
	while (remove_node)
	{
		if (!my_strncmp(remove_node->key, key,
				my_strlen(remove_node->key)))
			break ;
		prev_node = remove_node;
		remove_node = remove_node->next;
		flag = 1;
	}
	if (!remove_node)
		return ;
	if (!flag)
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

char	*get_value_from_var(char *var)
{
	char	*value;
	int		i;
	int		j;
	int		flag;

	i = -1;
	j = -1;
	flag = 0;
	while (*var)
	{
		if (*var++ == '=')
		{
			flag = 1;
			break ;
		}
	}
	while (var[++i])
		continue ;
	if (!i && !flag)
		return (NULL);
	value = (char *)malloc(sizeof(char) * (i + 1));
	if (!value)
		return (NULL);
	value[i] = '\0';
	while (var[++j])
		value[j] = var[j];
	return (value);
}

void	create_new_entry(char *var, t_env **head)
{
	t_env	*new_node;
	char	*key;
	char	*value;

	new_node = (t_env *)malloc(sizeof(t_env));
	if (!new_node)
		return ;
	if (!*head)
	{
		*head = new_node;
		new_node->next = NULL;
	}
	else
	{
		new_node->next = *head;
		*head = new_node;
	}
	key = get_key_from_var(var);
	value = get_value_from_var(var);
	new_node->key = key;
	new_node->value = value;
	new_node->diff = my_strlen(value) - (my_strlen(key) + 1);
}

t_env	*create_symbol_table(int ac, char **av, char **env)
{
	t_env	*head;
	int		i;

	head = NULL;
	i = -1;
	while (env[++i])
		create_new_entry(env[i], &head);
	return (head);
	if (ac < 2 || !av[1])
		return (head);
}

/*
int	main(int ac, char **av, char **env)
{
	t_env	*head;
	t_env	*temp;

	if (ac < 2 || !av[1])
		return (1);
	head = create_symbol_table(env);
	temp = head;
	remove_table_entry(temp, "LANGUAG");
	while (temp)
	{
		printf("key = %s\n", temp->key);
		printf("value = %s\n", temp->value);
		printf("diff = %d\n", temp->diff);
		temp = temp->next;
	}
	free_symbol_table(&head);
}
*/
