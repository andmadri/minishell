/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maraasve <maraasve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/07 19:49:23 by andmadri          #+#    #+#             */
/*   Updated: 2024/07/02 15:51:01 by maraasve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static char	**set_env_var(char **array, char *variable)
{
	char	**cpy_env;
	int		i;
	int		num_env;

	i = 0;
	num_env = ft_arraylen(array) + EXPORT_VAR + NULL_TERMINATOR;
	cpy_env = (char **)malloc(num_env * sizeof(char *));
	if (!cpy_env)
		return (NULL);
	while (array[i])
	{
		cpy_env[i] = ft_strdup(array[i]);
		if (!cpy_env)
			return (NULL);
		free(array[i]);
		array[i] = NULL;
		i++;
	}
	cpy_env[i] = ft_strdup(variable);
	if (!cpy_env)
		return (NULL);
	i++;
	cpy_env[i] = NULL;
	free(array);
	return (cpy_env);
}

static int	true_exportable_var(t_data *data, char *str)
{
	int		i;

	i = 1;
	if (str[0] != '_' && !ft_isalpha(str[0]))
	{
		print_export_error(str);
		data->exit_status = 1;
		return (1);
	}
	while (str[i] && str[i] != '=' && (ft_isalnum(str[i]) || str[i] == '_'))
		i++;
	if (str[i] == '=')
		return (0);
	if (str[i] == '\0')
		return (2);
	print_export_error(str);
	data->exit_status = 1;
	return (1);
}

int	ft_export_var(t_data *data, t_command *command, int i)
{
	if (find_env(command->argv[i], \
		ft_env_len(command->argv[i]), data->export_var))
	{
		if (ft_unset_export(data, command->argv[i]) == EXIT_FAILURE)
			return (free(command->argv[i]), \
				error_memory_allocation(data, data->cmd_head));
	}
	data->export_var = set_env_var(data->export_var, command->argv[i]);
	if (!data->export_var)
	{
		print_export_error(command->argv[i]);
		return (free(command->argv[i]), \
			error_memory_allocation(data, data->cmd_head));
	}
	return (EXIT_SUCCESS);
}

static int	ft_export_env(t_data *data, t_command *command, int i)
{
	if (find_env(command->argv[i], ft_env_len(command->argv[i]), data->env))
	{
		if (ft_unset(data, command->argv) == EXIT_FAILURE)
			return (error_memory_allocation(data, data->cmd_head));
	}
	if (find_env(command->argv[i], \
		ft_env_len(command->argv[i]), data->export_var))
	{
		if (ft_unset_export(data, command->argv[i]) == EXIT_FAILURE)
			return (error_memory_allocation(data, data->cmd_head));
	}
	data->env = set_env_var(data->env, command->argv[i]);
	if (!data->env)
		return (error_memory_allocation(data, data->cmd_head));
	if (ft_export_var(data, command, i) == EXIT_FAILURE)
		return (error_memory_allocation(data, data->cmd_head));
	return (EXIT_SUCCESS);
}

int	ft_export(t_data *data, t_command *command)
{
	int		i;
	int		true_exp_var;

	i = 1;
	if (!command->argv[i])
		return (print_sorted_array(command, data->export_var));
	true_exp_var = true_exportable_var(data, command->argv[i]);
	while (command->argv[i])
	{
		if (true_exp_var == 0)
		{
			if (ft_export_env(data, command, i) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			data->exit_status = EXIT_SUCCESS;
		}
		else if (true_exp_var == 2)
		{
			if (ft_export_var(data, command, i) == EXIT_FAILURE)
				return (EXIT_FAILURE);
			data->exit_status = EXIT_SUCCESS;
		}
		i++;
	}
	return (EXIT_SUCCESS);
}
