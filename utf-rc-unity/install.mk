# -*- mode: makefile-gmake; coding: utf-8 -*-
DESTDIR ?= ~

RCUNITY_ROOT  = /utf/rc-unity
SRC=./src


install:
	install -vd $(DESTDIR)$(RCUNITY_ROOT)/include
	install -v -m 444 $(SRC)/*.h $(DESTDIR)$(RCUNITY_ROOT)/include
	install -vd $(DESTDIR)$(RCUNITY_ROOT)/src
	install -v -m 444 $(SRC)/*.c* $(DESTDIR)$(RCUNITY_ROOT)/src
	install -vd $(DESTDIR)$(RCUNITY_ROOT)/auto
	install -v -m 444 auto/* $(DESTDIR)$(RCUNITY_ROOT)/auto




