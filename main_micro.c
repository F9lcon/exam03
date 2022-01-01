#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


typedef struct s_img_params
{
	char	rec_type;
	float	x;
	float	y;
	float	width;
	float	height;
	char	pixel;
}	t_img_params;

typedef struct s_window_params
{
	int	width;
	int	height;
	char	bg_char;
}	t_window_params;





int	get_window_params(t_window_params *window_params, FILE *file)
{
	int res;

	res = fscanf(file, "%i %i %c \n", &window_params->width,
		&window_params->height, &window_params->bg_char);
	if (window_params->width < 1 || window_params->width >300)
	{
		write(1, "Error: Operation file corrupted\n", 33);
		return (-1);
	}
	if (window_params->height < 1 || window_params->height >300)
	{
		write(1, "Error: Operation file corrupted\n", 33);
		return (-1);
	}
	if (res < 0)
	{
		write(1, "Error: Operation file corrupted\n", 33);
		return (-1);
	}
	return (0);
}

int get_img_params(t_img_params *img_params, FILE *file)
{
	int res;

	res =  fscanf(file, "%c %f %f %f %f %c \n", &img_params->rec_type, &img_params->x, &img_params->y, 
				&img_params->width, &img_params->height, &img_params->pixel);
	if (img_params->width <= 0 || img_params->height <= 0)
		{
			write(1, "Error: Operation file corrupted\n", 33);
			return (-1);
		}
	return (res);
}

int check_for_perimetr(float num_i, float num_param)
{
	if (sqrtf(powf(num_i - num_param, 2)) < 1)
		{
			// printf("num_i %f num_param %f %f\n", num_i, num_param, sqrtf(powf(num_i - num_param, 2)));
			return (1);
		}
	else
		{
			// printf("%f\n", sqrtf(powf(num_i - num_param, 2)));
			return (0);
		}
}
int check_pixel(t_img_params *i_params, float x_i, float y_i)
{
	float x_right = i_params->x + i_params->width;
	float y_bottom = i_params->y + i_params->height;

	if (x_i >= i_params->x && x_i <= x_right && y_i >= i_params->y && y_i <= y_bottom)
		{
			if (i_params->rec_type == 'R')
				return (1);
			else if (check_for_perimetr(x_i, i_params->x) || check_for_perimetr(x_i, x_right)
						|| check_for_perimetr(y_i, i_params->y) || check_for_perimetr(y_i, y_bottom))
				return (1);
			else
				return (0);
		}
	else
		return (0);
}

int fill_str(t_window_params *w_params, t_img_params *i_params, char **arr)
{
	float x_i = 0, y_i = 0;
	while (y_i < w_params->height)
	{
		x_i = 0;
		while (x_i < w_params->width)
		{
			if (check_pixel(i_params, x_i, y_i))
				arr[(int)y_i][(int)x_i] = i_params->pixel;
			x_i++;
		}
		arr[(int)y_i][(int)x_i] = '\n';
		y_i++;
	}
	return (0);
}

char **get_arr(int y, int x, t_window_params *w_params)
{
	char **arr;
	arr = malloc((y + 1) * sizeof(char *));
	arr[y] = NULL;
	for (int i = 0; i < y; i++)
	{
		arr[i] = calloc(x, sizeof(char));
		memset(arr[i], w_params->bg_char, x - 1);
		if (arr[i] == NULL)
			printf("some shit here\n");
	}
	return (arr);
}

void free_arr(char **arr, int y)
{
	for (int i = 0; i < y; i++)
	{
		free(arr[i]);
		i++;
	}
	free(arr);
}

int main(int argc, char **argv)
{
	FILE *file;
	t_window_params window_params;
	t_img_params	img_params;

	if (argc != 2)
	{
		write(1, "Error: argument\n", 17);
		return (1);
	}
	if (0) // fail open file
	{
		write(1, "Error: Operation file corrupted\n", 33);
		return (1);
	}
	file = fopen(argv[1], "r");
	if (!file)
		{
			write(1,"fuck with open file\n", 5);
			return (1);
		}
	if (get_window_params(&window_params, file) == -1)
		return (1);
	char **arr = get_arr(window_params.height, window_params.width + 1, &window_params);
	while (get_img_params(&img_params, file) > 0)
			fill_str(&window_params, &img_params, arr);
	int k = 0;
	while (k < window_params.height)
	{
		printf("%s", arr[k]);
		k++;
	}
	free_arr(arr, window_params.height);
	fclose(file);
	return (0);
}
