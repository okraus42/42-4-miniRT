/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   load_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/13 09:18:20 by plouda            #+#    #+#             */
/*   Updated: 2023/11/13 09:20:54 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

int	get_ambient(t_rt *rt, char **split)
{
	//t_ambient	*ambient;
	char		**rgb;

	/* needs protection against:
		- swapped values
		- values not digits (?)
		- only some values (e.g. missing rgb)
		- only some rgb specs (e.g. missing blue)
		- too many decimal places (check before atof)
		- number too big (check before atof)
		- unique properties more than once
	*/

	if (check_format_ambient(split))
		return (1);
	/* ambient = malloc(sizeof(t_ambient));
 	rt->ambient = ambient; */
	rt->ambient->ratio = ft_atof(split[1]);
	if (rt->ambient->ratio < 0.0 || rt->ambient->ratio > 1.0)
	{
		throw_error("A: Ratio out of bounds, expected number between 0.0 and 1.0");
		return (1);
	}
	rgb = ft_split(split[2], ',');
	// rt->ambient->rgb = malloc(sizeof(int) * 3);
	rt->ambient->rgb[R] = ft_atoi(rgb[0]);
	rt->ambient->rgb[G] = ft_atoi(rgb[1]);
	rt->ambient->rgb[B] = ft_atoi(rgb[2]);
	ft_free_split(&rgb);
	if (rt->ambient->rgb[R]> 255 || rt->ambient->rgb[R] < 0
		|| rt->ambient->rgb[G] > 255 || rt->ambient->rgb[G] < 0
		|| rt->ambient->rgb[B] > 255 || rt->ambient->rgb[B] < 0)
	{
		throw_error("A: RGB out of bounds, expected integers between 0 and 255");
		return (1);
	}
	return (0);
}

int	get_camera(t_rt *rt, char **split)
{
	//t_camera	*camera;
	char		**coords;
	char		**nvect;

	if (check_format_camera(split))
		return (1);
	/* camera = malloc(sizeof(t_camera));
	rt->camera = camera; */
	coords = ft_split(split[1], ',');
	//rt->camera->coords = malloc(sizeof(double) * 3);
	rt->camera->coords[X] = ft_atof(coords[0]);
	rt->camera->coords[Y] = ft_atof(coords[1]);
	rt->camera->coords[Z] = ft_atof(coords[2]);
	ft_free_split(&coords);
	nvect = ft_split(split[2], ',');
	//rt->camera->nvect = malloc(sizeof(double) * 3);
	rt->camera->nvect[X] = ft_atof(nvect[0]);
	rt->camera->nvect[Y] = ft_atof(nvect[1]);
	rt->camera->nvect[Z] = ft_atof(nvect[2]);
	ft_free_split(&nvect);
	if (rt->camera->nvect[X] < -1. || rt->camera->nvect[X] > 1.
		|| rt->camera->nvect[Y] < -1. || rt->camera->nvect[Y] > 1.
		|| rt->camera->nvect[Z] < -1. || rt->camera->nvect[Z] > 1.)
	{
		throw_error("C: Vector coordinates out of bounds, expected numbers between [-1.0;1.0]");
		ft_dprintf(2, "%.*CC: Vector coordinates out of bounds, expected numbers between [-1.0;1.0]\n%C", 0xff0000);
		return (1);
	}
	rt->camera->fov = ft_atoi(split[3]);
	if (rt->camera->fov < 0 || rt->camera->fov > 180)
	{
		throw_error("C: FoV coordinates out of bounds, expected a number between [0;180]");
		return (1);
	}
	return (0);
}

