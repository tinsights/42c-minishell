/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 19:28:30 by achak             #+#    #+#             */
/*   Updated: 2024/03/31 18:24:02 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

char	*find_env_var_value(t_env *head_env, char *var)
{
	while (head_env)
	{
		if (!my_strncmp(var, head_env->key, my_strlen(var)))
			break ;
		head_env = head_env->next;
	}
	if (!head_env)
		return (NULL);
	if (!head_env->value)
		return (NULL);
	return (head_env->value);
}

int	check_valid_dir(t_env **head_env, char *dir_arr, char **new_path,
		char *path)
{
	struct stat	statbuf;
	int			flag;

	flag = -1;
	*new_path = strjoin_and_free_str(dir_arr, path, 0);
	if (stat(*new_path, &statbuf) == 0 && S_ISDIR(statbuf.st_mode))
	{
		flag = cd_wrapper(head_env, *new_path, path, NULL);
		if (!flag)
			printf("%s\n", *new_path);
		free(*new_path);
		return (flag);
	}
	return (flag);
}

int	check_present_dir(t_env **head_env, char *path)
{
	char	*str;
	int		rv;

	str = strjoin_and_free_str(getcwd(NULL, 200), "/", 1);
	str = strjoin_and_free_str(str, path, 1);
	rv = cd_wrapper(head_env, str, path, NULL);
	free(str);
	return (rv);
}

int	search_dir_arr(t_env **head_env, char **dir_arr, char *path)
{
	int			i;
	int			j;
	int			flag;
	char		*new_path;

	i = -1;
	flag = -1;
	new_path = NULL;
	while (dir_arr[++i])
	{
		j = -1;
		while (dir_arr[i][++j])
			continue ;
		if (dir_arr[i][--j] != '/')
			dir_arr[i] = strjoin_and_free_str(dir_arr[i], "/", 1);
		flag = check_valid_dir(head_env, dir_arr[i], &new_path, path);
		if (flag != -1)
			break ;
		free(new_path);
	}
	free_array(dir_arr);
	if (flag == -1)
		return (check_present_dir(head_env, path));
	return (flag);
}
