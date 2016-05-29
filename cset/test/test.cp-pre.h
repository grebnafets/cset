#ifndef CSET_TEST_CROSS_PLATFORM_PRE
#define CSET_TEST_CROSS_PLATFORM_PRE 1

// Possible hooks for every function and do while macro.
// You can test this with #define CSET_TEST_PRE printf("%s\n", __func__);
// Add it _before_ you add test.h
#ifndef CSET_TEST_PRE
	#define CSET_TEST_PRE
#endif
#ifndef CSET_TEST_POST
	#define CSET_TEST_POST
#endif

// dependency {{{
#include <time.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
// }}}

#define CSET_TEST_MODE_SHOW_SUCCESS (size_t)1
#define CSET_TEST_MODE_SHOW_FAILURE (size_t)2
#define CSET_TEST_MODE_SHOW_MESSAGE (size_t)4
#define CSET_TEST_MODE_SHOW_SHEBANG (size_t)7

struct cset_test_case {
	int line;
	char *filename;
	char *funcname;
	char *testname; // 1==1 => "1==1", condition preproccessed to str.
	char *description;
	size_t result;  // 1 or 0 (true or false)
};

struct cset_test_Data {
	char *name;          // Name of the test bundle.
	size_t mode;         // Show {0:nothing, 1:success, 2:failure, 4:msg}
	size_t success;      // Total number of successful cases.
	size_t total;        // Total cases.
	struct cset_test_Data_bg {
		unsigned char success[5]; // {isset, r, g, b, a} a is not supported.
		unsigned char failure[5]; // {isset, r, g, b, a} a is not supported.
		unsigned char result[5];  // {isset, r, g, b, a} a is not supported.
	} bg;
	struct cset_test_Data_fg {
		unsigned char success[5]; // {isset, r, g, b, a} a is not supported.
		unsigned char failure[5]; // {isset, r, g, b, a} a is not supported.
		unsigned char result[5];  // {isset, r, g, b, a} a is not supported.
	} fg;
	struct cset_test_case **cases;
};

// Experimental toy to guard memory for malloc, calloc, realloc and free.
// {{{
static volatile int cset_test_atomic_gate_memory = 0;

static inline void
cset_test_atomic_open
(volatile int *gate)
{CSET_TEST_PRE
	asm volatile (
		"jmp check\n"                // Renegade selected, lets skip the line!
		"wait:\n"                    // Honest citizens wait in line.
		"pause\n"                    // Stroke beard, check phone/watch.
		"check:\n"                   // Ok, lets do this...
		"mov %[lock], %%eax\n"       // eax = 1
		"lock xchg %%eax, %[gate]\n" // Exhange eax with gate value.
		"test %%eax, %%eax\n"        // 1 = closed, 0 = open.
		"jnz wait\n"                 // Ohhh man, here I go again...
		: [gate] "=m" (*gate)
		: [lock] "r" (1)
		: "eax"                      // Inform compiler you want eax.
	);
CSET_TEST_POST
}

static inline void
cset_test_atomic_close
(volatile int *gate)
{CSET_TEST_PRE
	asm volatile (
		"mov %[unlock], %[gate]\n"
		: [gate] "=m" (*gate)
		: [unlock] "r" (0)
	);
CSET_TEST_POST
}

static void *cset_test_malloc(size_t size)
{
	cset_test_atomic_open(&cset_test_atomic_gate_memory);
	void *mem = malloc(size);
	cset_test_atomic_close(&cset_test_atomic_gate_memory);
	return mem;
}

static void *cset_test_calloc(size_t n, size_t size)
{
	cset_test_atomic_open(&cset_test_atomic_gate_memory);
	void *mem = calloc(n, size);
	cset_test_atomic_close(&cset_test_atomic_gate_memory);
	return mem;
}

static void *cset_test_realloc(void *ptr, size_t size)
{
	cset_test_atomic_open(&cset_test_atomic_gate_memory);
	void *mem = realloc(ptr, size);
	cset_test_atomic_close(&cset_test_atomic_gate_memory);
	return mem;
}

static void cset_test_free(void *ptr)
{
	cset_test_atomic_open(&cset_test_atomic_gate_memory);
	free(ptr);
	cset_test_atomic_close(&cset_test_atomic_gate_memory);
}

// }}}

// err {{{
static char *
_cset_test_err(const char *desc, const char *file, const char *func, int line)
{CSET_TEST_PRE
	char *err = NULL;
	size_t desclen, filelen, funclen, len;
	desclen = strlen(desc);
	filelen = strlen(file);
	funclen = strlen(func);
	// I reserve 128 bytes for the representation of line a number.
	// 128 is overkill but who knows. Maybe registers size will
	// change in the future, expanding the need to reserve more
	// bytes for number repesentation.
	len = desclen + filelen + funclen + 128;
	char tmp[len];
	memset(tmp, '\0', len);
	cset_test_atomic_open(&cset_test_atomic_gate_memory);
	sprintf(tmp, "%d:%s:%s:%s", line, func, file, desc);
	cset_test_atomic_close(&cset_test_atomic_gate_memory);
	len = strlen(tmp)+1;
	err = cset_test_calloc(len, sizeof(char));
	if (err == NULL) { // Time to panic.
		fprintf(stderr, "ENOMEM\n");
		fflush(stderr);
		abort();
	}
	cset_test_atomic_open(&cset_test_atomic_gate_memory);
	strncpy(err, tmp, len);
	cset_test_atomic_close(&cset_test_atomic_gate_memory);
CSET_TEST_POST
	return err;
}
#define cset_test_err(desc) _cset_test_err(desc, __FILE__, __func__, __LINE__)
// }}}

#endif // CSET_TEST_CROSS_PLATFORM_PRE
