FORM_DIR = $(wildcard ../Form)

ifneq ($(FORM_DIR),)
PAGES = $(wildcard ../Overlay/Pages/Sheet_??.pdf)
else
PAGES = $(wildcard ../Body/Pages/Sheet_??.pdf)
ifeq ($(PAGES),)
PAGES = ../Body/Body.pdf
endif
endif

MERGED = merged.pdf

all : $(MERGED)

$(MERGED) : $(PAGES)
	$(VERBOSE)echo "  MERGE      -> $@"
	$(VERBOSE)$(PDFTK) $^ cat output $@

clean :
	rm -rf *.pdf
