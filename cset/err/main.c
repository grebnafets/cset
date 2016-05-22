#include <stdio.h>
#include <stdlib.h>
//#define CSET_ERR_PRE printf("%s\n", __func__);
//#define CSET_ERR_POST printf("%s\n", __func__);
#include <cset/test/test.h>
#include <cset/err/err.h>

int main(int argc, char **argv)
{
	char *err = cset_err("My error");
	printf("%s\n", err);
	int cleared = cset_err_clear(err);
	printf("%d\n", cleared);
	return EXIT_SUCCESS;
}
