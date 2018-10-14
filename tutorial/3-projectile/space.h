#ifndef SPACE_H
#define SPACE_H

/* ASCII "escape" character */
#define ESC 0x1B

#include <stdio.h>
#include <stddef.h>
#include <windows.h>

struct ship {
	int x;
	int y;
	int width;
	int height;
};

struct projectile {
	int x;
	int y;
	int speed;
	int width;
	int height;
};

/* Fetches the dimensions in columns and lines (aka rows) of the current 
 * terminal window. Returns nonzero on failure.
 */
int get_terminal_dimensions(int *columns, int *lines);

/* Enables processing of virtual terminal sequences through the input stream.
 * Returns nonzero on failure.
 */
int setup_terminal();

/* Erases the terminal screen.
 */
void clear_terminal();

/* Draws a ship at the given position. Width and height are constant.
 */
void draw_ship(struct ship player);

/* Draws a projectile at the given position.
 */
void draw_projectile(struct projectile p);

/* Moves the cursor to the given position.
 */
void move_cursor(int x, int y);

/* Hides and reveals the cursor, respectively.
 */
void hide_cursor();
void show_cursor();

/* Nonblocking version of stdlib function getchar.
 */
char getchar_nonblock();

/* Make a given number "wrap around" if it is larger or smaller than the min or
 * max, respectively.
 */
int wrap_around(int actual, int min, int max);

/* Removes an array item, moving the rest of the array "downwards".
 */
int remove_array_item(void *array, int index, int length, size_t item_size);

#endif
