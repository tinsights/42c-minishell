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
void	free_cmds(void *ptr);
char	**find_paths(void);
char	*check_valid_cmd(char **paths, char *cmd);
int		count_cmds(char *line);
void	create_cmds(t_params *params);
int		run_command(t_params *params, t_list *cmd_lst);
void	set_env(char *str);
void	ms_exit(t_params *params, int g_code);
int		run_builtin(t_params *params, t_list *cmd_lst);
int		len_to_alloc(char **line_ptr, char qstart, bool is_heredoc);
bool	word_copy(char **line_ptr, char qstart, char *word, bool is_heredoc);
void	process_heredocs(t_params *params, t_list *cmd_lst);
int		g_code;

void	handle_sigint(int sig)
{
	ft_putstr_fd("^C\n", STDIN_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_code = 130;
}

void	set_g_code(int code)
{
	char	*result;
	char	*key;

	g_code = code;
	result = ft_itoa(g_code);
	key = ft_strjoin("?=", result);
	set_env(key);
	free_str(&key);
	free_str(&result);
}

void	init_io(t_params *params)
{
	rl_outstream = stderr;
	params->default_io[0] = dup(STDIN_FILENO);
	params->default_io[1] = dup(STDOUT_FILENO);
	params->default_io[2] = dup(STDERR_FILENO);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	rl_catch_signals = 0;
	readline(NULL);
	dup2(params->default_io[0], STDIN_FILENO);
	dup2(params->default_io[1], STDOUT_FILENO);
	dup2(params->default_io[2], STDERR_FILENO);
}

bool	child_interrupted(t_list *cmd_lst)
{
	t_cmd	*cmd;

	while (cmd_lst)
	{
		cmd = cmd_lst->content;
		if (WIFSIGNALED(cmd->proc.exit_status))
			return (true);
		cmd_lst = cmd_lst->next;
	}
	return (false);
}

bool	run_line(t_params *params)
{
	add_history(params->line);
	params->num_cmds = count_cmds(params->line);
	if (params->num_cmds <= 0)
		return (false);
	create_cmds(params);
	if (params->total_heredocs)
		process_heredocs(params, params->cmd_list);
	if (params->interactive)
	{
		signal(SIGINT, SIG_IGN);
		g_code = run_command(params, params->cmd_list);
		if (child_interrupted(params->cmd_list))
			write(1, "\n", 1);
	}
	set_g_code(g_code);
	ft_lstclear(&params->cmd_list, free_cmds);
	return (true);
}

void	set_global_envs(void)
{
	int		lvl;
	char	*result;
	char	*key;

	if (!getenv("?"))
		set_g_code(0);
	else
		set_g_code(ft_atoi(getenv("?")));
	if (getenv("SHLVL"))
	{
		lvl = ft_atoi(getenv("SHLVL"));
		result = ft_itoa(lvl + 1);
		key = ft_strjoin("SHLVL=", result);
		set_env(key);
		free_str(&key);
		free_str(&result);
	}
	set_env("SHELL=minishell");
}

void	init_env(t_params *params, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	params->env_count = i;
	params->envs = ft_calloc((i + 2), sizeof(char *));
	i = -1;
	while (envp[++i])
		params->envs[i] = ft_strdup(envp[i]);
	__environ = params->envs;
	set_global_envs();
}

int	main(int ac, char **av, char **envp)
{
	t_params	params;
	int			i;

	ft_memset(&params, 0, sizeof(t_params));
	init_io(&params);
	init_env(&params, envp);
	signal(SIGQUIT, SIG_IGN);
	while (true)
	{
		params.sa.sa_handler = &handle_sigint;
		sigaction(SIGINT, &params.sa, NULL);
		free_str(&(params.line));
		params.interactive = true;
		params.line = readline("minishell$> ");
		set_g_code(g_code);
		if (params.line && *params.line)
			if (!run_line(&params))
				continue ;
		if (!params.line)
			ms_exit(&params, 0);
	}
}

void	heredoc_sigint(int sig)
{
	ft_putstr_fd("\n", STDERR_FILENO);
	g_code = 130;
}
bool	is_space(char c);
int		is_redirect(char *line);

void	finalize_heredoc(t_cmd *cmd, char **line, int heredoc_pipe[2])
{
	if (cmd->heredoc_fd > 0)
		close(cmd->heredoc_fd);
	if (*line)
		cmd->heredoc_fd = dup(heredoc_pipe[0]);
	else
		cmd->heredoc_fd = -1;
	close(heredoc_pipe[0]);
	close(heredoc_pipe[1]);
	free_str(line);
	get_next_line(-1);
}

void	expand_heredoc_line(char *line_ref, int heredoc_pipe[2])
{
	char	*copy;
	int		len;
	char	*word;

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
		copy = line_ref;
		len = len_to_alloc(&line_ref, 0, false);
		word = ft_calloc(len + 1, sizeof(char));
		word_copy(&copy, 0, word, false);
		write(heredoc_pipe[1], word, ft_strlen(word));
		free_str(&word);
	}
}

