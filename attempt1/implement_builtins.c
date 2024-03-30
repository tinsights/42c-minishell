/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   implement_builtins.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 17:40:05 by achak             #+#    #+#             */
/*   Updated: 2024/03/30 12:53:53 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	exit_builtin(t_params *params, char **cmd_args)
{
	int	i;
	int	exit_status;

	i = -1;
	exit_status = 0;
	write(STDOUT_FILENO, "exit\n", 5);
	if (cmd_args[1] && cmd_args[2])
		write(STDERR_FILENO, "exit: too many arguments\n", 25);
	else if (cmd_args[1] && !cmd_args[2])
	{
		while (cmd_args[1][++i])
		{
			if (!(cmd_args[1][i] >= '0' && cmd_args[1][i] <= '9'))
			{
				write(STDERR_FILENO, "exit: numeric argument required\n", 32);
				exit_status = 1;
				break ;
			}
		}
		if (!exit_status)
			exit_status = ft_atoi(cmd_args[1]);
	}
	free_symbol_table(params->head_env);
	handle_exit_failure(NULL, params);
	exit(exit_status);
}

int	echo_builtin(char **cmd_args)
{
	int	flag;
	int	i;

	flag = 0;
	if (cmd_args[1])
	{
		if (!my_strncmp("-n", cmd_args[1], 2))
			i = 1;
		else
			i = 0;
		while (cmd_args[++i])
		{
			if (flag)
				write(STDOUT_FILENO, " ", 1);
			write(STDOUT_FILENO, cmd_args[i], my_strlen(cmd_args[i]));
			flag = 1;
		}
		if (my_strncmp("-n", cmd_args[1], 2))
			write(STDOUT_FILENO, "\n", 1);
	}
	else
		write(STDOUT_FILENO, "\n", 1);
	return (0);
}

int	pwd_builtin(void)
{
	char	*cwd;

	cwd = getcwd(NULL, 300);
	if (!cwd)
	{
		perror("pwd");
		return (2);
	}
	else
		printf("%s\n", cwd);
	free(cwd);
	return (0);
}

int	env_builtin(t_env *head_env)
{
	while (head_env)
	{
		if (head_env->value)
			printf("%s=%s\n", head_env->key, head_env->value);
		head_env = head_env->next;
	}
	return (0);
}

int	unset_builtin(t_env **head_env, char **cmd_args)
{
	int		i;
	t_env	*temp;

	i = 0;
	while (cmd_args[++i])
	{
		temp = *head_env;
		while (temp)
		{
			if (!my_strncmp(cmd_args[i], temp->key,
					my_strlen(temp->key)))
				break ;
			temp = temp->next;
		}
		if (!temp)
			continue ;
		remove_table_entry(head_env, temp->key);
	}
	return (0);
}
