// #warning remove  assert.h and ./ of assign2.h when submitting
#include "assign2.h"
// #include <assert.h>
Point *findOutLine(Box boxes[], int size, int *outputsize)
{
	// assert(size);
	if (size == 1)
	{
		Point *point_array = new Point[2];
		point_array[0].x = boxes[0].left;
		point_array[0].y = boxes[0].ht;
		point_array[1].x = boxes[0].right;
		point_array[1].y = 0;
		*outputsize = 2;
		return point_array;
	}

	Box *fhalf = new Box[size / 2];
	Box *shalf = new Box[size - (size / 2)];
	for (int i = 0; i < size / 2; i++)  //divide
	{
		fhalf[i].ht = boxes[i].ht;
		fhalf[i].left = boxes[i].left;
		fhalf[i].right = boxes[i].right;
	}
	for (int i = size / 2; i < size; i++) //divide
	{
		shalf[i - (size / 2)].ht = boxes[i].ht;
		shalf[i - (size / 2)].left = boxes[i].left;
		shalf[i - (size / 2)].right = boxes[i].right;
	}

	*outputsize = 0;
	int outline_fhalf_size = 0;
	int outline_shalf_size = 0;
	Point *outline_fhalf = findOutLine(fhalf, size / 2, &outline_fhalf_size);   
	Point *outline_shalf = findOutLine(shalf, size - (size / 2), &outline_shalf_size);
	Point *point_array = new Point[outline_fhalf_size + outline_shalf_size];  //merged output

	// fprintf(stderr, "\n");
	// fprintf(stderr, "Case-> n = %d\n", size);
	// fprintf(stderr, "Input Boxes:\t");
	// for (int i = 0; i < size; i++)
	// {
	// 	fprintf(stderr, "(%d, %d, %d), ", boxes[i].left, boxes[i].right, boxes[i].ht);
	// }
	// fprintf(stderr, "\n");
	// fprintf(stderr, "First half outline (%d) :\t", outline_fhalf_size);
	// for (int i = 0; i < outline_fhalf_size; i++)
	// {
	// 	fprintf(stderr, " (%d, %d), ", outline_fhalf[i].x, outline_fhalf[i].y);
	// }
	// fprintf(stderr, "\n");
	// fprintf(stderr, "Second half outline (%d) :\t", outline_shalf_size);
	// for (int i = 0; i < outline_shalf_size; i++)
	// {
	// 	fprintf(stderr, " (%d, %d), ", outline_shalf[i].x, outline_shalf[i].y);
	// }
	// fprintf(stderr, "\n");
	// fprintf(stderr,"outline_fhalf_size and outline_shalf_size=(%d+%d)\n",outline_fhalf_size,outline_shalf_size);

	int i = 0, j = 0;
	int fhalf_prev_h = 0;
	int shalf_prev_h = 0;
	while ((i < outline_fhalf_size) and (j < outline_shalf_size))
	{
		// fprintf(stderr,"%d\t",*outputsize);
		if (outline_fhalf[i].x < outline_shalf[j].x)
		{
			int nx, nh;
			nx = outline_fhalf[i].x;
			fhalf_prev_h = outline_fhalf[i].y;
			if (fhalf_prev_h > shalf_prev_h)
				nh = fhalf_prev_h;
			else
				nh = shalf_prev_h;

			//insertion
			if ((*outputsize) == 0)
			{
				point_array[*outputsize].x = nx;
				point_array[*outputsize].y = nh;
				(*outputsize)++;
			}
			else
			{
				int px = point_array[(*outputsize) - 1].x;
				int py = point_array[(*outputsize) - 1].y;
				if (py == nh)
				{
					//dont add
					i++;
					continue;
				}
				else if (px == nx)
				{
					//dont add just update the last one
					if (py < nh)
						point_array[(*outputsize) - 1].y = nh;
				}
				else
				{
					point_array[*outputsize].x = nx;
					point_array[*outputsize].y = nh;
					(*outputsize)++;
				}
			}
			i++;
		}
		else
		{
			int nx, nh;
			nx = outline_shalf[j].x;
			shalf_prev_h = outline_shalf[j].y;
			if (fhalf_prev_h > shalf_prev_h)
				nh = fhalf_prev_h;
			else
				nh = shalf_prev_h;

			if ((*outputsize) == 0)
			{
				point_array[*outputsize].x = nx;
				point_array[*outputsize].y = nh;
				(*outputsize)++;
			}
			else
			{
				int px = point_array[(*outputsize) - 1].x;
				int py = point_array[(*outputsize) - 1].y;
				if (py == nh)
				{
					//dont add
					j++;
					continue;
				}
				else if (px == nx)
				{
					//dont add just update the last one
					if (py < nh)
						point_array[(*outputsize) - 1].y = nh;
				}
				else
				{
					point_array[*outputsize].x = nx;
					point_array[*outputsize].y = nh;
					(*outputsize)++;
				}
			}
			j++;
		}
		// assert(*outputsize <= outline_fhalf_size + outline_shalf_size);
	}
	while (i < outline_fhalf_size)
	{
		int nx, nh;
		nx = outline_fhalf[i].x;
		nh = outline_fhalf[i].y;
		if ((*outputsize) == 0)
		{
			point_array[*outputsize].x = nx;
			point_array[*outputsize].y = nh;
			(*outputsize)++;
		}
		else
		{
			int px = point_array[(*outputsize) - 1].x;
			int py = point_array[(*outputsize) - 1].y;
			if (py == nh)
			{
				//dont add
				i++;
				continue;
			}
			else if (px == nx)
			{
				//dont add just update the last one
				if (py < nh)
					point_array[(*outputsize) - 1].y = nh;
			}
			else
			{
				point_array[*outputsize].x = nx;
				point_array[*outputsize].y = nh;
				(*outputsize)++;
			}
		}
		i++;
		// assert(*outputsize <= outline_fhalf_size + outline_shalf_size);
	}
	while (j < outline_shalf_size)
	{
		int nx, nh;
		nx = outline_shalf[j].x;
		nh = outline_shalf[j].y;
		if ((*outputsize) == 0)
		{
			point_array[*outputsize].x = nx;
			point_array[*outputsize].y = nh;
			(*outputsize)++;
		}
		else
		{
			int px = point_array[(*outputsize) - 1].x;
			int py = point_array[(*outputsize) - 1].y;
			if (py == nh)
			{
				//dont add
				j++;
				continue;
			}
			else if (px == nx)
			{
				//dont add just update the last one
				if (py < nh)
					point_array[(*outputsize) - 1].y = nh;
			}
			else
			{
				point_array[*outputsize].x = nx;
				point_array[*outputsize].y = nh;
				(*outputsize)++;
			}
		}
		j++;
		// assert(*outputsize <= outline_fhalf_size + outline_shalf_size);
	}

	// set rightmost point ka height to 0
	point_array[(*outputsize) - 1].y = 0;


	// fprintf(stderr, "Merged outline (%d):\t", *outputsize);
	// for (int i = 0; i < *outputsize; i++)
	// {
	// 	fprintf(stderr, " (%d, %d), ", point_array[i].x, point_array[i].y);
	// }
	// fprintf(stderr, "\n");


	// here solve the problem, where boxes contain the input data and size is the number of
	// boxes on the table
	// set outputsize to the number of points in Point array you are yet to return

	return point_array;
}
int main()
{
	process();
	return 0;
}