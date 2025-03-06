NAME := minishell
SRC_DIR := src/
BUILTINS_DIR := $(SRC_DIR)builtins/
PARS_DIR := $(SRC_DIR)pars/
OBJ_DIR := .obj/
INCLUDES := include/
LIBFT_DIR := libft/

SRC := $(addprefix $(SRC_DIR), \
	main.c \
	utils.c \
	utils_lst.c \
	signal.c \
	exec.c \
	path_env.c \
	pipe.c \
	redirection.c \
	here_doc.c \
	tree_ast.c \
)

BUILTINS_SRC := $(addprefix $(BUILTINS_DIR), \
	ft_cd.c \
	handle.c \
	ft_pwd.c \
	ft_env.c \
	ft_exit.c \
	ft_unset.c \
	ft_echo.c \
	ft_export.c \
	print_export.c \
)

PARS_SRC := $(addprefix $(PARS_DIR), \
	handle_commands.c \
	handle_whitespace.c \
	identify_tokens.c \
	list_utils.c \
	parsing_utils.c \
	parsing.c \
	pre_tokenisation.c \
	tokenisation.c \
)

SRC += $(BUILTINS_SRC) $(PARS_SRC)
OBJ := $(SRC:%.c=$(OBJ_DIR)%.o)
DEPS := $(OBJ:%.o=%.d)

CC := cc
CFLAGS := -Wextra -Wall -Werror
LDFLAGS := -lreadline
CPPFLAGS := -MMD -MP
HEADERS := -I./include -I$(LIBFT_DIR)
LIBFT := $(LIBFT_DIR)libft.a
LIBFT_FLAGS := -L$(LIBFT_DIR) -lft

# Colors
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

$(NAME): $(LIBFT) $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT_FLAGS) -o $(NAME) $(LDFLAGS)
	@echo "🌊 Surfing the compilation wave: $(BLUE)$@$(DEF_COLOR)"
	@echo "$(GREEN)🏄 Cowabunga! $(NAME) is ready to ride the shell waves!$(DEF_COLOR)"

$(OBJ_DIR)%.o: %.c
	@mkdir -p $(dir $@)
	@echo "🐚 $(MAGENTA)Collecting seashell: $<$(DEF_COLOR)"
	@$(CC) $(CFLAGS) $(HEADERS) $(CPPFLAGS) -c $< -o $@

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

welcome:
	@echo "🏖️ $(CYAN)Setting up the beach for $(NAME)...$(DEF_COLOR)"

clean:
	@echo "🧼 $(YELLOW)Cleaning up the beach...$(DEF_COLOR)"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	@echo "$(RED)🌊 Washing away $(NAME)...$(DEF_COLOR)"
	@$(MAKE) fclean -C $(LIBFT_DIR)
	@rm -f $(NAME)

re: fclean all

valgrind: $(NAME)
	@echo "$(CYAN)🔍 Running Valgrind leak check...$(DEF_COLOR)"
	@valgrind --leak-check=full --suppressions=readline.supp --show-leak-kinds=all --trace-children=yes --track-fds=yes ./$(NAME)

readline.supp:
	@echo "$(YELLOW)Creating readline suppression file...$(DEF_COLOR)"
	@echo "{\n    Readline\n    Memcheck:Leak\n    match-leak-kinds: reachable\n    ...\n    fun:readline\n}" > readline.supp
	@echo "$(GREEN)Created readline.supp file$(DEF_COLOR)"

info:
	@echo "Sources:" $(SRC)
	@echo "Objects:" $(OBJ)
	@echo "Dependencies:" $(DEPS)

-include $(DEPS)

#valgrind --leak-check=full --suppressions=readline.supp --show-leak-kinds=all --trace-children=yes --track-fds=yes ./minishell

.PHONY: all clean fclean re welcome info
