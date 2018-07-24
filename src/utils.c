#include "vt.h"

#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>

static inline int clamp_inside(int target, int min, int max);

int term_w;
int term_h;

struct termios inital_settings;

void clear_screen()
{
	printf("\e[2J");
}

void get_terminal_dimensions(int *columns, int *lines)
{
	struct winsize w;
    	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	*lines = w.ws_row;
	*columns = w.ws_col;
}

void clamp_in_terminal(position *p)
{
	p->x = clamp_inside(p->x, 0, term_w);
	p->y = clamp_inside(p->y, 0, term_h);	
}

static inline int clamp_inside(int target, int min, int max)
{
	return target < min ? min : target > max ? max : target;
}

void restore_terminal()
{
	tcsetattr(0, TCSANOW, &inital_settings);
}

void set_terminal_nonblock()
{
	struct termios new;
	tcgetattr(0, &inital_settings);

	new = inital_settings;

	new.c_lflag &= ~ECHO;
	new.c_lflag &= ~ISIG;
	new.c_lflag &= ~ICANON;

	new.c_cc[VMIN] = 0;
	new.c_cc[VTIME] = 0;

	tcsetattr(0, TCSANOW, &new);
}