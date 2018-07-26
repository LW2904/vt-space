#include "space.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define MAX_ENEMIES 32
#define MAX_PROJECTILES 16

static inline int run_frame();
static inline void draw_status();
static inline int vertical_run(int runner, int min, int max);
static inline int pos_inside(position p, position rp, int rw, int rh);

int term_h;
int term_w;

ship player = { 0 };

int enemy_freq = 40;

int enemies_len = 0;
enemy *enemies = NULL;

int projectiles_len = 0;
projectile *projectiles = NULL;

int main()
{
	// Disable stdout buffering.
	setbuf(stdout, NULL);

	set_terminal_nonblock();
	get_terminal_dimensions(&term_w, &term_h);

	enemies = malloc(sizeof(enemy) * MAX_ENEMIES);
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
	case ' ': projectiles[projectiles_len++] = (projectile){
			1, 3, 2, {
				player.pos.x + player.width / 2,
				player.pos.y - 1
			}
		};
		break;
	}

	if (projectiles_len >= MAX_PROJECTILES) {
		(void)remove_array_item(projectiles, 0, projectiles_len,
			sizeof(projectile));

		projectiles_len--;
	}

	draw_player(player);

	static int fc = 0;
	static int ic = 0;

	fc++;
	ic++;

	if (fc >= enemy_freq) {
		fc = 0;

		int x = (rand() % term_w * 0.8) + term_w * 0.1;

		enemies[enemies_len++] = (enemy){ 4, 4, 1, { x, 0 } };
	}

	if (ic >= enemy_freq * 5) {
		ic = 0;

		enemy_freq -= 5;
	}

	for (int i = 0; i < enemies_len; i++) {
		enemy *e = enemies + i;

		e->pos.y += e->speed;

		if (e->pos.y >= term_h) {
			return -1;
		}

		draw_enemy(*e);
	}

	for (int i = 0; i < projectiles_len; i++) {
		projectile *p = projectiles + i;

		p->pos.y -= p->speed;

		if (p->pos.y <= 0) {
			(void)remove_array_item(projectiles, i, projectiles_len,
				sizeof(projectile));

			projectiles_len--;

			continue;
		}

		for (int j = 0; j < enemies_len; j++) {
			enemy *e = enemies + j;

			if (pos_inside(p->pos, e->pos, e->width, e->height)) {
				enemies_len = remove_array_item(enemies, j,
					enemies_len, sizeof(enemy));

				projectiles_len = remove_array_item(projectiles,
					i, projectiles_len, sizeof(projectile));
			}
		}

		draw_projectile(*p);
	}

	draw_status();
	cursor_move((position){ 0, 0 });	

	return 0;
}

static inline int vertical_run(int runner, int min, int max)
{
	return runner >= max ? min + 1: runner <= min ? max - 1 : runner;
}

static inline int pos_inside(position p, position rp, int rw, int rh)
{
	return p.x >= rp.x && p.x <= rp.x + rw && p.y >= rp.y && p.y <= rp.y + rh;
}

static inline void draw_status()
{
	cursor_move((position){ 0, 0 });
	printf("\e[2K");
	cursor_move((position){ 0, 0 });

	printf("enm: %d | prj: %d | %d / %d", enemies_len, projectiles_len,
		player.pos.x, player.pos.y);
}