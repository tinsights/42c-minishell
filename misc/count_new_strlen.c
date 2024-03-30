/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_new_strlen.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 12:47:08 by achak             #+#    #+#             */
/*   Updated: 2024/03/21 17:19:39 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

/*
int	count_var_len(char *line_read, t_env *head_env, int *i)
{
	int	j;
	int	k;

	j = 0;
	while (j++ <= *i)
		line_read++;
	k = j;
	while (!is_whitespace(line_read[j]) && line_read[j])
		j++;
	while (head_env)
	{
		if (!my_strncmp(line_read, head_env->key, j - k))
			break ;
		head_env = head_env->next;
	}
	if (!head_env)
		return (-1000);
	*i = j;
	return (head_env->diff);
}

int	count_redir_len(char *line_read, int *i)
{
	int	flag;
	int	j;

	flag = 0;
	j = *i;
	if (is_whitespace(line_read[(*i) + 1]))
		flag = 1;
	else if (is_whitespace(line_read[(*i) + 2]))
		flag = 2;
	while (line_read[++(*i)])
	{
		if (flag == -1 && is_whitespace(line_read[(*i)]))
			break ;
		else if (flag && is_whitespace(line_read[(*i)]))
			continue ;
		else if (flag && !is_whitespace(line_read[(*i)]))
			flag--;
		else if (!is_whitespace(line_read[(*i)]))
			flag = -1;
	}
	return (*i - j);
}

int	count_regular_chars(char *line_read, int *i, int *flag)
{
	int	count;

	count = 0;
	while (line_read[++(*i)])
	{
		if ((*flag == 3 && line_read[(*i)] == 39)
			|| (*flag == 4 && line_read[(*i)] == '"')
			|| (*flag != 3 && line_read[(*i)] == '$')
			|| (*flag < 3 && (line_read[(*i)] == '>'
			|| line_read[(*i)] == '<')))
			break ;
		else if (is_whitespace(line_read[*i]))
		{
			if (*flag != -1)
				*flag = 2;
		}
		else
		{
			if (*flag == 2)
				count++;
			*flag = 1;
			count++;
		}
	}
	return (count + 1);
}

int	count_new_strlen(char *line_read, t_env *head_env)
{
	int	count;
	int	flag;
	int	i;

	count = 0;
	flag = -1;
	i = 0;
	while (line_read[i])
	{
		if ((flag == 3 && line_read[i] == 39) ||
			(flag == 4 && line_read[i] == '"'))
		{
			i++;
			flag = 0;
		}
		else if (line_read[i] == 39)
		{
			i++;
			flag = 3;
		}
		else if (line_read[i] == '"')
		{
			i++;
			flag = 4;
		}
		else if (flag != 3 && line_read[i] == '$')
			count += count_var_len(line_read, head_env, &i);
		else if (line_read[i] == '>' || line_read[i] == '<')
			count += count_redir_len(line_read, &i);
		else
			count += count_regular_chars(line_read, &i, &flag);
		printf("i = %d\n", i);
	}
	return (count);
}
*/

int	compare_var_diff(char *line_read, t_env *head_env)
{
	int	i;

	i = 0;
	while (!is_whitespace(line_read[i]) && line_read[i])
		i++;
	line_read++;
	while (head_env)
	{
		if (!my_strncmp(line_read, head_env->key, i - 1))
			break ;
		head_env = head_env->next;
	}
	if (!head_env)
		return (-i);
	return (head_env->diff);
}

int	count_var_diff(char *line_read, t_env *head_env)
{
	int	flag;
	int	count;

	flag = 0;
	count = 0;
	while (*line_read)
	{
		if (flag && *line_read == 39)
			flag = 0;
		else if (flag)
		{
			line_read++;
			continue ;
		}
		else if (*line_read == 39)
			flag = 1;
		else if (*line_read == '$')
			count += compare_var_diff(line_read, head_env);
		line_read++;
	}
	return (count);
}

int	count_without_extra_whitespace(char *line_read)
{
	int	count;
	int	flag;

	count = 0;
	flag = -1;
	while (*line_read)
	{
		if (flag == 3 && (*line_read == 39 || *line_read == '"'))
		{
//			printf("end quote\n");
			flag = 1;
		}
		else if (flag == 3 && is_whitespace(*line_read))
			count++;
		else if (*line_read == 39 || *line_read == '"')
		{
//			printf("start quote\n");
			flag = 3;
		}
		else if (flag == 2 && !is_whitespace(*line_read))
		{
			count += 2;
			flag = 1;
		}
		else if (flag == 1 && is_whitespace(*line_read))
			flag = 2;
		else if (!is_whitespace(*line_read))
		{
			count++;
			if (flag != 3)
				flag = 1;
		}
		line_read++;
	}
	return (count);
}

int	count_new_strlen(char *line_read, t_env *head_env)
{
	int	count;

	count = count_without_extra_whitespace(line_read);
	count += count_var_diff(line_read, head_env);
	if (count < 0)
		count *= -1;
	return (count);
}

int	main(int ac, char **av, char **env)
{
	t_env	*head_env;
	static char	*line_read;
	head_env = create_symbol_table(env);
	if (ac < 2 || !av[1])
		return (1);
	while (1)
	{
		if (line_read)
		{
			free(line_read);
			line_read = NULL;
		}
		line_read = readline("$> ");
		printf("%s\n", line_read);
		printf("%d\n", count_new_strlen(line_read, head_env));
		char	*new_str;
		new_str = transform_original_command(line_read, head_env);
		printf("new_str = %s\n", new_str);
		free(new_str);
	}
	free_symbol_table(&head_env);
}
