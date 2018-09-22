#include "space.h"

#include <stdio.h>

#define DRAW_CHAR '#'

static inline void draw_dot(position p);
static inline void draw_rectangle(position p, int width, int height, int filled);

// TODO: This is ugly and doesn't respect custom witdh and height properly. 
void draw_ship(ship s)
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

void draw_enemy(enemy e)
{
	draw_rectangle(e.pos, e.width, e.height, 1);
}

static inline void draw_rectangle(position p, int width, int height, int filled)
{
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			// TODO: Just look at it.
			if (filled || x == 0 || x == width - 1 || y == 0
				|| y == height - 1)
			{
				draw_dot((position){ p.x + x, p.y + y });
			}
		}
	}
}

static inline void draw_dot(position p)
{
	cursor_move(p);

	putchar(DRAW_CHAR);
}