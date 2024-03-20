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

void detect_redirections(char *line_read)
{
	int i;
	char c;

	i = 0;
	c = line_read[i];
	while (c)
	{
		if (line_read[i] == '<')
		{
			// TODO:
			// if end of string, or special chars
			// or heredoc **IMPORTANT**

			printf("found input redirection.\n");
			// skip the redirection char,
			i++;

			// ensure we didnt reach end of line
			// print out the filename that we should be opening

			while (line_read[i] && line_read[i] == ' ')
				i++;

			// eventually we want to have this in its own string
			// so we can pass it into open()



			printf("opening file to read: ");
			while (line_read[i] && line_read[i] != ' ')
			{
				printf("%c", line_read[i]);
				i++;
			}
			printf("\n");
		}
		else if (c =='>')
		{

			// TODO:
			// if end of string, or special chars
			// or truncate **IMPORTANT**


			printf("found output redirection.\n");
			// skip the redirection char,
			i++;

			// ensure we didnt reach end of line
			// skip spaces
			while (line_read[i] && line_read[i] == ' ')
				i++;

			// print out the filename that we should be opening
			// eventually we want to have this in its own string
			// so we can pass it into open()


			printf("opening file to write: ");

			while (line_read[i] && line_read[i] != ' ')
			{
				printf("%c", line_read[i]);
				i++;
			}
			printf("\n");

		}
		i++;
		c = line_read[i];
	}
}
void	parse_line(char *line_read)
{
	int i;

	// detect pipes
	//		opening pipe
	//		forking
	//		execve cmds, args
	//		left of pipe
	//		right of pipe
	

	 // detect_redirections(line_read);

	int pid;

	pid = fork();

	if (pid == 0)
	{
		chdir("../");
		exit (1);
	}

}

int main(void)
{
	static char	*line_read;
	int		i;
	char	*cwd;


	// cwd = malloc(400);
	i = 0;
	while (1)
	{
		if (line_read)
		{
			free(line_read);
			line_read = NULL;
		}
		cwd = getcwd(NULL, 200);
		line_read = readline(cwd);
		parse_line(line_read);
		if (line_read && *line_read)
			add_history(line_read);
	}
	if (line_read)
	{
		free(line_read);
		line_read = NULL;
	}

	free(cwd);
}
