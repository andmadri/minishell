/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maraasve <maraasve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 14:22:43 by andmadri          #+#    #+#             */
/*   Updated: 2024/06/28 13:42:21 by maraasve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

int	error_no_such(char *file_name, t_data *data)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file_name, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	data->exit_status = EXIT_FAILURE;
	return (EXIT_FAILURE);
}

int	error_permission(char *file_name, t_data *data)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(file_name, 2);
	ft_putstr_fd(": Permission denied\n", 2);
	data->exit_status = EXIT_FAILURE;
	return (EXIT_FAILURE);
}

int	error_cmd_not_found(char *cmd_name, char *cmd_not_f, t_data *data)
{
	ft_putstr_fd(cmd_name, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(cmd_not_f, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	data->exit_status = CMD_NOT_F;
	return (EXIT_SUCCESS);
}

int	error_syntax(t_data *data)
{
	ft_putstr_fd("minishell: syntax error near unexpected token\n", 2);
	data->exit_status = EXIT_MISSUSE;
	return (EXIT_FAILURE);
}

int	print_export_error(char *str)
{
	ft_putstr_fd("minishell: export: ", 2);
	ft_putstr_fd(str, 2);
	ft_putstr_fd(": not a valid identifier\n", 2);
	return (EXIT_FAILURE);
}
