#include <stdio.h>
#include <stdlib.h>
//#define CSET_SAFE_PRE printf("%s\n", __func__);
//#define CSET_SAFE_POST printf("%s\n", __func__);
#include <cset/safe/safe.h>

int main(int argc, char **argv)
{
	char foobar[256];
	int count;
	cset_safe_sprintf(count, foobar, "-%s-\n", "foobar");
	printf("%d:%s", count, foobar);
	char line[1024];
	while (1) {
		printf("$ ");
		if (!fgets(line, 1024, stdin)) {
			break;
		}
		system(line);
	}
	return EXIT_SUCCESS;
}
