/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_symbol_table.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 10:14:56 by achak             #+#    #+#             */
/*   Updated: 2024/03/31 19:03:15 by achak            ###   ########.fr       */
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

char	*copy_value_from_var(char *value, char *var, int i)
{
	int	j;

	j = -1;
	if (value)
	{
		value[i] = '\0';
		while (var[++j])
			value[j] = var[j];
	}
	return (value);
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
	return (copy_value_from_var(value, var, i));
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
