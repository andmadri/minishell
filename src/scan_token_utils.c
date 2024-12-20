/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_token_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marieke <marieke@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 11:35:34 by andmadri          #+#    #+#             */
/*   Updated: 2024/06/27 10:34:41 by marieke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	skip_space(t_scanner *scanner)
{
	while (ft_isspace(*scanner->current))
		scanner->current++;
}

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\f' || c == '\n')
		return (1);
	else if (c == '\r' || c == '\t' || c == '\v')
		return (1);
	return (0);
}

int	is_other_tokens(char c)
{
	return (c == '|' || c == '>' || c == '<' || c == '\0');
}

void	find_end_word(t_scanner *scanner)
{
	char	quote;

	quote = '\0';
	if (*scanner->current == '"' || *scanner->current == '\'')
	{
		quote = *scanner->current;
		while (1)
		{
			scanner->current++;
			if (!*scanner->current)
				break ;
			if (*scanner->current == quote)
			{
				scanner->current++;
				if (ft_isspace(*scanner->current))
					break ;
			}
		}
	}
	else
	{
		while (*scanner->current && !ft_isspace(*scanner->current))
			scanner->current++;
	}
}

bool	match(t_scanner *scanner, char match)
{
	scanner->current++;
	if (*scanner->current == '\0')
	{
		scanner->current--;
		return (false);
	}
	if (*scanner->current == match)
		return (true);
	scanner->current--;
	return (false);
}
