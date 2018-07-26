#include "space.h"

#include <stdio.h>

#define DRAW_CHAR '#'

static void draw_rectangle(position p, int width, int height, int filled);

void draw_dot(position p)
{
	cursor_move(p);

	putchar(DRAW_CHAR);
}

void draw_player(ship s)
{
	// Body.
	draw_rectangle(s.pos, s.width, s.height, 1);

	// Snout.
	draw_dot((position){ s.pos.x + (s.width / 2), s.pos.y - 1 });

	// Left wing.
	draw_dot((position){ s.pos.x - 1, s.pos.y + 1 });
	draw_dot((position){ s.pos.x - 1, s.pos.y + 2 });

	draw_dot((position){ s.pos.x - 2, s.pos.y + 2 });
	draw_dot((position){ s.pos.x - 3, s.pos.y + 2 });

	// Right wing.
	draw_dot((position){ s.pos.x + s.width, s.pos.y + 1 });
	draw_dot((position){ s.pos.x + s.width, s.pos.y + 2 });

	draw_dot((position){ s.pos.x + s.width + 1, s.pos.y + 2 });
	draw_dot((position){ s.pos.x + s.width + 2, s.pos.y + 2 });
}

void draw_projectile(projectile pr)
{
	draw_rectangle(pr.pos, pr.width, pr.height, 1);
}

static void draw_rectangle(position p, int width, int height, int filled)
{
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			if (filled || x == 0 || x == width - 1 || y == 0
				|| y == height - 1)
			{
				draw_dot((position){ p.x + x, p.y + y });
			}
		}
	}
}