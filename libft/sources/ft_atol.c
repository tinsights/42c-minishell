/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atol.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/06 10:56:01 by tjegades          #+#    #+#             */
/*   Updated: 2024/02/06 10:56:02 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

int	ft_isspace(int c);

long	ft_atol(const char *nptr)
{
	int			polarity;
	long long	nb;

	nb = 0;
	polarity = 1;
	while (ft_isspace(*nptr))
		nptr++;
	if (*nptr == '-' || *nptr == '+')
		if (*nptr++ == '-')
			polarity *= -1;
	while (*nptr > 47 && *nptr < 58)
		nb = nb * 10 + *nptr++ - 48;
	return ((polarity * nb));
}
