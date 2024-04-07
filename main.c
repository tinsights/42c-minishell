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


typedef struct s_cmd
{
	int		num_words;
	int		num_redirects;
	char	*line;
	char	**words;
	char	*input_redirect;
	char	*output_redirect;
}	t_cmd;

typedef struct s_env
{
	char				*key;
	char				*value;
} t_env;


typedef struct s_params
{
	int			num_cmds;
	char		*line;
	char		**paths;

	t_list		*cmd_list;
	t_list		*env_list;
	t_list		*mem_lst;
}	t_params;

void	*ft_realloc(void *ptr, size_t old_size, size_t size);
char	**find_paths(char **envp);
void	free_str(char **p);
void 	safe_free(void **ptr);
char	*check_valid_cmd(char **paths, char *cmd);
void	run_child_command(char **paths, char **cmd, char **envp);
void	parse_quotes(char *line, char ***words);
int		count_cmds(t_params *params);
int		count_strings(char *line);
void	create_cmds(t_params *params);
void	create_words(t_params *params);
bool 	is_meta(char *line);
bool 	is_space(char c);
int		is_redirect(char *line);



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
	t_env *var = (t_env *) ptr;

	safe_free((void **) &(var->key));
	safe_free((void **) &(var->value));
	safe_free((void **) &(var));
}

void free_cmds(void *ptr)
{
	t_cmd *cmd = (t_cmd *) ptr;
	// char **words = cmd->words;

	// int i = 0;
	// while (words[i])
	// {
	// 	free_str(words + i);
	// 	i++;
	// }
	// safe_free((void **) &words);
	safe_free((void **) &(cmd->line));
	safe_free((void **) &(cmd));

}

int main(int ac, char **av, char **envp)
{
	t_params	params;
	int 		i;
	ft_memset(&params, 0, sizeof(t_params));


	/* -------------------------------------------------------------------------- */
	/*                         Process ENVP, create PATHS                         */
	/* -------------------------------------------------------------------------- */
	params.paths = find_paths(envp);
	i = 0;
	while (envp && envp[i])
	{
		char *eqsign = ft_strchr(envp[i], '=');
		unsigned int start = eqsign - envp[i];
		if (eqsign)
		{
			t_env	*node = ft_calloc(1, sizeof(t_env));
			t_list		*list = ft_calloc(1, sizeof(t_list));

			node->key = ft_substr(envp[i], 0, start);
			node->value = ft_substr(envp[i], start + 1, ft_strlen(envp[i]));

			list->content = node;
			ft_lstadd_back(&params.env_list, list);

		}
		i++;
	}


	/* -------------------------------------------------------------------------- */
	/*                            Read line with prompt                           */
	/* -------------------------------------------------------------------------- */
	static unsigned int totallen;
	while (i--)
	{
		free_str(&(params.line));
		params.line = readline("$> ");
		if (params.line && *params.line)
		{
			add_history(params.line);
			// parse_quotes(params.line, &params.words);

			// i = 0;
			// while (params.words[i])
			// 	printf("%s\n", params.words[i++]);

			/** DONE: valid syntax check
			 *  > < << and >> must be followed by at least one word
			 *  | must be preceded by and followed by at least one word or valid redir
			*/
			params.num_cmds = count_cmds(&params);
			if (params.num_cmds <= 0)
				continue ;
			/** TODO: Prepare linked list of simple commands
			 * with output and input redirects
			 * 
			*/
			create_cmds(&params);
			create_words(&params);
			ft_lstclear(&params.cmd_list, free_cmds);

		}
	}

	ft_lstclear(&params.env_list, free_env);
	i = 0;
	while (params.paths[i])
		safe_free((void **) (params.paths + i++));
	safe_free((void **) &(params.paths));
	free_str(&(params.line));
}

bool valid_env_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}


