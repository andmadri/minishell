/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmadri <andmadri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 15:12:26 by andmadri          #+#    #+#             */
/*   Updated: 2024/12/19 14:29:59 by andmadri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static int	update_pwd_oldpwd(t_data *data, t_command *command)
{
	if (!find_env("PWD", 0, data->env) || !find_env("OLDPWD", 0, data->env))
	{
		data->exit_status = EXIT_FAILURE;
		return (EXIT_SUCCESS);
	}
	if (replace_env("OLDPWD=", find_env("PWD", 0, data->env), data) \
	== EXIT_FAILURE)
		return (EXIT_FAILURE);
	ft_pwd(data, command, false);
	if (replace_env("PWD=", data->pwd, data) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

int	ft_changedir(t_data *data, t_command *command, int argc)
{
	if (argc == 1 || (argc == 2 && ft_strncmp(command->argv[1], "~", 1) == 0))
	{
		if (chdir(find_env("HOME", 0, data->env)) == -1)
		{
			perror("minishell: cd");
			return (EXIT_FAILURE);
		}
	}
	else if (ft_strncmp(command->argv[1], "-", 1) == 0)
	{
		if (chdir(find_env("OLDPWD", 0, data->env)) == -1)
		{
			perror("minishell: cd");
			return (EXIT_FAILURE);
		}
	}
	else if (chdir(command->argv[1]) == -1)
	{
		perror("minishell: cd");
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	ft_cd(t_data *data, t_command *command)
{
	int	argc;

	argc = ft_arraylen(command->argv);
	data->exit_status = EXIT_FAILURE;
	if (argc > 2)
	{
		write(2, "minishell: cd: too many arguments\n", 34);
		return (EXIT_FAILURE);
	}
	if (ft_changedir(data, command, argc) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	data->exit_status = update_pwd_oldpwd(data, command);
	return (data->exit_status);
}
