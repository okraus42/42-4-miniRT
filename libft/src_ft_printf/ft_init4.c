/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_init4.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 17:48:46 by okraus            #+#    #+#             */
/*   Updated: 2024/02/28 16:46:13 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/libft.h"

int	ft_init_pointer(char c, t_pf_info *data, t_pf_vargs *varg)
{
	if (data->type_flag & LENGTH_MODIFIER)
		return (1);
	if (c == 's')
	{
		data->type_flag |= LOWERCASE_S;
		data->value_flag = CHAR_POINTER;
		data->value.s = va_arg(varg->arg, char *);
	}
	else if (c == 'p')
	{
		data->type_flag |= LOWERCASE_P;
		data->value_flag = VOID_POINTER;
		data->value.p = va_arg(varg->arg, void *);
	}
	return (0);
}

int	ft_init_conversion(int i, t_pf_info *data, t_pf_vargs *varg)
{
	char	c;
	int		err;

	c = data->orig[i];
	err = 0;
	if (c == 'i' || c == 'd' || c == 'c')
		err = ft_init_int(c, data, varg);
	else if (c == 'o' || c == 'u' || c == 'x' || c == 'X'
		|| c == 'b' || c == 'B')
		err = ft_init_unsigned(c, data, varg);
	else if (c == 'e' || c == 'E' || c == 'f' || c == 'F'
		|| c == 'g' || c == 'G' || c == 'a' || c == 'A')
		err = ft_init_double(c, data, varg);
	else if (data->orig[i] == 's' || data->orig[i] == 'p')
		err = ft_init_pointer(c, data, varg);
	else if (data->orig[i] == 'C')
		data->type_flag |= UPPERCASE_C;
	else if (data->orig[i] == 'P')
		data->type_flag |= UPPERCASE_P;
	else if (data->orig[i] == '%')
		data->type_flag |= PERCENTAGE;
	else
		err = 1;
	return (err);
}

static void	ft_init_baselen(int *i, int *err, t_pf_info *data, t_pf_vargs *varg)
{
	int			n;
	long long	num;

	data->flag |= CIRCUMFLEX;
	++(*i);
	if (data->orig[*i] == '*')
	{
		data->baselen = va_arg(varg->arg, int);
		++(*i);
	}
	else
	{
		n = 0;
		num = ft_latoi(&data->orig[*i]);
		while (ft_isdigit(data->orig[*i]))
		{
			++n;
			++(*i);
		}
		if (n > 15 || num > 0xFFFFFFFFLL)
			*err = 1;
		else if (num > 0)
			data->baselen = (unsigned int)num;
	}
}

static int	ft_init_list_helper(t_pf_vargs *varg,
	t_pf_info *data, int *i, int *err)
{
	if (data->type)
	{
		if (data->orig[*i] && ft_strchr(F_FLAGS, data->orig[*i]))
			ft_init_flags(i, data);
		if (data->orig[*i] && ft_strchr(F_NUMBERS, data->orig[*i]))
			ft_init_field_width(i, err, data, varg);
		if (data->orig[*i] == '.')
		{
			data->flag |= PERIOD;
			++(*i);
			ft_init_precision(i, err, data, varg);
		}
		if (data->orig[*i] == '^')
			ft_init_baselen(i, err, data, varg);
		if (data->orig[*i] && ft_strchr(F_MODIFIER, data->orig[*i]))
			ft_init_modifiers(i, data);
		if (*err || !ft_strchr(F_TYPES, data->orig[*i]))
			return (1);
		if (ft_init_conversion(*i, data, varg))
			return (1);
	}
	return (0);
}

int	ft_init_list(t_pf_vargs *varg, t_list *lst)
{
	t_pf_info	*data;
	int			i;
	int			err;

	err = 0;
	while (lst)
	{
		data = lst->content;
		i = 1;
		if (ft_init_list_helper(varg, data, &i, &err))
			return (1);
		lst = lst->next;
	}
	return (0);
}
