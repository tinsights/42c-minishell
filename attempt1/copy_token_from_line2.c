/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_token_from_line2.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 19:48:11 by achak             #+#    #+#             */
/*   Updated: 2024/03/30 19:48:18 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	copy_meta_char(char **temp, char *token_arr, int *i)
{
	token_arr[++(*i)] = 39;
	if (**temp == '|')
		token_arr[++(*i)] = **temp;
	else if (*temp + 1)
	{
		if (*(*temp + 1) != '<' && *(*temp + 1) != '>')
			token_arr[++(*i)] = **temp;
		else if ((**temp == '>' && *(*temp + 1) == '>')
			|| (**temp == '<' && *(*temp + 1) == '<'))
		{
			token_arr[++(*i)] = **temp;
			(*temp)++;
			token_arr[++(*i)] = **temp;
		}
		else
			token_arr[++(*i)] = **temp;
	}
	else
		token_arr[++(*i)] = **temp;
	(*temp)++;
}

void	copy_quote_len(char **temp, int *i, char *token_arr, t_env *head_env)
{
	char	quote;

	quote = **temp;
	(*temp)++;
	while (**temp != quote && **temp)
	{
		if (**temp == '$')
		{
			copy_var_len(temp, token_arr, i, head_env);
			continue ;
		}
		token_arr[++(*i)] = **temp;
		(*temp)++;
	}
	if (**temp == quote)
		(*temp)++;
}

int	move_ptr_past_var(char **temp, t_env *head_env)
{
	int	j;

	j = 0;
	while (!is_whitespace((*temp)[j]) && (*temp)[j] != '|' && (*temp)[j]
		&& (*temp)[j] != '<' && (*temp)[j] != '>' && (*temp)[j] != 39
		&& (*temp)[j] != '"' && (*temp)[j] != '$')
		j++;
	while (head_env)
	{
		if (!my_strncmp(*temp, head_env->key, j))
			break ;
		head_env = head_env->next;
	}
	if (!head_env)
	{
		(*temp) += j;
		return (-1);
	}
	return (j);
}
