# -*- mode: makefile-gmake; coding: utf-8 -*-
DESTDIR ?= /opt

C2IPSRC_ROOT=./c2IPsrc
RCUNITY_ROOT=./utf-rc-unity

TOOL_ROOTS = $(C2IPSRC_ROOT) $(RCUNITY_ROOT)

install:
	@for i in $(TOOL_ROOTS); do \
	  cd $$i; make -f install.mk DESTDIR=$(DESTDIR); cd - ;\
	done






