#include<stdio.h>
typedef struct  {
	// x coordinate of left side
	int left;
	// height
	int ht;
	// x coordinate of right side
	int right;
} Box;

// A Point in Outline
typedef struct  {
	// x coordinate
	int x;
	// height or the y coordinate
	int y;
} Point;

Point* findOutLine(Box boxes[], int size, int *outputsize);
void prettyPrint(Point* points, int outputsize) {
	int i;
	printf(" OutLine for given Boxes is \n[");
	for (i = 0; i < outputsize; i++) {

		printf(" (%d, %d), ", points[i].x, points[i].y);

	}
	printf("\b\b ]\n");
}
void process() {
	int n;
	scanf("%d", &n);
	Box boxes[n];
	int i;
	for (i = 0; i < n; i++) {
		scanf("%d %d %d", &boxes[i].left , &boxes[i].right , &boxes[i].ht);
	}
	int outputsize;
	Point* ptr = findOutLine(boxes, n, &outputsize);

	prettyPrint(ptr, outputsize);
}