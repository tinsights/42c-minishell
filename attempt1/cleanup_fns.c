/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup_fns.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 11:29:29 by achak             #+#    #+#             */
/*   Updated: 2024/03/31 19:58:46 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	free_symbol_table(t_env **head)
{
	t_env	*temp;

	while (*head)
	{
		temp = (*head)->next;
		free((*head)->key);
		free((*head)->value);
		free(*head);
		*head = temp;
	}
	*head = NULL;
}

void	free_array(char **arr)
{
	int	i;

	i = -1;
	if (arr && *arr)
		while (arr[++i])
			free(arr[i]);
	free(arr);
	arr = NULL;
}

void	free_cmd_arr(t_command *cmd_arr, int cmd_nbr)
{
	int	i;

	i = -1;
	while (++i < cmd_nbr)
	{
		if (cmd_arr[i].cmd_args)
			free(cmd_arr[i].cmd_args);
		if (cmd_arr[i].cmd_path)
			free(cmd_arr[i].cmd_path);
		if (cmd_arr[i].stdin_fds)
			free(cmd_arr[i].stdin_fds);
		if (cmd_arr[i].stdout_fds)
			free(cmd_arr[i].stdout_fds);
	}
	free(cmd_arr);
	cmd_arr = NULL;
}

void	handle_exit_failure(char **token_arr, t_params *params)
{
	if (!token_arr && params->token_arr)
		token_arr = params->token_arr;
	if (token_arr)
		free_array(token_arr);
	if (params)
	{
		if (params->cmd_arr)
			free_cmd_arr(params->cmd_arr, params->cmd_nbr);
	}
}
