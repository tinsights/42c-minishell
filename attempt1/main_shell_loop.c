/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_shell_loop.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/29 14:00:05 by achak             #+#    #+#             */
/*   Updated: 2024/03/30 12:48:30 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	main_shell_loop(t_params *params, char *line_read)
{
	int			pipe_nbr;

	add_history(line_read);
	params->token_arr = parse_input_string(line_read, *(params->head_env));
	params->cmd_arr = NULL;
	pipe_nbr = nbr_of_pipes(params);
	if (pipe_nbr == -1)
		return ;
	params->cmd_nbr = pipe_nbr + 1;
	if (alloc_cmd_struct_array(params) == -1)
		return ;
	if (locate_command_names(params) == -1)
		return ;
	preparing_fork_and_exec(params);
}

void	check_line_read_eof(char *line_read, int flag, t_params *params)
{
	if (line_read)
	{
		free(line_read);
		line_read = NULL;
	}
	else if (flag)
	{
		free_symbol_table(params->head_env);
		handle_exit_failure(NULL, params);
		exit(EXIT_SUCCESS);
	}
}

int	main(int ac, char **av, char **env)
{
	static char	*line_read;
	int			flag;
	char		*cwd;
	t_env		*head_env;
	t_params	params;

	flag = 0;
	head_env = create_symbol_table(ac, av, env);
	params.head_env = &head_env;
	while (1)
	{
		check_line_read_eof(line_read, flag, &params);
		cwd = strjoin_and_free_str(getcwd(NULL, 200), "$ ", 1);
		line_read = readline(cwd);
		if (line_read && *line_read)
			main_shell_loop(&params, line_read);
		free(cwd);
		flag = 1;
	}
}
