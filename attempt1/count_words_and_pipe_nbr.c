/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_words_and_pipe_nbr.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 14:39:48 by achak             #+#    #+#             */
/*   Updated: 2024/03/30 14:40:42 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	check_syntax_around_pipe(t_params *params, int i, int *nbr)
{
	if (params->token_arr[i][0] == 39 && params->token_arr[i][1] == '|')
	{
		if (check_next_token_pipe(params->token_arr, i))
			(*nbr)++;
		else
		{
			write(STDERR_FILENO, "syntax error near '|'\n", 22);
			handle_exit_failure(NULL, params);
			return (-1);
		}
	}
	return (0);
}

int	check_syntax_around_redir(t_params *params, int i)
{
	if (params->token_arr[i][0] == 39 && (params->token_arr[i][1] == '<'
		|| params->token_arr[i][1] == '>'))
	{
		if (check_next_token_null(params->token_arr, i))
		{
			ft_dprintf(STDERR_FILENO, "syntax error near %s\n",
				params->token_arr[i] + 1);
			handle_exit_failure(NULL, params);
			return (-1);
		}
	}
	return (0);
}

int	nbr_of_pipes(t_params *params)
{
	int	nbr;
	int	i;
	int	flag;

	nbr = 0;
	i = -1;
	flag = 0;
	while (params->token_arr[++i])
	{
		if (params->token_arr[i] && my_strlen(params->token_arr[i]) >= 2)
		{
			if (check_syntax_around_pipe(params, i, &nbr) == -1)
				return (-1);
			else if (check_syntax_around_redir(params, i) == -1)
				return (-1);
		}
	}
	return (nbr);
}
//	if (!params->head_env)
//		return (nbr);

int	check_operator_token(char **token_arr, int i, t_params *params, int *count)
{
	if (token_arr[i][0] == 39 && (token_arr[i][1] == '<'
		|| token_arr[i][1] == '>'))
	{
		if (!check_next_token_file(token_arr, ++i))
		{
			write(STDERR_FILENO, "syntax error near redir token\n", 30);
			handle_exit_failure(NULL, params);
			return (-1);
		}
	}
	else if (token_arr[i][0] == 39
		&& token_arr[i][1] == '|')
		return (1);
	else
		(*count)++;
	return (0);
}

int	count_words_nbr_in_cmd(char **token_arr, t_params *params)
{
	int	count;
	int	rv;
	int	i;

	count = 0;
	rv = 0;
	i = 0;
	while (check_token_pipe(token_arr[i]))
		i++;
	i--;
	while (token_arr[++i])
	{
		if (my_strlen(token_arr[i]) >= 2)
		{
			rv = check_operator_token(token_arr, i, params, &count);
			if (rv == -1)
				return (-1);
			else if (rv == 1)
				break ;
		}
		else
			count++;
	}
	return (count);
}
