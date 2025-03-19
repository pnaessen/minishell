#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	char	*input;
	t_env	*head;
	t_ast	*cmd;

	head = NULL;
	(void)argv;
	if (argc != 1)
	{
		printf("Usage : minishell doesn't take argument\n");
		return (0);
	}
	if (!isatty(STDIN_FILENO) || !isatty(STDOUT_FILENO))
	{
		printf("minishell: cannot be used in pipes or redirections\n");
		return (1);
	}
	handle_signals();
	handle_env(env, &head);
	while (1)
	{
		input = readline("minishell$ ");
		if (!input)
		{
			printf("exit\n");
			free_env_list(head);
			exit(0);
		}
		if (*input)
		{
			add_history(input);
			cmd = parse_and_build_ast(input);
			if (cmd)
			{
				print_ast(cmd, 0);
				execute_ast(cmd, head);
				cleanup_heredoc_files(cmd);
				// printf("%d\n", cmd->error_code);
				free_ast(cmd);
			}
		}
		free(input);
	}
	free_env_list(head);
	return (0);
}

void	print_ast(t_ast *node, int level)
{
	int	i;

	if (!node)
		return ;
	for (i = 0; i < level; i++)
		printf("  ");
	switch (node->token)
	{
	case CMD:
		printf("CMD: ");
		if (node->cmd && node->cmd->args)
		{
			i = 0;
			while (node->cmd->args[i])
			{
				printf("%s", node->cmd->args[i]);
				if (node->cmd->args[i + 1])
					printf(" ");
				i++;
			}
		}
		break ;
	case PIPE:
		printf("PIPE");
		break ;
	case REDIR_IN:
		printf("REDIR_IN: %s", node->cmd->args[0]);
		break ;
	case REDIR_OUT:
		printf("REDIR_OUT: %s", node->cmd->args[0]);
		break ;
	case REDIR_HEREDOC:
		printf("HEREDOC: %s", node->cmd->args[0]);
		break ;
	case APPEND:
		printf("APPEND: %s", node->cmd->args[0]);
		break ;
	default:
		printf("UNKNOWN");
	}
	printf("\n");
	if (node->token == CMD && node->cmd && node->cmd->redirs)
		print_redirections(node->cmd->redirs, level + 1);
	if (node->left)
		print_ast(node->left, level + 1);
	if (node->right)
		print_ast(node->right, level + 1);
}

void	print_redirections(t_redir *redirs, int level)
{
	t_redir	*current;
	int		i;

	current = redirs;
	while (current)
	{
		for (i = 0; i < level; i++)
			printf("  ");
		switch (current->type)
		{
		case REDIR_IN:
			printf("< %s\n", current->file);
			break ;
		case REDIR_OUT:
			printf("> %s\n", current->file);
			break ;
		case REDIR_HEREDOC:
			printf("<< %s\n", current->file);
			break ;
		case APPEND:
			printf(">> %s\n", current->file);
			break ;
		default:
			printf("Unknown redirection: %s\n", current->file);
		}
		current = current->next;
	}
}
