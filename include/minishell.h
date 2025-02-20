/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 15:39:11 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/20 10:12:52 by pnaessen         ###   ########lyon.fr   */
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
	//char			**envp;
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
int					print_stack(t_env **head);

/////////utils/////
int					ft_strcmp(char *src, char *cmp);
int					execute_command(t_ast *cmd, t_env *env);

////////////builtins////////////////////////
int					check_builtin(t_ast *input, t_env *env);
int					ft_pwd(void);

//////////////////signal.c////////////////////////
void				handle_signals(void);
void				handle_sig(int sig);

#endif