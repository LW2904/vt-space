#include "space.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#define STATUS_FAIL -1
#define STATUS_QUIT -2

#define MAX_ENEMIES 32
#define MAX_PROJECTILES 16

void start_game();

void draw_status();
void print_help(int add_note);

void handle_loss();
void handle_pause();
void handle_restart();

int handle_enemies();
void handle_player(char c);
void handle_projectiles(char c);
void handle_projectile(int index);

static inline int remove_enemy(int index);
static inline int remove_projectile(int index);

static inline int run_frame();
static inline int pos_inside(struct position p, struct position rp,
	int rw, int rh);

int term_h;
int term_w;

struct player player_ship = { 0 };

int enemy_freq = 40;
int enemies_len = 0;
struct enemy *enemies = NULL;

int projectiles_len = 0;
struct projectile *projectiles = NULL;

int paused = 0;

int eliminations = 0;

int main()
{
	// Disable stdout buffering.
	setbuf(stdout, NULL);

	terminal_setup();
	get_terminal_dimensions(&term_w, &term_h);

	cursor_hide();

	enemies = malloc(sizeof(struct enemy) * MAX_ENEMIES);
	projectiles = malloc(sizeof(struct projectile) * MAX_PROJECTILES);

	if (!enemies || !projectiles) {
		printf("malloc failed\n");
		return EXIT_FAILURE;
	}

	start_game();

	return EXIT_SUCCESS;
}

void start_game()
{
	// Start near the bottom and in the center of the screen.
	struct position player_pos = { term_w * 0.5, term_h * 0.8 };

	player_ship = (struct player){ 3, 4, 1, player_pos };

	int frame_status = 0;

	while (1) {
		if ((frame_status = run_frame()) < 0) {
			break;
		}

		// Sleep for 0.5s.
		nanosleep((struct timespec[]){{ 0, 50000000L }}, NULL);
	}

	if (frame_status == STATUS_FAIL) {
		handle_loss();		
	}

	terminal_restore();

	clear_screen();
	cursor_show();
}

static inline int run_frame()
{
	static int standby = 1;

	char c = getchar_nonblock();
	switch (c) {
	case 'q': return STATUS_QUIT;
		break;
	case 'p': paused = paused ? 0 : 1;
		break;
	}

	if (c > 0) {
		standby = 0;
	}

	if (paused) {
		handle_pause();

		return 0;
	}

	clear_screen();
	draw_status();

	handle_player(c);

	handle_projectiles(c);

	if (standby) {
		print_help(1);
		return 0;
	}

	if (!eliminations) {
		print_help(0);
	}

	if ((handle_enemies()) == STATUS_FAIL) {
		return STATUS_FAIL;
	}

	return 0;
}

static inline int remove_enemy(int index)
{
	return remove_array_item(enemies, index, enemies_len,
		sizeof(struct enemy));
}

static inline int remove_projectile(int index)
{
	return remove_array_item(projectiles, index, projectiles_len,
		sizeof(struct projectile));
}

void draw_status()
{
	cursor_move((struct position){ 0, 0 });
	clear_line(0);
	cursor_move((struct position){ 0, 0 });

	printf("enm: %d | prj: %d | %d / %d | [q]uit, [p]ause",
		enemies_len, projectiles_len, player_ship.pos.x,
		player_ship.pos.y);
}

void handle_player(char c)
{
	// If the key is uppercase...
	int offset = c > 64 && c < 91 ? 2 : 10;

	switch (c) {
	case 'w':
	case 'W': player_ship.pos.y -= offset;
		break;
	case 'a':
	case 'A': player_ship.pos.x -= offset;
		break;
	case 's':
	case 'S': player_ship.pos.y += offset;
		break;
	case 'd':
	case 'D': player_ship.pos.x += offset;
		break;
	}

	draw_player(player_ship);
}

void handle_projectiles(char c)
{
	// If space was pressed, spawn a new projectile.
	if (c == ' ') {
		struct position proj_pos = {
			player_ship.pos.x + player_ship.width / 2,
			player_ship.pos.y - 1
		};

		projectiles[projectiles_len++] = (struct projectile){
			1, 3, 2, proj_pos
		};
	}

	// Remove oldest projectile(s) if there are too many.
	for (int i = 0; i <= projectiles_len - MAX_PROJECTILES; i++) {
		projectiles_len = remove_projectile(i);
	}

	for (int i = 0; i < projectiles_len; i++) {
		handle_projectile(i);
	}
}

void handle_projectile(int index)
{
	struct projectile *p = projectiles + index;

	p->pos.y -= p->speed;

	// If projectile moved out of screen, remove it.
	if (p->pos.y <= 0) {
		projectiles_len = remove_projectile(index);
	}

	for (int j = 0; j < enemies_len; j++) {
		struct enemy *e = enemies + j;

		// If projectile is inside of an enemy, remove both.
		if (pos_inside(p->pos, e->pos, e->width, e->height)) {
			enemies_len = remove_enemy(j);
			projectiles_len = remove_projectile(index);

			eliminations++;
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

		enemies[enemies_len++] = (struct enemy){ 4, 4, 1, { x, 0 - 4} };
	}

	if (ic >= enemy_freq) {
		ic = 0;

		enemy_freq -= 1;
	}

	for (int i = 0; i < enemies_len; i++) {
		struct enemy *e = enemies + i;

		e->pos.y += e->speed;

		if (e->pos.y >= term_h) {
			return STATUS_FAIL;
		}

		draw_enemy(*e);
	}

	return 0;
}

void handle_loss()
{
	clear_screen();

	char *lbuf = malloc(sizeof(char) * 32);

	sprintf(lbuf, "%d eliminations.", eliminations);

	const int lines_num = 4;
	char *lines[lines_num];

	lines[0] = "You lost!";
	lines[1] = lbuf;
	lines[2] = "";
	lines[3] = "[q]uit, [r]estart";
	
	free(lbuf);

	print_centered_block(lines, lines_num);

	char c;
	while ((c = getchar_nonblock())) {
		switch (c) {
		case 'q':
			return;
		case 'r':
			handle_restart();
			return;
		}

		nanosleep((struct timespec[]){{ 0, 50000000L }}, NULL);
	}
}

void handle_pause()
{
	const int lines_num = 3;
	char *lines[lines_num];

	lines[0] = "Game paused.";
	lines[1] = "";
	lines[2] = "Press p to continue...";

	print_centered_block(lines, lines_num);
}

void print_help(int add_note)
{
	const int lines_num = 7;
	char *lines[lines_num];

	lines[0] = "WASD to move your spacecraft.";
	lines[1] = "Shift for precise movement.";
	lines[2] = "Space to shoot.";
	lines[3] = "";
	lines[4] = "Don't let the evil rectangoloids reach the bottom of the scre- err, world!";
	lines[5] = "";
	lines[6] = add_note ? "Press any key to start." : "";

	print_centered_block(lines, lines_num);
}

void handle_restart()
{
	// Reset difficulty.
	enemy_freq = 40;

	// Reset enemies and projectiles.
	enemies_len = 0;
	projectiles_len = 0;

	paused = 0;
	eliminations = 0;

	start_game();
}
