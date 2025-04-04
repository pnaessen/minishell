/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 15:39:11 by pnaessen          #+#    #+#             */
/*   Updated: 2025/04/01 16:17:26 by vicperri         ###   ########lyon.fr   */
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
# define PROCESS_LIMIT 1025

typedef struct s_ast
{
	struct s_cmd		*cmd;
	struct s_ast		*left;
	struct s_ast		*right;
	t_node_type			token;
	struct s_ast		*head;
	int					error_code;
	struct s_ast		*root;
	struct s_fd_garbage	*garbage;
}						t_ast;

typedef struct s_env
{
	char				*str;
	struct s_env		*next;
	int					error_code;
	int					process_count;
}						t_env;

typedef struct s_cmd
{
	char				**args;
	char				*path;
	// t_redir				*redirs;
	int					has_heredoc;
}						t_cmd;

typedef struct s_fd_garbage
{
	int					fd;
	struct s_fd_garbage	*next;
}						t_fd_garbage;

extern int				g_signal_status;

////////////////////////SRC/////////////////////////////////////

///////////////////////AST/////////////////////////////////////

///////////////////////AST_BUILD.C/////////////////////////////
void					set_current_position(t_stack *stack, t_stack *end,
							t_stack **current);
int						process_current_token(t_ast **current_node,
							t_stack **current, t_stack *stack, t_ast **root);
t_ast					*build_tree(t_stack *stack);

///////////////////////AST_CASE.C//////////////////////////////
t_ast					*process_right_side(t_stack *start, t_stack *end,
							t_stack **current);
void					update_current_position(t_stack **current,
							t_stack *cmd_token, t_stack *start, t_stack *end);
t_ast					*process_redirections_ast(t_ast *result,
							t_stack **redir_tokens, int redir_count);
t_ast					*process_output_redir(t_ast *result, t_stack *redir);
t_ast					*process_input_redir(t_ast *result, t_stack *redir);

///////////////////////AST_FIRST_NODE.C////////////////////////
void					init_cmd_true(t_ast **root);
void					init_cmd_node_base(t_ast *root);
t_ast					*check_cmd_after_redir(t_stack *stack, t_stack *end);
t_ast					*init_first_node(t_stack *stack, t_stack *end,
							t_ast **current_node);

///////////////////////AST_FREE.C//////////////////////////////
// void					free_redirections(t_redir *redirs);
void					free_ast_cmd(t_ast *node);
void					free_ast(t_ast *node);
void					free_ast_cmd_args(t_ast *node);

///////////////////////AST_PIPE.C//////////////////////////////
t_ast					*create_cmd_node(t_stack *cmd_token);
int						count_redirections(t_stack *start, t_stack *end);
t_stack					**collect_redirections(t_stack *start, t_stack *end,
							t_stack **cmd_token, int *redir_count);
t_ast					*find_cmd_for_input(t_ast *result);
void					replace_cmd_with_redir(t_ast **result,
							t_ast *target_cmd, t_ast *redir_node);

///////////////////////AST_REDI.C//////////////////////////////
int						init_redir_node_for_pipe(t_ast *node, char *filename);
t_ast					*create_new_redir_node(t_stack *redir_current,
							t_ast *current_node);
t_ast					*process_redirections(t_stack *current, t_stack *stack,
							t_ast *cmd_node, t_stack **last_token);
t_ast					*build_right_side(t_stack *next_cmd, t_stack *stack,
							t_ast *new_cmd, t_stack **last_token);
t_ast					*handle_pipe(t_ast **current_node, t_stack **current,
							t_stack *stack, t_ast **root);

///////////////////////AST_TOOLS.C/////////////////////////////
t_ast					*create_ast_command(char **args);
t_ast					*create_ast_operator(t_node_type token, t_ast *left,
							t_ast *right);
t_ast					*create_pipe_node(t_ast *left_cmd, t_ast *right_cmd);

///////////////////////AST_UTILS.C/////////////////////////////
t_stack					*find_next_cmd(t_stack *current, t_stack *end);
int						count_args(char **args);
t_ast					*init_cmd_node_alloc(t_ast *node, int args_count);
t_ast					*init_cmd_struct(t_ast *node);
t_ast					*init_cmd_node(t_ast *node, char **args,
							int args_count);
char					**create_default_cmd(void);

///////////////////////AST.C//////////////////////////////////
t_ast					*parse_and_build_ast(char *input, t_env **env);
void					handle_redirection(t_ast **current_node,
							t_stack **current, t_ast **root);
void					init_redir_node(t_ast *redir_node, char *filename,
							t_ast **current_node, t_ast **root);
void					init_redir_properties(t_ast *redir_node,
							t_ast *cmd_node);
t_ast					*create_redir_node(t_node_type token, char *filename,
							t_ast *cmd_node);

