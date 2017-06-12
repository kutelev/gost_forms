SUBDIRS  := $(shell find -maxdepth 1 -type d -name "????.??????.???*")
ifneq ($(SUBDIRS),)
SUBDIRS  += $(shell find -maxdepth 1 -type d -name "???.????????.?????*")
else
SUBDIRS  := $(shell find -maxdepth 1 -type d -name "???.????????.?????*")
endif

SUBDIRS := $(subst $(SPACE),$(SEP),$(SUBDIRS))
SUBDIRS := $(subst $(RIGHT_PAR)$(SEP),$(RIGHT_PAR)$(SPACE),$(SUBDIRS))

SUBDIRS_CLEAN := $(subst $(LEFT_PAR),\$(LEFT_PAR),$(SUBDIRS))
SUBDIRS_CLEAN := $(subst $(RIGHT_PAR),\$(RIGHT_PAR),$(SUBDIRS_CLEAN))

SUBDIRS_ALL := $(subst $(LEFT_PAR),$(LEFT_PAR_SUB),$(SUBDIRS))
SUBDIRS_ALL := $(subst $(RIGHT_PAR),$(RIGHT_PAR_SUB),$(SUBDIRS_ALL))

.NOTPARALLEL : tools subdirs_all

all : tools subdirs_all

clean : subdirs_clean

.PHONY : tools
.PHONY : subdirs_all
.PHONY : subdirs_clean

.PHONY : $(SUBDIRS_ALL)

TOOLS_DIR = $(wildcard Tools)

tools :
ifneq ($(TOOLS_DIR),)
	$(VERBOSE)$(MAKE) -C Tools all
endif

subdirs_all : $(SUBDIRS_ALL)

$(SUBDIRS_ALL) :
	$(VERBOSE)echo "  MAKING DOC   \"$(subst $(SEP), ,$(subst $(LEFT_PAR_SUB),$(LEFT_PAR),$(subst $(RIGHT_PAR_SUB),$(RIGHT_PAR),$@)))\""
	$(VERBOSE)$(MAKE) $(SILENT) -C $(subst $(SEP),\ ,$(subst $(LEFT_PAR_SUB),\$(LEFT_PAR),$(subst $(RIGHT_PAR_SUB),\$(RIGHT_PAR),$@)))

subdirs_clean :
	$(foreach dir,$(SUBDIRS_CLEAN),$(MAKE) -C $(subst $(SEP),\ ,$(dir)) clean; )
ifneq ($(TOOLS_DIR),)
	$(VERBOSE)$(MAKE) -C Tools clean
endif