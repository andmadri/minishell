/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marieke <marieke@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/04 17:11:15 by maraasve          #+#    #+#             */
/*   Updated: 2024/06/27 10:34:53 by marieke          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static void	signal_handler(int signal_code)
{
	if (signal_code == SIGINT)
	{
		write(1, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	g_signum = signal_code;
}

static void	signal_handler_running(int signal_code)
{
	if (g_signum != signal_code && signal_code == SIGINT)
	{
		write(1, "\n", 1);
	}
	if (g_signum != signal_code && signal_code == SIGQUIT)
	{
		ft_putstr_fd("Quit (core dumped)\n", 1);
	}
	if (g_signum != signal_code)
	{
		kill(0, signal_code);
	}
	g_signum = signal_code;
}

void	set_signal_handler(int process, t_command *command)
{
	if (process == false)
	{
		g_signum = 0;
		signal(SIGINT, &signal_handler);
		signal(SIGQUIT, SIG_IGN);
	}
	else if (process == RUNNING)
	{
		g_signum = 0;
		if (command && command->argv)
		{
			if (command && ft_strncmp(command->argv[0], \
			"./minishell", ft_strlen(command->argv[0])) == 0)
			{
				signal(SIGINT, SIG_IGN);
				return ;
			}
		}
		signal(SIGINT, signal_handler_running);
		signal(SIGQUIT, &signal_handler_running);
	}
}
