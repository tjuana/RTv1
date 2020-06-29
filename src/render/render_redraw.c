#include "rtv1.h"

// Ф-ия определяла, пересекает ли луч объект (шар)
// ->
// Ф-ия возвращает точку попадания
double		hit_sphere(t_vector3 center, t_vector3 orig, t_vector3 dir, float radius)
{
	t_vector3	oc;
	float		desc;
	float		a;
	float		b;
	float		c;

	oc = ft_vec3_sub(&orig, &center);
	a = ft_vec3_dot_product(&dir, &dir);
	b = 2.0 * ft_vec3_dot_product(&oc, &dir);
	c = ft_vec3_dot_product(&oc, &oc) - radius * radius;
	desc = b * b - 4 * a * c;

	// Возвращает -1, если луч не проходит через круг
	if (desc < 0)
		return (-1.0);
	else
		return (-b - sqrt(desc)) / (2.0 * a);
}

// Определяет цвет каждого пикселю по лучу
t_vector3	ft_color(t_wolf3d *w, t_vector3 orig, t_vector3 dir)
{
	float		t;
	double		intensive;

	t_rt_obj *light = w->light->content;
	// t_vector3	color_vec = (t_vector3){1.0, 0.0, 0.0, 0.0}; // red

	t_list *ptr_obj = w->obj;
	while (ptr_obj)
	{
		t_rt_obj *obj = ptr_obj->content;
		t = hit_sphere(obj->coord, (t_vector3){0.0, 0.0, 0.0, 0.0}, dir, obj->radius);

		// Если hit_sphere вернёт true, то область занята некоторым объектом
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
				t_vector3 v = ft_vec3_add(ft_vec3_add(
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
			intensive = intensive > 1 ? 1 : intensive;

			return (ft_vec3_scalar_product(&obj->vec_rgb, intensive));
		}

		ptr_obj = ptr_obj->next;
	}


	// Расчёт фонового градиента отключён, вместо него -- чёрный цвет
	return ((t_vector3){0.0, 0.0, 0.0, 0.0});
}

void		ft_render_redraw(t_wolf3d *w, t_list *dom)
{
	(void)dom;

	// Last version
	t_rt_obj *camera;

	if (w->camera == NULL)
		return ;
	camera = w->camera->content;

	// Счётчики
	float x;
	float y;
	// int wid = 0;

	// Коэффициенты отклонения луча
	float u;
	float v;

	// Вектор цвета (буфер для каждого луча)
	t_vector3 col;

	// Свойства сцены
	t_vector3 *orig;
	t_vector3 *left_corner;
	t_vector3 *horizontal;
	t_vector3 *vertical;

	orig = ft_my_malloc(sizeof(t_vector3));
	left_corner = ft_my_malloc(sizeof(t_vector3));
	horizontal = ft_my_malloc(sizeof(t_vector3));
	vertical = ft_my_malloc(sizeof(t_vector3));

	*orig = (t_vector3){0.0, 0.0, 0.0, 0.0};

	// left_corner -- настройки камеры (расположение камеры?)
	// left_corner = (t_vector3){смещение по горизонтали, смещение по вертикали, ~zoom, 0}
	*left_corner = (t_vector3){-camera->width / 2, -camera->height / 2, -10.0, 0.0};

	// horizontal, vertical -- отвечают за пропорцию проекции
	// (за угол обзора fov)
	*horizontal = (t_vector3){camera->width, 0.0, 0.0, 0.0};
	*vertical = (t_vector3){0.0, camera->height, 0.0, 0.0};

	x = 0;
	// y = WIN_HEIGHT - 1;
	y = 0;

	while ((int)y < WIN_HEIGHT)
	{
		x = 0;
		while (x < WIN_WIDTH)
		{
			// Коэффициенты отклонения луча
			u = (float)(x / WIN_WIDTH);
			v = (float)(y / WIN_HEIGHT);

			// Цвет выражен через вектор
			col = ft_color(w, *orig, ft_vec3_add(ft_vec3_add(*left_corner, \
				ft_vec3_scalar_product(horizontal, u)), ft_vec3_scalar_product(vertical, v)));
			int ir = (255.99 * col.x);
			int ig = (255.99 * col.y);
			int ib = (255.99 * col.z);
			w->sdl->pixels[(WIN_HEIGHT - 1 - (int)y) * WIN_WIDTH + (int)x] = ft_rgb_to_hex(ir, ig, ib);
			x++;
		}
		y++;
	}
}