/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marieke <marieke@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/07 13:07:19 by andmadri          #+#    #+#             */
/*   Updated: 2024/06/27 10:34:33 by marieke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	init_command(t_command *command)
{
	command->argv = NULL;
	command->infile = NULL;
	command->outfile = NULL;
	command->delimiter = NULL;
	command->pipe = NULL;
	command->pipe_fd[0] = -1;
	command->pipe_fd[1] = -1;
	command->pid = -1;
	command->out_fd = STDOUT_FILENO;
	command->in_fd = STDIN_FILENO;
	command->delimiter = NULL;
	command->pipe = NULL;
}

void	init_t_data(t_data *data)
{
	data->input = NULL;
	data->env = NULL;
	data->export_var = NULL;
	data->cmd_head = NULL;
	data->exit_status = 0;
}

t_parse	init_parser(t_command *cmd, t_data *data, t_token tkn, t_scanner *scnr)
{
	t_parse	info;

	info.cmd = cmd;
	info.data = data;
	info.token = tkn;
	info.next_token = ft_scan_token(scnr, data);
	info.scanner = scnr;
	return (info);
}
