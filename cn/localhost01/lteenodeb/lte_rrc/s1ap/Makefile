#/******************************************************************************
# *
# *  ARICENT -
# *
# *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
# *
# ******************************************************************************
# *
# *  $Id: Makefile,v 1.4 2010/03/24 09:50:45 gur18569 Exp $
# *
# ******************************************************************************
# *
# *  File Description : Makefile for building S1AP Modules. 
# *
# ******************************************************************************
# *
# * Revision Details
# * ----------------
# * $Log: Makefile,v $
# * Revision 1.4  2010/03/24 09:50:45  gur18569
# * Merged files from Rel 1.0 support branch
# *
# * Revision 1.3  2010/01/04 16:27:20  ukr15916
# * no message
# *
# * Revision 1.1.2.5  2009/12/04 13:56:15  gur18569
# * makin bld
# *
# * Revision 1.1.2.4  2009/12/04 09:03:23  gur18569
# * changes due to shift of s1ap
# *
# * Revision 1.1.2.3  2009/11/30 08:50:36  gur18569
# * modified because shift of s1ap folder
# *
# * Revision 1.1.2.2  2009/11/26 18:33:49  ukr16018
# * Merge S1AP and RRC (from branch dev_rel_1_0).
# *
# * Revision 1.1.2.1  2009/11/25 13:10:55  gur18569
# * Shifted to level of rrc dir
# *
# * Revision 1.1.2.5  2009/11/06 11:08:13  gur21006
# * Path for s1ap_asn_enc_dec.o changed in rule for asn_gen
# *
# * Revision 1.1.2.4  2009/10/29 14:44:47  gur20052
# * Added Setting for RRC_ROOT
# *
# * Revision 1.1.2.3  2009/10/28 07:24:59  gur18569
# * added asn msg buff gen's corresponding changes
# *
# * Revision 1.1.2.2  2009/10/23 16:13:42  gur18569
# * updated for new objs
# *
# *
# *****************************************************************************/

# Specify RRC Root
include lterrc_s1ap.mk


S1AP_BIN_DIR=${RRC_ROOT}/s1ap/bld

SRCDIR= ${S1AP_ROOT}/src

ASN_FUNC_TEST_OBJS= ${S1AP_BIN_DIR}/s1ap_asn_gen.o ${S1AP_BIN_DIR}/s1ap_asn_codec.o

target: create_dirs OBJECTS 

exe: create_dirs $(ASN_FUNC_TEST_OBJS)
	gcc ./bld/s1ap_asn_gen.o ./bld/s1ap_asn_codec.o ../rrc/bld/debug/obj/list_wrap.o -L../rrc/bld/debug/lib/ -L../cspl/build/ ../rrc/bld/debug/obj/rrc_common_utils.o ../rrc/bld/debug/obj/s1ap_asn_enc_dec_3gpp.o -L../rrc/asn/lib/ -lrrccommon -lcspl -lasn1per -lasn1rt -o asn_gen

create_dirs:
	${MKDIR} ${S1AP_BIN_DIR}

OBJECTS:${OBJS} $(ASN_FUNC_TEST_OBJS)

clean:
	${RM} ${ASN_FUNC_TEST_OBJS} asn_gen

#-------------------- Some rules. -------------#
${S1AP_BIN_DIR}/%.o : ${SRCDIR}/%.c
	${COMPILE} $< -c -DTESTING_BUFFER_GENERATION  -g -o $@
