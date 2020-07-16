PAGES          = $(addprefix Pages/,$(notdir $(sort $(wildcard ../Body/Pages/Sheet_??.pdf))))

.PHONY : pages_files

all : pages_files

pages_files : Pages $(PAGES)

Pages :
	rm -rf Pages
	mkdir Pages

Pages/%.pdf : ../Form/Pages/%.pdf ../Body/Pages/%.pdf
	$(VERBOSE)echo "  OVERLAY       $@"
	$(VERBOSE)$(PDFTK) ../Form/$@ stamp ../Body/$@ output ./$@

clean :
	rm -rf Pages
