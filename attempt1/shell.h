/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/20 19:25:15 by achak             #+#    #+#             */
/*   Updated: 2024/03/30 19:59:55 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

# include <unistd.h>
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>
# include <termios.h>
# include <curses.h>
# include <signal.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "get_next_line.h"
/*
typedef struct s_redirfds
{
	int			stdin_fd;
	int			stdout_fd;
	int			here_doc;
	int			cmd_nbr;
	struct s_redirfds	*next;
}	t_redirfds;
*/
typedef struct s_env
{
	char			*key;
	char			*value;
	int				diff;
	struct s_env	*next;
}	t_env;
/*
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
*/
typedef struct s_command
{
	char	*here_doc;
	char	*cmd_path;
	char	**cmd_args;
	int		*stdin_fds;
	int		*stdout_fds;
}	t_command;

typedef struct s_params
{
	t_env		**head_env;
	t_command	*cmd_arr;
	int			cmd_nbr;
	char		**token_arr;
}	t_params;

//void	copy_var_to_newstr(char *line_read, char *new_str, t_env *head_env,
//		int *i);
//void	copy_old_to_newstr(char *line_read, char *new_str, t_env *head_env);
//char	*transform_original_command(cha, int *flagr *line_read,
//t_env *head_env);
//int		count_var_len(char *line_read, t_env *head_env, int *i);
//int		count_redir_len(char *line_read, int *i);
//int		count_regular_chars(char *line_read, int *i, int *flag);
//int		compare_var_diff(char *line_read, t_env *head_env);
//int		count_var_diff(char *line_read, t_env *head_env);
//int		count_without_extra_whitespace(char *line_read);
//int		count_new_strlen(char *line_read, t_env *head_env);

void	main_shell_loop(t_params *params, char *line_read);
void	check_read_line_eof(char *line_read, int flag, t_params *params);

void	settle_child_process_stdin2(t_params *params, int i);
void	settle_child_process_stdin(t_params *params, int i, int (*old_fds)[2]);
void	settle_child_process_stdout(t_params *params, int i, int *new_fds);
void	child_process(t_params *params, int i, int *new_fds, int (*old_fds)[2]);

int		check_for_char_in_str(char *str, char c);
int		check_and_assign_cmd_path3(t_params *params, char *cmd);
int		check_and_assign_cmd_path2(t_params *params, char *cmd, int i,
			char **arr_path);
int		check_and_assign_cmd_path(t_params *params, char *cmd, int i);
int		locate_command_names(t_params *params);

void	wrapper(int func_rv, const char *func_name);
void	preserve_stdin_and_stdout(t_params *params, int i, int *dup_stdin,
			int *dup_stdout);
void	restore_stdin_and_stdout(int dup_stdin, int dup_stdout);
void	choose_which_builtin_to_exec(t_params *params, int i, int *exit_status);
int		execute_builtin(t_params *params, int i, int flag);

int		check_if_cmd_is_builtin(char *cmd);
int		check_if_fork_needed(t_params *params);
void	parent_process(t_params *params, int i, int *new_fds,
			int (*old_fds)[2]);
void	preparing_fork_and_exec(t_params *params);

int		check_next_token_pipe(char **token_arr, int i);
int		check_next_token_null(char **token_arr, int i);
int		check_next_token_file(char **token_arr, int i);
int		check_token_pipe(char *token_arr);
int		check_token_redir(char *token_arr);

int		check_for_redirect(char *token_arr);
void	determine_mode_to_open_file(t_params *params, int i, char *token_arr,
			int rv);
int		open_files_for_redirect(char **token_arr, int i, t_params *params);

int		check_syntax_around_pipe(t_params *params, int i, int *nbr);
int		check_syntax_around_redir(t_params *params, int i);
int		nbr_of_pipes(t_params *params);
int		check_operator_token(char **token_arr, int i, t_params *params,
			int *count);
int		count_words_nbr_in_cmd(char **token_arr, t_params *params);

void	ft_heredoc(t_params *params, int i, char *token_arr);

int		check_token_valid(char **token_arr, int *flag);
void	assigning_tokens_to_cmd_args(char **cmd_args, char ***token_arr,
			int cmd_words, int *flag);
