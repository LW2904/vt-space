#include "space.h"

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

#define PLAYER_WIDTH 3
#define PLAYER_HEIGHT 4

#define MOVEMENT_INTERVAL_LARGE 8
#define MOVEMENT_INTERVAL_DEFAULT 2

int term_w, term_h;

struct ship player;

void run_frame();

void handle_player(char c);

void handle_sigint();

int main()
{
	/* Disable stdout buffering. */
	setbuf(stdout, NULL);

	int err;

	if ((err = setup_terminal())) {
		printf("setup_terminal error: %d\n", err);
		return 1;
	}

	if ((err = get_terminal_dimensions(&term_w, &term_h))) {
		printf("get_terminal_dimensions error: %d\n", err);
		return 1;
	}

	hide_cursor();
	clear_terminal();

	/* Catch Ctrl + C. */
	signal(SIGINT, handle_sigint);

	/* Start in the center and near the bottom of the screen. */
	player = (struct ship){ term_w * 0.5, term_h * 0.8,
		PLAYER_WIDTH, PLAYER_HEIGHT };

	while (1) {
		run_frame();

		Sleep(100);
	}

	show_cursor();

	return 0;
}

void run_frame()
{
	clear_terminal();

	char c = getchar_nonblock();

	handle_player(c);
}

void handle_player(char c)
{
	/* If it's uppercase (ergo shift was held), use the small interval,
	   else use default. */
	int interval = c > 64 && c < 91 ? MOVEMENT_INTERVAL_DEFAULT :
		MOVEMENT_INTERVAL_LARGE;

	switch (c) {
	/* Normalize movement distance, characters are taller than they are
	   wide. */
	case 'w':
	case 'W': player.y -= interval / 2;
		break;
	case 'a':
	case 'A': player.x -= interval;
		break;
	/* See above. */
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

void handle_sigint()
{
	show_cursor();

	exit(0);
}
