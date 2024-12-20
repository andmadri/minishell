/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmadri <andmadri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/08 21:23:13 by andmadri          #+#    #+#             */
/*   Updated: 2024/12/20 17:18:20 by andmadri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s)
{
	char	*ptr;
	char	*cpy_s;

	ptr = malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!ptr)
		return (0);
	cpy_s = ptr;
	while (*s)
	{
		*cpy_s = *s;
		cpy_s++;
		s++;
	}
	*cpy_s = '\0';
	return (ptr);
}
