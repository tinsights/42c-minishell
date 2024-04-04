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
int		count_cmds(char *line);
int		count_strings(char *line);



typedef struct s_cmd
{
	char	**words;
	char	*input_redirect;
	char	*output_redirect;
}	t_cmd;

typedef struct s_env_var
{
	char				*key;
	char				*value;
} t_env_var;


typedef struct s_params
{
	char		*line;
	char		**paths;
	char		**words;
	t_list		*var_list;
	t_list		*mem_lst;
}	t_params;

void *ms_calloc(size_t nmemb, size_t size, t_list **mem_lst)
{
	void *result = ft_calloc(nmemb, size);
	// error management?
	t_list *node = ft_calloc(1, sizeof(t_list));
	node->content = result;
	ft_lstadd_back(mem_lst, node);
	return (result);
}

void safe_free(void **ptr)
{
	if (*ptr)
		free (*ptr);
	*ptr = NULL;
}

void free_env(void *ptr)
{
	t_env_var *var = (t_env_var *) ptr;

	safe_free((void **) &(var->key));
	safe_free((void **) &(var->value));
	safe_free((void **) &(var));
}

int main(int ac, char **av, char **envp)
{
	t_params	params;
	int 		i;
	ft_memset(&params, 0, sizeof(t_params));


	// process envp
	params.paths = find_paths(envp);

	i = 0;
	while (envp && envp[i])
	{
		char *eqsign = ft_strchr(envp[i], '=');
		unsigned int start = eqsign - envp[i];
		if (eqsign)
		{
			t_env_var	*node = ft_calloc(1, sizeof(t_env_var));
			t_list		*list = ft_calloc(1, sizeof(t_list));

			node->key = ft_substr(envp[i], 0, start);
			node->value = ft_substr(envp[i], start + 1, ft_strlen(envp[i]));

			list->content = node;
			ft_lstadd_back(&params.var_list, list);

		}
		i++;
	}

	while (true)
	{
		free_str(&(params.line));
		params.line = readline("$> ");
		if (params.line && *params.line)
		{
			add_history(params.line);

			// syntax_check(params.line);
			// parse_quotes(params.line, &params.words);

			// i = 0;
			// while (params.words[i])
			// 	printf("%s\n", params.words[i++]);

			/* TODO: valid syntax check
			** > < << and >> must be followed by at least one word
			** | must be preceded by and followed by at least one word
			** and cannot be a metachar (before handling quotes)
			*/
			printf("%i\n", count_cmds(params.line));

		}
	}

	ft_lstclear(&params.var_list, free_env);
	i = 0;
	while (params.paths[i])
		safe_free((void **) (params.paths + i++));
	safe_free((void **) &(params.paths));
	free_str(&(params.line));
}




bool ms_isspace(char c)
{
	return (c == ' ' || c == '\t');
}

int count_strings(char *line)
{
	int count;

	if (!line)
		return (-1);
	count = 0;
	while (*line)
	{
		// skip any leading whitespace
		while (*line && ms_isspace(*line))
			line++;
		if (*line)
			count++;
		while (*line && !ms_isspace(*line))
			line++;
	}
	return (count);
}

int syntax_check(char *line)
{
	int str_count;
	int	i;

	if (!line)
		return (-1);
	i = 0;
	str_count = count_strings(line);
	while (*line)
	{
		// skip any leading whitespace
		while (*line && ms_isspace(*line))
			line++;
		if (!ft_strncmp(line, "|", 2))
		{

		}
		// while (*line && !ms_isspace(*line))
		// 	line++;
	}

}

int	count_cmds(char *line)
{
	int		cmd_count;
	char	*start;
	char	*ptr;

	if (!line)
		return (0);

	cmd_count = 1;
	start = line;
	while (*line)
	{
		//assuming syntax is valid

		if (*line == '\'' && *(line + 1))
			line = ft_strchr(line + 1, '\'');
		else if (*line == '"' && *(line + 1))
			line = ft_strchr(line + 1, '"');
		else if (*line == '|')
		{
			ptr = line;
			if (ptr == start || !ptr[1])
			{
				printf("syntax error0\n");
				return (-1);
			}
			ptr++;
			while (ms_isspace(*ptr))
			{
				ptr++;
				if (!(*ptr))
				{
					printf("syntax error1\n");
					return (-1);
				}
			}
			ptr = line - 1;
			while (ms_isspace(*ptr))
			{
				if (ptr == start)
				{
					printf("syntax error2\n");
					return (-1);
				}
				ptr--;
			}
			cmd_count++;

		}

		// check if heredoc
		// process heredoc

		if (line && *line)
			line++;
		else
			break;
	}
	return cmd_count;
}


void	print_env_lst(t_params *params)
{
	t_list *next = params->var_list;
	while (true)
	{
		if (!next)
			break;
		t_env_var *var = (t_env_var *) next->content;
		printf("%s=%s\n", var->key, var->value);
		next = next->next;
	}
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
		free (*str);
	*str = NULL;
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