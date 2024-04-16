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

typedef enum e_redir_type
{
	input,
	out_append,
	out_trunc,
	heredoc
}	t_redir_type;

typedef struct s_redir
{
	char			*file;
	t_redir_type	type;
} t_redir;

typedef struct s_proc
{
	int 	pid;
	int 	exit_status;
	bool	exited; // false
}	t_proc;


typedef struct s_cmd
{
	t_proc	proc;
	int		num_words;
	int		num_redirects;
	int		num_heredocs;

	int		io[2];

	char	*line;
	char	**words;
	t_redir	*redirs;
	t_list	*cmd_env_lst;
}	t_cmd;

typedef struct s_env
{
	char				*key;
	char				*value;
} t_env;


typedef struct s_params
{
	int			num_cmds;
	int			total_heredocs;
	char		*line;
	char		**paths;
	bool		interactive; // default true


	int			default_io[2];

	t_list		*cmd_list;
	t_list		*env_list;
}	t_params;

void	*ft_realloc(void *ptr, size_t old_size, size_t size);
char	**find_paths(char **envp);
void	free_str(char **p);
void 	safe_free(void **ptr);
char	*check_valid_cmd(char **paths, char *cmd);
void	parse_quotes(char *line, char ***words);
int		count_cmds(t_params *params);
int		count_strings(char *line);
void	create_cmds(t_params *params);
void	create_words(t_params *params);
bool 	is_meta(char *line);
bool 	is_space(char c);
int		is_redirect(char *line);
t_redir_type get_redir_type(char *line);
void	recurse_pipe(char **paths, t_list *cmd_lst);
void 	run_command(t_params *params, t_list *cmd_lst);
void 	free_env(void *ptr);
bool valid_env_str(char *line);	
bool valid_env_char(char c);



void free_cmds(void *ptr)
{
	t_cmd *cmd = (t_cmd *) ptr;
	char **words = cmd->words;
	t_redir *redirects = cmd->redirs;

	char *types[4] = {"input", "append", "trunc", "heredoc"};


	t_proc proc = cmd->proc;

	if (WIFEXITED(proc.exit_status))
	{
		// printf("%i for command %s exited with status %i\n", proc.pid, words[0], WEXITSTATUS(proc.exit_status));
	}
	else
		printf("PROCESS %i command %s did not exit\n", proc.pid, words[0]);
	int i = 0;
	while (i < cmd->num_words)
	{
		// printf("clearing %p %s\n", words[i], words[i]);
		free_str(words + i);
		i++;
	}
	i = 0;
	while (i < cmd->num_redirects)
	{
		// printf("clearing %p %s ", redirects[i].file, redirects[i].file);
		// printf("of type %s\n", types[redirects[i].type]);
		free_str(&(redirects[i].file));
		i++;
	}
	safe_free((void **) &words);
	safe_free((void **) &redirects);
	safe_free((void **) &(cmd->line));
	safe_free((void **) &(cmd));

}

// if recvd sigiint
// 	if during heredoc (i.e. heredoc_proc = true)
//		close any temp files
//		clear gnl static memory
//		set heredoc_interrupt = true;
//		
//  else if child processes running
// 		if yes, send sigint to all children
// 		optional received and store the return values (esp for last process)
// 		clear the child process list/array
// display new promt


