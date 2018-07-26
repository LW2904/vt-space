#include "space.h"

#include <stdio.h>

position current;

void cursor_move(position p)
{
	clamp_in_terminal(&p);

	// Note the argument order since this expects "ESC line;column H"
	// where line is equivalent to Y and column to X.
	printf("%c[%d;%dH", ASCII_ESC, p.y, p.x);

	current = p;
}

void cursor_move_by(position delta)
{
	position new_pos = { current.x + delta.x, current.y + delta.y };

	cursor_move(new_pos);
}

void cursor_hide()
{
	printf("%c[?25l", ASCII_ESC);
}

void cursor_show()
{
	printf("%c[?25h", ASCII_ESC);
}