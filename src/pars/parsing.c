#include "minishell.h"
#include "pars.h"

t_stack	*tokenise_args(char *args_cleaned)
{
	char	**token;
	int		i;
	t_stack	*stack;

	stack = NULL;
	token = pre_tokenisation(args_cleaned);
	i = 0;
	if (!token)
		return (NULL);
	while (token[i])
	{
		if (fill_the_list(tokenisation(token[i]), &stack) == ERROR)
		{
			ft_free_all(token);
			return (NULL);
		}
		i++;
	}
	if (identify_token_type(&stack) == ERROR)
		return (NULL);
	if (quoting(&stack) == ERROR)
		return (NULL);
	print_stack(&stack);
	ft_free_all(token);
	return (stack);
}

t_stack	*parsing_input(char *input, t_env **env)
{
	char	*args;
	char	*args_cleaned;
	char	*env_handled;
	t_stack	*stack;

	if (!input)
		return (NULL);
	args = handle_whitespaces(input);
	if (!args)
		return (NULL);
	printf("[DEBUG] args = %s\n", args);
	if (ft_strchr(args, '$'))
	{
		env_handled = find_and_replace_var(args, env);
		printf("[DEBUG] env = %s\n", env_handled);
	}
	else
		env_handled = ft_strdup(args);
	printf("[DEBUG] env = %s\n", env_handled);
	free(args);
	if (!env_handled)
		return (NULL);
	args_cleaned = handle_commands(env_handled);
	printf("[DEBUG] args cleaned = %s\n", args_cleaned);
	free(env_handled);
	if (!args_cleaned)
		return (NULL);
	if (check_num_of_quotes(args_cleaned) == ERROR)
	{
		free(args_cleaned);
		return (NULL);
	}
	stack = tokenise_args(args_cleaned);
	if (!stack)
		return (NULL);
	return (stack);
}
