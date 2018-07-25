#include "space.h"

#include <stdio.h>

#define DRAW_CHAR '#'

static void draw_rectangle(position p, int width, int height, int filled);

void draw_dot(position p)
{
	cursor_move(p);

	putchar(DRAW_CHAR);
}

void draw_player(player pl)
{
	// Body.
	draw_rectangle(pl.pos, pl.width, pl.height, 1);

	// Snout.
	draw_dot((position){ pl.pos.x + (pl.width / 2), pl.pos.y - 1 });

	// Left wing.
	draw_dot((position){ pl.pos.x - 1, pl.pos.y + 1 });
	draw_dot((position){ pl.pos.x - 1, pl.pos.y + 2 });

	draw_dot((position){ pl.pos.x - 2, pl.pos.y + 2 });
	draw_dot((position){ pl.pos.x - 3, pl.pos.y + 2 });

	// Right wing.
	draw_dot((position){ pl.pos.x + pl.width, pl.pos.y + 1 });
	draw_dot((position){ pl.pos.x + pl.width, pl.pos.y + 2 });

	draw_dot((position){ pl.pos.x + pl.width + 1, pl.pos.y + 2 });
	draw_dot((position){ pl.pos.x + pl.width + 2, pl.pos.y + 2 });
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