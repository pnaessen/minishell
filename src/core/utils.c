#include "minishell.h"

int	ft_strrcmp(char *src, char *cmp)
{
	size_t			i;
	unsigned char	*t1;
	unsigned char	*t2;

	t1 = (unsigned char *)src;
	t2 = (unsigned char *)cmp;
	i = 0;
	while (t2[i])
	{
		if (t1[i] != t2[i])
			return (t1[i] - t2[i]);
		i++;
	}
	return (0);
}

void	free_env_array(char **env_array)
{
	int	i;

	if (!env_array)
		return ;
	i = 0;
	while (env_array[i])
	{
		free(env_array[i]);
		i++;
	}
	free(env_array);
	env_array = NULL;
}

int	ft_isdigiter(int c)
{
	if ((c > 47 && c < 58) || c == 61)
	{
		return (1);
	}
	else
		return (0);
}

char	**ft_free_ta(char **tab)
{
	size_t	i;

	i = 0;
	while (tab[i])
	{
		free(tab[i]);
		i++;
	}
	free(tab);
	return (NULL);
}

int	count_args(char **args)
{
	int	args_count;

	args_count = 0;
	while (args[args_count])
		args_count++;
	return (args_count);
}
