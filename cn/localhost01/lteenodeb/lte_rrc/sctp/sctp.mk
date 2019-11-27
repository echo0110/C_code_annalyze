


# SCTP stack type
SCTP_STACK=lksctp
SCTP_STACK_TYPE := aricent_sctp_ipr lksctp



#Check stack type
ifeq ($(filter $(SCTP_STACK),$(SCTP_STACK_TYPE)),)
  $(error Unknown stack type '$(SCTP_STACK)'. Known types are: $(foreach m,$(SCTP_STACK_TYPE),'$m'))
endif


ifeq '$(SCTP_STACK)' 'aricent_sctp_ipr'

include ${SIGTRAN_DIR}/sctp_mac.mk


SCTP_BASE_DIR=$(SIGTRAN_DIR)/sctp
SIGTRAN_COMMON_DIR=$(SIGTRAN_DIR)/common
COM_BGTD_PATH= $(SCTP_BASE_DIR)/basic

##################
#  INCLUDES
##################  

ARICENT_IPR_INCLUDES = 		-I$(COM_BGTD_PATH)/stack/co/h\
                            -I$(COM_BGTD_PATH)/stack/err/h\
                            -I$(COM_BGTD_PATH)/stack/mh/h\
                            -I$(COM_BGTD_PATH)/stack/in/h\
                            -I$(COM_BGTD_PATH)/stack/ui/mib/h\
                            -I$(COM_BGTD_PATH)/stack/ui/sm/h\
                            -I$(COM_BGTD_PATH)/stack/ui/su/h\
                            -I$(COM_BGTD_PATH)/stack/ui/co/h\
                            -I$(COM_BGTD_PATH)/stack/port/os_if_linux/h\
                            -I$(COM_BGTD_PATH)/stack/port/tl_if/h\
                            -I$(COM_BGTD_PATH)/client/su/h\
                            -I$(COM_BGTD_PATH)/stack/ui/port/h\
                            -I$(COM_BGTD_PATH)/stack/db/h\
                            -I$(COM_BGTD_PATH)/stack/fsm/h\
                            -I$(COM_BGTD_PATH)/stack/mm/h\
                            -I$(COM_BGTD_PATH)/stack/red/h\
                            -I$(COM_BGTD_PATH)/stack/st/h\
                            -I$(COM_BGTD_PATH)/stack/dbg/h\
                            -I$(COM_BGTD_PATH)/stack/err/h\
                            -I$(COM_BGTD_PATH)/stack/err/port/h\
                            -I$(COM_BGTD_PATH)/stack/port/co/h \
                            -I$(COM_BGTD_PATH)/client/co/h\
                            -I$(COM_BGTD_PATH)/stack/port/e_shell/h \
                            -I$(COM_BGTD_PATH)/stack/port/res/h \
                            -I$(COM_BGTD_PATH)/client/su/h \
                            -I$(COM_BGTD_PATH)/client/sm/h \
                            -I$(COM_BGTD_PATH)/client/su/port/h\
                            -I$(COM_BGTD_PATH)/client/sm/port/h\
                            -I$(COM_BGTD_PATH)/stack/port/cspl/h\
                            -I$(SIGTRAN_DIR)/common/port/h\
                            -I$(SIGTRAN_DIR)/common/port/linux/h\
                            -I$(SIGTRAN_DIR)/common/util/h

SCTP_INCLUDES += ${ARICENT_IPR_INCLUDES}

##################
# FLAGS
##################  

SCTP_FLAGS += -DSIG_COMMON_WITH_CSPL -DSIGTRAN_PORT_LINUX 
SCTP_FLAGS += -DSCTP_IP_HEADER_PARAMS
SCTP_FLAGS += -DSCTP_INET6_SUPPORT

SCTP_DEFINES+=	-DARICENT_SCTP_IPR


##################
#  OBJs
##################  

SCTP_IPR_OBJS=$(SIGTRAN_DIR)/sctp/basic/stack/obj/sctp.o $(SIGTRAN_DIR)/common/port/obj/linux/sig_os.o

endif


ifeq '$(SCTP_STACK)' 'lksctp'

##################
#  OBJs
##################  

SCTP_IPR_OBJS= 

##################
# FLAGS
##################  

SCTP_FLAGS +=  

SCTP_DEFINES+=	-DLKSCTP
SCTP_DEFINES+=  -DLKSCTP_1_0_11
#SPR 23215 CHANGES START
ifeq ($(INTEL_T3300),intel_t3300_ca) 
SCTP_FLAGS += -DINTEL_COMMON
endif
#SPR 23215 CHANGES END


##################
# LIB
##################  

#SPR 23215 CHANGES START
ifeq ($(INTEL_COMMON), 1)
SCTP_LIBS += $(SIGTRAN_DIR)/../rrc/common/lib/lksctp-arm/libsctp.so.1
else
ifeq ($(BIT_64_SYSTEM),1)
SCTP_LIBS += /usr/lib64/libsctp.so.1
else
SCTP_LIBS += /usr/lib64/libsctp.so.1
endif
endif
#SPR 23215 CHANGES END

endif

SCTP_OBJS=${SCTP_IPR_OBJS} 

SCTP_LIBS += -lpthread 


