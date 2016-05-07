#include <cset/test/test.h>
#include <cset/prof/prof.h>

int main(int argc, char **argv)
{
	size_t mode = test_get_mode_from_arg(argc, argv);
	test_set(mode, "prof");
	char name[32];
	size_t count = 0;
	clock_t t1, t2;
        t1 = clock();
	for (int i = 0; i < 10; i += 1) {
		memset(name, '\0', 32);
		sprintf(name, "%s%d", "number_", i);
		t2 = clock();
		for (int j = 0; j < 1000; j += 1) {
			count++;
		}
		prof_sec(t2, name);
	}
	prof_sec(t1, "first");

	return EXIT_SUCCESS;
}
