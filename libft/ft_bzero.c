/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_bzero.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmadri <andmadri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/06 23:54:08 by andmadri          #+#    #+#             */
/*   Updated: 2023/10/28 17:06:28 by andmadri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_bzero(void *s, size_t n)
{
	while (n--)
	{
		*(unsigned char *)s = '\0';
		s++;
	}
}
/*
#include <stdio.h>
#include <strings.h>

int main(void)
{
    char c[] = "Hello";

	bzero(c, sizeof(c));
    //ft_bzero(c, sizeof(c)); 
    printf("%s\n", c);

    return 0;
}*/