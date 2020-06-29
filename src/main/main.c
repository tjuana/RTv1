#include "rtv1.h"

/*
** **************************************************************************
**	void ft_main(int c, char **v)
**
**	General programm function.
** **************************************************************************
*/

void	ft_main(int c, char **v)
{
	t_wolf3d	w;

	ft_bzero(&w, sizeof(t_wolf3d));
	// (void)v;
	if (c > 2)
		ft_error("[Error]: WRONG arguments");
	else if (c == 2)
		w.xml = parser_xml_file(v[1]);
	else
		w.xml = NULL;
	render_set_scene(&w);
	// ft_unpack(); // disabled
	w.sdl = sdl_init(w.sdl);
	ft_gui_init(&w);
	ft_main_gui_init(&w);
	ft_gui_redraw(&w);
	while (w.sdl->running)
		ft_main_events(&w);
	ft_clean_sdl(&w);
	// ft_check_folders(); // disabled
}

int		main(int c, char **v)
{
	ft_main(c, v);
	return (0);
}
