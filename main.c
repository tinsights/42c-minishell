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

#include "minishell.h"

void	free_str(char **p);
// void 	safe_free(void **ptr);
void 	free_cmds(void *ptr);

char	**find_paths(void);

char	*check_valid_cmd(char **paths, char *cmd);
// void	parse_quotes(char *line, char ***words);
int		count_cmds(char *line);
// int		count_strings(char *line);
void	create_cmds(t_params *params);
// void	create_words(t_params *params);

// void parse_cmd(t_list *cmd_lst);


// bool 	is_meta(char *line);
// bool 	is_space(char c);
// int		is_redirect(char *line);
// t_redir_type get_redir_type(char *line);
// void	recurse_pipe(char **paths, t_list *cmd_lst);
int 	run_command(t_params *params, t_list *cmd_lst);
// void 	free_env(void *ptr);
// bool 	valid_env_start(char *line);	
// bool 	valid_env_char(char c);
void		set_env(char *str);
// void 		print_env(void);
// void 		ms_export(char *arg);
// void 		unset_env(char *var);

void		ms_exit(t_params *params, int code);
int 	run_builtin(t_params *params, t_list *cmd_lst);


int		len_to_alloc(char **line_ptr, char qstart, bool is_heredoc);

bool	word_copy(char **line_ptr, char qstart, char *word, bool is_heredoc);

void	process_heredocs(t_params* params, t_list	*cmd_lst);


int code;

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


