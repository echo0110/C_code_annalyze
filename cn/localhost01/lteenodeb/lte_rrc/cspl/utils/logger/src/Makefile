INCLUDES= -I${LOGGER_ROOT}/h

include_path =  $(INCLUDES)

ifdef 	UDP
DEFINES= -DUDP
endif

ifdef 	TCP
DEFINES= -DTCP
endif

ifdef 	ENABLE_TRACE
DEFINES+= -DENABLE_TRACE
endif

LOCALOBJ = common_nw_util.o logger_utl.o logger_flrd.o logger_init.o logger_nw_recv.o logger_print.o logger_main.o

targets = logger.exe 
	
default: $(targets)

$(LOCALOBJ): %.o: %.c
	$(COMPILE) $(C_FLAGS) -c $(DEFINES) $(include_path) -c $< -o $@

logger.exe: $(LOCALOBJ) 
	$(LINKEXE) $(C_FLAGS) -o $@ $(LOCALOBJ) -lsocket -lnsl

clean:
	rm -f $(targets) $(LOCALOBJ) core gmon.out
