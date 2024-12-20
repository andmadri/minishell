/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marieke <marieke@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:12:50 by andmadri          #+#    #+#             */
/*   Updated: 2024/06/27 11:29:58 by marieke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

int	ft_pwd(t_data *data, t_command *command, bool print)
{
	if (getcwd(data->pwd, sizeof(data->pwd)) == NULL)
	{
		perror("minishell: pwd");
		data->exit_status = EXIT_FAILURE;
		return (EXIT_SUCCESS);
	}
	if (print == true)
	{
		ft_putstr_fd(data->pwd, command->out_fd);
		ft_putstr_fd("\n", command->out_fd);
	}
	data->exit_status = EXIT_SUCCESS;
	return (EXIT_SUCCESS);
}
