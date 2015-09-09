# cset
##Small C toy projects

---

### ts

Project is simply an abstraction for ansi escape codes.

### test

![alt tag](https://raw.githubusercontent.com/grebnafets/cset/master/test/images/demo.valid.1.jpg)

"test" function is basically an assert function on steroids. Node that ansi escape codes are used for detailed display. It would be a good idea to filter details out and get only the results if you want to run many tests at the same time via custom made script.

The function has several modes, namely:
	1. -> display failure
	2. -> display success
	4. -> display results
You can then use combo of any of the three flags, just like with linux permissions. 7 displays all three like the shows.
