#include "rtv1.h"

/*
** **************************************************************************
**	void ft_gui_events_keydown_me(t_wolf3d *w, SDL_Event *e)
** **************************************************************************
*/

void	ft_gui_events_keydown_me(t_wolf3d *w, SDL_Event *e)
{
	(void)e;
	ft_gui_redraw(w);
}

// Подумать о разделении событий
void	ft_gui_events_keydown_render(t_wolf3d *w, SDL_Event *e)
{
	(void)w;
	(void)e;

	w->gui.search_elem = GUI_EVENT_ON;
	// Y transform
	if (e->key.keysym.scancode == SDL_SCANCODE_W)
		render_camera_add_dir(w, ft_ry_matrix(ft_init_matrix(), 0.1745));
	else if (e->key.keysym.scancode == SDL_SCANCODE_S)
		render_camera_add_dir(w, ft_ry_matrix(ft_init_matrix(), -0.1745));
	// X transform
	else if (e->key.keysym.scancode == SDL_SCANCODE_A)
		render_camera_add_dir(w, ft_rx_matrix(ft_init_matrix(), 0.1745));
	else if (e->key.keysym.scancode == SDL_SCANCODE_D)
		render_camera_add_dir(w, ft_rx_matrix(ft_init_matrix(), -0.1745));
	// Z transform
	else if (e->key.keysym.scancode == SDL_SCANCODE_Q)
		render_camera_add_dir(w, ft_rz_matrix(ft_init_matrix(), 0.1745));
	else if (e->key.keysym.scancode == SDL_SCANCODE_E)
		render_camera_add_dir(w, ft_rz_matrix(ft_init_matrix(), -0.1745));
	else
		w->gui.search_elem = GUI_EVENT_OFF;
}

/*
** **************************************************************************
**	void ft_gui_events_keydown(t_wolf3d *w, SDL_Event *e)
** **************************************************************************
*/

void	ft_gui_events_keydown(t_wolf3d *w, SDL_Event *e)
{
	if (e->type == SDL_KEYDOWN)
	{
		if (w->gui.mode == GUI_MD_MENU || w->gui.mode == GUI_MD_RENDER)
		{
			(e->key.keysym.scancode == SDL_SCANCODE_ESCAPE) ? \
				w->sdl->running = 0 : 0;
		}
		if (w->gui.mode == GUI_MD_RENDER)
			ft_gui_events_keydown_render(w, e);
		if (w->gui.mode == GUI_MD_ME)
			ft_gui_events_keydown_me(w, e);
		if (w->gui.focus_elem != NULL)
			ft_gui_focus_keydown(w, *e, w->gui.focus_elem);
	}
}

/*
** **************************************************************************
**	void ft_gui_events(t_wolf3d *w)
**
**	Function that handle user events.
** **************************************************************************
*/

void	ft_gui_events(t_wolf3d *w)
{
	SDL_Event e;

	while (SDL_PollEvent(&e))
	{
		e.type == SDL_QUIT ? w->sdl->running = 0 : 0;
		ft_gui_events_keydown(w, &e);
		ft_gui_events_mouse(w, &e);
	}
	if (w->gui.search_elem == GUI_EVENT_ON/* || \
		w->gui.search_elem == GUI_EVENT_SEARCH*/)
	{
		ft_gui_redraw(w);
		w->gui.search_elem = GUI_EVENT_OFF;
	}
}
