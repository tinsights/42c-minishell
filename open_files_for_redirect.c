/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   open_files_for_redirect.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 14:00:28 by achak             #+#    #+#             */
/*   Updated: 2024/03/30 14:53:07 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	check_for_redirect(char *token_arr)
{
	if (token_arr && my_strlen(token_arr) >= 2)
	{
		if (!(token_arr + 1))
			return (-1);
		if (token_arr[0] == '|')
			return (-2);
		if (token_arr[1] == '>' && my_strlen(token_arr) == 2)
			return (1);
		else if (token_arr[1] == '<' && my_strlen(token_arr) == 2)
			return (2);
		else if (token_arr[1] == '>' && token_arr[2] == '>')
			return (3);
		else if (token_arr[1] == '<' && token_arr[2] == '<')
			return (4);
	}
	else
		return (0);
	return (-1);
}

void	determine_mode_to_open_file(t_params *params, int i, char *token_arr,
		int rv)
{
	if ((rv == 1 || rv == 3) && *(params->cmd_arr[i].stdout_fds) != -2)
		close(*(params->cmd_arr[i].stdout_fds));
	if ((rv == 2 || rv == 4) && *(params->cmd_arr[i].stdin_fds) != -2)
	{
		close(*(params->cmd_arr[i].stdin_fds));
		if (params->cmd_arr[i].here_doc)
			wrapper(unlink(params->cmd_arr[i].here_doc), "unlink");
	}
	if (rv == 1)
		*(params->cmd_arr[i].stdout_fds) = open(token_arr,
				O_WRONLY | O_TRUNC | O_CREAT, 0644);
	else if (rv == 2)
		*(params->cmd_arr[i].stdin_fds) = open(token_arr, O_RDONLY);
	else if (rv == 3)
		*(params->cmd_arr[i].stdout_fds) = open(token_arr,
				O_WRONLY | O_APPEND | O_CREAT, 0644);
	else if (rv == 4)
		ft_heredoc(params, i, token_arr);
}

int	open_files_for_redirect(char **token_arr, int i, t_params *params)
{
	int	rv;

	while (check_token_pipe(*token_arr))
		token_arr++;
	while (*token_arr && !check_token_pipe(*token_arr))
	{
		rv = check_for_redirect(*token_arr);
		if (rv == -2)
			return (1);
		if (rv >= 1 && rv <= 4)
		{
			token_arr++;
			determine_mode_to_open_file(params, i, *token_arr, rv);
		}
		if (*(params->cmd_arr[i].stdin_fds) == -1
			|| *(params->cmd_arr[i].stdout_fds) == -1)
		{
			ft_dprintf(STDERR_FILENO, "%s: %s\n",
				*token_arr, strerror(errno));
			handle_exit_failure(NULL, params);
			return (-1);
		}
		token_arr++;
	}
	return (0);
}
