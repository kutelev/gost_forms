CC    = gcc
STRIP = strip
CXX   = g++
AR    = ar

CPPFLAGS += -Wall -Os

SEP = ~
EMPTY =
SPACE = $(EMPTY) $(EMPTY)

APPNAME := $(subst $(SPACE),$(SEP),$(CURDIR))
APPNAME := $(notdir $(APPNAME))
APPNAME := $(subst lib,,$(APPNAME))
APPNAME := lib$(APPNAME).a

OBJECTS = $(patsubst %.cxx,%.o,$(wildcard *.cxx))

.DEFAULT_GOAL := all

.PHONY : all
.PHONY : clean

all: $(APPNAME)

%.o : %.cxx
	$(CXX) -c $(CPPFLAGS) $(CFLAGS) -o $@ $<

$(APPNAME): $(OBJECTS)
	$(AR) -cr $@ $^
	cp $@ ..


clean:
	rm -rf $(APPNAME) *.o
