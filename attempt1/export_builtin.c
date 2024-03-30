/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_builtin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 16:37:34 by achak             #+#    #+#             */
/*   Updated: 2024/03/30 18:46:20 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

//	if (success)
//		return (0);
//	else if (at least 1 invalid var name)
//		return (2);
int	check_if_entry_exists(char *var, t_env *head_env)
{
	int	i;

	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	while (head_env)
	{
		if (!my_strncmp(var, head_env->key, i))
			break ;
		head_env = head_env->next;
	}
	if (!head_env)
		return (0);
	return (1);
}

void	update_existing_entry(char *var, t_env **head_env)
{
	t_env	*update_node;
	int		i;
	char	*value;

	update_node = *head_env;
	i = 0;
	while (var[i] && var[i] != '=')
		i++;
	while (update_node)
	{
		if (!my_strncmp(var, update_node->key, i))
			break ;
		update_node = update_node->next;
	}
	value = get_value_from_var(var);
	if (!value || !update_node)
		return ;
	if (update_node->value)
		free(update_node->value);
	update_node->value = value;
}

int	check_valid_var_name(char *var)
{
	int	j;

	j = 0;
	if (!is_alphabet(var[0]))
	{
		printf("export: `%s': not a valid identifier\n", var);
		return (0);
	}
	while (var[++j])
	{
		if (!is_alphabet(var[j]) && !is_numeric(var[j]) && var[j] != '_')
		{
			printf("export: `%s': not a valid identifier\n", var);
			return (0);
		}
	}
	return (1);
}

int	export_var_into_env_list(t_env **head_env, char **cmd_args)
{
	int	i;
	int	flag;

	i = 0;
	flag = 1;
	while (cmd_args[++i])
	{
		if (!check_valid_var_name(cmd_args[i]))
		{
			flag = 0;
			continue ;
		}
		if (!check_if_entry_exists(cmd_args[i], *head_env))
			create_new_entry(cmd_args[i], head_env);
		else
			update_existing_entry(cmd_args[i], head_env);
	}
	return (flag);
}

int	export_builtin(t_env **head_env, char **cmd_args)
{
	int		i;
	int		j;
	int		flag;
	t_env	*temp;

	i = 0;
	j = 1;
	flag = 0;
	if (!cmd_args[1])
	{
		temp = *head_env;
		while (temp)
		{
			printf("declare -x %s", temp->key);
			if (temp->value)
				printf("=\"%s\"\n", temp->value);
			else
				printf("\n");
			temp = temp->next;
		}
	}
	else
		if (!export_var_into_env_list(head_env, cmd_args))
			return (2);
	return (0);
}

/*
		while (cmd_args[++i])
		{
			if (!((cmd_args[i][0] >= 'a' && cmd_args[i][0] <= 'z')
				|| (cmd_args[i][0] >= 'A' && cmd_args[i][0]
				<= 'Z')) && cmd_args[i][0] != '_')
			{
				printf("export: `%s': not a valid identifier\n",
					cmd_args[i]);
				flag = 1;
				continue ;
			}
			while (cmd_args[i][j] && cmd_args[i][j] != '=')
			{
				if (!((cmd_args[i][j] >= 'a' &&
					cmd_args[i][j] <= 'z')
				|| (cmd_args[i][j] >= 'A' && cmd_args[i][j] <= 'Z')
				|| (cmd_args[i][j] >= '0' && cmd_args[i][j] <= '9'))
					&& cmd_args[i][j] != '_')
				{
					printf("export: `%s': not a valid identifier\n",
						cmd_args[i]);
					flag = 2;
					break ;
				}
				j++;
			}
			if (flag != 2)
			{
				if (!check_if_entry_exists(cmd_args[i], *head_env))
					create_new_entry(cmd_args[i], head_env);
				else
					update_existing_entry(cmd_args[i], head_env);
			}
		}

int	main(int ac, char **av, char **env)
{
	static char	*line_read;
	char	*prompt;
	t_env	*head_env;

	if (ac < 2 || !av[1])
		return (1);
	head_env = create_symbol_table(env);
	while (1)
	{
		if (line_read)
		{
			free(line_read);
			line_read = NULL;
		}
		prompt = strjoin_and_free_str(getcwd(NULL, 200), "$ ", 1);
		line_read = readline(prompt);
		if (line_read && *line_read)
			add_history(line_read);
		char	**arr = ft_split(line_read, ' ');
		check_if_builtin(&head_env, arr);
//		int	rv = check_if_builtin(&head_env, arr);
//		printf("checking head ptr of env list: head_env->key = %s\n", head_env->key);
//		printf("return value of builtin = %d\n", rv);
//		export_builtin(&head_env, arr);
//		printf("return value of export = %d\n", rv);

	}
}
*/
