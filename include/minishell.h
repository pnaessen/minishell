/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 15:39:11 by pnaessen          #+#    #+#             */
/*   Updated: 2025/03/07 12:18:52 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include "pars.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# define PATH_MAX 4096

typedef struct s_ast
{
	struct s_cmd	*cmd;
	struct s_ast	*left;
	struct s_ast	*right;
	t_node_type		token;
	struct s_ast	*head;
	int				error_code;
}					t_ast;

typedef struct s_env
{
	char			*str;
	struct s_env	*next;
}					t_env;

typedef struct s_cmd
{
	char			**args;
	char			*path;
}					t_cmd;

////////////////////////SRC/////////////////////////////////////

/////////////tree_ast//////////////////////////////
t_ast				*create_ast_command(char **args);
t_ast				*init_cmd_node(t_ast *node, char **args, int args_count);
t_ast				*free_cmd_node(t_ast *node, int i);
t_ast				*create_ast_operator(t_node_type token, t_ast *left,
						t_ast *right);
t_stack				*find_next_cmd(t_stack *current);
t_ast				*parse_and_build_ast(char *input);
t_ast				*create_command_and_continue(t_stack *cmd_token,
						t_ast *left);
t_ast				*process_pipe_node(t_stack *current, t_ast *left_ast);
t_ast				*build_pipe_tree(t_stack *stack);
void				free_ast_cmd(t_ast *node);
void				free_ast(t_ast *node);

///////////exec.c///////////////////////
void				execute_cmd(t_ast *cmd, t_env *env);
void				child_process(t_ast *cmd, char **env_array);
int					parent_process(pid_t pid, t_ast *cmd, char **env_array);
void				execute_ast(t_ast *cmd, t_env *env);

////////////here_doc////////////////////////////////
char				*create_temp_filename(void);
int					write_to_temp_file(char *delimiter, char *filename);
void				execute_with_heredoc(t_ast *cmd, t_env *env,
						char *filename);
void				handle_heredoc(t_ast *cmd, t_env *env);
char				*ft_strjoin_free(char *s1, char *s2);
///////////path_env.c///////////////////
char				*get_path(char *cmd, char **env_array);
char				*find_in_path(char *cmd, char **env_array);
char				*search_command_in_path(char *cmd, char **path_dirs);
char				**env_to_tab(t_env **env);

///////////pipe.c///////////////////////
void				pipe_child_left(t_ast *cmd, t_env *env, int *pipefd);
void				pipe_child_right(t_ast *cmd, t_env *env, int *pipefd);
void				execute_pipe(t_ast *cmd, t_env *env);
void				handle_pipe_parent(t_ast *cmd, pid_t pid1, pid_t pid2,
						int *pipefd);

////////////redirection.c///////////////////
void				handle_redir_in(t_ast *cmd, t_env *env);
void				handle_redir_out(t_ast *cmd, t_env *env);
void				handle_redir_append(t_ast *cmd, t_env *env);

//////////////////signal.c////////////////////////
void				handle_signals(void);
void				handle_sig(int sig);

////////utils_lst////
void				handle_env(char **env, t_env **head);
void				lstadd_back(t_env **head, t_env *new_node);
void				free_env_list(t_env *env);
void				creat_mini_env(t_env **head);

/////////utils/////
int					ft_strcmp(char *src, char *cmp);
void				free_env_array(char **env_array);
int					ft_isdigiter(int c);
char				**ft_free_ta(char **tab);

////////////BUILTINS////////////////////////

///////////////ft_cd.c/////////////////////
char				*get_home_var(t_env *env);
int					env_var_exists(t_env *env, char *name);
void				add_to_env(t_env **env, char *new_str);
void				set_env_var(t_env **env, char *name, char *value);
void				update_env_cd(t_ast *cmd, t_env **env, char *old_dir);
void				ft_cd(t_ast *cmd, t_env **env);

///////////////ft_echo.c/////////////////////
int					is_valid_flag(char *flag);
void				ft_echo(t_ast *cmd);

////////////////ft_env.c///////////////////////
void				print_env(t_env **head, t_ast *cmd);
void				print_env_debug(t_env **head);

/////////////////ft_exit.c////////////////////
void				ft_exit(t_ast *cmd, t_env *env);
int					ft_is_valid_number(char *str);

////////////////ft_pwd.c/////////////////////
void				ft_pwd(t_ast *cmd);

////////////////ft_unset.c////////////////////
int					valid_var(char *str);
void				ft_unset(t_ast *input, t_env **env);

//////////////////export.c/////////////////////
void				ft_export(t_ast *cmd, t_env **env);
int					is_valid_identifier(char *str);
void				update_env_var(t_env **env, char *var_str);

/////////////////////print_export.c/////////////////////
void				print_env_var(t_env *var);
void				print_sorted_env(t_env **env);
void				sort_env_array(t_env **sorted, int env_size);

///////////////handle.c//////////////////////
void				check_builtin(t_ast *input, t_env *env);

////////////////main.c/////////////////////////
t_ast				*create_test_command(char *cmd_str);
void				free_ast(t_ast *node);
t_ast				*create_command_pipeline(char **cmds, int count);
t_ast				*create_test_pipeline(char *cmds);
void				print_ast(t_ast *node, int level);

#endif