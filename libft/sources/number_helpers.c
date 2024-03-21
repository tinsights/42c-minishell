/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   number_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 20:03:20 by tjegades          #+#    #+#             */
/*   Updated: 2023/09/14 20:03:20 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_putnbr(long n, int *count)
{
	char	c;

	if (n == -2147483648)
	{
		write(1, "-2147483648", 11);
		*count += 11;
		return ;
	}
	else if (n < 0)
	{
		write(1, "-", 1);
		*count += 1;
		n = -1 * n;
	}
	if (n >= 10)
		ft_putnbr(n / 10, count);
	c = n % 10 + 48;
	write(1, &c, 1);
	*count += 1;
}

void	ft_putnbr_args(va_list *args, const char **str, int *count)
{
	int	i;

	i = va_arg(*args, int);
	ft_putnbr(i, count);
	*str += 2;
}

void	ft_putunsignednbr_args(va_list *args, const char **str, int *count)
{
	long	i;

	i = va_arg(*args, unsigned int);
	ft_putnbr(i, count);
	*str += 2;
}
