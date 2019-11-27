#/**************************************************************************
#** FILE NAME  :
#**               Makefile for GW
#**# description :
#**            Makefile for GATEWAY shAREDF mODULE
#**  DATE     NAME              REFERENCE         REASON
#**  ----     ----              ---------         ------
#**  13Oct2002  Anjali     -----            Create Original
#**
#**   Copyright (C) 2006 Aricent Inc . All Rights Reserved
#******************************************************************************/
GW_DIR = $(SIGTRAN_DIR)/common/gw

all         : setup objs

all_hash    : setup objs objs_hash

setup       :
		if [ -d $(GW_DIR)/obj ] ; \
                then \
                   echo "Dir Exists" ; \
                else \
                   mkdir -p $(GW_DIR)/obj ; \
                fi
		if [ -d $(GW_DIR)/obj/$(CPU) ] ; \
                then \
                   echo "Dir Exists" ; \
                else \
                   mkdir -p $(GW_DIR)/obj/$(CPU) ; \
                fi
objs         :
		@cd $(GW_DIR)/genlib/make/solaris; \
 		CFLAGS_CMN=-DGL_TOTAL_MT_SAFE;\
		export CFLAGS_CMN;\
		make -f makegl_os gl_tm_mt GW_CSPL_DIR=$(CSPL_DIR); \
		make -f makegl_os gl_gscim_mt GW_CSPL_DIR=$(CSPL_DIR); \
                make -f makegl_os gl_mm_mt GW_CSPL_DIR=$(CSPL_DIR); \
		make -f makegl_os gl_tmport GW_CSPL_DIR=$(CSPL_DIR); \
	        make -f makegl_cmn gl_ulm GW_CSPL_DIR=$(CSPL_DIR); \
	        cd $(GW_DIR)/obj/$(CPU);\
	        ar -x $(GW_DIR)/lib/solaris/libgl_tm_mtl.a;\
		ar -x $(GW_DIR)/lib/solaris/libgl_mm_mtl.a; \
		ar -x $(GW_DIR)/lib/solaris/libgl_gscim_mtl.a ;\
		ar -x $(GW_DIR)/lib/solaris/libgl_tmportl.a ;\
		ar -x $(GW_DIR)/lib/solaris/libgl_ulml.a  ;\
		ar -r $(GW_DIR)/lib/solaris/libgl.a $(GW_DIR)/obj/$(CPU)/*.o;

objs_hash : 
		@cd $(GW_DIR)/genlib/make/solaris; \
 		CFLAGS_CMN=-DGL_TOTAL_MT_SAFE;\
		export CFLAGS_CMN;\
		make -f makegl_db gl_htm_mt GW_CSPL_DIR=$(CSPL_DIR); \
	        cd $(GW_DIR)/obj/$(CPU);\
		ar -x $(GW_DIR)/lib/solaris/libgl_htm_mtl.a  ;\
		ar -r $(GW_DIR)/lib/solaris/libgl.a $(GW_DIR)/obj/$(CPU)/*.o;
		

clean          :
		cd $(GW_DIR)/lib/solaris; \
		rm -f libgl*.a; \
		cd $(GW_DIR)/obj/$(CPU); \
                rm -f *.o; \
		cd $(GW_DIR)/genlib/obj/ ; \
		rm -f *.o;

