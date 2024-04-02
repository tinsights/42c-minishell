/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 09:50:41 by achak             #+#    #+#             */
/*   Updated: 2024/03/31 19:50:29 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	heredoc_til_delimiter(t_command *cmd_struct, int fd1, char *token_arr,
		char *here_doc)
{
	char	*str;
	int		fd2;

	str = get_next_line(STDIN_FILENO);
	while (str)
	{
		if (my_strcmp(token_arr, str))
		{
			free(str);
			fd2 = open(here_doc, O_RDONLY);
			close(fd1);
			*(cmd_struct->stdin_fds) = fd2;
			cmd_struct->here_doc = here_doc;
			return ;
		}
		write(fd1, str, my_strlen(str));
		free(str);
		str = get_next_line(STDIN_FILENO);
	}
	free(here_doc);
	close(fd1);
}

void	ft_heredoc(t_params *params, int i, char *token_arr)
{
	static int	nbr = 0;
	char		*here_doc;
	int			fd1;

	here_doc = strjoin_and_free_str("here_doc", ft_itoa(nbr++), 2);
	here_doc = strjoin_and_free_str("/tmp/", here_doc, 2);
	fd1 = open(here_doc, O_WRONLY | O_CREAT | O_TRUNC, 0600);
	if (fd1 == -1)
		perror("open heredoc");
	else
		heredoc_til_delimiter(&params->cmd_arr[i], fd1, token_arr,
			here_doc);
}
