#include "space.h"

#define DRAW_CHAR '#'

void draw_ship(int x, int y)
{
	const int width = 3;
	const int height = 4;

	/* Main body */
	draw_rectangle(x, y, width, height);

	/* Snout */
	draw_dot(x + (width / 2), y - 1);

	/* Left wing */
	draw_dot(x - 1, y + 1);
	draw_dot(x - 1, y + 2);

	draw_dot(x - 2, y + 2);
	draw_dot(x - 3, y + 2);

	/* Right wing */
	draw_dot(x + width, y + 1);
	draw_dot(x + width, y + 2);

	draw_dot(x + width + 1, y + 2);
	draw_dot(x + width + 2, y + 2);
}

void draw_rectangle(int x, int y, int width, int height)
{
	for (int rx = 0; rx < width; rx++)
		for (int ry = 0; ry < height; ry++)
			draw_dot(x + rx, y + ry);
}

void draw_dot(int x, int y)
{
	move_cursor(x, y);

	putchar(DRAW_CHAR);
}
