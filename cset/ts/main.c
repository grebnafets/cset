#include <stdio.h>
#include <stdlib.h>
#include <cset/ts/ts.h>

void __pcolor(const char *color, const char *name)
{
	fprintf(stdout, "%s%s%s\n", color, name, TS_RESET);
	fprintf(stdout, "%s%s%s%s\n", TS_BOLD, color, name, TS_RESET);
}
#define pcolor(color) __pcolor(color, #color)

void __parrow(const char *arrow, const char *name)
{
	fprintf(stdout, "%s:%s\n", name, arrow);
}
#define parrow(arrow) __parrow(arrow, #arrow)

int main(void)
{
	pcolor(TS_YELLOW);
	pcolor(TS_RED);
	pcolor(TS_GREEN);
	pcolor(TS_BLUE);
	pcolor(TS_BLACK);
	pcolor(TS_WHITE);
	pcolor(TS_PURPLE);

	parrow(TS_ARROW_DOWN_RIGHT);
	parrow(TS_ARROW_DOWN_LEFT);
	parrow(TS_ARROW_UP_RIGHT);
	parrow(TS_ARROW_UP_LEFT);

	parrow(TS_ARROW_RIGHT_DOWN);
	parrow(TS_ARROW_LEFT_DOWN);
	parrow(TS_ARROW_RIGHT_UP);
	parrow(TS_ARROW_LEFT_UP);

	return EXIT_SUCCESS;
}