///////////////////////FREE_TOOLS.C////////////////////////////
t_ast					*free_cmd_node(t_ast *node, int i);
void					free_stack_node(t_stack *node);
void					free_stack(t_stack *stack);
void					free_args_array(char **args);
void					print_env_debug(t_env **head);
int						is_redirection(t_node_type token);

///////////////////////TREE_AST.C//////////////////////////////
t_ast					*init_first_cmd(t_stack *stack, t_stack *end,
							t_ast **current_node);
void					set_root_pointers(t_ast *node, t_ast *root);
void					handle_heredoc_case(t_ast **current_node, t_ast **root,
							t_ast *redir_node);
void					handle_standard_case(t_ast **current_node, t_ast **root,
							t_ast *redir_node);
t_ast					*create_redir_args(t_ast *redir_node, char *filename);

///////////////////////BUILTINS////////////////////////////////

////////////////////////cd_tools.c/////////////////////////////
char					*get_home_var(t_env *env);
int						env_var_exists(t_env *env, char *name);
void					add_to_env(t_env **env, char *new_str);

////////////////////////ft_cd.c////////////////////////////////
void					set_env_var(t_env **env, char *name, char *value);
void					update_env_cd(t_ast *cmd, t_env **env, char *old_dir);
void					handle_cd_path(t_ast *cmd, t_env **env, char *old_dir,
							char *path);
void					ft_cd(t_ast *cmd, t_env **env);

////////////////////////ft_echo.c//////////////////////////////
int						is_valid_flag(char *flag);
void					ft_echo(t_ast *cmd);

////////////////////////ft_env.c///////////////////////////////
void					print_env(t_env **head, t_ast *cmd);

////////////////////////ft_exit.c//////////////////////////////
int						ft_is_valid_number(char *str);
int						handle_numeric_exit(t_ast *cmd, t_env *env, char *arg);
void					ft_exit(t_ast *cmd, t_env *env);

////////////////////////ft_export_tools.c//////////////////////////
void					update_env_var(t_env **env, char *var_str);
void					handle_export_arg(t_env **env, char *arg,
							int *error_code);
int						check_env_match(t_env *current, char *name,
							int name_len);

////////////////////////ft_export.c////////////////////////////
void					update_var_export(t_env *env, char *name, int name_len,
							char *var_str);
int						is_valid_identifier(char *str);
void					ft_export(t_ast *cmd, t_env **env);
void					print_sorted_env(t_env **env);

////////////////////////ft_pwd.c///////////////////////////////
void					ft_pwd(t_ast *cmd);

////////////////////////ft_unset.c/////////////////////////////
int						valid_var(char *str);
void					remove_env_var(t_env **env, char *var_name);
void					ft_unset(t_ast *input, t_env **env);

////////////////////////handle.c///////////////////////////////
void					check_builtin(t_ast *input, t_env *env);

////////////////////////print_export.c/////////////////////////
void					sort_env_array(t_env **sorted, int env_size);
void					print_env_array(t_env **sorted, int size);
void					populate_env_array(t_env **sorted, t_env *env_list);
int						count_env_vars(t_env *env_list);
void					print_env_var(t_env *var);

////////////////////////CORE//////////////////////////////////

////////////////////////mini_env.c////////////////////////////
t_env					*create_env_var(char *var_string);
void					add_pwd_to_env(t_env **head);
void					add_shlvl_to_env(t_env **head);
void					add_underscore_to_env(t_env **head);
void					create_mini_env(t_env **head);

////////////////////////path_env.c/////////////////////////////
char					*get_path(char *cmd, char **env_array);
char					*find_in_path(char *cmd, char **env_array);
char					*try_command_path(char *dir_path, char *cmd);
char					*search_command_in_path(char *cmd, char **path_dirs);
char					**env_to_tab(t_env **env);

////////////////////////signal_tools.c//////////////////////////
void					reset_signals(void);
void					handle_signals_child(void);
void					handle_signals(void);

////////////////////////signal.c///////////////////////////////
void					handle_sig(int sig);
void					handle_signals_heredoc(void);
void					heredoc_sig_handler(int sig);
int						check_heredoc_signals(void);

/////////////////////////tools.c///////////////////////////////
int						can_create_process(t_env *env);
void					process_finished(t_env *env);
void					free_env_fail(char **env_array, int count);
////////////////////////utils_lst.c////////////////////////////
void					handle_env(char **env, t_env **head);
void					lstadd_back(t_env **head, t_env *new_node);
void					free_env_list(t_env *env);
int						count_env_nodes(t_env *env);
////////////////////////utils.c////////////////////////////////
int						ft_strrcmp(char *src, char *cmp);
void					free_env_array(char **env_array);
int						ft_isdigiter(int c);
char					**ft_free_ta(char **tab);

