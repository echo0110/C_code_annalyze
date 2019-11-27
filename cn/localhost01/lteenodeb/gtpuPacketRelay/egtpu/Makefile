#
# Top level makefile for EGTPU
#
include macros.mk
all :
	$(MAKE) -C stack $@
	$(MAKE) -C port/generic/src $@

clean distclean :
	$(MAKE) -C stack $@
	$(MAKE) -C port/generic/src $@
