#include "space.h"

#include <stdio.h>

#define MOVEMENT_INTERVAL_SMALL 2
#define MOVEMENT_INTERVAL_DEFAULT 8

int term_w, term_h;
int player_x, player_y;

int main()
{
	int err;

	if ((err = setup_terminal())) {
		printf("setup_terminal error: %d\n", err);
		return -1;
	}

	clear_terminal();
	
	if ((err = get_terminal_dimensions(&term_w, &term_h))) {
		printf("get_terminal_dimensions error: %d\n", err);
		return -1;
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