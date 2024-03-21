/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   str_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 20:01:52 by tjegades          #+#    #+#             */
/*   Updated: 2023/09/14 20:01:53 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putstr(char *str)
{
	while (*str)
		write(1, str++, 1);
}

void	ft_putchar_args(va_list *args, const char **str, int *count)
{
	char	c;

	c = va_arg(*args, int);
	write(1, &c, 1);
	*count += 1;
	*str += 2;
}

void	ft_putstr_args(va_list *args, const char **str, int *count)
{
	char	*s;
	int		len;

	s = (char *) va_arg(*args, void *);
	if (!s)
	{
		write(1, "(null)", 6);
		*count += 6;
		*str += 2;
		return ;
	}
	len = ft_strlen(s);
	write(1, s, len);
	*count += len;
	*str += 2;
}

void	ft_putpercent(const char **str, int *count)
{
	ft_putstr("%");
	*count += 1;
	*str += 2;
}
