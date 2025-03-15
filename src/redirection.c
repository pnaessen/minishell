#include "minishell.h"



// void	exec_with_redirects(t_ast *node, t_env *env)
// {
// 	int	saved_stdin;
// 	int	saved_stdout;

// 	if (save_std_fds(&saved_stdin, &saved_stdout, node))
// 		return ;
// 	if (apply_all_redirections(node->cmd))
// 	{
// 		node->error_code = 1;
// 		restore_std_fds(saved_stdin, saved_stdout);
// 		return ;
// 	}
// 	check_builtin(node, env);
// 	restore_std_fds(saved_stdin, saved_stdout);
// 	if (node->error_code == -1)
// 		execute_cmd(node, env);
// }



// int	handle_child_redirects(t_ast *cmd_node, char **env_array)
// {
// 	if (cmd_node->cmd->redirs)
// 	{
// 		if (apply_all_redirections(cmd_node->cmd))
// 		{
// 			ft_free_ta(env_array);
// 			exit(1);
// 		}
// 	}
// 	child_process(cmd_node, env_array);
// 	return (0);
// }
