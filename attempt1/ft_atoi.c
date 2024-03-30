/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: achak <marvin@42.fr>                       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/13 10:57:09 by achak             #+#    #+#             */
/*   Updated: 2024/03/30 12:24:01 by achak            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "shell.h"

int	ft_atoi(char *nptr)
{
	int		check;
	long	result;

	check = 1;
	result = 0;
	while ((*nptr == 32 || (*nptr >= 9 && *nptr <= 13)) && *nptr)
		nptr++;
	if (*nptr == '-')
		check = -1;
	if (*nptr == '-' || *nptr == '+')
		nptr++;
	while (*nptr != '\0' && *nptr >= '0' && *nptr <= '9')
	{
		result = result * 10 + (*nptr - '0');
		nptr++;
	}
	return ((int)(result * check));
}

/*#include <stdio.h>

int	main(void)
{
	const char *nptr = "    	 	   +2147483647abcde90";

	printf("ft_atoi: %d\n", ft_atoi(nptr));
	printf("atoi: %d\n", atoi(nptr));
}*/
