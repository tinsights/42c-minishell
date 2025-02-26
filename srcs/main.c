/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <tjegades@student.42singapore.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 10:15:00 by tjegades          #+#    #+#             */
/*   Updated: 2025/02/20 08:56:19 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int		count_cmds(char *line);
void	create_cmds(t_params *params);

void	handle_sigint(int sig);
void	init_io(t_params *params);
void	init_env(t_params *params, char **envp);
bool	is_builtin(char **argv);

void	process_heredocs(t_params *params, t_list *cmd_lst);

int		g_code;

bool	child_interrupted(t_list *cmd_lst)
{
	t_cmd	*cmd;
	int		status;

	while (cmd_lst)
	{
		cmd = cmd_lst->content;
		status = cmd->proc.exit_status;
		if (!is_builtin(cmd->words))
			if (WIFSIGNALED(status) && (WTERMSIG(status) == SIGINT
					|| WTERMSIG(status) == SIGQUIT))
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
		run_command(params, params->cmd_list);
		if (child_interrupted(params->cmd_list))
			write(1, "\n", 1);
	}
	set_g_code(g_code);
	ft_lstclear(&params->cmd_list, free_cmds);
	return (true);
}

int	main(int _ac, char **_av, char **envp)
{
	t_params	params;

	UNUSED(_ac);
	UNUSED(_av);

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
		params.line = readline("minishell 🥥$> ");
		set_g_code(g_code);
		if (params.line && *params.line)
			if (!run_line(&params))
				continue ;
		if (!params.line)
			ms_exit(&params, 0, true);
	}
}

void	ms_dup(int oldfd, int newfd)
{
	dup2(oldfd, newfd);
	close(oldfd);
}

void	ms_exit(t_params *params, int code, bool print)
{
	int	i;

	if (params->paths)
	{
		i = -1;
		while (params->paths[++i])
			free(params->paths[i]);
		free(params->paths);
	}
	ft_lstclear(&(params->cmd_list), free_cmds);
	i = -1;
	while (__environ[++i])
		free_str(__environ + i);
	free(__environ);
	free_str(&(params->line));
	if (print && isatty(STDIN_FILENO) && isatty(STDOUT_FILENO))
		ft_putstr_fd("Goodbye~\n", 2);
	rl_clear_history();
	close(params->default_io[0]);
	close(params->default_io[1]);
	close(params->default_io[2]);
	exit(code);
}
