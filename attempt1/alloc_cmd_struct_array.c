/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   alloc_cmd_struct_array.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/26 14:19:24 by achak             #+#    #+#             */
/*   Updated: 2024/03/30 19:48:58 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	check_token_valid(char **token_arr, int *flag)
{
	if (*token_arr)
	{
		if (my_strlen(*token_arr) >= 2)
		{
			if ((*token_arr)[0] == 39 && (((*token_arr)[1] == '<')
				|| ((*token_arr)[1] == '>')))
			{
				*flag = 1;
				return (0);
			}
			else if ((*token_arr)[0] != 39 && !(*flag))
				return (1);
			else if ((*token_arr)[0] != 39 && *flag)
			{
				*flag = 0;
				return (0);
			}
			else
				return (0);
		}
		else
			return (1);
	}
	else
		return (0);
}

void	assigning_tokens_to_cmd_args(char **cmd_args, char ***token_arr,
		int cmd_words, int *flag)
{
	int	j;

	j = 0;
	while (**token_arr && !check_token_valid(*token_arr, flag))
		(*token_arr)++;
	while (**token_arr && !check_token_pipe(**token_arr))
	{
		if (check_token_valid(*token_arr, flag))
		{
			while (j < cmd_words && check_token_valid(*token_arr, flag))
			{
				cmd_args[j++] = **token_arr;
				(*token_arr)++;
			}
		}
		else if (!check_token_pipe(**token_arr))
		{
			if (check_token_redir(**token_arr))
				*flag = 1;
			else
				*flag = 0;
			(*token_arr)++;
		}
	}
}

int	alloc_cmd_struct_node(int i, char ***token_arr, t_params *params)
{
	int		cmd_words;
	int		flag;
	char	**cmd_args;

	cmd_words = count_words_nbr_in_cmd(*token_arr, params);
	if (cmd_words == -1)
		return (-1);
	flag = 0;
	params->cmd_arr[i].stdin_fds = (int *)malloc(sizeof(int));
	params->cmd_arr[i].stdout_fds = (int *)malloc(sizeof(int));
	cmd_args = (char **)malloc(sizeof(char *) * (cmd_words + 1));
	if (!cmd_args || !params->cmd_arr[i].stdin_fds
		|| !params->cmd_arr[i].stdout_fds)
		return (-1);
	cmd_args[cmd_words] = NULL;
	*(params->cmd_arr[i].stdin_fds) = -2;
	*(params->cmd_arr[i].stdout_fds) = -2;
	if (open_files_for_redirect(*token_arr, i, params) == -1)
		return (-1);
	assigning_tokens_to_cmd_args(cmd_args, token_arr, cmd_words, &flag);
	params->cmd_arr[i].cmd_args = cmd_args;
	return (0);
}

int	alloc_cmd_struct_array(t_params *params)
{
	t_command	*cmd_arr;
	int			i;
	char		**temp;

	i = -1;
	temp = params->token_arr;
	cmd_arr = (t_command *)malloc(sizeof(t_command) * (params->cmd_nbr + 1));
	if (cmd_arr)
	{
		ft_memset(cmd_arr, 0, (sizeof(t_command) * params->cmd_nbr));
		while (++i < params->cmd_nbr)
		{
			cmd_arr[i].here_doc = NULL;
			cmd_arr[i].cmd_path = NULL;
			cmd_arr[i].cmd_args = NULL;
		}
		params->cmd_arr = cmd_arr;
		i = -1;
		while (++i < params->cmd_nbr)
			if (alloc_cmd_struct_node(i, &temp, params) == -1)
				return (-1);
	}
	return (0);
}

/*
char	***prepare_cmds_for_exec(char **token_arr)
{
	int		cmds_nbr;
	int		i;
	int		tkn_nbr;
	char	***cmd_arr;

	cmds_nbr = nbr_of_pipes(token_arr) + 1;
	i = -1;
	cmd_arr = (char ***)malloc(sizeof(char **) * (cmds_nbr + 1));
	if (cmd_arr)
	{
		cmd_arr[cmds_nbr] = NULL;
		while (++i < cmds_nbr)
		{
			tkn_nbr = count_tokens_till_pipe(token_arr);
			cmd_arr[i] = (char **)malloc(sizeof(char *) *
				(tkn_nbr + 1));
			if (cmd_arr[i])
			{
				cmd_arr[i][tkn_nbr] = NULL;
				j = -1;
				while (++j < tkn_nbr)
				{
					cmd_arr[i][j] = token_arr[k];
	}
	return (cmd_arr);
}
*/
