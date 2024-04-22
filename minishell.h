/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:17:57 by tjegades          #+#    #+#             */
/*   Updated: 2024/04/20 11:17:58 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

typedef enum e_redir_type
{
	input,
	out_append,
	out_trunc,
	heredoc
}							t_redir_type;

typedef struct s_redir
{
	char					*file;
	t_redir_type			type;
	bool					quoted;
}							t_redir;

typedef struct s_proc
{
	int						pid;
	int						exit_status;
	bool					exited;
}							t_proc;

typedef struct s_cmd
{
	t_proc					proc;
	int						num_words;
	int						num_redirects;
	int						num_heredocs;
	int						heredoc_fd;
	char					*line;
	char					**words;
	t_redir					*redirs;
}							t_cmd;

typedef struct sigaction	t_sig;

typedef struct s_params
{
	int						num_cmds;
	int						total_heredocs;
	char					*line;
	char					**paths;
	char					**envs;
	char					**orig_envp;
	bool					interactive;

	int						env_count;
	int						default_io[3];

	t_sig					sa;

	t_list					*cmd_list;
}							t_params;

void						*ft_realloc(void *ptr, size_t old_size,
								size_t size);

#endif