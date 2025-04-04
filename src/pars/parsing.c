#include "minishell.h"
#include "pars.h"

int	tokenise_args(char *args_cleaned, t_stack **stack)
{
	char	**token;
	int		i;

	token = pre_tokenisation(args_cleaned);
	i = 0;
	if (!token)
		return (ERROR);
	while (token[i])
	{
		if (fill_the_list(tokenisation(token[i]), stack) == ERROR)
		{
			ft_free_all(token);
			return (ERROR);
		}
		i++;
	}
	if (identify_token_type(stack) == ERROR)
		return (ERROR);
	if (quoting(stack) == ERROR)
		return (ERROR);
	print_stack(stack);
	ft_free_all(token);
	return (SUCCESS);
}

t_stack	*parsing_input(char *input, t_env **env)
{
	char	*args;
	char	*args_cleaned;
	char	*env_handled;
	t_stack	*stack;

	stack = NULL;
	if (!input)
		return (NULL);
	args = handle_whitespaces(input);
	if (!args)
		return (NULL);
	if (ft_strchr(args, '$'))
		env_handled = find_and_replace_var(args, env);
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
		printf("[DEBUG] num of quotes\n");
		free(args_cleaned);
		return (NULL);
	}
	if (tokenise_args(args_cleaned, &stack) == ERROR)
		return (NULL);
	if (!stack)
		return (NULL);
	free(args_cleaned);
	return (stack);
}
