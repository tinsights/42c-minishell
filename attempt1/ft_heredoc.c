/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_heredoc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/30 09:50:41 by achak             #+#    #+#             */
/*   Updated: 2024/03/30 19:59:47 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

void	ft_heredoc(t_params *params, int i, char *token_arr)
{
	static int	nbr = 0;
	char	*here_doc;
	char	*str;
	int		fd1;
	int		fd2;

	here_doc = strjoin_and_free_str("here_doc", ft_itoa(nbr++), 2);
	here_doc = strjoin_and_free_str("/tmp/", here_doc, 2);
	fd1 = open(here_doc, O_WRONLY | O_CREAT | O_EXCL | O_TRUNC, 0600);
	if (fd1 == -1)
		perror("open heredoc");
	else
	{
		str = get_next_line(STDIN_FILENO);
		while (str)
		{
		// if (check_delimiter(params, token_arr, str, fd1))
			if (my_strcmp(token_arr, str))
			{
				free(str);
				fd2 = open(here_doc, O_RDONLY);
				close(fd1);
				*(params->cmd_arr[i].stdin_fds) = fd2;
				params->cmd_arr[i].here_doc = here_doc;
				return ;
			}
			write(fd1, str, my_strlen(str));
			free(str);
			str = get_next_line(STDIN_FILENO);
		}
		close(fd1);
	}
}
			// EXPAND ENV VARS IN HEREDOC IF DELIMITER IS UNQUOTED
//			str = expand_vars(str);
