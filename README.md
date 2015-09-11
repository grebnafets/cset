# cset
##Small C toy projects

---

Node that these toy projects can depend on each other, it is better to think of this project as a whole.

---

### ts

Project is simply an abstraction for ansi escape codes.

### test

![alt tag](https://raw.githubusercontent.com/grebnafets/cset/master/test/images/demo.valid.1.jpg)

"test" function is basically an assert function on steroids. Node that ansi escape codes are used for detailed display. It would be a good idea to filter details out and get only the results if you want to run many tests at the same time via custom made script.

There are several modes to use that are set via "test_set(size_t mode)" function, namely:

| Mode | Display |
|:----:|:-------:|
| 1    | failure |
| 2    | success |
| 4    | results |

You can then use combo of any of the three flags, just like with linux permissions. 7 displays all three like the image above shows.

Currently, it does not support threads, but that feature will be added later.

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
