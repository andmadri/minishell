/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmadri <andmadri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 20:39:21 by andmadri          #+#    #+#             */
/*   Updated: 2024/12/20 17:17:52 by andmadri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char	*ptr;

	if (dest == NULL && src == NULL)
		return (dest);
	ptr = dest;
	if (dest <= src)
		return (ft_memcpy(dest, src, n));
	else if (dest > src)
	{
		dest += n - 1;
		src += n - 1;
		while (n--)
		{
			*((unsigned char *)dest) = *((unsigned char *)src);
			dest--;
			src--;
		}
	}
	return (ptr);
}
