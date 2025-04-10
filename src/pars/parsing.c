/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vicperri <vicperri@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/10 15:03:02 by vicperri          #+#    #+#             */
/*   Updated: 2025/04/10 15:03:04 by vicperri         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include "pars.h"

static int	process_tokens(char **token, t_stack **stack)
{
	int		i;
	char	**tmp;

	i = 0;
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
	return (SUCCESS);
}

int	tokenise_args(char *args_cleaned, t_stack **stack)
{
	char	**token;

	token = pre_tokenisation(args_cleaned);
	if (!token)
		return (ERROR);
	if (process_tokens(token, stack) == ERROR)
		return (ERROR);
	if (identify_token_type(stack) == ERROR)
		return (ERROR);
	if (quoting(stack) == ERROR)
		return (ERROR);
	if (check_errors(stack) == ERROR)
		return (ERROR);
	return (SUCCESS);
}

t_stack	*parsing_input(char *input, t_env **env, int tmp_error)
{
	char	*args_cleaned;
	t_stack	*stack;
	int		token_status;

	stack = NULL;
	args_cleaned = prepare_args(input, env, tmp_error);
	if (!args_cleaned)
		return (NULL);
	token_status = tokenise_args(args_cleaned, &stack);
	if (token_status == ERROR || !stack)
	{
		free(args_cleaned);
		return (NULL);
	}
	free(args_cleaned);
	return (stack);
}

static char	*handle_args(char *input, t_env **env, int tmp_error)
{
	char	*args;
	char	*env_handled;

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
	return (env_handled);
}

char	*prepare_args(char *input, t_env **env, int tmp_error)
{
	char	*env_handled;
	char	*args_cleaned;

	if (!input)
		return (NULL);
	env_handled = handle_args(input, env, tmp_error);
	if (!env_handled)
		return (NULL);
	args_cleaned = handle_commands(env_handled);
	free(env_handled);
	if (!args_cleaned)
		return (NULL);
	return (args_cleaned);
}
