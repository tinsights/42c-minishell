/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredocs.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 12:24:35 by tjegades          #+#    #+#             */
/*   Updated: 2024/04/23 12:24:35 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	heredoc_sigint(int sig)
{
	ft_putstr_fd("\n", STDERR_FILENO);
	g_code = 130;
}

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
