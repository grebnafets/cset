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
