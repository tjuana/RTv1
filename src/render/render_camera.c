#include "rtv1.h"

void	render_camera_add_dir(t_wolf3d *w, t_matrix_4x4 matrix)
{
	t_rt_obj		*camera;

	camera = w->camera->content;
	// camera->normal_dir = ft_transform_vertex(camera->normal_dir, matrix);
	camera->rotation_matrix = ft_mult_matrix(camera->rotation_matrix, matrix);
}
