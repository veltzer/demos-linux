# This makefile shows how to ignore errors in the build process

.PHONY: ignore_dot
.IGNORE: ignore_dot
ignore_dot:
	exit 1

.PHONY: ignore_minus
ignore_minus:
	-exit 1

.PHONY: ignore_no_help
ignore_no_help:
	@ls nonexistant 2> /dev/null || exit 0
