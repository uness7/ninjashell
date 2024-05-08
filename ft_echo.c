/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_echo.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hbenaddi <hbenaddi@student.42lehavre.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/22 21:42:27 by hbenaddi          #+#    #+#             */
/*   Updated: 2024/05/04 16:13:45 by yzioual          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	check_n_option(char ***args, char *str, int *all_n)
{
	int	j;

	j = 0;
	if (str[0] == '-' && ft_strlen(str) > 1)
	{
		*all_n = 1;
		j = 1;
		while ((size_t)j < ft_strlen(str))
		{
			if (str[j] != 'n')
			{
				*all_n = 0;
				break ;
			}
			j++;
		}
		if (*all_n)
			(*args)++;
	}
}

static void	print_value_inside_quotes(t_arena *arena, char *str)
{
	char	**subargs;

	subargs = ft_split(arena, trim_quotes(arena, str), ' ');
	while (*subargs)
	{
		if (ft_strstr(*subargs, "$"))
		{
			if (ft_strncmp(*subargs, "$", 2) == 0)
				printf("$\n");
		}
		else
			printf("%s", *subargs);
		if (*(subargs + 1) != NULL)
			printf(" ");
		subargs++;
	}
}

static void	print_value(t_arena *arena, char **args)
{
	while (*args)
	{
		if (ft_strstr(*args, "-"))
			handle_n_arg(*args);
		else if (ft_strncmp(*args, "\"", 1) == 0)
			print_value_inside_quotes(arena, *args);
		else if (ft_strncmp(*args, "\'", 1) == 0)
			printf("%s ", trim_single_quotes(arena, *args));
		else
			printf("%s", trim_quotes(arena, *args));
		args++;
	}
}

#include <stdlib.h>
#include <string.h>

/*
*/
char    **ft_split_echo(char *s)
{
    char    **res;
    int     len = strlen(s);
    int     i = 0;
    int     k = 0;
    int     start = 0;

    res = malloc(sizeof(char *) * (len + 1));
    if (res == NULL)
        return NULL;

    while (s[i])
    {
        if (s[i] == '\"')
        {
            start = ++i;
            while (s[i] && s[i] != '\"')
                i++;
            res[k] = malloc(sizeof(char) * (i - start + 1));
            strncpy(res[k], s + start, i - start);
            res[k++][i - start] = '\0';
            i++;
        }
        else if (s[i] == '\'')
        {
            start = ++i;
            while (s[i] && s[i] != '\'')
                i++;
            res[k] = malloc(sizeof(char) * (i - start + 1));
            strncpy(res[k], s + start, i - start);
            res[k++][i - start] = '\0';
            i++;
        }
        else if (s[i] != ' ')
        {
            start = i;
            while (s[i] && s[i] != ' ' && s[i] != '\'' && s[i] != '\"')
                i++;
            res[k] = malloc(sizeof(char) * (i - start + 1));
            strncpy(res[k], s + start, i - start);
            res[k++][i - start] = '\0';
        }
        else
        {
            i++;
        }
    }

    res[k] = NULL;
    return res;
}

int	ft_echo(t_arena *arena, char *input, char **envp_cp)
{
	char	**args;
	char	**temp;
	int		n_flag;
	int		all_n;

	(void)envp_cp;
	args = ft_split_echo(input);	
	while (*args)
		printf("%s\n", *args++);
//	exit(0);
	n_flag = 0;
	if (ft_strncmp(*args, "-", 1) == 0)
	{
		temp = args;
		temp++;
		while (*temp)
		{
			check_n_option(&args, *temp, &all_n);
			temp++;
		}
		check_n_arg(&n_flag, args);
		if (n_flag == 1)
			args++;
	}
	print_value(arena, args);
	if (n_flag == 0)
		printf("\n");
	return (0);
}
