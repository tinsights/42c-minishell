/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printptr.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/29 09:52:36 by achak             #+#    #+#             */
/*   Updated: 2024/03/27 13:04:21 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	ft_ptrlen(unsigned long int ptr)
{
	int	len;

	len = 0;
	if (ptr == 0)
		return (1);
	while (ptr >= 1)
	{
		len++;
		ptr /= 16;
	}
	return (len);
}

void	ft_putptr(unsigned long int ptr, int fd)
{
	if (ptr >= 16)
	{
		ft_putptr(ptr / 16, fd);
		ft_putptr(ptr % 16, fd);
	}
	else
	{
		if (ptr < 10)
			print_char(ptr + '0', fd);
		else
			print_char((ptr - 10) + 'a', fd);
	}
}

int	print_ptr(unsigned long int ptr, int fd)
{
	int	count;

	count = 0;
	if (ptr == 0)
		return (write(2, "(nil)", 5));
	count += write(fd, "0x", 2);
	ft_putptr(ptr, fd);
	count += ft_ptrlen(ptr);
	return (count);
}
