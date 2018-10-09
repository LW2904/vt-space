#include "space.h"

#include <stdio.h>

#define DRAW_CHAR '#'

static inline void draw_dot(int x, int y);
static inline void draw_rectangle(struct position p, int width, int height,
	int filled);

/* Note that this function expects that p.width and p.height are set to the 
   player dimension defines. */
void draw_player(struct player p)
{
	// Body.
	draw_rectangle(p.pos, p.width, p.height, 1);

	// Snout.
	draw_dot(p.pos.x + (p.width / 2), p.pos.y - 1);

	// Left wing.
	draw_dot(p.pos.x - 1, p.pos.y + 1);
	draw_dot(p.pos.x - 1, p.pos.y + 2);

	draw_dot(p.pos.x - 2, p.pos.y + 2);
	draw_dot(p.pos.x - 3, p.pos.y + 2);

	// Right wing.
	draw_dot(p.pos.x + p.width, p.pos.y + 1);
	draw_dot(p.pos.x + p.width, p.pos.y + 2);

	draw_dot(p.pos.x + p.width + 1, p.pos.y + 2);
	draw_dot(p.pos.x + p.width + 2, p.pos.y + 2);
}

void draw_projectile(struct projectile pr)
{
	draw_rectangle(pr.pos, pr.width, pr.height, 1);
}

void draw_enemy(struct enemy e)
{
	draw_rectangle(e.pos, e.width, e.height, 1);
}

/* The for loops are split up like this since it would always do width * height
   loop iterations otherwise, even though only width * 2 + height * 2 are needed
   for an empty one. */
static inline void draw_rectangle(struct position p, int width, int height,
	int filled)
{
	// Draw horizontal borders.
	for (int x = 0; x < width; x++) {
		draw_dot(p.x + x, p.y);
		draw_dot(p.x + x, p.y + height - 1);
	}

	// Draw vertical borders.
	for (int y = 0; y < height; y++) {
		draw_dot(p.x, p.y + y);
		draw_dot(p.x + width - 1, p.y + y);
	}

	if (!filled)
		return;
	
	// "Fill up" the inside.
	for (int x = 1; x < width - 1; x++) {
		for (int y = 1; y < height - 1; y++) {
			draw_dot(p.x + x, p.y + y);
		}
	}
}

/* This doesn't take a struct position argument because it'd be really
   inconvenient to type out compound literals all the time, even though it would
   be more consistent. */
static inline void draw_dot(int x, int y)
{
	cursor_move((struct position){ x, y });

	putchar(DRAW_CHAR);
}