/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tjegades <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 21:08:38 by tjegades          #+#    #+#             */
/*   Updated: 2023/09/12 21:08:39 by tjegades         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <limits.h>
# include <unistd.h>
# include "libft.h"

void	ft_putstr(char *str);
void	ft_putchar_args(va_list *args, const char **str, int *count);
void	ft_putstr_args(va_list *args, const char **str, int *count);
void	ft_putpercent(const char **str, int *count);
void	ft_putnbr_args(va_list *args, const char **str, int *count);
void	ft_putunsignednbr_args(va_list *args, const char **str, int *count);
void	ft_putaddr_args(va_list *args, const char **str, int *count);
void	ft_puthex_args(va_list *args, const char **str, int *count, int caps);
int		ft_printf(const char *str, ...);

#endif