int		alloc_cmd_struct_node(int i, char ***token_arr, t_params *params);
int		alloc_cmd_struct_array(t_params *params);

char	*find_env_var_value(t_env *head_env, char *var);
int		check_valid_dir(t_env **head_env, char *dir_arr, char *new_path,
			char *path);
int		check_present_dir(t_env **head_env, char *path);
int		search_dir_arr(t_env **head_env, char **dir_arr, char *path);

int		free_cd_strings(char *oldpwd, char *pwd, char *new_path, int flag);
int		cd_wrapper(t_env **head_env, char *new_path, char *path);
int		different_cd_criteria(t_env **head_env, char **cmd_args, char *cd_path,
			char **dir_arr);
int		cd_builtin(t_env **head_env, char **cmd_args);

void	exit_builtin(t_params *params, char **cmd_args);
int		echo_builtin(char **cmd_args);
int		pwd_builtin(void);
int		env_builtin(t_env *head_env);
int		unset_builtin(t_env **head_env, char **cmd_args);

int		check_if_entry_exists(char *var, t_env *head_env);
void	update_existing_entry(char *var, t_env **head_env);
int		check_valid_var_name(char *var);
int		export_var_into_env_list(t_env **head_env, char **cmd_args);
int		export_builtin(t_env **head_env, char **cmd_args);

void	copy_meta_char(char **temp, char *token_arr, int *i);
void	copy_quote_len(char **temp, int *i, char *token_arr, t_env *head_env);
int		move_ptr_past_var(char **temp, t_env *head_env);

void	copy_var_len(char **temp, char *token_arr, int *i, t_env *head_env);
void	copy_regular_len(char **temp, char *token_arr, int *i);
void	copy_token_from_line(char **temp, char *token_arr, t_env *head_env);

void	count_var_len(char **temp, int *token_len, t_env *head_env, int i);
void	count_regular_len(char **temp, int *token_len);

void	count_meta_char(char **temp_line, int *token_len);
void	count_quote_len(char **temp_line, char quote, int *token_len,
			t_env *head_env);
int		count_token_len(char *temp, t_env *head_env);
void	alloc_token_by_index(char **temp_line, char **token_arr, int i,
			t_env *head_env);
char	**parse_input_string(char *line_read, t_env *head_env);

void	iterate_thru_quotes(char **temp, char quote, int *count, int *flag);
void	check_if_valid_var(char **temp, t_env *head_env, int *count, int *flag);
int		count_nbr_of_tokens(char *line_read, t_env *head_env);

void	iterate_thru_meta_char(char **temp, int *count, int *flag);
void	iterate_thru_word(char **temp, int *count, int *flag);
void	reset_flag_for_new_word(char **line_read, int *flag);

void	remove_table_entry(t_env **head, char *key);
char	*get_key_from_var(char *var);
char	*get_value_from_var(char *var);
void	create_new_entry(char *var, t_env **head);
t_env	*create_symbol_table(int ac, char **av, char **env);

void	ft_memset(void *s, int c, size_t n);
int		my_strcmp(char *delimiter, char *str);
int		is_alphabet(char c);
int		is_numeric(char c);

void	free_symbol_table(t_env **head);
void	free_array(char **arr);
void	free_cmd_arr(t_command *cmd_arr, int cmd_nbr);
void	handle_exit_failure(char **token_arr, t_params *params);

int		my_strlen(char *s);
int		my_strncmp(char *s1, char *s2, size_t n);
void	free_str_nbr(char *s1, char *s2, int str_nbr);
char	*strjoin_and_free_str(char *s1, char *s2, int str_nbr);
int		is_whitespace(char c);

char	**ft_split(char *str, char sep);

char	*ft_itoa(int n);

int		ft_atoi(char *nptr);

int		ft_dprintf(int fd, const char *format, ...);
int		print_char(int c, int fd);
int		print_str(char *str, int fd);
int		print_digit(long n, int base, char specifier, int fd);
int		print_format(char sp, va_list ap, int fd);

int		ft_ptrlen(unsigned long int ptr);
void	ft_putptr(unsigned long int ptr, int fd);
int		print_ptr(unsigned long int ptr, int fd);

#endif
