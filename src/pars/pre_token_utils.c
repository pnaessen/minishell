#include "pars.h"

int	handle_multi_operators(const char *s1, int i)
{
	t_data	data;

	data.count = 0;
	if (ft_is_operator(s1[i]) == SUCCESS)
	{
		while (ft_is_operator(s1[i]) == SUCCESS)
		{
			i++;
			data.count++;
		}
		return (data.count);
	}
	return (SUCCESS);
}

int	is_redirection_operator(const char *s1, int index)
{
	return (index >= 2 && (s1[index - 2] == '<' || s1[index - 2] == '>'));
}

int	is_end_of_word(const char *s1, int index, t_data *data)
{
	return (((ft_is_operator(s1[index]) == ERROR && ft_is_operator(s1[index
					+ 1]) == SUCCESS) || s1[index + 1] == '\0')
		&& data->quotes == ERROR);
}

int	is_operator_sequence(const char *s1, int index, t_data *data)
{
	return (ft_is_operator(s1[index]) == SUCCESS && data->quotes == ERROR);
}