/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <tjegades@student.42singapore.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 12:24:35 by tjegades          #+#    #+#             */
/*   Updated: 2025/02/19 17:37:33 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_sigint(int sigint)
{
	(void) sigint;
	g_code = 130;
}

void	finalize_heredoc(t_params *p, t_cmd *cmd, char **line, int hd_pipe[2])
{
	if (!*line)
		write(p->default_io[1], "\n", 1);
	free_str(line);
	get_next_line(-1);
	if (cmd->heredoc_fd > 0)
		close(cmd->heredoc_fd);
	cmd->heredoc_fd = dup(hd_pipe[0]);
	close(hd_pipe[0]);
	close(hd_pipe[1]);
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
		len = len_to_alloc(&line_ref, 0, 2);
		word = ft_calloc(len + 1, sizeof(char));
		word_copy(&copy, 0, word, 2);
		write(heredoc_pipe[1], word, ft_strlen(word));
		free_str(&word);
	}
}

void	run_heredoc(t_params *params, t_redir redir, t_cmd *cmd)
{
	int		heredoc_pipe[2];
	char	*line;
	char	*delim;

	if (redir.type == heredoc)
	{
		pipe(heredoc_pipe);
		delim = ft_strjoin(redir.file, "\n");
		ft_putstr_fd("heredoc> ", 2);
		line = get_next_line(params->default_io[0]);
		while (line)
		{
			if (!ft_strncmp(line, delim, ft_strlen(delim)))
				break ;
			if (redir.quoted)
				write(heredoc_pipe[1], line, ft_strlen(line));
			else
				expand_heredoc_line(line, heredoc_pipe);
			free_str(&line);
			ft_putstr_fd("heredoc> ", 2);
			line = get_next_line(params->default_io[0]);
		}
		free_str(&delim);
		finalize_heredoc(params, cmd, &line, heredoc_pipe);
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
