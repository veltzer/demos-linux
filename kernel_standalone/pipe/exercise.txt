This is a pipe exercise:

create a module that exposes files in /dev of the form:
/dev/mypipe[0-8]

Each pipe represents a PAGE_SIZE buffer in kernel.
Allow reading and writing to the pipe.
