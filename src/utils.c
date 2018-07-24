#include "vt.h"

#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>

static inline int clamp_inside(int target, int min, int max);

int term_w;
int term_h;

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