char *valid_env_var(char *line)
{
	char *result;

	int i = 1;
	/**
	 * TODO: special treatment for $?
	*/
	if (line[i] == '_' || ft_isalpha(line[i]))
	{
		i++;
		while (valid_env_char(line[i]))
			i++;
		result = ft_substr(line, 1, i - 1);
		return result;
	}

	return (NULL);
}

char *parse_env_var(t_list *env_lst, char *var)
{

	while (env_lst)
	{
		t_env *env_node = env_lst->content;

		if (!ft_strncmp(var, env_node->key, ft_strlen(var)))
		{
			// printf("%s\n", env_node->value);
			return (env_node->value);
		}

		env_lst = env_lst->next;
	}
	return (NULL);
}

int		len_to_alloc(char *line, int *ptr)
{
	int i = 0;

	if (!line[i] || is_redirect(line) || is_space(line[i]))
		return (0);
	else if (line[i] == '\'')
	{
		i++;
		while (line[i] != '\'')
			i++;
		i++;
		*ptr += 2;	
		return (i + len_to_alloc(line + i, ptr) - 2);
	}
	else if (line[i] == '"')
	{
		i++;
		while (line[i] != '"')
		{
			i++;
		}
		i++;
		*ptr += 2;	
		return (i + len_to_alloc(line + i, ptr) - 2);
	}
	else
		return (1 + len_to_alloc(line + 1, ptr));
}

void	create_words(t_params *params)
{
	/**
	 * TODO: Go through entire word,
	 * skip IFS/whitespace,
	 * if unquoted string
	 * malloc and add to **words arr
	 * if single quoted string
	 * remove quotes and add to **words
	 * if redirect operator,
	 * find filename and run redir prep
	*/

	t_list *env_lst = params->env_list;
	t_list *cmd_lst = params->cmd_list;

	while (cmd_lst)
	{
		t_cmd *cmd = (t_cmd *) cmd_lst->content;
		char *line = cmd->line;
		// printf("line : %s\n", cmd->line);
		
		int i = 0;

		char **words = ft_calloc(1, sizeof(char *));
		while (line[i])
		{
			if (line[i] && is_space(line[i]))
			{
				i++;
				continue ;
			}
			if (is_redirect(line + i))
			{
				cmd->num_redirects++;
				// skip and process redirect
				// since we have verified that it isn't final
				i += is_redirect(line + i);
				while (is_space(line[i]))
					i++;
				int len = len_to_alloc(line + i, &i);
				i += len;
				printf("redirect len to alloc: %i\n", len);
			}
			else if (line[i] == '$')
			{
				cmd->num_words++;

				char *var = valid_env_var(line + i);
				if (var)
				{
					printf("VAR: %s\n", var);
					char *value = parse_env_var(env_lst, var);
					if (value)
					{
						printf("VALUE: %s\n", value);
					}
					i+= ft_strlen(var) + 1;
					free_str(&var);
				}	
				else
				{
					// invalid $var
					int len = len_to_alloc(line + i, &i);
					i += len;
					printf("len to alloc: %i\n", len);
				}
			}
			else
			{
				cmd->num_words++;
				int len = len_to_alloc(line + i, &i);
				i += len;
				printf("len to alloc: %i\n", len);
			}
			// printf("line: %s\n", line + i);

		}
		printf("num words: %i. num redirects: %i\n", cmd->num_words, cmd->num_redirects);
		cmd_lst = cmd_lst->next;
	}
}



bool is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n');
}

bool is_single_pipe(char *line)
{
	return (line && line[0] == '|' && line[1] != '|');
}

int is_redirect(char *line)
{
	if (!ft_strncmp(line, ">>", 2) || !ft_strncmp(line, "<<", 2))
		return (2);
	else if (!ft_strncmp(line, ">", 1) || !ft_strncmp(line, "<", 1))
		return (1);
	else
		return (0);
}

bool is_meta(char *line)
{
	return (is_redirect(line) || !ft_strncmp(line, "|", 1));
}


