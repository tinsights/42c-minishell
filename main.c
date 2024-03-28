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
#include <stdio.h> // open flags
#include <unistd.h> // write, getcwd
#include <stdlib.h> // malloc, free

#include <sys/types.h>
#include <sys/wait.h>

#include <fcntl.h> // open

void	*ft_realloc(void *ptr, size_t old_size, size_t size);
char	**find_paths(char **envp);
void	free_str(char **p);
char	*check_valid_cmd(char **paths, char *cmd);
void	run_child_command(char **paths, char **cmd, char **envp);
void	parse_quotes(char *line, char ***words);

typedef struct s_redirect
{
	char	*file;
	int		fd;
} t_redirect;

typedef struct s_cmd
{
	char	***words;
	char	*input_redirect;
	char	*output_redirect;
}	t_cmd;

void	check_redirects(t_cmd *cmd)
{
	int i = 0;

	char ***words = cmd->words;

	if (!*words)
		return ;
	while ((*words)[i])
	{
		if (!ft_strncmp((*words)[i], "<", 2))
		{
			printf("input redirection detected\n");
			free_str((*words) + i);
			if ((*words)[i + 1])
			{
				cmd->input_redirect = (*words)[i + 1];
				(*words)[i + 1] = NULL;
				int j = i;
				while ((*words)[j + 2])
				{
					(*words)[j] = (*words)[j + 2]; // copying the pointer
					(*words)[j + 2] = NULL; // not valid
					j++;
				}
			}
			else
				cmd->input_redirect = NULL;

			if (cmd->input_redirect)
			{
				printf("input redirect: %s\n", cmd->input_redirect);
				int fd = open(cmd->input_redirect, O_RDONLY);
				dup2(fd, STDIN_FILENO);
				close(fd);
			}

		}
		else if (!ft_strncmp((*words)[i], ">", 2))
		{
			printf("output redirection detected\n");

			// move all pointers down
			// keep track of number of pointers moved down
			// set end N poiners to null?
			// TODO: free all redirects !! IMPT
			free_str((*words) + i);
			if ((*words)[i + 1])
			{
				cmd->output_redirect = (*words)[i + 1];
				(*words)[i + 1] = NULL;
				int j = i;
				while ((*words)[j + 2])
				{
					(*words)[j] = (*words)[j + 2]; // copying the pointer
					(*words)[j + 2] = NULL; // not valid
					j++;
				}
			}
			else
				cmd->output_redirect = NULL;

			if (cmd->output_redirect)
			{
				printf("output redirect: %s\n", cmd->output_redirect);

				int fd = open(cmd->output_redirect, O_WRONLY | O_TRUNC | O_CREAT, 0644);
				dup2(fd, STDOUT_FILENO);
				close(fd);
			}

		}
		else if (!ft_strncmp((*words)[i], ">>", 3))
		{
			printf("append output\n");
		}
		else if (!ft_strncmp((*words)[i], "<<", 3))
		{
			printf("heredoc\n");
		}
		else
			i++;
	}
}

int main(int ac, char **av, char **envp)
{
	char	*line;
	char	**paths;
	char	**words;
	
	int i = 0;

	paths = find_paths(envp);

	while (paths[i])
		printf("%s\n", paths[i++]);

	line = NULL;
	words = NULL;
	int runs = -1;
	while (runs--)
	{
		free_str(&line);
		line = readline("$> ");
		if (line && *line)
		{
			add_history(line);
			words = (char **) ft_calloc(1, sizeof(char *));
			parse_quotes(line, &words);
		}


		int j = 0;

		if (words && *words)
		{
			t_cmd cmd;

			cmd.words = &words;
			cmd.input_redirect = NULL;
			cmd.output_redirect = NULL;

			// int ii = -1;
			// while (words[++ii])
			// 	printf("words %i: %s\n", ii, words[ii]);



			// 1) shell expansion
			// 2) redirections
			// 3) exec

			int pid = fork();
			if (pid == 0)
			{


				check_redirects(&cmd);
				
				run_child_command(paths, words, envp);
				while (words[j])
				{
					free_str(&words[j]);
					j++;
				}
				free(words);
				break;
			}
			else
				waitpid(pid, NULL, 0);
		}

		j = 0;
		while (words && words[j])
		{
			free_str(&words[j]);
			j++;
		}
		free(words);
	}
	free_str(&line);

	i = 0;
	while (paths[i])
	{
		free_str(&paths[i]);
		i++;
	}
	free(paths);
}

