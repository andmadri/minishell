/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marieke <marieke@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/06 19:15:12 by andmadri          #+#    #+#             */
/*   Updated: 2024/06/27 11:29:23 by marieke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

char	*find_env(char *var_env, int length, char **array)
{
	int	i;

	i = 0;
	if (length == 0)
		length = ft_strlen(var_env);
	while (array[i])
	{
		if (ft_strncmp(var_env, array[i], length) == 0)
			return (array[i] + length + 1);
		i++;
	}
	return (NULL);
}

int	ft_env(t_data *data, t_command *command, bool check_argv)
{
	int	i;

	i = 0;
	if (find_env("PATH", 4, data->env) == NULL)
	{
		data->exit_status = CMD_NOT_F;
		ft_putstr_fd("minishell: No such file or directory\n", 2);
		return (EXIT_SUCCESS);
	}
	if (command->argv[1] != NULL && check_argv == true)
		return (error_cmd_not_found("env", command->argv[1], data));
	while (data->env[i])
	{
		ft_putstr_fd(data->env[i], command->out_fd);
		ft_putstr_fd("\n", command->out_fd);
		i++;
	}
	data->exit_status = EXIT_SUCCESS;
	return (EXIT_SUCCESS);
}
