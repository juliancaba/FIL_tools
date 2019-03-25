# -*- mode: makefile-gmake; coding: utf-8 -*-
DESTDIR ?= ~

FIL_ROOT  = /fil_testing


install:
	install -vd $(DESTDIR)$(FIL_ROOT)/bin
	install -v -m 755 ./*.py $(DESTDIR)$(FIL_ROOT)/bin
	install -vd $(DESTDIR)$(FIL_ROOT)/src/c2IPsrc/templates
	install -v -m 444 ./templates/*.tmpl $(DESTDIR)$(FIL_ROOT)/src/c2IPsrc/templates
	install -vd $(DESTDIR)$(FIL_ROOT)/src/c2IPsrc/static
	install -v -m 444 ./static/* $(DESTDIR)$(FIL_ROOT)/src/c2IPsrc/static





