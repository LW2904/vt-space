#include "vt100/vt100.h"

#include <time.h>
#include <stdio.h>

int main()
{
	int term_h, term_w;

	screen_clear();
	get_terminal_dimensions(&term_h, &term_w);

	// Disable stdout buffering.
	setbuf(stdout, NULL);

	int i = 0;

	while (1) {
		i++;

		screen_clear();

		for (int i = 0; i < 90; i++) {
			int cx, cy;
			get_circle_point(term_w / 2, 270 + i, &cx, &cy);

			cx += term_w / 2;
			cy += term_h * 0.8;

			draw_dot(cx, cy);
		}

		nanosleep((struct timespec[]){{ 0, 500000000L }}, NULL);
	}

	return 0;
}