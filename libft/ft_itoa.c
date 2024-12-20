/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmadri <andmadri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/09 20:22:02 by andmadri          #+#    #+#             */
/*   Updated: 2024/12/20 17:17:30 by andmadri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <limits.h>
#include <stddef.h>

static size_t	ft_int_length(int n)
{
	size_t	i;

	i = 0;
	if (n == 0)
		return (1);
	else if (n < 0)
		i++;
	while (n)
	{
		n = n / 10;
		i++;
	}
	return (i);
}

static int	ft_absolute(int n)
{
	if (n < 0)
		return (-n);
	else
		return (n);
}

static void	*ft_print_num(char *ptr, int n)
{
	if (n / 10 != 0)
		ft_print_num(ptr - 1, n / 10);
	*ptr = ft_absolute(n % 10) + '0';
	return (ptr);
}

char	*ft_itoa(int n)
{
	char	*ptr;
	size_t	int_length;

	int_length = ft_int_length(n);
	ptr = malloc((int_length + 1) * sizeof(char));
	if (!ptr)
		return (0);
	ptr[int_length] = '\0';
	if (n < 0)
		ptr[0] = '-';
	ft_print_num(ptr + int_length - 1, n);
	return (ptr);
}
