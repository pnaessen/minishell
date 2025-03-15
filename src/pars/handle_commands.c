#include "pars.h"

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

int	len_to_sep_commands(char *args)
{
	int	i;
	int	len;

	i = 0;
	len = 0;
	while (args[i])
	{
		if (handle_operators(args, i) == SUCCESS)
			len++;
		i++;
		len++;
	}
	return (len);
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

char	*handle_commands(char *args)
{
	int		size;
	char	*token;

	size = len_to_sep_commands(args) + 1;
	token = separate_commands(args, size);
	return (token);
}
