/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 12:22:37 by tjegades          #+#    #+#             */
/*   Updated: 2024/04/23 12:22:38 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	handle_sigint(int sig)
{
	ft_putstr_fd("^C\n", STDIN_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
	g_code = 130;
}

void	set_g_code(int code)
{
	char	*result;
	char	*key;

	g_code = code;
	result = ft_itoa(g_code);
	key = ft_strjoin("?=", result);
	set_env(key);
	free_str(&key);
	free_str(&result);
}

void	init_io(t_params *params)
{
	rl_outstream = stderr;
	params->default_io[0] = dup(STDIN_FILENO);
	params->default_io[1] = dup(STDOUT_FILENO);
	params->default_io[2] = dup(STDERR_FILENO);
	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
	rl_catch_signals = 0;
	readline(NULL);
	dup2(params->default_io[0], STDIN_FILENO);
	dup2(params->default_io[1], STDOUT_FILENO);
	dup2(params->default_io[2], STDERR_FILENO);
}

void	set_global_envs(void)
{
	int		lvl;
	char	*result;
	char	*key;

	if (!getenv("?"))
		set_g_code(0);
	else
		set_g_code(ft_atoi(getenv("?")));
	if (getenv("SHLVL"))
	{
		lvl = ft_atoi(getenv("SHLVL"));
		result = ft_itoa(lvl + 1);
		key = ft_strjoin("SHLVL=", result);
		set_env(key);
		free_str(&key);
		free_str(&result);
	}
	else
		set_env("SHLVL=1");
	set_env("SHELL=minishell");
}

void	init_env(t_params *params, char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	params->env_count = i;
	params->envs = ft_calloc((i + 2), sizeof(char *));
	i = -1;
	while (envp[++i])
		params->envs[i] = ft_strdup(envp[i]);
	__environ = params->envs;
	set_global_envs();
}
