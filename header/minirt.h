/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: okraus <okraus@student.42prague.com>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/04 15:43:08 by okraus            #+#    #+#             */
/*   Updated: 2024/03/13 10:10:35 by okraus           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINIRT_H

# define MINIRT_H
# define HEIGHT 720
# define WIDTH 1080
# define X 0
# define Y 1
# define Z 2
# define R 0
# define G 1
# define B 2
# define PRECISION 0.000001
# define E_FILE "Invalid file"
# define E_FILE_EXT "Invalid file extension"
# define E_SPEC "Invalid number of specifiers"
# define E_RATIO "Invalid ratio format"
# define E_RGB "Invalid RGB format"
# define E_COORD "Invalid coordinate format"
# define E_VECT "Invalid vector format"
# define E_NORM_ZERO "Vector magnitude must not equal 0"
# define E_FOV "Invalid FOV format"
# define E_BRIGHT "Invalid brightness format"
# define E_DIA "Invalid diameter format"
# define E_HEIGHT "Invalid height format"
# define E_TRIAD_INTFLOAT "3 integers/floats separated by commas"
# define E_MAX_DIGITS "max. 4 integer digits and 5 decimals"
# define E_TRIAD_INT "3 integers, separated by commas"
# define E_INTFLOAT "int or float"
# define E_RATIO_RANGE "Ratio value out of bounds"
# define E_RGB_RANGE "RGB value out of bounds"
# define E_VECT_RANGE "Normalized vector coordinate value out of bounds"
# define E_FOV_RANGE "FOV value out of bounds"
# define E_BRIGHT_RANGE "Brightness value out of bounds"
# define E_DIA_RANGE "Diameter value too low"
# define E_HEIGHT_RANGE "Height value too low"
# define E_RANGE_INT "integers in range (0;255)"
# define E_RANGE_POS "a value in range (0.0;1.0)"
# define E_RANGE_NORM "values in range (-1.0;1.0)"
# define E_RANGE_STRICT "a strictly positive value"
# define W_CHECKER_ID "Checkerboard ID not matched to existing IDs"
# define W_TEXT_ID "Texture ID not matched to existing IDs"
# define W_VMAP_ID "Vector map ID not matched to existing IDs"
# define W_CHECKER_CONT "checkerboard will not display"
# define W_TEXT_CONT "texture will not display"
# define W_VMAP_CONT "vector map will not display"

# define STRINGS 16
# define BLK "\e[0;30m"
# define RED "\e[0;31m"
# define GRN "\e[0;32m"
# define YEL "\e[0;33m"
# define BLU "\e[0;34m"	
# define MAG "\e[0;35m"
# define CYN "\e[0;36m"
# define WHT "\e[0;37m"
# define CRESET "\e[0m"

// INCLUDES

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <stdint.h>
# include <stdarg.h>
# include <sys/wait.h>
# include <time.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <fcntl.h>
# include <stdbool.h>
# include <math.h>
# include "../.MLX42/include/MLX42/MLX42.h"
# include "../libft/header/libft.h"

typedef enum e_mode
{
	DEFAULT,
	OBJECT_CHOICE,
	HIGHLIGHT,
	LIGHTING,
	CAMERA
}			t_mode;

typedef enum e_iterator
{
	BREAK,
	CONTINUE,
	NEXT_OBJECT
}			t_iterator;

typedef struct s_quadterms
{
	double	a;
	double	b;
	double	c;
	double	discr;
	double	res[2];
}			t_quadterms;

typedef enum e_object
{
	EMPTY,
	SPHERE,
	PLANE,
	CYLINDER,
	DISC,
	CONE,
	LIGHT,
	SKYBOX
}				t_object;

typedef struct s_objlist
{
	void				*content;
	struct s_objlist	*next;
	t_object			object;
}				t_objlist;

typedef enum e_raytype
{
	PRIMARY,
	SHADOW
}			t_raytype;

typedef struct s_vect3f
{
	double	x;
	double	y;
	double	z;
}				t_vect3f;

typedef struct s_vect33f
{
	t_vect3f	*normal;
	t_vect3f	*right;
	t_vect3f	*up;
}				t_vect33f;

typedef struct s_quat
{
	double	q0;
	double	q1;
	double	q2;
	double	q3;
}				t_quat;

typedef struct s_qmath
{
	t_quat		q;
	t_quat		first;
	t_quat		second;
	t_vect3f	z;
	t_vect3f	y;
	t_vect3f	up;
	t_vect3f	newup;
	t_vect3f	tmp;
	t_vect3f	right;
}				t_qmath;

typedef struct s_map
{
	long long	u;
	long long	v;
	int			w;
	double		s[3];
	t_vect3f	p;
	t_vect3f	newnormal;
	t_quat		tanq;
	t_quat		invt;
	t_quat		invq;
}				t_map;

typedef struct s_ray
{
	t_vect3f	origin;
	t_vect3f	direction;
}				t_ray;

// rgb array of three colours [R]==[0], [G]==[1], [B]==[2]
typedef struct s_checkerboard
{
	int			*rgb1;
	int			*rgb2;
	double		magnitude;
	int			id;
}				t_checkerboard;

typedef struct s_texture
{
	char			tx_path[1024];
	char			topcap_tx_path[1024];
	char			botcap_tx_path[1024];
	int				id;
	mlx_texture_t	*tx_main;
	mlx_texture_t	*tx_top;
	mlx_texture_t	*tx_bot;
}				t_texture;

typedef struct s_vector_map
{
	char			vm_path[1024];
	char			topcap_vm_path[1024];
	char			botcap_vm_path[1024];
	mlx_texture_t	*vm_main;
	mlx_texture_t	*vm_top;
	mlx_texture_t	*vm_bot;
	int				id;
}				t_vector_map;

typedef struct s_ambient
{
	double	ratio;
	int		*rgb;
}				t_ambient;

typedef struct s_camera
{
	double		*coords;
	double		*nvect;
	int			fov;
	t_vect3f	*normal;
	t_vect3f	*right;
	t_vect3f	*up;
	double		**matrix;
	t_quat		*quat;
}				t_camera;

typedef struct s_sphere
{
	double			*coords;
	double			radius;
	int				*rgb;
	double			brightness; // only relevant for light-emitting spheres
	double			glossiness;
	int				camera_inside;
	t_vect3f		*normal;
	t_vect3f		*right;
	t_vect3f		*up;
	t_quat			q;
	t_mode			mode;
	t_checkerboard	*checkerboard;
	t_texture		*texture;
	t_vector_map	*vector_map;
}				t_sphere;

typedef struct s_plane
{
	double			*coords;
	double			*nvect;
	int				*rgb;
	double			glossiness;
	int				camera_inside;
	t_vect3f		*normal;
	t_vect3f		*right;
	t_vect3f		*up;
	t_quat			q;
	t_mode			mode;
	t_checkerboard	*checkerboard;
	t_texture		*texture;
	t_vector_map	*vector_map;
}				t_plane;

# define NORMALDISC 0
# define INVERSEDISC 1

typedef struct s_disc
{
	double			*coords;
	double			*nvect;
	double			radius;
	int				*rgb;
	double			glossiness;
	int				camera_inside;
	int				is_inversed;
	t_vect3f		*normal;
	t_vect3f		*right;
	t_vect3f		*up;
	t_quat			q;
	t_mode			mode;
	t_checkerboard	*checkerboard;
	t_texture		*texture;
	t_vector_map	*vector_map;
	mlx_texture_t	*tx_disc;
	mlx_texture_t	*vm_disc;
}				t_disc;

typedef struct s_cylinder
{
	double			*coords;
	double			*nvect;
	double			radius;
	double			height;
	int				*rgb;
	double			glossiness;
	int				camera_inside;
	t_disc			*botcap;
	t_disc			*topcap;
	t_vect3f		*normal;
	t_vect3f		*right;
	t_quat			q;
	t_vect3f		*up;
	t_mode			mode;
	t_checkerboard	*checkerboard;
	t_texture		*texture;
	t_vector_map	*vector_map;
}				t_cylinder;

typedef struct s_cone
{
	double			*coords;
	double			*nvect;
	double			radius;
	double			height;
	int				*rgb;
	double			glossiness;
	int				camera_inside;
	double			half_angle;
	double			dist_term; // only used for surface normal math,
	//DO NOT USE ELSEWHERE
	t_vect3f		*axis; // only used for intersection test and
	//surface normal, DO NOT USE ANYWHERE ELSE
	t_disc			*base;
	t_disc			*pinnacle;
	t_vect3f		*normal;
	t_vect3f		*right;
	t_vect3f		*up;
	t_quat			q;
	t_mode			mode;
	t_checkerboard	*checkerboard;
	t_texture		*texture;
	t_vector_map	*vector_map;
}				t_cone;

typedef struct s_rt
{
	t_ambient		*ambient;
	t_camera		*camera;
	t_sphere		**spheres;
	t_plane			**planes;
	t_cylinder		**cylinders;
	t_cone			**cones;
	t_sphere		**light_spheres;
	t_checkerboard	**checkerboards;
	t_texture		**textures;
	t_vector_map	**vector_maps;
	t_sphere		*skybox;
	int				n_spheres;
	int				n_planes;
	int				n_cylinders;
	int				n_cones;
	int				n_lights;
	int				n_checkerboards;
	int				n_textures;
	int				n_vector_maps;
}				t_rt;

typedef struct s_options
{
	t_mode		mode;
	//int			light_intensity;
	int			spec_highlight_size;
}				t_options;

typedef struct s_master
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_rt		*rt;
	t_options	*options;
	char		str[STRINGS][1024];
	mlx_image_t	*string[STRINGS];
	t_objlist	*obj_list;
	int			w_height;
	int			w_width;
}				t_master;

typedef struct s_rayfinder
{
	double		fov;
	double		ratio;
	double		scale;
	double		t_near;
	double		t;
	double		object_flag;
	void		*object_ptr;
	uint32_t	clr;
	t_vect3f	origin;
	double		**cam_mat;
	t_ray		shadowray;
	t_ray		ray;
	t_vect3f	shadow_inter;
	double		inter_dist;
	int			w_height;
	int			w_width;
}				t_rayfinder;

typedef struct s_shader
{
	int			rgb_object[3];
	t_vect3f	hit_normal;
	t_vect3f	light_dir;
	t_vect3f	incident_light;
	t_vect3f	reflect_vect;
	t_vect3f	view_dir;
	t_vect3f	intersection;
	double		dot_reflect;
	double		light_dist;
	double		diffuse_ratio;
	double		specular_ratio;
	double		illumination[3];
	int			pix_color[3];
	double		id;
	double		is;
	double		obj_glossiness;
}				t_shader;

// Initialize objects
void		init_ambient(t_rt *rt);
void		init_camera(t_rt *rt);
void		init_spheres(t_rt *rt, int *ids);
void		init_planes(t_rt *rt, int *ids);
void		init_cylinders(t_rt *rt, int *ids);
void		init_light_sphere(t_rt *rt, int *ids);
void		init_checkerboards(t_rt *rt, int *ids);
void		init_textures(t_rt *rt, int *ids);
void		init_vector_maps(t_rt *rt, int *ids);
void		init_skybox(t_rt *rt);

// Identifier check
int			*init_ids(void);
int			count_identifiers(char *line, int *ids, int *flag);
int			check_identifiers(int fd, int *ids, int *flag);
void		check_missing(int *ids, int *flag);
void		check_duplicates(int *ids, int *flag);
void		get_identifiers(char **split, int *ids, int *flag);

// Format check
int			is_float_triad(char *str);
int			is_rgb_format(char *str);
int			is_floatable(char *str);
int			is_integer(char *str);
int			float_in_range(char *str); // format 9999.99999 allowed
int			triad_in_range(char *triad);
int			has_spec_count(char **split, int target);
int			has_spec_count_strict(char **split, int target);
int			has_valid_id_attribute(char *str); //ex. .ch/03;
int			contains_valid_key_value_pair(char *str); //ex. 1="hello"
int			is_valid_texture_file(char *str);

int			check_format_ambient(char **split);
int			check_format_camera(char **split);
int			check_format_light(char **split);
int			check_format_sphere(char **split);
int			check_format_plane(char **split);
int			check_format_cylinder(char **split);
int			check_format_checkerboard(char **split);
int			check_format_skybox(char **split);

// Data loading
//void	get_data_from_line(char *line, t_rt *rt, int *flag);
void		fill_objects(t_rt *rt, char **split, int *flag);
int			fill_ambient(t_rt *rt, char **split);
int			fill_camera(t_rt *rt, char **split);
int			fill_light(t_rt *rt, char **split);
int			fill_plane(t_rt *rt, char **split);
int			fill_sphere(t_rt *rt, char **split);
int			fill_light_sphere(t_rt *rt, char **split);
int			fill_cylinder(t_rt *rt, char **split);
int			fill_checkerboard(t_rt *rt, char **split);
int			fill_texture(t_rt *rt, char **split);
int			fill_vector_map(t_rt *rt, char **split);
int			fill_skybox(t_rt *rt, char **split);

double		ft_atof(char *str);
int			id_err(char *id, char *err_str, char *details);
int			id_warn(char *id, char *err_str, char *details, int comma);
void		free_objects(t_rt *rt);

int			get_rgb(int *rgb, char *triad);
int			get_coords(double *coords, char *triad);
int			get_nvect(double *nvect, char *triad);

void		init_objects(t_rt *rt, int *ids);
int			print_contents(t_rt *rt);

// Free objects
void		free_ambient(t_rt *rt);
//void		free_light(t_rt *rt);
void		free_camera(t_rt *rt);
void		free_spheres(t_rt *rt);
void		free_planes(t_rt *rt);
void		free_cylinders(t_rt *rt);
void		free_light_sphere(t_rt *rt);
void		free_checkerboards(t_rt *rt);
void		free_textures(t_rt *rt);
void		free_vector_maps(t_rt *rt);
void		free_skybox(t_rt *rt);

// Ray casting
void		cast_rays(t_master *master);

int			solve_quad_sphere(double *t, t_quadterms quad);
int			solve_quad_cyl(double *t, t_quadterms quad,
				t_ray ray, t_cylinder *cylinder);
double		deg(double rad);
double		rad(double deg);
void		ft_swapf(double *a, double *b);
double		absf(double n);
void		normalize(t_vect3f *vect);
t_vect3f	get_normal(double px, double py, double pz);
t_vect3f	subtract_center(t_vect3f vect1, double *coords);
t_vect3f	add_vect3f(t_vect3f vect1, t_vect3f vect2);
t_vect3f	subtract_vect3f(t_vect3f vect1, t_vect3f vect2);
t_vect3f	scale_vect3f(double scale, t_vect3f vect);
t_vect3f	invert_vect3f(t_vect3f vect);
double		dot_product(t_vect3f vect1, t_vect3f vect2);
t_vect3f	cross_product(t_vect3f vect1, t_vect3f vect2);
double		get_vector_length(t_vect3f vector);

// Cylinder caps functions
int			is_between_caps(t_disc	*cap1, t_disc *cap2, t_ray ray, double t);
void		init_discs(t_cylinder *cylinder);
void		define_botcap(t_cylinder *cylinder);
void		define_topcap(t_cylinder *cylinder);
void		get_discs(t_cylinder *cylinder);
void		free_discs(t_cylinder *cylinder);
void		attribute_texture_data_cylinder_bot(t_disc *disc,
				t_cylinder *cylinder);
void		attribute_texture_data_cylinder_top(t_disc *disc,
				t_cylinder *cylinder);

// Cone
void		init_cones(t_rt *rt, int *ids);
int			check_format_cone(char **split);
int			fill_cone(t_rt *rt, char **split);
void		free_cones(t_rt *rt);
void		init_cone_discs(t_cone *cone);
void		get_cone_discs(t_cone *cone);
void		free_cone_discs(t_cone *cone);
void		attribute_texture_data_cone(t_disc *disc, t_cone *cone);

// Camera
void		set_camera(t_camera *camera);
t_vect3f	shift_origin(double **cam);
void		change_ray_direction(double **cam,
				t_vect3f *direction, t_vect3f temp);
void		shift_camera(t_master *master, mlx_key_data_t keydata);
void		rotate_camera(t_master *master, mlx_key_data_t keydata);
void		update_camera_matrix(t_camera *camera);
void		detect_camera_inside_cone(t_cone *cone, t_camera *camera);
void		detect_camera_inside_disc(t_disc *disc, t_camera *camera);
void		detect_camera_inside_cylinder(t_cylinder *cylinder,
				t_camera *camera);
void		detect_camera_inside_plane(t_plane *plane, t_camera *camera);
void		detect_camera_inside_sphere(t_sphere *sphere, t_camera *camera);

// Quaternions
t_quat		get_rot_quat(t_vect3f axis, double angle);
t_quat		get_point_quat(t_vect3f axis);
t_quat		get_inverse_quat(t_quat quat);
t_quat		mult_quat(t_quat i, t_quat j);
void		rotate_vect(t_vect3f *vect, t_quat q);
t_quat		get_rotvect_quat(t_vect3f v1, t_vect3f v2);
t_quat		get_obj_quat(t_vect3f norm, t_vect3f up);
t_quat		get_cylinder_tan_quat(t_vect3f norm);
t_quat		get_cone_tan_quat(t_vect3f norm, t_cone *cone, t_vect3f	i);
t_quat		get_sphere_tan_quat(t_vect3f norm);
void		normalize_quat(t_quat *q);

// Camera movements
void		move(keys_t key, t_camera *camera, double *coords);
void		move_right(t_camera *camera, double *coords);
void		move_left(t_camera *camera, double *coords);
void		move_up(t_camera *camera, double *coords);
void		move_down(t_camera *camera, double *coords);
void		move_forward(t_camera *camera, double *coords);
void		move_backward(t_camera *camera, double *coords);

// Rotations
void		rotate(keys_t key,
				t_vect3f *forward, t_vect3f *right, t_vect3f *up);
void		tilt(t_vect3f *forward,
				t_vect3f *right, t_vect3f *up, double angle);
void		pan(t_vect3f *forward, t_vect3f *right, t_vect3f *up, double angle);
void		cant(t_vect3f *forward,
				t_vect3f *right, t_vect3f *up, double angle);

// Intersections
int			intersect_sphere(t_ray ray, t_sphere *sphere, double *t);
int			intersect_plane(t_ray ray, void *object, double *t, t_object flag);
int			intersect_disc(t_ray ray, t_disc *disc, double *t);
int			is_between_caps(t_disc	*cap1, t_disc *cap2, t_ray ray, double t);
int			intersect_cylinder(t_ray ray, t_cylinder *cylinder, double *t);
t_vect3f	get_intersection(t_vect3f origin, t_vect3f direction, double t);
int			find_intersections(t_master *master,
				t_ray ray, t_rayfinder *rf, t_raytype type);
int			solve_quad_cone(double *t,
				t_quadterms quad, t_ray ray, t_cone *cone);
int			intersect_cone(t_ray ray, t_cone *cone, double *t);
void		set_cone_vects(t_cone *cone);

// Shaders
void		sphere_shader(t_rayfinder *rf,
				t_vect3f intersection, void *object_ptr, t_master *master);
void		plane_shader(t_rayfinder *rf,
				t_vect3f intersection, void *object_ptr, t_master *master);
void		cylinder_shader(t_rayfinder *rf, t_vect3f intersection,
				void *object_ptr, t_master *master);
void		disc_shader(t_rayfinder *rf,
				t_vect3f intersection, void *object_ptr, t_master *master);
void		cone_shader(t_rayfinder *rf,
				t_vect3f intersection, void *object_ptr, t_master *master);
void		light_sphere_shader(t_rayfinder *rf, void *object_ptr);
void		skybox_shader(t_rayfinder *rf, t_vect3f intersection,
				void *object_ptr, t_master *master);

// Shader utils
t_vect3f	get_new_normal(int x, int y, int z);
void		set_ambient_intensity(t_shader *shader,
				int *ambient_rgb, double ambient_ratio);
uint32_t	get_clr_int(int *rgb);
uint32_t	ft_max_clr(uint32_t a, uint32_t b);
void		clampf(double min, double max, double *value);
double		point_distance(t_vect3f p1, t_vect3f p2);
void		get_clr_components(int *light,
				int *rgb, double ratio, double bright);
t_vect3f	get_cylinder_hit_normal(t_vect3f intersection, t_cylinder cylinder);
t_vect3f	get_cone_hit_normal(t_vect3f intersection, t_cone cone);
void		set_cylinder_rgb(t_shader *shader,
				t_cylinder *cylinder, t_vect3f intersection);
void		set_cone_rgb(t_shader *shader, t_cone *cone, t_vect3f intersection);
void		set_disc_rgb(t_shader *shader, t_disc *disc, t_vect3f intersection);
void		set_plane_rgb(t_shader *shader,
				t_plane *plane, t_vect3f intersection);
void		set_sphere_rgb(t_shader *shader,
				t_sphere *sphere, t_vect3f intersection);
void		set_sphere_normal(t_shader *shader,
				t_sphere *sphere, t_vect3f intersection);
void		set_cylinder_normal(t_shader *shader,
				t_cylinder *cylinder, t_vect3f intersection);
void		set_cone_normal(t_shader *shader,
				t_cone *cone, t_vect3f intersection);
void		set_disc_normal(t_shader *shader,
				t_disc *disc, t_vect3f intersection);
void		set_plane_normal(t_shader *shader,
				t_plane *plane, t_vect3f intersection);
t_vect3f	array_to_vect(double *array);
void		get_disc_uv(t_vect3f p,
				long long *u, long long *v, double scale[3]);
void		get_cylinder_uv(t_vect3f p,
				long long *u, long long *v, double scale[3]);
void		get_cone_uv(t_vect3f p,
				long long *u, long long *v, double scale[3]);
void		get_plane_uv(t_vect3f p,
				long long *u, long long *v, double scale[2]);
void		get_sphere_uv(t_vect3f p,
				long long *u, long long *v, double scale[2]);
void		set_sphere_vects(t_sphere *sphere);
void		set_plane_vects(t_plane *plane);
void		set_cylinder_vects(t_cylinder *cylinder);
void		set_disc_vects(t_disc *disc);
void		clamp(int min, int max, int *value);

void		rotate_o(keys_t key, t_vect33f *axes, t_camera *camera);

void		update_ray_direction(t_rayfinder *rf, t_ray *ray, int x, int y);
t_rayfinder	init_rayfinder(t_master	*master);
int			set_checkerboard_pointer(char *obj,
				t_rt *rt, char **split, t_checkerboard **ptr);
int			set_texture_pointer(char *obj,
				t_rt *rt, char **split, t_texture **ptr);
int			set_vector_map_pointer(char *obj,
				t_rt *rt, char **split, t_vector_map **ptr);

// Phong model
void		phong_illumination(t_shader *shader, t_sphere *light);
void		get_reflection_vector(t_shader *shader);
void		diff_and_spec_ratios(t_shader *shader, t_options options);
void		trace_shadow(t_master *master, t_rayfinder *rf,
				t_shader *shader, double *light_pos);

// Hooks and window drawing
void		mousehook(mouse_key_t button,
				action_t action, modifier_key_t mods, void *param);
void		keyhook(mlx_key_data_t keydata, void *param);
t_rayfinder	trace_singular_object_ray(t_master *master,
				int32_t xpos, int32_t ypos);
void		object_movements(t_master *master, mlx_key_data_t keydata);
void		light_movements(t_master *master, mlx_key_data_t keydata);
void		camera_movements(t_master *master, mlx_key_data_t keydata);
void		ft_draw_string(t_master *master);

// File functions
int			open_file(char *path);
int			load_file(char *file, t_rt *rt, int fd);
void		get_texture_data(t_rt *rt, char *file, int *flag);
void		get_object_data(t_rt *rt, char *file, int *flag);

// Object choice
int			change_modes_cone(t_cone *cone, t_mode mode);
int			change_modes_cylinder(t_cylinder *cylinder, t_mode mode);
void		set_highlight_from_reference(t_master *master, t_rayfinder rf);
int			iterate_and_pick(t_master *master, int *flag);
void		reset_to_default(t_master *master);
void		choose_object(t_master *master);

// Object manipulation
void		change_glossiness(double *glossiness, keys_t key);
void		change_radius(double *radius, keys_t key);
void		change_height(double *height, keys_t key);
void		rotate_o(keys_t key, t_vect33f *axes, t_camera *camera);
void		manipulate_light(t_master *master, mlx_key_data_t keydata);
void		manipulate_objects(t_master *master, mlx_key_data_t keydata);
void		manipulate_cone(t_rt *rt, t_cone *cone, mlx_key_data_t keydata);
void		manipulate_cylinder(t_rt *rt,
				t_cylinder *cylinder, mlx_key_data_t keydata);
void		manipulate_plane(t_rt *rt, t_plane *plane, mlx_key_data_t keydata);
void		manipulate_sphere(t_rt *rt,
				t_sphere *sphere, mlx_key_data_t keydata);
void		qrotate_o(keys_t key, t_quat *q, t_camera *camera);

// Texture and vector map helpers
int			set_texture_id(t_texture *texture, char *specifier);
int			set_texture_paths(t_texture *texture, char *str);
int			set_vector_map_id(t_vector_map *vector_map, char *specifier);
int			set_vector_map_paths(t_vector_map *vector_map, char *str);

// Vector setter
void		set_cone_vects_ptrs(t_cone *cone, t_vect33f *o);
void		set_cone_cap_inversion(t_cone *cone);
void		set_cylinder_vects_ptrs(t_cylinder *cylinder, t_vect33f *o);
void		set_cylinder_cap_inversion(t_cylinder *cylinder);

// Rayfinder
t_rayfinder	init_rayfinder(t_master	*master);
void		reset_rayfinder(t_rayfinder *rf);

void		disc_or_plane(void *object,
				t_vect3f *pt, t_vect3f *normal, t_object f);

// Testing functions
/* void	trace_shadow_t(t_master *master, t_rayfinder *rf, t_vect3f intersection,
	t_shader *shader, double *light_pos);
int	find_intersections_t(t_master *master, t_ray ray, t_rayfinder *rf,
	t_raytype type);
void	diff_and_spec_ratios_t(t_shader *shader, t_options options); */
int			point_lies_in_cylinder(t_vect3f point,
				t_vect3f bot_point, t_vect3f top_point, double radius);
int			point_lies_in_cone(t_vect3f point,
				t_cone cone, double radius, double height);
void		shader_loop(t_shader *sh, t_master *m,
				t_vect3f inter, t_rayfinder *rf);
void		detect_camera_inside_objects(t_rt *rt);

int			specifier_parsing_check(int i, int len, int *id, char **split);

#endif
