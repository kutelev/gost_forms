PAGES_DIR     = Pages

SEP_PAGES    := $(patsubst %.svg,$(PAGES_DIR)/%.pdf,$(sort $(wildcard *.svg)))

MERGED_PAGES  = $(PAGES_DIR)/merged.pdf

MAKEFILE_2    = $(wildcard makefile.2)

.PHONY : pages

all : pages
pages : $(SEP_PAGES)

$(SEP_PAGES) : $(PAGES_DIR)

$(PAGES_DIR) :
	mkdir $@


$(PAGES_DIR)/%.pdf : %.svg
	$(VERBOSE)echo "  SVG     $< -> $@"
	$(VERBOSE)$(INKSCAPE) --export-pdf=$@ --without-gui $<

clean:
	rm -rf *.pdf $(PAGES_DIR)
