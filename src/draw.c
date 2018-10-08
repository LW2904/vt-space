#include "space.h"

#define DRAW_CHAR '#'

/* Draws an empty rectangle of the given dimensions, starting at the given
 * position and continuing downwards and to the right.
 */
static inline void draw_rectangle(int x, int y, int width, int height);

/* Draws a dot at the given position
 */
static inline void draw_dot(int x, int y);

void draw_ship(int x, int y)
{
	/* Main body */
	draw_rectangle(x, y, PLAYER_WIDTH, PLAYER_HEIGHT);

	/* Snout */
	draw_dot(x + (PLAYER_WIDTH / 2), y - 1);

	/* Left wing */
	draw_dot(x - 1, y + 1);
	draw_dot(x - 1, y + 2);

	draw_dot(x - 2, y + 2);
	draw_dot(x - 3, y + 2);

	/* Right wing */
	draw_dot(x + PLAYER_WIDTH, y + 1);
	draw_dot(x + PLAYER_WIDTH, y + 2);

	draw_dot(x + PLAYER_WIDTH + 1, y + 2);
	draw_dot(x + PLAYER_WIDTH + 2, y + 2);
}

void draw_projectile(int x, int y)
{
	const int width = 1;
	const int height = 4;

	draw_rectangle(x, y, width, height);
}

static inline void draw_rectangle(int x, int y, int width, int height)
{
	for (int rx = 0; rx < width; rx++)
		for (int ry = 0; ry < height; ry++)
			draw_dot(x + rx, y + ry);
}

static inline void draw_dot(int x, int y)
{
	move_cursor(x, y);

	putchar(DRAW_CHAR);
}