#include "space.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define MAX_PROJECTILES 16

static inline int run_frame();
static inline int vertical_run(int runner, int min, int max);

int term_h;
int term_w;

ship player = { 0 };

int projectile_len = 0;
projectile *projectiles = NULL;

int main()
{
	// Disable stdout buffering.
	setbuf(stdout, NULL);

	set_terminal_nonblock();
	get_terminal_dimensions(&term_w, &term_h);

	projectiles = malloc(sizeof(projectile) * MAX_PROJECTILES);

	player = (ship){ 3, 4, 1, { term_w * 0.5, term_h * 0.8 } };

	while (1) {
		if ((run_frame()) == -1) {
			break;
		}

		nanosleep((struct timespec[]){{ 0, 50000000L }}, NULL);
	}

	restore_terminal();

	return 0;
}

static inline int run_frame()
{
	clear_screen();

	char c = getchar();
	if (c == 'q') {
		return -1;
	}

	// If the key is uppercase...
	int offset = c > 64 && c < 91 ? 2 : 10;

	switch (c) {
	case 'w':
	case 'W': player.pos.y -= offset;
		break;
	case 'a':
	case 'A': player.pos.x -= offset;
		break;
	case 's':
	case 'S': player.pos.y += offset;
		break;
	case 'd':
	case 'D': player.pos.x += offset;
		break;
	case ' ': projectiles[projectile_len++] = (projectile){
			1, 3, 2, {
				player.pos.x + player.width / 2,
				player.pos.y - 1
			}
		};
		break;
	}

	if (projectile_len >= MAX_PROJECTILES) {
		(void)remove_array_item(projectiles, 0, projectile_len,
			sizeof(projectile));

		projectile_len--;
	}

	draw_player(player);

	for (int i = 0; i < projectile_len; i++) {
		projectile *p = projectiles + i;

		p->pos.y -= p->speed;

		if (p->pos.y <= 0) {
			(void)remove_array_item(projectiles, i, projectile_len,
				sizeof(projectile));

			projectile_len--;

			continue;
		}

		draw_projectile(*p);
	}

	cursor_move((position){ 0, 0 });	

	return 0;
}

static inline int vertical_run(int runner, int min, int max)
{
	return runner >= max ? min + 1: runner <= min ? max - 1 : runner;
}