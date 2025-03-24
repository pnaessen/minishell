#include "minishell.h"

int	update_existing_var(t_env *temp, char *name_equal, char *value)
{
	char	*new_str;

	if (ft_strncmp(temp->str, name_equal, ft_strlen(name_equal)) == 0)
	{
		new_str = ft_strjoin(name_equal, value);
		if (!new_str)
			return (0);
		free(temp->str);
		temp->str = new_str;
		return (1);
	}
	return (0);
}

void	set_env_var(t_env **env, char *name, char *value)
{
	t_env	*temp;
	char	*name_equal;
	char	*new_str;

	if (!name || !value || !env)
		return ;
	name_equal = ft_strjoin(name, "=");
	if (!name_equal)
		return ;
	temp = *env;
	while (temp && !update_existing_var(temp, name_equal, value))
		temp = temp->next;
	if (!temp)
	{
		new_str = ft_strjoin(name_equal, value);
		if (new_str)
			add_to_env(env, new_str);
	}
	free(name_equal);
}

void	update_env_cd(t_ast *cmd, t_env **env, char *old_dir)
{
	char	*current_dir;

	current_dir = getcwd(NULL, PATH_MAX);
	if (!current_dir)
	{
		perror("minishell: getcwd");
		cmd->error_code = 1;
		free(old_dir);
		return ;
	}
	set_env_var(env, "PWD", current_dir);
	set_env_var(env, "OLDPWD", old_dir);
	free(current_dir);
	free(old_dir);
}

void	handle_cd_path(t_ast *cmd, t_env **env, char *old_dir, char *path)
{
	if (chdir(path) != 0)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": ", 2);
		perror("");
		cmd->error_code = 1;
		free(old_dir);
		return ;
	}
	update_env_cd(cmd, env, old_dir);
	cmd->error_code = 0;
}

void	ft_cd(t_ast *cmd, t_env **env)
{
	char	*path;
	char	*old_dir;

	old_dir = getcwd(NULL, PATH_MAX);
	if (!cmd->cmd->args[1])
	{
		path = get_home_var(*env);
		if (!path)
		{
			ft_putstr_fd("minishell: cd: HOME not set\n", 2);
			cmd->error_code = 1;
			free(old_dir);
			return ;
		}
	}
	else
		path = cmd->cmd->args[1];
	handle_cd_path(cmd, env, old_dir, path);
}
