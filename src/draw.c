#include "space.h"

#include <stdio.h>

#define DRAW_CHAR '#'

static inline void draw_dot(position p);
static inline void draw_rectangle(position p, int width, int height, int filled);

// Note that this function expects that p.width and p.height are set to the 
// player dimension defines. 
void draw_player(player p)
{
	// Body.
	draw_rectangle(p.pos, p.width, p.height, 1);

	// Snout.
	draw_dot((position){ p.pos.x + (p.width / 2), p.pos.y - 1 });

	// Left wing.
	draw_dot((position){ p.pos.x - 1, p.pos.y + 1 });
	draw_dot((position){ p.pos.x - 1, p.pos.y + 2 });

	draw_dot((position){ p.pos.x - 2, p.pos.y + 2 });
	draw_dot((position){ p.pos.x - 3, p.pos.y + 2 });

	// Right wing.
	draw_dot((position){ p.pos.x + p.width, p.pos.y + 1 });
	draw_dot((position){ p.pos.x + p.width, p.pos.y + 2 });

	draw_dot((position){ p.pos.x + p.width + 1, p.pos.y + 2 });
	draw_dot((position){ p.pos.x + p.width + 2, p.pos.y + 2 });
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