/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maraasve <maraasve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 20:20:50 by andmadri          #+#    #+#             */
/*   Updated: 2024/07/02 15:51:30 by maraasve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

volatile sig_atomic_t	g_signum = 0;

static void	check_terminals(void)
{
	if (!isatty(STDIN_FILENO))
	{
		write(2, "minishell: stdin: not a tty\n", 28);
		exit(1);
	}
	if (!isatty(STDOUT_FILENO))
	{
		write(2, "minishell: stdout: not a tty\n", 29);
		exit(1);
	}
	if (!isatty(STDERR_FILENO))
	{
		write(2, "minishell: stderr: not a tty\n", 29);
		exit(1);
	}
}

static int	lexer(t_data *data, t_command *command)
{
	t_scanner	scanner;
	t_token		token;

	scanner.start = data->input;
	scanner.current = data->input;
	if (check_input(data) == false)
		return (error_syntax(data));
	token = ft_scan_token(&scanner, data);
	return (analyze_tkn(command, data, token, &scanner));
}

static void	read_input(t_data *data)
{
	data->input = readline("minishell -> ");
	if (g_signum == SIGINT)
	{
		data->exit_status = 130;
		g_signum = 0;
	}
	if (!data->input)
	{
		write(2, "exit\n", 6);
		ft_exit(data, data->cmd_head, NULL, NULL);
	}
}

static int	read_eval_print_loop(t_data *data)
{
	while (1)
	{
		data->cmd_head = NULL;
		read_input(data);
		add_history(data->input);
		data->input = expander_test(data, 0);
		if (!data->input)
			continue ;
		data->cmd_head = (t_command *)malloc(sizeof(t_command));
		if (!data->cmd_head)
		{
			error_memory_allocation(data, data->cmd_head);
			continue ;
		}
		init_command(data->cmd_head);
		lexer(data, data->cmd_head);
		execution(data, data->cmd_head);
		free_all(data, data->cmd_head);
	}
	return (0);
}

int	main(int argc, char *argv[], char *envp[])
{
	t_data	data;

	if (argc != 1 || argv[1] != NULL || envp[0] == NULL)
		return (EXIT_FAILURE);
	check_terminals();
	init_t_data(&data);
	data.env = copy_envp(envp);
	if (!data.env)
		exit(error_memory_allocation(&data, data.cmd_head));
	data.export_var = copy_envp(envp);
	if (!data.env)
		exit(error_memory_allocation(&data, data.cmd_head));
	set_signal_handler(FALSE, NULL);
	read_eval_print_loop(&data);
	return (EXIT_SUCCESS);
}
