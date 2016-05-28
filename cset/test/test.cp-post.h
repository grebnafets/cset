#ifndef CSET_TEST_CROSS_PLATFORM_POST
#define CSET_TEST_CROSS_PLATFORM_POST 1

// Prepare memory for data {{{
static char *
cset_test_malloc_data
(char *err, struct cset_test_Data **data)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	if (*data != NULL) {
		err = cset_test_err("Struct parameter is not initialized with NULL.");
		return err;
	}
	*data = cset_test_malloc(sizeof(struct cset_test_Data));
	if (*data == NULL) {
		err = cset_test_err("Allocation failure.");
		return err;
	}
CSET_TEST_POST
	return err;
}

static char *
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
	data->bg.success[0] = false;
	data->bg.failure[0] = false;
	data->bg.result[0] = false;
	data->fg.success[0] = false;
	data->fg.failure[0] = false;
	data->fg.result[0] = false;
CSET_TEST_POST
	return err;
}
// }}}

// Prepare memory for cases {{{
static char *
cset_test_realloc_cases
(char *err, struct cset_test_case ***cases, size_t count)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	size_t size = sizeof(struct cset_test_case) * count;
	*cases = cset_test_realloc(*cases, size);
	if (*cases == NULL) {
		err = cset_test_err("Allocation failure.");
		return err;
	}
	(*cases)[count-1] = NULL;
CSET_TEST_POST
	return err;
}

static char *
cset_test_malloc_case
(char *err, struct cset_test_case **testCase)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	if (*testCase != NULL) {
		err = cset_test_err("Struct parameter is not initialized with NULL.");
		return err;
	}
	*testCase = cset_test_malloc(sizeof(struct cset_test_case));
	if (*testCase == NULL) {
		err = cset_test_err("Allocation failure.");
		return err;
	}
CSET_TEST_POST
	return err;
}

static char *
cset_test_init_case
(char *err, struct cset_test_case *testCase)
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
static char *
cset_test_strcpy
(char *err, char **dstname, const char *srcname)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	if (srcname == NULL) {
		err = cset_test_err("Missing source name. (NULL)");
		return err;
	}
	if (*dstname != NULL) { 
		err = cset_test_err("Destination name is not initialized with NULL.");
		return err;
	}
	int len = strlen(srcname);
	if (len == 0) {
		err = cset_test_err("Missing source name. (\"\")");
		return err;
	}
	*dstname = cset_test_calloc(len+1, sizeof(char));
	if (*dstname == NULL) {
		err = cset_test_err("Allocation failure.");
		return err;
	}
	cset_test_atomic_open(&cset_test_atomic_gate_memory);
	strncpy(*dstname, srcname, len);
	cset_test_atomic_close(&cset_test_atomic_gate_memory);
CSET_TEST_POST
	return err;
}
// }}}

// Case creation {{{
static char *
cset_test_add_case
(
 	char *err,
	const char *filename,
	const char *funcname,
	int line,
	const char *testname,
	const char *description,
	size_t result,
	struct cset_test_case **testCase
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
	(*testCase)->line = line;
	(*testCase)->result = result;
CSET_TEST_POST
	return err;
}
// }}}

// New {{{
char *
cset_test_New
(
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

// Colors {{{
static char *
cset_test_set_bg(
	char *err,
	unsigned char r,
	unsigned char g,
	unsigned char b,
	unsigned char a, // Not supported.
	const char *text,
	char **colortext
)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	// Prevent memory overlapping with a buffer.
	size_t len = strlen(text);
	char buffer[len+1];
	memset(buffer, '\0', len+1);
	cset_test_atomic_open(&cset_test_atomic_gate_memory);
	strncpy(buffer, text, len);
	sprintf(*colortext, "\x1b[48;2;%d;%d;%dm%s\x1b[0m", r, g, b, buffer);
	cset_test_atomic_close(&cset_test_atomic_gate_memory);
CSET_TEST_POST
	return err;
}

static char *
cset_test_set_fg(
	char *err,
	unsigned char r,
	unsigned char g,
	unsigned char b,
	unsigned char a, // Not supported.
	const char *text,
	char **colortext
)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	// Prevent memory overlapping with a buffer.
	size_t len = strlen(text);
	char buffer[len+1];
	memset(buffer, '\0', len+1);
	cset_test_atomic_open(&cset_test_atomic_gate_memory);
	strncpy(buffer, text, len);
	sprintf(*colortext, "\x1b[38;2;%d;%d;%dm%s\x1b[0m", r, g, b, buffer);
	cset_test_atomic_close(&cset_test_atomic_gate_memory);
CSET_TEST_POST
	return err;
}

char *
cset_test_SetBGSuccess(
	char *err,
	unsigned char r,
	unsigned char g,
	unsigned char b,
	unsigned char a,
	struct cset_test_Data *data
)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	data->bg.success[0] = true;
	data->bg.success[1] = r;
	data->bg.success[2] = g;
	data->bg.success[3] = b;
	data->bg.success[4] = a;
CSET_TEST_POST
	return err;
}

