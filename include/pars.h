/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pars.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:05:04 by vicperri          #+#    #+#             */
/*   Updated: 2025/04/11 14:40:54 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

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

///////////////////////parsing////////////////////////////////////

int					tokenise_args(char *args_cleaned, t_stack **stack);

///////////////////////handle commands//////////////////////////////

char				*handle_commands(char *args);
char				*separate_commands(char *args, int size);
int					len_to_sep_commands(char *args);
int					handle_operators(char *args, int i);

//////////////////////handle whitespace//////////////////////////////

char				*handle_whitespaces(char *args);
char				*rm_whitespaces(char *args, int size);
int					len_without_whitespaces(char *args);
int					add_whitespace(char b, char a, int quotes);

////////////////////////////////quoting////////////////////////////////////

int					quoting(t_stack **stack);
char				*handling_quotes(char *args, int size);
int					final_len(char *args);
int					check_num_of_quotes(char *args);

//////////////////////quotes_utils///////////////////////////

int					ft_is_quotes(char c);
int					calculate_len(char *args, t_data *data);
void				check_quotes(char argv, t_data *data);
void				handle_quotes(char argv, t_data *data);

//////////////////////////parsing_utils//////////////////////
char				**ft_free_all(char **args);
int					ft_iswhitespace(int c);
int					ft_is_operator(int c);

///////////////////////////////list_utils////////////////////////////

t_stack				*node_init(char **cmd);
int					fill_the_list(char **cmd, t_stack **stack);
void				rot_lstadd_back(t_stack **stack, t_stack *new_node);

//////////////////////////////identify tokens//////////////////////////

void				define_type(t_stack *temp, char *cmd, int quotes);
int					check_errors(t_stack **stack);
int					loop_through_stack(t_stack *tmp, t_stack *frst);

////////////////////////////////identify_tokens_utils/////////////

int					identify_token_type(t_stack **stack);
int					check_initial_errors(t_stack *temp, t_stack *first);
int					check_redirection_syntax(t_stack *temp);

///////////////////////////////////splits////////////////////////

char				**pre_tokenisation(char const *s);
char				**tokenisation(char const *s);
char				**split_var(char const *s);

////////////////////////////////////pre_token_utils//////////////

int					handle_multi_operators(const char *s1, int i);
int					is_redirection_operator(const char *s1, int index);
int					is_end_of_word(const char *s1, int index, t_data *data);
int					is_operator_sequence(const char *s1, int index,
						t_data *data);
//////////////////////////////////////replace env////////////////

char				*replace_without_dollar(char *args, int pos, int quote);
char				*replace_value(char *args, int pos, char *value,
						char *var_name);
char				*replace_with_empty(char *args, int pos);
void				size_of_args(char *args, int pos, char *value,
						t_data *data);
void				init_data(t_data *data);

////////////////////////////////env utils/////////////////////

int					is_valid_var_char(char *args, int i);
int					size_of_var(char *args, int i);
int					check_heredoc(char **tab);
char				*extract_variable_name(char *args, int i);
void				size_of_empty_args(char *args, int pos, t_data *data);
void				size_of_args(char *args, int pos, char *value,
						t_data *data);

////////////////////////////var_replacement////////////////////

char				*handle_invalid_variable(char *args, int i, char quote);
char				*handle_no_env_case(char *args, t_data *data);
char				*handle_value_replacement(char *args, t_data *data,
						char *var_name, char *value);

/////////////////////////handle_env//////////////

char				*join_tabs(char **tab_args, int space);
char				*return_env(char *args, char **tab_args);
int					len_tab_of_tab(char **tab_args);
int					spaces_to_add(const char *s1);

#endif