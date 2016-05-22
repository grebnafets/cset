#ifndef CSET_VM_CROSS_PLATFORM_PRE
#define CSET_VM_CROSS_PLATFORM_PRE 1

// dependency {{{
#include <stdbool.h>
#include <cset/mem/mem.h>
#include <cset/err/err.h>
// }}}

// Possible hooks for every function and do while macro.
// You can test this with #define CSET_VM_PRE printf("%s\n", __func__);
// Add it _before_ you add test.h
#ifndef CSET_VM_PRE
	#define CSET_VM_PRE
#endif
#ifndef CSET_VM_POST
	#define CSET_VM_POST
#endif

#define MOV 1
#define ADD 2
#define CALL 3

struct cset_vm_cmd {
	int cmd;
	void *src, *dst;
};

struct cset_VM {
	struct cset_VM_stack {
		size_t *storage;
		size_t ptr;
	} stack;
	struct cset_VM_line {
		char **raw;
		char **compiled; // compile to assembly.
		struct cset_vm_cmd **decoded;
		size_t pos, len; // program counter and number of lines.
	} line;
	bool isDecoded; // Is set when VM is ready to execute.
};

// Will decode all lines first, then run.
char *cset_vm_decode_line(char *err, const char *line, char **decoded)
{
	if (err != NULL) {
		return err;
	}
	// TODO: strtok get "[cmd] [src], [dest]" and save in decoded.
	return err;
}

#undef MOV
#undef ADD
#undef CALL

#endif // CSET_VM_CROSS_PLATFORM_PRE
