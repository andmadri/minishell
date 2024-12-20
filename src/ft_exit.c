/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maraasve <maraasve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/02 13:51:45 by andmadri          #+#    #+#             */
/*   Updated: 2024/07/02 15:28:52 by maraasve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

int	terminate_minishell(t_data *data, char **paths)
{
	free_dbl_array(&data->env);
	free_dbl_array(&data->export_var);
	free_dbl_array(&paths);
	free_all(data, data->cmd_head);
	rl_clear_history();
	return (data->exit_status);
}

int	is_numeric_str(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str && str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

void	exit_request(t_data *data, t_command *command)
{
	int	i;

	if (command->pipe == NULL)
	{
		i = 0;
		while (command->argv[i])
		{
			if (ft_strncmp(command->argv[i], "exit", \
			ft_strlen(command->argv[i])) == 0)
				ft_exit(data, data->cmd_head, command, NULL);
			i++;
		}
	}
}

int	ft_exit(t_data *data, t_command *cmd, t_command *cur_cmd, char **paths)
{
	int	argc;

	if (cur_cmd && cur_cmd->pid != 0)
		write(2, "exit\n", 6);
	if (!cmd)
		exit(terminate_minishell(data, paths));
	argc = ft_arraylen(cmd->argv);
	if (argc > 1 && !is_numeric_str(cmd->argv[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd("numeric argument required\n", 2);
		data->exit_status = 2;
	}
	else if (argc > 2)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		return (data->exit_status = 1, EXIT_SUCCESS);
	}
	else if (argc == 2)
		data->exit_status = ft_atoi(cmd->argv[1]);
	if (data->exit_status < 0)
		data->exit_status = 256 + data->exit_status;
	if (data->exit_status > 256)
		data->exit_status = data->exit_status % 256;
	exit(terminate_minishell(data, paths));
}
