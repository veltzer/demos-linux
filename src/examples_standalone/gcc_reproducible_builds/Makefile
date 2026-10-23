include ../../../Makefile.mk

# Build the same source more than once and compare the executables byte for
# byte. Each check states the expected outcome and fails the build if gcc
# does not deliver it, so this example also guards the claims in README.md.
CFLAGS:=-I$(CURDIR)/../../include -O2 -g -Wall -Werror

# $(call expect,same|differ,file1,file2)
define expect
	$(Q)if cmp -s $(2) $(3); then got=same; else got=differ; fi;\
	echo "  $(2) vs $(3): $$got (expected $(1))";\
	test $$got = $(1)
endef

.PHONY: all
all: check_twice check_date check_dir
	$(info doing [$@])

# 1. same source, same flags, same directory: identical
.PHONY: check_twice
check_twice:
	$(info doing [$@])
	$(Q)$(CC) $(CFLAGS) -o twice1.$(SUFFIX_BIN) main.c
	$(Q)$(CC) $(CFLAGS) -o twice2.$(SUFFIX_BIN) main.c
	$(call expect,same,twice1.$(SUFFIX_BIN),twice2.$(SUFFIX_BIN))

# 2. __DATE__/__TIME__ embed the build time; SOURCE_DATE_EPOCH pins it
.PHONY: check_date
check_date:
	$(info doing [$@])
	$(Q)SOURCE_DATE_EPOCH=0 $(CC) $(CFLAGS) -o date_epoch0.$(SUFFIX_BIN) stamp.c
	$(Q)SOURCE_DATE_EPOCH=86400 $(CC) $(CFLAGS) -o date_epoch1.$(SUFFIX_BIN) stamp.c
	$(Q)SOURCE_DATE_EPOCH=0 $(CC) $(CFLAGS) -o date_epoch0_again.$(SUFFIX_BIN) stamp.c
	$(call expect,differ,date_epoch0.$(SUFFIX_BIN),date_epoch1.$(SUFFIX_BIN))
	$(call expect,same,date_epoch0.$(SUFFIX_BIN),date_epoch0_again.$(SUFFIX_BIN))

# 3. two checkouts in different directories: the build directory lands in
# the debug info (DW_AT_comp_dir) and in __FILE__, unless -ffile-prefix-map
# rewrites it
.PHONY: check_dir
check_dir:
	$(info doing [$@])
	$(Q)a=$$(mktemp -d) && b=$$(mktemp -d) && cp main.c $$a && cp main.c $$b &&\
	(cd $$a && $(CC) $(CFLAGS) -o $(CURDIR)/dir_a.$(SUFFIX_BIN) $$a/main.c) &&\
	(cd $$b && $(CC) $(CFLAGS) -o $(CURDIR)/dir_b.$(SUFFIX_BIN) $$b/main.c) &&\
	(cd $$a && $(CC) $(CFLAGS) -ffile-prefix-map=$$a=. -o $(CURDIR)/dir_map_a.$(SUFFIX_BIN) $$a/main.c) &&\
	(cd $$b && $(CC) $(CFLAGS) -ffile-prefix-map=$$b=. -o $(CURDIR)/dir_map_b.$(SUFFIX_BIN) $$b/main.c);\
	status=$$?; rm -rf $$a $$b; exit $$status
	$(call expect,differ,dir_a.$(SUFFIX_BIN),dir_b.$(SUFFIX_BIN))
	$(call expect,same,dir_map_a.$(SUFFIX_BIN),dir_map_b.$(SUFFIX_BIN))

# not part of all: shows where two differing binaries differ
.PHONY: diff
diff: check_dir
	$(info doing [$@])
	$(Q)readelf --debug-dump=info dir_a.$(SUFFIX_BIN) | grep -m2 -E 'DW_AT_(name|comp_dir)'
	$(Q)readelf --debug-dump=info dir_b.$(SUFFIX_BIN) | grep -m2 -E 'DW_AT_(name|comp_dir)'

.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f *.$(SUFFIX_BIN)
