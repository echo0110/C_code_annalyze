###################################################################
# 
#  FILE NAME   : lterrc_asn.mk  
# 
#  DESCRIPTION : The RRC ASN module make file 
# 
#  REVISION HISTORY : 
# 
#  DATE          Name        Reference        Comments 
#  ----          ----        ---------        -------- 
#
#  Copyright (c) 2009, Aricent Inc. 
# 
###################################################################


ASN_INCLUDES:=	-I${RRC_ROOT}/asn/src \
				-I$(RRC_ROOT)/asn/include -I$(RRC_ROOT)/asn/include/utran_asn_enc_dec

ASN_SRCDIR:= ${RRC_ROOT}/asn/src ${RRC_ROOT}/asn/src/utran_asn_enc_dec

ASN_SRCFILES:=	rrc_asn_enc_dec.c \
				InformationElements.c \
				InformationElements_1.c \
				InformationElements_2.c \
				InformationElementsCopy.c \
				InformationElementsCopy_1.c \
				InformationElementsCopy_2.c \
				InformationElementsDec.c \
				InformationElementsDec_1.c \
				InformationElementsDec_2.c \
				InformationElementsDec_3.c \
				InformationElementsDec_4.c \
				InformationElementsEnc.c \
				InformationElementsEnc_1.c \
				InformationElementsEnc_2.c \
				InformationElementsEnc_3.c \
				InformationElementsEnc_4.c \
				InformationElementsPrint.c \
				InformationElementsPrint_1.c \
				InformationElementsPrint_2.c \
				Internode-definitions.c \
				Internode-definitionsCopy.c \
				Internode-definitionsDec.c \
				Internode-definitionsEnc.c \
				Internode-definitionsPrint.c \
				PDU-definitions.c \
				PDU-definitionsCopy.c \
				PDU-definitionsDec.c \
				PDU-definitionsDec_1.c \
				PDU-definitionsDec_2.c \
				PDU-definitionsEnc.c \
				PDU-definitionsEnc_1.c \
				PDU-definitionsEnc_2.c \
				PDU-definitionsPrint.c \
				PDU-definitionsPrint_1.c \
				s1ap_asn_enc_dec_3gpp.c \
				s1ap_3gpp.c\
                ranap_asn_enc_dec.c \
                x2ap_asn_enc_dec_3gpp.c \
                x2ap_3gpp.c \
                lppa_asn_enc_dec_3gpp.c \
                lppa_3gpp.c 

ifeq '$(EMBMS_FEAT)' 'enable'
ASN_SRCFILES+=m2ap_asn_enc_dec_3gpp.c
endif

#MENB_CHANGES_START				
ifeq '$(ENDC_FEAT)' 'enable'
ASN_SRCFILES+=rrc_asn_enc_dec_nr.c 
endif
#MENB_CHANGES_END	

ASN_OBJFILES:=$(ASN_SRCFILES)
ASN_OBJFILES:=$(patsubst %.c,$(OBJDIR)/%.o,$(ASN_OBJFILES))

ASN_DEPFILES:=$(ASN_SRCFILES)
ASN_DEPFILES:=$(patsubst %.c,$(OBJDIR)/%.d,$(ASN_DEPFILES))

ASN_LIB:=$(LIBDIR)/librrcasn.a

asn: $(MODE) $(ASN_LIB) 

$(ASN_OBJFILES) : lterrc_asn.mk

ifeq ($(wildcard $(LIBDIR)/librrcasn.a),)
FLAG = 1
else
FLAG = 0
endif


$(ASN_LIB) : $(ASN_OBJFILES) 
	@echo '[$(ASN_LIB)]'
	$(RRC_AR) $(RRC_ARFLAGS) $(ASN_LIB) $(ASN_OBJFILES)
	$(RRC_RANLIB) $(ASN_LIB)
	@echo

ifneq ($(strip $(ASN_DEPFILES)),)
-include $(ASN_DEPFILES)
endif

.PHONY : asn 

#add unit data to global scope variables
VPATH += $(ASN_SRCDIR)

ifeq ($(FLAG),1)
RRC_OBJFILES += $(ASN_OBJFILES)
endif

RRC_INCLUDES += $(ASN_INCLUDES)
RRC_LIBS += $(ASN_LIB)
