/***********************************************************************
**  FUNCTION:
**      Definitions for Relay Entity API prototypes
************************************************************************
**
**  FILE NAME:
**      egtpu_relay.h
**
**  DATE           NAME               REFERENCE        REASON
**  ----           ----               ---------        ------
**  Oct, 2009     Vipul Aggarwal                    	Original
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/
#ifndef __EGTPU_RELAY_DECL__
#define __EGTPU_RELAY_DECL__

egtpu_return_t egtpu_data_req (egtpu_global_t *,egtpu_info_t *p_egtpu_info);
egtpu_return_t egtpu_reloc_data_req(egtpu_global_t*,
										egtpu_info_t *p_egtpu_info);
egtpu_return_t egtpu_send_emi_to_peer_req (egtpu_global_t *,egtpu_info_t *p_egtpu_info);

#ifdef ENDC_ENABLED
egtpu_return_t egtpu_x2u_delivery_status(
               egtpu_global_t*  p_global,
               egtpu_info_t*    p_egtpu_info);
#endif
#endif
