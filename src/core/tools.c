#include "minishell.h"

int	can_create_process(t_env *env)
{
	if (env->process_count >= PROCESS_LIMIT)
	{
		ft_putstr_fd("minishell: fork: Resource temporarily unavailable\n", 2);
		return (0);
	}
	env->process_count++;
	return (1);
}

void	process_finished(t_env *env)
{
	if (env->process_count > 0)
		env->process_count--;
}

void	free_env_fail(char **env_array, int count)
{
	while (count > 0)
		free(env_array[--count]);
	free(env_array);
}
