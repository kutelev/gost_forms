ADDITIONAL_ARGUMENTS =

ifeq ($(TOOL_NAME),formgen)
ADDITIONAL_ARGUMENTS = -p $(NUM_PAGES)
endif

all : ./Pages/*.pdf

./Pages/*.pdf : *.kxg $(ROOT_DIR)/../tools/apps/$(TOOL_NAME)/$(TOOL_NAME)
	rm -rf *.svg ./Pages/*.pdf
	(PATH=${PATH}:$(ROOT_DIR)/../tools/apps/$(TOOL_NAME); $(TOOL_NAME) -i $< $(ADDITIONAL_ARGUMENTS))
	$(MAKE) -f makefile.2 all

clean:
	rm -rf *.svg Pages *.pdf
