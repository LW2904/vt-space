#include "space.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define STATUS_FAIL -1
#define STATUS_QUIT -2
#define STATUS_CONTNUE 0

#define FRAME_INTERVAL 50

#define ENEMIES_MAX 32
#define PROJECTILES_MAX 16

#define ENEMY_SPEED 1
#define ENEMY_WIDTH 4
#define ENEMY_HEIGHT 4
#define ENEMY_FREQUENCY 40

#define PROJECTILE_SPEED 3
#define PROJECTILE_WIDTH 1
#define PROJECTILE_HEIGHT 4

#define PLAYER_WIDTH 3
#define PLAYER_HEIGHT 4

#define MOVEMENT_INTERVAL_LARGE 8
#define MOVEMENT_INTERVAL_DEFAULT 2

int term_w, term_h;

int paused = 0;
int eliminations = 0;

struct ship player;

int num_enemies = 0;
int enemy_freq = ENEMY_FREQUENCY;
struct enemy enemies[ENEMIES_MAX];

int num_projectiles = 0;
struct projectile projectiles[PROJECTILES_MAX];

int run_frame();

void draw_status();

void handle_exit();
int handle_enemies();
void handle_player(char c);
void handle_projectiles(char c);
void handle_projectile(int index);

static inline void remove_enemy(int index);
static inline void remove_projectile(int index);
static inline int pos_inside(int x1, int y1, int x2, int y2, int w, int h);

int main()
{
	/* Disable stdout buffering */
	if (setvbuf(stdout, NULL, _IONBF, 0)) {
		printf("error disabling stdout buffering\n");
		return EXIT_FAILURE;
	}

	int err;

	if ((err = setup_terminal())) {
		printf("setup_terminal error: %d\n", err);
		return EXIT_FAILURE;
	}

	if ((err = get_terminal_dimensions(&term_w, &term_h))) {
		printf("get_terminal_dimensions error: %d\n", err);
		return EXIT_FAILURE;
	}

	hide_cursor();
	clear_terminal();

	/* Catch Ctrl + C */
	signal(SIGINT, handle_exit);

	/* Start in the center and near the bottom of the screen */
	player = (struct ship){ term_w * 0.5, term_h * 0.8,
		PLAYER_WIDTH, PLAYER_HEIGHT };

	while (1) {
		if (run_frame() != STATUS_CONTNUE)
			break;

		Sleep(FRAME_INTERVAL);
	}

	handle_exit();

	/* This will never be reached */
	return EXIT_SUCCESS;
}

int run_frame()
{
	char c = getchar_nonblock();

	switch (c) {
	case 'q': return STATUS_QUIT;
		break;
	case 'p': paused = !paused;
		break;
	}

	if (paused)
		return STATUS_CONTNUE;

	clear_terminal();

#ifdef DEBUG
	draw_status();
#endif /* DEBUG */

	handle_player(c);
	handle_projectiles(c);

	/* On failure, move status up the chain */
	if (handle_enemies() == STATUS_FAIL)
		return STATUS_FAIL;

	return STATUS_CONTNUE;
}

void handle_player(char c)
{
	/* If it's uppercase (ergo shift was held), use the small interval,
	   else use default. */
	int interval = c > 64 && c < 91 ? MOVEMENT_INTERVAL_DEFAULT :
		MOVEMENT_INTERVAL_LARGE;

	/* Intervals are normalized (in this case halved) for vertical movement,
	   since monospace characters are usually around half as wide as they
	   are tall. */
	switch (c) {
	case 'w':
	case 'W': player.y -= interval / 2;
		break;
	case 'a':
	case 'A': player.x -= interval;
		break;
	case 's':
	case 'S': player.y += interval / 2;
		break;
	case 'd':
	case 'D': player.x += interval;
		break;
	}

	player.x = wrap_around(player.x, 0, term_w);
	player.y = wrap_around(player.y, 0, term_h);

	draw_ship(player);
}

int handle_enemies()
{
	static int spawn_counter = 0;
	static int increase_counter = 0;

	if (++spawn_counter > enemy_freq) {
		spawn_counter = 0;

		/* Range in terminal X: [10%, 90%] */
		int x = (rand() % term_w * 0.8) + term_w * 0.1;
		/* Start just outside the terminal Y */
		int y = 0 - ENEMY_WIDTH;

		enemies[num_enemies++] = (struct enemy){ x, y, ENEMY_SPEED,
			ENEMY_WIDTH, ENEMY_HEIGHT };
	}

	if (++increase_counter > enemy_freq * 2) {
		increase_counter = 0;

		enemy_freq--;
	}

	/* Remove oldest enemies if there are too many, this should never
	   realistically happen */
	for (int i = 0; i <= num_enemies - ENEMIES_MAX; i++)
		remove_enemy(i);
	
	for (int i = 0; i < num_enemies; i++) {
		struct enemy *e = enemies + i;

		e->y += e->speed;

		/* If an enemy reached the bottom of the terminal, notify our
		   caller */
		if (e->y >= term_h)
			return STATUS_FAIL;
		
		draw_enemy(*e);
	}

	return 0;
}

void handle_projectiles(char c)
{
	if (c == ' ') {
		projectiles[num_projectiles++] = (struct projectile){
			player.x + player.width / 2, player.y - 1,
			PROJECTILE_SPEED, PROJECTILE_WIDTH, PROJECTILE_HEIGHT
		};
	}

	/* Remove oldest projectile(s) if there are too many */
	for (int i = 0; i <= num_projectiles - PROJECTILES_MAX; i++)
		remove_projectile(i);

	for (int i = 0; i < num_projectiles; i++)
		handle_projectile(i);
}

void handle_projectile(int index)
{
	struct projectile *p = projectiles + index;

	p->y -= p->speed;

	/* If the projectile moved out of the terminal... */
	if (p->y <= 0 - p->height) {
		remove_projectile(index);

		return;
	}

	for (int i = 0; i < num_enemies; i++) {
		struct enemy *e = enemies + i;

		/* If the projectile is "inside" of any enemy... */
		if (pos_inside(p->x, p->y, e->x, e->y, e->width, e->height)) {
			remove_enemy(i);
			remove_projectile(index);

			eliminations++;
		}
	}

	draw_projectile(*p);
}

void handle_exit()
{
	show_cursor();
	clear_terminal();

	exit(EXIT_SUCCESS);
}

void draw_status()
{
	move_cursor(0, 0);

	printf("term: %04dx%04d", term_w, term_h);
	printf(" | p: { %04d, %04d, %04d, %04d }", player.x, player.y,
		player.width, player.height);
	printf(" | enm: %02d, prj: %02d, elm: %02d", num_enemies,
		num_projectiles, eliminations);
}

static inline void remove_enemy(int index)
{
	num_enemies = remove_array_item(enemies, index, num_enemies,
		sizeof(struct enemy));
}

static inline void remove_projectile(int index)
{
	num_projectiles = remove_array_item(projectiles, index, num_projectiles,
		sizeof(struct projectile));
}

static inline int pos_inside(int x1, int y1, int x2, int y2, int w, int h)
{
	return 	x1 >= x2 && x1 <= x2 + w &&
		y1 >= y2 && y1 <= y2 + h;
}
