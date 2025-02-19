/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_builtins.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 12:13:15 by tjegades          #+#    #+#             */
/*   Updated: 2024/04/23 12:13:16 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	unset_env(char *var)
{
	char	*key;
	char	*loc;
	int		i;

	key = ft_strdup(var);
	loc = ft_strchr(key, '=');
	if (loc)
		*loc = '\0';
	i = 0;
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

void	set_env(char *var)
{
	char	*key;
	char	*loc;
	int		i;

	key = ft_strdup(var);
	loc = ft_strchr(key, '=');
	if (loc)
		*loc = '\0';
	i = -1;
	while (__environ[++i])
	{
		if (!ft_strncmp(__environ[i], key, ft_strlen(key)))
		{
			free(__environ[i]);
			__environ[i] = ft_strdup(var);
			break ;
		}
	}
	if (!__environ[i])
	{
		__environ = ft_realloc(__environ, (i + 1) * sizeof(char *), (i + 2)
				* sizeof(char *));
		__environ[i] = ft_strdup(var);
	}
	free(key);
}

bool	check_valid(char *arg, char **key, char **eqls)
{
	bool	valid;
	int		i;

	valid = true;
	*eqls = ft_strchr(arg, '=');
	if (*eqls && *eqls != arg)
		(*key) = ft_substr(arg, 0, *eqls - arg);
	else
		(*key) = ft_strdup(arg);
	i = -1;
	while ((*key)[++i])
		if (!valid_env_char((*key)[i]))
			valid = false;
	return (valid);
}

int	ms_export(char *arg)
{
	char	*key;
	char	*value;
	char	*equals_sign;
	bool	valid;

	valid = check_valid(arg, &key, &equals_sign);
	if (!valid)
	{
		ft_putstr_fd("export: '", 2);
		ft_putstr_fd(key, 2);
		ft_putstr_fd("' not a valid identifier\n", 2);
	}
	else if (equals_sign)
	{
		value = ft_strdup(equals_sign + 1);
		if (valid && value != NULL)
			set_env(arg);
		free_str(&value);
	}
	free_str(&key);
	return (!valid);
}

void	print_env(void)
{
	int	i;

	i = 0;
	while (__environ[i])
	{
		if (ft_strncmp(__environ[i], "?", 1) && ft_strchr(__environ[i], '='))
			printf("%s\n", __environ[i]);
		i++;
	}
}
