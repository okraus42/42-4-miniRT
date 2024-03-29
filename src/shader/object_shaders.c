/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_shaders.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 17:40:11 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 15:37:11 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	sphere_shader(t_rayfinder *rf, t_vect3f intersection,
	void *object_ptr, t_master *master)
{
	t_sphere	*sphere;
	t_shader	shader;

	sphere = (t_sphere *)object_ptr;
	shader.intersection = (t_vect3f){intersection.x,
		intersection.y, intersection.z};
	shader.obj_glossiness = sphere->glossiness;
	set_sphere_rgb(&shader, sphere, intersection);
	set_ambient_intensity(&shader, master->rt->ambient->rgb,
		master->rt->ambient->ratio);
	shader.hit_normal = subtract_vect3f(intersection,
			array_to_vect(sphere->coords));
	normalize(&shader.hit_normal);
	set_sphere_normal(&shader, sphere, intersection);
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords),
			intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	if (sphere->camera_inside)
		shader.hit_normal = invert_vect3f(shader.hit_normal);
	shader_loop(&shader, master, intersection, rf);
	shader.pix_color[R] = 255 * shader.illumination[R];
	shader.pix_color[G] = 255 * shader.illumination[G];
	shader.pix_color[B] = 255 * shader.illumination[B];
	rf->clr = get_clr_int(shader.pix_color);
}

void	plane_shader(t_rayfinder *rf, t_vect3f intersection,
	void *object_ptr, t_master *master)
{
	t_plane		*plane;
	t_shader	shader;

	plane = (t_plane *)object_ptr;
	shader.obj_glossiness = plane->glossiness;
	shader.intersection = (t_vect3f){intersection.x,
		intersection.y, intersection.z};
	set_plane_rgb(&shader, plane, intersection);
	set_ambient_intensity(&shader, master->rt->ambient->rgb,
		master->rt->ambient->ratio);
	shader.hit_normal = *plane->normal;
	normalize(&shader.hit_normal);
	set_plane_normal(&shader, plane, intersection);
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords),
			intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	if (plane->camera_inside)
		shader.hit_normal = invert_vect3f(shader.hit_normal);
	shader_loop(&shader, master, intersection, rf);
	shader.pix_color[R] = 255 * shader.illumination[R];
	shader.pix_color[G] = 255 * shader.illumination[G];
	shader.pix_color[B] = 255 * shader.illumination[B];
	rf->clr = get_clr_int(shader.pix_color);
}

void	cylinder_shader(t_rayfinder *rf, t_vect3f intersection,
	void *object_ptr, t_master *master)
{
	t_cylinder	*cylinder;
	t_shader	shader;

	cylinder = (t_cylinder *)object_ptr;
	shader.obj_glossiness = cylinder->glossiness;
	shader.intersection = (t_vect3f){intersection.x,
		intersection.y, intersection.z};
	set_cylinder_rgb(&shader, cylinder, intersection);
	set_ambient_intensity(&shader, master->rt->ambient->rgb,
		master->rt->ambient->ratio);
	shader.hit_normal = get_cylinder_hit_normal(intersection, *cylinder);
	normalize(&shader.hit_normal);
	set_cylinder_normal(&shader, cylinder, intersection);
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords),
			intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	if (cylinder->camera_inside)
		shader.hit_normal = invert_vect3f(shader.hit_normal);
	shader_loop(&shader, master, intersection, rf);
	shader.pix_color[R] = 255 * shader.illumination[R];
	shader.pix_color[G] = 255 * shader.illumination[G];
	shader.pix_color[B] = 255 * shader.illumination[B];
	rf->clr = get_clr_int(shader.pix_color);
}

void	disc_shader(t_rayfinder *rf, t_vect3f intersection, void *object_ptr,
	t_master *master)
{
	t_disc		*disc;
	t_shader	shader;

	disc = (t_disc *)object_ptr;
	shader.obj_glossiness = disc->glossiness;
	shader.intersection = (t_vect3f){intersection.x,
		intersection.y, intersection.z};
	set_disc_rgb(&shader, disc, intersection);
	set_ambient_intensity(&shader, master->rt->ambient->rgb,
		master->rt->ambient->ratio);
	shader.hit_normal = *disc->normal;
	normalize(&shader.hit_normal);
	set_disc_normal(&shader, disc, intersection);
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords),
			intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	if (disc->camera_inside)
		shader.hit_normal = invert_vect3f(shader.hit_normal);
	shader_loop(&shader, master, intersection, rf);
	shader.pix_color[R] = 255 * shader.illumination[R];
	shader.pix_color[G] = 255 * shader.illumination[G];
	shader.pix_color[B] = 255 * shader.illumination[B];
	rf->clr = get_clr_int(shader.pix_color);
}

void	cone_shader(t_rayfinder *rf, t_vect3f intersection,
	void *object_ptr, t_master *master)
{
	t_cone		*cone;
	t_shader	shader;

	cone = (t_cone *)object_ptr;
	shader.obj_glossiness = cone->glossiness;
	shader.intersection = (t_vect3f){intersection.x,
		intersection.y, intersection.z};
	set_cone_rgb(&shader, cone, intersection);
	set_ambient_intensity(&shader, master->rt->ambient->rgb,
		master->rt->ambient->ratio);
	shader.hit_normal = get_cone_hit_normal(intersection, *cone);
	normalize(&shader.hit_normal);
	set_cone_normal(&shader, cone, intersection);
	shader.view_dir = subtract_vect3f(array_to_vect(master->rt->camera->coords),
			intersection);
	normalize(&shader.view_dir);
	normalize(&shader.hit_normal);
	if (cone->camera_inside)
		shader.hit_normal = invert_vect3f(shader.hit_normal);
	shader_loop(&shader, master, intersection, rf);
	shader.pix_color[R] = 255 * shader.illumination[R];
	shader.pix_color[G] = 255 * shader.illumination[G];
	shader.pix_color[B] = 255 * shader.illumination[B];
	rf->clr = get_clr_int(shader.pix_color);
}
