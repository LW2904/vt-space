#include "space.h"

/* Polyfill for older API versions */
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
  #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

int get_terminal_dimensions(int *columns, int *lines)
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	if (console == INVALID_HANDLE_VALUE)
		return GetLastError();

	CONSOLE_SCREEN_BUFFER_INFO screen;
	if (!GetConsoleScreenBufferInfo(console, &screen))
		return GetLastError();

    	*lines = screen.srWindow.Bottom - screen.srWindow.Top + 1;
	*columns = screen.srWindow.Right - screen.srWindow.Left + 1;

	return 0;
}

int setup_terminal()
{
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	if (console == INVALID_HANDLE_VALUE)
		return GetLastError();

	DWORD mode;
	if (!GetConsoleMode(console, &mode))
		return GetLastError();

	mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(console, mode))
		return GetLastError();

	return 0;
}

void clear_terminal()
{
	printf("%c[2J", ESC);
}
