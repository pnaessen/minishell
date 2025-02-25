/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pn <pn@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 15:39:11 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/25 20:50:15 by pn               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# define PATH_MAX 4096

typedef enum e_node_type
{
	CMD,
	PIPE,
	AND,
	OR,
	REDIR_IN,
	REDIR_OUT,
	REDIR_HEREDOC
}					t_node_type;

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

////////utils_lst////
void				handle_env(char **env, t_env **head);
void				lstadd_back(t_env **head, t_env *new_node);

/////////utils/////
int					ft_strcmp(char *src, char *cmp);

////////////builtins////////////////////////
void				check_builtin(t_ast *input, t_env *env);
void				ft_pwd(t_ast *cmd);
void				ft_exit(t_ast *cmd);
void				print_env(t_env **head, t_ast *cmd);
void				ft_cd(t_ast *cmd, t_env **env);

//////////////////signal.c////////////////////////
void				handle_signals(void);
void				handle_sig(int sig);

void				execute_ast(t_ast *cmd, t_env *env);
void				execute_pipe(t_ast *cmd, t_env *env);
void				handle_redir_in(t_ast *cmd, t_env *env);
void				handle_redir_out(t_ast *cmd, t_env *env);
void				execute_command(t_ast *cmd, t_env *env);
void				child_process(t_ast *cmd, char **env_array);
int					parent_process(pid_t pid, t_ast *cmd, char **env_array);
char				**env_to_tab(t_env **env);
char				*get_path(char *cmd, char **env);
char				*get_path_var(char **env);
char				*find_command_path(char **paths, char *cmd);
void				handle_pipe_parent(t_ast *cmd, pid_t pid1, pid_t pid2,
						int *pipefd);
void				pipe_child_right(t_ast *cmd, t_env *env, int *pipefd);
void				pipe_child_left(t_ast *cmd, t_env *env, int *pipefd);
void				ft_unset(t_ast *input, t_env **env);
int					ft_isdigit(char *str);
int					ft_is_valid_number(char *str);

#endif