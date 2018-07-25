#include "vt.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define MAX_PROJECTILES 16

static inline int vertical_run(int runner, int min, int max);

int term_h;
int term_w;

int main()
{
	// Disable stdout buffering.
	setbuf(stdout, NULL);

	set_terminal_nonblock();
	get_terminal_dimensions(&term_w, &term_h);

	float player_speed = 1;
	position player_pos = { term_w * 0.5, term_h * 0.8 };

	char c = 0;
	int offset;

	int projectile_i = 0;
	position projectiles[MAX_PROJECTILES];

	while (1) {
		clear_screen();

		c = getchar();
		if (c == 'q') {
			break;
		}

		// If the key is uppercase...
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
		case ' ': projectiles[projectile_i++] = player_pos;
			break;
		}

		projectile_i %= MAX_PROJECTILES - 1;

		draw_player(player_pos);

		for (int i = 0; i <= projectile_i; i++) {
			
		}

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