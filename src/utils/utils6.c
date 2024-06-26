/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenaddi <hbenaddi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/20 18:43:39 by yzioual           #+#    #+#             */
/*   Updated: 2024/05/23 13:11:31 by yzioual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_atoi(const char *str)
{
	int	i;
	int	result;
	int	sign;

	i = 0;
	result = 0;
	sign = 1;
	while (str[i] == ' ' || str[i] == '\t' \
		|| str[i] == '\n' || str[i] == '\v'
		|| str[i] == '\f' || str[i] == '\r')
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (sign * result);
}

char	*ft_strtok_2(char *str, char *delim)
{
	static char	*save_ptr = NULL;
	char		*start;

	if (str != NULL)
		save_ptr = str;
	else if (save_ptr == NULL)
		return (NULL);
	while (*save_ptr && ft_strchr(delim, *save_ptr))
		save_ptr++;
	if (*save_ptr == '\0')
		return (NULL);
	start = save_ptr;
	while (*save_ptr && !ft_strchr(delim, *save_ptr))
		save_ptr++;
	if (*save_ptr == '\0')
		save_ptr = NULL;
	else
	{
		*save_ptr = '\0';
		save_ptr++;
	}
	return (start);
}

int	is_num(char *str)
{
	if (*str == '-' || *str == '+')
		str++;
	while (*str)
	{
		if (!(*str >= '0' && *str <= '9'))
			return (0);
		str++;
	}
	return (1);
}

int	ft_memcmp(const void *s1, const void *s2, size_t n)
{
	unsigned int	i;
	unsigned char	*source1;
	unsigned char	*source2;

	i = 0;
	source1 = (unsigned char *)s1;
	source2 = (unsigned char *)s2;
	while (i < n)
	{
		if (source1[i] != source2[i])
			return (source1[i] - source2[i]);
		i++;
	}
	return (0);
}

void	helper(char **av, t_stock *stock)
{
	while (*av)
	{
		if (ft_strncmp(*av, "\"", 1) == 0)
			*av = trim_quotes(stock->arena, *av);
		else if (ft_strncmp(*av, "\'", 1) == 0)
			*av = trim_single_quotes(stock->arena, *av);
		av++;
	}
}