void	add_to_words(char ***words, char *str)
{
	int		num_strs;
	size_t	size;

	num_strs = 0;
	while (*words && (*words)[num_strs] != NULL)
		num_strs++;
	size = (num_strs + 1) * sizeof(char *);
	*words = (char **) ft_realloc(*words, size, size + sizeof(char *));
	(*words)[num_strs] = str;
	(*words)[num_strs + 1] = NULL;
}


void	split_line_by_space(char ***words, char *str)
{
	int		x;
	char	**line;

	x = 0;
	line = ft_split(str, ' ');
	while (line[x])
		add_to_words(words, line[x++]);
	free(line);
}

int	process_subquote(char *arg, char ***words, char *qstart, char qchar)
{
	int		qlen;
	char	*cmd;
	char	*qarg;

	if (qstart != arg)
	{
		cmd = ft_substr(arg, 0, qstart - arg);
		split_line_by_space(words, cmd);
		free(cmd);
	}
	qlen = 0;
	while (qstart[qlen + 1] && qstart[qlen + 1] != qchar)
		qlen++;
	if (qlen)
	{
		qarg = ft_substr(qstart, 1, qlen + 1);
		add_to_words(words, qarg);
		qarg[qlen] = 0;
	}
	return (qlen);
}


void parse_quotes(char *line, char ***words)
{
	char	*qstart;
	char	qchar;
	int		j;
	int		qlen;

	j = -1;
	qchar = 0;
	qstart = NULL;
	while (line && line[++j])
	{
		if (line[j] == '\'' || line[j] == '"')
		{
			qstart = line + j;
			qchar = line[j];
			break ;
		}
	}
	if (qstart)
	{
		qlen = process_subquote(line, words, qstart, qchar);
		if (j + qlen < ft_strlen(line))
		{
			if (line[j + qlen + 1] && (line[j + qlen + 1] == qchar))
				j++;
			j++;
			parse_quotes(line + j + qlen, words);
		}
	}
	else
		split_line_by_space(words, line);
}


void free_str(char **str)
{
	if (*str)
	{
		free(*str);
		*str = NULL;
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

	// calc num of paths in order to realloc first one to cwd
	int i = 0;
	while (paths[i])
		i++;
	paths = ft_realloc(paths, i * sizeof(char *), (i + 2) * sizeof(char *));
	paths[i] = getcwd(NULL, 500);
	paths[i + 1] = NULL;
	return paths;
}

void	*ft_realloc(void *ptr, size_t old_size, size_t size)
{
	void	*res;
	size_t	maxcpy;

	if (!size)
	{
		if (ptr)
			free (ptr);
		return (NULL);
	}
	if (!ptr)
		return (ft_calloc(1, size));
	res = ft_calloc(1, size);
	if (size >= old_size)
		maxcpy = old_size;
	else
		maxcpy = size;
	ft_memmove(res, ptr, maxcpy);
	free(ptr);
	return (res);
}

char	*check_valid_cmd(char **paths, char *cmd)
{
	int		j;
	char	*cmdpath;
	char	*binpath;

	cmdpath = ft_strjoin("/", cmd);
	j = 0;
	while (paths && paths[j])
	{
		binpath = ft_strjoin(paths[j], cmdpath);
		if (!access(binpath, X_OK))
			break ;
		free(binpath);
		j++;
	}
	free(cmdpath);
	if (paths[j] == NULL)
		return (NULL);
	return (binpath);
}

void	run_child_command(char **paths, char **cmd, char **envp)
{
	char	*binpath;

	if (cmd && cmd[0] && paths)
		binpath = check_valid_cmd(paths, cmd[0]);
	else
		binpath = NULL;
	if (!binpath)
	{
		ft_putstr_fd(cmd[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		// close(p_fd[1]);
		// dup2(p_fd[0], STDIN_FILENO);
		// close(p_fd[0]);
	}
	else
	{
		// dup2(p_fd[1], STDOUT_FILENO);
		// close(p_fd[1]);
		execve(binpath, cmd, envp);
		perror("");
		if (binpath)
			free(binpath);
	}
}