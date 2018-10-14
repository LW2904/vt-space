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

int remove_array_item(void *array, int index, int length, size_t item_size)
{
	size_t byte_offset = item_size * index;
	size_t new_size = item_size * (length - index - 1);

	char *arr = (char *)array;

	memmove(arr + byte_offset, arr + byte_offset + item_size, new_size);

	return length - 1;
}
