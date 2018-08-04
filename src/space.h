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

char getchar_nonblock();

void clamp_in_terminal(position *p);
void get_terminal_dimensions(int *columns, int *lines);

void terminal_setup();
void terminal_restore();

void print_centered(int y, char *string);
void print_centered_block(char **lines, int lines_num);

int remove_array_item(void *array, int index, int length, size_t item_size);

static inline int pos_inside(position p, position rp, int rw, int rh)
{
	return 	p.x >= rp.x && p.x <= rp.x + rw &&
		p.y >= rp.y && p.y <= rp.y + rh;
}

#endif /* VT_H */