/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_input.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marieke <marieke@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:12:42 by andmadri          #+#    #+#             */
/*   Updated: 2024/06/27 10:33:56 by marieke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static bool	check_unexepected_token(char *input, int i, int *j)
{
	while (input[*j])
	{
		while (ft_isspace(input[*j]))
			*j += 1;
		if (input[i] == '|' && input[*j] == '|')
			return (true);
		else if (input[i] == '<' && input[*j] == '|')
			return (true);
		else if (input[i] == '>' && input[*j] == '>' && *j - i > 1)
			return (true);
		else if (input[i] == '<' && input[*j] == '<' && *j - i > 1)
			return (true);
		else
			break ;
	}
	return (false);
}

static char	last_char_non_space(char *str, int length)
{
	while (length >= 0)
	{
		if (str[length] && !ft_isspace(str[length]))
			return (str[length]);
		length--;
	}
	return ('\0');
}

static bool	is_unexpected_token(char *input)
{
	int		i;
	int		j;
	char	start;
	char	end;

	i = 0;
	end = last_char_non_space(input, ft_strlen(input));
	while (input[i] && !isspace(input[i]))
		i++;
	start = input[i];
	if (start == '|')
		return (true);
	if (end == '|' || end == '<' || end == '>')
		return (true);
	i = 0;
	while (i < (int)ft_strlen(input) && input[i])
	{
		j = i + 1;
		if (check_unexepected_token(input, i, &j))
			return (true);
		i = j;
	}
	return (false);
}

static bool	unclosed_quotes(char *input)
{
	int		i;
	bool	dbl_quotes;
	bool	single_quotes;

	i = 0;
	dbl_quotes = true;
	single_quotes = true;
	while (input[i])
	{
		if (input[i] == '"' && single_quotes == true)
			dbl_quotes = !dbl_quotes;
		else if (input[i] == '\'' && dbl_quotes == true)
			single_quotes = !single_quotes;
		i++;
	}
	if (dbl_quotes != true || single_quotes != true)
		return (true);
	return (false);
}

bool	check_input(t_data *data)
{
	if (unclosed_quotes(data->input))
		return (false);
	if (is_unexpected_token(data->input))
		return (false);
	if (not_valid_redirections(data->input))
		return (false);
	if (ft_strncmp(data->input, "", ft_strlen(data->input)) == 0)
		return (true);
	return (true);
}
