/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_sphere.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:40:34 by plouda            #+#    #+#             */
/*   Updated: 2024/03/02 15:56:35 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	init_spheres(t_rt *rt, int *ids)
{
	int	i;

	i = 0;
	rt->spheres = ft_calloc(ids[3] + 1, sizeof(t_sphere *));
	rt->n_spheres = 0;
	while (i < (ids[3]))
	{
		rt->spheres[i] = ft_calloc(1, sizeof(t_sphere));
		rt->spheres[i]->coords = ft_calloc(3, sizeof(double));
		rt->spheres[i]->rgb = ft_calloc(3, sizeof(int));
		rt->spheres[i]->normal = ft_calloc(1, sizeof(t_vect3f));
		rt->spheres[i]->right = ft_calloc(1, sizeof(t_vect3f));
		rt->spheres[i]->up = ft_calloc(1, sizeof(t_vect3f));
		rt->spheres[i]->glossiness = 0.5;
		rt->spheres[i]->checkerboard = NULL;
		rt->spheres[i]->texture = NULL;
		rt->spheres[i]->vector_map = NULL;
		rt->spheres[i++]->mode = DEFAULT;
	}
	rt->spheres[i] = NULL;
}

int	check_format_sphere(char **split)
{
	int	spec_count_flag;

	spec_count_flag = has_spec_count(split, 4);
	if (!spec_count_flag)
		return (id_err("sp", E_SPEC, "3"));
	else if (spec_count_flag < 0)
		return (id_err("sp", "Invalid specifier format",
				"unique texture identifiers after the 3rd specifier"));
	if (!is_float_triad(split[1]))
		return (id_err("sp", E_COORD, E_TRIAD_INTFLOAT));
	if (!triad_in_range(split[1]))
		return (id_err("sp", E_COORD, E_MAX_DIGITS));
	if (!is_floatable(split[2]))
		return (id_err("sp", E_DIA, E_INTFLOAT));
	if (!float_in_range(split[2]))
		return (id_err("sp", E_DIA, E_MAX_DIGITS));
	if (!is_rgb_format(split[3]))
		return (id_err("sp", E_RGB, E_TRIAD_INT));
	return (0);
}

int	fill_sphere(t_rt *rt, char **split)
{
	int			i;
	double		diameter;

	i = rt->n_spheres;
	if (check_format_sphere(split))
		return (1);
	get_coords(rt->spheres[i]->coords, split[1]);
	diameter = ft_atof(split[2]);
	if (diameter <= 0)
		return (id_err("sp", E_DIA_RANGE, E_RANGE_STRICT));
	rt->spheres[i]->radius = diameter / 2.;
	if (!get_rgb(rt->spheres[i]->rgb, split[3]))
		return (id_err("sp", E_RGB_RANGE, E_RANGE_INT));
	*rt->spheres[i]->normal = (t_vect3f){0, 0, 1};
	set_sphere_vects(rt->spheres[i]);
	set_checkerboard_pointer("sp", rt, split, &rt->spheres[i]->checkerboard);
	set_texture_pointer("sp", rt, split, &rt->spheres[i]->texture);
	set_vector_map_pointer("sp", rt, split, &rt->spheres[i]->vector_map);
	rt->n_spheres++;
	return (0);
}

void	free_spheres(t_rt *rt)
{
	int	i;

	i = 0;
	while (rt->spheres[i] != NULL)
	{
		free(rt->spheres[i]->coords);
		free(rt->spheres[i]->rgb);
		free(rt->spheres[i]->normal);
		free(rt->spheres[i]->right);
		free(rt->spheres[i]->up);
		free(rt->spheres[i++]);
	}
	free(rt->spheres);
}
