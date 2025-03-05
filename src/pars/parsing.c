#include "pars.h"

int	tokenise_args(char *args_cleaned)
{
	char	**token;
	int		i;
	t_stack	*stack;

	token = pre_tokenisation(args_cleaned);
	stack = NULL;
	i = 0;
	while (token[i])
	{
		if (fill_the_list(tokenisation(token[i]), &stack) == ERROR)
		{
			printf("DEBUG : in fill the lst");
			ft_free_all(token);
			return (ERROR);
		}
		i++;
	}
	identify_token_type(&stack);
	print_stack(&stack);
	return (SUCCESS);
}

int	parsing_input(char *input)
{
	char	*args;
	char	*args_cleaned;

	if (!input)
		return (ERROR);
	args = handle_whitespaces(input);
	if (!args)
		return (ERROR);
	args_cleaned = handle_commands(args);
	free(args);
	if (!args_cleaned)
		return (ERROR);
	if (tokenise_args(args_cleaned) == ERROR)
	{
		printf("DEBUG : tokenisation");
		return (ERROR);
	}
	return (SUCCESS);
}

// int	main(int argc, char **env)
// {
// 	char	*input;

// 	(void)env;
// 	if (argc != 1)
// 	{
// 		printf("Usage : minishell doesn't take argument\n");
// 		return (0);
// 	}
// 	while (1)
// 	{
// 		input = readline("minishell$ ");
// 		if (!input)
// 		{
// 			printf("exit\n");
// 			exit(0);
// 		}
// 		if (*input)
// 			add_history(input);
// 		if (parsing_input(input) == ERROR)
// 		{
// 			free(input);
// 			return (ERROR);
// 		}
// 		free(input);
// 	}
// 	return (0);
// }
