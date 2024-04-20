/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:16:32 by tjegades          #+#    #+#             */
/*   Updated: 2024/04/20 11:16:33 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "minishell.h"

void	free_str(char **p);
void    free_cmds(void *ptr);


bool    valid_env_char(char c);


void ms_exit(t_params *params, int code)
{

	if (params->paths)
	{
	for (int i = 0; params->paths[i]; i++)
	{
		// printf("%s\n", params->paths[i]);
		free(params->paths[i]);
	}

	free(params->paths);
	}

	ft_lstclear(&(params->cmd_list), free_cmds);

	int i = 0;
	while (__environ[i])
		i++;
	if (i > params->env_count)
	{
		for (int i = params->env_count; __environ[i]; i++)
			free_str(__environ + i);
		free(__environ);
	}

	free_str(&(params->line));
	exit(code);
}

void unset_env(char *var)
{
	char *key = ft_strdup(var);
	char *loc = ft_strchr(key, '=');
	if (loc) *loc = '\0';  // split the key and value

	// check if key exists in environ
	// if yes, free and replace

	int i = 0;
	while (__environ[i])
	{
		if (!ft_strncmp(__environ[i], key, ft_strlen(key)))
		{
			__environ[i][ft_strlen(key)] = '\0';
			break ;
		}
		i++;
	}
	free(key);
}

void set_env(char *var)
{
    char *key = ft_strdup(var);
    char *loc = ft_strchr(key, '=');
    if (loc) *loc = '\0';  // split the key and value

	// check if key exists in environ
	// if yes, free and replace

	int i = 0;
	while (__environ[i])
	{
		if (!ft_strncmp(__environ[i], key, ft_strlen(key)))
		{
			free(__environ[i]);
			__environ[i] = ft_strdup(var);
			break ;
		}
		i++;
	}
	if (!__environ[i])
	{
		__environ = ft_realloc(__environ, i * sizeof(char *), (i + 2) * sizeof(char *));
		__environ[i] = ft_strdup(var);
		__environ[i + 1] = NULL;
	}
	free(key);

}

void ms_export(char *arg)
{
	char *key = NULL;
	char *value = NULL;
	char *equals_sign = NULL;
	bool valid = true;


	equals_sign = ft_strchr(arg, '=');
	key = ft_substr(arg, 0, equals_sign - arg);

	// printf("key: %s\n", key);
	for (int i = 0; key[i]; i++)
		if (!valid_env_char(key[i]))
			valid = false;

	if (valid)
	{
		value = ft_strdup(equals_sign + 1);
		if (valid && value != NULL)
				set_env(arg);
	}
	else
	{
		ft_putstr_fd("Invalid var: ", 2);
		ft_putstr_fd(key, 2);
		ft_putstr_fd("\n", 2);
	}

	free_str(&key);
	free_str(&value);
}

void print_env(void)
{
    int i = 0;
    while (__environ[i])
    {
        printf("%s\n", __environ[i]);
        i++;
    }
}
