#ifndef RENDER_H
# define RENDER_H

# include <libft.h>
# include "SDL2/SDL.h"
# include "SDL2/SDL_ttf.h"
# include "general_struct.h"

# define RT_OBJ_CAMERA		0x00
# define RT_OBJ_PLANE		0x01
# define RT_OBJ_SPHERE		0x02
# define RT_OBJ_CYLINDER	0x03
# define RT_OBJ_CONE		0x04
# define RT_OBJ_LIGHT		0xF1

typedef struct				s_rt_obj
{
	int						rt_obj_type;
	char*					type;
	t_vector3				coord;
	t_vector3				dir;
	int						color;
	double					radius;
	double					width;
	double					height;
}							t_rt_obj;

typedef struct				s_rt_scene
{
	int						i;
}							t_rt_scene;

/*
** **************************************************************************
**	src/render/render_redraw.c
** **************************************************************************
*/
void						ft_render_redraw(t_wolf3d *w, t_list *dom);

/*
** **************************************************************************
**	src/render/render_set.c
** **************************************************************************
*/
void						render_set_scene(t_wolf3d *w);

#endif