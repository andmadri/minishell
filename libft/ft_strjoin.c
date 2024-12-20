/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strjoin.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmadri <andmadri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 15:21:42 by andmadri          #+#    #+#             */
/*   Updated: 2024/12/20 17:18:26 by andmadri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strjoin(char const *s1, char const *s2)
{
	char	*strjoin;
	int		len;
	int		i;
	int		j;

	len = ft_strlen(s1) + ft_strlen(s2);
	i = 0;
	j = 0;
	strjoin = malloc(sizeof(char) * len + 1);
	if (strjoin == NULL)
		return (NULL);
	while (s1 && s1[i] != '\0')
	{
		strjoin[i] = s1[i];
		i++;
	}
	while (s2 && s2[j] != '\0')
	{
		strjoin[i] = s2[j];
		i++;
		j++;
	}
	strjoin[i] = '\0';
	return (strjoin);
}
