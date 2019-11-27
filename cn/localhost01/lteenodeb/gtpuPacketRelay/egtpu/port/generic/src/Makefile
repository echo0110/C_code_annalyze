#
# Makefile for GTP port files
#

include $(ccm_project_path)/macros.mk

CPPFLAGS += -I../h -I../../../stack/common/h -I../../../stack/fsm/h

lib = $(ccm_project_path)/stack/libegtpu.a

src = egtpu_stack_init.c

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