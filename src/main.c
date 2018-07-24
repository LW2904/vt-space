#include "vt.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

static inline int vertical_run(int runner, int min, int max);

int term_h;
int term_w;

int main()
{
	// Disable stdout buffering.
	setbuf(stdout, NULL);

	set_terminal_nonblock();
	get_terminal_dimensions(&term_w, &term_h);

	int player_speed = 1;
	position player_pos = { term_w * 0.5, term_h * 0.8 };

	char c = 0;
	int frame = 0, offset;

	while (++frame) {
		if (frame > 100) {
			frame = 0;
		}

		clear_screen();

		c = getchar();
		if (c == 'q') {
			break;
		}

		offset = c > 64 && c < 91 ? 1 : 5;

		switch (c) {
		case 'w':
		case 'W': player_pos.y -= offset;
			break;
		case 'a':
		case 'A': player_pos.x -= offset;
			break;
		case 's':
		case 'S': player_pos.y += offset;
			break;
		case 'd':
		case 'D': player_pos.x += offset;
			break;
		}

		draw_player(player_pos);

		cursor_move((position){ 0, 0 });
		nanosleep((struct timespec[]){{ 0, 30000000L }}, NULL);
	}

	restore_terminal();

	return 0;
}

static inline int vertical_run(int runner, int min, int max)
{
	return runner >= max ? min + 1: runner <= min ? max - 1 : runner;
}