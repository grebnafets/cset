#ifndef CSET_TEST_CROSS_PLATFORM_PRE
#define CSET_TEST_CROSS_PLATFORM_PRE 1

#define EQU ==
#define NOT !=
#define LTE <=
#define GTE >=
#define LT <
#define GT >

#include <stdio.h>
#include <string.h>

#ifndef CSET_TEST_PRE
	#define CSET_TEST_PRE
#endif
#ifndef CSET_TEST_POST
	#define CSET_TEST_POST
#endif

char cset_test_error[1024];
#define seterr(desc) sprintf(cset_test_error, "%d:%s:%s", __LINE__, __func__, desc); err = cset_test_error

struct cset_test_Case {
	int line;
	char *filename;
	char *funcname;
	char *testname;
	char *description;
	size_t result;
};

struct cset_test_Data {
	char *name;
	size_t mode;
	size_t success;
	size_t total;
	struct cset_test_Case **cases;
};

// Prepare memory for data {{{
char *
cset_test_malloc_data
(char *err, struct cset_test_Data **data)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	if (*data != NULL) {
		seterr("Struct parameter is not initialized with NULL.");
		return err;
	}
	*data = malloc(sizeof(struct cset_test_Data));
	if (*data == NULL) {
		seterr("Allocation failure.");
		return err;
	}
CSET_TEST_POST
	return err;
}

char *
cset_test_init_data
(char *err, struct cset_test_Data *data)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	data->name = NULL;
	data->mode = 0;
	data->total = 0;
	data->success = 0;
	data->cases = NULL;
CSET_TEST_POST
	return err;
}
// }}}

// Prepare memory for cases {{{
char *
cset_test_realloc_cases
(char *err, struct cset_test_Case ***cases, size_t count)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	size_t size = sizeof(struct cset_test_Case) * count;
	*cases = realloc(*cases, size);
	if (*cases == NULL) {
		seterr("Allocation failure.");
	}
	(*cases)[count-1] = NULL;
CSET_TEST_POST
	return err;
}

char *
cset_test_malloc_case
(char *err, struct cset_test_Case **testCase)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	if (*testCase != NULL) {
		seterr("Struct parameter is not initialized with NULL.");
		return err;
	}
	*testCase = malloc(sizeof(struct cset_test_Case));
	if (*testCase == NULL) {
		seterr("Allocation failure.");
		return err;
	}
CSET_TEST_POST
	return err;
}

char *
cset_test_init_case
(char *err, struct cset_test_Case *testCase)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	testCase->line = 0;
	testCase->filename = NULL;
	testCase->funcname = NULL;
	testCase->testname = NULL;
	testCase->description = NULL;
	testCase->result = 0;
CSET_TEST_POST
	return err;
}
// }}}

// String handling {{{
char *
cset_test_strcpy
(char *err, char **dstname, const char *srcname)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	if (*dstname != NULL) { 
		seterr("Destination name is not initialized with NULL.");
		return err;
	}
	int len = strlen(srcname);
	if (len == 0) {
		seterr("Missing source name.");
		return err;
	}
	*dstname = calloc(len+1, sizeof(char));
	if (*dstname == NULL) {
		seterr("Allocation failure.");
		return err;
	}
	strncpy(*dstname, srcname, len);
CSET_TEST_POST
	return err;
}
// }}}

// Case creation {{{
char *
cset_test_add_case
(
 	char *err,
	const char *filename,
	const char *funcname,
	int line,
	const char *testname,
	const char *description,
	size_t result,
	struct cset_test_Case **testCase
)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	err = cset_test_malloc_case(err, testCase);
	err = cset_test_init_case(err, *testCase);
	err = cset_test_strcpy(err, &(*testCase)->filename, filename);
	err = cset_test_strcpy(err, &(*testCase)->funcname, funcname);
	err = cset_test_strcpy(err, &(*testCase)->testname, testname);
	err = cset_test_strcpy(err, &(*testCase)->description, description);
	if (err != NULL) {
		return err;
	}
	if (filename == NULL || strlen(filename) == 0) {
		seterr("Could not determine filename.");
		return err;
	}
	(*testCase)->line = line;
	(*testCase)->result = result;
CSET_TEST_POST
	return err;
}
// }}}

// Run test and prepare results {{{
char *
cset_test_Run
(
 	char *err,
	const char *filename,
	const char *funcname,
	int line,
	const char *testname,
	const char *description,
	size_t result,
	struct cset_test_Data *data
)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	size_t index = data->total++;
	err = cset_test_realloc_cases(err, &data->cases, data->total);
	err = cset_test_add_case(
		err,
		filename, funcname, line,
		testname, description, result,
		&data->cases[index]
	);
	if (data->cases[index]->result) {
		data->success++;
	}
CSET_TEST_POST
	return err;
}

#ifndef test
	#define test(err, cond, desc, data) cset_test_Run(err, __FILE__, __func__, __LINE__, #cond, desc, cond, data)
#endif
// }}}

// New {{{
char *
cset_test_New
(CSET_TEST_PRE
 	char *err,
	const char *name,
	size_t mode,
	struct cset_test_Data **data
)
{
	if (err != NULL) {
		return err;
	}
	err = cset_test_malloc_data(err, data);
	err = cset_test_init_data(err, *data);
	err = cset_test_strcpy(err, &(*data)->name, name);
	(*data)->mode = mode;
CSET_TEST_POST
	return err;
}
// }}}

// Cleanup {{{
char *
cset_test_Fini
(char *err, struct cset_test_Data **data)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	while ((*data)->total--) {
		free((*data)->cases[(*data)->total]->filename);
		free((*data)->cases[(*data)->total]->funcname);
		free((*data)->cases[(*data)->total]->testname);
		free((*data)->cases[(*data)->total]->description);
		free((*data)->cases[(*data)->total]);
	}
	free((*data)->name);
	free((*data)->cases);
	free(*data);
	*data = NULL;
CSET_TEST_POST
	return err;
}
// }}}

#endif // CSET_TEST_CROSS_PLATFORM_PRE
