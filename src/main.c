#include "vt100/vt100.h"

#include <time.h>
#include <stdio.h>

int main()
{
	int term_h, term_w;

	screen_clear();
	get_terminal_dimensions(&term_h, &term_w);

	int i = 0;

	while (1) {
		i++;

		// screen_clear();

		draw_dot(i, i);

		nanosleep((struct timespec[]){{ 0, 500000000L }}, NULL);
	}

	return 0;
}