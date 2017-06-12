ADDITIONAL_ARGUMENTS =

ifeq ($(TOOL_NAME),formgen)
ADDITIONAL_ARGUMENTS = -p $(NUM_PAGES)
endif

all : ./Pages/*.pdf

./Pages/*.pdf : *.kxg $(ROOT_DIR)/Tools/apps/$(TOOL_NAME)/$(TOOL_NAME)
	rm -rf *.svg ./Pages/*.pdf
	(PATH=${PATH}:$(ROOT_DIR)/Tools/apps/$(TOOL_NAME); $(TOOL_NAME) -i $< $(ADDITIONAL_ARGUMENTS))
	$(MAKE) -f makefile.2 all

$(ROOT_DIR)/Tools/apps/$(TOOL_NAME)/$(TOOL_NAME) :
	echo "  BUILDING TOOLS..."
	$(MAKE) -C $(ROOT_DIR)/Tools

clean:
	rm -rf *.svg Pages *.pdf
