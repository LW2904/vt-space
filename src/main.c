#include <stdio.h>

int main()
{
	// Disable stdout buffering.
	setbuf(stdout, NULL);

	return 0;
}