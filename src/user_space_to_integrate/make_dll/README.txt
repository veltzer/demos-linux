This example shows how to make a shared library on linux.

- compile all sources and link with -shared -fpic.

How to see what you've created:
make show_results
make show_results_verbose

This also demostrates the fact that shared libraries carry their own depenency information with them. You can see this by linking a library when you create your library and then seeing this information in the output of ldd.
