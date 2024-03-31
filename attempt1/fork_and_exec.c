/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork_and_exec.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 10:28:54 by achak             #+#    #+#             */
/*   Updated: 2024/03/31 19:59:04 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	check_if_cmd_is_builtin(char *cmd)
{
	if (!my_strncmp("echo", cmd, 4))
		return (1);
	else if (!my_strncmp("pwd", cmd, 3))
		return (1);
	else if (!my_strncmp("cd", cmd, 2))
		return (1);
	else if (!my_strncmp("export", cmd, 6))
		return (1);
	else if (!my_strncmp("unset", cmd, 5))
		return (1);
	else if (!my_strncmp("env", cmd, 3))
		return (1);
	else if (!my_strncmp("exit", cmd, 4))
		return (1);
	else
		return (0);
}

int	check_if_fork_needed(t_params *params)
{
	if (params->cmd_nbr > 1)
		return (1);
	if (check_if_cmd_is_builtin(params->cmd_arr[0].cmd_args[0]))
		return (0);
	else
		return (1);
}

void	fork_and_exec(t_params *params, int *new_fds, int *old_fds)
{
	int		i;
	pid_t	pids;

	i = -1;
	while (++i < params->cmd_nbr)
	{
		if (i + 1 != params->cmd_nbr)
			wrapper(pipe(new_fds), "pipe");
		wrapper(pids = fork(), "fork");
		if (pids == 0)
			child_process(params, i, new_fds, old_fds);
		else
			parent_process(params, i, new_fds, old_fds);
	}
}

void	preparing_fork_and_exec(t_params *params)
{
	int		i;
	int		new_fds[2];
	int		*old_fds;

	i = -1;
	old_fds = NULL;
	if (!check_if_fork_needed(params))
		execute_builtin(params, 0, 0, NULL);
	else
	{
		if (params->cmd_nbr > 1)
		{
			old_fds = malloc(sizeof(int) * 2);
			if (!old_fds)
				return ;
			old_fds[0] = -1;
			old_fds[1] = -1;
		}
		fork_and_exec(params, new_fds, old_fds);
		if (old_fds)
			free(old_fds);
		handle_exit_failure(NULL, params);
	}
}

/*
void	child_process(t_params *params, int i, int *new_fds, int (*old_fds)[2])
{
	if (i == 0 && *(params->cmd_arr[i].stdin_fds) != 2)
	{
		wrapper(dup2(*(params->cmd_arr[i].stdin_fds),
			STDIN_FILENO), "dup2");
		wrapper(close(*(params->cmd_arr[i].stdin_fds)), "close");
		if (params->cmd_arr[i].here_doc)
			wrapper(unlink(params->cmd_arr[i].here_doc), "unlink");
	}
	else
	{
		wrapper(close((*old_fds)[1]), "close");
		if (*(params->cmd_arr[i].stdin_fds) != -2)
		{
			wrapper(close((*old_fds)[0]), "close");
			wrapper(dup2(*(params->cmd_arr[i].stdin_fds),
				STDIN_FILENO), "dup2");
			wrapper(close(*(params->cmd_arr[i].stdin_fds)), "close");
			if (params->cmd_arr[i].here_doc)
				wrapper(unlink(params->cmd_arr[i].here_doc), "unlink");
		}
		else
		{
			wrapper(dup2((*old_fds)[0], STDIN_FILENO), "dup2");
			wrapper(close((*old_fds)[0]), "close");
		}
	}
	if (i == params->cmd_nbr - 1 && *(params->cmd_arr[i].stdout_fds) != 2)
	{
		wrapper(dup2(*(params->cmd_arr[i].stdout_fds),
			STDOUT_FILENO), "dup2");
		wrapper(close(*(params->cmd_arr[i].stdout_fds)), "close");
	}
	else
	{
		wrapper(close(new_fds[0]), "close");
		if (*(params->cmd_arr[i].stdout_fds) != -2)
		{
			wrapper(close(new_fds[1]), "close");
			wrapper(dup2(*(params->cmd_arr[i].stdout_fds),
				STDOUT_FILENO), "dup2");
			wrapper(close(*(params->cmd_arr[i].stdout_fds)), "close");
		}
		else
		{
			wrapper(dup2(new_fds[1], STDOUT_FILENO), "dup2");
			wrapper(close(new_fds[1]), "close");
		}
	}
	if (check_if_cmd_is_builtin(params->cmd_arr[i].cmd_args[0]))
		execute_builtin(params, i, 1);
	else if (!params->cmd_arr[i].cmd_path)
		wrapper(execve(params->cmd_arr[i].cmd_args[0], params->cmd_arr[i].cmd_args,
			NULL), "execve");
	else
		wrapper(execve(params->cmd_arr[i].cmd_path, params->cmd_arr[i].cmd_args, NULL),
			"execve");
}
*/
