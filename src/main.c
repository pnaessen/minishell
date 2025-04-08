#include "minishell.h"

int	main(int argc, char **argv, char **env)
{
	char	*input;
	t_env	*head;
	t_ast	*cmd;
	int		tmp_error;

	tmp_error = 0;
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
			cmd = parse_and_build_ast(input, &head, tmp_error);
			if (cmd)
			{
				execute_ast(cmd, &head);
				tmp_error = cmd->error_code;
				free_ast(cmd);
			}
		}
		free(input);
	}
	return (0);
}

// unset LD_LIBRARY_PATH GLIBCPP_FORCE_NEW PWD GLIBCXX_FORCE_NEW LD_PRELOAD