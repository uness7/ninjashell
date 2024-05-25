/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_exit.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenaddi <hbenaddi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 13:36:08 by hbenaddi          #+#    #+#             */
/*   Updated: 2024/05/25 20:07:54 by yzioual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_fds(t_stock *stock)
{
	int	i;

	if (stock->last_fd < 3)
		return ;
	i = 3;
	while (i <= stock->last_fd)
	{
		if (close(i) == -1)
			i++;
		i++;
	}
	stock->last_fd = -1;
}

static int	check_number_args(t_stock *stock, int i, int *status)
{
	if (i > 2)
	{
		dprintf(2, " too many arguments\n");
		*status = 1;
		rl_clear_history();
		close_fds(stock);
		exit(*status);
	}
	return (0);
}

void	cleanup(t_stock *stock, int *status)
{
	free_arena(stock->arena);
	free_arena(stock->env_arena);
	rl_clear_history();
	close_fds(stock);
	exit(*status);
}

int	ft_exit(t_stock *stock, char *input, int *status)
{
	int		i;
	char	**args;
	char	*endptr;
	char	*temp;
	long	num;

	i = 0;
	args = convert_list_array(stock->arena, tokenize(stock->arena, input));
	i = array_size(args);
	check_number_args(stock, i, status);
	if (args[1] != NULL)
	{
		temp = trim_quotes(stock->arena, trim_space(args[1]));
		errno = 0;
		num = strtol(temp, &endptr, 10);
		if ((*endptr != '\0' || errno != 0) || num > INT_MAX || num < INT_MIN)
		{
			dprintf(2, " numeric argument required\n");
			*status = 2;
		}
		else
			*status = (int)num;
	}
	cleanup(stock, status);
	return (0);
}
