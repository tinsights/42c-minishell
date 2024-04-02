/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parent_process.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/31 19:59:08 by achak             #+#    #+#             */
/*   Updated: 2024/03/31 19:59:17 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	get_status_of_child_process(t_params *params)
{
	int		wstatus;
	char	*exit_str;

	wait(&wstatus);
	if (WIFEXITED(wstatus))
	{
		exit_str = strjoin_and_free_str("?=",
				ft_itoa(WEXITSTATUS(wstatus)), 2);
		if (!check_if_entry_exists("?", *(params->head_env)))
			create_new_entry(exit_str, params->head_env);
		else
			update_existing_entry(exit_str, params->head_env);
		free(exit_str);
	}
}

void	parent_process(t_params *params, int i, int *new_fds, int *old_fds)
{
	if (i != 0)
	{
		close(old_fds[0]);
		close(old_fds[1]);
	}
	if (i != params->cmd_nbr - 1)
	{
		old_fds[0] = new_fds[0];
		old_fds[1] = new_fds[1];
	}
	get_status_of_child_process(params);
	if (params->cmd_arr[i].here_doc)
	{
		free(params->cmd_arr[i].here_doc);
		params->cmd_arr[i].here_doc = NULL;
	}
}
