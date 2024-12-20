/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andmadri <andmadri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 15:01:28 by marieke           #+#    #+#             */
/*   Updated: 2024/12/20 12:50:45 by andmadri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static void heredoc_child(t_data *data, t_command *cmd, char *del, char **pth)
{
	char *str;

	str = NULL;
	signal(SIGINT, SIG_DFL);
	while (ft_strncmp(str, del, ft_strlen(del)) != 0)
	{
		if (str)
		{
			ft_putstr_fd(str, cmd->pipe_fd[1]);
			ft_putstr_fd("\n", cmd->pipe_fd[1]);
			free(str);
		}
		str = readline("> ");
		if (!str)
			exit(1);
	}
	free(str);
	close(cmd->pipe_fd[0]);
	close(cmd->pipe_fd[1]);
	terminate_minishell(data, pth);
	if (cmd->in_fd == -1 || cmd->out_fd == -1)
	{
		exit(1);
	}
	exit(0);
}

void wait_heredoc(t_data *data, t_command *command)
{
	waitpid(command->pid, &data->exit_status, 0);
	if (WIFSIGNALED(data->exit_status))
		data->exit_status += 128;
	if (WIFEXITED(data->exit_status))
		data->exit_status = WEXITSTATUS(data->exit_status);
}

int ft_heredoc(t_data *data, t_command *command, char **paths)
{
	int i;

	i = 0;
	while (command->delimiter && command->delimiter[i])
	{
		if (pipe(command->pipe_fd) < 0)
		{
			ft_putstr_fd("pipe: error\n", 2);
			return (EXIT_FAILURE);
		}
		if (command->in_fd != -1 && command->out_fd != -1)
			command->in_fd = command->pipe_fd[0];
		else
			close(command->pipe_fd[0]);
		command->pid = fork();
		if (command->pid == 0)
			heredoc_child(data, command, command->delimiter[i], paths);
		set_signal_handler(RUNNING, NULL);
		close(command->pipe_fd[1]);
		if (command->delimiter[i + 1] != NULL)
			close(command->pipe_fd[0]);
		wait_heredoc(data, command);
		i++;
	}
	return (set_signal_handler(FALSE, NULL), EXIT_SUCCESS);
}
