#include "rtv1.h"

static void		render_set_object(t_wolf3d *w, t_list *ptr_list, t_rt_obj *ptr_elem)
{
	(void)w;
	t_list*		list;

	list = ft_lstnew(ptr_elem, sizeof(t_rt_obj));
	if (ptr_list)
		ft_lstadd(&ptr_list, list);
	else
		ptr_list = list;
}

static void		render_xml_elem_to_object(t_wolf3d *w, t_xml_elem *elem, int type)
{
	t_rt_obj*	obj;
	t_xml_attr*	attr;

	obj = ft_memalloc(sizeof(t_rt_obj));
	ft_bzero(obj, sizeof(t_rt_obj));
	obj->rt_obj_type = type;
	attr = elem->attr->content;
	while (attr)
	{
		// sega!
		if (ft_strcmp(attr->key, "width"))
			obj->width = (double)ft_atoi(attr->value);
		else if (ft_strcmp(attr->key, "height"))
			obj->height = (double)ft_atoi(attr->value);
		else if (ft_strcmp(attr->key, "radius"))
			obj->radius = (double)ft_atoi(attr->value);
		else if (ft_strcmp(attr->key, "type"))
			obj->type = attr->value;
		attr++;
	}

	if (obj->rt_obj_type == RT_OBJ_CAMERA)
		render_set_object(w, w->camera, obj);
	else if (obj->rt_obj_type == RT_OBJ_LIGHT)
		render_set_object(w, w->light, obj);
	else
		render_set_object(w, w->obj, obj);
}

static void		render_xml_file_to_object_list(t_wolf3d *w)
{
	t_list*		list;
	t_xml_elem	*ptr_elem;

	list = w->xml;
	while (list)
	{
		ptr_elem = list->content;
		if (ft_strcmp(ptr_elem->name, "camera"))
			render_xml_elem_to_object(w, ptr_elem, RT_OBJ_CAMERA);
		if (ft_strcmp(ptr_elem->name, "light"))
			render_xml_elem_to_object(w, ptr_elem, RT_OBJ_LIGHT);
		if (ft_strcmp(ptr_elem->name, "sphere"))
			render_xml_elem_to_object(w, ptr_elem, RT_OBJ_SPHERE);
		list++;
	}
}

void			render_set_scene(t_wolf3d *w)
{
	if (w->xml)
		render_xml_file_to_object_list(w);
}
