# **************************************************************************** #
#                       🌊 MINISHELL MAKEFILE DELUXE 🌊                      #
# **************************************************************************** #

# ----------------------------- Configuration --------------------------------- #
NAME := minishell
AUTHOR := $(shell whoami)
DATE := $(shell date '+%Y-%m-%d %H:%M:%S')


SRC_DIR := src/
BUILTINS_DIR := $(SRC_DIR)builtins/
PARS_DIR := $(SRC_DIR)pars/
OBJ_DIR := .obj/
BACKUP_DIR := .backups/
INCLUDES := include/
LIBFT_DIR := libft/

# ------------------------------ Source Files --------------------------------- #

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
	ast_utils.c \
	ast_free.c \
	ast_tools.c \
	free_tools.c \
	mini_env.c \
	apply_redi.c \
	exec_redi.c \
	ast.c \
	ast_redi.c \
	pipe_left.c \
	pipe_right.c \
	command.c \
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
	cd_tools.c \
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
	quoting.c \
)

# -------------------------- Combine All Sources ------------------------------ #
SRC += $(BUILTINS_SRC) $(PARS_SRC)

# ------------------------ Object and Dependency Files ----------------------- #
OBJ := $(SRC:%.c=$(OBJ_DIR)%.o)
DEPS := $(OBJ:%.o=%.d)

# -------------------------- Compiler and Flags ------------------------------ #
CC := cc
CFLAGS := -Wextra -Wall -Werror
LDFLAGS := -lreadline
CPPFLAGS := -MMD -MP
HEADERS := -I./$(INCLUDES) -I$(LIBFT_DIR)

# -------------------------------- Libft ------------------------------------- #
LIBFT := $(LIBFT_DIR)libft.a
LIBFT_FLAGS := -L$(LIBFT_DIR) -lft

