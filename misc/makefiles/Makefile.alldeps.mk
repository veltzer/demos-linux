
HEADERS:=$(shell find . -name "*.h")
ALL_DEPS:=Makefile $(HEADERS)

a: b $(ALL_DEPS)
	$(COMMAND)
