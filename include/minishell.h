/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pnaessen <pnaessen@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/15 15:39:11 by pnaessen          #+#    #+#             */
/*   Updated: 2025/02/18 16:52:27 by pnaessen         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

// typedef struct s_ast
// {
// 	char	**args;
// 	t_ast	*left;
// 	t_ast	*right;
// }			t_ast;

typedef struct s_env
{
	char			*str;
	struct s_env	*next;
}					t_env;

typedef struct s_cmd
{
	char			**args;
	int				argc;
}					t_cmd;

typedef struct s_shell
{
	t_env			*env;
	int				exit_status;
	char			**envp;
}					t_shell;

# include "libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

////////utils_lst////
void				handle_env(char **env, t_env **head);
void				lstadd_back(t_env **head, t_env *new_node);
void				print_stack(t_env **head);

/////////utils/////
int					ft_strcmp(char *src, char *cmp);

////////////builtins////////////////////////
void				check_build_in(char *str);
int					pwd(void);

#endif