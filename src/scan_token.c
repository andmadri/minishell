/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scan_token.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maraasve <maraasve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 18:14:58 by andmadri          #+#    #+#             */
/*   Updated: 2024/06/28 18:10:17 by maraasve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static t_token	create_token(t_type type, t_scanner *scanner)
{
	t_token	token;

	if (!scanner)
	{
		token.type = TOKEN_ERROR;
		return (token.start = NULL, token.length = 0, token);
	}
	if (type == TOKEN_WORD)
		scanner->current = traverse_token_word(scanner->current);
	else if (*scanner->current)
		scanner->current++;
	token.type = type;
	token.start = scanner->start;
	token.length = scanner->current - scanner->start;
	return (token);
}

static t_token	ft_scan_pipe(t_scanner *scanner, t_data *data)
{
	skip_space(scanner);
	if (match(scanner, '|') || match(scanner, '>') || match(scanner, '<'))
	{
		error_syntax(data);
		return (create_token(TOKEN_ERROR, NULL));
	}
	return (create_token(TOKEN_PIPE, scanner));
}

static t_token	ft_scan_redirect_in(t_scanner *scanner, t_data *data)
{
	if (match(scanner, '<'))
		return (create_token(TOKEN_HERE_DOC, scanner));
	if (match(scanner, '|'))
	{
		error_syntax(data);
		return (create_token(TOKEN_ERROR, NULL));
	}
	if (match(scanner, '>'))
	{
		error_syntax(data);
		return (create_token(TOKEN_ERROR, NULL));
	}
	return (create_token(TOKEN_REDIRECT_IN, scanner));
}

static t_token	ft_scan_redirect_out(t_scanner *scanner, t_data *data)
{
	if (match(scanner, '>'))
		return (create_token(TOKEN_REDIRECT_APPEND, scanner));
	if (match(scanner, '|'))
	{
		error_syntax(data);
		return (create_token(TOKEN_ERROR, NULL));
	}
	if (match(scanner, '<'))
	{
		error_syntax(data);
		return (create_token(TOKEN_ERROR, NULL));
	}
	skip_space(scanner);
	return (create_token(TOKEN_REDIRECT_OUT, scanner));
}

t_token	ft_scan_token(t_scanner *scanner, t_data *data)
{
	skip_space(scanner);
	scanner->start = scanner->current;
	if (*scanner->current == '\0')
		return (create_token(TOKEN_END, scanner));
	else if (*scanner->current == '|')
		return (ft_scan_pipe(scanner, data));
	else if (*scanner->current == '>')
		return (ft_scan_redirect_out(scanner, data));
	else if (*scanner->current == '<')
		return (ft_scan_redirect_in(scanner, data));
	else if (!is_other_tokens(*scanner->current))
		return (create_token(TOKEN_WORD, scanner));
	return (create_token(TOKEN_END, scanner));
}
