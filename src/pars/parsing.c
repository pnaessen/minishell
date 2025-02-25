#include "pars.h"

int	handle_quotes(char argv, int quotes)
{
	if (ft_is_quotes(argv) == SUCCESS)
	{
		if (quotes == SUCCESS)
			quotes = ERROR;
		else
			quotes = SUCCESS;
	}
	return (quotes);
}

int	add_whitespace(char b, char a, int quotes)
{
	if (ft_iswhitespace(b) == SUCCESS && quotes == ERROR)
	{
		if (ft_iswhitespace(a) == ERROR)
			return (SUCCESS);
	}
	return (ERROR);
}

char	*rm_whitespaces(char *argv, int size)
{
	char	*str;
	int		quotes;
	int		i;
	int		j;

	i = 0;
	j = 0;
	quotes = ERROR;
	str = malloc(size * sizeof(char));
	if (!str)
		return (NULL);
	while (argv[i])
	{
		quotes = handle_quotes(argv[i], quotes);
		if (add_whitespace(argv[i], argv[i - 1], quotes) == SUCCESS)
			str[j++] = ' ';
		else if (ft_iswhitespace(argv[i]) == ERROR || quotes == SUCCESS)
			str[j++] = argv[i];
		i++;
	}
	str[j] = '\0';
	return (str);
}

int	handle_operators(char *args, int i)
{
	if (ft_is_operator(args[i]) == SUCCESS)
	{
		if (ft_isalnum(args[i - 1]) == SUCCESS)
			return (SUCCESS);
	}
	else if (ft_isalnum(args[i]) == SUCCESS)
	{
		if (ft_is_operator(args[i - 1]) == SUCCESS && args[i - 1] != '-')
			return (SUCCESS);
	}
	return (ERROR);
}

char	*separate_commands(char *args, int size)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	str = malloc(size * sizeof(char));
	if (!str)
		return (NULL);
	while (args[i])
	{
		if (handle_operators(args, i) == SUCCESS)
		{
			str[j] = ' ';
			j++;
		}
		str[j] = args[i];
		j++;
		i++;
	}
	str[j] = '\0';
	return (str);
}
char	*handle_whitespaces(char *argv)
{
	int		size;
	char	*args;
	char	*args_trim;

	if (check_num_of_quotes(argv) == ERROR)
		return (NULL);
	size = len_without_whitespaces(argv) + 1;
	// printf("[DEBUG] size w/ spaces : [%d]\n", size);
	args = rm_whitespaces(argv, size);
	args_trim = ft_strtrim(args, " ");
	free(args);
	// printf("[DEBUG] args w/ spaces : [%s]\n", args_trim);
	return (args_trim);
}

char	*handle_commands(char *args)
{
	int		size;
	char	*args_cleaned;

	size = len_for_cleaned_args(args) + 1;
	// printf("[DEBUG]size w proper spaces : [%d]\n", size);
	args_cleaned = separate_commands(args, size);
	printf("[DEBUG] args cleaned : [%s]\n", args_cleaned);
	return (args_cleaned);
}

int	parsing_argv(char *input)
{
	char	*args;
	char	*args_cleaned;

	if (!input)
		return (ERROR);
	args = handle_whitespaces(input);
	if (!args)
	{
		printf("[DEBUG] error when handling whitespace\n");
		return (ERROR);
	}
	args_cleaned = handle_commands(args);
	free(args);
	if (!args_cleaned)
	{
		printf("[DEBUG] error when handling commands\n");
		return (ERROR);
	}
	if (split_and_list_args(args_cleaned) == ERROR)
	{
		printf("[DEBUG] error when handling listing\n");
		return (ERROR);
	}
	return (SUCCESS);
}

int	split_and_list_args(char *args_cleaned)
{
	char	**args_split;
	int		i;
	//t_stack	*stack;

	//stack = NULL;
	i = 0;
	args_split = minishell_split(args_cleaned, ' ');
	while (args_split[i])
	{
		printf("args split[%d] : [%s]\n", i, args_split[i]);
		// if (fill_the_list(args_split[i], &stack) == ERROR)
		// {
		// 	ft_free_all(args_split);
		// 	return (ERROR);
		// }
		i++;
	}
//	print_stack(&stack);
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
// 		if (parsing_argv(input) == ERROR)
// 		{
// 			free(input);
// 			return (ERROR);
// 		}
// 		free(input);
// 	}
// 	return (0);
// }
