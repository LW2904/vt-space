#include "vt.h"

#include <stdio.h>

#define DRAW_CHAR '#'

void draw_dot(position p)
{
	cursor_move(p);

	putchar(DRAW_CHAR);
}

void draw_player(position p)
{
	int width = 3;
	int height = 3;

	int xoff = width / 2;
	int yoff = height / 2;

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			draw_dot((position){
				(p.x + x) + xoff, (p.y + y) + yoff
			});
		}
	}

	draw_dot((position){ (p.x + 1) + xoff, (p.y - 1) + yoff });
}

void draw_projectile(position p)
{
	
}