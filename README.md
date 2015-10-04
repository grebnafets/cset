# cset
##Small C toy projects
--
Tested only on Linux but is developed for *nix systems.
--
## How to prepare.
export environment variable CSET_SEARCH_PATH to contain the path of working
directory.

example: (in .bashrc)

export CSET_SEARCH_PATH=path_to_downloaded_git_repo

The directory "build" contains original makefiles for the whole project.
Doing "make" within the directory "build" will issue "make" within all
subdirectories within "cset". It will also create .so files within
"build/so".

Each project element within directory "cset" contains tests*. You run the
tests by running ./main [0-7] within projects directory.

*More tests need to be added.

---

Node that these toy projects can depend on each other, it is better to think of this project as a whole.

---

### ts

Project is simply an abstraction for ansi escape codes.

### test

(How it currerntly looks)
![alt tag](https://raw.githubusercontent.com/grebnafets/cset/master/cset/test/images/demo.valid.2.jpg)

(Warning, old image, but it shows how you can use it.)
![alt tag](https://raw.githubusercontent.com/grebnafets/cset/master/cset/test/images/demo.invalid.1.jpg)

"test" function is basically an assert function on steroids. Node that ansi escape codes are used for detailed display. It would be a good idea to filter details out and get only the results if you want to run many tests at the same time via custom made script.

There are several modes to use that are set via "test_set(size_t mode, const char[] testname)" function, namely:

| Mode | Display |
|:----:|:-------:|
| 1    | failure |
| 2    | success |
| 4    | results |

You can then use combo of any of the three flags, just like with linux permissions. 7 displays all three like the image above shows.

### support

Sometimes, we just need to know if something is supported on targed platform.

### badcontext

Defines two thread local variables, "bad" and "cntxt".
"cntxt" simply tells what state the program is in now and bad lets us know
if it is bad or not.

### safefork

An attempt to make fork easier via semaphores.

### logger

So we can log stuff...

### f

Abstraction for common file handleling algorithms.

### mem

Abstraction for memory allocations.
The API is simple. The "x" prefix simply means allocate or die.

| mem. | procedure |
|:----:|:---------:|
| xm   | xmalloc   |
| xc   | xcalloc   |
| xr   | xrealloc  |
| m    | malloc    |
| c    | calloc    |
| r    | realloc   |

Example: mem.m is abstraction for malloc

I do not bother setting any guards for threads nor forks as they are most
likely already in place.

### critical

Critical is used when there is no way to recover.

### assemblyline

With assemblyline you create set of functions that act on the
same variable or a structure in sequence.

### threadwrapp

With threadwrapp you can hook threads with pthread_push_pre and
pthread_push_post as long you create the threads with thr.create;
You can also just hook pthread_create like is done in test/test.h.

### hashmap

Just a simple hashmap. Just check hashmap/main.c to see how it is used.
The thing to remember that the default hash function is just something I wrote
out of the blue, I have yet to check how well it distributes arbitrary data.

Anyway, if you know the data beforehand, you should replace the function with
the perfect hash function. You might have to do minor change to the default
function prototype though.
