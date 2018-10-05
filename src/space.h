#ifndef SPACE_H
#define SPACE_H

/* ASCII "escape" character */
#define ESC 0x1B

#include <stddef.h>

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

#endif