#include "minishell.h"

int	can_create_process(t_env *env)
{
	if (env->process_count >= 1025)
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
