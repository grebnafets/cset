#include <stdio.h>
#include <stdlib.h>
//#define CSET_ERR_PRE printf("%s\n", __func__);
//#define CSET_ERR_POST printf("%s\n", __func__);
#include <cset/err/err.h>

char *func(ERROR)
{
	if (err != NULL) {
		return err;
	}
	err = cset_err("My error");
	printf("%s\n", err);
	int cleared = cset_err_clear(err);
	printf("%d\n", cleared);
	return err;
}

int main(int argc, char **argv)
{
	INITERR;
	err = func(error);
	return EXIT_SUCCESS;
}
