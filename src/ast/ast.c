#include "minishell.h"

t_ast	*parse_and_build_ast(char *input, t_env **env, int tmp_error)
{
	t_stack	*parsed_stack;
	t_ast	*ast_result;

	if (!input || !*input)
		return (NULL);
	parsed_stack = parsing_input(input, env, tmp_error);
	if (!parsed_stack)
		return (NULL);
	ast_result = build_tree(parsed_stack);
	free_stack(parsed_stack);
	return (ast_result);
}

void	init_redirection_data(t_ast **redir_node, t_ast **current_node,
		t_stack **current, t_ast **root)
{
	char	*filename;

	filename = (*current)->next->cmd[0];
	if (!filename)
		return ;
	*redir_node = malloc(sizeof(t_ast));
	if (!*redir_node)
	{
		perror("minishell: malloc handle_redirection");
		free_ast(*root);
		*root = NULL;
		return ;
	}
	(*redir_node)->token = (*current)->token;
	(*redir_node)->cmd = malloc(sizeof(t_cmd));
	if (!(*redir_node)->cmd)
	{
		perror("minishell: malloc handle_redirection");
		free(*redir_node);
		free_ast(*root);
		*root = NULL;
		return ;
	}
	init_redir_node(*redir_node, filename, current_node, root);
}

void	handle_redirection(t_ast **current_node, t_stack **current,
		t_ast **root)
{
	t_ast	*redir_node;

	redir_node = NULL;
	init_redirection_data(&redir_node, current_node, current, root);
	if (!redir_node || !redir_node->cmd)
		return ;
	if ((*current)->token == REDIR_HEREDOC && (*current_node)->token != CMD)
		handle_heredoc_case(current_node, root, redir_node);
	else
		handle_standard_case(current_node, root, redir_node);
	*current = (*current)->next;
}

void	init_redir_node(t_ast *redir_node, char *filename, t_ast **current_node,
		t_ast **root)
{
	redir_node->cmd->args = malloc(sizeof(char *) * 2);
	if (!redir_node->cmd->args)
	{
		free(redir_node->cmd);
		free(redir_node);
		free_ast(*root);
		*root = NULL;
		return ;
	}
	redir_node->cmd->args[0] = ft_strdup(filename);
	if (!redir_node->cmd->args[0])
	{
		free(redir_node->cmd->args);
		free(redir_node->cmd);
		free(redir_node);
		free_ast(*root);
		*root = NULL;
		return ;
	}
	redir_node->cmd->args[1] = NULL;
	init_redir_properties(redir_node, *current_node);
}

t_ast	*create_redir_node(t_node_type token, char *filename, t_ast *cmd_node)
{
	t_ast	*redir_node;

	redir_node = malloc(sizeof(t_ast));
	if (!redir_node)
	{
		perror("minishell: malloc create_redir_node");
		return (NULL);
	}
	redir_node->token = token;
	redir_node->cmd = malloc(sizeof(t_cmd));
	if (!redir_node->cmd)
	{
		perror("minishell: malloc create_redir_node");
		free(redir_node);
		return (NULL);
	}
	redir_node = create_redir_args(redir_node, filename);
	if (!redir_node)
		return (NULL);
	init_redir_properties(redir_node, cmd_node);
	return (redir_node);
}