int	get_light(t_rt *rt, char **split)
{
	//t_light	*light;
	char	**coords;
	char	**rgb;

	if (check_format_light(split))
		return (1);
	/* light = malloc(sizeof(t_light));
	rt->light = light; */
	coords = ft_split(split[1], ',');
	//rt->light->coords = malloc(sizeof(double) * 3);
	rt->light->coords[X] = ft_atof(coords[0]);
	rt->light->coords[Y] = ft_atof(coords[1]);
	rt->light->coords[Z] = ft_atof(coords[2]);
	ft_free_split(&coords);
	rt->light->brightness = ft_atof(split[2]);
	if (rt->light->brightness < 0.0 || rt->light->brightness > 1.0)
	{
		throw_error("L: Brightness out of bounds, expected number between 0.0 and 1.0");
		return (1);
	}
	rgb = ft_split(split[3], ',');
	//rt->light->rgb = malloc(sizeof(int) * 3);
	rt->light->rgb[R] = ft_atoi(rgb[0]);
	rt->light->rgb[G] = ft_atoi(rgb[1]);
	rt->light->rgb[B] = ft_atoi(rgb[2]);
	ft_free_split(&rgb);
	if (rt->light->rgb[R]> 255 || rt->light->rgb[R] < 0
		|| rt->light->rgb[G] > 255 || rt->light->rgb[G] < 0
		|| rt->light->rgb[B] > 255 || rt->light->rgb[B] < 0)
	{
		throw_error("L: RGB out of bounds, expected integers between 0 and 255");
		return (1);
	}
	return (0);
}

int	add_sphere(t_rt *rt, char **split)
{
	//t_sphere	*sphere;
	char		**coords;
	char		**rgb;
	int			i;

	if (check_format_sphere(split))
		return (1);
	//sphere = malloc(sizeof(t_sphere));
	/* while (rt->spheres[i] != NULL)
		i++; */
	//rt->spheres[i] = sphere;
	i = rt->n_spheres;
	ft_printf("Processing %i. sphere\n", i);
	coords = ft_split(split[1], ',');
	ft_printf("ptr: %p\n", rt->spheres[i]);
	//rt->spheres[i]->coords = malloc(sizeof(double) * 3);
	rt->spheres[i]->coords[X] = ft_atof(coords[0]);
	rt->spheres[i]->coords[Y] = ft_atof(coords[1]);
	rt->spheres[i]->coords[Z] = ft_atof(coords[2]);
	ft_free_split(&coords);
	rt->spheres[i]->diameter = ft_atof(split[2]);
	if (rt->spheres[i]->diameter <= 0)
	{
		throw_error("sp: Nice try. Expected diameter value larger than 0.");
		return (1);
	}
	rgb = ft_split(split[3], ',');
	//rt->spheres[i]->rgb = malloc(sizeof(int) * 3);
	rt->spheres[i]->rgb[R] = ft_atoi(rgb[0]);
	rt->spheres[i]->rgb[G] = ft_atoi(rgb[1]);
	rt->spheres[i]->rgb[B] = ft_atoi(rgb[2]);
	ft_free_split(&rgb);
	if (rt->spheres[i]->rgb[R]> 255 || rt->spheres[i]->rgb[R] < 0
		|| rt->spheres[i]->rgb[G] > 255 || rt->spheres[i]->rgb[G] < 0
		|| rt->spheres[i]->rgb[B] > 255 || rt->spheres[i]->rgb[B] < 0)
	{
		throw_error("sp: RGB out of bounds, expected integers between 0 and 255");
		return (1);
	}
	rt->n_spheres++;
	return (0);
}

int	load_data(char *line, t_rt *rt, int *flag)
{
	char	**split;
	char	*trimmed;
	int		i;

	i = 0;
	trimmed = ft_strtrim(line, " \n");
	split = ft_split(trimmed, ' ');
	free(trimmed);
	if (ft_strlen(split[0]) != 0)
	{
		if (!ft_strncmp(split[0], "A", ft_strlen(split[0])))
			*flag = get_ambient(rt, split);
		else if (!ft_strncmp(split[0], "C", ft_strlen(split[0])))
			*flag = get_camera(rt, split);
		else if (!ft_strncmp(split[0], "L", ft_strlen(split[0])))
			*flag = get_light(rt, split);
		else if (!ft_strncmp(split[0], "sp", ft_strlen(split[0])))
			*flag = add_sphere(rt, split);
		/*
		else if (!ft_strncmp(split[0], "pl", ft_strlen(split[0])))
			add_plane(rt, split);
		else if (!ft_strncmp(split[0], "cy", ft_strlen(split[0])))
			add_cylinder(rt, split); 
		*/
	}
	ft_free_split(&split);
	return (0);
}