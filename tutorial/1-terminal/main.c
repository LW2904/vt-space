#include "space.h"

#include <stdio.h>

int main()
{
	/* Disable stdout buffering. */
	setbuf(stdout, NULL);

	int err;

	if ((err = setup_terminal())) {
		printf("setup_terminal error: %d\n", err);
		return EXIT_FAILURE;
	}

	clear_terminal();
	
	int term_w, term_h;
	if ((err = get_terminal_dimensions(&term_w, &term_h))) {
		printf("get_terminal_dimensions error: %d\n", err);
		return EXIT_FAILURE;
	}

	printf("terminal dimensions: %d/%d\n", term_w, term_h);

	return EXIT_SUCCESS;
}
