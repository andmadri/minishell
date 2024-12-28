# Minishell
The objective was to recreate a shell-like program that mimics the behavior of bash in a Unix system. This was a collaborative project done with [Marieke's Last] 

## Build Instructions
First git clone the repository:
```bash
git clone git@github.com:andmadri/minishell.git minishell
```
Afterwards, move to the directory minishell, run the makefile by doing make all, and you should be good to go:

```bash
cd minishell 
make all
...
./minishell 
minishell -> echo hello
hello
minishell -> 
```

It creates an executable called 'minishell', you should be able to run it inside the repository you cloned by './minishell'.

## External Functions
In order to achieve this, we were allowed to use the GNU Readline library which provides a set of functions for reading input from the termnial:

- `readline()`: Used for the read-eval-print loop (**REPL**). The loop repeatedly reads user input, parse it and finally uses the input to execute a command

- `add_history()`: add a line of input to the history list. Navigation of previous commands happens automatically if the up-arrow key is pressed

```c
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

```c
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

```c
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

```bash
minishell -> ./minishell > outfile
minishell: stdout: not a tty
```

This is checked before we enter our REPL

```c
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
## Parsing & Tokenising
There are many ways in which one can parse the input read by readline. One possibility is using a binary tree; however, we found that it was rather complicated not only to find a logical way to fill in the tree but also complex and confusing when it came to use the binary tree for execution. Therefore we implemented a better system.

First of all, we needed **TOKENS**. The tokens are enums that can identify what elements our input string is composed of. The Tokens can be of type:
- REDIRECTION_OUT: >
- REDIRECTION_IN: <
- APPEND: >>
- PIPE: |
- HERE_DOC: <<
- WORD: It can be a command like `ls` or a command with options `echo -n` (these are two separate WORD tokens) or the name of a file `outfile.txt`. Basically anything that is not any of the above types
- EOF: Used to determine when you have reached the end of the input string

![image](https://github.com/user-attachments/assets/c38e89f7-35ab-4ab0-856b-84da979cafd9)

For tokenizing, we used a struct that in principle determines what type of token we have, a char pointer to the beginning of the token and the length of that token:

```c
typedef enum e_type
{
	TOKEN_WORD = 1,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HERE_DOC,
	TOKEN_ERROR,
	TOKEN_END
}			t_type;
```

With every call to our tokenizer function, a `t_token` instance is saved in the stack (avoiding allocating memory). This `t_token` has a pointer to the beginning of the token in the input string, the length of the token and the type of token given by the previously discussed enum. This is useful for the structure we used while parsing, as there we needed to allocate name for a command or filename, and then use the length and pointer of the `t_token` instance to copy it to our parsing structure.

```c
typedef struct s_token
{
	char	*start;
	int		length;
	t_type	type;
}	t_token;
```

This is the struct we used for parsing the input:

```c
typedef struct s_parse
{
	t_command	*cmd;
	t_data		*data;
	t_token		token;
	t_token		next_token;
	t_scanner	*scanner;
}	t_parse;
```

Parsing is fundamental; it should make execution straight-forward and easy.

The struct is in principle composed of:
- char **argv: Used to save the command and the options for the command.
- char *infile: After checking that token == <, it is then expected that the next token == WORD. Therefore if < is found, the word next to it is the name of the infile.
- char *outfile: Same as infile but with token == > or token == >>.
- char **delimiter: This was needed to handle HERE_DOC, given that if `minishell->: << eof1 << eof2`, all these "delimiters" need to be written in order to stop HERE_DOC from executing. Therefore, they need to be saved in an array of delimiters.
- int in_fd: After you determine the name of the infile, it is good to check whether you can read from it by using open().
- int out_fd: Same as in_fd but for outfiles. If the outfile does not exist then it needs to be created. Depending on whether you found a redirection_out or redirection_append, then you need different flags to open the file.
- struct s_command *pipe: If a pipe is found, then you allocate memory for a new struct of type t_command, the current t_command struct is going to be a linked list with as many t_command structs as there are pipes. This is useful because when we had to handle pipes, we could just simply check `while(command->pipe != NULL)`. This allowed us to `fork()` as long as there were pipes.

The function  `analyze_token()` would be called recursively in order to fill in the t_command struct and finish when token == EOF:

```c
int	check_token_type(t_parse *info)
{
	t_token	tkn;

	tkn = info->token;
	if (tkn.type == TOKEN_WORD)
	{
		if (analyze_word(info) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	if (tkn.type == TOKEN_REDIRECT_IN)
	{
		if (analyze_redirect_in(info) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	if (tkn.type == TOKEN_REDIRECT_OUT || tkn.type == TOKEN_REDIRECT_APPEND)
	{
		if (analyze_redirect_out(info) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	if (tkn.type == TOKEN_HERE_DOC)
	{
		if (analyze_here_doc(info) == EXIT_FAILURE)
			return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	analyze_tkn(t_command *cmd, t_data *data, t_token tkn, t_scanner *scnr)
{
	t_parse	info;

	info = init_parser(cmd, data, tkn, scnr);
	if (check_token_type(&info) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	if (tkn.type == TOKEN_ERROR || info.next_token.type == TOKEN_ERROR)
		return (EXIT_FAILURE);
	if (info.next_token.type == TOKEN_PIPE && cmd->argv)
		return (analyze_pipe(info));
	if (info.next_token.type == TOKEN_END && !cmd->argv && \
		!cmd->infile && !cmd->outfile && !cmd->delimiter)
	{
		return (EXIT_FAILURE);
	}
	if (info.next_token.type == TOKEN_END)
		return (EXIT_SUCCESS);
	if (analyze_tkn(cmd, data, info.next_token, scnr) == EXIT_FAILURE)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
```
This is the basic idea of how the structure should look like after parsing the input string:
![image](https://github.com/user-attachments/assets/415fdd02-aca4-49a1-9ff8-00f8ef544cdb)

## Execution
To be able to execute the commands passed to the input string, `execve()` was used. The function is used in Unix-like operating systems to replace the current process with a new process specified by a pathname. It provides a way to run a new program within the context of an existing process.

`int execve(const char *pathname, char *const argv[], char *const envp[])`

To get the pathname parameter of execve, the environment variable **PATH=** was copied into a 2D char array, where every path in **PATH=** was a separate string. To handle whether a command exists or if you do not have the permissions to execute it, the command which is stored in t_command struct char *argv[0], should be attached to every string in the newly 2D paths array, and be checked if it can be accessed:

```c
bool	access_true(t_data *data, char *cmd)
{
	DIR	*dir;

	dir = opendir(cmd);
	if (dir != NULL)
	{
		closedir(dir);
		if ((ft_strlen(cmd) >= 2 && cmd[0] == '.' && cmd[1] == '/') \
			|| cmd[ft_strlen(cmd) - 1] == '/' || cmd[0] == '/')
		{
			data->exit_status = CMD_NOT_X;
			ft_putstr_fd("minishell: Is a directory\n", 2);
		}
		else
			return (data->exit_status = CMD_NOT_F, false);
		return (false);
	}
	if (access(cmd, X_OK) == 0)
		return (true);
	if (access(cmd, F_OK) == 0 && ft_strchr(cmd, '/'))
	{
		data->exit_status = CMD_NOT_X;
		ft_putstr_fd("minishell: Command not executable\n", 2);
		return (false);
	}
	return (false);
}
```

`int access(const char *pathname, int mode)`
It is used to check the accessibility of a file based on the permissions. It can have different flags, the ones used here are **X_OK** (check for execute) and **F_OK** (check for the existance of the file). If a command, which is in theory as well a file, exists but is not executable, then the return code should be **126**. On the other hand when a command is not found, you should return the famous **127**.

```c
int	execution_pipe(t_data *data, t_command *command, char **paths)
{
	t_command	*cur_cmd;
	t_command	*prev_cmd;

	cur_cmd = command;
	prev_cmd = NULL;
	while (cur_cmd)
	{
		set_fds(cur_cmd, prev_cmd);
		if (cur_cmd->argv)
		{
			if (!cur_cmd->pipe && !prev_cmd && is_builtin(cur_cmd->argv[0]))
				return (execute_builtin(data, cur_cmd, paths));
			else if (cur_cmd->argv)
				execute_cmd(data, cur_cmd, paths);
			close_pipes(cur_cmd, prev_cmd);
			if (cur_cmd->pipe)
				prev_cmd = cur_cmd;
		}
		else if (!cur_cmd->argv && cur_cmd->in_fd != -1 && \
					cur_cmd->out_fd != -1)
			data->exit_status = 0;
		cur_cmd = cur_cmd->pipe;
	}
	wait_child(data, command, prev_cmd);
	return (free_dbl_array(&paths), data->exit_status);
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
```

## Builtins
### echo
Prints a line of text to the standard output. Commonly used to display messages or output the value of variables. If *echo -n*, then a newline should be omitted at the end of echo's output.

```c
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
Changes the current working directory. It updates the shell’s working directory to a specified path.

```c
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

- `chdir()`: Changes the current working directory to a char *string taken as the PATH to the directory you want to change.

Part of cd's function is also to update the environment variables PWD and OLDPWD. Cd should also be able to take you to the **HOME** directory if cd ~ and cd should be able to take you to the **OLDPWD** directory if cd -.

### pwd
Prints the current working directory. It shows the absolute path of the shell’s current directory.

```c
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

- `getcwd()`: Gets the current working directory and puts it into a buffer. You have to pass a buffer, in our case data->pwd and the size, sizeof(data->pwd).

It can be that we do not want to print the current working directory and that is because we use `ft_pwd()` for `ft_cd()` to update the environment variable **PWD=**.


### export
Sets environment variables. It makes variables available to child processes.

```bash
minishell -> export NEW_NAME=mary
minishell -> env | grep mary
NEW_NAME=mary
```

If export has no arguments, then all environment variables should be printed in alphabetical order:
```bash
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
```bash
minishell -> export tu=
minishell -> env | grep tu
tu=
```
```c
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

Remember that you have to copy your own environment variables as you need to change them throughout the minishell execution therefore when you export and unset variables, you need to free and allocate memory everytime.

### unset
Removes environment variables. It deletes specified variables from the shell’s environment.

```c
char	**rearrange_env(char *ptr_unset, char **array)
{
	char	**cpy_env;
	int		i;
	int		j;

	i = 0;
	j = 0;
	cpy_env = (char **)malloc(((ft_arraylen(array) - 1) + 1) \
	* sizeof(char *));
	if (!cpy_env)
		return (NULL);
	while (array[i])
	{
		if (array[i] == ptr_unset)
		{
			free(array[i]);
			array[i++] = NULL;
			continue ;
		}
		cpy_env[j++] = array[i];
		array[i++] = NULL;
	}
	cpy_env[j] = NULL;
	free(array);
	array = cpy_env;
	return (cpy_env);
}

int	ft_unset(t_data *data, char **argv)
{
	int		i;
	int		j;

	i = 1;
	while (argv[i])
	{
		j = 0;
		while (data->env[j])
		{
			if (ft_strncmp(argv[i], data->env[j], ft_env_len(argv[i])) == 0)
			{
				if (ft_strncmp("=", data->env[j] + ft_env_len(argv[i]), 1) == 0)
				{
					data->env = rearrange_env(data->env[j], data->env);
					if (!data->env)
						return (error_memory_allocation(data, data->cmd_head));
					break ;
				}
			}
			j++;
		}
		i++;
	}
	return (data->exit_status = EXIT_SUCCESS, EXIT_SUCCESS);
}
```

### env
Displays the environment variables. It lists all environment variables available in the shell.

```c
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
```

## exit
Exits the shell. It terminates the current shell session.

```c
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

The function exit can in theory be executed by itself or take one numeric argument which is what the shell's exit code would be.

```bash
minishell -> exit 23
exit
f0r1s13% echo $?
23
```

However it should be able to handle multiple arguments. The behaviour of exit when a non-numeric argument and a numeric argument are passed together is to exit the shell with an exit code of 2:

```bash
minishell -> exit hello 23
exit
minishell: exit: numeric argument required
f0r1s13% echo $?
2
```

It cannot take the 23 although it is a numeric argument because the first argument should be a digit. On the other hand, if the exit is provided two numeric arguments, it will not terminate the shell but just print "exit". This is because it does not know which numeric argument it should be exited with:

```bash
minishell -> exit 45 67
exit
minishell: exit: too many arguments
```

## Remarks
Throughout this project, we came to understand that replicating every detail of bash’s functionality is a monumental task, especially within the constraints of a project deadline. It's crucial to prioritize the requirements and focus on what is achievable within the project's scope. A robust foundation, particularly in designing the parser, is essential for the successful completion of Minishell.

When working on this project, consider the design of your data structures carefully, as they will be pivotal in the execution phase. Remember, it’s more effective to work smart rather than just hard. Strive for simplicity in your code to avoid unnecessary complications, but maintain diligence to prevent having to patch your work later. 



