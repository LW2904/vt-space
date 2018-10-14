#include "space.h"

#define DRAW_CHAR '#'

/* Draws an empty rectangle of the given dimensions, starting at the given
 * position and continuing downwards and to the right.
 */
static inline void draw_rectangle(int x, int y, int width, int height);

/* Draws a dot at the given position.
 */
static inline void draw_dot(int x, int y);

void draw_ship(struct ship s)
{
	/* Main body */
	draw_rectangle(s.x, s.y, s.width, s.height);

	/* Snout */
	draw_dot(s.x + (s.width / 2), s.y - 1);

	/* Left wing */
	draw_dot(s.x - 1, s.y + 1);
	draw_dot(s.x - 1, s.y + 2);

	draw_dot(s.x - 2, s.y + 2);
	draw_dot(s.x - 3, s.y + 2);

	/* Right wing */
	draw_dot(s.x + s.width, s.y + 1);
	draw_dot(s.x + s.width, s.y + 2);

	draw_dot(s.x + s.width + 1, s.y + 2);
	draw_dot(s.x + s.width + 2, s.y + 2);
}

void draw_projectile(struct projectile p)
{
	draw_rectangle(p.x, p.y, p.width, p.height);
}

void draw_enemy(struct enemy e)
{
	draw_rectangle(e.x, e.y, e.width, e.height);
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
