/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transform_original_command.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 15:14:11 by achak             #+#    #+#             */
/*   Updated: 2024/03/21 17:18:29 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	copy_var_to_newstr(char *line_read, char *new_str, t_env *head_env,
		int *i)
{
	int	j;

	j = 0;
	while ((line_read)[++j])
		if (is_whitespace((line_read)[j]))
			break ;
	printf("j = %d\n", j);
	printf("*line_read = %c\n", *line_read);
	(line_read)++;
	while (head_env)
	{
		if (!my_strncmp((line_read), head_env->key, j - 1))
			break ;
		head_env = head_env->next;
	}
	if (!head_env)
	{
		printf("--------------------------->\n");
		return ;
	}
	printf("head_env->value = %s\n", head_env->value);
	while (*(head_env->value))
		new_str[++(*i)] = *(head_env->value)++;
}

void	copy_old_to_newstr(char *line_read, char *new_str, t_env *head_env)
{
	int	i;
	int	flag;

	i = -1;
	flag = -1;
	while (*line_read)
	{
		if ((flag == 3 && *line_read == 39)
			|| (flag == 4 && *line_read == '"'))
			flag = 1;
		else if (flag == 3 && *line_read != 39)
			new_str[++i] = *line_read;
		else if (flag == 5)
		{
			if (is_whitespace(*line_read))
				flag = 0;
			else
			{
				line_read++;
				continue ;
			}
		}
		else if (flag != 3 && *line_read == '$')
		{
			flag = 5;
			copy_var_to_newstr(line_read, new_str, head_env, &i);
		}
		else if (flag == 4 && *line_read != '"')
			new_str[++i] = *line_read;
		else if (*line_read == 39)
			flag = 3;
		else if (*line_read == '"')
			flag = 4;
		else if (flag == 2 && !is_whitespace(*line_read))
		{
			new_str[++i] = ' ';
			new_str[++i] = *line_read;
			flag = 1;
		}
		else if (flag == 1 && is_whitespace(*line_read))
			flag = 2;
		else if (!is_whitespace(*line_read))
		{
			new_str[++i] = *line_read;
			if (flag != 3)
				flag = 1;
		}
		line_read++;
	}
}

char	*transform_original_command(char *line_read, t_env *head_env)
{
	int		new_strlen;
	char	*new_str;

	new_strlen = count_new_strlen(line_read, head_env);
	printf("count_new_strlen = %d\n", count_new_strlen(line_read, head_env));
	new_str = (char *)malloc(sizeof(char) * (new_strlen + 1));
	if (new_str)
	{
		new_str[new_strlen] = '\0';
		copy_old_to_newstr(line_read, new_str, head_env);
	}
	return (new_str);
}
