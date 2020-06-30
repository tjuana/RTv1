#include "rtv1.h"

t_matrix_4x4	ft_init_matrix()
{
	t_matrix_4x4	matrix;
	int				i;
	int				j;

	j = 0;
	while(j < 4)
	{
		i = 0;
		while(i < 4)
		{
			if (i == j)
				matrix.matrix[j][i] = 1.0;
			else
				matrix.matrix[j][i] = 0.0;
			i++;
		} 
		j++;
	}
	matrix.j = 4;
	matrix.i = 4;
	return (matrix);
}

double			ft_math_deg_to_rad(double angle_degrees)
{
	return ((angle_degrees) * M_PI / 180.0);
}

double			ft_math_rad_to_deg(double angle_radians)
{
	return ((angle_radians) * 180.0 / M_PI);
}