int main(int ac, char **av, char **envp)
{
	t_params	params;
	int 		i;
	ft_memset(&params, 0, sizeof(t_params));
	
	params.default_io[0] = dup(STDIN_FILENO);
	params.default_io[1] = dup(STDOUT_FILENO);

	if (!isatty(params.default_io[1]))
		dup2(open("/dev/tty", O_WRONLY), STDOUT_FILENO);

	/* -------------------------------------------------------------------------- */
	/*                         Process ENVP, create PATHS                         */
	/* -------------------------------------------------------------------------- */
	/**
	 * TODO:
	 * paths should be ascertained 
	 * right before cmd execution.
	 * 
	 * currently, editing PATHS via unset / export
	 * will not change PATHS searched durin cmd execution.
	 * 
	*/
	params.paths = find_paths(envp);
	// i = 0;
	// while (envp && envp[i])
	// {
	// 	char *eqsign = ft_strchr(envp[i], '=');
	// 	unsigned int start = eqsign - envp[i];
	// 	if (eqsign)
	// 	{
	// 		t_env	*node = ft_calloc(1, sizeof(t_env));
	// 		t_list		*list = ft_calloc(1, sizeof(t_list));

	// 		node->key = ft_substr(envp[i], 0, start);
	// 		node->value = ft_substr(envp[i], start + 1, ft_strlen(envp[i]));

	// 		list->content = node;
	// 		ft_lstadd_back(&params.env_list, list);

	// 	}
	// 	i++;
	// }


	/* -------------------------------------------------------------------------- */
	/*                            Read line with prompt                           */
	/* -------------------------------------------------------------------------- */
	static unsigned int totallen;
	while (true)
	{
		free_str(&(params.line));
		params.line = readline("$> ");
		// params.line = get_next_line(params.default_io[0]);
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
			/** DONE: Prepare linked list of simple commands
			 * with output and input redirects
			 * TODO: parse heredoc?
			*/
			create_cmds(&params);

			// create_words(&params);

			/**
			 * DOING: Process heredoc
			 * set heredoc_processing = true;
			 * 
			 * if sigint recvd,
			 * go to next loop iteration
			 * 
			 * once done, set hereodc_proc = false
			*/

			// printf("there are a total of %i heredocs\n", params.total_heredocs);


			/**
			 * TODO: check if resolved to empty string(s)?
			*/


			/**
			 * DONE: Run each child command
			 * 1) for each cmd in cmd_lst
			 * 2) open a pipe
			 * 3) fork
			 * 4) if child, execve cmd words
			 * 5) if parent, recurse
			*/

			// check if heredoc_interrupt == true
			// if yes, skip

			// set interactive to false
			params.interactive = false;
			run_command(&params, params.cmd_list);
			// set interactive to true
			params.interactive = true;
			ft_lstclear(&params.cmd_list, free_cmds);
		}
	}

	// ft_lstclear(&params.env_list, free_env);
	// i = 0;
	// while (params.paths[i])
	// 	safe_free((void **) (params.paths + i++));
	// safe_free((void **) &(params.paths));
	free_str(&(params.line));
}

