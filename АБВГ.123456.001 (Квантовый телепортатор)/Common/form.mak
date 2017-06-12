NUM_PAGES = $(shell ls -1 ../Body/Pages/Sheet_??.pdf | wc -l)

TOOL_NAME=formgen

include $(ROOT_DIR)/Common/common.form.mak
