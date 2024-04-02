/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_nbr_of_tokens.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/24 13:40:40 by achak             #+#    #+#             */
/*   Updated: 2024/03/30 19:30:04 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	iterate_thru_quotes(char **temp, char quote, int *count, int *flag)
{
	(*temp)++;
	while (**temp != quote && **temp)
		(*temp)++;
	if (*temp + 1)
		(*temp)++;
	if (!*flag)
	{
		(*count)++;
		*flag = 1;
	}
}

void	check_if_valid_var(char **temp, t_env *head_env, int *count, int *flag)
{
	int	i;

	i = 0;
	(*temp)++;
	while (!is_whitespace((*temp)[i]) && (*temp)[i] != '|'
		&& (*temp)[i] != '<' && (*temp)[i] != '>' && (*temp)[i]
		&& (*temp)[i] != 39 && (*temp)[i] != '"' && (*temp)[i] != '$')
		if ((*temp)[i++] == '?')
			break ;
	while (head_env)
	{
		if (!my_strncmp(*temp, head_env->key, i))
			break ;
		head_env = head_env->next;
	}
	*temp += i;
	if (!head_env)
		return ;
	if (!*flag)
	{
		(*count)++;
		*flag = 1;
	}
}

int	count_nbr_of_tokens(char *line_read, t_env *head_env)
{
	int		count;
	int		flag;
	char	*temp;

	count = 0;
	flag = 0;
	temp = line_read;
	while (*temp)
	{
		if (*temp == 39 || *temp == '"')
			iterate_thru_quotes(&temp, *temp, &count, &flag);
		else if (*temp == '$')
			check_if_valid_var(&temp, head_env, &count, &flag);
		else if (*temp == '|' || *temp == '<' || *temp == '>')
			iterate_thru_meta_char(&temp, &count, &flag);
		else if (!is_whitespace(*temp))
			iterate_thru_word(&temp, &count, &flag);
		else if (is_whitespace(*temp))
			reset_flag_for_new_word(&temp, &flag);
	}
	return (count);
}

/*
int	main(int ac, char **av, char **env)
{
	static char	*line_read;
	t_env	*head_env;
	int			flag;
	int			count;

	if (ac < 2 || !av[1])
		return (1);
	head_env = create_symbol_table(env);
//	t_env	*temp = head_env;
//	while (temp)
//	{
//		printf("key = %s\n", temp->key);
//		printf("value = %s\n", temp->value);
//		printf("diff = %d\n\n", temp->diff);
//		temp = temp->next;
//	}
	flag = 0;
	count = 0;
	while (1)
	{
		if (line_read)
		{
			free(line_read);
			line_read = NULL;
		}
		else if (flag)
			exit(EXIT_SUCCESS);
		line_read = readline("$> ");
		if (line_read && *line_read)
		{
			add_history(line_read);
//			char	**arr = parse_input_string(line_read, head_env);
//			int	i = -1;
//			while (arr[++i])
//				printf("arr[%d] = %s\n", i, arr[i]);
			t_params	params;
			params.token_arr = parse_input_string(line_read, head_env);
			params.head_env = head_env;
			int	pipe_nbr = nbr_of_pipes(&params);
			printf("nbr of pipes = %d\n", pipe_nbr);
			int	cmd_nbr = pipe_nbr + 1;

			params.cmd_nbr = cmd_nbr;
			//alloc_cmd_struct_array(cmd_nbr, arr, &head_env);
			alloc_cmd_struct_array(&params);
			i = -1;
			while (++i < cmd_nbr)
			{
				int	j = -1;
				while (params.cmd_arr[i].cmd_args[++j])
					printf("params.cmd_arr[%d]->cmd_args[%d] = %s\n",
						i, j, params.cmd_arr[i].cmd_args[j]);
				printf("params.cmd_arr[%d]->stdin_fds = %d\n", i,
						(*params.cmd_arr[i].stdin_fds));
				printf("params.cmd_arr[%d]->stdout_fds = %d\n\n", i,
						(*params.cmd_arr[i].stdout_fds));
			}
			if (locate_command_names(&params) == -1)
				continue ;
			if (preparing_fork_and_exec(&params) == -1)
				continue ;
		//	iterate_thru_single_quote(&line_read, &count);
		}
		flag = 1;
	}
}
*/
