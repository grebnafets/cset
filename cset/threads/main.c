#include <stdio.h>
#include <stdlib.h>
//#define CSET_THREADS_PRE printf("%s\n", __func__);
//#define CSET_THREADS_POST printf("%s\n", __func__);
#include <cset/test/test.h>
#include <cset/threads/threads.h>

int main(int argc, char **argv)
{
	printf("hello foobar\n");
	return EXIT_SUCCESS;
}