char *
cset_test_SetBGFailure(
	char *err,
	unsigned char r,
	unsigned char g,
	unsigned char b,
	unsigned char a,
	struct cset_test_Data *data
)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	data->bg.failure[0] = true;
	data->bg.failure[1] = r;
	data->bg.failure[2] = g;
	data->bg.failure[3] = b;
	data->bg.failure[4] = a;
CSET_TEST_POST
	return err;
}

char *
cset_test_SetBGResult(
	char *err,
	unsigned char r,
	unsigned char g,
	unsigned char b,
	unsigned char a,
	struct cset_test_Data *data
)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	data->bg.result[0] = true;
	data->bg.result[1] = r;
	data->bg.result[2] = g;
	data->bg.result[3] = b;
	data->bg.result[4] = a;
CSET_TEST_POST
	return err;
}

char *
cset_test_SetFGSuccess(
	char *err,
	unsigned char r,
	unsigned char g,
	unsigned char b,
	unsigned char a,
	struct cset_test_Data *data
)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	data->fg.success[0] = true;
	data->fg.success[1] = r;
	data->fg.success[2] = g;
	data->fg.success[3] = b;
	data->fg.success[4] = a;
CSET_TEST_POST
	return err;
}

char *
cset_test_SetFGFailure(
	char *err,
	unsigned char r,
	unsigned char g,
	unsigned char b,
	unsigned char a,
	struct cset_test_Data *data
)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	data->fg.failure[0] = true;
	data->fg.failure[1] = r;
	data->fg.failure[2] = g;
	data->fg.failure[3] = b;
	data->fg.failure[4] = a;
CSET_TEST_POST
	return err;
}

char *
cset_test_SetFGResult(
	char *err,
	unsigned char r,
	unsigned char g,
	unsigned char b,
	unsigned char a,
	struct cset_test_Data *data
)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	data->fg.result[0] = true;
	data->fg.result[1] = r;
	data->fg.result[2] = g;
	data->fg.result[3] = b;
	data->fg.result[4] = a;
CSET_TEST_POST
	return err;
}

// }}}

// Display {{{
static int
cset_test_mode(struct cset_test_Data *data, size_t mode)
{
	return (data->mode & mode) == mode;
}

static char *
cset_test_display_case
(char *err, size_t index, struct cset_test_Data *data)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	size_t result;
	size_t len;
	char *desc = data->cases[index]->description;
	char *cond = data->cases[index]->testname;
	char *func = data->cases[index]->funcname;
	char *file = data->cases[index]->filename;
	int line = data->cases[index]->line;
	const int arbitrary = 1000; // "Reasonable large number to account for number in string.
	len = strlen(desc) + strlen(cond) + strlen(func) + strlen(file) + arbitrary;
	char storage[len];
	char *buf = storage;
	result = data->cases[index]->result;
	memset(buf, '\0', len);
	cset_test_atomic_open(&cset_test_atomic_gate_memory);
	sprintf(buf, "%d:%s:%s:%s:%s", line, cond, func, file, desc);
	cset_test_atomic_close(&cset_test_atomic_gate_memory);
	setbuf(stdout, NULL);
	// Display result if mode is set.
	if (result && cset_test_mode(data, CSET_TEST_MODE_SHOW_SUCCESS)) {
		// Color success {{{
		if (data->fg.success[0]) {
			err = cset_test_set_fg(
				err,
				data->fg.success[1],
				data->fg.success[2],
				data->fg.success[3],
				data->fg.success[4],
				buf, &buf
			);
		}
		if (data->bg.success[0]) {
			err = cset_test_set_bg(
				err,
				data->bg.success[1],
				data->bg.success[2],
				data->bg.success[3],
				data->bg.success[4],
				buf, &buf
			);
		}
		// }}}
		if (err == NULL) {
			printf("%s\n", buf);
		}
	} else if (!result && cset_test_mode(data, CSET_TEST_MODE_SHOW_FAILURE)) {
		// Color failure {{{
		if (data->fg.failure[0]) {
			err = cset_test_set_fg(
				err,
				data->fg.failure[1],
				data->fg.failure[2],
				data->fg.failure[3],
				data->fg.failure[4],
				buf, &buf
			);
		}
		if (data->bg.failure[0]) {
			err = cset_test_set_bg(
				err,
				data->bg.failure[1],
				data->bg.failure[2],
				data->bg.failure[3],
				data->bg.failure[4],
				buf, &buf
			);
		}
		// }}}
		if (err == NULL) {
			printf("%s\n", buf);
		}
	}
CSET_TEST_POST
	return err;
}

// Displays all test cases if mode allowes.
char *
cset_test_Display(char *err, struct cset_test_Data *data)
{CSET_TEST_PRE
	if (err != NULL) {
		return err;
	}
	size_t i, len;
	len = data->total;
	for (i = 0; i < len; i += 1) {
		err = cset_test_display_case(err, i, data);
		if (err != NULL) {
			break;
		}
	}
CSET_TEST_POST
	return err;
}

// }}}

// TODO: cset_test_DisplayResult

// Run test and prepare resulits {{{
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

#endif // CSET_TEST_CROSS_PLATFORM_POST