void	run_heredoc(t_params *params, t_redir redir, t_cmd *cmd)
{
	int		heredoc_pipe[2];
	char	*line;

	if (redir.type == heredoc)
	{
		pipe(heredoc_pipe);
		ft_putstr_fd(redir.file, 2);
		ft_putstr_fd("> ", 2);
		line = get_next_line(params->default_io[0]);
		while (line)
		{
			if (!ft_strncmp(line, redir.file, ft_strlen(redir.file)))
				break ;
			if (redir.quoted)
				write(heredoc_pipe[1], line, ft_strlen(line));
			else
				expand_heredoc_line(line, heredoc_pipe);
			free_str(&line);
			ft_putstr_fd(redir.file, 2);
			ft_putstr_fd("> ", 2);
			line = get_next_line(params->default_io[0]);
		}
		finalize_heredoc(cmd, &line, heredoc_pipe);
	}
}

void	process_heredocs(t_params *params, t_list *cmd_lst)
{
	t_cmd	*cmd;
	t_redir	*redirs;
	int		i;

	if (!cmd_lst)
		return ;
	params->sa.sa_handler = &heredoc_sigint;
	sigaction(SIGINT, &params->sa, NULL);
	g_code = 0;
	params->interactive = false;
	while (cmd_lst && !g_code)
	{
		cmd = cmd_lst->content;
		redirs = cmd->redirs;
		i = -1;
		while (++i < cmd->num_redirects)
			if (redirs[i].type == heredoc)
				run_heredoc(params, redirs[i], cmd);
		cmd_lst = cmd_lst->next;
	}
	set_g_code(g_code);
	params->interactive = !g_code;
}

bool	is_builtin(char **argv)
{
	if (!argv || !argv[0])
		return (false);
	return (!ft_strncmp(argv[0], "export", 7) || !ft_strncmp(argv[0], "env", 4)
		|| !ft_strncmp(argv[0], "unset", 6) || !ft_strncmp(argv[0], "exit", 5)
		|| !ft_strncmp(argv[0], "echo", 5) || !ft_strncmp(argv[0], "cd", 3)
		|| !ft_strncmp(argv[0], "pwd", 4));
}

void	ms_dup(int newfd, int oldfd)
{
	dup2(newfd, oldfd);
	close(newfd);
}

