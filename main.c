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
#include <readline/readline.h>
#include <readline/history.h>
#include <stdbool.h>
// #include <stdio.h>
#include <unistd.h> // write, getcwd
#include <stdlib.h> // malloc, free

void free_str(char *p)
{
	if (p)
	{
		free(p);
		p = NULL;
	}
}

char **find_paths(char **envp)
{
	char	**paths;

	while (*envp && ft_strncmp(*envp, "PATH", 4))
		envp++;
	if (*envp)
		paths = ft_split(*envp + 5, ':');
	else
		paths = (char **)(ft_calloc(1, sizeof(char *)));
	return paths;
}

int main(int ac, char **av, char **envp)
{
	char	*line;
	char	**paths;


	int i = 1;

	// paths = find_paths(envp);

	// while (paths[i])
	// 	printf("%s\n", paths[i++]);

	line = NULL;
	while (i--)
	{
		free_str(line);
		line = readline("$> ");
		if (line && *line)
			add_history(line);
	}
	free_str(line);

	// while (paths)
	// {
	// 	free_str(*paths);
	// 	paths++;
	// }
}
