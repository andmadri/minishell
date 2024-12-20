/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   memory_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maraasve <maraasve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/05 15:41:51 by andmadri          #+#    #+#             */
/*   Updated: 2024/06/28 14:18:58 by maraasve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

void	free_dbl_array(char ***array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	if (!*array)
		return ;
	if (!(*array)[i])
		return ;
	while ((*array)[i] != NULL)
	{
		free((*array)[i]);
		(*array)[i] = NULL;
		i++;
	}
	free(*array);
	*array = NULL;
}

void	free_all(t_data *data, t_command *command)
{
	if (command)
	{
		free_command_struct(command);
		command = NULL;
	}
	if (data)
	{
		free(data->input);
		data->input = NULL;
		data = NULL;
	}
}

void	free_command_struct(t_command *command)
{
	if (command->argv)
		free_null_set(NULL, &command->argv, -1);
	if (command->infile)
		free_null_set(&command->infile, NULL, -1);
	if (command->outfile)
		free_null_set(&command->outfile, NULL, -1);
	if (command->in_fd != STDIN_FILENO)
		free_null_set(NULL, NULL, command->in_fd);
	if (command->out_fd != STDOUT_FILENO)
		free_null_set(NULL, NULL, command->out_fd);
	if (command->delimiter)
		free_null_set(NULL, &command->delimiter, -1);
	if (command->pipe)
		free_command_struct(command->pipe);
	free(command);
	command = NULL;
}

void	free_null_set(char **str, char ***dbl_array, int fd)
{
	if (str && *str)
	{
		free(*str);
		*str = NULL;
	}
	else if (dbl_array)
		free_dbl_array(dbl_array);
	else if (fd != -1)
	{
		close(fd);
		fd = -1;
	}
}