void handle_sigint(int sig)
{
	ft_putstr_fd("^C\n", STDIN_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	code = 130;
}


void set_code(int code)
{
	char *result = ft_itoa(code);
	char *key = ft_strjoin("?=", result);
	set_env(key);
	free_str(&key);
	free_str(&result);
}

int main(int ac, char **av, char **envp)
{
	t_params	params;
	int 		i;
	ft_memset(&params, 0, sizeof(t_params));
	struct sigaction sa;
	
	
	rl_outstream = stderr;
	// sa.sa_handler = &handle_sigint;
	// sigaction(SIGINT, &sa, NULL);

	params.default_io[0] = dup(STDIN_FILENO);
	params.default_io[1] = dup(STDOUT_FILENO);
	params.default_io[2] = dup(STDERR_FILENO);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	rl_catch_signals = 0;
	readline(NULL);
	dup2(params.default_io[0], STDIN_FILENO);
	dup2(params.default_io[1], STDOUT_FILENO);
	dup2(params.default_io[2], STDERR_FILENO);

	if (!params.env_count)
	{
		i = 0;
		while (envp[i])
			i++;  // find the end of __

		params.env_count = i;

		params.envs = ft_calloc((i + 2), sizeof(char *));
		i = -1;
		while (envp[++i])
			params.envs[i] = ft_strdup(envp[i]);
		
		__environ = params.envs;
		if (!getenv("?"))
			set_env("?=0");

		if (getenv("SHLVL"))
		{
			int lvl = ft_atoi(getenv("SHLVL"));
			char *result = ft_itoa(lvl + 1);
			char *key = ft_strjoin("SHLVL=", result);
			set_env(key);
			free_str(&key);
			free_str(&result);
		}
	}
	/* -------------------------------------------------------------------------- */
	/*                            Read line with prompt                           */
	/* -------------------------------------------------------------------------- */
	bool init = false;
	while (true)
	{
		sa.sa_handler = &handle_sigint;
		sigaction(SIGINT, &sa, NULL);
		free_str(&(params.line));
		params.interactive = true;
		
		params.line = readline("minishell$> ");
		set_code(code);

		if (params.line && *params.line)
		{
			add_history(params.line);

			params.num_cmds = count_cmds(params.line);
			if (params.num_cmds <= 0)
				continue ;
			/** DONE: Prepare linked list of simple commands
			 * with output and input redirects
			 * 
			*/
			create_cmds(&params);

			// printf("there are a total of %i heredocs\n", params.total_heredocs);
			/**
			 * DOING: Process heredoc
			 * set heredoc_processing = true;
			 * 
			 * if sigint recvd,
			 * go to next loop iteration
			 * 
			 * once done, set hereodc_proc = false
			*/
			
			if (params.total_heredocs)
				process_heredocs(&params, params.cmd_list);

			// check if heredoc_interrupt == true
			// if yes, skip

			/**
			 * DONE: Run each child command
			 * 1) for each cmd in cmd_lst
			 * 2) open a pipe
			 * 3) fork
			 * 4) if child, execve cmd words
			 * 5) if parent, recurse
			*/
			if (params.interactive)
			{	
				sa.sa_handler = SIG_IGN;
				sigaction(SIGINT, &sa, NULL);
				code = run_command(&params, params.cmd_list);
				// dprintf(2, "%i: received %i in main\n", getpid(), code);
			}
			set_code(code);
			ft_lstclear(&params.cmd_list, free_cmds);
		}
		else if (!params.line)
		{
			ms_exit(&params, 0);
		}
	}
}

void heredoc_sigint(int sig)
{
	ft_putstr_fd("\n", STDERR_FILENO);
	code = 130;
}
bool	is_space(char c);
int		is_redirect(char *line);


void	process_heredocs(t_params* params, t_list	*cmd_lst)
{
	if (!cmd_lst)
		return;

	struct sigaction sa;
	sa.sa_handler = &heredoc_sigint;
	sigaction(SIGINT, &sa, NULL);

	code = 0;
	params->interactive = false;
	while (cmd_lst && !code)
	{
		t_cmd *cmd = cmd_lst->content;
		t_redir *redirs = cmd->redirs;
		int i = 0;
		int processed = 0;
		while (i < cmd->num_redirects)
		{
			if (redirs[i].type == heredoc)
			{
				int heredoc_pipe[2];
				pipe(heredoc_pipe);
				char *delim = redirs[i].file;
				int len = ft_strlen(delim);
				ft_putstr_fd(delim, 2);
				ft_putstr_fd("> ", 2);
				char *line = get_next_line(params->default_io[0]);
				while (line)
				{
					if (!ft_strncmp(line, delim, len))
						break ;
					if (redirs[i].quoted)
						write(heredoc_pipe[1], line, ft_strlen(line));
					else
					{
						char *line_ref = line;
						while (line_ref && *line_ref)
						{
							if (*line_ref && is_space(*line_ref))
							{
								write(heredoc_pipe[1], line_ref, 1);
								line_ref++;
								continue ;
							}
							if (is_redirect(line_ref))
							{
								write(heredoc_pipe[1], line_ref, is_redirect(line_ref));
								line_ref += is_redirect(line_ref);
								continue ;
							}
							char *copy = line_ref;
							int len = len_to_alloc(&line_ref, 0, false);
							char *word = ft_calloc(len + 1, sizeof(char));
							word_copy(&copy, 0, word, false);
							write(heredoc_pipe[1], word, ft_strlen(word));
							free_str(&word);
						}
					}
					free_str(&line);
					ft_putstr_fd(delim, 2);
					ft_putstr_fd("> ", 2);
					line = get_next_line(params->default_io[0]);
				}
				if (processed)
					close(cmd->heredoc_fd);
				if (line)
					cmd->heredoc_fd = dup(heredoc_pipe[0]);
				else
				{
					cmd->heredoc_fd = -1;
				}
				close(heredoc_pipe[0]);
				close(heredoc_pipe[1]);
				free_str(&line);
				get_next_line(-1);
				processed++;
			}
			i++;
		}
		cmd_lst = cmd_lst->next;
	}
	set_code(code);
	params->interactive = !code;
}

bool is_builtin(char **argv)
{
	if (!argv || !argv[0])
		return false;
	
	return (!ft_strncmp(argv[0], "export", 7)
	|| !ft_strncmp(argv[0], "env", 4)
	|| !ft_strncmp(argv[0], "unset", 6)
	// || !ft_strncmp(argv[0], "echo", 5)
	|| !ft_strncmp(argv[0], "exit", 5));
}


int run_command(t_params *params, t_list *cmd_lst)
{
	t_cmd *cmd = cmd_lst->content;
	char **argv = cmd->words;
	char *binpath = NULL;

	int p_fd[2];

	if (cmd_lst->next)
		pipe(p_fd);	// error checking??
	else if (params->num_cmds == 1 &&  is_builtin(argv))
	{
		// TODO: run redirections
		return run_builtin(params, cmd_lst);
	}
	int pid = fork();

	if (pid == 0)
	{
		bool redirect_success = true;

		if (cmd_lst->next)
		{
			close(p_fd[0]);
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
						code = errno;
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
						code = errno;
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
						code = errno;
						perror(redir.file);
						redirect_success = false;
					}
				}
				else if (redir.type == heredoc)
				{
					if (cmd->heredoc_fd > 0)
					{
						dup2(cmd->heredoc_fd, STDIN_FILENO);
						close(cmd->heredoc_fd);
					}
					else
						dup2(open("/dev/null", O_RDONLY), STDIN_FILENO);
				}
				redir_ctr++;
			}
		}

		if (redirect_success && is_builtin(argv))
			code = run_builtin(params, cmd_lst);
		else if (argv[0])
		{
			params->paths = find_paths();
			char *binpath = check_valid_cmd(params->paths, argv[0]);
			if (binpath && redirect_success)
			{
				// printf("\t\t EXECVE  %s\n", binpath);
				struct sigaction sa;
				sa.sa_handler = SIG_DFL;
				sigaction(SIGINT, &sa, NULL);
				execve(binpath, argv, __environ);	// do we need to pass in envp?
				perror("");
				code = 2;
			}
			else if (redirect_success)
			{
				ft_putstr_fd(argv[0], STDERR_FILENO);
				ft_putstr_fd(": command not found\n", STDERR_FILENO);
				code = 127;
			}
		}
		free_str(&binpath);
		ms_exit(params, code);
	}
	//parent
	cmd->proc.pid = pid;
	if (cmd_lst->next)
	{
		close(p_fd[1]);
		dup2(p_fd[0], STDIN_FILENO);
		close(p_fd[0]);
		run_command(params, cmd_lst->next);
	}
	else
		dup2(params->default_io[0], STDIN_FILENO);
	waitpid(pid, &(cmd->proc.exit_status), 0);
	cmd->proc.exited = true;
	return (WEXITSTATUS(cmd->proc.exit_status));
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

char **find_paths(void)
{
	char	*paths_var;
	char	**paths;

	// while (*envp && ft_strncmp(*envp, "PATH", 4))
	// 	envp++;
	paths_var = getenv("PATH");
	if (paths_var)
		paths = ft_split(paths_var, ':');
	else
		paths = (char **)(ft_calloc(1, sizeof(char *)));

	// calc num of paths in order to realloc first one to cwd
	int i = 0;
	while (paths[i])
		i++;
	paths = ft_realloc(paths, (i + 1) * sizeof(char *), (i + 2) * sizeof(char *));
	paths[i] = getcwd(NULL, 500);
	return paths;
}