////////////////////////EXEC///////////////////////////////////

////////////////////////apply_redir.c//////////////////////////
int						apply_redirection(t_ast *redir);
int						save_std_fds(int *saved_stdin, int *saved_stdout,
							t_ast *node);
void					restore_std_fds(int saved_stdin, int saved_stdout);

////////////////////////cmd_tools.c////////////////////////////
void					handle_path_error(t_ast *cmd, char **env_array);
char					*prepare_command_path(t_ast *cmd, char **env_array);
char					**prepare_command_args(t_ast *cmd, char **env_array);
void					prepare_execution(t_ast *cmd, char **env_array,
							char **args_copy, char *path_copy);
void					child_process(t_ast *cmd, t_env *env);

////////////////////////command.c//////////////////////////////
char					**create_args_copy(t_ast *cmd, char **env_array);
void					copy_args(t_ast *cmd, char **args_copy,
							char **env_array);
void					execute_command(char *path, char **args,
							char **env_array);
void					handle_command_not_found(t_ast *cmd, char **env_array);

////////////////////////exec_redi.c////////////////////////////
void					exec_with_redirects(t_ast *node, t_env *env);
int						is_cmd_invalid(t_ast *cmd_node);
t_ast					*find_cmd_node(t_ast *node, int *has_error);
void					execute_cmd_with_redir(t_ast *cmd_node, t_ast *node,
							t_env *env);

////////////////////////exec_tools.c//////////////////////////
int						setup_pipe(t_ast *cmd, int pipefd[2]);
void					cleanup_pipe(int pipefd[2]);
void					handle_ast_node(t_ast *node, t_env *env);
void					update_error_codes(t_ast *node, t_env *env);
void					fork_fail(t_ast **cmd, int *pipefd);

////////////////////////exec.c////////////////////////////////
int						parent_process(pid_t pid, t_ast *cmd);
void					execute_cmd(t_ast *cmd_node, t_env *env);
void					execute_cmd_node(t_ast *node, t_env *env);
void					execute_ast(t_ast *node, t_env *env);

// ////////////////////////fd_garbage.c///////////////////////////
void					clean_fd_garbage(t_fd_garbage **head);
void					free_fd_node(t_fd_garbage *node);
void					add_fd_to_garbage(t_fd_garbage **head, int fd);

////////////////////////here_doc.c/////////////////////////////
char					*create_temp_filename(void);
int						write_to_temp_file(char *delimiter, char *filename);
char					*ft_strjoin_free(char *s1, char *s2);
int						setup_heredoc_file(t_ast *node, char *delimiter);
int						process_all_heredocs(t_ast *node);
////////////////////////pipe_left.c////////////////////////////

void					setup_pipe_left(int *pipefd);
int						get_exit_code(t_ast *cmd);
void					clean_and_exit(t_ast *cmd, t_env *env, int exit_code);
void					pipe_child_left(t_ast *cmd, t_env *env, int *pipefd);

////////////////////////pipe_right.c///////////////////////////
void					setup_pipe_right(int *pipefd);
int						get_right_exit_code(t_ast *cmd);
void					pipe_child_right(t_ast *cmd, t_env *env, int *pipefd);

////////////////////////pipe.c////////////////////////////////
int						handle_first_fork(t_ast *cmd, t_env *env, int pipefd[2],
							pid_t *pid1);
int						check_second_process(t_ast *cmd, t_env *env,
							int pipefd[2], pid_t pid1);
int						create_second_fork(t_ast *cmd, t_env *env,
							int pipefd[2], pid_t pid1);
void					execute_pipe(t_ast *cmd, t_env *env);
void					handle_pipe_parent(t_ast *cmd, pid_t pid1, pid_t pid2,
							int *pipefd);

////////////////////////redirection.c//////////////////////////
int						apply_input_redirection(t_ast *redir);
int						apply_output_redirection(t_ast *redir);
int						apply_append_redirection(t_ast *redir);
int						apply_all_redirections(t_ast *node);
void					cleanup_heredoc_files(t_ast *node);

///////////////////////main.c/////////////////////////////////
void					print_ast(t_ast *node, int level);

///////////////////////ENV VIVI/////////////////////////////////
char					*find_and_replace_var(char *args, t_env **env);
t_stack					*parsing_input(char *input, t_env **env);
char					*get_env_value(char *var_name, t_env **env);
char					*replace_without_dollar(char *args, int pos, int quote);
char					*handle_variable_replacement(char *args, int i,
							char quote_type, t_env **env);
char					*replace_without_dollar(char *args, int pos, int quote);
char					*replace_with_empty(char *args, int pos);
int						size_of_exp(t_env **env, char *var_name);

#endif