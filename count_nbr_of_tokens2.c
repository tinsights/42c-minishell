/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_nbr_of_tokens2.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 18:06:32 by achak             #+#    #+#             */
/*   Updated: 2024/03/25 14:31:22 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	iterate_thru_meta_char(char **temp, int *count, int *flag)
{
	if (**temp == '|')
		(*count)++;
	else if (*temp + 1)
	{
		if (*(*temp + 1) != '<' && *(*temp + 1) != '>')
			(*count)++;
		else if ((**temp == '<' && *(*temp + 1) == '<')
			|| (**temp == '>' && *(*temp + 1) == '>'))
		{
			(*count)++;
			(*temp)++;
		}
		else
			(*count)++;
	}
	else
		(*count)++;
	(*temp)++;
	*flag = 0;
}

void	iterate_thru_word(char **temp, int *count, int *flag)
{
	int	flag1;

	flag1 = 0;
	while (**temp)
	{
		if ((**temp == 39 && flag1 == 1) || (**temp == '"'
				&& flag1 == 2))
			flag1 = 0;
		else if (flag1 != 0)
			;
		else if (**temp == 39)
			flag1 = 1;
		else if (**temp == '"')
			flag1 = 2;
		else if ((is_whitespace(**temp) || **temp == '|' || **temp
				== '<' || **temp == '>') && flag1 == 0)
			break ;
		(*temp)++;
	}
	if (!*flag)
	{
		(*count)++;
		*flag = 1;
	}
}

void	reset_flag_for_new_word(char **line_read, int *flag)
{
	(*line_read)++;
	*flag = 0;
}
