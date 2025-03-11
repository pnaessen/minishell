#ifndef PARS_H
# define PARS_H

# include "libft.h"
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

# define ERROR 1
# define SUCCESS 0

typedef enum e_node_type
{
	CMD,
	PIPE,          // |
	AND,           // &&
	OR,            // ||
	REDIR_IN,      // <
	REDIR_OUT,     // >
	REDIR_HEREDOC, // <<
	APPEND         // >>
}					t_node_type;

typedef struct s_stack
{
	t_node_type		token;
	char			**cmd;
	struct s_stack	*next;
	struct s_stack	*prev;
}					t_stack;

// Structure pour stocker une redirection
typedef struct s_redir
{
    t_node_type     type;       // Type de redirection (REDIR_IN, REDIR_OUT, APPEND, REDIR_HEREDOC)
    char            *file;      // Nom du fichier ou délimiteur (pour heredoc)
    struct s_redir  *next;      // Pointeur vers la prochaine redirection pour cette commande
} t_redir;

// parsing
t_stack				*parsing_input(char *input);
t_stack 			*tokenise_args(char *args_cleaned);

/// handle commands
char				*handle_commands(char *args);
char				*separate_commands(char *args, int size);
int					len_to_sep_commands(char *args);
int					handle_operators(char *args, int i);

/// handle whitespace
char				*handle_whitespaces(char *args);
char				*rm_whitespaces(char *args, int size);
int					len_without_whitespaces(char *args);
int					check_num_of_quotes(char *args);
int					add_whitespace(char b, char a, int quotes);

/// parsing_utils
char				**ft_free_all(char **args);
int					ft_iswhitespace(int c);
int					ft_is_operator(int c);
int					ft_is_quotes(char c);
int					handle_quotes(char args, int quotes);

// list_utils
int					fill_the_list(char **cmd, t_stack **stack);
t_stack				*node_init(char **cmd);
void				rot_lstadd_back(t_stack **stack, t_stack *new_node);
void				print_stack(t_stack **stack);

// identify tokens
int					identify_token_type(t_stack **stack);
void				define_type(t_stack *temp, char *cmd, int quotes);

// minishell_split
char				**pre_tokenisation(char const *s);
char				**tokenisation(char const *s);

#endif