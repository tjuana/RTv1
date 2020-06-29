#include "rtv1.h"

static void		render_set_object(t_wolf3d *w, t_list **ptr_list, t_rt_obj *ptr_elem)
{
	(void)w;
	t_list*		list;

	list = ft_lstnew(ptr_elem, sizeof(t_rt_obj));
	if (*ptr_list)
		ft_lstadd(ptr_list, list);
	else
		*ptr_list = list;
}

static void		render_xml_elem_to_object(t_wolf3d *w, t_xml_elem *elem, int type)
{
	t_list		*attr_list;
	t_rt_obj	*obj;
	t_xml_attr	*attr;

	obj = ft_memalloc(sizeof(t_rt_obj));
	ft_bzero(obj, sizeof(t_rt_obj));
	obj->rt_obj_type = type;
	attr_list = elem->attr;
	while (attr_list)
	{
		attr = attr_list->content;
		ft_strcmp(attr->key, "width");
		if (!ft_strcmp(attr->key, "width"))
			obj->width = (double)ft_atoi(attr->value);
		else if (!ft_strcmp(attr->key, "height"))
			obj->height = (double)ft_atoi(attr->value);
		else if (!ft_strcmp(attr->key, "radius"))
			obj->radius = (double)ft_atoi(attr->value);
		else if (!ft_strcmp(attr->key, "type"))
			obj->type = attr->value;

		// for coord. and dir.
		else if (!ft_strcmp(attr->key, "coord"))
		{
			char** array = ft_strsplit(attr->value, ',');
			obj->coord.x = (double)ft_atoi(array[0]);
			obj->coord.y = (double)ft_atoi(array[1]);
			obj->coord.z = (double)ft_atoi(array[2]);
			obj->normal_coord = ft_vec3_normalize(&obj->coord);
			// need to delete array
		}
		else if (!ft_strcmp(attr->key, "dir"))
		{
			char** array = ft_strsplit(attr->value, ',');
			obj->dir.x = (double)ft_atoi(array[0]);
			obj->dir.y = (double)ft_atoi(array[1]);
			obj->dir.z = (double)ft_atoi(array[2]);
			obj->normal_dir = ft_vec3_normalize(&obj->dir);
			// need to delete array
		}
		// for rgb color
		else if (!ft_strcmp(attr->key, "rgb"))
		{
			char** array = ft_strsplit(attr->value, ',');
			obj->vec_rgb.x = (double)ft_atoi(array[0]) / 256.0;
			obj->vec_rgb.y = (double)ft_atoi(array[1]) / 256.0;
			obj->vec_rgb.z = (double)ft_atoi(array[2]) / 256.0;
			// need to delete array
		}

		attr_list = attr_list->next;
	}

	if (obj->rt_obj_type == RT_OBJ_CAMERA)
		render_set_object(w, &w->camera, obj);
	else if (obj->rt_obj_type == RT_OBJ_LIGHT)
		render_set_object(w, &w->light, obj);
	else
		render_set_object(w, &w->obj, obj);
}

static void		render_xml_file_to_object_list(t_wolf3d *w)
{
	t_list*		list;
	t_xml_elem	*ptr_elem;

	list = w->xml;
	while (list)
	{
		ptr_elem = list->content;
		if (!ft_strcmp(ptr_elem->name, "camera"))
			render_xml_elem_to_object(w, ptr_elem, RT_OBJ_CAMERA);
		else if (!ft_strcmp(ptr_elem->name, "light"))
			render_xml_elem_to_object(w, ptr_elem, RT_OBJ_LIGHT);
		else if (!ft_strcmp(ptr_elem->name, "sphere"))
			render_xml_elem_to_object(w, ptr_elem, RT_OBJ_SPHERE);
		else if (!ft_strcmp(ptr_elem->name, "plane"))
			render_xml_elem_to_object(w, ptr_elem, RT_OBJ_PLANE);
		list = list->next;
	}
}

void			render_set_scene(t_wolf3d *w)
{
	if (w->xml)
		render_xml_file_to_object_list(w);
}
