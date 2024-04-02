/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy_token_from_line.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 12:04:37 by achak             #+#    #+#             */
/*   Updated: 2024/03/31 16:48:55 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

// Need to account for var len for '$?' ?
void	copy_var_len(char **temp, char *token_arr, int *i, t_env *head_env)
{
	int	j;
	int	k;

	k = 0;
	(*temp)++;
	j = move_ptr_past_var(temp, &head_env);
	if (j == -1)
		return ;
	if ((*temp)[0] && j != 0)
	{
		if (!head_env || (!(((*temp)[0] >= 'a' && (*temp)[0] <= 'z')
			|| ((*temp)[0] >= 'A' && (*temp)[0] <= 'Z'))
			&& ((*temp)[0] != '_') && ((*temp)[0] != '?')))
		{
			(*temp) += j;
			return ;
		}
		while (k < my_strlen(head_env->value))
			token_arr[++(*i)] = head_env->value[k++];
	}
	else
		token_arr[++(*i)] = '$';
	(*temp) += j;
}

void	copy_regular_len(char **temp, char *token_arr, int *i)
{
	while (!is_whitespace(**temp) && **temp != '|' && **temp != '<'
		&& **temp != '>' && **temp != 39 && **temp != '"' && **temp
		&& **temp != '$')
	{
		token_arr[++(*i)] = **temp;
		(*temp)++;
	}
}

void	copy_token_from_line(char **temp, char *token_arr, t_env *head_env)
{
	int	i;

	i = -1;
	while (is_whitespace(**temp))
		(*temp)++;
	if (**temp == '|' || **temp == '<' || **temp == '>')
		copy_meta_char(temp, token_arr, &i);
	else
	{
		while (**temp != '|' && **temp != '<' && **temp != '>'
			&& **temp && !is_whitespace(**temp))
		{
			if (**temp == 39 || **temp == '"')
				copy_quote_len(temp, &i,
					token_arr, head_env);
			else if (**temp == '$')
				copy_var_len(temp, token_arr,
					&i, head_env);
			else if (!is_whitespace(**temp))
				copy_regular_len(temp, token_arr, &i);
		}
	}
}
