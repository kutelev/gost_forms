FORM_DIR = $(wildcard Form)

all : $(TARGET)

$(TARGET) : Merge/merged.pdf
	$(VERBOSE)echo "  COPY    ./Merge/merged.pdf -> $(subst \ , ,$(TARGET))"
	$(VERBOSE)cp ./Merge/merged.pdf "$(subst \ , ,$(TARGET))"

Merge/merged.pdf :
	$(VERBOSE)echo "  MAKING BODY ..."
	$(VERBOSE)$(MAKE) $(SILENT) -C Body all
ifneq ($(FORM_DIR),)
	$(VERBOSE)echo "  MAKING FORM ..."
	$(VERBOSE)$(MAKE) $(SILENT) -C Form all
	$(VERBOSE)echo "  MAKING OVERLAY ..."
	$(VERBOSE)$(MAKE) $(SILENT) -C Overlay all
endif
	$(VERBOSE)echo "  MAKING MERGE ..."
	$(VERBOSE)$(MAKE) $(SILENT) -C Merge all

clean :
	$(MAKE) -C Body $@
ifneq ($(FORM_DIR),)
	$(MAKE) -C Form $@
	$(MAKE) -C Overlay $@
endif
	$(MAKE) -C Merge $@
	rm -rf *.pdf