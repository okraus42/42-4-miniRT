/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   o_vector_map.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: plouda <plouda@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/19 09:47:41 by plouda            #+#    #+#             */
/*   Updated: 2024/03/05 13:46:31 by plouda           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../header/minirt.h"

void	init_vector_maps(t_rt *rt, int *ids)
{
	int	i;

	i = 0;
	rt->vector_maps = ft_calloc(ids[9] + 1, sizeof(t_vector_map *));
	rt->n_vector_maps = 0;
	while (i < (ids[9]))
	{
		rt->vector_maps[i] = ft_calloc(1, sizeof(t_vector_map));
		i++;
	}
	rt->vector_maps[i] = NULL;
}

int	check_format_vector_map(char **split)
{
	int	i;
	int	spec_count;

	i = 1;
	spec_count = 0;
	if (!has_spec_count_strict(split, 4)
		&& !has_spec_count_strict(split, 3) && !has_spec_count_strict(split, 2))
		return (id_err(".vm", E_SPEC, "up to 3"));
	if (!has_valid_id_attribute(split[0]))
		return (id_err(".vm", "Invalid vector_map ID", NULL));
	while (split[spec_count])
		spec_count++;
	while (i < spec_count)
	{
		if (!contains_valid_key_value_pair(split[i]))
			return (id_err(".vm", "invalid key-value pair",
					"key{0,1,2}=\"value\""));
		if (!is_valid_texture_file(split[i]))
			return (id_err(".vm", "Invalid vector_map file", NULL));
		i++;
	}
	return (0);
}

void	ft_load_vector_map(t_vector_map *vm)
{
	if (vm->vm_path[0])
	{
		vm->vm_main = mlx_load_png(vm->vm_path);
		if (!vm->vm_main)
			id_warn(".vm", "Cannot read vector map file under", vm->vm_path, 0);
	}
	if (vm->topcap_vm_path[0])
	{
		vm->vm_top = mlx_load_png(vm->topcap_vm_path);
		if (!vm->vm_top)
		{
			id_warn(".vm", "Cannot read vector map file under",
				vm->topcap_vm_path, 0);
		}
	}
	if (vm->botcap_vm_path[0])
	{
		vm->vm_bot = mlx_load_png(vm->botcap_vm_path);
		if (!vm->vm_bot)
		{
			id_warn(".vm", "Cannot read vector map file under",
				vm->botcap_vm_path, 0);
		}
	}
}

int	fill_vector_map(t_rt *rt, char **split)
{
	int			i;
	int			j;

	i = rt->n_vector_maps;
	j = 1;
	if (check_format_vector_map(split))
		return (1);
	while (split[j])
	{
		set_vector_map_paths(rt->vector_maps[i], split[j]);
		j++;
	}
	if (!set_vector_map_id(rt->vector_maps[i], split[0]))
		return (id_err(".vm", "Invalid .vm specification", NULL));
	ft_load_vector_map(rt->vector_maps[i]);
	rt->n_vector_maps++;
	return (0);
}

void	free_vector_maps(t_rt *rt)
{
	int	i;

	i = 0;
	while (rt->vector_maps[i] != NULL)
	{
		if (rt->vector_maps[i]->vm_main)
			mlx_delete_texture(rt->vector_maps[i]->vm_main);
		if (rt->vector_maps[i]->vm_top)
			mlx_delete_texture(rt->vector_maps[i]->vm_top);
		if (rt->vector_maps[i]->vm_bot)
			mlx_delete_texture(rt->vector_maps[i]->vm_bot);
		free(rt->vector_maps[i++]);
	}
	free(rt->vector_maps);
}
