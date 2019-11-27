#####################################################
# Inclusion of all variables and rules defined in mk file
#####################################################
include ${X2AP_DIR}/x2ap.mk

export SOURCE = $(shell pwd)

#LIBNAME = $(X2AP_INIT_LIB)
DESTINATION = $(SOURCE)/../obj
VPATH = ./:$(DESTINATION)


#TARGET START
all:$(OBJECTS)
	$(CP) $(SOURCE)/*.o $(X2AP_DIR)/build/obj/.

clean:
	$(DELETE) $(SOURCE)/*.o

#TARGET END

