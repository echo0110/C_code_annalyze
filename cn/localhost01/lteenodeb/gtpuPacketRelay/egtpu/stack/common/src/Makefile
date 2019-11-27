#
# Makefile for GTP-U common files
#

include $(ccm_project_path)/macros.mk

CPPFLAGS += -I../h -I../../fsm/h

lib = ../../libegtpu.a

src = egtpu_cdb.c egtpu_list.c egtpu_parse.c egtpu_stat.c \
				egtpu_tmr.c egtpu_trace.c egtpu_util.c

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

