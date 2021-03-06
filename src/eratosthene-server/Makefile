#
#   makefile - configuration
#

    # c/cpp, suite-base/suite/library #
    MAKE_NAME:=eratosthene-server
    MAKE_LANG:=c
    MAKE_TYPE:=suite

    MAKE_DBIN:=bin
    MAKE_DDOC:=doc
    MAKE_DLIB:=lib
    MAKE_DSRC:=src
    MAKE_DOBJ:=obj

    MAKE_OSYS:=$(shell uname -s)

    MAKE_CMKD:=mkdir -p
    MAKE_CRMF:=rm -f
    MAKE_CRMD:=rm -rf
    MAKE_CFCP:=cp

    MAKE_CCMP:=gcc
ifeq ($(MAKE_OSYS),Linux)
    MAKE_CLNK:=gcc-ar rcs
else
ifeq ($(MAKE_OSYS),Darwin)
    MAKE_CLNK:=ar -rv
endif
endif
    MAKE_CDOC:=doxygen

    MAKE_FCMP:=-std=c99 -O3 -Wall -Wno-deprecated -funsigned-char -pedantic -D _DEFAULT_SOURCE -flto
    MAKE_FLNK:=-lm -lpthread -flto

#
#   makefile - configuration
#

ifneq (,$(filter $(MAKE_TYPE),suite libcommon))
    MAKE_SWAP:=../../
else
    MAKE_SWAP:=
endif
ifeq ($(MAKE_TYPE),libcommon)
    MAKE_SUBS:=$(filter-out $(MAKE_SWAP)$(MAKE_DLIB)/libcommon, $(wildcard $(MAKE_SWAP)$(MAKE_DLIB)/*))
else
    MAKE_SUBS:=$(wildcard $(MAKE_SWAP)$(MAKE_DLIB)/*)
endif
    MAKE_SEGS:=$(wildcard $(MAKE_DSRC)/*)
    MAKE_FSRC:=$(wildcard $(MAKE_DSRC)/*.$(MAKE_LANG))
    MAKE_FOBJ:=$(addprefix $(MAKE_DOBJ)/,$(addsuffix .o,$(notdir $(basename $(MAKE_FSRC)))))
ifeq ($(MAKE_TYPE),suite)
    MAKE_FLNK:=$(addsuffix /bin/*.a,$(MAKE_SUBS)) $(MAKE_FLNK)
else
ifneq (,$(filter $(MAKE_TYPE),library libcommon))
    MAKE_FLNK:=$(addsuffix /bin/*.a,$(MAKE_SUBS))
endif
endif
    MAKE_FCMP:=$(MAKE_FCMP) $(addprefix -I$(MAKE_LIB),$(addsuffix /src,$(MAKE_SUBS)))

#
#   makefile - target
#

    all:module build
    clean-all:clean clean-module
    module:make-module
    clean-module:make-clean-module
ifeq ($(MAKE_TYPE),suite-base)
    build:make-directories make-base
    clean:make-clean-base
    doc:make-module-doc make-segment-doc
    clean-doc:make-clean-module-doc make-clean-segment-doc
else
    build:make-directories make-$(MAKE_NAME)
    clean:make-clean-$(MAKE_NAME)
    all-doc:doc module-doc
    clean-all-doc:clean-doc clean-module-doc
    doc:make-doc
    clean-doc:make-clean-doc
    module-doc:make-module-doc
    clean-module-doc:make-clean-module-doc
endif

#
#   makefile - directive
#

    make-$(MAKE_NAME):$(MAKE_FOBJ)
ifeq ($(MAKE_TYPE),suite)
	$(MAKE_CCMP) -o $(MAKE_DBIN)/$(MAKE_NAME) $^ $(MAKE_FLNK)
else
ifneq (,$(filter $(MAKE_TYPE),library libcommon))
	$(MAKE_CLNK) $(MAKE_DBIN)/$(MAKE_NAME).a $^ $(MAKE_FLNK)
endif
endif

    $(MAKE_DOBJ)/%.o:$(MAKE_DSRC)/%.$(MAKE_LANG)
	$(MAKE_CCMP) -c -o $@ $< $(MAKE_FCMP)

    make-clean-$(MAKE_NAME):
	$(MAKE_CRMF) $(MAKE_DBIN)/* $(MAKE_DOBJ)/*

    make-base:
	@$(foreach SEG, $(MAKE_SEGS), $(MAKE) -C $(SEG) build && $(MAKE_CFCP) $(SEG)/$(MAKE_DBIN)/* $(MAKE_DBIN)/ && ) true

    make-clean-base:
	$(MAKE_CRMF) $(MAKE_DBIN)/*
	@$(foreach SEG, $(MAKE_SEGS), $(MAKE) -C $(SEG) clean && ) true

    make-module:
	@$(foreach LIB, $(MAKE_SUBS), $(MAKE) -C $(LIB) all && ) true

    make-clean-module:
	@$(foreach LIB, $(MAKE_SUBS), $(MAKE) -C $(LIB) clean-all && ) true

    make-doc:
	$(MAKE_CDOC)

    make-clean-doc:
	$(MAKE_CRMD) $(MAKE_DDOC)/html

    make-module-doc:
	@$(foreach LIB, $(MAKE_SUBS), $(MAKE) -C $(LIB) all-doc && ) true

    make-clean-module-doc:
	@$(foreach LIB, $(MAKE_SUBS), $(MAKE) -C $(LIB) clean-all-doc && ) true

    make-segment-doc:
	@$(foreach SEG, $(MAKE_SEGS), $(MAKE) -C $(SEG) doc && ) true

    make-clean-segment-doc:
	@$(foreach SEG, $(MAKE_SEGS), $(MAKE) -C $(SEG) clean-doc && ) true

    make-directories:
	$(MAKE_CMKD) $(MAKE_DBIN) $(MAKE_DDOC) $(MAKE_DOBJ)

