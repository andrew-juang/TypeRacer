#include "client.h"

int main() {
	initscr();

	printw("Hello world!");
	refresh();

	getch();
	endwin();

	return 0;
}
