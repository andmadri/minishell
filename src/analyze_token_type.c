/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyze_tkn_type.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marieke <marieke@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/10 17:35:43 by andmadri          #+#    #+#             */
/*   Updated: 2024/06/17 11:57:22 by marieke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

int	analyze_redirect_in(t_parse *info)
{
	int			token_length;
	t_command	*cmd;
	t_data		*data;

	cmd = info->cmd;
	data = info->data;
	token_length = info->next_token.length;
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = ft_create_str(info->next_token.start, token_length);
	if (!cmd->infile)
		return (error_memory_allocation(data, data->cmd_head));
	if (cmd->out_fd >= 0 && cmd->in_fd >= 0)
	{
		cmd->in_fd = open(cmd->infile, O_RDONLY);
		if (cmd->in_fd == -1)
		{
			if (errno == ENOENT)
				error_no_such(cmd->infile, data);
			else if (errno == EACCES)
				error_permission(cmd->infile, data);
		}
	}
	info->next_token = ft_scan_token(info->scanner, data);
	return (EXIT_SUCCESS);
}

void	analyze_redirect_append(t_data *data, t_command *cmd)
{
	if (cmd->out_fd >= 0 && cmd->in_fd >= 0)
	{
		cmd->out_fd = open(cmd->outfile, O_CREAT | O_APPEND | O_RDWR, 00644);
		if (cmd->out_fd == -1)
		{
			if (errno == ENOENT)
				error_no_such(cmd->outfile, data);
			else if (errno == EACCES)
				error_permission(cmd->outfile, data);
		}
	}
}

int	analyze_redirect_out(t_parse *info)
{
	t_command	*cmd;
	t_data		*data;

	cmd = info->cmd;
	data = info->data;
	if (!create_outfile(info))
		return (error_memory_allocation(data, data->cmd_head));
	if (info->token.type == TOKEN_REDIRECT_APPEND)
		analyze_redirect_append(data, cmd);
	else
	{
		if (cmd->out_fd >= 0 && cmd->in_fd >= 0)
		{
			cmd->out_fd = open(cmd->outfile, O_CREAT | O_TRUNC | O_RDWR, 00644);
			if (cmd->out_fd == -1)
			{
				if (errno == ENOENT)
					error_no_such(cmd->outfile, data);
				else if (errno == EACCES)
					error_permission(cmd->outfile, data);
			}
		}
	}
	info->next_token = ft_scan_token(info->scanner, data);
	return (EXIT_SUCCESS);
}

int	analyze_here_doc(t_parse *info)
{
	int			token_length;
	t_command	*cmd;
	t_data		*data;
	char		*del;

	cmd = info->cmd;
	data = info->data;
	token_length = info->next_token.length;
	del = ft_create_str(info->next_token.start, token_length);
	if (!del)
		return (error_memory_allocation(data, data->cmd_head));
	cmd->delimiter = expand_array(cmd->delimiter, del);
	if (!cmd->delimiter)
		return (error_memory_allocation(data, data->cmd_head));
	info->next_token = ft_scan_token(info->scanner, data);
	return (EXIT_SUCCESS);
}

int	analyze_pipe(t_parse info)
{
	info.cmd->pipe = (t_command *)malloc(sizeof(t_command));
	if (!info.cmd->pipe)
		return (error_memory_allocation(info.data, info.data->cmd_head));
	init_command(info.cmd->pipe);
	return (analyze_tkn(info.cmd->pipe, info.data, \
		ft_scan_token(info.scanner, info.data), info.scanner));
}
