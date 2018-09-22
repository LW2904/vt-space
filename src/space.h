#ifndef VT_H
#define VT_H

#include <stddef.h>
#include <string.h>

#ifdef _WIN32
  #define ON_WINDOWS
#endif /* _WIN32 */

#ifdef __linux__
  #define ON_LINUX
#endif /* __linux__ */

#define ASCII_ESC 27

extern int term_w;
extern int term_h;

struct position {
    int x;
    int y;
};

struct player {
    int width;
    int height;
    float speed;
    struct position pos;
};

struct projectile {
    int width;
    int height;
    float speed;
    struct position pos;
};

struct enemy {
    int width;
    int height;
    float speed;
    struct position pos;
};

/* cursor operations */

void cursor_hide();
void cursor_show();
void cursor_move(struct position p);
void cursor_move_by(struct position delta);

/* draw functions */

void draw_enemy(struct enemy e);
void draw_player(struct player s);
void draw_projectile(struct projectile pr);

/* utility functions */

void clear_screen();
void clear_line(int y);

char getchar_nonblock();

void clamp_in_terminal(struct position *p);
void get_terminal_dimensions(int *columns, int *lines);

void terminal_setup();
void terminal_restore();

void print_centered(int y, char *string);
void print_centered_block(char **lines, int lines_num);

int remove_array_item(void *array, int index, int length, size_t item_size);

static inline int pos_inside(struct position p, struct position rp,
	int rw, int rh)
{
	return 	p.x >= rp.x && p.x <= rp.x + rw &&
		p.y >= rp.y && p.y <= rp.y + rh;
}

#endif /* VT_H */