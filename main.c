/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 10:15:00 by tjegades          #+#    #+#             */
/*   Updated: 2024/03/20 11:10:42 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

int	is_whitespace(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return (1);
	return (0);
}

void	read_word(char *line_read, int *i)
{
	int	j;
	int	k;
	char	c;

	j = 0;
	k = *i;
	c = line_read[*i];
	// skip whitespace
	while (c && is_whitespace(c))
	{
		(*i)++;
		c = line_read[*i];
	}
	while (c && !is_whitespace(c))
	{
		c = line_read[*i];
		(*i)++;
		j++;
	}
//	line_read[i] = 0;
//	write(STDIN_FILENO, line_read, j);
	while (j > 0 && line_read[k])
	{
		printf("%c", line_read[k]);
		j--;
		k++;
	}
}

void	parse_line(char *line_read)
{
	int	i;
	int	flag;

	i = 0;
	flag = 0;
	while (line_read[i])
	{
//		if (flag && !is_whitespace(line_read[i]))
//		{
//			read_word(line_read, &i);
//			flag = 0;
//			continue ;
//		}
		if (line_read[i] == '>')
		{
			i++;
			printf("redirecting output to ");
			read_word(line_read, &i);
			printf("\n");
		}
		else if (line_read[i] == '<')
		{
			i++;
			printf("taking input from ");
			read_word(line_read, &i);
			printf("\n");
		}
		else
			i++;
	}
}

int main(void)
{
	static char	*line_read;
	int		i;

	i = 0;
	while (1)
	{
		if (line_read)
		{
			free(line_read);
			line_read = NULL;
		}
		line_read = readline("$> ");
		parse_line(line_read);
		if (line_read && *line_read)
			add_history(line_read);
	}
	if (line_read)
	{
		free(line_read);
		line_read = NULL;
	}
}
