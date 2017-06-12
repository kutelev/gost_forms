all : $(TARGET)

$(TARGET) : $(SEP_PAGES)
	$(VERBOSE)echo "  MERGE      -> $@"
	$(VERBOSE)$(PDFTK) $^ cat output $(MERGED_PAGES)
	$(VERBOSE)$(RENAME) $(MERGED_PAGES) "$@"
