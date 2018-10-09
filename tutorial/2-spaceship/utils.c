#include "space.h"

#include <conio.h>

extern inline char getchar_nonblock()
{
	/* If a key was pressed just now... */
	if (_kbhit())
		/* ...return it */
		return _getch();
	
	return EOF;
}

extern inline int wrap_around(int actual, int min, int max)
{
	if (actual < min)
		return (min - actual) - max;
	if (actual > max)
		return (actual - max) + min;
	
	return actual;
}
