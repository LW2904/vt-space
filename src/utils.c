#include "space.h"

#include <stdio.h>

#ifdef ON_LINUX
  #include <unistd.h>
  #include <termios.h>
  #include <sys/ioctl.h>

  struct termios inital_settings;
#endif /* ON_LINUX */

#ifdef ON_WINDOWS
  #include <conio.h>
  #include <windows.h>

  #define ENABLE_VIRTUAL_TERMINAL_PROCESSING  0x0004
#endif /* ON_WINDOWS */

static inline int clamp_inside(int target, int min, int max);

int term_w;
int term_h;

void get_terminal_dimensions(int *columns, int *lines)
{
#ifdef ON_LINUX
	struct winsize w;
    	ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

	*lines = w.ws_row;
	*columns = w.ws_col;
#endif /* ON_LINUX */

#ifdef ON_WINDOWS
	DWORD access = GENERIC_READ | GENERIC_WRITE;
	DWORD mode = FILE_SHARE_READ | FILE_SHARE_WRITE;
	HANDLE console = CreateFileW(L"CONOUT$", access, mode, NULL,
		OPEN_EXISTING, 0, NULL);

	CONSOLE_SCREEN_BUFFER_INFO screen;
	if (!GetConsoleScreenBufferInfo(console, &screen))
		return;

    	*lines = screen.srWindow.Bottom - screen.srWindow.Top + 1;
	*columns = screen.srWindow.Right - screen.srWindow.Left + 1;
#endif /* ON_WINDOWS */
}

void terminal_restore()
{
#ifdef ON_LINUX
	tcsetattr(0, TCSANOW, &inital_settings);
#endif /* ON_LINUX */
}

void terminal_setup()
{
#ifdef ON_LINUX
	struct termios new;
	tcgetattr(0, &inital_settings);

	new = inital_settings;

	new.c_lflag &= ~ECHO;
	new.c_lflag &= ~ISIG;
	new.c_lflag &= ~ICANON;

	new.c_cc[VMIN] = 0;
	new.c_cc[VTIME] = 0;

	tcsetattr(0, TCSANOW, &new);
#endif /* ON_LINUX */

#ifdef ON_WINDOWS
	DWORD access = GENERIC_READ | GENERIC_WRITE;
	DWORD mode = FILE_SHARE_READ | FILE_SHARE_WRITE;
	HANDLE console = CreateFileW(L"CONOUT$", access, mode, NULL,
		OPEN_EXISTING, 0, NULL);

	if (!GetConsoleMode(console, &mode)) {
		printf("GetConsoleMode error: %ld\n", GetLastError());
		return;
	}

	mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	if (!SetConsoleMode(console, mode)) {
		printf("SetConsoleMode error: %ld\n", GetLastError());
		return;
	}
#endif /* ON_WINDOWS */
}

char getchar_nonblock()
{
#ifdef ON_LINUX
	return getchar();
#endif /* ON_LINUX */

#ifdef ON_WINDOWS
	if (_kbhit())
		return _getch();
#endif /* ON_WINDOWS */

	return EOF;
}

void clamp_in_terminal(struct position *p)
{
	p->x = clamp_inside(p->x, 0, term_w);
	p->y = clamp_inside(p->y, 0, term_h);	
}

void clear_screen()
{
	cursor_move((struct position){ 0, 0 });
	printf("%c[2J", ASCII_ESC);
}

void clear_line(int y)
{
	cursor_move((struct position){ 0, y });
	printf("%d[2K", ASCII_ESC);
}

void print_centered(int y, char *string)
{
	cursor_move((struct position){
		(term_w * 0.5) - (strlen(string) / 2),
		y,
	});

	printf("%s", string);
}

void print_centered_block(char **lines, int lines_num)
{
	for (int i = 0; i < lines_num; i++) {
		int y = (term_h * 0.5) - (lines_num / 2) + i;

		print_centered(y, lines[i]);
	}
}

int remove_array_item(void *array, int index, int length, size_t item_size)
{
	size_t byte_offset = item_size * index;
	size_t new_size = item_size * (length - index - 1);

	char *arr = (char *)array;

	memmove(arr + byte_offset, arr + byte_offset + item_size, new_size);

	return length - 1;
}

static inline int clamp_inside(int target, int min, int max)
{
	return target < min ? min : target > max ? max : target;
}