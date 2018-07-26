#include "space.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#define MAX_ENEMIES 32
#define MAX_PROJECTILES 16

void draw_status();

static inline int run_frame();
static inline int pos_inside(position p, position rp, int rw, int rh);

static inline int remove_enemy(int index);
static inline int remove_projectile(int index);

void handle_enemies();
void handle_player(char c);
void handle_projectiles(char c);
void handle_projectile(int index);

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

	// Start near the bottom and in the center of the screen.
	player = (ship){ 3, 4, 1, { term_w * 0.5, term_h * 0.8 } };

	while (1) {
		if ((run_frame()) == -1) {
			break;
		}

		// Sleep for 0.5s.
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

	handle_player(c);

	handle_projectiles(c);

	handle_enemies();

	draw_status();
	// Purely aesthetical.
	cursor_move((position){ 0, 0 });	

	return 0;
}

static inline int pos_inside(position p, position rp, int rw, int rh)
{
	return p.x >= rp.x && p.x <= rp.x + rw && p.y >= rp.y && p.y <= rp.y + rh;
}

static inline int remove_enemy(int index)
{
	return remove_array_item(enemies, index, enemies_len, sizeof(enemy));
}

static inline int remove_projectile(int index)
{
	return remove_array_item(projectiles, index, projectiles_len,
		sizeof(projectile));
}

void draw_status()
{
	cursor_move((position){ 0, 0 });
	printf("\e[2K");
	cursor_move((position){ 0, 0 });

	printf("enm: %d | prj: %d | %d / %d", enemies_len, projectiles_len,
		player.pos.x, player.pos.y);
}

void handle_player(char c)
{
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
	}

	draw_player(player);
}

void handle_projectiles(char c)
{
	if (c == ' ') {
		position proj_pos = {
			player.pos.x + player.width / 2, player.pos.y - 1
		};

		projectiles[projectiles_len++] = (projectile){
			1, 3, 2, proj_pos
		};
	}

	if (projectiles_len >= MAX_PROJECTILES) {
		projectiles_len = remove_projectile(0);
	}

	for (int i = 0; i < projectiles_len; i++) {
		handle_projectile(i);
	}
}

void handle_projectile(int index)
{
	projectile *p = projectiles + index;

	p->pos.y -= p->speed;

	if (p->pos.y <= 0) {
		projectiles_len = remove_projectile(index);
	}

	for (int j = 0; j < enemies_len; j++) {
		enemy *e = enemies + j;

		if (pos_inside(p->pos, e->pos, e->width, e->height)) {
			enemies_len = remove_enemy(j);
			projectiles_len = remove_projectile(index);
		}
	}

	draw_projectile(*p);
}

void handle_enemies()
{
	static int sc = 0;	// Spawn counter.
	static int ic = 0;	// (Difficulty) Increase counter.

	sc++;
	ic++;

	if (sc >= enemy_freq) {
		sc = 0;

		// Range: [10% of term, 90%]
		int x = (rand() % term_w * 0.8) + term_w * 0.1;

		enemies[enemies_len++] = (enemy){ 4, 4, 1, { x, 0 } };
	}

	if (ic >= enemy_freq * 2) {
		ic = 0;

		enemy_freq -= 2;
	}

	for (int i = 0; i < enemies_len; i++) {
		enemy *e = enemies + i;

		e->pos.y += e->speed;

		if (e->pos.y >= term_h) {
			return -1;
		}

		draw_enemy(*e);
	}
}