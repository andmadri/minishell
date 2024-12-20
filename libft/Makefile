NAME	=	libft.a
CC	=	cc
CFLAGS	=	-Wall -Werror -Wextra
SRCS	=	ft_atoi.c\
ft_isalnum.c\
ft_isdigit.c\
ft_memchr.c\
ft_memmove.c\
ft_putendl_fd.c\
ft_split.c\
ft_striteri.c\
ft_strlcpy.c\
ft_strncmp.c\
ft_strtrim.c\
ft_toupper.c\
ft_bzero.c\
ft_isalpha.c\
ft_isprint.c\
ft_memcmp.c\
ft_memset.c\
ft_putnbr_fd.c\
ft_strchr.c\
ft_strjoin.c\
ft_strlen.c\
ft_strnstr.c\
ft_substr.c\
ft_calloc.c\
ft_isascii.c\
ft_itoa.c\
ft_memcpy.c\
ft_putchar_fd.c\
ft_putstr_fd.c\
ft_strdup.c\
ft_strlcat.c\
ft_strmapi.c\
ft_strrchr.c\
ft_tolower.c

BONUS_SRC = ft_lstnew.c\
ft_lstadd_front.c\
ft_lstsize.c\
ft_lstlast.c\
ft_lstadd_back.c\
ft_lstdelone.c\
ft_lstclear.c

BONUS_OBJS = $(BONUS_SRC:.c=.o)

OBJS	=	$(SRCS:.c=.o)
RM	=	rm	-f

all:	$(NAME)

$(NAME): $(OBJS)
	ar -rcs $(NAME) $(OBJS)

$(OBJS): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(OBJS)	$(BONUS_OBJS)

fclean: clean
	$(RM)	$(NAME)

re: fclean all

bonus: $(NAME) $(BONUS_OBJS)
	ar -rcs $(NAME) $(BONUS_OBJS)

$(BONUS_OBJS): %.o : %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean fclean all re