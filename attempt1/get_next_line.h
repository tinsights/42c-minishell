/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 10:06:36 by achak             #+#    #+#             */
/*   Updated: 2023/12/02 17:36:27 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10
# endif

# include <unistd.h>
# include <stdlib.h>

typedef struct s_list
{
	char			*buffer;
	struct s_list	*next;
}	t_list;

char		*get_next_line(int fd);
void		create_buffer(t_list **list, int fd);
void		add_to_list(t_list **list, char *buffer);
char		*fetch_line(t_list *list);
void		clean_list(t_list **list);
int			new_line_check(t_list *list);
t_list		*find_last_node(t_list *list);
int			len_til_newline(t_list *list);
void		copy_next_line(t_list *list, char *next_line);
void		ft_free(t_list **list, t_list *clean_node, char *buffer, int c);

#endif
