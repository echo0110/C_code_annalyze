
ifndef CSPL_FLAGS
	CSPL_FLAGS = -g -O3 -Wall -Wimplicit -Wmissing-prototypes\
                -Wshadow -Wcomment -Waggregate-return\
                -Wmissing-declarations -Wreturn-type \
                -Wparentheses -Wpointer-arith \
                -fno-builtin -fno-inline-functions \
                -fsigned-char -pipe -Wstrict-prototypes \
                -Wno-format -W -fcond-mismatch -Wnested-externs\
                -Wswitch
 
endif

ifndef CSPL_COMPILE
	CSPL_COMPILE =  $(LTE_CC) 
endif

ifndef CSPL_LINKOBJ
	CSPL_LINKOBJ =  $(LTE_LD) -r
endif

ifndef CSPL_LINKEXE
	CSPL_LINKEXE = $(CSPL_COMPILE)
endif

ifndef CSPL_AR
	CSPL_AR = $(LTE_AR) rv
endif

ifndef CSPL_RANLIB
	CSPL_RANLIB =  $(LTE_RANLIB) 
endif

ifndef CSPL_LIBRARIES
	CSPL_LIBRARIES = 
endif


