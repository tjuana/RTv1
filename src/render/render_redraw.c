#include "rtv1.h"

// Ф-ия определяла, пересекает ли луч объект (шар)
// ->
// Ф-ия возвращает точку попадания
double		render_hit_sphere(t_vector3 center, t_vector3 orig, t_vector3 dir, float radius)
{
	t_vector3	oc;
	float		disc;
	float		a;
	float		b;
	float		c;

	oc = ft_vec3_sub(&orig, &center);
	a = ft_vec3_dot_product(&dir, &dir);
	b = 2.0 * ft_vec3_dot_product(&dir, &oc);
	c = ft_vec3_dot_product(&oc, &oc) - radius * radius;
	disc = b * b - 4 * a * c;

	// Возвращает -1, если луч не проходит через круг
	if (disc < 0) // missed
		return (-1.0);
	else
		return (-b - sqrt(disc)) / (2.0 * a);
}

// Определяет цвет каждого пикселю по лучу
t_vector3	render_get_pixel_color(t_wolf3d *w, t_vector3 orig, t_vector3 dir)
{
	float		t;
	double		intensive;

	t_rt_obj	*light = w->light->content;
	t_list		*ptr_obj = w->obj;

	// Пока есть объекты
	while (ptr_obj)
	{
		// Записываем объект
		t_rt_obj *obj = ptr_obj->content;

		// Если объект относится к какой-то категории, проверяем пересечение
		if (obj->rt_obj_type == RT_OBJ_SPHERE)
			t = render_hit_sphere(obj->coord, (t_vector3){0.0, 0.0, 0.0, 0.0}, dir, obj->radius);
		else
			t = 0.0;

		// Если render_hit_sphere вернёт > 0.0, то область занята некоторым объектом
		if (t > 0.0)
		{
			// Добавляем степень освещённости объекта
			// Чем больше угол между источником света и нормали к точке пересечения, тем ярче цвет объекта

			// Рассчитываем для света (несколько источников):
			t_list *light_list = w->light;
			intensive = 0.0;
			while (light_list)
			{
				// 1. Вычислим ненормализованный вектор
				t_vector3 v = ft_vec3_add(ft_vec3_add(\
					orig, // orig
					ft_vec3_scalar_product(&dir, t) // dir * t
				), (t_vector3){0.0, 0.0, -1.0, 0.0});

				// 2. Вычислим нормализованный вектор
				t_vector3 n = ft_vec3_normalize(&v);

				// 3. Рассчитаем угол между источником света и объектом
				// Скалярное произведение нормированных векторов = cos угла между ними
				intensive += ft_vec3_dot_product(&n, &light->normal_coord);
				light_list = light_list->next;
			}

			intensive = fabs(intensive);
			intensive = (intensive > 1) ? 1 : intensive;

			return (ft_vec3_scalar_product(&obj->vec_rgb, intensive));
		}

		ptr_obj = ptr_obj->next;
	}


	// Расчёт фонового градиента отключён, вместо него -- чёрный цвет
	return ((t_vector3){0.0, 0.0, 0.0, 0.0});
}

void		render_redraw(t_wolf3d *w, t_list *dom)
{
	(void)dom;

	// Last version
	t_rt_obj *camera;

	if (w->camera == NULL)
		return ;
	camera = w->camera->content;

	// Счётчики
	int		x;
	int		y;

	// Коэффициенты отклонения луча
	float	u;
	float 	v;

	// Вектор цвета (буфер для каждого луча)
	t_vector3 vec_color;

	// Свойства сцены
	t_vector3	*orig;

	t_vector3	ray;

	orig = ft_my_malloc(sizeof(t_vector3));
	*orig = (t_vector3){0.0, 0.0, 0.0, 0.0};

	x = 0;
	y = 0;

	// Проходим по каждой строке...
	while (y < WIN_HEIGHT)
	{
		x = 0;
		// По каждому пикселю...
		while (x < WIN_WIDTH)
		{
			// Коэффициенты отклонения луча
			u = ((float)x / WIN_WIDTH);
			v = ((float)y / WIN_HEIGHT);

			// Получаем луч, который пускаем: к координате края изображения добавляем смещение
			ray = (t_vector3){-camera->width / 2 + camera->width * u, -camera->height / 2 + camera->height * v, -10.0, 0.0};

			// Цвет выражен через вектор
			vec_color = render_get_pixel_color(w, *orig, ray);

			// Ставим пиксель (y по пикселям идет вниз, а должен вверх)
			w->sdl->pixels[(WIN_HEIGHT - 1 - y) * WIN_WIDTH + x] = ft_vec_rgb_to_hex(vec_color);
			x++;
		}
		y++;
	}
}