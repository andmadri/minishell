/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maraasve <maraasve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/30 20:20:46 by andmadri          #+#    #+#             */
/*   Updated: 2024/07/02 14:48:07 by maraasve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <dirent.h>
# include <fcntl.h>
# include <errno.h>
# include <linux/limits.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdlib.h>
# include <sys/ioctl.h>
# include <sys/types.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <string.h>
# include <unistd.h>
# include "../libft/libft.h"

extern volatile sig_atomic_t	g_signum;

# define EXIT_MISSUSE 2
# define CMD_NOT_X 126
# define CMD_NOT_F 127
# define EXPORT_VAR 1
# define NULL_TERMINATOR 1
# define TRUE 1
# define FALSE 0
# define CHILD 2
# define RUNNING 3
# define HERE_DOC 4

typedef struct s_command
{
	char				**argv;
	char				*infile;
	char				*outfile;
	char				**delimiter;
	int					in_fd;
	int					out_fd;
	int					pipe_fd[2];
	int					pid;
	struct s_command	*pipe;
}				t_command;

typedef struct s_data
{
	char		*input;
	char		**env;
	char		**export_var;
	int			exit_status;
	char		pwd[PATH_MAX];
	t_command	*cmd_head;
}				t_data;

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

typedef struct s_scanner
{
	char	*start;
	char	*current;
}	t_scanner;

typedef struct s_token
{
	char	*start;
	int		length;
	t_type	type;
}	t_token;

typedef struct s_parse
{
	t_command	*cmd;
	t_data		*data;
	t_token		token;
	t_token		next_token;
	t_scanner	*scanner;
}	t_parse;

/* ******************************EXECUTION******************************* */
void	get_cmd_array(t_command *command);
void	dup_fds(t_data *data, t_command *command, char **paths);
int		ft_heredoc(t_data *data, t_command *command, char **paths);
void	ft_pipe_heredoc(t_data *data, t_command *command, char **paths);
int		execution(t_data *data, t_command *command);
int		execution_pipe(t_data *data, t_command *command, char **paths);
int		is_builtin(char *cmd);
bool	paths_exists(t_data *data, char **paths_array);
bool	access_true(t_data *data, char *cmd);
char	*get_command(t_data *data, char **paths_array, char *cmd);
void	close_pipes(t_command *cur_cmd, t_command *prev_cmd);

/* ******************************ENVP******************************* */
int		ft_arraylen(char **array);
int		replace_env(char *var_env, char *replace_str, t_data *data);
char	**copy_envp(char **envp);

/* ***************************INIT_DATA******************************** */
void	init_command(t_command *command);
void	init_t_data(t_data *data);
t_parse	init_parser(t_command *cmd, t_data *data, t_token tkn, t_scanner *scnr);

/* ***************************EXPANDER*************************************** */
char	*expander_test(t_data *data, int i);

/* ****************************TOKENIZER************************************* */
void	skip_space(t_scanner *scanner);
void	find_end_word(t_scanner *scanner);
int		ft_isspace(char c);
int		is_other_tokens(char c);
int		ft_arraylen(char **array);
int		ft_quote_count(char *str, int length, char quote);
int		create_outfile(t_parse *info);
int		analyze_tkn(t_command *cmd, t_data *data, t_token tkn, t_scanner *scnr);
int		analyze_pipe(t_parse info);
int		analyze_here_doc(t_parse *info);
int		analyze_redirect_out(t_parse *info);
int		analyze_redirect_in(t_parse *info);
int		analyze_word(t_parse *info);
bool	match(t_scanner *scanner, char match);
bool	check_input(t_data *data);
bool	not_valid_redirections(char *input);
char	*traverse_token_word(char *str);
char	*ft_create_str(char *str, int length);
char	**expand_array(char **old_array, char *str);
t_token	ft_scan_token(t_scanner *scanner, t_data *data);

/* ****************************BUILT_IN************************************* */
int		print_sorted_array(t_command *cmd, char **array);
void	exit_request(t_data *data, t_command *command);
int		ft_echo(t_data *data, t_command command);
int		ft_cd(t_data *data, t_command *command);
int		ft_pwd(t_data *data, t_command *command, bool print);
int		ft_export(t_data *data, t_command *command);
int		ft_unset(t_data *data, char **argv);
int		ft_unset_export(t_data *data, char *str);
int		ft_env(t_data *data, t_command *command, bool check_argv);
int		ft_env_len(char *str);
int		ft_exit(t_data *data, t_command *cmd, t_command *cur_cmd, char **paths);
char	*find_env(char *var_env, int length, char **array);
char	**rearrange_env(char *ptr_unset, char **array);

/* *****************************MEMORY******************************* */
void	free_dbl_array(char ***array);
void	free_all(t_data *data, t_command *command);
void	free_command_struct(t_command *command);
void	free_null_set(char **str, char ***dbl_array, int fd);
int		terminate_minishell(t_data *data, char **paths);

/* *****************************ERRORS******************************* */
int		error_syntax(t_data *data);
int		error_cmd_not_found(char *cmd_name, char *cmd_not_f, t_data *data);
int		error_permission(char *file_name, t_data *data);
int		error_memory_allocation(t_data *data, t_command *command);
int		print_export_error(char *str);
int		error_no_such(char *file_name, t_data *data);

/* *****************************SIGNALS******************************* */
void	set_signal_handler(int process, t_command *command);

#endif