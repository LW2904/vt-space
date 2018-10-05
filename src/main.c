#include "space.h"

#include <stdio.h>

int main()
{
	int err;

	if (!(err = setup_terminal())) {
		printf("setup_terminal error: %d\n", err);
		return -1;
	}

	clear_terminal();
	
	int term_w, term_h;
	if (!(err = get_terminal_dimensions(&term_w, &term_h))) {
		printf("get_terminal_dimensions error: %d\n", err);
		return -1;
	}

	printf("terminal dimensions: %d/%d\n", term_w, term_h);

	return 0;
}