bool	handle_redir(t_cmd *cmd, t_redir redir)
{
	int	fd;

	fd = 0;
	if (redir.type == input)
	{
		fd = open(redir.file, O_RDONLY);
		if (fd > 0)
			ms_dup(fd, STDIN_FILENO);
	}
	else if (redir.type == out_append)
	{
		fd = open(redir.file, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (fd > 0)
			ms_dup(fd, STDOUT_FILENO);
	}
	else if (redir.type == out_trunc)
	{
		fd = open(redir.file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (fd > 0)
			ms_dup(fd, STDOUT_FILENO);
	}
	else if (redir.type == heredoc)
		ms_dup(cmd->heredoc_fd, STDIN_FILENO);
	return (fd >= 0);
}

bool	process_redirects(t_cmd *cmd)
{
	bool	redirect_success;
	int		redir_ctr;

	redirect_success = true;
	redir_ctr = -1;
	while (redirect_success && ++redir_ctr < cmd->num_redirects)
		redirect_success = handle_redir(cmd, cmd->redirs[redir_ctr]);
	if (!redirect_success)
	{
		perror(cmd->redirs[redir_ctr].file);
		g_code = 1;
	}
	return (redirect_success);
}

void	run_cmd(t_params *params, bool redir_suc, char **argv)
{
	char	*binpath;

	params->paths = find_paths();
	binpath = check_valid_cmd(params->paths, argv[0]);
	if (binpath && redir_suc)
	{
		signal(SIGQUIT, SIG_DFL);
		signal(SIGINT, SIG_DFL);
		execve(binpath, argv, __environ);
		perror("");
		g_code = 2;
	}
	else
	{
		ft_putstr_fd(argv[0], STDERR_FILENO);
		ft_putstr_fd(": command not found\n", STDERR_FILENO);
		g_code = 127;
	}
	free_str(&binpath);
}

void	run_child(t_params *params, t_list *cmd_lst, char **argv, int p_fd[2])
{
	t_cmd	*cmd;
	bool	redirect_success;

	cmd = cmd_lst->content;
	redirect_success = true;
	if (cmd_lst->next)
	{
		close(p_fd[0]);
		ms_dup(p_fd[1], STDOUT_FILENO);
	}
	else
		dup2(params->default_io[1], STDOUT_FILENO);
	if (cmd->num_redirects > 0 && cmd->redirs)
		redirect_success = process_redirects(cmd);
	if (redirect_success)
	{
		if (is_builtin(argv))
			g_code = run_builtin(params, cmd_lst);
		else if (argv[0])
			run_cmd(params, redirect_success, argv);
	}
	ms_exit(params, g_code);
}

void	run_parent(t_params *params, t_list *cmd_lst, t_cmd *cmd, int p_fd[2])
{
	if (cmd_lst->next)
	{
		close(p_fd[1]);
		ms_dup(p_fd[0], STDIN_FILENO);
		run_command(params, cmd_lst->next);
	}
	else
		dup2(params->default_io[0], STDIN_FILENO);
}

int	run_single(t_params *params, t_list *cmd_lst)
{
	int	e_status;

	e_status = 1;
	if (process_redirects(cmd_lst->content))
		e_status = run_builtin(params, cmd_lst);
	dup2(params->default_io[0], STDIN_FILENO);
	dup2(params->default_io[1], STDOUT_FILENO);
	return (e_status);
}

int	run_command(t_params *params, t_list *cmd_lst)
{
	t_cmd	*cmd;
	int		pid;
	int		p_fd[2];

	cmd = cmd_lst->content;
	if (cmd_lst->next)
		pipe(p_fd);
	else if (params->num_cmds == 1 && is_builtin(cmd->words))
		return (run_single(params, cmd_lst));
	pid = fork();
	if (pid == 0)
		run_child(params, cmd_lst, cmd->words, p_fd);
	cmd->proc.pid = pid;
	run_parent(params, cmd_lst, cmd, p_fd);
	waitpid(pid, &(cmd->proc.exit_status), 0);
	if (WIFEXITED(cmd->proc.exit_status))
		return (WEXITSTATUS(cmd->proc.exit_status));
	else
		return (128 + WTERMSIG(cmd->proc.exit_status));
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

char	**find_paths(void)
{
	char	*paths_var;
	char	**paths;
	int		i;

	paths_var = getenv("PATH");
	if (paths_var)
		paths = ft_split(paths_var, ':');
	else
		paths = (char **)(ft_calloc(1, sizeof(char *)));
	i = 0;
	while (paths[i])
		i++;
	paths = ft_realloc(paths, (i + 1) * sizeof(char *), (i + 2)
			* sizeof(char *));
	paths[i] = getcwd(NULL, 500);
	return (paths);
}
