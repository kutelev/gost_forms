BODY = $(wildcard ../*.pdf)
ifeq ($(BODY),)
BODY = $(wildcard ../Body/*.pdf)
endif
NUM_PAGES = $(shell $(PDFTK) "$(BODY)" dump_data | grep NumberOfPages | awk '{print $$2}')

TOOL_NAME=formgen

include $(ROOT_DIR)/Common/common.form.mak
