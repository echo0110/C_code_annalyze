##############################################################################
## QCLI Makefile #############################################################
##############################################################################
SYSTEM := $(shell uname -s)

CC = $(LTE_CC) -g -O3

AR = ../library/

BASECC = $(CC)

## Definitions ###############################################################
ifeq ($(LTE_CC), aarch64-linux-gnu-gcc)
NORMALCC = $(BASECC) -Wall -W -Wno-unused-parameter -DAARCH64
else
NORMALCC = $(BASECC) -Wall -W -Wno-unused-parameter 
endif
EASYCC = $(BASECC) \
	-Wall -W \
	-Wno-return-type \
	-Wno-unused-parameter \
	-Wno-unused-label \
	-Wno-unused-function \
	-Wno-unused-variable \
	-Wno-missing-braces \
	-Wno-unknown-pragmas 

SRCOBJ = qcli.o qrl.o qval.o qutil.o qmacro.o
HEADERS = \
	$(ROOTDIR)./include/qcli.h \
	$(ROOTDIR)./include/qrl.h \
	$(ROOTDIR)./include/qval.h \
	$(ROOTDIR)./include/qmacro.h \
	$(ROOTDIR)./include/qutil.h

ifeq ($(LTE_CC), aarch64-linux-gnu-gcc)
INCLUDE = -I $(ROOTDIR)./include -I/home/zhanghh/sysroot/include
endif

ifeq ($(LTE_CC), arm-linux-gnueabi-gcc)
INCLUDE = -I $(ROOTDIR)./include -I/home/zhanghh/sysroot/include
endif

INCLUDE += -I $(ROOTDIR)./include

LIBTARGET = libqcli.a

## Targets ###################################################################
all: $(LIBTARGET) 

# The Library
$(SRCOBJ):%.o: $(ROOTDIR)./src/%.c $(HEADERS)
	$(NORMALCC) -c -o $@ $(INCLUDE) $< 

$(LIBTARGET): $(SRCOBJ)
	ar rv $(AR)$(LIBTARGET) $(SRCOBJ)
	ranlib $(AR)$(LIBTARGET)


## Cleanup ###################################################################
clean:
	rm -f $(AR)$(LIBTARGET) 
	rm -f $(SRCOBJ) 

