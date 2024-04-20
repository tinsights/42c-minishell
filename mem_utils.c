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

void safe_free(void **ptr)
{
	if (*ptr)
		free (*ptr);
	*ptr = NULL;
}

void free_str(char **str)
{
	if (*str)
		free (*str);
	*str = NULL;
}


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
