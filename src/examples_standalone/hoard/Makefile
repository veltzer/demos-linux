include ../../../Makefile.mk

# variables
FILENAME:=Hoard-3.10-source.tar.gz
URL:=https://github.com/emeryberger/Hoard/releases/download/3.10/Hoard-3.10-source.tar.gz

# rules
.PHONY: all
all:
	$(info doing [$@])
.PHONY: download
download: $(FILENAME)
	$(info doing [$@])
$(FILENAME):
	$(info doing [$@])
	$(Q)wget $(URL)
.PHONY: clean
clean:
	$(info doing [$@])
	$(Q)-rm -f $(FILENAME)
