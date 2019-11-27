include ../build/cspl.mk

include_path = $(CSPL_INCLUDES)

LIBOBJ = libcspl.a libporting.a
LOCALOBJ = sample-main.o sample-stack.o
CSPL=..

targets = sample
	
default: $(targets)

$(LIBOBJ):
	$(MAKE) -f $(CSPL)/build/Makefile ROOTDIR=$(CSPL)/build/ default

$(LOCALOBJ): %.o: %.c
	$(CSPL_COMPILE) $(CSPL_FLAGS) -c $(include_path) -I../samples -c $< -o $@

sample: $(LOCALOBJ) $(LIBOBJ)
	$(CSPL_LINKEXE) $(CSPL_FLAGS) -o $@ $(LOCALOBJ) $(LIBOBJ) $(CSPL_LIBRARIES)

clean:
	rm -f $(targets) $(LIBOBJ) $(LOCALOBJ) core gmon.out
	$(MAKE) -f $(CSPL)/build/Makefile ROOTDIR=$(CSPL)/build/ clean
