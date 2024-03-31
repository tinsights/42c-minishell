/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_input_string.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 13:36:16 by achak             #+#    #+#             */
/*   Updated: 2024/03/31 19:13:39 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	count_meta_char(char **temp_line, int *token_len)
{
	(*token_len)++;
	if (**temp_line == '|')
		(*token_len)++;
	else if (*temp_line + 1)
	{
		if (*(*temp_line + 1) != '<' && *(*temp_line + 1) != '>')
			(*token_len)++;
		else if ((**temp_line == '>' && *(*temp_line + 1) == '>')
			|| (**temp_line == '<' && *(*temp_line + 1) == '<'))
		{
			(*token_len) += 2;
			(*temp_line)++;
		}
		else
			(*token_len)++;
	}
	else
		(*token_len)++;
	(*temp_line)++;
}

void	count_quote_len(char **temp_line, char quote, int *token_len,
		t_env *head_env)
{
	int	flag;

	if (quote == 39)
		flag = 0;
	else
		flag = -1;
	(*temp_line)++;
	while (**temp_line != quote && **temp_line)
	{
		if (!flag)
			flag = 1;
		else
			flag = 2;
		if (**temp_line == '$' && flag == 2)
		{
			count_var_len(temp_line, token_len, head_env);
			continue ;
		}
		(*temp_line)++;
		(*token_len)++;
	}
	if (**temp_line == quote)
		(*temp_line)++;
}

//	if (flag <= 0)
//		(*token_len)++;

int	count_token_len(char *temp, t_env *head_env)
{
	int		token_len;

	token_len = 0;
	while (is_whitespace(*temp))
		temp++;
	if (*temp == '|' || *temp == '<' || *temp == '>')
		count_meta_char(&temp, &token_len);
	else
	{
		while (*temp != '|' && *temp != '<'
			&& *temp != '>' && *temp && !is_whitespace(*temp))
		{
			if (*temp == 39 || *temp == '"')
				count_quote_len(&temp,
					*temp, &token_len, head_env);
			else if (*temp == '$')
				count_var_len(&temp, &token_len,
					head_env);
			else if (!is_whitespace(*temp))
				count_regular_len(&temp, &token_len);
		}
	}
	return (token_len);
}

void	alloc_token_by_index(char **temp_line, char **token_arr, int i,
		t_env *head_env)
{
	int		token_len;
	char	*temp;

	temp = *temp_line;
	token_len = count_token_len(temp, head_env);
	if (!token_arr)
		return ;
	token_arr[i] = (char *)malloc(sizeof(char) * (token_len + 1));
	if (!token_arr[i])
	{
		free_array(token_arr);
		return ;
	}
	token_arr[i][token_len] = '\0';
	if (token_len)
		copy_token_from_line(temp_line, token_arr[i], head_env);
	else
	{
		while (is_whitespace(**temp_line) && **temp_line)
			(*temp_line)++;
		while (!is_whitespace(**temp_line) && **temp_line)
			(*temp_line)++;
	}
}

char	**parse_input_string(char *line_read, t_env *head_env)
{
	int		token_nbr;
	int		i;
	char	*temp;
	char	**token_arr;

	token_nbr = count_nbr_of_tokens(line_read, head_env);
	i = -1;
	temp = line_read;
	token_arr = (char **)malloc(sizeof(char *) * (token_nbr + 1));
	if (token_arr)
	{
		token_arr[token_nbr] = NULL;
		while (++i < token_nbr && temp)
		{
			alloc_token_by_index(&temp, token_arr, i, head_env);
			if (!token_arr)
				break ;
		}
	}
	return (token_arr);
}
