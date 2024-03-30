/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 13:33:43 by achak             #+#    #+#             */
/*   Updated: 2024/03/30 13:35:24 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	wrapper(int func_rv, const char *func_name)
{
	if (func_rv == -1)
		ft_dprintf(STDERR_FILENO, "%s: %s\n", func_name, strerror(errno));
}

void	choose_which_builtin_to_exec(t_params *params, int i, int *exit_status)
{
	if (!my_strncmp("echo", params->cmd_arr[i].cmd_args[0], 4))
		*exit_status = echo_builtin(params->cmd_arr[i].cmd_args);
	else if (!my_strncmp("pwd", params->cmd_arr[i].cmd_args[0], 3))
		*exit_status = pwd_builtin();
	else if (!my_strncmp("cd", params->cmd_arr[i].cmd_args[0], 2))
		*exit_status = cd_builtin(params->head_env,
				params->cmd_arr[i].cmd_args);
	else if (!my_strncmp("export", params->cmd_arr[i].cmd_args[0], 6))
		*exit_status = export_builtin(params->head_env,
				params->cmd_arr[i].cmd_args);
	else if (!my_strncmp("unset", params->cmd_arr[i].cmd_args[0], 5))
		*exit_status = unset_builtin(params->head_env,
				params->cmd_arr[i].cmd_args);
	else if (!my_strncmp("env", params->cmd_arr[i].cmd_args[0], 3))
		*exit_status = env_builtin(*(params->head_env));
	else if (!my_strncmp("exit", params->cmd_arr[i].cmd_args[0], 4))
		exit_builtin(params, params->cmd_arr[i].cmd_args);
}

void	preserve_stdin_and_stdout(t_params *params, int i, int *dup_stdin,
		int *dup_stdout)
{
	if (*(params->cmd_arr[i].stdin_fds) != -2)
	{
		*dup_stdin = dup(STDIN_FILENO);
		if (*dup_stdin == -1)
			perror("dup stdin");
		wrapper(dup2(*(params->cmd_arr[i].stdin_fds), STDIN_FILENO), "dup2");
		wrapper(close(*(params->cmd_arr[i].stdin_fds)), "close");
		if (params->cmd_arr[i].here_doc)
			wrapper(unlink(params->cmd_arr[i].here_doc), "unlink");
	}
	if (*(params->cmd_arr[i].stdout_fds) != -2)
	{
		*dup_stdout = dup(STDOUT_FILENO);
		if (*dup_stdout == -1)
			perror("dup stdout");
		wrapper(dup2(*(params->cmd_arr[i].stdout_fds), STDOUT_FILENO), "dup2");
		wrapper(close(*(params->cmd_arr[i].stdout_fds)), "close");
	}
}

void	restore_stdin_and_stdout(int dup_stdin, int dup_stdout)
{
	if (dup_stdin != -2)
	{
		wrapper(dup2(dup_stdin, STDIN_FILENO), "dup2");
		wrapper(close(dup_stdin), "close");
	}
	if (dup_stdout != -2)
	{
		wrapper(dup2(dup_stdout, STDOUT_FILENO), "dup2");
		wrapper(close(dup_stdout), "close");
	}
}

int	execute_builtin(t_params *params, int i, int flag)
{
	int	exit_status;
	int	dup_stdin;
	int	dup_stdout;

	exit_status = 0;
	dup_stdin = -2;
	dup_stdout = -2;
	if (!flag)
		preserve_stdin_and_stdout(params, i, &dup_stdin, &dup_stdout);
	choose_which_builtin_to_exec(params, i, &exit_status);
	if (flag)
	{
		free_symbol_table(params->head_env);
		handle_exit_failure(NULL, params);
		exit(exit_status);
	}
	else
		restore_stdin_and_stdout(dup_stdin, dup_stdout);
	return (exit_status);
}
