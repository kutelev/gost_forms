PARTS_DIRS    := $(wildcard Part??)
PARTS         := $(patsubst %,%/%.pdf,$(wildcard Part??))
PARTS         := $(shell find -maxdepth 1 -name "Part??" -type d -exec echo '{}'/'{}'.pdf \; | sort | sed "s/\/.\//\//g")
PARTS_COUNT   := $(words $(PARTS))
MERGED        := merged.pdf

MERGED_TARGET := $(subst $(SPACE),$(SEP),$(CURDIR))
MERGED_TARGET := $(notdir $(MERGED_TARGET))
MERGED_TARGET := $(subst $(SEP),\ ,$(MERGED_TARGET)).pdf

.PHONY : parts_dirs_clean
.PHONY : $(PARTS_DIRS)


all : $(MERGED_TARGET)

$(MERGED_TARGET) : $(PARTS)
ifeq ($(PARTS_COUNT),1)
	$(VERBOSE)echo "  COPY       -> $(subst \ , ,$(MERGED_TARGET))"
	$(VERBOSE)$(COPY) $(PARTS) "$(subst \ , ,$(MERGED_TARGET))"
else
	$(VERBOSE)echo "  MERGE      -> $(MERGED)"
	$(VERBOSE)$(PDFTK) $(PARTS) cat output $(MERGED)
	$(VERBOSE)echo "  RENAME  $(MERGED) -> $(subst \ , ,$(MERGED_TARGET))"
	$(VERBOSE)$(RENAME) $(MERGED) "$(subst \ , ,$(MERGED_TARGET))"
endif

$(PARTS) : $(PARTS_DIRS)
	$(MAKE) -C $(dir $@)

$(PARTS_DIRS) :
	$(MAKE) -C $@

clean : parts_dirs_clean
	rm -rf *.pdf

parts_dirs_clean :
	$(foreach dir,$(PARTS_DIRS),$(MAKE) -C $(dir) clean; )
