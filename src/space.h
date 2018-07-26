#ifndef VT_H
#define VT_H

#include <stddef.h>
#include <string.h>

extern int term_w;
extern int term_h;

struct position {
    int x;
    int y;
};
typedef struct position position;

struct player {
    int width;
    int height;
    float speed;
    position pos;
};
typedef struct player player;

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

void cursor_hide();
void cursor_show();
void cursor_move(position p);
void cursor_move_by(position delta);

void draw_dot(position p);
void draw_player(player pl);
void draw_projectile(projectile pr);

void clear_screen();
void clamp_in_terminal(position *p);
void get_terminal_dimensions(int *columns, int *lines);
void restore_terminal();
void set_terminal_nonblock();

static inline int remove_array_item(void *array, int index, int length,
	size_t item_size)
{
	size_t byte_offset = item_size * index;
	size_t new_size = item_size * (length - index - 1);

	memmove(array + byte_offset, array + byte_offset + item_size, new_size);

	return length - 1;
};


#endif /* VT_H */