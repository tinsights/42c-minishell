/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hex_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/14 20:06:52 by tjegades          #+#    #+#             */
/*   Updated: 2023/09/14 20:06:53 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putunsignednbr_base(unsigned long nbr, char *base)
{
	int				num_of_digits;
	int				length;
	unsigned long	copy;
	char			result[32];

	copy = nbr;
	num_of_digits = 0;
	length = ft_strlen(base);
	while (copy > 0)
	{
		result[31 - num_of_digits++] = base[copy % length];
		copy /= length;
	}
	if (!nbr)
	{
		write(1, base, 1);
		return (1);
	}
	write(1, result + 32 - num_of_digits, num_of_digits);
	return (num_of_digits);
}

void	ft_putaddr_args(va_list *args, const char **str, int *count)
{
	unsigned long	addr;

	addr = va_arg(*args, unsigned long);
	if (!addr)
	{
		write(1, "(nil)", 5);
		*count += 5;
		*str += 2;
		return ;
	}
	ft_putstr("0x");
	*count += 2 + ft_putunsignednbr_base(addr, "0123456789abcdef");
	*str += 2;
}

void	ft_puthex_args(va_list *args, const char **str, int *count, int caps)
{
	unsigned int	i;

	i = va_arg(*args, unsigned int);
	if (caps)
		*count += ft_putunsignednbr_base(i, "0123456789ABCDEF");
	else
		*count += ft_putunsignednbr_base(i, "0123456789abcdef");
	*str += 2;
}
