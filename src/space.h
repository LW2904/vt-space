#ifndef VT_H
#define VT_H

#include <stddef.h>
#include <string.h>

#define ASCII_ESC 27

extern int term_w;
extern int term_h;

struct position {
    int x;
    int y;
};
typedef struct position position;

struct ship {
    int width;
    int height;
    float speed;
    position pos;
};
typedef struct ship ship;

struct projectile {
    int width;
    int height;
    float speed;
    position pos;
};
typedef struct projectile projectile;

struct enemy {
    int width;
    int height;
    float speed;
    position pos;
};
typedef struct enemy enemy;

/* cursor operations */
void cursor_hide();
void cursor_show();
void cursor_move(position p);
void cursor_move_by(position delta);

/* draw functions */
void draw_enemy(enemy e);
void draw_player(ship s);
void draw_projectile(projectile pr);

/* utility functions */
void clear_screen();
void clear_line(int y);
void clamp_in_terminal(position *p);
void get_terminal_dimensions(int *columns, int *lines);

void restore_terminal();
void set_terminal_nonblock();

void print_centered(int y, char *string);

static inline int remove_array_item(void *array, int index, int length,
	size_t item_size)
{
	size_t byte_offset = item_size * index;
	size_t new_size = item_size * (length - index - 1);

	char *arr = (char *)array;

	memmove(arr + byte_offset, arr + byte_offset + item_size, new_size);

	return length - 1;
}


#endif /* VT_H */