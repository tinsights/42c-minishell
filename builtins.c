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

void	unset_env(char *var);
int		ms_export(char *arg);
void	print_env(void);

int	ms_echo(char **argv)
{
	int		i;
	bool	newline;

	if (!argv || !argv[0])
		return (write(STDOUT_FILENO, "\n", 1));
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
		if (argv[++i])
			write(STDOUT_FILENO, " ", 1);
	}
	if (newline)
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

void	set_pwd(void)
{
	char	*cwd;
	char	*envwd;
	char	*var;

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
	int	result;

	result = 0;
	if (!argv || !argv[0])
		return (1);
	if (chdir(argv) < 0)
	{
		result = errno;
		perror(argv);
	}
	else
		set_pwd();
	return (result);
}

void	ms_pwd(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 500);
	printf("%s\n", cwd);
	free_str(&cwd);
}

int	run_builtin(t_params *params, t_cmd *cmd)
{
	char	**argv;
	int		e_status;
	int		i;

	argv = cmd->words;
	e_status = 0;
	i = 0;
	if (!ft_strncmp(argv[0], "export", 7))
		while (argv[++i])
			e_status |= ms_export(argv[i]);
	else if (!ft_strncmp(argv[0], "env", 4))
		print_env();
	else if (!ft_strncmp(argv[0], "exit", 5))
		ms_exit(params, 0, true);
	else if (!ft_strncmp(argv[0], "unset", 6))
		while (argv[++i])
			unset_env(argv[i]);
	else if (!ft_strncmp(argv[0], "echo", 5))
		ms_echo(argv + 1);
	else if (!ft_strncmp(argv[0], "cd", 3))
		e_status = ms_cd(argv[1]);
	else if (!ft_strncmp(argv[0], "pwd", 4))
		ms_pwd();
	return (e_status);
}
