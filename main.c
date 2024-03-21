/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 10:15:00 by tjegades          #+#    #+#             */
/*   Updated: 2024/03/20 10:15:26 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdbool.h>
#include <stdio.h>

bool	is_blank(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

bool 	is_pipe(char c)
{
	return (c == '|');
}

bool	is_redirect(char c)
{
	return (c == '<' || c == '>');
}

bool	is_meta(char c)
{
	return (is_pipe(c) || is_redirect(c) || is_blank(c));
}

void	parser(char *line)
{
	int		i;
	char	c;

	if (!line)
		return ;
	// run through the line
	// look for operators?
	// in order of precendence?
	
}


int main(void)
{
	char *line = NULL;

	while (true)
	{
		if (line)
			free(line);
		 line = get_next_line(0);
		 parser(line);
	}
}
