/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_token_utils2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marieke <marieke@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/19 09:56:53 by andmadri          #+#    #+#             */
/*   Updated: 2024/06/27 10:34:43 by marieke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

char	*traverse_token_word(char *str)
{
	char	quote;

	quote = '\0';
	while (*str && !ft_isspace(*str) && !is_other_tokens(*str))
	{
		if (*str == '"' || *str == '\'')
		{
			quote = *str;
			while (1)
			{
				str++;
				if (!*str)
					return (str);
				if (*str == quote)
				{
					str++;
					if (ft_isspace(*str) || !*str)
						return (str);
				}
			}
		}
		str++;
	}
	return (str);
}
