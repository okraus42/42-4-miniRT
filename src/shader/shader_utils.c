/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shader_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/27 17:46:40 by plouda            #+#    #+#             */
/*   Updated: 2024/02/17 16:57:02 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

double	point_distance(t_vect3f p1, t_vect3f p2)
{
	double	dist;

	dist = sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2) + pow(p1.z - p2.z, 2));
	return (dist);
}

void	get_clr_components(int *light, int *rgb, double ratio, double bright)
{
	light[R] = (double)rgb[R] * ratio * bright;
	light[G] = (double)rgb[G] * ratio * bright;
	light[B] = (double)rgb[B] * ratio * bright;
}

uint32_t	get_clr_int(int *rgb)
{
	uint32_t	clr;

	clr = (rgb[R] << 24 | rgb[G] << 16 | rgb[B] << 8 | 0xFF);
	return (clr);
}

uint32_t	ft_max_clr(uint32_t a, uint32_t b)
{
	uint32_t	c;

	c = MAX(a & 0xFF000000, b & 0xFF000000) | MAX(a & 0xFF0000, b & 0xFF0000)
		| MAX(a & 0xFF00, b & 0xFF00) | 0xFF;
	return (c);
}

void	clamp(int min, int max, int *value)
{
	if (*value < min)
		*value = min;
	else if (*value > max)
		*value = max;
}

void	clampf(double min, double max, double *value)
{
	if (*value < min)
		*value = min;
	else if (*value > max)
		*value = max;
}

t_vect3f	get_cylinder_hit_normal(t_rayfinder *rf, t_ray ray, t_vect3f intersection, t_cylinder cylinder)
{

	double t;
	t_vect3f	normal;

	(void)rf;
	(void)ray;
	t = dot_product(*cylinder.normal, subtract_vect3f(intersection, array_to_vect(cylinder.botcap->coords)));
	normal = subtract_vect3f(intersection, scale_vect3f(t, *cylinder.normal));
	normal = subtract_vect3f(normal, array_to_vect(cylinder.botcap->coords));
	normalize(&normal);
	return (normal);
}

t_vect3f	get_cone_hit_normal(t_vect3f intersection, t_cone cone)
{
	double	dist;
	double	half_angle;
	t_vect3f	axis;
	t_vect3f	a;
	double		d;
	t_vect3f	normal;

	axis = invert_vect3f(*cone.normal);
	half_angle = atan2(cone.diameter / 2, cone.height);
	dist = point_distance(intersection, array_to_vect(cone.pinnacle->coords));
	d = dist * sqrt(1 + pow(half_angle, 2));
	a.x = cone.pinnacle->coords[X] + axis.x * d;
	a.y = cone.pinnacle->coords[Y] + axis.y * d;
	a.z = cone.pinnacle->coords[Z] + axis.z * d;
	normal = subtract_vect3f(intersection, a);
	normalize(&normal);
	return (normal);
}

// void	set_ambient_intensity(t_shader *shader, int *ambient_rgb, double ambient_ratio, int *object_rgb)
// {
// 	shader->illumination[R] = ambient_rgb[R] / 255. * ambient_ratio * object_rgb[R] / 255.;
// 	shader->illumination[G] = ambient_rgb[G] / 255. * ambient_ratio * object_rgb[G] / 255.;
// 	shader->illumination[B] = ambient_rgb[B] / 255. * ambient_ratio * object_rgb[B] / 255.;
// }

void	get_plane_uv(t_vect3f p, long long *u, long long *v, int scale[2])
{
	double		uu;
	double		vv;

	uu = p.x / 16;	//to make bigger checkerboard
	vv = p.y / 16;
	// rewrite to get values 0 - (scale[0] - 1)
	*u = (long long)(uu * scale[0] + 2147483648LL); //offset to not get weird stuff around 0
	*v = (long long)(vv * scale[1] + 2147483648LL);
	// to work well on negative coordinates
}

void	set_plane_rgb(t_shader *shader, t_plane *plane, t_vect3f intersection)
{
	long long	u;
	long long	v;
	int			s[2];
	t_vect3f	p;

	if (plane->checkerboard)
	{
		p.x = intersection.x - plane->coords[X];
		p.y = intersection.y - plane->coords[Y];
		p.z = intersection.z - plane->coords[Z];
		s[0] = (int)plane->checkerboard->magnitude;
		s[1] = (int)plane->checkerboard->magnitude;
		get_plane_uv(p, &u, &v, s);
		if (((u & 1) && (v & 1)) || (!(u & 1) && !(v & 1)))
		{
			shader->rgb_object[R] = plane->checkerboard->rgb1[R];
			shader->rgb_object[G] = plane->checkerboard->rgb1[G];
			shader->rgb_object[B] = plane->checkerboard->rgb1[B];
		}
		else
		{
			shader->rgb_object[R] = plane->checkerboard->rgb2[R];
			shader->rgb_object[G] = plane->checkerboard->rgb2[G];
			shader->rgb_object[B] = plane->checkerboard->rgb2[B];
		}
	}
	else
	{
		shader->rgb_object[R] = plane->rgb[R];
		shader->rgb_object[G] = plane->rgb[G];
		shader->rgb_object[B] = plane->rgb[B];
	}
}

void	get_sphere_uv(t_vect3f p, int *u, int *v, int scale[2])
{
	double		uu;
	double		vv;

	normalize(&p);
	uu = 0.5 + (atan2(p.z, p.x) / (6.28318530718));	
	vv = 0.5 + (asin(p.y) / 3.14159265358979);
	*u = (int)(uu * scale[0]);
	*v = (int)(vv * scale[1]);
}

void	set_sphere_rgb(t_shader *shader, t_sphere *sphere, t_vect3f intersection)
{
	int			u;
	int			v;
	int			s[2];
	t_vect3f	p;

	if (sphere->checkerboard)
	{
		p.x = intersection.x - sphere->coords[X];
		p.y = intersection.y - sphere->coords[Y];
		p.z = intersection.z - sphere->coords[Z];
		printf("Q %f %f %f %f\n", sphere->q.q0, sphere->q.q1, sphere->q.q2, sphere->q.q3);
		printf("N %f %f %f \n", sphere->normal->x, sphere->normal->y, sphere->normal->z);
		printf("P1 %f %f %f \n", p.x, p.y, p.z);
		rotate_vect(&p, &sphere->q);
		printf("P2 %f %f %f \n", p.x, p.y, p.z);
		s[0] = (int)sphere->checkerboard->magnitude;
		s[1] = (int)sphere->checkerboard->magnitude;
		get_sphere_uv(p, &u, &v, s);
		if (((u & 1) && (v & 1)) || (!(u & 1) && !(v & 1)))
		{
			shader->rgb_object[R] = sphere->checkerboard->rgb1[R];
			shader->rgb_object[G] = sphere->checkerboard->rgb1[G];
			shader->rgb_object[B] = sphere->checkerboard->rgb1[B];
		}
		else
		{
			shader->rgb_object[R] = sphere->checkerboard->rgb2[R];
			shader->rgb_object[G] = sphere->checkerboard->rgb2[G];
			shader->rgb_object[B] = sphere->checkerboard->rgb2[B];
		}
	}
	else
	{
		shader->rgb_object[R] = sphere->rgb[R];
		shader->rgb_object[G] = sphere->rgb[G];
		shader->rgb_object[B] = sphere->rgb[B];
	}
}

void	get_cone_uv(t_vect3f p, int *u, int *v, double scale[2])
{
	double		uu;
	double		vv;

	vv = (p.z + scale[2] / 2) / scale[2];
	normalize(&p);
	uu = 0.5 + (atan2(p.y, p.x) / (6.28318530718));	
	*u = (int)(uu * scale[0]);
	*v = (int)(vv * scale[1]);
}

void	set_cone_rgb(t_shader *shader, t_cone *cone, t_vect3f intersection)
{
	int			u;
	int			v;
	double		s[3];
	t_vect3f	p;

	if (cone->checkerboard)
	{
		p.x = intersection.x - cone->coords[X];
		p.y = intersection.y - cone->coords[Y];
		p.z = intersection.z - cone->coords[Z];
		s[0] = (int)cone->checkerboard->magnitude;
		s[1] = (int)cone->checkerboard->magnitude;
		s[2] = cone->height;
		printf("%f %f %f\n",intersection.z, p.z, cone->coords[Z]);
		get_cone_uv(p, &u, &v, s);
		if (((u & 1) && (v & 1)) || (!(u & 1) && !(v & 1)))
		{
			shader->rgb_object[R] = cone->checkerboard->rgb1[R];
			shader->rgb_object[G] = cone->checkerboard->rgb1[G];
			shader->rgb_object[B] = cone->checkerboard->rgb1[B];
		}
		else
		{
			shader->rgb_object[R] = cone->checkerboard->rgb2[R];
			shader->rgb_object[G] = cone->checkerboard->rgb2[G];
			shader->rgb_object[B] = cone->checkerboard->rgb2[B];
		}
		// if (intersection.z < 0)
		// {
		// 	shader->rgb_object[R] = 255;
		// 	shader->rgb_object[G] = 0;
		// 	shader->rgb_object[B] = 255;
		// }
	}
	else
	{
		shader->rgb_object[R] = cone->rgb[R];
		shader->rgb_object[G] = cone->rgb[G];
		shader->rgb_object[B] = cone->rgb[B];
	}
}

void	get_disc_uv(t_vect3f p, long long *u, long long *v, double scale[3])
{
	double		uu;
	double		vv;

	uu = (p.x + scale[2]) / (2 * scale[2]);	//to make bigger checkerboard
	vv = (p.y + scale[2]) / (2 * scale[2]);
	*u = (long long)(uu * scale[0]); //offset to not get weird stuff around 0
	*v = (long long)(vv * scale[1]);
	// to work well on negative coordinates
}

void	set_disc_rgb(t_shader *shader, t_disc *disc, t_vect3f intersection)
{
	long long	u;
	long long	v;
	double		s[3];
	t_vect3f	p;

	if (disc->checkerboard)
	{
		p.x = intersection.x - disc->coords[X];
		p.y = intersection.y - disc->coords[Y];
		p.z = intersection.z - disc->coords[Z];
		s[0] = (int)disc->checkerboard->magnitude;
		s[1] = (int)disc->checkerboard->magnitude;
		s[2] = 1 * disc->radius;
		get_disc_uv(p, &u, &v, s);
		if (((u & 1) && (v & 1)) || (!(u & 1) && !(v & 1)))
		{
			shader->rgb_object[R] = disc->checkerboard->rgb1[R];
			shader->rgb_object[G] = disc->checkerboard->rgb1[G];
			shader->rgb_object[B] = disc->checkerboard->rgb1[B];
		}
		else
		{
			shader->rgb_object[R] = disc->checkerboard->rgb2[R];
			shader->rgb_object[G] = disc->checkerboard->rgb2[G];
			shader->rgb_object[B] = disc->checkerboard->rgb2[B];
		}
	}
	else
	{
		shader->rgb_object[R] = disc->rgb[R];
		shader->rgb_object[G] = disc->rgb[G];
		shader->rgb_object[B] = disc->rgb[B];
	}
}

void	get_cylinder_uv(t_vect3f p, int *u, int *v, double scale[2])
{
	double		uu;
	double		vv;

	vv = (p.z + scale[2] / 2) / scale[2];
	normalize(&p);
	uu = 0.5 + (atan2(p.y, p.x) / (6.28318530718));	
	*u = (int)(uu * scale[0]);
	*v = (int)(vv * scale[1]);
}

void	set_cylinder_rgb(t_shader *shader, t_cylinder *cylinder, t_vect3f intersection)
{
	int			u;
	int			v;
	double		s[3];
	t_vect3f	p;

	if (cylinder->checkerboard)
	{
		p.x = intersection.x - cylinder->coords[X];
		p.y = intersection.y - cylinder->coords[Y];
		p.z = intersection.z - cylinder->coords[Z];
		s[0] = (int)cylinder->checkerboard->magnitude;
		s[1] = (int)cylinder->checkerboard->magnitude;
		s[2] = cylinder->height;
		//printf("%f %f %f\n",intersection.z, p.z, cylinder->coords[Z]);
		get_cylinder_uv(p, &u, &v, s);
		if (((u & 1) && (v & 1)) || (!(u & 1) && !(v & 1)))
		{
			shader->rgb_object[R] = cylinder->checkerboard->rgb1[R];
			shader->rgb_object[G] = cylinder->checkerboard->rgb1[G];
			shader->rgb_object[B] = cylinder->checkerboard->rgb1[B];
		}
		else
		{
			shader->rgb_object[R] = cylinder->checkerboard->rgb2[R];
			shader->rgb_object[G] = cylinder->checkerboard->rgb2[G];
			shader->rgb_object[B] = cylinder->checkerboard->rgb2[B];
		}
		// if (intersection.z < 0)
		// {
		// 	shader->rgb_object[R] = 255;
		// 	shader->rgb_object[G] = 0;
		// 	shader->rgb_object[B] = 255;
		// }
	}
	else
	{
		shader->rgb_object[R] = cylinder->rgb[R];
		shader->rgb_object[G] = cylinder->rgb[G];
		shader->rgb_object[B] = cylinder->rgb[B];
	}
}

void	set_ambient_intensity(t_shader *shader, int *ambient_rgb, double ambient_ratio)
{
	shader->illumination[R] = ambient_rgb[R] / 255. * ambient_ratio * shader->rgb_object[R] / 255.;
	shader->illumination[G] = ambient_rgb[G] / 255. * ambient_ratio * shader->rgb_object[G] / 255.;
	shader->illumination[B] = ambient_rgb[B] / 255. * ambient_ratio * shader->rgb_object[B] / 255.;
}