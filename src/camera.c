/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   camera.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 12:47:11 by plouda            #+#    #+#             */
/*   Updated: 2023/12/13 09:59:07 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../header/minirt.h"

void	set_camera(t_camera *camera)
{
	t_vect3f	tmp;
	t_vect3f	*forward;
	t_vect3f	*right;
	t_vect3f	*up;

	forward = camera->normal;
	tmp.x = 0;
	tmp.y = 1;
	tmp.z = 0;
	forward->x = camera->normal->x;
	forward->y = camera->normal->y;
	forward->z = camera->normal->z;
	right = camera->right;
	up = camera->up;
	if (forward->x == 0. && forward->y == 1. && forward->z == 0.)
	{
		right->x = 1;
		right->y = 0;
		right->z = 0;
		up->x = 0;
		up->y = 0;
		up->z = 1;
	}
	else if (forward->x == 0. && forward->y == -1 && forward->z == 0.)
	{
		right->x = 1;
		right->y = 0;
		right->z = 0;
		up->x = 0;
		up->y = 0;
		up->z = -1;
	}
	else
	{
		*right = cross_product(tmp, *forward);
		normalize(right, right->x, right->y, right->z);
		*up = cross_product(*forward, *right);
		normalize(up, up->x, up->y, up->z);
	}
}

void	update_camera_matrix(t_camera *camera)
{
	double		**matrix;
	t_vect3f	*forward;

	matrix = camera->matrix;
	forward = camera->normal;
	matrix[0][0] = camera->right->x;
	matrix[0][1] = camera->right->y;
	matrix[0][2] = camera->right->z;
	matrix[0][3] = 0;
	matrix[1][0] = camera->up->x;
	matrix[1][1] = camera->up->y;
	matrix[1][2] = camera->up->z;
	matrix[1][3] = 0;
	matrix[2][0] = forward->x;
	matrix[2][1] = forward->y;
	matrix[2][2] = forward->z;
	matrix[2][3] = 0;
	matrix[3][0] = camera->coords[X];
	matrix[3][1] = camera->coords[Y];
	matrix[3][2] = camera->coords[Z];
	matrix[3][3] = 1;
	printf("WorldToCamera:\n%.5f %.5f %.5f;\n%.5f %.5f %.5f;\n%.5f %.5f %.5f;\n%.5f %.5f %.5f;\n", \
	matrix[0][0], matrix[0][1], matrix[0][2], matrix[1][0], matrix[1][1], matrix[1][2], \
	matrix[2][0], matrix[2][1], matrix[2][2], matrix[3][0], matrix[3][1], matrix[3][2]);
}

/* since origin is 0,0,0, it's just shifting it to "from" point from origin.
Proper matrix multiplication becomes relevant when:
1/ origin is not 0,0,0
2/ homogenous coordinate is not 1 (e.g. after projection matrix)
This could also be encoded in the matrix itself instead of 0,0,0,1
*/
t_vect3f	shift_origin(double **cam)
{
	t_vect3f	origin;

	origin.x = cam[3][0];
	origin.y = cam[3][1];
	origin.z = cam[3][2];
	return (origin);
}

void	change_ray_direction(double **cam, t_vect3f *direction, t_vect3f temp)
{
	direction->x = temp.x * cam[0][0] + temp.y * cam[1][0] + temp.z * cam[2][0];
	direction->y = temp.x * cam[0][1] + temp.y * cam[1][1] + temp.z * cam[2][1];
	direction->z = temp.x * cam[0][2] + temp.y * cam[1][2] + temp.z * cam[2][2];
	normalize(direction, direction->x, direction->y, direction->z);
}

void	tilt(t_camera *camera, double angle)
{
	double	y;
	double	z;

	y = camera->normal->y;
	z = camera->normal->z;
	camera->normal->y = y * cos(rad(angle)) - z * sin(rad(angle));
	camera->normal->z = y * sin(rad(angle)) + z * cos(rad(angle));
	y = camera->up->y;
	z = camera->up->z;
	camera->up->y = y * cos(rad(angle)) - z * sin(rad(angle));
	camera->up->z = y * sin(rad(angle)) + z * cos(rad(angle));
}

void	pan(t_camera *camera, double angle)
{
	double	x;
	double	z;

	x = camera->normal->x;
	z = camera->normal->z;
	camera->normal->z = z * cos(rad(angle)) - x * sin(rad(angle));
	camera->normal->x = z * sin(rad(angle)) + x * cos(rad(angle));
	x = camera->right->x;
	z = camera->right->z;
	camera->right->z = z * cos(rad(angle)) - x * sin(rad(angle));
	camera->right->x = z * sin(rad(angle)) + x * cos(rad(angle));
}

// doesn't work for now
void	cant(t_camera *camera, double angle)
{
	double	x;
	double	y;

	x = camera->up->x;
	y = camera->up->y;
	camera->up->x = x * cos(rad(angle)) - y * sin(rad(angle));
	camera->up->y = x * sin(rad(angle)) + y * cos(rad(angle));
	x = camera->right->x;
	y = camera->right->y;
	camera->right->x = x * cos(rad(angle)) - y * sin(rad(angle));
	camera->right->y = x * sin(rad(angle)) + y * cos(rad(angle));
}

void	rotate_camera(t_master *master, mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_A)
		pan(master->rt->camera, 5);
	if (keydata.key == MLX_KEY_D)
		pan(master->rt->camera, -5);
	if (keydata.key == MLX_KEY_W)
		tilt(master->rt->camera, 5);
	if (keydata.key == MLX_KEY_S)
		tilt(master->rt->camera, -5);
	if (keydata.key == MLX_KEY_Q)
		cant(master->rt->camera, 5);
	if (keydata.key == MLX_KEY_E)
		cant(master->rt->camera, -5);
	normalize(master->rt->camera->normal, master->rt->camera->normal->x, master->rt->camera->normal->y, \
	master->rt->camera->normal->z);
	normalize(master->rt->camera->right, master->rt->camera->right->x, master->rt->camera->right->y, \
	master->rt->camera->right->z);
	normalize(master->rt->camera->up, master->rt->camera->up->x, master->rt->camera->up->y, \
	master->rt->camera->up->z);
	find_rays(master);
}

void	shift_camera(t_master *master, mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_RIGHT)
		master->rt->camera->coords[X] += 5;
	if (keydata.key == MLX_KEY_LEFT)
		master->rt->camera->coords[X] -= 5;
	if (keydata.key == MLX_KEY_UP)
		master->rt->camera->coords[Y] += 5;
	if (keydata.key == MLX_KEY_DOWN)
		master->rt->camera->coords[Y] -= 5;
	if (keydata.key == MLX_KEY_K)
		master->rt->camera->coords[Z] -= 5;
	if (keydata.key == MLX_KEY_L)
		master->rt->camera->coords[Z] += 5;
	if (keydata.key == MLX_KEY_KP_ADD)
		master->rt->camera->fov += 5;
	if (keydata.key == MLX_KEY_KP_SUBTRACT)
		master->rt->camera->fov -= 5;
	find_rays(master);
}
