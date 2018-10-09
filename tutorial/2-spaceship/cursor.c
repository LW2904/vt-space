#include "space.h"

extern inline void move_cursor(int x, int y)
{
	/* Note the argument order since this expects line, column (ergo y, x)
	   instead of the more common x, y. */
	printf("%c[%d;%dH", ESC, y, x);
}

extern inline void hide_cursor()
{
	printf("%c[?25l", ESC);
}

extern inline void show_cursor()
{
	printf("%c[?25h", ESC);
}