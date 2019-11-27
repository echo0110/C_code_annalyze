#
# Makefile for GTP-U fsm files
#

include $(ccm_project_path)/macros.mk

CPPFLAGS += -I../h -I../../common/h -I../../init/h

lib = ../../libegtpu.a

src = egtpu_entr.c egtpu_pmm.c egtpu_relay.c egtpu_sap.c egtpu_udi.c

obj = $(src:%.c=%.o)

dep = $(src:%.c=%.d)

#all: $(lib)($(obj))
all:$(obj)	
	ar rv $(lib)  $(obj) 
	
%.o:%.c
	$(COMPILE)  $(CPPFLAGS)  -o $@ $<
	
clean:
	-$(RM) $(obj) *.a *.d
	-$(RM) $(lib)
	
#include $(ccm_project_path)/rules.mk
$(warning COMPILE = $(COMPILE) )