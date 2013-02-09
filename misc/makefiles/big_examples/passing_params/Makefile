FOO:=foo
# causes all variables to be exported to sub-make (best way)
#export
# causes all variables to be exported to sub-make (other way)
#.EXPORT_ALL_VARIABLES: foo

.PHONY: all
all:
	$(MAKE) -C inner
