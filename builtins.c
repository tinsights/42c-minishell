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
void	free_cmds(void *ptr);

bool	valid_env_char(char c);

void	ms_exit(t_params *params, int code)
{
	int	i;

	if (params->paths)
	{
		i = -1;
		while (params->paths[++i])
			free(params->paths[i]);
		free(params->paths);
	}
	ft_lstclear(&(params->cmd_list), free_cmds);
	i = -1;
	while (__environ[++i])
		free_str(__environ + i);
	free(__environ);
	free_str(&(params->line));
	rl_clear_history();
	close(params->default_io[0]);
	close(params->default_io[1]);
	close(params->default_io[2]);
	exit(code);
}

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

void	ms_echo(char **argv)
{
	int		i;
	bool	newline;

	if (!argv || !argv[0])
		return ;
	newline = true;
	i = 0;
	if ((!ft_strncmp("-n", argv[i], 3)))
	{
		newline = false;
		i++;
	}
	while (argv[i])
	{
		write(STDOUT_FILENO, argv[i], ft_strlen(argv[i]));
		write(STDOUT_FILENO, " ", 1);
		i++;
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
}

void set_pwd(void)
{
	char *cwd;
	char *envwd;
	char *var;

	cwd = getcwd(NULL, 500);
	envwd = ft_strjoin("PWD", "=");
	var = ft_strjoin(envwd, cwd);
	set_env(var);
	free_str(&envwd);
	free_str(&var);
	free_str(&cwd);
}

int	ms_cd(char *argv)
{
	int result;

	result = 0;
	if (!argv || !argv[0])
		return 1;
	if (chdir(argv) < 0)
	{
		result = errno;
		perror(argv);
	}
	else
		set_pwd();
	return result;
}

void ms_pwd(void)
{
	char *cwd;

	cwd = getcwd(NULL, 500);
	printf("%s\n", cwd);
	free_str(&cwd);
}

int	run_builtin(t_params *params, t_list *cmd_lst)
{
	t_cmd	*cmd;
	char	**argv;
	int		e_status;
	int		i;

	cmd = cmd_lst->content;
	argv = cmd->words;
	e_status = 0;
	i = 0;
	if (!ft_strncmp(argv[0], "export", 7))
	{
		while (argv[++i])
			e_status |= ms_export(argv[i]);
	}
	else if (!ft_strncmp(argv[0], "env", 4))
		print_env();
	else if (!ft_strncmp(argv[0], "exit", 5))
	{
		if (params->num_cmds == 1 && !cmd_lst->next)
		// ft_putstr_fd("exit\n", STDERR_FILENO);
		ms_exit(params, 0);
	}
	else if (!ft_strncmp(argv[0], "unset", 6))
	{
		i = 0;
		while (argv[++i])
			unset_env(argv[i]);
	}
	else if (!ft_strncmp(argv[0], "echo", 5))
		ms_echo(argv + 1);
	else if (!ft_strncmp(argv[0], "cd", 3))
		e_status = ms_cd(argv[1]);
	else if (!ft_strncmp(argv[0], "pwd", 4))
		ms_pwd();
	return (e_status);
}
