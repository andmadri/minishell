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

int	analyze_word(t_parse *info)
{
	t_command	*cmd;
	t_data		*data;
	char		*str;

	cmd = info->cmd;
	data = info->data;
	str = ft_create_str(info->token.start, info->token.length);
	if (!str)
		return (error_memory_allocation(data, data->cmd_head));
	cmd->argv = expand_array(cmd->argv, str);
	if (!cmd->argv)
		return (error_memory_allocation(data, data->cmd_head));
	return (EXIT_SUCCESS);
}
