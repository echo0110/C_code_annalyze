#####################################################
# Inclusion of all variables and rules defined in mk file
#####################################################
ifdef X2AP_DIR

include $(X2AP_DIR)/x2ap.mk


#####################################################
# Pattern Rules
#####################################################

OBJDIR = $(X2AP_DIR)/build/obj/

#---------
COMM_SRC_DIR = $(X2AP_DIR)/common/src/

#source = $(wildcard *.c) $(notdir $(wildcard $(COMM_SRC_DIR)/*.c))
COMM_SRC = $(notdir $(wildcard $(COMM_SRC_DIR)/*.c))
COMM_OBJ = $(COMM_SRC:%.c=$(OBJDIR)/%.o)

#---------
UTILS_SRC_DIR = $(X2AP_DIR)/common/utils/src/
UTILS_SRC = $(notdir $(wildcard $(UTILS_SRC_DIR)/*.c))
UTILS_OBJ = $(UTILS_SRC:%.c=$(OBJDIR)/%.o)

#---------
DB_SRC_DIR = $(X2AP_DIR)/db/src/
DB_SRC = $(notdir $(wildcard $(DB_SRC_DIR)/*.c))
DB_OBJ = $(DB_SRC:%.c=$(OBJDIR)/%.o)

#---------
ENCDEC_SRC_DIR = $(X2AP_DIR)/encdec/src/
ENCDEC_SRC = $(notdir $(wildcard $(ENCDEC_SRC_DIR)/*.c))
ENCDEC_OBJ = $(ENCDEC_SRC:%.c=$(OBJDIR)/%.o)

#---------
INIT_SRC_DIR = $(X2AP_DIR)/init/src/
INIT_SRC = $(notdir $(wildcard $(INIT_SRC_DIR)/*.c))
INIT_OBJ = $(INIT_SRC:%.c=$(OBJDIR)/%.o)

#---------
MSGDIST_SRC_DIR = $(X2AP_DIR)/msgDist/src/
MSGDIST_SRC = $(notdir $(wildcard $(MSGDIST_SRC_DIR)/*.c))
MSGDIST_OBJ = $(MSGDIST_SRC:%.c=$(OBJDIR)/%.o)


#---------
SCTP_SRC_DIR = $(X2AP_DIR)/sctp_intf/src/
SCTP_SRC = $(notdir $(wildcard $(SCTP_SRC_DIR)/*.c))
SCTP_OBJ = $(SCTP_SRC:%.c=$(OBJDIR)/%.o)


#---------
SU_SRC_DIR = $(X2AP_DIR)/su/src/
SU_SRC = $(notdir $(wildcard $(SU_SRC_DIR)/*.c))
SU_OBJ = $(SU_SRC:%.c=$(OBJDIR)/%.o)

#---------
X2APSM_SRC_DIR = $(X2AP_DIR)/x2apsm/src/
X2APSM_SRC = $(notdir $(wildcard $(X2APSM_SRC_DIR)/*.c))
X2APSM_OBJ = $(X2APSM_SRC:%.c=$(OBJDIR)/%.o)



#######################################
# X2AP RULES
######################################
all:$(X2AP_CSPL_LIBS) $(SCTP_LIBS) x2ap
clean: clean_x2ap

x2ap: x2ap_make

.PHONY:x2ap_make 
x2ap_make:create_dir $(COMM_OBJ) $(UTILS_OBJ) $(DB_OBJ) $(ENCDEC_OBJ) $(INIT_OBJ) $(MSGDIST_OBJ)  $(SCTP_OBJ)  $(SU_OBJ)  $(X2APSM_OBJ)
	$(CP) -f $(X2AP_DIR)/../rrc/bld/$(MODE)/obj/x2ap_asn_enc_dec_3gpp.o $(X2AP_LIB_OBJ_PATH)
	cd $(X2AP_LIB_OBJ_PATH) && $(ARCHIVE) $(X2AP_LIB) *.o
	$(MOVE) $(X2AP_LIB_OBJ_PATH)/$(X2AP_LIB) $(X2AP_DIR)/../rrc/bld/$(MODE)/lib


ifeq ($(OAM_STUB), 0)
ifeq ($(X2AP_UNIT_TESTING), 1)
	#Create Exe, since X2AP is StandAlone #
	$(LINKER)
	$(MOVE) x2ap.exe $(X2AP_DIR)/build/
endif
endif


ifeq ($(OAM_STUB), 1)
stub:   all
	cd $(X2AP_DIR)/test/stub/src && $(MAKE)
	$(STUB_LINKER)
	$(MOVE) stub.exe $(X2AP_DIR)/test/stub/obj/

stub_clean: clean
	cd $(X2AP_DIR)/test/stub/src && $(MAKECLEAN)
	$(DELETE) $(X2AP_DIR)/test/stub/obj/*
endif 
clean_x2ap:
	$(DELETE) $(X2AP_DIR)/build/obj/*

.PHONY:create_dir
create_dir:
	${MKD} $(X2AP_DIR)/build/obj
	

$(COMM_OBJ):$(OBJDIR)/%.o:$(COMM_SRC_DIR)%.c
	$(COMPILER)

$(UTILS_OBJ):$(OBJDIR)/%.o:$(UTILS_SRC_DIR)%.c
	$(COMPILER)

$(DB_OBJ):$(OBJDIR)/%.o:$(DB_SRC_DIR)%.c
	$(COMPILER)		

$(ENCDEC_OBJ):$(OBJDIR)/%.o:$(ENCDEC_SRC_DIR)%.c
	$(COMPILER)	

$(INIT_OBJ):$(OBJDIR)/%.o:$(INIT_SRC_DIR)%.c
	$(COMPILER)	
	
$(MSGDIST_OBJ):$(OBJDIR)/%.o:$(MSGDIST_SRC_DIR)%.c
	$(COMPILER)	

$(SCTP_OBJ):$(OBJDIR)/%.o:$(SCTP_SRC_DIR)%.c
	$(COMPILER)	

$(SU_OBJ):$(OBJDIR)/%.o:$(SU_SRC_DIR)%.c
	$(COMPILER)	

$(X2APSM_OBJ):$(OBJDIR)/%.o:$(X2APSM_SRC_DIR)%.c
	$(COMPILER)

else
	$(error X2AP_DIR variable is not exported and set)
endif