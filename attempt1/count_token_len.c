/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_token_len.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 09:40:12 by achak             #+#    #+#             */
/*   Updated: 2024/03/30 09:44:19 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	count_var_len(char **temp, int *token_len, t_env *head_env, int i)
{
	int	j;

	j = 0;
	(*temp)++;
//	if ((*temp)[0])
//		if (!(((*temp)[0] >= 'a' && (*temp)[0] <= 'z')
//			|| ((*temp)[0] >= 'A' && (*temp)[0] <= 'Z'))
//			&& !((*temp)[0] != '_'))
//			return ;

	// TEMP
//	while (!is_whitespace((*temp)[j]) && (*temp)[j] != '|'
//		&& (*temp)[j] != '<' && (*temp)[j] != '>' && (*temp)[j]
//		&& (*temp)[j] != 39 && (*temp)[j] != '"' && (*temp)[j] != '$')
//		j++;
	while (!is_whitespace((*temp)[j]) && (*temp)[j] != '|'
		&& (*temp)[j] != '<' && (*temp)[j] != '>' && (*temp)[j]
		&& (*temp)[j] != 39 && (*temp)[j] != '"' && (*temp)[j] != '$')
		if ((*temp)[j++] == '?')
			break ;

	// int i has no use for now
//	if ((*temp)[j])
//		if (i == 1 && (*temp)[j] == '"')
//			i = 2;
	if (!i)
		i = 1;
	while (head_env)
	{
		if (!my_strncmp(*temp, head_env->key, j))
			break ;
		head_env = head_env->next;
	}
	if ((*temp)[0] && j != 0)
	{
		if (!head_env || (!(((*temp)[0] >= 'a' && (*temp)[0] <= 'z')
				|| ((*temp)[0] >= 'A' && (*temp)[0] <= 'Z'))
				&& ((*temp)[0] != '_') && ((*temp)[0] != '?')))
		{
		//	printf("------>>>in here------->>>\n");
		// PUT IF (*temp)[0] == '?', (*token_len)++;
//			if ((*temp)[0] == '?')
//				(*token_len) += my_strlen(head_env->value);
			(*temp) += j;
			return ;
		}
		(*token_len) += my_strlen(head_env->value);
	}
	else
		(*token_len)++;
	//why this else statement ^^?
	(*temp) += j;
	//printf("*token_len in var fn = %d\n", *token_len);
}

void	count_regular_len(char **temp, int *token_len)
{
	while (!is_whitespace(**temp) && **temp != '|' && **temp != '<'
		&& **temp != '>' && **temp != 39 && **temp != '"' && **temp
		&& **temp != '$')
	{
		//printf("(*temp) = %s\n", *temp);
		(*temp)++;
		(*token_len)++;
	}
	//printf("*token_len in regular fn = %d\n", *token_len);
}
