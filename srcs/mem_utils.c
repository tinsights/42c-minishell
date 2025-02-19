/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mem_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:21:50 by tjegades          #+#    #+#             */
/*   Updated: 2024/04/20 11:21:52 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_realloc(void *ptr, size_t old_size, size_t size)
{
	void	*res;
	size_t	maxcpy;

	if (!size)
	{
		free(ptr);
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

void	free_str(char **str)
{
	if (*str)
		free(*str);
	*str = NULL;
}

void	free_cmds(void *ptr)
{
	t_cmd	*cmd;
	char	**words;
	t_redir	*redirects;
	int		i;

	cmd = (t_cmd *)ptr;
	words = cmd->words;
	redirects = cmd->redirs;
	i = -1;
	while (++i < cmd->num_words)
		free_str(words + i);
	i = -1;
	while (++i < cmd->num_redirects)
		free_str(&(redirects[i].file));
	if (cmd->heredoc_fd > 0)
		close(cmd->heredoc_fd);
	ft_free((void **)&words);
	ft_free((void **)&redirects);
	ft_free((void **)&(cmd->line));
	ft_free((void **)&(cmd));
}

void	close_fds(t_params *params)
{
	t_list	*master_lst;
	t_cmd	*cmd_ptr;

	master_lst = params->cmd_list;
	while (master_lst)
	{
		cmd_ptr = master_lst->content;
		if (cmd_ptr->heredoc_fd > 0)
			close (cmd_ptr->heredoc_fd);
		master_lst = master_lst->next;
	}
	close(params->default_io[0]);
	close(params->default_io[1]);
	close(params->default_io[2]);
}
