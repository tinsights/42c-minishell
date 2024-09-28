/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 12:27:42 by tjegades          #+#    #+#             */
/*   Updated: 2024/04/23 12:27:54 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	**find_paths(void);
char	*check_valid_cmd(char **paths, char *cmd);
bool	is_builtin(char **argv);
bool	process_redirects(t_cmd *cmd);
int		run_builtin(t_params *params, t_cmd *cmd);

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
		ms_dup(params->default_io[1], STDOUT_FILENO);
	if (cmd->num_redirects > 0 && cmd->redirs)
		redirect_success = process_redirects(cmd);
	close(params->default_io[0]);
	close(params->default_io[1]);
	close(params->default_io[2]);
	if (redirect_success)
	{
		if (is_builtin(argv))
			g_code = run_builtin(params, cmd_lst->content);
		else if (argv && argv[0])
			run_cmd(params, redirect_success, argv);
	}
	ms_exit(params, g_code, false);
}

void	run_parent(t_params *params, t_list *cmd_lst, t_cmd *cmd, int p_fd[2])
{
	int	code;

	if (cmd_lst->next)
	{
		close(p_fd[1]);
		ms_dup(p_fd[0], STDIN_FILENO);
		run_command(params, cmd_lst->next);
		waitpid(cmd->proc.pid, &(cmd->proc.exit_status), 0);
	}
	else
	{
		dup2(params->default_io[0], STDIN_FILENO);
		waitpid(cmd->proc.pid, &(cmd->proc.exit_status), 0);
		if (WIFEXITED(cmd->proc.exit_status))
			code = WEXITSTATUS(cmd->proc.exit_status);
		else
			code = 128 + WTERMSIG(cmd->proc.exit_status);
		set_g_code(code);
	}
}

void	run_single(t_params *params, t_list *cmd_lst)
{
	int	e_status;

	e_status = 1;
	if (process_redirects(cmd_lst->content))
		e_status = run_builtin(params, cmd_lst->content);
	dup2(params->default_io[0], STDIN_FILENO);
	dup2(params->default_io[1], STDOUT_FILENO);
	set_g_code(e_status);
}

void	run_command(t_params *params, t_list *cmd_lst)
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
}
