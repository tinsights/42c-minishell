/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 19:25:15 by achak             #+#    #+#             */
/*   Updated: 2024/03/25 18:30:02 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <unistd.h>
# include <errno.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>
# include <termios.h>
# include <curses.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef union nodetype
{
	char	*cmd_word;
	char	*metachar;
}	t_nodetype;

typedef struct s_token
{
	t_nodetype	type;
	int		len;
}	t_token;

typedef struct s_input
{
	char	*buf;
	int		buffer_size;
	int		cursor;
}	t_input;

typedef struct s_env
{
	char		*key;
	char		*value;
	int			diff;
	struct s_env	*next;
}	t_env;

typedef struct s_params
{
	char	**cmd_paths;
	char	***cmd_args;
	int		*pipefds[2];
	int		*stdin_fds;
	int		*stdout_fds;
	bool	fork;
	t_env	*head_env;
}	t_params;

//void	copy_var_to_newstr(char *line_read, char *new_str, t_env *head_env,
//		int *i);
//void	copy_old_to_newstr(char *line_read, char *new_str, t_env *head_env);
//char	*transform_original_command(char *line_read, t_env *head_env);

//int		count_var_len(char *line_read, t_env *head_env, int *i);
//int		count_redir_len(char *line_read, int *i);
//int		count_regular_chars(char *line_read, int *i, int *flag);
//int		compare_var_diff(char *line_read, t_env *head_env);
//int		count_var_diff(char *line_read, t_env *head_env);
//int		count_without_extra_whitespace(char *line_read);
//int		count_new_strlen(char *line_read, t_env *head_env);

void	iterate_thru_quotes(char **temp, char quote, int *count, int *flag);
void	check_if_valid_var(char **temp, t_env *head_env, int *count, int *flag);
int		count_nbr_of_tokens(char *line_read, t_env *head_env);

void	iterate_thru_meta_char(char **temp, int *count, int *flag);
void	iterate_thru_word(char **temp, int *count, int *flag);
void	reset_flag_for_new_word(char **line_read, int *flag);

void	remove_table_entry(t_env *head, char *key);
char	*get_key_from_var(char *var);
char	*get_value_from_var(char *var);
void	create_new_entry(char *var, t_env **head);
t_env	*create_symbol_table(char **env);

void	free_symbol_table(t_env **head);

int		my_strlen(char *s);
int		my_strncmp(char *s1, char *s2, size_t n);
char	*strjoin_and_frees1(char *s1, char *s2);
int		is_whitespace(char c);

#endif
