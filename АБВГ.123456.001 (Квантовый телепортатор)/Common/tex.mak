PRE_SOURCE := $(wildcard *.tex)
SOURCE := $(subst $(SPACE),$(SSPACE),$(PRE_SOURCE))
PRE_SOURCE := $(SOURCE)
SOURCE := $(subst $(LEFT_PAR),\$(LEFT_PAR),$(PRE_SOURCE))
PRE_SOURCE := $(SOURCE)
SOURCE := $(subst $(RIGHT_PAR),\$(RIGHT_PAR),$(PRE_SOURCE))

BASE_NAME := $(basename $(TARGET))
AUX_FILE := $(BASE_NAME).aux
TOC_FILE := $(BASE_NAME).toc
BIB_FILE := $(basename $(TARGET)).bib
OLD_AUX_FILE := $(BASE_NAME).aux_old
OLD_TOC_FILE := $(BASE_NAME).toc_old

PNG_PICS = $(wildcard ./Pics/*.png)
PDF_PICS = $(patsubst %.svg,%.pdf,$(wildcard ./Pics/*.svg))
TEX_PICS = $(patsubst %.tex,%.pdf,$(wildcard ./Pics/*.tex))

PICS = $(PNG_PICS) $(PDF_PICS) $(TEX_PICS)

FORM_ORDER = 2

FORM_DIR = $(wildcard Form)

ifeq ($(FORM_DIR),)
FORM_DIR = $(wildcard Form2)
endif

ifeq ($(FORM_DIR),)
FORM_DIR = $(wildcard Form1)
ifneq ($(FORM_DIR),)
FORM_ORDER = 1
ifneq ($(wildcard ./$(FORM_DIR)/*.pdf),)
NUM_PAGES := $(shell $(PDFTK) "./$(FORM_DIR)/$(FORM_DIR).pdf" dump_data | grep NumberOfPages | awk '{print $$2}')
NUM_PAGES := \def\numpages{$(NUM_PAGES)}
endif
endif
endif

.PHONY : aux_toc

.NOTPARALLEL : aux_toc tex

all : aux_toc tex

aux_toc : $(AUX_FILE) $(TOC_FILE) $(OLD_AUX_FILE) $(OLD_TOC_FILE)

$(OLD_AUX_FILE) $(OLD_TOC_FILE) : $(SOURCE)

$(AUX_FILE) :
	touch $@

$(TOC_FILE) :
	touch $@

$(OLD_AUX_FILE) :
	touch $@

$(OLD_TOC_FILE) :
	touch $@

Pics/%.pdf : Pics/%.svg
	$(INKSCAPE) --export-pdf="$(CURDIR)/$@" "$(CURDIR)/$<"

$(TARGET) : $(PICS)

XETEX_FLAGS = -interaction=nonstopmode --output-driver="xdvipdfmx -V 5" -synctex=1

.PHONY : tex

ifeq ($(FORM_ORDER),1)
tex : form
endif

tex :
	$(MAKE) $(TARGET)

$(TARGET) : $(SOURCE) $(ROOT_DIR)/Common/*.sty
	$(VERBOSE)echo "  TEX first pass ..."
	$(VERBOSE)TEXINPUTS=".:$(ROOT_DIR)/Common:${TEXINPUT}" $(XETEX) $(XETEX_FLAGS) -jobname=$(basename $(TARGET)) "\def\pdf{}$(NUM_PAGES)\input{$<}" $(SUPPRESS_OUTPUT)

	$(VERBOSE)if [ -f $(BIB_FILE) ]; \
	then \
		echo "  BIBTEX ..."; \
		$(BIBTEX) $(basename $(BIB_FILE)) $(SUPPRESS_OUTPUT); \
	fi

	$(VERBOSE)if [ -f $(BIB_FILE) ] || ! cmp $(AUX_FILE) $(OLD_AUX_FILE) $(SUPPRESS_OUTPUT) || ! cmp $(TOC_FILE) $(OLD_TOC_FILE) $(SUPPRESS_OUTPUT); \
	then \
		echo "  TEX second pass ..."; \
		TEXINPUTS=".:$(ROOT_DIR)/Common:${TEXINPUT}" $(XETEX) $(XETEX_FLAGS) -jobname=$(basename $(TARGET)) "\def\pdf{}$(NUM_PAGES)\input{$<}" $(SUPPRESS_OUTPUT); \
		cp $(AUX_FILE) $(OLD_AUX_FILE); \
		cp $(TOC_FILE) $(OLD_TOC_FILE); \
	fi

	$(VERBOSE)if [ -f $(BIB_FILE) ] || ! cmp $(AUX_FILE) $(OLD_AUX_FILE) $(SUPPRESS_OUTPUT) || ! cmp $(TOC_FILE) $(OLD_TOC_FILE) $(SUPPRESS_OUTPUT); \
	then \
		echo "  TEX third pass ..."; \
		TEXINPUTS=".:$(ROOT_DIR)/Common:${TEXINPUT}" $(XETEX) $(XETEX_FLAGS) -jobname=$(basename $(TARGET)) "\def\pdf{}$(NUM_PAGES)\input{$<}" $(SUPPRESS_OUTPUT); \
	fi

ifneq ($(FORM_DIR),)
	$(MAKE) overlay
endif

.PHONY : form

form:
	$(MAKE) -C $(FORM_DIR)

.PHONY : overlay

overlay:
ifeq ($(FORM_ORDER),2)
	$(MAKE) form
endif
	$(VERBOSE)echo "  TEX overlay pass ..."
	$(VERBOSE)TEXINPUTS=".:$(ROOT_DIR)/Common:${TEXINPUT}" $(XETEX) $(XETEX_FLAGS) -jobname=$(basename $(TARGET)) "\def\pdf{}\def\overlaypass{./$(FORM_DIR)/$(FORM_DIR).pdf}$(NUM_PAGES)\input{$(SOURCE)}" $(SUPPRESS_OUTPUT)

clean:
	rm -rf *.pdf *.log *.aux* *.toc* *.out *.bbl *.blg *.lg *.html *.css *.dvi *.4* *.id* *.tmp *.xref *.synctex* $(PDF_PICS) $(TEX_PICS)
ifneq ($(FORM_DIR),)
	$(MAKE) -C $(FORM_DIR) $@
endif
	rm -rf tmp*.*
