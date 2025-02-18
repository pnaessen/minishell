NAME := minishell

SRC:= $(addprefix $(SRC_DIR),main.c utils.c utils_lst.c)
BUILTINS_SRC:=$(addprefix $(BUILTINS_DIR), echo.c handle.c pwd.c)
SRC += $(addprefix $(BUILTINS_DIR), $(BUILTINS_SRC))

OBJ_DIR:= .obj/
OBJ:= $(SRC:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)
DEPS:= $(OBJ:%.o=%.d)

CC:= cc
CCFLAGS:= -Wextra -Wall -Werror
LDFLAGS := -lreadline
CPPFLAGS = -MMD -MP
SRC_DIR:= src/
BUILTINS_DIR:=$(SRC_DIR)builtins/
INCLUDES:= include/

LIBFT_DIR := libft/
LIBFT := $(LIBFT_DIR)libft.a 
LIBFT_FLAG := -L $(LIBFT_DIR) $(LIBFT)

HEADERS:= -I $(INCLUDES) -I $(LIBFT_DIR)

all: welcome $(NAME)

$(NAME): $(LIBFT) $(OBJ) 
	$(CC) $(CCFLAGS) $(OBJ) $(LIBFT_FLAG) $(LDFLAGS) -o $(NAME)
	@echo "🌊 Surfing the compilation wave: $(BLUE)$(CC) $(CCFLAGS) $(OBJ) $(LIBFT_FLAG) -o $(NAME)$(DEF_COLOR)"
	@echo "$(GREEN)🏄 Cowabunga! $(NAME) is ready to ride the shell waves! 🏄$(DEF_COLOR)"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(OBJ_DIR)
	@echo "🐚 $(MAGENTA)Collecting seashell: $< $(DEF_COLOR)"
	$(CC) $(CCFLAGS) $(CPPFLAGS) $(HEADERS) -o $@ -c $<

$(OBJ_DIR)builtins/%.o: $(BUILTINS_DIR)%.c
	@mkdir -p $(@D)
	@echo "🐚 $(MAGENTA)Collecting builtin seashell: $< $(DEF_COLOR)"
	$(CC) $(CCFLAGS) $(CPPFLAGS) $(HEADERS) -o $@ -c $<

$(LIBFT): libft

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m

	
welcome:
	@echo "🏖️ $(CYAN)Setting up the beach for $(NAME)... Compilation is about to make waves! $(DEF_COLOR)"

clean:
	@echo "🧼 $(YELLOW)Cleaning up the beach...$(DEF_COLOR)"
	rm -rf $(OBJ_DIR)

fclean: clean
	@echo "$(RED)"
	@echo "        . . . . . . . . . ."
	@echo "      .     *     *     *"
	@echo "   .       *       *       ."
	@echo "         *   SPLASH!   *"
	@echo "      .       *       *       ."
	@echo "    *     *     *     *     *"
	@echo "      ' . . . . . . . . '"
	@echo "$(DEF_COLOR)"
	@echo "$(BLUE)🌊🌊🌊🌊🌊💦 $(NAME) washed away 💦🌊🌊🌊🌊🌊 $(DEF_COLOR)"
	$(MAKE) fclean -C $(LIBFT_DIR)
	rm -f $(NAME)
re: fclean all

info:
	@echo "OBJ": $(OBJ)
	@echo "DEPS": $(DEPS)
	@echo "BUILTINS_SRC = $(BUILTINS_SRC)"


-include $(DEPS)
.PHONY: all clean fclean re libft 