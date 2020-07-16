SEP = ~
EMPTY =
SPACE = $(EMPTY) $(EMPTY)
SSPACE = \$(SPACE)

LEFT_PAR = (
RIGHT_PAR = )

LEFT_PAR_SUB = <
RIGHT_PAR_SUB = >

VERBOSE = @
SILENT  = -s
SUPPRESS_OUTPUT = 1>/dev/null 2>&1

# dbus-run-session is required due to the following issue:
# https://gitlab.com/inkscape/inkscape/-/issues/294
INKSCAPE      = dbus-run-session inkscape
PDFTK         = pdftk
XETEX         = xelatex
BIBTEX        = bibtex

RENAME        = mv
COPY          = cp

TARGET := $(subst $(SPACE),$(SEP),$(CURDIR))
TARGET := $(notdir $(TARGET))
TARGET := $(subst $(SEP),\ ,$(TARGET)).pdf

.DEFAULT_GOAL := all
.PHONY : all

.DELETE_ON_ERROR :