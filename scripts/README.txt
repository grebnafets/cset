Notice that cset is a symbolic link in each script directory.
Make sure it is right before you run any scripts.

While it is tempting to use hard links, your version control system is
likely to break among other things. Hard links are low level feature that
could be dependend on OS implementation. If I am somehow wrong and you 
can correct me on this, then I will be really happy and I will sing you
bard songs to the end of times. For now, lets just use higher level soft
links.
