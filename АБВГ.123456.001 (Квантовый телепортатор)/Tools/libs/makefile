.DEFAULT_GOAL := all

.PHONY : all
.PHONY : clean

SUBDIRS := $(wildcard *lib)
SUBDIRS := $(subst makefile,,$(SUBDIRS))
SUBDIRS := $(subst common.mak,,$(SUBDIRS))

all :
	$(foreach dir,$(SUBDIRS),$(MAKE) -C $(dir) $@; )

clean :
	$(foreach dir,$(SUBDIRS),$(MAKE) -C $(dir) $@; )
	rm -rf *.a