Keywords err, INITER, ERROR, error and __test__ will be reserved by this
header file.

Usage:
```
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
```

The idea is to make every function at higher level to take in err and line
as parameter to make debugging easier. Every value that needs to be acted
on will have to be passed as a pointer because multiple returns are not
allowed in C.
