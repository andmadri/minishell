/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyze_tkn.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmadri <andmadri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/06 12:34:04 by andmadri          #+#    #+#             */
/*   Updated: 2024/06/16 19:04:52 by andmadri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

int	check_token_type(t_parse *info)
{
	t_token	tkn;

	tkn = info->token;
	if (tkn.type == TOKEN_WORD)
	{
		if (analyze_word(info) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	if (tkn.type == TOKEN_REDIRECT_IN)
	{
		if (analyze_redirect_in(info) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	if (tkn.type == TOKEN_REDIRECT_OUT || tkn.type == TOKEN_REDIRECT_APPEND)
	{
		if (analyze_redirect_out(info) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	if (tkn.type == TOKEN_HERE_DOC)
	{
		if (analyze_here_doc(info) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	analyze_tkn(t_command *cmd, t_data *data, t_token tkn, t_scanner *scnr)
{
	t_parse	info;

	info = init_parser(cmd, data, tkn, scnr);
	if (check_token_type(&info) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (tkn.type == TOKEN_ERROR || info.next_token.type == TOKEN_ERROR)
		return (EXIT_FAILURE);
	if (info.next_token.type == TOKEN_PIPE && cmd->argv)
		return (analyze_pipe(info));
	if (info.next_token.type == TOKEN_END && !cmd->argv && \
		!cmd->infile && !cmd->outfile && !cmd->delimiter)
	{
		return (EXIT_FAILURE);
	}
	if (info.next_token.type == TOKEN_END)
		return (EXIT_SUCCESS);
	if (analyze_tkn(cmd, data, info.next_token, scnr) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