# -------------------------------- Colors ------------------------------------ #
DEF_COLOR = \033[0;39m
GRAY = \033[0;90m
RED = \033[0;91m
GREEN = \033[0;92m
YELLOW = \033[0;93m
BLUE = \033[0;94m
MAGENTA = \033[0;95m
CYAN = \033[0;96m
WHITE = \033[0;97m
BOLD = \033[1m
UNDERLINE = \033[4m
BLINK = \033[5m

# --------------------------------- Rules ------------------------------------ #

# Default rule
all: welcome dirs $(NAME)
	@printf "\n$(BOLD)$(GREEN)🚀 $(NAME) ready to launch!$(DEF_COLOR)\n"
	@echo "$(CYAN)Type '$(BOLD)./$(NAME)$(DEF_COLOR)$(CYAN)' to start your journey!$(DEF_COLOR)"

# Create necessary directories
dirs:
	@mkdir -p $(OBJ_DIR) $(BACKUP_DIR)
	@mkdir -p $(dir $(OBJ))

# Link final executable
$(NAME): $(LIBFT) $(OBJ)
	@echo "\n$(BOLD)$(YELLOW)🔗 Linking: $(BLUE)$@$(DEF_COLOR)"
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT_FLAGS) -o $(NAME) $(LDFLAGS)
	@echo "$(BOLD)$(GREEN)✅ Built '$(NAME)' successfully!$(DEF_COLOR)"
	@echo "$(GRAY)Compiled on: $(DATE) by $(AUTHOR)$(DEF_COLOR)"

$(OBJ_DIR)%.o: %.c Makefile
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) $(HEADERS) $(CPPFLAGS) -c $< -o $@

$(LIBFT):
	@echo "$(BOLD)$(CYAN)📚 Building libft...$(DEF_COLOR)"
	@$(MAKE) -C $(LIBFT_DIR)

welcome:
	@echo "$(BOLD)$(BLUE)╔═════════════════════════════════════════════════════════════╗$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)║                                                             ║$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)║$(CYAN)                  🐚 MINISHELL PROJECT 🐚                  $(BLUE)║$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)║                                                             ║$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)╚═════════════════════════════════════════════════════════════╝$(DEF_COLOR)"
	@echo ""
	@echo "$(YELLOW)⚡ Starting build process...$(DEF_COLOR)"

clean:
	@echo "$(BOLD)$(YELLOW)🧹 Cleaning up object files...$(DEF_COLOR)"
	@if [ -d "$(OBJ_DIR)" ]; then \
		rm -rf $(OBJ_DIR); \
		echo "$(GREEN)✅ Object files removed!$(DEF_COLOR)"; \
	else \
		echo "$(GRAY)ℹ️  Nothing to clean.$(DEF_COLOR)"; \
	fi
	@$(MAKE) clean -C $(LIBFT_DIR)

fclean: clean
	@echo "$(BOLD)$(RED)🗑️  Deep cleaning...$(DEF_COLOR)"
	@if [ -f "$(NAME)" ]; then \
		rm -f $(NAME); \
		echo "$(GREEN)✅ $(NAME) removed!$(DEF_COLOR)"; \
	fi
	@$(MAKE) fclean -C $(LIBFT_DIR)
	@echo "$(GREEN)✅ All clean!$(DEF_COLOR)"

re: fclean all

valgrind: $(NAME)
	@echo "$(CYAN)🔍 Running Valgrind leak check...$(DEF_COLOR)"
	@valgrind --leak-check=full --suppressions=readline.supp --show-leak-kinds=all --trace-children=yes --track-fds=yes ./$(NAME)

readline.supp:
	@echo "$(YELLOW)Creating readline suppression file...$(DEF_COLOR)"
	@echo "{\n    Readline\n    Memcheck:Leak\n    match-leak-kinds: reachable\n    ...\n    fun:readline\n}" > readline.supp
	@echo "$(GREEN)Created readline.supp file$(DEF_COLOR)"

loc:
	@echo "$(BOLD)$(BLUE)📊 Code Statistics:$(DEF_COLOR)"
	@echo "$(YELLOW)======================$(DEF_COLOR)"
	@echo "$(CYAN)Total files:$(DEF_COLOR) $(words $(SRC))"
	@echo "$(CYAN)Lines of code:$(DEF_COLOR) $$(cat $(SRC) | wc -l)"
	@echo "$(CYAN)Lines by type:$(DEF_COLOR)"
	@echo "  - Main source: $$(cat $(addprefix $(SRC_DIR), *.c) 2>/dev/null | wc -l || echo "0")"
	@echo "  - Builtins: $$(cat $(BUILTINS_SRC) 2>/dev/null | wc -l || echo "0")"
	@echo "  - Parser: $$(cat $(PARS_SRC) 2>/dev/null | wc -l || echo "0")"

# --------------------------- Maintenance Tools ------------------------------ #

# Create a timestamped backup
backup:
	@echo "$(BOLD)$(YELLOW)💾 Creating backup...$(DEF_COLOR)"
	@mkdir -p $(BACKUP_DIR)
	@mkdir -p .backup_temp
	@cp -r src include Makefile libft .backup_temp/
	@tar -czf $(BACKUP_DIR)$(NAME)_$(shell date +%Y%m%d_%H%M%S).tar.gz -C .backup_temp .
	@rm -rf .backup_temp
	@echo "$(GREEN)✅ Backup created in $(BACKUP_DIR)$(DEF_COLOR)"

# Restore the most recent backup
restore:
	@if [ -d "$(BACKUP_DIR)" ] && [ "$$(ls -A $(BACKUP_DIR))" ]; then \
		echo "$(BOLD)$(YELLOW)📂 Restoring latest backup...$(DEF_COLOR)"; \
		LATEST=$$(ls -t $(BACKUP_DIR)*.tar.gz | head -1); \
		mkdir -p .restore_temp; \
		tar -xzf $$LATEST -C .restore_temp; \
		cp -rf .restore_temp/* .; \
		rm -rf .restore_temp; \
		echo "$(GREEN)✅ Restored from $$LATEST$(DEF_COLOR)"; \
	else \
		echo "$(RED)❌ No backups found in $(BACKUP_DIR)$(DEF_COLOR)"; \
	fi

# --------------------------- Information ------------------------------------ #

# Show detailed info about the project
info:
	@echo "$(BOLD)$(CYAN)======================= PROJECT INFO =======================$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)📦 Project: $(WHITE)$(NAME)$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)👤 Author: $(WHITE)$(AUTHOR)$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)📅 Date: $(WHITE)$(DATE)$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)🔧 Compiler: $(WHITE)$(CC) $(CFLAGS)$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)📂 Source files: $(WHITE)$(words $(SRC))$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)📁 Directories:$(DEF_COLOR)"
	@echo "  $(YELLOW)➤ $(WHITE)Sources: $(SRC_DIR)$(DEF_COLOR)"
	@echo "  $(YELLOW)➤ $(WHITE)Objects: $(OBJ_DIR)$(DEF_COLOR)"
	@echo "  $(YELLOW)➤ $(WHITE)Includes: $(INCLUDES)$(DEF_COLOR)"
	@echo "  $(YELLOW)➤ $(WHITE)Libft: $(LIBFT_DIR)$(DEF_COLOR)"
	@echo "$(BOLD)$(CYAN)======================== TARGETS =========================$(DEF_COLOR)"
	@echo "  $(GREEN)make$(DEF_COLOR)            - Build the project"
	@echo "  $(GREEN)make clean$(DEF_COLOR)      - Remove object files"
	@echo "  $(GREEN)make fclean$(DEF_COLOR)     - Remove all generated files"
	@echo "  $(GREEN)make re$(DEF_COLOR)         - Rebuild from scratch"
	@echo "  $(GREEN)make debug$(DEF_COLOR)      - Build with debug symbols"
	@echo "  $(GREEN)make sanitize$(DEF_COLOR)   - Build with sanitizers"
	@echo "  $(GREEN)make optimize$(DEF_COLOR)   - Build with optimization"
	@echo "  $(GREEN)make strict$(DEF_COLOR)     - Build with extra warnings"
	@echo "  $(GREEN)make valgrind$(DEF_COLOR)   - Run with valgrind"
	@echo "  $(GREEN)make gdb$(DEF_COLOR)        - Debug with GDB"
	@echo "  $(GREEN)make backup$(DEF_COLOR)     - Create a backup"
	@echo "  $(GREEN)make restore$(DEF_COLOR)    - Restore from backup"
	@echo "  $(GREEN)make format$(DEF_COLOR)     - Format code (clang-format)"
	@echo "  $(GREEN)make analyze$(DEF_COLOR)    - Static analysis (cppcheck)"
	@echo "  $(GREEN)make docs$(DEF_COLOR)       - Generate documentation"
	@echo "  $(GREEN)make loc$(DEF_COLOR)        - Count lines of code"
	@echo "  $(GREEN)make help$(DEF_COLOR)       - Show available targets"
	@echo "$(BOLD)$(CYAN)=========================================================$(DEF_COLOR)"

# Show help
help:
	@echo "$(BOLD)$(GREEN)🔍 Available commands:$(DEF_COLOR)"
	@echo "$(BOLD)$(CYAN)=========================================================$(DEF_COLOR)"
	@echo "$(BOLD)$(YELLOW)Basic:$(DEF_COLOR)"
	@echo "  $(GREEN)make$(DEF_COLOR)            - Build the project"
	@echo "  $(GREEN)make clean$(DEF_COLOR)      - Remove object files"
	@echo "  $(GREEN)make fclean$(DEF_COLOR)     - Remove all generated files"
	@echo "  $(GREEN)make re$(DEF_COLOR)         - Rebuild from scratch"
	@echo ""
	@echo "$(BOLD)$(YELLOW)Development:$(DEF_COLOR)"
	@echo "  $(GREEN)make debug$(DEF_COLOR)      - Build with debug symbols"
	@echo "  $(GREEN)make sanitize$(DEF_COLOR)   - Build with sanitizers"
	@echo "  $(GREEN)make optimize$(DEF_COLOR)   - Build with optimization"
	@echo "  $(GREEN)make strict$(DEF_COLOR)     - Build with extra warnings"
	@echo ""
	@echo "$(BOLD)$(YELLOW)Testing & Profiling:$(DEF_COLOR)"
	@echo "  $(GREEN)make valgrind$(DEF_COLOR)   - Run with valgrind"
	@echo "  $(GREEN)make gdb$(DEF_COLOR)        - Debug with GDB"
	@echo "  $(GREEN)make profile$(DEF_COLOR)    - Build for profiling"
	@echo "  $(GREEN)make profile-report$(DEF_COLOR) - Generate profiling report"
	@echo ""
	@echo "$(BOLD)$(YELLOW)Maintenance:$(DEF_COLOR)"
	@echo "  $(GREEN)make backup$(DEF_COLOR)     - Create a backup"
	@echo "  $(GREEN)make restore$(DEF_COLOR)    - Restore from backup"
	@echo ""
	@echo "$(BOLD)$(YELLOW)Information:$(DEF_COLOR)"
	@echo "  $(GREEN)make loc$(DEF_COLOR)        - Count lines of code"
	@echo "  $(GREEN)make info$(DEF_COLOR)       - Show detailed project info"
	@echo "$(BOLD)$(CYAN)=========================================================$(DEF_COLOR)"

matrix:
	@clear
	@echo "$(GREEN)MinIsheLl MaTRiX MOdE AcTIvaTEd...$(DEF_COLOR)"
	@sleep 1
	@clear
	@echo "$(GREEN)$(head -c 500 /dev/urandom  )$(DEF_COLOR)"
	@sleep 0.1
	@echo "$(head -c 500 /dev/urandom )"
	@sleep 0.1
	@echo "$(GREEN)$(head -c 500 /dev/urandom  )$(DEF_COLOR)"
	@sleep 0.1
	@echo "$(head -c 500 /dev/urandom )"
	@sleep 0.1
	@clear
	@echo "$(GREEN)$(BOLD)Wake up, Neo...$(DEF_COLOR)"
	@sleep 1
	@clear
	@echo "$(BOLD)$(GREEN)THE MINISHELL HAS YOU...$(DEF_COLOR)"
	@sleep 1

# Generate a fancy status screen with build info
status:
	@clear
	@echo "$(BOLD)$(BLUE)╔═════════════════════════════════════════════════════════════╗$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)║ $(CYAN)                  MINISHELL STATUS REPORT                  $(BLUE)║$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)╠═════════════════════════════════════════════════════════════╣$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)║ $(WHITE)Project:$(DEF_COLOR) $(NAME)                                         $(BLUE)║$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)║ $(WHITE)Author:$(DEF_COLOR) $(AUTHOR)                                        $(BLUE)║$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)║ $(WHITE)Date:$(DEF_COLOR) $(DATE)                         $(BLUE)║$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)╠═════════════════════════════════════════════════════════════╣$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)║ $(WHITE)Source files:$(DEF_COLOR) $(words $(SRC))                                    $(BLUE)║$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)║ $(WHITE)Lines of code:$(DEF_COLOR) $(cat $(SRC) 2>/dev/null | wc -l)                                 $(BLUE)║$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)║ $(WHITE)Build status:$(DEF_COLOR) $(if [ -f "$(NAME)" ]; then echo "$(GREEN)✅ Built"; else echo "$(RED)❌ Not built"; fi)                                 $(BLUE)║$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)║ $(WHITE)Last modified:$(DEF_COLOR) $(stat -c %y $(SRC) 2>/dev/null | sort -r | head -n1 | cut -d. -f1) $(BLUE)║$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)╠═════════════════════════════════════════════════════════════╣$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)║ $(WHITE)Git status:$(DEF_COLOR)                                                $(BLUE)║$(DEF_COLOR)"
	@if command -v git >/dev/null 2>&1 && git rev-parse --is-inside-work-tree >/dev/null 2>&1; then \
		echo "$(BOLD)$(BLUE)║ $(WHITE)Branch:$(DEF_COLOR) $(git branch --show-current)                                      $(BLUE)║$(DEF_COLOR)"; \
		echo "$(BOLD)$(BLUE)║ $(WHITE)Changes:$(DEF_COLOR) $(git status --porcelain | wc -l) files                                  $(BLUE)║$(DEF_COLOR)"; \
	else \
		echo "$(BOLD)$(BLUE)║ $(GRAY)Not a git repository                                    $(BLUE)║$(DEF_COLOR)"; \
	fi
	@echo "$(BOLD)$(BLUE)╚═════════════════════════════════════════════════════════════╝$(DEF_COLOR)"

# Fancy command to run the program with a splash screen
run: $(NAME)
	@clear
	@echo "$(BOLD)$(BLUE)╔═════════════════════════════════════════════════════════════╗$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)║                                                             ║$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)║$(CYAN)                     LAUNCHING MINISHELL                    $(BLUE)║$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)║                                                             ║$(DEF_COLOR)"
	@echo "$(BOLD)$(BLUE)╚═════════════════════════════════════════════════════════════╝$(DEF_COLOR)"
	@sleep 1
	@for i in 3 2 1; do \
		printf "\r$(YELLOW)Launching in $i...$(DEF_COLOR)"; \
		sleep 1; \
	done
	@printf "\r$(GREEN)Here we go!$(DEF_COLOR)       \n"
	@sleep 0.5
	@./$(NAME)

-include $(DEPS)

#valgrind --leak-check=full --suppressions=readline.supp --show-leak-kinds=all --trace-children=yes --track-fds=yes ./minishell

.PHONY: all clean fclean re welcome info help backup restore status run matrix loc valgrind readline.supp 
