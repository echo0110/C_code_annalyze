/******************************************************************************
*
*   FILE NAME:
*       csc_bch_m.h
*
*   DESCRIPTION:
*       This is the header file of the CSC BCH manager.
*       BCH manager work with MIB, SIB1 and SIs.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 June 2009    VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#ifndef _CSC_BCH_M_H_
#define _CSC_BCH_M_H_

#include "rrc_csc_llim_intf.h"
#include "rrc_rrm_intf.h"
#include "csc_cell_ctx.h"
#include "rrc_asn_enc_dec.h"

rrc_return_et bch_m_mib_sib1_sis
(
    rrc_csc_llim_config_cell_req_t  *p_llim_cfg_cell_req,
    rrc_rrm_cell_setup_req_t        *p_cfg_cell_param,
    csc_cell_ctx_t          *p_cell_ctx
);

rrc_return_et bch_m_mib_sib1_sis_recfg
(
    rrc_csc_llim_reconfig_cell_req_t  *p_llim_recfg_cell_req,   /* out message */
    rrc_rrm_cell_reconfig_req_t        *p_recfg_cell_param,       /* in message */
    csc_cell_ctx_t              *p_cell_ctx    /* cell context */   
);
rrc_return_et bch_m_update_si_transmission_info_sis_recfg
(
    rrc_csc_llim_reconfig_cell_req_t  *p_llim_recfg_cell_req,   /* out message */
    rrc_rrm_cell_setup_req_t        *p_cfg_cell_param,       /* in message */
    csc_cell_ctx_t              *p_cell_ctx,    /* cell context */  
    rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list
);
rrc_return_et bch_m_sib1_sis_pws
(
 rrc_csc_llim_pws_req_t  *p_llim_pws_req,   /* out message */
 scheduling_info_list_t  *p_scheduling_info_list, /*in scheduling info */
 csc_cell_ctx_t          *p_cell_ctx,    /* cell context */
 rrc_bool_et             is_kill_req,
 rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list
);

rrc_return_et bch_m_sib1_sis_updated_pws
(
 rrc_csc_llim_updated_pws_si_req_t  *p_llim_pws_req,   /* out message */
 csc_cell_ctx_t              *p_cell_ctx,    /* cell context */   
 rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list
 );


/* Update SI scheduling Info*/
void update_si_scheduling_info(csc_cell_ctx_t *p_cell_ctx);

void fill_si_periodicity (U16 *pws_si_periodicity,
                          U8  *si_periodicity);

rrc_return_et bch_m_check_and_convert_sib1_pws
(
    OSCTXT                      *pctxt,             /* for memory allocation */
    SystemInformationBlockType1 *p_asn1_sib1,       /* destination */
    sib_type_1_Info_t           *p_sib1,            /* source */
    /* SIBs present in message from RRM */
    rrc_bitmask_t               presence_bitmask,
    rrc_bool_et                 is_pws_req, /*Flag for PWS or updated PWS */
    csc_cell_ctx_t          *p_cell_ctx    /* cell context */
);

/******************************************************************************
*   FUNCTION NAME: rrc_asn1PrtToStr_BCCH_BCH_Message
*
*   DESCRIPTION:
*       This function prints ASN BCCH_BCH_Message using RRC logging mechanism.
*
*   RETURNS:
*       void
*
*****************************************************************************/
void rrc_asn1PrtToStr_BCCH_BCH_Message(U32 log_level, const char* name, 
                                        BCCH_BCH_Message*         pvalue,
                                        const char*               msgName,
                                        U8*                       p_buff,
                                        U16                       msgLen); 

/******************************************************************************
*   FUNCTION NAME: rrc_asn1PrtToStr_BCCH_DL_SCH_Message
*
*   DESCRIPTION:
*       This function prints ASN BCCH_DL_SCH_Message using RRC logging mechanism.
*
*   RETURNS:
*       void
*
*****************************************************************************/
void rrc_asn1PrtToStr_BCCH_DL_SCH_Message(U32 log_level, const char* name, 
                                        BCCH_DL_SCH_Message*         pvalue,
                                        const char*                  msgName,
                                        U8*                          p_buff,
                                        U16                          msgLen); 

#endif /* _CSC_BCH_M_H_ */

