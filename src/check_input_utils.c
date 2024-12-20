/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input_utils.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marieke <marieke@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/22 14:46:24 by andmadri          #+#    #+#             */
/*   Updated: 2024/06/27 11:48:15 by marieke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

bool	not_valid_redirections(char *input)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (input[i])
	{
		if (input[i] == '<' || input[i] == '>')
			count++;
		else if (count == 2 && (input[i] != '<' || input[i] != '>'))
			count = 0;
		if (count > 2)
			return (true);
		if (input[i] == '|' || ft_isspace(input[i]))
			count = 0;
		i++;
	}
	return (false);
}
