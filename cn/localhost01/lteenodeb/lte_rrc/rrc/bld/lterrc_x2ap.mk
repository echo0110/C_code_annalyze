###################################################################
# 
#  FILE NAME   : lterrc_x2ap.mk  
# 
#  DESCRIPTION : The RRC X2AP module make file 
# 
#  REVISION HISTORY : 
# 
#  DATE          Name        Reference        Comments 
#  ----          ----        ---------        -------- 
#
#  Copyright (c) 2009, Aricent Inc. 
# 
###################################################################
#Set X2AP common includes files
X2AP_COMMON_INCLUDES   =   -I${RRC_ROOT}/../x2ap/common/inc \
                           -I${RRC_ROOT}/../x2ap/db/inc \
                           -I${RRC_ROOT}/../x2ap/encdec/inc \
                           -I${RRC_ROOT}/../x2ap/init/inc \
                           -I${RRC_ROOT}/../x2ap/msgDist/inc \
                           -I${RRC_ROOT}/../x2ap/sctp_intf/inc \
                           -I${RRC_ROOT}/../x2ap/x2apsm/inc \
                           -I${RRC_ROOT}/../x2ap/common/utils/inc \
                           -I${RRC_ROOT}/../../common/in_memory_logging/include

X2AP_LIB=$(LIBDIR)/libx2ap.a

#add unit data to global scope variables
RRC_COMMON_INCLUDES += $(X2AP_COMMON_INCLUDES)
RRC_LIBS += $(X2AP_LIB)
