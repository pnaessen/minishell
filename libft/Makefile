# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/11/12 12:55:24 by vicperri          #+#    #+#              #
#    Updated: 2025/03/25 10:21:42 by pnaessen         ###   ########lyon.fr    #
#                                                                              #
# **************************************************************************** #

NAME = libft.a

SRCS = ft_isalpha.c ft_isdigit.c ft_isalnum.c ft_isascii.c ft_isprint.c \
	ft_strlen.c ft_memset.c ft_bzero.c ft_memcpy.c ft_memmove.c \
	ft_strlcpy.c ft_strlcat.c ft_toupper.c ft_tolower.c ft_strchr.c \
	ft_strrchr.c ft_strncmp.c ft_memchr.c ft_memcmp.c ft_strnstr.c \
	ft_atoi.c ft_calloc.c ft_strdup.c ft_substr.c ft_strjoin.c \
	ft_striteri.c ft_strtrim.c ft_split.c ft_itoa.c ft_strmapi.c \
	ft_putchar_fd.c ft_putstr_fd.c ft_putendl_fd.c ft_putnbr_fd.c \
	ft_printf.c ft_putchar_printf.c ft_putstr_printf.c ft_unsign_nbr.c \
	ft_hexa_nbr.c ft_putnbr_printf.c ft_hexa_void.c get_next_line.c \
	ft_lstnew_bonus.c ft_lstadd_front_bonus.c ft_lstsize_bonus.c \
	ft_lstlast_bonus.c ft_lstadd_back_bonus.c ft_lstdelone_bonus.c \
	ft_lstclear_bonus.c  ft_lstiter_bonus.c ft_lstmap_bonus.c ft_strcmp.c

OBJDIR = .obj
OBJS = $(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
DEPS := $(OBJS:%.o=%.d)
HEADER = libft.h
CC = cc
CFLAGS = -Wall -Wextra -Werror
CPPFLAGS = -MMD -MP
AR = ar -rcs

# Couleurs
DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

all: welcome $(NAME)

$(NAME): $(OBJS) $(HEADER)
	$(AR) $(NAME) $(OBJS)
	@echo "📚 $(GREEN)Creating library: $@ $(DEF_COLOR)"

$(OBJDIR)/%.o: %.c
	@mkdir -p $(OBJDIR)
	@echo "🛠️  $(GREEN)Compiling: $< $(DEF_COLOR)"
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $< -o $@

-include $(DEPS)

welcome:
	@echo "🚀 $(CYAN)Starting compilation of $(NAME)... Hold on to your bits! $(DEF_COLOR)"

clean:
	rm -rf $(OBJDIR)

fclean: clean
	rm -f $(NAME)

re: fclean all

info:
	@echo "OBJS: $(OBJS)"
	@echo "DEPS: $(DEPS)"

.PHONY: all clean fclean re info welcome
