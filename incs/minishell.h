/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <tjegades@student.42singapore.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 11:17:57 by tjegades          #+#    #+#             */
/*   Updated: 2025/02/19 17:25:19 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <signal.h>
# include <stdbool.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>


# ifdef __APPLE__
# include <crt_externs.h>
# define __environ (*_NSGetEnviron())
# define rl_clear_history() (clear_history());
# define rl_catch_signals()
# else
	extern char **__environ;
# endif

#define UNUSED(x) (void)(x)

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

bool						valid_env_char(char c);
bool						valid_env_start(char *line);
bool						is_space(char c);
int							is_redirect(char *line);
bool						is_meta(char *line);
void						*ft_realloc(void *ptr, size_t old_sz, size_t sz);
void						free_str(char **str);
bool						word_copy(char **ptr, char qstart, char *word,
								int flag);
int							len_to_alloc(char **line_ptr, char qstart,
								int hd_flag);
void						ms_exit(t_params *params, int code, bool print);
void						set_env(char *var);
void						free_cmds(void *ptr);
void						set_g_code(int code);
void						ms_dup(int newfd, int oldfd);
void						run_command(t_params *params, t_list *cmd_lst);

extern int					g_code;

#endif