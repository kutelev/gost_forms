.PHONY : burst
.PHONY : burst_clean

.NOTPARALLEL : aux_toc $(TARGET) burst

all : Pages

Pages : $(TARGET)
	$(VERBOSE)echo "  BURST      ->"
	$(VERBOSE)rm -rf "./Pages"
	$(VERBOSE)cp "$(subst \ , ,$(TARGET))" tmp.pdf
	$(VERBOSE)$(PDFTK) tmp.pdf burst
	$(VERBOSE)rm tmp.pdf
	$(VERBOSE)rename 's/pg_00/Sheet_/' pg_00??.pdf

	$(VERBOSE)rm -rf pg_*.pdf *.txt
	$(VERBOSE)mkdir ./Pages
	$(VERBOSE)mv ./Sheet_*.pdf ./Pages

clean : burst_clean

burst_clean :
	rm -rf ./Pages
