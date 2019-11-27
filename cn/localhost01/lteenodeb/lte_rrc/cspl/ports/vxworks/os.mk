##
## Default cross-build configuration for vxSim
##

ifndef CSPL_FLAGS
	CSPL_FLAGS = -nostdinc -DNDEBUG -DCPU=$(VX_CPU) -I$(WIND_BASE)/target/h -Wall -ansi -pedantic -fno-builtin -fvolatile -DVXWORKS -c -W -Wcomment -Wshadow -Wimplicit -Wreturn-type -Wmissing-prototypes -Wnested-externs -Wno-unused -fsigned-char -pipe -Wparentheses -fcond-mismatch -fno-builtin -fno-inline-functions -Wno-format -Wpointer-arith -Wswitch -Waggregate-return 
endif

ifndef CSPL_COMPILE
	CSPL_COMPILE = ccppc
endif

ifndef CSPL_LINKOBJ
	CSPL_LINKOBJ = ldppc -r
endif

ifndef CSPL_LINKEXE
	CSPL_LINKEXE = $(CSPL_COMPILE)
endif

ifndef CSPL_AR
	CSPL_AR = arppc rv
endif

ifndef CSPL_RANLIB
	CSPL_RANLIB = ranlibppc
endif

ifndef CSPL_LIBRARIES
	CSPL_LIBRARIES =
endif

