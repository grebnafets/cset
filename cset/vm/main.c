#include <stdio.h>
#include <stdlib.h>
//#define CSET_VM_PRE printf("%s\n", __func__);
//#define CSET_VM_POST printf("%s\n", __func__);
#include <cset/test/test.h>
#include <cset/vm/vm.h>

int main(int argc, char **argv)
{
	printf("hello there\n");
	return EXIT_SUCCESS;
}
