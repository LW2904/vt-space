#include "space.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_ENEMIES 32
#define MAX_PROJECTILES 16

void draw_status();

static inline int run_frame();
static inline int pos_inside(position p, position rp, int rw, int rh);

static inline int remove_enemy(int index);
static inline int remove_projectile(int index);

int handle_enemies();
void handle_player(char c);
void handle_projectiles(char c);
void handle_projectile(int index);

void handle_loss();
void handle_pause();

int term_h;
int term_w;

ship player = { 0 };

int enemy_freq = 40;

int enemies_len = 0;
enemy *enemies = NULL;

int projectiles_len = 0;
projectile *projectiles = NULL;

int eliminated = 0;

int paused = 0;

int main()
{
	// Disable stdout buffering.
	setbuf(stdout, NULL);

	set_terminal_nonblock();
	get_terminal_dimensions(&term_w, &term_h);

	cursor_hide();

	enemies = malloc(sizeof(enemy) * MAX_ENEMIES);
	projectiles = malloc(sizeof(projectile) * MAX_PROJECTILES);

	// Start near the bottom and in the center of the screen.
	player = (ship){ 3, 4, 1, { term_w * 0.5, term_h * 0.8 } };

	int exit_status = 0;

	while (1) {
		if ((exit_status = run_frame()) < 0) {
			break;
		}

		// Sleep for 0.5s.
		nanosleep((struct timespec[]){{ 0, 50000000L }}, NULL);
	}

	restore_terminal();

	if (exit_status == -1) {
		handle_loss();		
	}

	clear_screen();
	cursor_show();

	return 0;
}

static inline int run_frame()
{
	clear_screen();
	draw_status();

	char c = getchar();
	switch (c) {
	case 'q': return -2;
		break;
	case 'p': paused = paused ? 0 : 1;
		break;
	}

	if (paused) {
		handle_pause();

		return 0;
	}

	handle_player(c);

	handle_projectiles(c);

	if ((handle_enemies()) == -1) {
		return -1;
	}

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
	clear_line(0);
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

			eliminated++;
		}
	}

	draw_projectile(*p);
}

int handle_enemies()
{
	static int sc = 0;	// Spawn counter.
	static int ic = 0;	// (Difficulty) Increase counter.

	sc++;
	ic++;

	if (sc >= enemy_freq) {
		sc = 0;

		// Range: [10% of term, 90%]
		int x = (rand() % term_w * 0.8) + term_w * 0.1;

		enemies[enemies_len++] = (enemy){ 4, 4, 1, { x, 0 - 4} };
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

	return 0;
}

void handle_loss()
{
	clear_screen();

	char *lbuf = malloc(sizeof(char) * 32);

	sprintf(lbuf, "%d eliminations.", eliminated);

	const int lines_num = 4;
	char *lines[lines_num];

	lines[0] = "You lost!";
	lines[1] = lbuf;
	lines[2] = "";
	lines[3] = "Press ENTER to quit...";

	for (int i = 0; i < lines_num; i++) {
		print_centered((term_h * 0.5) + (lines_num / 2) + i, lines[i]);
	}

	getchar();
}

void handle_pause()
{
	const int lines_num = 3;
	char *lines[lines_num];

	lines[0] = "Game paused.";
	lines[1] = "";
	lines[2] = "Press p to continue...";

	for (int i = 0; i < lines_num; i++) {
		int y = (term_h * 0.5) + (lines_num / 2) + i;

		print_centered(y, lines[i]);
	}
}