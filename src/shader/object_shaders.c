/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_shaders.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 17:40:11 by plouda            #+#    #+#             */
/*   Updated: 2024/01/10 13:09:53 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	sphere_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master)
{
	t_sphere	*sphere;
	int			rgb_light_arr[3];
	uint32_t	rgb_light;
	uint32_t	rgb_ambient;
	
	sphere = (t_sphere *)object_ptr;
	if (sphere->mode == HIGHLIGHT)
	{
		rf->clr = 0x00FF00FF;
		return ;
	}
	rf->hit_normal = subtract_vect3f(intersection, array_to_vect(sphere->coords));
	rf->light_dir = subtract_vect3f(array_to_vect(master->rt->light->coords), intersection);
	normalize(&rf->hit_normal);
	normalize(&rf->light_dir);
	rf->light_ratio = dot_product(rf->hit_normal, rf->light_dir);
	clampf(0, 1, &rf->light_ratio);
	get_clr_components(rgb_light_arr, sphere->rgb_light, \
	rf->light_ratio, master->rt->light->brightness);
	rgb_light = get_clr_int(rgb_light_arr);
	rgb_ambient = get_clr_int(sphere->rgb_ambient);
	rf->shadowray.direction = rf->light_dir;
	rf->shadowray.origin = add_vect3f(intersection, scale_vect3f(1e-4, rf->hit_normal));
	rf->light_dist = point_distance(rf->shadowray.origin, array_to_vect(master->rt->light->coords));
	rf->t_near = (double)INT_MAX;
	trace_shadow(master, rf, rgb_ambient, rgb_light);
}

void	plane_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master)
{
	t_plane		*plane;
	int			rgb_light_arr[3];
	uint32_t	rgb_light;
	uint32_t	rgb_ambient;
	
	plane = (t_plane *)object_ptr;
	if (plane->mode == HIGHLIGHT)
	{
		rf->clr = 0x00FF00FF;
		return ;
	}
	rf->hit_normal = *plane->normal;
	rf->light_dir = subtract_vect3f(array_to_vect(master->rt->light->coords), intersection);
	normalize(&rf->light_dir);
	rf->light_ratio = dot_product(rf->hit_normal, rf->light_dir);
	//clampf(0, 1, &rf->light_ratio);
	if (rf->light_ratio < 0)
	{
		rf->light_ratio = rf->light_ratio * -1;
		rf->hit_normal = invert_vect3f(rf->hit_normal); // because shadowray's origin will be shadowed by the plane itself otherwise
	}
	get_clr_components(rgb_light_arr, plane->rgb_light, \
	rf->light_ratio, master->rt->light->brightness);
	rgb_light = get_clr_int(rgb_light_arr);
	rgb_ambient = get_clr_int(plane->rgb_ambient);
	rf->shadowray.direction = rf->light_dir;
	rf->shadowray.origin = add_vect3f(intersection, scale_vect3f(1e-4, rf->hit_normal));
	rf->light_dist = point_distance(rf->shadowray.origin, array_to_vect(master->rt->light->coords));
	rf->t_near = (double)INT_MAX;
	trace_shadow(master, rf, rgb_ambient, rgb_light);
}

void	cylinder_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master, t_ray ray)
{
	t_cylinder	*cylinder;
	int			rgb_light_arr[3];
	uint32_t	rgb_light;
	uint32_t	rgb_ambient;
	
	cylinder = (t_cylinder *)object_ptr;
	if (cylinder->mode == HIGHLIGHT)
	{
		rf->clr = 0x00FF00FF;
		return ;
	}
	rf->hit_normal = get_hit_normal(rf, ray, intersection, *cylinder);
	rf->light_dir = subtract_vect3f(array_to_vect(master->rt->light->coords), intersection);
	normalize(&rf->light_dir);
	rf->light_ratio = dot_product(rf->hit_normal, rf->light_dir);
	clampf(0, 1, &rf->light_ratio);
	get_clr_components(rgb_light_arr, cylinder->rgb_light, \
	rf->light_ratio, master->rt->light->brightness);
	rgb_light = get_clr_int(rgb_light_arr);
	rgb_ambient = get_clr_int(cylinder->rgb_ambient);
	rf->shadowray.direction = rf->light_dir;
	rf->shadowray.origin = add_vect3f(intersection, scale_vect3f(1e-4, rf->hit_normal));
	rf->light_dist = point_distance(rf->shadowray.origin, array_to_vect(master->rt->light->coords));
	rf->t_near = (double)INT_MAX;
	trace_shadow(master, rf, rgb_ambient, rgb_light);
}

void	disc_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr, t_master *master)
{
	t_disc		*disc;
	int			rgb_light_arr[3];
	uint32_t	rgb_light;
	uint32_t	rgb_ambient;
	
	disc = (t_disc *)object_ptr;
	if (disc->mode == HIGHLIGHT)
	{
		rf->clr = 0x00FF00FF;
		return ;
	}
	rf->hit_normal = *disc->normal;
	rf->light_dir = subtract_vect3f(array_to_vect(master->rt->light->coords), intersection);
	normalize(&rf->light_dir);
	rf->light_ratio = dot_product(rf->hit_normal, rf->light_dir);
	clampf(0, 1, &rf->light_ratio);
	get_clr_components(rgb_light_arr, disc->rgb_light, \
	rf->light_ratio, master->rt->light->brightness);
	rgb_light = get_clr_int(rgb_light_arr);
	rgb_ambient = get_clr_int(disc->rgb_ambient);
	rf->shadowray.direction = rf->light_dir;
	rf->shadowray.origin = add_vect3f(intersection, scale_vect3f(1e-4, rf->hit_normal));
	rf->light_dist = point_distance(rf->shadowray.origin, array_to_vect(master->rt->light->coords));
	rf->t_near = (double)INT_MAX;
	trace_shadow(master, rf, rgb_ambient, rgb_light);
}

void	light_shader(t_rayfinder *rf, void *object_ptr, t_master *master)
{
	t_sphere	*sphere;
	int			rgb_light_arr[3];
	
	sphere = (t_sphere *)object_ptr;
	get_clr_components(rgb_light_arr, sphere->rgb, 1, master->rt->light->brightness);
	rf->clr = get_clr_int(rgb_light_arr);
}