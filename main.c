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
int		count_cmds(t_params *params);
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



#include <string.h>

int main(int ac, char **av, char **envp)
{
	t_params	params;
	int 		i;
	ft_memset(&params, 0, sizeof(t_params));
	

	// params.default_io[0] = dup(STDIN_FILENO);
	// params.default_io[1] = dup(STDOUT_FILENO);

	// params.tty_io[0] = open("/dev/tty", O_RDONLY);
	// params.tty_io[1] = open("/dev/tty", O_WRONLY);
	// if (!isatty(params.default_io[1]))
	// {
	// 	// write(2, "xx\n", 3);
	// 	dup2(params.tty_io[1], STDOUT_FILENO);
	// 	close(params.tty_io[1]);
	// }

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
	
    // environ = (char **) ft_calloc(i + 1, sizeof(char *));
	// // i = 0;

	// while (envp[i])
	// {
	// 	// printf("xx %s\n", envp[i]);
    // 	environ[i] = ft_strdup(envp[i]);
	// 	i++;
	// }

	// __environ = NULL;

	
	// printf("default: %i \n", rl_prefer_env_winsize);
	// rl_prefer_env_winsize = 1;
	/* -------------------------------------------------------------------------- */
	/*                            Read line with prompt                           */
	/* -------------------------------------------------------------------------- */
	static unsigned int totallen;
	while (true)
	{
		free_str(&(params.line));
		// rl_outstream = stderr;
		params.line = readline("$> ");
		if (!params.env_count)
		{

			i = 0;
			while (envp[i])
				i++;  // find the end of __

			params.env_count = i;

			params.envs = ft_calloc((i + 1), sizeof(char *));
			i = -1;
			while (envp[++i])
				params.envs[i] = ft_strdup(envp[i]);

			__environ = params.envs;

		}
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
			int code = run_command(&params, params.cmd_list);
			// set interactive to true
			params.interactive = true;

			printf("%i: received %i in main\n", getpid(), code);
			char *result = ft_itoa(code);
			char *key = ft_strjoin("?=", result);
			set_env(key);
			free_str(&key);
			free_str(&result);

			ft_lstclear(&params.cmd_list, free_cmds);
		}
		else if (!params.line)
		{
			ms_exit(&params, 0);
		}
	}

	free_str(&(params.line));
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
		return run_builtin(params, cmd_lst);
	}
	int pid = fork();

	// else, run single builtin?

	static int default_stdin;
	static int default_stdout;

	if (!default_stdin)
		default_stdin = dup(STDIN_FILENO);
	if (!default_stdout)
		default_stdout = dup(STDOUT_FILENO);

	if (pid == 0)
	{
		// printf("hello from %i\n", getpid());
		// printf("\t\t %p %s\n", argv, argv[0]);
		bool redirect_success = true;
		int code = 0;

		if (cmd_lst->next)
		{
			close(p_fd[0]);
			// printf("executing piped %s\n", argv[0]);
			dup2(p_fd[1], STDOUT_FILENO);
			close(p_fd[1]);
		}
		// else
		// {
		// 	dup2(default_stdout, STDOUT_FILENO);
		// 	// close(default_stdout);
		// }


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
					char *delim = redir.file;
					int len = ft_strlen(delim);
					char *line = get_next_line(default_stdin);
					while (line)
					{
						if (!ft_strncmp(line, delim, len))
							break ;
						write(heredoc_pipe[1], line, ft_strlen(line));
						free_str(&line);
						line = get_next_line(default_stdin);
					}
					dup2(heredoc_pipe[0], STDIN_FILENO);
					close(heredoc_pipe[0]);
					close(heredoc_pipe[1]);
					free_str(&line);
					get_next_line(-1);

				}
				redir_ctr++;
			}
		}

		if (is_builtin(argv))
			code = run_builtin(params, cmd_lst);
		else if (argv[0])
		{
			params->paths = find_paths();
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
		ms_exit(params, code);
	}
	//parent
	cmd->proc.pid = pid;

	// eventually done at parent level

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
		waitpid(pid, &(cmd->proc.exit_status), 0);
		cmd->proc.exited = true;
		// if (isatty(params->default_io[0]))

		dup2(default_stdin, STDIN_FILENO);
		// close(default_stdin);
		// dup2(params->default_io[1], STDOUT_FILENO);
		// else
		// 	dup2(open("/dev/tty", O_RDONLY), STDIN_FILENO);

		// printf("final exit status of %s is %i\n", cmd->words[0], WEXITSTATUS(cmd->proc.exit_status));

		return (WEXITSTATUS(cmd->proc.exit_status));

	}
	// printf("hello from parent of %i\n", pid);
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