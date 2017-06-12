all : Pages/Sheet_01.pdf

Pages/Sheet_01.pdf : Sheet_01.svg
	$(MAKE) -f makefile.2

Sheet_01.svg : $(ROOT_DIR)/Common/Registration_List_Of_Changes.svg
	cp $(ROOT_DIR)/Common/Registration_List_Of_Changes.svg Sheet_01.svg

clean:
	rm -rf *.svg *.pdf Pages
