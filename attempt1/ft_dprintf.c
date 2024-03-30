/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_dprintf.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/22 16:12:56 by achak             #+#    #+#             */
/*   Updated: 2024/03/27 13:04:10 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	print_char(int c, int fd)
{
	return (write(fd, &c, 1));
}

int	print_str(char *str, int fd)
{
	int	count;

	count = 0;
	if (str == NULL)
		return (write(2, "(null)", 6));
	while (*str != '\0')
	{
		write(fd, str, 1);
		count++;
		str++;
	}
	return (count);
}

int	print_digit(long n, int base, char specifier, int fd)
{
	int		count;
	char	*symbols;
	char	*symbols2;

	symbols = "0123456789abcdef";
	symbols2 = "0123456789ABCDEF";
	if (n < 0)
	{
		write(fd, "-", 1);
		return (print_digit(-n, base, specifier, fd) + 1);
	}
	else if (n < base)
	{
		if (specifier == 'X')
			return (print_char(symbols2[n], fd));
		return (print_char(symbols[n], fd));
	}
	else
	{
		count = print_digit(n / base, base, specifier, fd);
		return (count + print_digit(n % base, base, specifier, fd));
	}
}

int	print_format(char sp, va_list ap, int fd)
{
	int	count;

	count = 0;
	if (sp == 'c')
		count += print_char(va_arg(ap, int), fd);
	else if (sp == 's')
		count += print_str(va_arg(ap, char *), fd);
	else if (sp == 'd' || sp == 'i')
		count += print_digit((long)(va_arg(ap, int)), 10, sp, fd);
	else if (sp == 'u')
		count += print_digit((long)(va_arg(ap, unsigned int)), 10, sp, fd);
	else if (sp == 'x')
		count += print_digit((long)(va_arg(ap, unsigned int)), 16, sp, fd);
	else if (sp == 'X')
		count += print_digit((long)(va_arg(ap, unsigned int)), 16, sp, fd);
	else if (sp == '%')
		count += write(fd, "%", 1);
	else if (sp == 'p')
		count += print_ptr(va_arg(ap, unsigned long int), fd);
	return (count);
}

int	ft_dprintf(int fd, const char *format, ...)
{
	va_list	ap;
	int		count;

	va_start(ap, format);
	count = 0;
	while (*format != '\0')
	{
		if (*format == '%')
			count += print_format(*(++format), ap, fd);
		else
			count += write(fd, format, 1);
		format++;
	}
	va_end(ap);
	return (count);
}

/*#include <stdio.h>
#include <limits.h>

int	main(void)
{
	int	count;
	char	*str;
	char	*str2;
	char	c1;
	char	c2;
	char	c3 = 0;
	char	c4 = '\0';

	count = 0;
	str = NULL;
	str2 = '\0';
	c1 = 65;
	c2 = 122 - 5;

	ft_printf("\n\nTest 1: %%c\n");
	count = ft_printf("ft_printf: %c %c %c %c\n", c1, c2, c3, c4);
	ft_printf("ft_printf count: %d\n", count - 3);
	count = printf("printf: %c %c %c %c\n", c1, c2, c3, c4);
	printf("printf count: %d\n\n", count);

	ft_printf("Test 2: %%s\n");
	char	str3[50] = "ajfaiefhiawhfi";
	char	*str4 = "\a\xfdåeæioøu";

	count = ft_printf("ft_printf: %s %s %s %s\n", str, str2, str3, str4);
	ft_printf("ft_printf count = %d\n", count - 3);
	count = printf("printf: %s %s %s %s\n", str, str2, str3, str4);
	printf("printf count = %d\n\n", count);

	ft_printf("Test 3: %%p\n");
	count = ft_printf("ft_printf: %p %p %p %p\n", str, str2, str3, str4);
	ft_printf("ft_printf count: %d\n", count - 3);
	count = printf("printf: %p %p %p %p\n", str, str2, str3, str4);
	printf("printf count = %d\n\n", count);

	ft_printf("Test 4: %%d\n");
	int	i = '\0';
	int	j = 0;

	count = ft_printf("ft_printf: %d %d %d %d\n", i, j, INT_MIN, INT_MAX);
	ft_printf("ft_printf count: %d\n", count - 3);
	count = printf("printf: %d %d %d %d\n", i, j, INT_MIN, INT_MAX);
	printf("printf count: %d\n\n", count);

	ft_printf("Test 5: %%i\n");
	count = ft_printf("ft_printf count: %i %i %i %i\n", i, j, INT_MIN, INT_MAX);
	ft_printf("ft_printf count: %d\n", count - 3);
	count = printf("printf count: %i %i %i %i\n", i, j, INT_MIN, INT_MAX);
	printf("printf count: %d\n\n", count);

	ft_printf("Test 6: %%u\n");
	count = ft_printf("ft_printf: %u %u %u %u\n", i, j, INT_MIN, INT_MAX);
	ft_printf("ft_printf count: %d\n", count - 3);
	count = printf("printf: %u %u %u %u\n", i, j, INT_MIN, INT_MAX);	
	printf("printf count: %d\n\n", count);

	ft_printf("Test 7: %%x\n");
	count = ft_printf("ft_printf: %x %x %x %x\n", i, j, INT_MIN, INT_MAX);
	ft_printf("ft_printf count: %d\n", count - 3);
	count = printf("printf: %x %x %x %x\n", i, j, INT_MIN, INT_MAX);
	printf("printf count: %d\n\n", count);

	ft_printf("Test 8: %%X\n");
	count = ft_printf("ft_printf: %X %X %X %X\n", i, j, INT_MIN, INT_MAX);
	ft_printf("ft_printf count: %d\n", count - 3);
	count = printf("printf: %X %X %X %X\n", i, j, INT_MIN, INT_MAX);
	printf("printf count: %d\n\n", count);

	ft_printf("Test 9: %%%%\n");
	count = ft_printf("ft_printf: %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	ft_printf("ft_printf count: %d\n", count - 3);
	count = printf("printf: %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%\n");
	printf("printf count: %d\n\n", count);

	ft_printf("Test 10: No specifiers\n");
	count = ft_printf("ft_printf: Hello World\n");
	ft_printf("ft_printf count: %d\n", count - 3);
	count = printf("printf: Hello World\n");
	printf("printf count: %d\n\n", count);

	ft_printf("Test 11: Mix\n");
	str2 = "fnsnianidn8444afw";
	i = 54646464;
	j = -789656;
	int a = 'p';
	int b = 'r';
	count = ft_printf("ft_printf:0 %p %u %d %X%% %ib\n", str2, i, j, a, b);
	ft_printf("ft_printf count: %d\n", count - 3);
	count = printf("printf:0 %p %u %d %X%% %ib\n", str2, i, j, a, b);
	printf("printf count: %d\n", count);
}*/
