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

typedef struct s_data
{
	int				i;
	int				j;
	int				error_code;
	int				start;
	int				count;
	int				quotes;
	int				quote_num;
	char			quote_type;
	char			*temp;
}					t_data;

typedef enum e_node_type
{
	CMD,
	PIPE,
	AND,
	OR,
	REDIR_IN,
	REDIR_OUT,
	REDIR_HEREDOC,
	APPEND
}					t_node_type;

typedef struct s_stack
{
	t_node_type		token;
	char			**cmd;
	struct s_stack	*next;
	struct s_stack	*prev;
}					t_stack;

// parsing
int					tokenise_args(char *args_cleaned, t_stack **stack);

/// handle commands
char				*handle_commands(char *args);
char				*separate_commands(char *args, int size);
int					len_to_sep_commands(char *args);
int					handle_operators(char *args, int i);

/// handle whitespace
char				*handle_whitespaces(char *args);
char				*rm_whitespaces(char *args, int size);
int					len_without_whitespaces(char *args);
int					add_whitespace(char b, char a, int quotes);

// quoting
int					quoting(t_stack **stack);
char				*handling_quotes(char *args, int size);
int					final_len(char *args);
int					check_num_of_quotes(char *args);

/// parsing_utils
char				**ft_free_all(char **args);
int					ft_iswhitespace(int c);
int					ft_is_operator(int c);
int					ft_is_quotes(char c);
void				check_quotes(char argv, t_data *data);
void				handle_quotes(char argv, t_data *data);

// list_utils
t_stack				*node_init(char **cmd);
int					fill_the_list(char **cmd, t_stack **stack);
void				rot_lstadd_back(t_stack **stack, t_stack *new_node);
void				print_stack(t_stack **stack);

// identify tokens
int					identify_token_type(t_stack **stack);
void				define_type(t_stack *temp, char *cmd, int quotes);
int					check_errors(t_stack **stack);

// minishell_split
char				**pre_tokenisation(char const *s);
char				**tokenisation(char const *s);
char				**split_var(char const *s);

// pre_token_utils
int					handle_multi_operators(const char *s1, int i);
int					is_redirection_operator(const char *s1, int index);
int					is_end_of_word(const char *s1, int index, t_data *data);
int					is_operator_sequence(const char *s1, int index,
						t_data *data);
// replace env
char				*replace_without_dollar(char *args, int pos, int quote);
char				*replace_value(char *args, int pos, char *value,
						char *var_name);
char				*replace_with_empty(char *args, int pos);

// env utils
int					is_valid_var_char(char *args, int i);
int					size_of_var(char *args, int i);
void				size_of_args(char *args, int pos, char *value,
						t_data *data);
char				*extract_variable_name(char *args, int i);
char				*join_tabs(char **tab_args, int space);
int					len_tab_of_tab(char **tab_args);
int					spaces_to_add(const char *s1);
char				*return_env(char *args, char **tab_args);

#endif