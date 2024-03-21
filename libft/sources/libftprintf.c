/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tinaes <tinaes@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 20:29:51 by tjegades          #+#    #+#             */
/*   Updated: 2023/09/14 15:01:37 by tinaes           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	ft_handle_percent(const char **str, va_list *args, int *count)
{
	if (*(*str + 1) == 'c')
		ft_putchar_args(args, str, count);
	else if (*(*str + 1) == 's')
		ft_putstr_args(args, str, count);
	else if (*(*str + 1) == 'd' || *(*str + 1) == 'i')
		ft_putnbr_args(args, str, count);
	else if (*(*str + 1) == 'u')
		ft_putunsignednbr_args(args, str, count);
	else if (*(*str + 1) == 'p')
		ft_putaddr_args(args, str, count);
	else if (*(*str + 1) == 'x' || *(*str + 1) == 'X')
		ft_puthex_args(args, str, count, *(*str + 1) == 'X');
	else if (*(*str + 1) == '%')
		ft_putpercent(str, count);
	else
		str++;
}

int	ft_printf(const char *str, ...)
{
	va_list	args;
	int		count;

	count = 0;
	va_start(args, str);
	while (*str)
	{
		if (*str != '%')
		{
			write(1, str++, 1);
			count++;
		}
		else
			ft_handle_percent(&str, &args, &count);
	}
	va_end(args);
	return (count);
}

// #include <stdio.h>
// int	main(void)
// {
// 	char *str = "bonjour";
// 	printf("%c%sl%co, %s.\nI am %d years old.\n", 72, "e", 'l', "world", 31);
// 	ft_printf("%c%sl%co, %s.\nI am %d years old.\n", 72, "e", 'l', "world", 31);
// 	printf("at the address %p, the string %s is stored\n", str, str);
// 	ft_printf("at the address %p, the string %s is stored\n", str, str);
// 	unsigned int big = (unsigned int) INT_MAX * 2;
// 	ft_printf("%u\n", big);
// 	printf("%u\n", big);
// 	printf("%li\n", sizeof(str));
// 	printf("  ||  %i\n", printf(" NULL %s NULL", (char *)NULL));
// 	printf("  ||  %i\n", ft_printf(" NULL %s NULL", (char *)NULL));
//   	printf("  ||  %i\n", ft_printf("%p", NULL));
//   	printf("  ||  %i\n", printf("%p", NULL));
// 	printf("%p %p \n", (void *) (void *) 0 , (void *)0);
// 	ft_printf("%p %p \n", (void *) (void *) 0 , (void *)0);
// 	printf("  ||  %i\n", ft_printf(" %x ", 0));
// 	printf("  ||  %i\n", printf(" %x ", 0));
// 	ft_printf("  %%%%  \n");
// 	printf("  %%%%  \n");

// 	printf("  ||  %i\n", ft_printf("\x01\x02\a\v\b\f\r"));
// 	printf("  ||  %i\n", printf("\x01\x02\a\v\b\f\r"));

// 	printf("  ||  %i\n", ft_printf(""));
// 	printf("  ||  %i\n", printf(""));

// 	printf("  ||  %i\n", ft_printf("%%c"));
// 	printf("  ||  %i\n", printf("%%c"));

// 	printf("  ||  %i\n", ft_printf("%%%%%%"));
// 	printf("  ||  %i\n", printf("%%%%%%"));

// 	printf("  ||  %i\n", ft_printf("%%%c", 'x'));
// 	printf("  ||  %i\n", printf("%%%c", 'x'));

// 	printf("  ||  %i\n", ft_printf("%cc%cc%c", 'a', '\t', 'b'));
// 	printf("  ||  %i\n", printf("%cc%cc%c", 'a', '\t', 'b'));

// 	printf("  ||  %i\n", ft_printf("%s", (char *)NULL));
// 	printf("  ||  %i\n", printf("%s", (char *)NULL));

// 	printf("  ||  %i\n", ft_printf("%sx", "{} al$#@@@^&$$^#^@@^$*((&"));
// 	printf("  ||  %i\n", printf("%sx", "{} al$#@@@^&$$^#^@@^$*((&"));

// 	printf("  ||  %i\n", ft_printf(" %s", "can it handle \t and \n?"));
// 	printf("  ||  %i\n", printf(" %s", "can it handle \t and \n?"));

// 	printf("  ||  %i\n", ft_printf("%u", UINT_MAX));
// 	printf("  ||  %i\n", printf("%u", UINT_MAX));

// 	printf("  ||  %i\n", ft_printf("%u", -6000023));
// 	printf("  ||  %i\n", printf("%u", -6000023));

// 	printf("  ||  %i\n", ft_printf("%X", UINT_MAX));
// 	printf("  ||  %i\n", printf("%X", UINT_MAX));

// 	printf("  ||  %i\n", ft_printf("%pp", (void *)LONG_MAX + 423856));
// 	printf("  ||  %i\n", printf("%pp", (void *)LONG_MAX + 423856));
// 	printf("  ||  %i\n", ft_printf("%pp", (void *)INT_MAX));
// 	printf("  ||  %i\n", printf("%pp", (void *)INT_MAX));
// 	printf("  ||  %i\n", ft_printf("%pp", (void *)0));
// 	printf("  ||  %i\n", printf("%pp", (void *)0));

// 	printf("  ||  %i\n", ft_printf("0x%p-", (void *)ULONG_MAX));
// 	printf("  ||  %i\n", printf("0x%p-", (void *)ULONG_MAX));

// 	printf("  ||  %i\n", ft_printf("%p", (void *)-14523));
// 	printf("  ||  %i\n", printf("%p", (void *)-14523));

// 	printf("  ||  %i\n", ft_printf("%p", ""));
// 	printf("  ||  %i\n", printf("%p", ""));
// }
