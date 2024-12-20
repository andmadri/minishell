/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmadri <andmadri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/03 18:49:03 by andmadri          #+#    #+#             */
/*   Updated: 2024/12/20 17:18:55 by andmadri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *big, const char *lil, size_t len)
{
	size_t	length_lil;

	length_lil = ft_strlen(lil);
	if (*lil == '\0')
		return ((char *)big);
	while (*big)
	{
		if (len < length_lil)
			return (0);
		if (ft_strncmp(big, lil, length_lil) == 0)
			return ((char *) big);
		big++;
		len--;
	}
	return (0);
}
