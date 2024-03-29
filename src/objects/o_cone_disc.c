/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_cone_disc.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 10:16:02 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 13:13:53 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	init_cone_discs(t_cone *cone)
{
	cone->base = ft_calloc(1, sizeof(t_disc));
	cone->pinnacle = ft_calloc(1, sizeof(t_disc));
	cone->base->coords = ft_calloc(3, sizeof(double));
	cone->pinnacle->coords = ft_calloc(3, (sizeof(double)));
	cone->base->nvect = ft_calloc(3, sizeof(double));
	cone->pinnacle->nvect = ft_calloc(3, sizeof(double));
	cone->base->rgb = ft_calloc(3, sizeof(int));
	cone->pinnacle->rgb = ft_calloc(3, sizeof(int));
	cone->base->normal = ft_calloc(1, sizeof(t_vect3f));
	cone->pinnacle->normal = ft_calloc(1, sizeof(t_vect3f));
	cone->base->right = ft_calloc(1, sizeof(t_vect3f));
	cone->pinnacle->right = ft_calloc(1, sizeof(t_vect3f));
	cone->base->up = ft_calloc(1, sizeof(t_vect3f));
	cone->pinnacle->up = ft_calloc(1, sizeof(t_vect3f));
	cone->base->mode = DEFAULT;
	cone->pinnacle->mode = DEFAULT;
	cone->pinnacle->checkerboard = NULL;
	cone->base->checkerboard = NULL;
	cone->pinnacle->texture = NULL;
	cone->base->texture = NULL;
	cone->pinnacle->vector_map = NULL;
	cone->base->vector_map = NULL;
}

void	define_cone_base(t_cone *cone)
{
	t_disc	*base;

	base = cone->base;
	base->glossiness = cone->glossiness;
	base->coords[X] = cone->coords[X]
		+ (-1 * cone->normal->x * cone->height / 2);
	base->coords[Y] = cone->coords[Y]
		+ (-1 * cone->normal->y * cone->height / 2);
	base->coords[Z] = cone->coords[Z]
		+ (-1 * cone->normal->z * cone->height / 2);
	base->nvect[X] = cone->normal->x * -1;
	base->nvect[Y] = cone->normal->y * -1;
	base->nvect[Z] = cone->normal->z * -1;
	base->rgb[R] = cone->rgb[R];
	base->rgb[G] = cone->rgb[G];
	base->rgb[B] = cone->rgb[B];
	base->radius = cone->radius;
	*base->normal = get_normal(base->nvect[X],
			base->nvect[Y], base->nvect[Z]);
	set_disc_vects(base);
	attribute_texture_data_cone(base, cone);
}

void	define_cone_pinnacle(t_cone *cone)
{
	t_disc	*pinnacle;

	pinnacle = cone->pinnacle;
	pinnacle->glossiness = cone->glossiness;
	pinnacle->coords[X] = cone->coords[X]
		+ (cone->normal->x * cone->height / 2);
	pinnacle->coords[Y] = cone->coords[Y]
		+ (cone->normal->y * cone->height / 2);
	pinnacle->coords[Z] = cone->coords[Z]
		+ (cone->normal->z * cone->height / 2);
	pinnacle->nvect[X] = cone->normal->x;
	pinnacle->nvect[Y] = cone->normal->y;
	pinnacle->nvect[Z] = cone->normal->z;
	pinnacle->rgb[R] = cone->rgb[R];
	pinnacle->rgb[G] = cone->rgb[G];
	pinnacle->rgb[B] = cone->rgb[B];
	cone->pinnacle->radius = cone->radius;
	*pinnacle->normal = get_normal(pinnacle->nvect[X],
			pinnacle->nvect[Y], pinnacle->nvect[Z]);
	set_disc_vects(pinnacle);
	attribute_texture_data_cone(pinnacle, cone);
}

void	get_cone_discs(t_cone *cone)
{
	define_cone_base(cone);
	define_cone_pinnacle(cone);
}

void	free_cone_discs(t_cone *cone)
{
	free(cone->base->coords);
	free(cone->base->nvect);
	free(cone->base->rgb);
	free(cone->base->normal);
	free(cone->base->right);
	free(cone->base->up);
	free(cone->base);
	free(cone->pinnacle->coords);
	free(cone->pinnacle->nvect);
	free(cone->pinnacle->rgb);
	free(cone->pinnacle->normal);
	free(cone->pinnacle->right);
	free(cone->pinnacle->up);
	free(cone->pinnacle);
}
