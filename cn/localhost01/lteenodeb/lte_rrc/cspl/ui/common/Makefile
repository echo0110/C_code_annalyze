
ifndef ROOTDIR
	ROOTDIR = ../../build/
endif

include ../../build/cspl.mk

include_path = -I . -I ../../h -I ../../ports/$(PORT)
libraries = $(CSPL_LIBRARIES)

readline_inc = -I/user/trimode/native.sun/include
readline_lib = -L/user/trimode/native.sun/lib -lreadline -lcurses
include_path += $(readline_inc)
libraries += $(readline_lib)
libraries += -lposix4 -lsocket -lxnet -lpthread

LIBOBJ = ylib-list.o ylib-bufop.o
LOCALOBJ = ui.o util.o main.o
PORTOBJ = os.o
DRIVEROBJ = 

targets = ui
	
default: $(targets)

$(PORTOBJ): %.o: ../../ports/$(PORT)/%.c
	$(CSPL_COMPILE) $(CSPL_FLAGS) -c $(include_path) -c $< -o $@

$(DRIVEROBJ): %.o: ../../drivers/$(DRIVER)/%.c
	$(CSPL_COMPILE) $(CSPL_FLAGS) -c $(include_path) -c $< -o $@

$(LOCALOBJ): %.o: %.c
	$(CSPL_COMPILE) $(CSPL_FLAGS) -c $(include_path) -c $< -o $@

%.o: ../../src/%.c
	$(CSPL_COMPILE) $(CSPL_FLAGS) -c $(include_path) -c $< -o $@

ui: $(LOCALOBJ) $(PORTOBJ) $(LIBOBJ)
	$(CSPL_LINKEXE) $(CSPL_FLAGS) -o $@ $(LOCALOBJ) $(PORTOBJ) $(LIBOBJ) $(libraries)

clean:
	rm -f $(targets) $(LIBOBJ) $(LOCALOBJ) $(PORTOBJ) $(DRIVEROBJ) core gmon.out
