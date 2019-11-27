##
## Default cross-build configuration for vxSim
##

ifndef CSPL_FLAGS
	CSPL_FLAGS = -nostdinc -DNDEBUG -DCPU=SIMSPARCSOLARIS -I/user/trimode/vxworks/target/h -Wall -ansi -pedantic
endif

ifndef CSPL_COMPILE
	CSPL_COMPILE = ccsimso
endif

ifndef CSPL_LINKOBJ
	CSPL_LINKOBJ = ldsimso -r
endif

ifndef CSPL_LINKEXE
	CSPL_LINKEXE = $(CSPL_COMPILE)
endif

ifndef CSPL_AR
	CSPL_AR = arsimso rv

endif

ifndef CSPL_RANLIB
	CSPL_RANLIB = ranlibsimso
endif

ifndef CSPL_LIBRARIES
	CSPL_LIBRARIES =
endif
