#include "minishell.h"

void	ft_pwd(t_ast *cmd)
{
	char	*path;

	path = getcwd(NULL, PATH_MAX);
	if (!path)
	{
		perror("minishell: pwd");
		cmd->error_code = 1;
		return ;
	}
	printf("%s\n", path);
	free(path);
	cmd->error_code = 0;
}
