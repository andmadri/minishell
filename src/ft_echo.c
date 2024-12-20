/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maraasve <maraasve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:11:05 by andmadri          #+#    #+#             */
/*   Updated: 2024/06/28 13:29:37 by maraasve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static bool	check_flag_n(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (false);
	if (str[i++] != '-')
		return (false);
	while (str[i])
	{
		if (str[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

int	ft_echo(t_data *data, t_command command)
{
	int		i;
	bool	flag_n;

	i = 1;
	flag_n = false;
	if (!command.argv)
		return (ft_putstr_fd("\n", command.out_fd), EXIT_SUCCESS);
	while (command.argv[i] && check_flag_n(command.argv[i]))
	{
		flag_n = true;
		i++;
	}
	while (command.argv[i])
	{
		if (command.argv[i][0] != '\0')
			ft_putstr_fd(command.argv[i], command.out_fd);
		if (command.argv[i + 1])
			ft_putstr_fd(" ", command.out_fd);
		i++;
	}
	if (flag_n == false)
		ft_putstr_fd("\n", command.out_fd);
	if (command.out_fd != -1 && command.in_fd != -1)
		data->exit_status = EXIT_SUCCESS;
	return (EXIT_SUCCESS);
}
