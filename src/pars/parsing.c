#include "minishell.h"
#include "pars.h"

int	tokenise_args(char *args_cleaned, t_stack **stack)
{
	char	**token;
	int		i;
	char	**tmp;

	token = pre_tokenisation(args_cleaned);
	i = 0;
	if (!token)
		return (ERROR);
	while (token[i])
	{
		tmp = tokenisation(token[i]);
		if (!tmp)
		{
			ft_free_all(token);
			return (ERROR);
		}
		if (fill_the_list(tmp, stack) == ERROR)
		{
			ft_free_all(token);
			ft_free_all(tmp);
			return (ERROR);
		}
		i++;
	}
	ft_free_all(token);
	if (identify_token_type(stack) == ERROR)
		return (ERROR);
	if (quoting(stack) == ERROR)
	{
		ft_free_all(tmp);
		return (ERROR);
	}
	if (check_errors(stack) == ERROR)
	{
		ft_free_all(tmp);
		return (ERROR);
	}
	print_stack(stack); // Debugging
	return (SUCCESS);
}

t_stack	*parsing_input(char *input, t_env **env, int tmp_error)
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
	if (check_num_of_quotes(args) == ERROR)
	{
		write(2, "ERROR : quotes\n", 16);
		free(args);
		return (NULL);
	}
	if (ft_strchr(args, '$'))
		env_handled = find_and_replace_var(args, env, tmp_error);
	else
		env_handled = ft_strdup(args);
	free(args);
	if (!env_handled)
		return (NULL);
	args_cleaned = handle_commands(env_handled);
	free(env_handled);
	if (!args_cleaned)
		return (NULL);
	if (tokenise_args(args_cleaned, &stack) == ERROR)
	{
		free(args_cleaned);
		free(stack);
		return (NULL);
	}
	if (!stack)
		return (NULL);
	free(args_cleaned);
	return (stack);
}
