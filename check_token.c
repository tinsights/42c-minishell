/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_token.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 13:50:08 by achak             #+#    #+#             */
/*   Updated: 2024/03/30 13:52:48 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	check_next_token_pipe(char **token_arr, int i)
{
	if (token_arr[++i])
	{
		if (token_arr[i][0] == 39 && token_arr[i][1] == '|')
			return (0);
		return (1);
	}
	else
		return (0);
}

int	check_next_token_null(char **token_arr, int i)
{
	if (token_arr[++i])
		return (0);
	else
		return (1);
}

int	check_next_token_file(char **token_arr, int i)
{
	if (token_arr[i])
	{
		if (token_arr[i][0] == 39)
			return (0);
		return (1);
	}
	else
		return (0);
}

int	check_token_pipe(char *token_arr)
{
	if (token_arr)
	{
		if (my_strlen(token_arr) == 2)
			if (token_arr[0] == 39 && token_arr[1] == '|')
				return (1);
		return (0);
	}
	else
		return (1);
}

int	check_token_redir(char *token_arr)
{
	if (token_arr && my_strlen(token_arr) >= 2)
	{
		if (token_arr[0] == 39 && (token_arr[1] == '<'
				|| token_arr[1] == '>'))
			return (1);
		return (0);
	}
	return (0);
}
