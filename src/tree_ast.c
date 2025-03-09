/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tree_ast.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pn <pn@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 09:52:29 by pnaessen          #+#    #+#             */
/*   Updated: 2025/03/09 21:14:28 by pn               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pars.h"

t_ast	*parse_and_build_ast(char *input)
{
	t_stack	*parsed_stack;
	t_ast	*ast_result;

	if (!input || !*input)
		return (NULL);
	parsed_stack = parsing_input(input);
	if (!parsed_stack)
		return (NULL);
	// parsed_stack = link_redi(parsed_stack);
	if (!parsed_stack)
	{
		free_stack(parsed_stack);
		return (NULL);
	}
	ast_result = build_tree(parsed_stack);
	free_stack(parsed_stack);
	return (ast_result);
}

t_ast	*build_tree(t_stack *parsed_stack)
{
	t_stack	*current;
	t_stack	*end;
	t_ast	*root;
	t_ast	*current_node;

	current = parsed_stack;
	end = parsed_stack->prev;
	root = init_first_cmd(parsed_stack, end, &current_node);
	if (!root)
		return (NULL);
	current = current->next;
	return (process_cmd_tokens(current, parsed_stack, root, current_node));
}

t_ast	*process_cmd_tokens(t_stack *current, t_stack *parsed_stack,
		t_ast *root, t_ast *current_node)
{
	while (current != parsed_stack)
	{
		if (current->token == PIPE)
			handle_pipe(&current_node, &current, parsed_stack, &root);
		// else if (is_redirection(current->token))
		// 	handle_redi();
		current = current->next;
	}
	return (root);
}

t_ast	*handle_pipe(t_ast **current_node, t_stack **current, t_stack *stack,
		t_ast **root)
{
	t_ast	*new_cmd;
	t_ast	*pipe_node;
	t_stack	*next_cmd;

	next_cmd = find_next_cmd((*current)->next, stack);
	if (next_cmd == stack)
		return (NULL);
	new_cmd = create_ast_command(next_cmd->cmd);
	if (!new_cmd)
		return (NULL);
	pipe_node = create_pipe_node(*current_node, new_cmd);
	if (!pipe_node)
		return (NULL);
	if (*current_node == *root)
		*root = pipe_node;
	*current_node = pipe_node;
	*current = next_cmd;
	return (pipe_node);
}

t_ast	*init_first_cmd(t_stack *stack, t_stack *end, t_ast **current_node)
{
	t_stack	*current;
	t_ast	*root;

	current = find_next_cmd(stack, end);
	if (current->token != CMD)
		return (NULL);
	root = create_ast_command(current->cmd);
	if (!root)
		return (NULL);
	*current_node = root;
	return (root);
}

////////////////////////////////////////////////////////////////

// int	is_redirection(t_stack *token)
// {
// 	if (token == REDIR_HEREDOC || token == APPEND || token == REDIR_IN
// 		|| token == REDIR_OUT)
// 		return (1);
// 	return (0);
// }