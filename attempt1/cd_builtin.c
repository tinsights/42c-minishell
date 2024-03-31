/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_builtin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 10:41:27 by achak             #+#    #+#             */
/*   Updated: 2024/03/31 18:22:50 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	free_cd_strings(char *oldpwd, char *pwd, char **dir_arr, int flag)
{
	free(oldpwd);
	if (pwd)
		free(pwd);
	if (dir_arr)
		free_array(dir_arr);
	return (flag);
}

int	cd_wrapper(t_env **head_env, char *new_path, char *path, char **dir_arr)
{
	char	*oldpwd;
	char	*pwd;
	char	*ch_path;
	int		flag;

	pwd = NULL;
	flag = 0;
	oldpwd = strjoin_and_free_str("OLDPWD=", getcwd(NULL, 200), 2);
	ch_path = path;
	if (new_path)
		ch_path = new_path;
	if (chdir(ch_path) == -1)
	{
		flag = 1;
		ft_dprintf(STDERR_FILENO, "cd: %s: %s\n", path,
			strerror(errno));
	}
	else
	{
		pwd = strjoin_and_free_str("PWD=", getcwd(NULL, 200), 2);
		update_existing_entry(oldpwd, head_env);
		update_existing_entry(pwd, head_env);
	}
	return (free_cd_strings(oldpwd, pwd, dir_arr, flag));
}

int	different_cd_criteria(t_env **head_env, char **cmd_args, char *cdpath,
		char **dir_arr)
{
	char	*new_path;
	int		rv;

	new_path = NULL;
	rv = 0;
	if (cmd_args[1][0] == '/')
	{
		if (!dir_arr)
			free(cdpath);
		return (cd_wrapper(head_env, NULL, cmd_args[1], dir_arr));
	}
	else if (!dir_arr)
	{
		new_path = strjoin_and_free_str("/", cmd_args[1], 0);
		new_path = strjoin_and_free_str(cdpath, new_path, 2);
		free(cdpath);
		rv = cd_wrapper(head_env, new_path, cmd_args[1], NULL);
		free(new_path);
	}
	else
		return (search_dir_arr(head_env, dir_arr, cmd_args[1]));
	return (rv);
}

int	cd_builtin(t_env **head_env, char **cmd_args)
{
	char	*cdpath;
	char	**dir_arr;

	cdpath = NULL;
	dir_arr = NULL;
	if (!cmd_args[1])
	{
		write(STDERR_FILENO, "cd: pathname needed\n", 20);
		return (1);
	}
	else
	{
		if (cmd_args[2])
		{
			write(STDERR_FILENO, "cd: too many arguments\n", 23);
			return (1);
		}
		cdpath = find_env_var_value(*head_env, "CDPATH");
		if (!cdpath)
			cdpath = getcwd(NULL, 200);
		else
			dir_arr = ft_split(cdpath, ':');
		return (different_cd_criteria(head_env, cmd_args, cdpath, dir_arr));
	}
}
