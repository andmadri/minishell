/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maraasve <maraasve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/27 17:42:53 by maraasve          #+#    #+#             */
/*   Updated: 2024/07/02 15:15:40 by maraasve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

char	*found_not_x(t_data *data, char *cmd)
{
	if (access(cmd, F_OK) == 0 && ft_strchr(cmd, '/'))
	{
		if (data->exit_status != CMD_NOT_F && data->exit_status != CMD_NOT_X)
		{
			data->exit_status = CMD_NOT_X;
			ft_putstr_fd("minishell: Command not executable\n", 2);
		}
		return (NULL);
	}
	return (cmd);
}

char	*get_command(t_data *data, char **paths_array, char *cmd)
{
	char	*tmp;
	char	*command;

	if (access_true(data, cmd))
		return (cmd);
	if (!found_not_x(data, cmd))
		return (NULL);
	if (!paths_exists(data, paths_array))
		return (NULL);
	while (*paths_array && cmd[0] != '\0')
	{
		tmp = ft_strjoin(*paths_array, "/");
		if (!tmp)
			return (write(2, "minishell: memory allocation error\n", 35), NULL);
		command = ft_strjoin(tmp, cmd);
		free (tmp);
		if (!command)
			return (write(2, "minishell: memory allocation error\n", 35), NULL);
		if (access_true(data, command))
			return (command);
		free(command);
		paths_array++;
	}
	data->exit_status = CMD_NOT_F;
	return (ft_putstr_fd("minishell: Command not found\n", 2), NULL);
}

char	*get_path(char *envp[])
{
	if (!envp)
		return (NULL);
	while (*envp)
	{
		if (ft_strncmp("PATH", *envp, 4) == 0)
			return (*envp + 5);
		envp++;
	}
	return (NULL);
}

void	check_heredoc(t_data *data, t_command *command, char **paths)
{
	int			here_doc;
	t_command	*tmp;

	here_doc = false;
	tmp = command;
	while (tmp)
	{
		if (tmp->delimiter)
		{
			here_doc = true;
			break ;
		}
		tmp = tmp->pipe;
	}
	if (here_doc)
		ft_heredoc(data, tmp, paths);
}

int	execution(t_data *data, t_command *command)
{
	char		*path;
	char		**path_array;
	int			return_value;

	path_array = NULL;
	path = get_path(data->env);
	if (path)
	{
		path_array = ft_split(path, ':');
		if (!path_array)
			return (error_memory_allocation(data, data->cmd_head));
	}
	check_heredoc(data, command, path_array);
	return_value = execution_pipe(data, command, path_array);
	return (return_value);
}
