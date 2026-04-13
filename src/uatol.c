/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   uatol.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vlnikola <vlnikola@student.42prague.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/13 14:13:49 by vlnikola          #+#    #+#             */
/*   Updated: 2026/04/13 15:24:51 by vlnikola         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	uatol(const char *str, long *result)
{
	long	num;

	num = 0;
	while (*str == ' ' || (*str >= 9 && *str <= 13))
		str++;
	if (*str == '-' || *str == '+')
		if (*str++ == '-')
			return (0);
	if (!*str || (*str < '0' || *str > '9'))
		return (0);
	while (*str)
	{
		if (*str < '0' || *str > '9')
			return (0);
		num = num * 10 + (*str - '0');
		if (num > INT_MAX || num < INT_MIN)
			return (0);
		str++;
	}
	*result = num;
	return (1);
}
