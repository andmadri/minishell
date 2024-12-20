/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: maraasve <maraasve@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/16 11:55:32 by marieke           #+#    #+#             */
/*   Updated: 2024/06/28 14:00:20 by maraasve         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../incl/minishell.h"

static int	compare_str_upper(char *str1, char *str2)
{
	int	i;

	i = 0;
	if (!str1 || !str2)
		return (0);
	while (str1[i] && str2[i] && str1[i] != '=' && str2[i] != '=')
	{
		if (ft_toupper(str1[i]) != ft_toupper(str2[i]))
		{
			return ((unsigned char)ft_toupper(str1[i]) \
					- (unsigned char)ft_toupper(str2[i]));
		}
		i++;
	}
	return (0);
}

void	sort_array(char **array)
{
	int		i;
	int		j;
	char	*tmp;

	i = 0;
	while (array[i])
	{
		j = 0;
		while (array[j + 1])
		{
			if (compare_str_upper(array[j], array[j + 1]) > 0)
			{
				tmp = array[j];
				array[j] = array[j + 1];
				array[j + 1] = tmp;
			}
			j++;
		}
		i++;
	}
}

int	ft_env_len(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (i);
	while (str[i] && str[i] != '=')
		i++;
	return (i);
}

int	print_sorted_array(t_command *cmd, char **array)
{
	int	i;
	int	j;

	sort_array(array);
	i = 0;
	while (array[i])
	{
		ft_putstr_fd("declare -x ", cmd->out_fd);
		j = 0;
		while (array[i][j] && array[i][j] != '=')
			ft_putchar_fd(array[i][j++], cmd->out_fd);
		if (array[i][j] == '=')
		{
			ft_putchar_fd('=', cmd->out_fd);
			ft_putchar_fd('\"', cmd->out_fd);
			j++;
			while (array[i][j])
				ft_putchar_fd(array[i][j++], cmd->out_fd);
			ft_putstr_fd("\"", cmd->out_fd);
		}
		ft_putstr_fd("\n", cmd->out_fd);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	ft_unset_export(t_data *data, char *str)
{
	int		length;
	int		j;

	length = 0;
	j = 0;
	while (str[length] && str[length] != '=')
		length++;
	while (data->export_var[j])
	{
		if (ft_strncmp(str, data->export_var[j], length) == 0)
		{
			data->export_var = \
				rearrange_env(data->export_var[j], data->export_var);
			if (!data->export_var)
				return (error_memory_allocation(data, data->cmd_head));
			break ;
		}
		j++;
	}
	return (EXIT_SUCCESS);
}
