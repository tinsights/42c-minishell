/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/31 10:08:20 by achak             #+#    #+#             */
/*   Updated: 2024/02/16 14:17:56 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

void	clean_list(t_list **list)
{
	t_list	*clean_node;
	t_list	*last_node;
	char	*buffer;
	int		i;
	int		j;

	clean_node = malloc(sizeof(t_list));
	buffer = malloc(BUFFER_SIZE + 1);
	if (clean_node == NULL || buffer == NULL)
		return ;
	last_node = find_last_node(*list);
	i = 0;
	j = 0;
	while (last_node->buffer[i] && last_node->buffer[i] != '\n')
		i++;
	while (last_node->buffer[i] && last_node->buffer[++i])
		buffer[j++] = last_node->buffer[i];
	buffer[j] = '\0';
	clean_node->buffer = buffer;
	clean_node->next = NULL;
	ft_free(list, clean_node, buffer, 0);
}

char	*fetch_line(t_list *list)
{
	int		length;
	char	*next_line;

	length = len_til_newline(list);
	if (length < 1)
		return (NULL);
	next_line = malloc(length + 1);
	if (next_line == NULL)
		return (NULL);
	next_line[length] = '\0';
	copy_next_line(list, next_line);
	return (next_line);
}

void	add_to_list(t_list **list, char *buffer)
{
	t_list	*new_node;
	t_list	*last_node;

	new_node = malloc(sizeof(t_list));
	last_node = find_last_node(*list);
	if (new_node == NULL)
		return ;
	if (last_node == NULL)
		*list = new_node;
	else
		last_node->next = new_node;
	new_node->buffer = buffer;
	new_node->next = NULL;
}

void	create_buffer(t_list **list, int fd)
{
	int		chars_read;
	char	*buffer;

	while (new_line_check(*list) < 1)
	{
		buffer = malloc(BUFFER_SIZE + 1);
		if (buffer == NULL)
			return ;
		chars_read = read(fd, buffer, BUFFER_SIZE);
		if (chars_read < 1)
		{
			free(buffer);
			return ;
		}
		buffer[chars_read] = '\0';
		add_to_list(list, buffer);
	}
}

/*
			if (chars_read == -1)
				ft_free(list, NULL, NULL, -1);
			*list = NULL;
*/

char	*get_next_line(int fd)
{
	static t_list	*list = NULL;
	char			*next_line;

	next_line = NULL;
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	create_buffer(&list, fd);
	if (list == NULL)
		return (NULL);
	next_line = fetch_line(list);
	clean_list(&list);
	return (next_line);
}

/*

#include <stdio.h>
#include <fcntl.h>

int	main(void)
{
*	int		fd;
//	char	*line;
	char	*pathname;

//	pathname = "read_error.txt";
//	pathname = "qq.txt";
//	pathname = "L.txt";
//	pathname = "drawing.txt";
	
	pathname = "abct";
	fd = open(pathname, O_RDONLY);
	printf("testing for %s\n", pathname);
//	fd = open("L.txt", O_RDONLY);
	while ((line = get_next_line(fd)))
	{
		printf("%s", line);
//		free(line);
	}

	pathname = "blank.txt";
	printf("\ntesting for %s\n", pathname);
	fd = open(pathname, O_RDONLY);
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
	
	printf("%s", get_next_line(fd));
	//printf("%s", get_next_line(-1));

	pathname = "drawing.txt";
	printf("\n\ntesting for %s\n", pathname);
//	fd = open(pathname, O_CREAT, 700 | 070 | 007);
	fd = open(pathname, O_RDONLY);
	printf("%s\n", get_next_line(fd));
	printf("===================%s", get_next_line(fd));
//	printf("\n\ntesting for fd = -1\n");
//	printf("%s", get_next_line(-1));
	close(fd);
	fd = open(pathname, O_RDONLY);
	printf("\n\nclose fd and open again\n");
	printf("%s\n", get_next_line(fd));
	printf("%s", get_next_line(fd));

cat > read_error.txt
aaaaaaaaaa
bbbbbbbbbb
cccccccccc
dddddddddd

cat > drawing.txt
░░░░░░░░░░░░░░░░░░░░░░█████████░░░░░░░░░
░░███████░░░░░░░░░░███▒▒▒▒▒▒▒▒███░░░░░░░
░░█▒▒▒▒▒▒█░░░░░░░███▒▒▒▒▒▒▒▒▒▒▒▒▒███░░░░
░░░█▒▒▒▒▒▒█░░░░██▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██░░
░░░░█▒▒▒▒▒█░░░██▒▒▒▒▒██▒▒▒▒▒▒██▒▒▒▒▒███░
░░░░░█▒▒▒█░░░█▒▒▒▒▒▒████▒▒▒▒████▒▒▒▒▒▒██
░░░█████████████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██
░░░█▒▒▒▒▒▒▒▒▒▒▒▒█▒▒▒▒▒▒▒▒▒█▒▒▒▒▒▒▒▒▒▒▒██
░██▒▒▒▒▒▒▒▒▒▒▒▒▒█▒▒▒██▒▒▒▒▒▒▒▒▒▒██▒▒▒▒██
██▒▒▒███████████▒▒▒▒▒██▒▒▒▒▒▒▒▒██▒▒▒▒▒██
█▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒█▒▒▒▒▒▒████████▒▒▒▒▒▒▒██
██▒▒▒▒▒▒▒▒▒▒▒▒▒▒█▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██░
░█▒▒▒███████████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒██░░░
░██▒▒▒▒▒▒▒▒▒▒████▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒▒█░░░░░
░░████████████░░░█████████████████░░░░░░

//	char	*next_line;
	char	*pathname;
	int		fd;

	pathname = "read_error.txt";
	fd = open(pathname, O_RDONLY);
//	printf("fd = %d\n", fd);
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(-1));
//	close(fd);
//	fd = open(pathname, O_RDONLY);
//	printf("fd = %d\n", fd);
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));

/
	int		fd;
	char	*pathname;

	pathname = "abc.txt";
	fd = open(pathname, O_RDONLY);	
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(-1));
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
	printf("%s", get_next_line(fd));
}*/
