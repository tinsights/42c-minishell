/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 12:30:44 by tjegades          #+#    #+#             */
/*   Updated: 2024/04/23 12:30:51 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

bool	is_builtin(char **argv)
{
	if (!argv || !argv[0])
		return (false);
	return (!ft_strncmp(argv[0], "export", 7) || !ft_strncmp(argv[0], "env", 4)
		|| !ft_strncmp(argv[0], "unset", 6) || !ft_strncmp(argv[0], "exit", 5)
		|| !ft_strncmp(argv[0], "echo", 5) || !ft_strncmp(argv[0], "cd", 3)
		|| !ft_strncmp(argv[0], "pwd", 4));
}

bool	handle_redir(t_cmd *cmd, t_redir redir)
{
	int	fd;

	fd = 0;
	if (redir.type == input)
	{
		fd = open(redir.file, O_RDONLY);
		if (fd > 0)
			ms_dup(fd, STDIN_FILENO);
	}
	else if (redir.type == out_append)
	{
		fd = open(redir.file, O_WRONLY | O_APPEND | O_CREAT, 0644);
		if (fd > 0)
			ms_dup(fd, STDOUT_FILENO);
	}
	else if (redir.type == out_trunc)
	{
		fd = open(redir.file, O_WRONLY | O_TRUNC | O_CREAT, 0644);
		if (fd > 0)
			ms_dup(fd, STDOUT_FILENO);
	}
	else if (redir.type == heredoc)
		ms_dup(cmd->heredoc_fd, STDIN_FILENO);
	return (fd >= 0);
}

bool	process_redirects(t_cmd *cmd)
{
	bool	redirect_success;
	int		redir_ctr;

	redirect_success = true;
	redir_ctr = -1;
	while (redirect_success && ++redir_ctr < cmd->num_redirects)
		redirect_success = handle_redir(cmd, cmd->redirs[redir_ctr]);
	if (!redirect_success)
	{
		perror(cmd->redirs[redir_ctr].file);
		g_code = 1;
	}
	return (redirect_success);
}

char	*check_valid_cmd(char **paths, char *cmd)
{
	int		j;
	char	*cmdpath;
	char	*binpath;

	if (!cmd || !cmd[0])
		return (NULL);
	if (!access(cmd, F_OK | X_OK))
		return (ft_strdup(cmd));
	cmdpath = ft_strjoin("/", cmd);
	j = 0;
	while (paths && paths[j])
	{
		binpath = ft_strjoin(paths[j], cmdpath);
		if (!access(binpath, F_OK | X_OK))
			break ;
		free(binpath);
		j++;
	}
	free(cmdpath);
	if (paths[j] == NULL)
		return (NULL);
	return (binpath);
}

char	**find_paths(void)
{
	char	*paths_var;
	char	**paths;
	int		i;

	paths_var = getenv("PATH");
	if (paths_var)
		paths = ft_split(paths_var, ':');
	else
		paths = (char **)(ft_calloc(1, sizeof(char *)));
	i = 0;
	while (paths[i])
		i++;
	paths = ft_realloc(paths, (i + 1) * sizeof(char *), (i + 2)
			* sizeof(char *));
	paths[i] = getcwd(NULL, 500);
	return (paths);
}
