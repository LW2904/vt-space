#include "space.h"

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
	cursor_move((position){ 0, 0 });
	printf("%c[2J", ASCII_ESC);
}

void clear_line(int y)
{
	cursor_move((position){ 0, y });
	printf("%d[2K", ASCII_ESC);
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

void print_centered(int y, char *string)
{
	cursor_move((position){
		(term_w * 0.5) - (strlen(string) / 2),
		y,
	});

	printf("%s", string);
}

void print_centered_block(char **lines, int lines_num)
{
	for (int i = 0; i < lines_num; i++) {
		int y = (term_h * 0.5) - (lines_num / 2) + i;

		print_centered(y, lines[i]);
	}
}

int remove_array_item(void *array, int index, int length, size_t item_size)
{
	size_t byte_offset = item_size * index;
	size_t new_size = item_size * (length - index - 1);

	char *arr = (char *)array;

	memmove(arr + byte_offset, arr + byte_offset + item_size, new_size);

	return length - 1;
}

static inline int clamp_inside(int target, int min, int max)
{
	return target < min ? min : target > max ? max : target;
}