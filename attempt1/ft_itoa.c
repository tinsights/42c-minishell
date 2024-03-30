/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/11 14:25:38 by achak             #+#    #+#             */
/*   Updated: 2024/03/29 11:07:33 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

static int	count_digits(long nbr)
{
	int	i;

	i = 0;
	if (nbr < 0)
	{
		nbr *= -1;
		i++;
	}
	while (nbr > 0)
	{
		nbr /= 10;
		i++;
	}
	return (i);
}

static char	*convert(char *str, long nbr, int i)
{
	if (nbr < 0)
	{
		str[0] = '-';
		nbr *= -1;
	}
	while (nbr > 0)
	{
		str[i] = (nbr % 10) + '0';
		nbr /= 10;
		i--;
	}
	return (str);
}

char	*ft_itoa(int n)
{
	long	nbr;
	char	*str;
	int		i;

	nbr = n;
	i = count_digits(nbr);
	if (i == 0)
		i = 1;
	str = malloc(sizeof(char) * (i + 1));
	if (str == NULL)
		return (NULL);
	str[i] = '\0';
	if (n == 0)
	{
		str[0] = '0';
		str[1] = '\0';
		return (str);
	}
	return (convert(str, nbr, i - 1));
}

/*#include <stdio.h>

int	main(void)
{
	printf("%s\n", ft_itoa(2147483647));
	printf("%s\n", ft_itoa(-2147483648));
	printf("%s\n", ft_itoa(4826486));
	printf("%s\n", ft_itoa(-4826486));
	printf("%s\n", ft_itoa(0));
	printf("%s\n", ft_itoa(-100004));
	
}*/
