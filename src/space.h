#ifndef VT_H
#define VT_H

extern int term_w;
extern int term_h;

struct position {
    int x;
    int y;
};

typedef struct position position;

void cursor_hide();
void cursor_show();
void cursor_move(position p);
void cursor_move_by(position delta);

void draw_dot(position p);
void draw_player(position p);

void clear_screen();
void clamp_in_terminal(position *p);
void get_terminal_dimensions(int *columns, int *lines);
void restore_terminal();
void set_terminal_nonblock();


#endif /* VT_H */