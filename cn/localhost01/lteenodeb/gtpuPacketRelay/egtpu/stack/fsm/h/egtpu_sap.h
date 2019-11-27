/***********************************************************************
**  FUNCTION:
**      Definitions for Control Entity API prototypes
************************************************************************
**
**  FILE NAME:
**      egtpu_sap.h
**
**  DATE           NAME               REFERENCE        REASON
**  ----           ----               ---------        ------
**  Oct, 2009     Vipul Aggarwal                    	Original
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/
#ifndef __EGTPU_SAP_DECL__
#define __EGTPU_SAP_DECL__

/* 0: SUCCESS */
/* on adding new error cause please take care */
#define EGTPU_RESOURCES_NOT_AVAILABLE 1
#define EGTPU_CTXT_NOT_FOUND          2
#define EGTPU_DUPLICATE_PEER_TEID     3
#define EGTPU_INV_SAP_CFG             4
#define EGTPU_IPV6_ADDR_RECEIVED      5
#define EGTPU_INV_SRC_ADDR_RECEIVED   6
/* LIPA changes start */
/*SPR 21767 Start*/
#define EGTPU_PEER_ADDRESS_MISSING   7
/*SPR 21767 End*/
/* LIPA changes end */
#if defined(ENDC_ENABLED) &&  !defined(UE_SIM_TESTING)
#define S1U_SN_TERMINTED_SPLIT_BEARER_SAP_FLAG      0x9F
#endif

egtpu_return_t egtpu_setup_sap_req (egtpu_global_t *,egtpu_info_t *);
egtpu_return_t egtpu_release_sap_req (egtpu_global_t *,egtpu_info_t *);

#endif
