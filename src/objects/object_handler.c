/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_handler.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/20 15:51:52 by plouda            #+#    #+#             */
/*   Updated: 2024/03/07 14:28:30 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

int	*init_ids(void)
{
	int		*ids;
	int		i;

	i = 0;
	ids = malloc(sizeof(int) * 11);
	while (i < 11)
		ids[i++] = 0;
	return (ids);
}

void	init_objects(t_rt *rt, int *ids)
{
	init_ambient(rt);
	init_light_sphere(rt, ids);
	init_camera(rt);
	init_spheres(rt, ids);
	init_planes(rt, ids);
	init_cylinders(rt, ids);
	init_cones(rt, ids);
	init_checkerboards(rt, ids);
	init_textures(rt, ids);
	init_vector_maps(rt, ids);
	init_skybox(rt);
}

void	free_objects(t_rt *rt)
{
	free_ambient(rt);
	free_camera(rt);
	free_spheres(rt);
	free_planes(rt);
	free_cylinders(rt);
	free_cones(rt);
	free_light_sphere(rt);
	free_checkerboards(rt);
	free_textures(rt);
	free_vector_maps(rt);
	free_skybox(rt);
}