void run_command(t_params *params, t_list *cmd_lst)
{
	t_cmd *cmd = cmd_lst->content;
	char **argv = cmd->words;
	char *binpath = NULL;

	int p_fd[2];

	if (cmd_lst->next)
		pipe(p_fd);	// error checking??

	// if not single builtin, fork
	int pid = fork();

	// else, run single builtin?

	
	// printf("pid: %i\n", pid);

	if (pid == 0)
	{
		// printf("hello from %i\n", getpid());
		// printf("\t\t %p %s\n", argv, argv[0]);
		bool redirect_success = true;

		if (cmd_lst->next)
		{
			close(p_fd[0]);
			// printf("executing piped %s\n", argv[0]);
			dup2(p_fd[1], STDOUT_FILENO);
			close(p_fd[1]);
		}
		else
			dup2(params->default_io[1], STDOUT_FILENO);


		// process redirects
		if (cmd->num_redirects > 0 && cmd->redirs)
		{

			int redir_ctr = 0;
			while (redirect_success && redir_ctr  < cmd->num_redirects )
			{
				t_redir redir = cmd->redirs[redir_ctr];

				if (redir.type == input)
				{
					int inputfd = open(redir.file, O_RDONLY);
					if (inputfd > 0)
					{
						dup2(inputfd, STDIN_FILENO);
						close(inputfd);
					}
					else
					{
						perror(redir.file);
						redirect_success = false;
					}
				}
				else if (redir.type == out_append)
				{
					int outputfd = open(redir.file, O_WRONLY | O_APPEND | O_CREAT, 0644);
					if (outputfd > 0)
					{
						dup2(outputfd, STDOUT_FILENO);
						close(outputfd);
					}
					else
					{
						perror(redir.file);
						redirect_success = false;
					}
				}
				else if (redir.type == out_trunc)
				{
					int outputfd = open(redir.file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
					if (outputfd > 0)
					{
						dup2(outputfd, STDOUT_FILENO);
						close(outputfd);
					}
					else
					{
						perror(redir.file);
						redirect_success = false;
					}
				}
				else if (redir.type == heredoc)
				{
					int heredoc_pipe[2];
					pipe(heredoc_pipe);
					dup2(heredoc_pipe[0], STDIN_FILENO);
					close(heredoc_pipe[0]);
					char *delim = redir.file;
					int len = ft_strlen(delim);
					char *line = get_next_line(params->default_io[0]);
					while (line)
					{
						if (!ft_strncmp(line, delim, len))
							break ;
						write(heredoc_pipe[1], line, ft_strlen(line));
						free_str(&line);
						line = get_next_line(params->default_io[0]);
					}
					free_str(&line);
					get_next_line(-1);
					close(heredoc_pipe[1]);
				}
				redir_ctr++;
			}
		}
		if (argv[0])
		{
			char *binpath = check_valid_cmd(params->paths, argv[0]);

			if (binpath && redirect_success)
			{
				// printf("\t\t EXECVE  %s\n", binpath);
				execve(binpath, argv, __environ);	// do we need to pass in envp?
				perror("");
			}
			else if (redirect_success)
			{
				ft_putstr_fd(argv[0], STDERR_FILENO);
				ft_putstr_fd(": command not found\n", STDERR_FILENO);
			}
			else
				perror("");
		}
		free_str(&binpath);
		// free all memory
		// use builtin exit
		// printf("\t\t %i EXITING\n", getpid());
		exit(1);
	}
	else
	{
		//parent

		cmd->proc.pid = pid;

		if (cmd->num_heredocs > 0)
		{
			// ft_putstr_fd("waiting for heredoc\n", STDERR_FILENO);
			waitpid(pid, &(cmd->proc.exit_status), 0);
			// ft_putstr_fd("heredoc completed\n", STDERR_FILENO);

		}

		if (cmd_lst->next)
		{
			close(p_fd[1]);
			// printf("executing piped %s\n", ((t_cmd *) cmd_lst->next->content)->words[0]);
			dup2(p_fd[0], STDIN_FILENO);
			close(p_fd[0]);
			run_command(params, cmd_lst->next);
		}
		else
		{
			// final command!

			// return stdin to default
			dup2(params->default_io[0], STDIN_FILENO);

		}
			// printf("hello from parent of %i\n", pid);
			if (WEXITSTATUS(cmd->proc.exit_status))
				printf("process %i exited with status %i \n", pid, WEXITSTATUS(cmd->proc.exit_status));
			else
				waitpid(pid, &(cmd->proc.exit_status), 0);
			cmd->proc.exited = true;

	}
	free_str(&binpath);
	// return exit status
}

char	*check_valid_cmd(char **paths, char *cmd)
{
	int		j;
	char	*cmdpath;
	char	*binpath;

	if (!cmd || !cmd[0])
		return (NULL);
	cmdpath = ft_strjoin("/", cmd);
	j = 0;
	while (paths && paths[j])
	{
		binpath = ft_strjoin(paths[j], cmdpath);
		if (!access(binpath, F_OK | X_OK))
			break ;
		free(binpath);
		j++;
	}
	free(cmdpath);
	if (paths[j] == NULL)
		return (NULL);
	return (binpath);
}

void	run_child_command(int p_fd[2], char **paths, char **cmd)
{
	char	*binpath;

	// printf("attempting to run ");
	// int i = 0;
	// while (cmd[i])
	// 	printf("%s ", cmd[i++]);
	// printf("\n");

	if (cmd && cmd[0] && paths)
		binpath = check_valid_cmd(paths, cmd[0]);
	else
		binpath = NULL;
	if (!binpath)
	{
		ft_putstr_fd(cmd[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		close(p_fd[1]);
		dup2(p_fd[0], STDIN_FILENO);
		close(p_fd[0]);
	}
	else
	{
		dup2(p_fd[1], STDOUT_FILENO);
		close(p_fd[1]);
		execve(binpath, cmd, NULL);
		perror("");
		if (binpath)
			free(binpath);
	}
}

void	recurse_pipe(char **paths, t_list *cmd_lst)
{
	int		p_fd[2];
	int		pid;
	t_cmd	*cmd = cmd_lst->content;
	char	**words = cmd->words;


	pipe(p_fd);
	pid = fork();
	if (pid == 0)
	{
		run_child_command(p_fd, paths, words);
	}
	else
	{
		if (cmd_lst->next)
		{
			close(p_fd[1]);
			dup2(p_fd[0], STDIN_FILENO);
			close(p_fd[0]);
			printf("\t\t recursing\n");
			recurse_pipe(paths, cmd_lst->next);
		}
		else
		{
			char c;
			close(p_fd[1]);

			while (read(p_fd[0], &c, 1))
				write(STDOUT_FILENO, &c, 1);
			waitpid(pid, NULL, 0);

		}
		wait(NULL);
	}
}


char *get_env_key(char *line)
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

int		len_to_alloc(char **line_ptr, t_list *env_lst, char qstart)
{
	if (!qstart && (!**line_ptr || is_redirect(*line_ptr) || is_space(**line_ptr)))
		return (0);

	if (qstart && **line_ptr == qstart)
	{
		(*line_ptr)++;
		qstart = 0;
	}

	if (!qstart && (!**line_ptr || is_redirect(*line_ptr) || is_space(**line_ptr)))
		return (1);
	else if (!qstart && **line_ptr == '\'')
	{
		(*line_ptr)++;
		return (len_to_alloc(line_ptr, env_lst, '\''));
	}
	else if (!qstart && **line_ptr == '"')
	{
		(*line_ptr)++;
		return (len_to_alloc(line_ptr, env_lst, '"'));

	}
	else if (qstart != '\'' && valid_env_str(*line_ptr))
	{
		char *var = get_env_key(*line_ptr);
		int	key_len = 0;
		int len = 0;

		// printf("\t\t VAR: %s\n", var);
		char *value = getenv(var);
		if (value)
		{
			// printf("\t\t VALUE: %s\n", value);
			len = ft_strlen(value);
		}
		key_len = ft_strlen(var) + 1;
		*line_ptr += key_len;
		free_str(&var);
		// printf("\t\t env var len to alloc: %i\n", len);
		return (len + len_to_alloc(line_ptr, env_lst, qstart));
	}
	else
	{	
		(*line_ptr)++;
		return (1 + len_to_alloc(line_ptr, env_lst, qstart));
	}
}


/**
 * takes the raw line,
 * pointer to word to copy
 * copies byte by byte till hits a delim based on previous logic
*/

void	word_copy(char **line_ptr, t_list *env_lst, char qstart, char *word)
{
	if (qstart && **line_ptr == qstart)
	{
		(*line_ptr)++;
		qstart = 0;
	}

	if (!qstart && (!**line_ptr || is_redirect(*line_ptr) || is_space(**line_ptr)))
	{
		// size = 0;
		return ;
	}

	else if (!qstart && **line_ptr == '\'')
	{
		(*line_ptr)++;
		return (word_copy(line_ptr, env_lst, '\'', word));
	}
	else if (!qstart && **line_ptr == '"')
	{
		(*line_ptr)++;
		return (word_copy(line_ptr, env_lst, '"', word));
	}
	else if (qstart != '\'' && valid_env_str(*line_ptr))
	{
		char *var = get_env_key(*line_ptr);
		int	key_len = 0;
		int len = 0;

		// printf("\t\t VAR: %s\n", var);
		char *value = getenv(var);
		if (value)
		{
			// printf("\t\t VALUE: %s\n", value);
			len = ft_strlen(value);
			ft_strlcpy(word, value, len + 1);
			word += len;
		}
		key_len = ft_strlen(var) + 1;
		*line_ptr += key_len;
		free_str(&var);
		// printf("\t\t env var len to alloc: %i\n", len);
		return (word_copy(line_ptr, env_lst, qstart, word));
	}
	else
	{
		*word = **line_ptr;
		(*line_ptr)++;
		word++;
		return (word_copy(line_ptr, env_lst, qstart, word));
	}
}

void parse_cmd(t_list *cmd_lst, t_list *env_lst)
{
	t_cmd *cmd = (t_cmd *) cmd_lst->content;
	char *line = cmd->line;
	// printf("line : %s\n", cmd->line);
	// int i = 0;

	char **words = ft_calloc(1, sizeof(char **));
	// char **redirects = ft_calloc(1, sizeof(char **));
	t_redir *redirs = cmd->redirs;
	while (*line)
	{
		if (*line && is_space(*line))
		{
			line++;
			continue ;
		}
		if (is_redirect(line))
		{
			t_redir_type type = get_redir_type(line);


			if (type == heredoc)
				cmd->num_heredocs++;
			// skip and process redirect
			// since we have verified that it isn't final
			line += is_redirect(line);
			while (is_space(*line))
				line++;
			char *copy = line;
			int len = len_to_alloc(&line, env_lst, 0);
			// printf("allocating pointer of len %i\n", len);
			// printf("line is crrently %s\n", line);
			redirs = ft_realloc(redirs, cmd->num_redirects * sizeof(t_redir), (cmd->num_redirects + 1) * sizeof(t_redir));
			char *redir_file = ft_calloc(len + 1, sizeof(char));
			word_copy(&copy, env_lst, 0, redir_file);

			redirs[cmd->num_redirects].file = redir_file;
			redirs[cmd->num_redirects].type = type;

			cmd->num_redirects++;
			// printf("redirect len to alloc: %i\n", len);
		}
		else
		{
			char *copy = line;
			int len = len_to_alloc(&line, env_lst, 0);
			if (len)
			{
				// char *word = ft_calloc(len, sizeof(char));
				// printf("allocating pointer of len %i\n", len);
				// printf("line is crrently %s\n", line);
				char *word = ft_calloc(len + 1, sizeof(char));
				words[cmd->num_words] = word;
				word_copy(&copy, env_lst, 0, words[cmd->num_words]);
				cmd->num_words++;
				words = ft_realloc(words, cmd->num_words * sizeof(char*), (cmd->num_words + 1) * sizeof(char*));
			}
		}
		// printf("i: %i. line: %s\n", i,  line + i);
	}
	cmd->words = words;
	cmd->redirs = redirs;
	// printf("num words: %i. num redirects: %i\n", cmd->num_words, cmd->num_redirects);
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
		parse_cmd(cmd_lst, env_lst); // cmd should point to env lst
		// copy_bytes(cmd_lst, env_lst);
		params->total_heredocs += ((t_cmd *) cmd_lst->content)->num_heredocs;
		cmd_lst = cmd_lst->next;
	}
}




void create_cmds(t_params *params)
{
	int total_len;
	char *line;
	char *start;

	if (!params)
		return ;
	line = params->line;

	if (!line)
		return ;

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

			// printf("len : %i\n", len);
			cmd->line = ft_substr(params->line, total_len, len);
			total_len += len + 1;

			node->content = cmd;
			parse_cmd(node, params->env_list);
			ft_lstadd_back(&params->cmd_list, node);
		}

		line++;
		if (!line[0])
		{
			t_cmd *cmd = ft_calloc(1, sizeof(t_cmd));
			t_list *node = ft_calloc(1, sizeof(t_list));

			int len = line - (start + total_len);

			// printf("total cmd len : %i\n", len);
			cmd->line = ft_substr(params->line, total_len, len);
			total_len += len + 1;

			node->content = cmd;
			parse_cmd(node, params->env_list);
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
		return (0); // zero? or neg?

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
		safe_free((void **) ptr);
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

/* -------------------------------------------------------------------------- */
/*                                    UTILS                                   */
/* -------------------------------------------------------------------------- */

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

bool valid_env_char(char c)
{
	return (ft_isalnum(c) || c == '_');
}


bool valid_env_str(char *line)
{
	return (*line == '$' && (line[1] == '?' || line[1] == '_' || ft_isalpha(line[1])));
}

t_redir_type get_redir_type(char *line)
{
	int i = 0;

	if (!ft_strncmp(line, ">>", 2))
		return out_append;
	else if (!ft_strncmp(line, "<<", 2))
		return heredoc;
	else if (!ft_strncmp(line, ">", 1))
		return out_trunc;
	else if (!ft_strncmp(line, "<", 1))
		return input;
	else
		return (-1);
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

