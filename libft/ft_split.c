/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmadri <andmadri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 17:44:52 by andmadri          #+#    #+#             */
/*   Updated: 2024/12/20 17:18:12 by andmadri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
#include <stdio.h>

static int	ft_count(const char *s, char c)
{
	int	num_strs;

	num_strs = 0;
	while (*s)
	{
		if (c != *s && *(s + 1) == '\0')
			num_strs++;
		else if (c == *s && *(s - 1) != c)
			num_strs++;
		s++;
	}
	return (num_strs);
}

static char	**ft_memcheck(char **array_array, int i)
{
	while (i >= 0)
	{
		free(array_array[i--]);
	}
	free(array_array);
	return (NULL);
}

static char	**ft_parsing(char **array_array, char const *s, char c, int count)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (*s && i < count)
	{
		while (c == *s && *s)
			s++;
		while (c != *s && *s)
		{
			j++;
			s++;
		}
		s = s - j;
		array_array[i] = ft_substr(s, 0, j);
		if (!array_array[i])
			return (ft_memcheck(array_array, i));
		s = s + j;
		j = 0;
		i++;
	}
	array_array[i] = NULL;
	return (array_array);
}

char	**ft_split(char const *s, char c)
{
	char	**array_array;
	int		count;

	if (s == NULL)
		return (NULL);
	if (c == *s && c != '\0')
		s++;
	count = ft_count(s, c);
	array_array = (char **)malloc((count + 1) * sizeof(char *));
	if (!array_array)
		return (0);
	return (ft_parsing(array_array, s, c, count));
}