void create_cmds(t_params *params)
{
	int total_len;
	char *line;
	char *start;

	if (!params)
		return ; // zero? or neg?
	line = params->line;

	if (!line)
		return ; // zero? or neg?

	total_len = 0;
	start = line;
	while (*line)
	{
		if (*line == '\'')
			line = ft_strchr(line + 1, '\'');
		else if (*line == '"')
			line = ft_strchr(line + 1, '"');
		else if (*line == '|')
		{

			t_cmd *cmd = ft_calloc(1, sizeof(t_cmd));
			t_list *node = ft_calloc(1, sizeof(t_list));

			int len = line - (start + total_len);

			printf("len : %i\n", len);
			cmd->line = ft_substr(params->line, total_len, len);
			total_len += len + 1;

			node->content = cmd;
			ft_lstadd_back(&params->cmd_list, node);
		}

		line++;
		if (!line[0])
		{
			t_cmd *cmd = ft_calloc(1, sizeof(t_cmd));
			t_list *node = ft_calloc(1, sizeof(t_list));

			int len = line - (start + total_len);

			printf("len : %i\n", len);
			cmd->line = ft_substr(params->line, total_len, len);
			total_len += len + 1;

			node->content = cmd;
			ft_lstadd_back(&params->cmd_list, node);
		}

	}


}


int	count_cmds(t_params *params)
{
	int		cmd_count;
	char	*start;
	char	*ptr;
	int		totallen;


	if (!params)
		return (0); // zero? or neg?
	char *line = params->line;

	if (!line)
		return (0); // zero? or neg?
	while (is_space(*line))
		line++;
	if (!line[0])
		return (0);

	cmd_count = 1;
	start = line;
	while (*line)
	{
		if (*line == '\'')
		{
			if (!(line[1]))
			{
				printf("quote syntax error\n");
				return (-1);
			}
			line = ft_strchr(line + 1, '\'');
			if (!line)
			{
				printf("quote syntax error\n");
				return (-1);
			}
		}
		else if (*line == '"')
		{
			if (!(line[1]))
			{
				printf("quote syntax error\n");
				return (-1);
			}
			line = ft_strchr(line + 1, '"');
			if (!line)
			{
				printf("quote syntax error\n");
				return (-1);
			}
		}
		else if (*line == '|')
		{
			ptr = line;
			if (ptr == start || !ptr[1])
			{
				printf("pipe syntax error0\n");
				return (-1);
			}
			ptr--;
			while (is_space(*ptr))
			{
				if (ptr == start)
				{
					printf("pipe syntax error1\n");
					return (-1);
				}
				ptr--;
			}
			if (*ptr == '|')
			{
				printf("pipe syntax error1.5\n");
				return (-1);
			}
			ptr = line + 1;
			while (is_space(*ptr))
			{
				ptr++;
				if (!(*ptr))
				{
					printf("pipe syntax error2\n");
					return (-1);
				}
			}
			if (*ptr == '|')
			{
				printf("pipe syntax error2.5\n");
				return (-1);
			}
			cmd_count++;
		}
		else if (is_redirect(line))
		{
			if (!ft_strncmp(line, "<<", 2) || !ft_strncmp(line, ">>", 2))
				line +=2;
			else
				line++;
			ptr = line;
			while (is_space(*ptr))
			{
				ptr++;
				if (!(*ptr))
				{
					printf("redirect syntax error1\n");
					return (-1);
				}
			}
			if (!ptr[0] || is_meta(ptr))
			{
				printf("redirect syntax error1\n");
				return (-1);
			}
			line--;
		}
		if (line && *line)
			line++;
		else
			break;
	}
	return cmd_count;
}


void	print_env_lst(t_params *params)
{
	t_list *next = params->env_list;
	while (true)
	{
		if (!next)
			break;
		t_env *var = (t_env *) next->content;
		printf("%s=%s\n", var->key, var->value);
		next = next->next;
	}
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