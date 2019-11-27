#******************************************************************************
#                                    
# FILE NAME    : makefile                
#                                    
# DESCRIPTION  : Contains the linux makefile for MD5 utility. 
#                                    
# Revision History :                            
#
#  DATE            NAME                 REFERENCE       REASON        
# ------         ------------           ---------     ----------
# 22-Dec-2008    Anuradha Gupta         None          Initial
#           
# Copyright 2008, Aricent.                       
#                                    
#******************************************************************************/
include $(CRYPTO_BASE_DIR)/build/make.rule
include $(CRYPTO_BASE_DIR)/build/make.h

ARMD5_DEV_DIR= $(CRYPTO_ALGO_DIR)/md5
ARMD5_SRC_DIR= $(ARMD5_DEV_DIR)/src
ARMD5_OBJ_DIR= $(ARMD5_DEV_DIR)/obj

ARMD5_INCLUDE     +=      -I $(CRYPTO_BASE_DIR)/common \
                    -I $(ARMD5_DEV_DIR)/h

ARMD5_SRCFILES	=			$(ARMD5_SRC_DIR)/arMD5_hash.c 
ARMD5_OBJFILES	=			$(ARMD5_OBJ_DIR)/arMD5_hash.o

TARGET=$(ARMD5_OBJ_DIR)/md5.o

all:$(TARGET)

$(TARGET):$(ARMD5_OBJFILES)
		 $(LTE_LD) $(LD_FLAGS) -o $(TARGET) $(ARMD5_OBJFILES)

$(ARMD5_OBJ_DIR)/%.o:$(ARMD5_SRC_DIR)/%.c
	$(LTE_CC)  $(CFLAGS) $(ARMD5_INCLUDE) -c  $< -o $@

clean:
		 $(RM) -f $(ARMD5_OBJ_DIR)/*.o


