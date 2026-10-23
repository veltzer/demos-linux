# `GCC` reproducible builds

Does compiling and linking the same source twice give byte-identical
programs?

Run `make` - every check builds two programs, compares them with
`cmp(1)` and fails if the outcome is not the expected one:

* `check_twice`: same source, same flags, same directory - identical.
gcc and ld add nothing that changes from run to run; even the
`build-id` is a hash of the contents, not a random number.
* `check_date`: `__DATE__` and `__TIME__` embed the time of compilation,
so builds differ. Setting `SOURCE_DATE_EPOCH` (honored since gcc 7) pins
that time and makes them identical again.
* `check_dir`: the same source built in two different directories differs
with `-g`, because the build directory is recorded in the debug info
(`DW_AT_comp_dir`) and the source path in `__FILE__`.
`-ffile-prefix-map=OLD=NEW` rewrites those paths and makes them identical.

Run `make diff` to see the recorded paths.

Other things that break reproducibility, not shown here: the order of
the linker inputs (unsorted `$(wildcard)`), `ar` without deterministic
mode, different system libraries, and `-frandom-seed` on old compilers
with `-flto`. See <https://reproducible-builds.org>.
