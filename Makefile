NAME = minishell

CC = cc
CFLAGS = -Wall -Werror -Wextra -g
RM = rm -rf
LIBFT_NAME = libft/libft.a

SRCS_DIR = src
SRCS_FILES = minishell.c\
envp_utils.c\
ft_echo.c\
ft_pwd.c\
ft_cd.c\
ft_env.c\
ft_export.c\
ft_export_utils.c\
ft_unset.c\
ft_exit.c\
execution_utils.c\
execution_pipe.c\
execution.c\
heredoc.c\
expandertest.c\
init_data.c\
check_input.c\
check_input_utils.c\
scan_token.c\
scan_token_utils.c\
scan_token_utils2.c\
analyze_token.c\
analyze_token_type.c\
analyze_token_utils.c\
errors.c\
memory_utils.c\
signals.c\
analyze_token_word.c\
errors_mem.c

SRCS = $(addprefix $(SRCS_DIR)/, $(SRCS_FILES))
OBJDIR = obj
OBJS = $(addprefix $(OBJDIR)/, $(subst .c,.o,$(SRCS_FILES)))

UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
    READLINE_PATH := $(shell brew --prefix readline)
    CFLAGS += -I$(READLINE_PATH)/include
    LDFLAGS += -L$(READLINE_PATH)/lib -lreadline
else
    LDFLAGS += -lreadline
endif

all: $(NAME)

$(LIBFT_NAME):
	$(MAKE) -C ./libft

$(NAME): $(OBJS) $(LIBFT_NAME)
	$(CC) $(OBJS) -L./libft -lft $(LDFLAGS) -o $(NAME)

$(OBJDIR)/%.o:$(SRCS_DIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	$(RM) $(OBJDIR)
	$(MAKE) -C ./libft clean

fclean: clean
	$(RM) $(NAME)
	$(MAKE) -C ./libft fclean

re: fclean all

.PHONY: all clean fclean re