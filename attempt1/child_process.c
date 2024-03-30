/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   child_process.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 13:21:46 by achak             #+#    #+#             */
/*   Updated: 2024/03/30 15:33:19 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	settle_child_process_stdin2(t_params *params, int i)
{
	if (*(params->cmd_arr[i].stdin_fds) != -2)
	{
		wrapper(dup2(*(params->cmd_arr[i].stdin_fds),
				STDIN_FILENO), "dup2");
		wrapper(close(*(params->cmd_arr[i].stdin_fds)), "close");
		if (params->cmd_arr[i].here_doc)
			wrapper(unlink(params->cmd_arr[i].here_doc), "unlink");
	}
}

void	settle_child_process_stdin(t_params *params, int i, int (*old_fds)[2])
{
	if (i == 0)
		settle_child_process_stdin2(params, i);
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
}

void	settle_child_process_stdout(t_params *params, int i, int *new_fds)
{
	if (i == params->cmd_nbr - 1)
	{
		if (*(params->cmd_arr[i].stdout_fds) != -2)
		{
			wrapper(dup2(*(params->cmd_arr[i].stdout_fds),
					STDOUT_FILENO), "dup2");
			wrapper(close(*(params->cmd_arr[i].stdout_fds)), "close");
		}
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
}

void	child_process(t_params *params, int i, int *new_fds, int (*old_fds)[2])
{
	settle_child_process_stdin(params, i, old_fds);
	settle_child_process_stdout(params, i, new_fds);
	if (check_if_cmd_is_builtin(params->cmd_arr[i].cmd_args[0]))
		execute_builtin(params, i, 1);
	else if (!params->cmd_arr[i].cmd_path)
		wrapper(execve(params->cmd_arr[i].cmd_args[0],
				params->cmd_arr[i].cmd_args, NULL), "execve");
	else
		wrapper(execve(params->cmd_arr[i].cmd_path,
				params->cmd_arr[i].cmd_args, NULL), "execve");
}
