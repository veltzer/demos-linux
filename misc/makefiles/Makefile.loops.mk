
MYEXECUTABLES:=a b c
MYVARIANTS:=debug release
a_INCLUDES:=sadfa asdf asdf asdfad
b_INCLUDES:=binclude1 binclude2
c_INCLUDES:=cinclude1 cinclude2

.PHONY: all
all:
	echo dont call me, you moron!

ACC:=
# this one gets $(1)(signular), $(2)(signular)
define templ_2
$(1)_$(2):
	echo gcc -o $(1)_$(2) -I$($(1)_INCLUDES)
endef

# this one gets $(1)(singular), $(2)(plural)
define templ_other
	$(foreach i,$(2),$(eval $(call templ_2,$(1),$(i))))
endef

# this one gets $(1),$(2)
define template
	$(foreach name,$(1),$(eval $(call templ_other,$(name),$(2))))
endef


$(eval $(call template,$(MYEXECUTABLES),$(MYVARIANTS)))

.PHONY: debug
debug:
	$(info ACC is $(ACC))
