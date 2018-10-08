#include "space.h"

#include <stdio.h>
#include <stdlib.h>

#define MAX_PROJECTILES 16
#define PROJECTILE_SPEED 2

#define MOVEMENT_INTERVAL_SMALL 2
#define MOVEMENT_INTERVAL_DEFAULT 8

int term_w, term_h;
int player_x, player_y;

int num_projectiles = 0;
struct projectile *projectiles = NULL;

static inline void run_frame();

static inline void handle_player(char c);

static inline void handle_projectiles(char c);
static inline void handle_projectile(int index);

int main()
{
	/* Disable stdout buffering. */
	setbuf(stdout, NULL);

	int err;

	if ((err = setup_terminal())) {
		printf("setup_terminal error: %d\n", err);
		return 1;
	}

	clear_terminal();
	
	if ((err = get_terminal_dimensions(&term_w, &term_h))) {
		printf("get_terminal_dimensions error: %d\n", err);
		return 1;
	}

	projectiles = malloc(MAX_PROJECTILES * sizeof(struct projectile));

	if (!projectiles) {
		printf("failled allocating memory\n");
		return 1;
	}

	/* Start in the center and near the bottom of the screen. */
	player_x = term_w * 0.5;
	player_y = term_h * 0.8;

	while (1) {
		run_frame();

		Sleep(500);
	}

	return 0;
}

static inline void run_frame()
{
	char c = getchar_nonblock();

	handle_player(c);
}

static inline void handle_player(char c)
{
	/* If it's uppercase (ergo shift was held), use the small interval,
	   else use default. */
	int interval = c > 64 && c < 91 ? MOVEMENT_INTERVAL_SMALL :
		MOVEMENT_INTERVAL_DEFAULT;

	switch (c) {
	case 'w':
	case 'W': player_y -= interval;
		break;
	case 'a':
	case 'A': player_x -= interval;
		break;
	case 's':
	case 'S': player_y += interval;
		break;
	case 'd':
	case 'D': player_x += interval;
		break;
	}

	player_x = wrap_around(player_x, 0, term_w);
	player_y = wrap_around(player_y, 0, term_h);

	draw_ship(player_x, player_y);
}

static inline void handle_projectiles(char c)
{
	if (c == ' ') {
		projectiles[num_projectiles++] = (struct projectile){
			player_x + PLAYER_WIDTH / 2, player_y - 1,
			PROJECTILE_SPEED
		};
	}

	/* Remove oldest projectile(s) if there are too many. */
	for (int i = 0; i <= num_projectiles - MAX_PROJECTILES; i++) {
		num_projectiles = remove_projectile(i);
	}

	for (int i = 0; i < num_projectiles; i++) {
		handle_projectile(i);
	}
}

static inline void handle_projectile(int index)
{
	struct projectile *p = projectiles + index;

	p->y -= p->speed;

	if (p->y <= 0) {
		num_projectiles = remove_projectile(index);
	}
}

static inline void remove_projectile(int index)
{
	(void)remove_array_item(projectiles, index, num_projectiles,
		sizeof(struct projectile));
}
