/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input_string2.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/25 18:38:07 by achak             #+#    #+#             */
/*   Updated: 2024/03/25 18:46:36 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	memset_last_index(void *s, int c, size_t n)
{
	unsigned char	*ptr;
	int		i;
	
	i = -1;
	ptr = (unsigned char *)s;
	while (++i < n)
		ptr[i] = c;
}

t_token	*parse_input_string2(char *line_read, t_env *head_env)
{
	int		token_nbr;
	int		i;
	t_token	*token_arr;

	token_nbr = count_nbr_of_tokens(line_read, head_env);
	i = -1;
	token_arr = (t_token *)malloc(sizeof(t_token) * (token_nbr + 1));
	if (token_arr)
	{
		memset_last_index(&token_arr[token_nbr], 0, sizeof(t_token));
		while (++i < token_nbr)
			alloc_token_array(i, line_read, head_env);
	}
	return (token_arr);
}
