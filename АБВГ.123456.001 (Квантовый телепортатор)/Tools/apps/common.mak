CC = gcc
STRIP = strip
CXX = g++

CPPFLAGS  = -Wall -Os
CPPFLAGS += -I../../libs/kxsvglib
CPPFLAGS += -I../../libs/kxparsetablelib
CPPFLAGS += -I../../libs/kxcomapplib
CPPFLAGS += -I../../libs/kxgostformlib

LDFLAGS   = -L../../libs
LDFLAGS  += -lkxsvg  -lkxparsetable -lkxcomapp -lkxgostform

SEP = ~
EMPTY =
SPACE = $(EMPTY) $(EMPTY)

APPNAME := $(subst $(SPACE),$(SEP),$(CURDIR))
APPNAME := $(notdir $(APPNAME))

OBJDIR  = obj

OBJECTS = $(addprefix obj/,$(patsubst %.cxx,%.o,$(notdir $(wildcard src/*.cxx))))

.DEFAULT_GOAL := all

.PHONY : all
.PHONY : clean

.NOTPARALLEL : $(OBJDIR) $(APPNAME)

all: $(OBJDIR) $(APPNAME)

$(OBJDIR) :
	mkdir $@

obj/%.o : src/%.cxx
	$(CXX) -c $(CPPFLAGS) -o $@ $<

$(APPNAME): $(OBJECTS)
	$(CXX) -o $@ $^ $(LDFLAGS)
	$(STRIP) $@

clean:
	rm -rf $(APPNAME) $(OBJDIR)
