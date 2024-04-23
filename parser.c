/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:29:44 by tjegades          #+#    #+#             */
/*   Updated: 2024/04/20 11:29:45 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_redir_type	get_redir_type(char *line);

void	parse_redirect(t_cmd *cmd, char **line_ptr)
{
	t_redir	*redirs;
	t_redir	redir;
	char	*copy;
	int		len;

	redirs = cmd->redirs;
	redir.type = get_redir_type((*line_ptr));
	if (redir.type == heredoc)
		cmd->num_heredocs++;
	(*line_ptr) += is_redirect((*line_ptr));
	while (is_space(*(*line_ptr)))
		(*line_ptr)++;
	copy = (*line_ptr);
	len = len_to_alloc(line_ptr, 0, redir.type != heredoc);
	redirs = ft_realloc(redirs, cmd->num_redirects * sizeof(t_redir),
			(cmd->num_redirects + 1) * sizeof(t_redir));
	redir.file = ft_calloc(len + 1, sizeof(char));
	redir.quoted = word_copy(&copy, 0, redir.file, redir.type != heredoc);
	redirs[cmd->num_redirects] = redir;
	cmd->num_redirects++;
	cmd->redirs = redirs;
}

void	parse_words(t_cmd *cmd, char **line_ptr)
{
	char	*copy;
	int		len;
	char	*word;

	copy = *line_ptr;
	len = len_to_alloc(line_ptr, 0, 1);
	if (!cmd->words)
		cmd->words = ft_calloc(1, sizeof(char *));
	if (len)
	{
		word = ft_calloc(len + 1, sizeof(char));
		cmd->words[cmd->num_words] = word;
		word_copy(&copy, 0, cmd->words[cmd->num_words], 1);
		cmd->num_words++;
		cmd->words = ft_realloc(cmd->words, cmd->num_words * sizeof(char *),
				(cmd->num_words + 1) * sizeof(char *));
	}
}

void	parse_cmd(t_list *cmd_lst)
{
	t_cmd	*cmd;
	char	*line;

	cmd = (t_cmd *)cmd_lst->content;
	line = cmd->line;
	while (*line)
	{
		if (*line && is_space(*line))
		{
			line++;
			continue ;
		}
		if (is_redirect(line))
			parse_redirect(cmd, &line);
		else
			parse_words(cmd, &line);
	}
}

void	add_cmd(t_params *params, int *total_len, int len)
{
	t_cmd	*cmd;
	t_list	*node;

	cmd = ft_calloc(1, sizeof(t_cmd));
	node = ft_calloc(1, sizeof(t_list));
	cmd->line = ft_substr(params->line, *total_len, len);
	node->content = cmd;
	parse_cmd(node);
	params->total_heredocs += cmd->num_heredocs;
	ft_lstadd_back(&params->cmd_list, node);
	*total_len += len + 1;
}

void	create_cmds(t_params *params)
{
	int		total_len;
	char	*line;
	char	*start;

	if (!params)
		return ;
	line = params->line;
	if (!line)
		return ;
	params->total_heredocs = 0;
	total_len = 0;
	start = line;
	while (*line)
	{
		if (*line == '\'')
			line = ft_strchr(line + 1, '\'');
		else if (*line == '"')
			line = ft_strchr(line + 1, '"');
		else if (*line == '|')
			add_cmd(params, &total_len, line - (start + total_len));
		line++;
		if (!line[0])
			add_cmd(params, &total_len, line - (start + total_len));
	}
}
