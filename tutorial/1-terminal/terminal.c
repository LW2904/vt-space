#include "space.h"

/* Polyfill for older API versions */
#ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
  #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#endif

int get_terminal_dimensions(int *columns, int *lines)
{
	DWORD access = GENERIC_READ | GENERIC_WRITE;
	DWORD mode = FILE_SHARE_READ | FILE_SHARE_WRITE;
	HANDLE console = CreateFileW(L"CONOUT$", access, mode, NULL,
		OPEN_EXISTING, 0, NULL);

	CONSOLE_SCREEN_BUFFER_INFO screen;
	if (!GetConsoleScreenBufferInfo(console, &screen))
		return GetLastError();

    	*lines = screen.srWindow.Bottom - screen.srWindow.Top + 1;
	*columns = screen.srWindow.Right - screen.srWindow.Left + 1;

	return 0;
}

int setup_terminal()
{


	DWORD access = GENERIC_READ | GENERIC_WRITE;
	DWORD mode = FILE_SHARE_READ | FILE_SHARE_WRITE;
	HANDLE console = CreateFileW(L"CONOUT$", access, mode, NULL,
		OPEN_EXISTING, 0, NULL);

	if (!GetConsoleMode(console, &mode)) {
		printf("GetConsoleMode error: %ld\n", GetLastError());
		return 1;
	}

	mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(console, mode)) {
		printf("SetConsoleMode error: %ld\n", GetLastError());
		return 1;
	}

	return 0;
}

void clear_terminal()
{
	printf("%c[2J", ESC);
}
