# Minishell
The objective was to recreate a shell-like program that mimics the behavior of bash in a Unix system. This was a collaborative project done with [Marieke's Last] 

## External Functions
In order to achieve this, we were allowed to use the GNU Readline library which provides a set of functions for reading input from the termnial:

- `readline()`: Used for the read-eval-print loop (**REPL**). The loop repeatedly reads user input, parse it and finally uses the input to execute a command

- `add_history()`: add a line of input to the history list. Navigation of previous commands happens automatically if the up-arrow key is pressed

```
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
```

- `rl_on_new_line()`: Informs the Readline library that the cursor is on the new line. It is used to update the state of the input line appropriately

- `rl_redisplay()`: Refreshes and re-displays the current input line. It's useful when the input line has been updated and needs to be shown to the user again.

- `rl_replace_line()`: Replaces the current line in the input buffer with a new line of text.

All of the above functions except for `readline()` were used for handling signals, particularly when there was no running child process.

```
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

```

- `rl_clear_history()`: Clears the history of the command which in theory frees whatever was allocated to save anything that was successfully passed to `readline()`. It is useful when terminating the shell

```
int	terminate_minishell(t_data *data, char **paths)
{
	free_dbl_array(&data->env);
	free_dbl_array(&data->export_var);
	free_dbl_array(&paths);
	free_all(data, data->cmd_head);
	rl_clear_history();
	return (data->exit_status);
}
```

- `isatty()`: Used to determine if a file descriptor refers to a terminal device: STDIN, STDOUT and STDERR. In principle, a shell should only be able to run if none of the previous file descriptors are "dupped", meaning that for instance a shell should not be able to be executed in a file:

```
minishell -> ./minishell > outfile
minishell: stdout: not a tty
```

This is checked before we enter our REPL

```
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
```
## Expansion

## Parsing & Tokenising

## Execution

## Signals

## Builtins
Part of the project was to implement our own builtins most of them without options except for echo that would only be able to handle option -n

### echo
Prints a line of text to the standard output. Commonly used to display messages or output the value of variables. If *echo -n*, then a newline should be omitted at the end of echo's output

```
static bool	check_flag_n(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (false);
	if (str[i++] != '-')
		return (false);
	while (str[i])
	{
		if (str[i] != 'n')
			return (false);
		i++;
	}
	return (true);
}

int	ft_echo(t_data *data, t_command command)
{
	int		i;
	bool	flag_n;

	i = 1;
	flag_n = false;
	if (!command.argv)
		return (ft_putstr_fd("\n", command.out_fd), EXIT_SUCCESS);
	while (command.argv[i] && check_flag_n(command.argv[i]))
	{
		flag_n = true;
		i++;
	}
	while (command.argv[i])
	{
		if (command.argv[i][0] != '\0')
			ft_putstr_fd(command.argv[i], command.out_fd);
		if (command.argv[i + 1])
			ft_putstr_fd(" ", command.out_fd);
		i++;
	}
	if (flag_n == false)
		ft_putstr_fd("\n", command.out_fd);
	if (command.out_fd != -1 && command.in_fd != -1)
		data->exit_status = EXIT_SUCCESS;
	return (EXIT_SUCCESS);
}
```

### cd
Changes the current working directory. It updates the shell’s working directory to a specified path

```
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
```

- `chdir()`: Changes the current working directory to a char *string taken as the PATH to the directory you want to change

Part of cd's function is also to update the environment variables PWD and OLDPWD. Cd should also be able to take you to the **HOME** directory if *cd ~* and cd should be able to take you to the **OLDPWD** directory if *cd -*

### pwd
Prints the current working directory. It shows the absolute path of the shell’s current directory

```
int	ft_pwd(t_data *data, t_command *command, bool print)
{
	if (getcwd(data->pwd, sizeof(data->pwd)) == NULL)
	{
		perror("minishell: pwd");
		data->exit_status = EXIT_FAILURE;
		return (EXIT_SUCCESS);
	}
	if (print == true)
	{
		ft_putstr_fd(data->pwd, command->out_fd);
		ft_putstr_fd("\n", command->out_fd);
	}
	data->exit_status = EXIT_SUCCESS;
	return (EXIT_SUCCESS);
}
```

- `getcwd()`: Gets the current working directory and puts it into a buffer. You have to pass a buffer, in our case data->pwd and the size, sizeof(data->pwd)

It can be that we do not want to print the current working directory and that is because we use `ft_pwd()` for `ft_cd()` to update the environment variable **PWD=**


### export
Sets environment variables. It makes variables available to child processes

```
minishell -> export NEW_NAME=mary
minishell -> env | grep mary
NEW_NAME=mary
```

If export has no arguments, then all environment variables should be printed in alphabetical order:
```
minishell -> export
declare -x COLORTERM="truecolor"
declare -x DBUS_SESSION_BUS_ADDRESS="unix:path=/run/user/154528/bus"
declare -x DEBUGINFOD_URLS="https://debuginfod.ubuntu.com "
declare -x DEFAULTS_PATH="/usr/share/gconf/ubuntu.default.path"
declare -x DESKTOP_SESSION="ubuntu"
declare -x DISPLAY=":0"
declare -x DOCKER_HOST="unix:///run/user/154528/docker.sock"
declare -x DOTNET_BUNDLE_EXTRACT_BASE_DIR="/home/andmadri/.cache/dotnet_bundle_extract"
declare -x FEH_PID="39431"
...
```
Not all names are exportable, only if the name of a variable starts with a letter or and underscore, it is considered a true exportable variable. Moreover, you can export a variable without any information. 
```
minishell -> export tu=
minishell -> env | grep tu
tu=
```
```
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
```

Remember that you have to copy your own environment variables as you need to change them throughout the minishell execution therefore when you export and unset variables, you need to free and allocate memory everytime

### unset
Removes environment variables. It deletes specified variables from the shell’s environment

### env
Displays the environment variables. It lists all environment variables available in the shell

## exit
Exits the shell. It terminates the current shell session

```
int	terminate_minishell(t_data *data, char **paths)
{
	free_dbl_array(&data->env);
	free_dbl_array(&data->export_var);
	free_dbl_array(&paths);
	free_all(data, data->cmd_head);
	rl_clear_history();
	return (data->exit_status);
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
```

The function exit can in theory be executed by itself or take one numeric argument which is what the shell's exit code would be

```
minishell -> exit 23
exit
f0r1s13% echo $?
23
```

However it should be able to handle multiple arguments. The behaviour of exit when a non-numeric argument and a numeric argument are passed together is to exit the shell with an exit code of 2:

```
minishell -> exit hello 23
exit
minishell: exit: numeric argument required
f0r1s13% echo $?
2
```

It cannot take the 23 although it is a numeric argument because the first argument should be a digit. On the other hand, if the exit is provided two numeric arguments, it will not terminate the shell but just print "exit". This is because it does not know which numeric argument it should be exited with:

```
minishell -> exit 45 67
exit
minishell: exit: too many arguments
```

## Remarks
Throughout this project, we came to understand that replicating every detail of bash’s functionality is a monumental task, especially within the constraints of a project deadline. It's crucial to prioritize the requirements and focus on what is achievable within the project's scope. A robust foundation, particularly in designing the parser, is essential for the successful completion of Minishell.

When working on this project, consider the design of your data structures carefully, as they will be pivotal in the execution phase. Remember, it’s more effective to work smart rather than just hard. Strive for simplicity in your code to avoid unnecessary complications, but maintain diligence to prevent having to patch your work later. 



