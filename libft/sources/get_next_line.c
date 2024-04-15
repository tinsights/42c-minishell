/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/23 13:07:36 by tjegades          #+#    #+#             */
/*   Updated: 2024/01/23 13:07:38 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include "libft.h"

int	init(char **store, char **buff, char **result)
{
	if (*store == NULL)
		*store = gnl_calloc(1);
	*result = gnl_calloc(1);
	*buff = gnl_calloc(BUFFER_SIZE + 1);
	if (*store == NULL || *result == NULL || *buff == NULL)
	{
		if (*store)
			free(*store);
		if (*buff)
			free(*buff);
		if (*result)
			free(*result);
		*store = NULL;
		return (0);
	}
	return (1);
}

int	check_for_newline(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '\n')
			return (i);
		i++;
	}
	return (-1);
}

void	gnl_concat_replace(char **s1, char *s2, int limit)
{
	int		len1;
	int		total_len;
	char	*result;
	int		i;

	len1 = gnl_strlen(*s1);
	total_len = len1 + limit;
	result = gnl_calloc(total_len + 1);
	i = -1;
	while (++i < len1)
		result[i] = (*s1)[i];
	i = -1;
	while (++i < limit)
	{
		if (s2[i] == 0)
			break ;
		result[len1 + i] = s2[i];
	}
	free(*s1);
	*s1 = result;
}

int	meat_and_potatoes(char **buff, char **store, char **result)
{
	int	nl_idx;
	int	num_read;

	nl_idx = check_for_newline(*buff);
	num_read = gnl_strlen(*buff);
	if (nl_idx >= 0)
	{
		gnl_concat_replace(result, *buff, nl_idx + 1);
		gnl_concat_replace(store, *buff + nl_idx + 1, num_read - nl_idx - 1);
		return (1);
	}
	gnl_concat_replace(result, *buff, num_read);
	gnl_bzero(*buff, BUFFER_SIZE);
	return (0);
}

char	*get_next_line(int fd)
{
	char		*buff;
	char		*result;
	static char	*store;

	if (fd < 0 || !init(&store, &buff, &result))
	{
		ft_free((void **) store);
		return (NULL);
	}
	if (gnl_strlen(store) > 0)
		gnl_concat_replace(&buff, store, BUFFER_SIZE);
	while (gnl_strlen(store) > 0 || read(fd, buff, BUFFER_SIZE) > 0)
	{
		if (gnl_strlen(store))
			gnl_bzero(store, gnl_strlen(store));
		if (meat_and_potatoes(&buff, &store, &result))
			break ;
	}
	free(buff);
	if (gnl_strlen(result) > 0)
		return (result);
	free(store);
	store = NULL;
	free(result);
	return (NULL);
}

// #include <stdio.h>
// int	main(void)
// {
// 	int fd;

// 	fd = open("file.txt", O_RDONLY);

// 	char *result = get_next_line(fd);
// 	printf("s1: %s", result);
// 	free(result);

// 	result = get_next_line(fd);
// 	printf("s2: %s", result);
// 	if (result)
// 		free(result);

// 	result = get_next_line(fd);
// 	printf("s3: %s", result);
// 	if (result)
// 		free(result);

// 	result = get_next_line(fd);
// 	printf("s4: %s", result);
// 	if (result)
// 		free(result);

// 	result = get_next_line(fd);
// 	printf("s5: %s", result);
// 	if (result)
// 		free(result);

// 	close(fd);
// }