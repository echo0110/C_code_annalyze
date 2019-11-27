/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_cprm_main.c,v 1.6.28.14 2010/12/02 08:57:11 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the entry points and initialization
 *                     functions of the CPRM module. The entry point function
 *                     can be called to either determine the packet type or to
 *                     process the received feedback packets.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_cprm_main.c,v $
 * Revision 1.6.28.14  2010/12/02 08:57:11  gur22059
 * Conflicting profile 1 functions were renamed acc to new Naming Convention
 *
 * Revision 1.6.28.13  2010/11/24 03:44:04  gur22059
 * Removal of conditional checks w.r.t profile1
 *
 * Revision 1.6.28.12  2010/11/09 05:24:08  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.6.28.11  2010/10/18 06:13:36  gur22059
 * Incorporated comments for RND
 *
 * Revision 1.6.28.10  2010/10/13 03:57:07  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.6.28.9  2010/09/24 09:20:51  gur22059
 * Removed the NR flag
 *
 * Revision 1.6.28.8  2010/09/23 05:19:35  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.6.28.7  2010/09/10 10:33:15  gur22059
 * Locality of reference is used for better performamce
 *
 * Revision 1.6.28.6  2010/07/20 11:50:41  gur22059
 * SPR 562 is fixed.
 *
 * Revision 1.6.28.5  2010/07/08 09:25:28  gur21145
 * code review comments for prof0,2 enhancement activity
 *
 * Revision 1.6.28.2  2010/06/25 09:16:38  gur21145
 * PROFILE2_ENHANCEMENT
 *
 * Revision 1.6  2009/05/28 13:51:09  gur19140
 * *** empty log message ***
 *
 * Revision 1.5  2009/05/28 04:26:50  gur19836
 * File Header updated
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "rohc_cprm_proto.h"
#include "rohc_ed_ext_if.h"
#include "rohc_cprm_ext_if.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/* +- SPR 17777 */
extern tickType_t pdcpCurrentTime_g;
/* 
** START_SPR_1197_FIX : externed packet type info
** array structure.
*/
extern rohc_p1_pkt_typ_info_t
rohc_p1_pkt_typ_info_r[ROHC_P1_R_MAX_PACKETS];
/* END_SPR_1197_FIX */

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

            /* + SPR 17439 */
rohc_return_t (*rohc_cprm_resolve_pkt_typ_fpt [ROHC_ALL_PROFILE - ROHC_UNCOMP]
                                              [ROHC_MODE_R - ROHC_MODE_U + 1]
                                              [ROHC_C_ST_SO - ROHC_C_ST_IR + 1 ])
                                              (
                                               rohc_entity_t     *p_entity,
                                               rohc_csc_rtp_t       *p_csc,
                                               rohc_U8bit_t      *sel_pkt,
                                               rohc_error_code_t *p_ecode
                                               ) = {
  { {rohc_cprm_sel_pkt_p1_uo_ir, rohc_cprm_sel_pkt_p1_uo_fo, 
      rohc_cprm_sel_pkt_p1_uo_so},
  {rohc_cprm_sel_pkt_p1_uo_ir, rohc_cprm_sel_pkt_p1_uo_fo,
    rohc_cprm_sel_pkt_p1_uo_so},
  {rohc_cprm_sel_pkt_p1_r_ir, rohc_cprm_sel_pkt_p1_r_fo,
    rohc_cprm_sel_pkt_p1_r_so}
  },

  { {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}},

  { {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}},

  { {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}},

{ {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}},

  { {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}},

{ {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}}

};
rohc_return_t (*rohc_cprm_resolve_pkt_typ_fpt_udp [ROHC_ALL_PROFILE - ROHC_UNCOMP]
                                              [ROHC_MODE_R - ROHC_MODE_U + 1]
                                              [ROHC_C_ST_SO - ROHC_C_ST_IR + 1 ])
                                              (
                                               rohc_entity_t     *p_entity,
                                               rohc_csc_udp_t       *p_csc,
                                               rohc_U8bit_t      *sel_pkt,
                                               rohc_error_code_t *p_ecode
                                               ) = {

  /* SPR 18804 Fix Start */
  { {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}},
  /* SPR 18804 Fix End */
  { {rohc_cprm_sel_pkt_p2_uo_ir, rohc_cprm_sel_pkt_p2_uo_fo ,
      rohc_cprm_sel_pkt_p2_uo_so}, 
  {rohc_cprm_sel_pkt_p2_uo_ir, rohc_cprm_sel_pkt_p2_uo_fo, 
    rohc_cprm_sel_pkt_p2_uo_so},
  {rohc_cprm_sel_pkt_p2_r_ir, rohc_cprm_sel_pkt_p2_r_fo,
    rohc_cprm_sel_pkt_p2_r_so}
  },

  { {rohc_cprm_sel_pkt_p2_uo_ir, rohc_cprm_sel_pkt_p2_uo_fo ,
      rohc_cprm_sel_pkt_p2_uo_so}, 
  {rohc_cprm_sel_pkt_p2_uo_ir, rohc_cprm_sel_pkt_p2_uo_fo, 
    rohc_cprm_sel_pkt_p2_uo_so},
  {rohc_cprm_sel_pkt_p2_r_ir, rohc_cprm_sel_pkt_p2_r_fo,
    rohc_cprm_sel_pkt_p2_r_so}
  },

  { {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}},

  { {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}},

  { {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}},

  { {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}}

};
rohc_return_t (*rohc_cprm_resolve_pkt_typ_fpt_tcp [ROHC_ALL_PROFILE - ROHC_UNCOMP]
                                              [ROHC_MODE_R - ROHC_MODE_U + 1]
                                              [ROHC_C_ST_SO - ROHC_C_ST_IR + 1 ])
                                              (
                                               rohc_entity_t     *p_entity,
                                               rohc_csc_tcp_t       *p_csc,
                                               rohc_U8bit_t      *sel_pkt,
                                               rohc_error_code_t *p_ecode
                                               ) = {
{ {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}},


{ {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}},

  { {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}},

  { {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}},

  { {NULL, NULL, NULL}, {NULL, NULL, NULL}, {NULL, NULL, NULL}},

  { {rohc_cprm_sel_pkt_p6_ir, rohc_cprm_sel_p6_co_pkt,
      rohc_cprm_sel_p6_co_pkt},
  {NULL, NULL, NULL},
  {NULL, NULL, NULL}
  }
};
 /* - SPR 17439 */
/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_init
 ******************************************************************************
 **
 **      DESCRIPTION:
 **          This function invokes the funcitons to initialize the static tables.
 **
 **      ARGUMENTS:
 **
 **      RETURN VALUE:
 **              rohc_void_t
 **      NOTES:
 **
 *****************************************************************************/
rohc_void_t rohc_cprm_init ROHC_NO_PARAMS()
{
  ROHC_ENTER_FN("rohc_cprm_init");
  rohc_init_p1_uo_table();
  rohc_init_p1_r_table();
  rohc_init_p23_uo_table();
  rohc_init_p23_r_table();
  ROHC_EXIT_FN("rohc_cprm_init");
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_p0_resolve_pkt_typ
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This is the main event processing entry point for C_RPM
 **              module. It determines the type of packet to be set to peer
 **              side compressor.  The possible packet type retunred by this
 **              are defined in the rohc_com_def.h
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **              p_pkt_typ [OUT]   Selected paacket type
 **      RETURN VALUE:
 **              rohc_U8bit_t :ROHC packet type
 **      NOTES:
 **
 *****************************************************************************/
rohc_return_t rohc_cprm_p0_resolve_pkt_typ ROHC_PARAMS(
    (p_entity, p_csc, p_pkt_typ, p_ecode),
    rohc_entity_t *p_entity _AND_
    rohc_csc_uncomp_t *p_csc _AND_
    rohc_U8bit_t *p_pkt_typ _AND_
    rohc_error_code_t *p_ecode)
{
  ROHC_ENTER_FN("rohc_cprm_p0_resolve_pkt_typ");

  *p_pkt_typ = rohc_cprm_resolve_pkt_typ_uncomp(p_entity,p_csc);
  /* +- SPR 17777 */
/* SPR 20636 Changes Start */
  LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
          __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
          ROHC_GET_CSC_CID(p_csc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
  ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"
        "Determining the pkt type for profile ROHC_UNCOMP",
         p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
         0);

  ROHC_EXIT_FN("rohc_cprm_p0_resolve_pkt_typ");
  return ROHC_SUCCESS;
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_p1_resolve_pkt_typ
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This is the main event processing entry point for C_RPM
 **              module. It determines the type of packet to be set to peer
 **              side compressor.  The possible packet type retunred by this
 **              are defined in the rohc_com_def.h
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **              p_pkt_typ [OUT]   Selected paacket type
 **      RETURN VALUE:
 **              rohc_U8bit_t :ROHC packet type
 **      NOTES:
 **
 *****************************************************************************/
rohc_return_t rohc_cprm_p1_resolve_pkt_typ ROHC_PARAMS(
    (p_entity, p_csc, p_pkt_typ, p_ecode),
    rohc_entity_t     *p_entity  _AND_
    rohc_csc_rtp_t    *p_csc     _AND_
    rohc_U8bit_t      *p_pkt_typ _AND_
    rohc_error_code_t *p_ecode)
{
  rohc_U8bit_t sel_pkt = ROHC_UNKNOWN_PKT;
  rohc_encoded_value_t en_sn;
  rohc_encoded_value_t en_ts;
  /* Coverity Fix 32414 Start*/
  rohc_encoded_value_t en_ip_id;
  /* Coverity Fix 32414 Start*/
  ROHC_ENTER_FN("rohc_cprm_p1_resolve_pkt_typ");

  if(((rohc_csc_common_t *)p_csc)->profile == ROHC_UNCOMP)
  {
      *p_pkt_typ = rohc_cprm_resolve_pkt_typ_uncomp(p_entity,(rohc_csc_uncomp_t*)p_csc);
      ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"
              "Determining the pkt type for profile ROHC_UNCOMP",
              p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
              0);
      return ROHC_SUCCESS;
  }
  en_sn.value = en_sn.bits = en_ts.value = en_ts.bits = 0;
  /* Coverity Fix 32414 Start*/
  en_ip_id.value = en_ip_id.bits = 0;
  /* Coverity Fix 32414 Start*/
  p_csc->field_value[ROHC_EXT_FID -ROHC_VAR_FID_BASE]=0;
  rohc_ed_p1_update_window(p_entity, p_csc);
  if( ROHC_CSC_GET_STATE(p_csc) >= ROHC_C_ST_IR      && 
      ROHC_CSC_GET_STATE(p_csc) <= ROHC_C_ST_SO      &&
      ROHC_CSC_GET_PROFILE(p_csc) >= ROHC_IP_UDP_RTP &&
      ROHC_CSC_GET_PROFILE(p_csc) <= ROHC_IP_ESP     && 
      ROHC_CSC_GET_MODE(p_csc)  >=ROHC_MODE_U        &&
      ROHC_CSC_GET_MODE(p_csc)  <= ROHC_MODE_R)
  {
      if(ROHC_FAILURE == (*rohc_cprm_resolve_pkt_typ_fpt 
                             [ROHC_CSC_GET_PROFILE(p_csc) - ROHC_IP_UDP_RTP]
                             [ROHC_CSC_GET_MODE(p_csc) - ROHC_MODE_U]
                             [ROHC_CSC_GET_STATE(p_csc) - ROHC_C_ST_IR])
                             (p_entity, p_csc,&sel_pkt, p_ecode))
      return ROHC_FAILURE;
  }
  else
  {
      /* 
      ** START_SPR_562_FIX : The mode & state variable name  is corrected
      */
      /* SPR 9591 fix start */
      ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"
              "Unsupportet profile [%u] OR mode [%u] OR state [%u]" ,
      /* SPR 9591 fix end */
              p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
              ROHC_GET_CSC_CID(p_csc),
              p_csc->profile , p_csc->c_mode , p_csc->c_state);

      /* END_SPR_562_FIX */
   }

  if(sel_pkt == ROHC_UNKNOWN_PKT)
  {
    /* SPR 9591 fix start */
    ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:Profile[%u]:"
    /* SPR 9591 fix end */
        "CPRM module failed to determine the pkt typ",
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
        ROHC_GET_CSC_CID(p_csc),
        p_csc->profile);
    *p_ecode = ROHC_PKT_DETERMINATION_FAILURE;
    ROHC_EXIT_FN("rohc_cprm_p1_resolve_pkt_typ");
    return ROHC_FAILURE;
  }

  if((p_csc->send_irdyn_flag == ROHC_TRUE) && (p_csc->c_state != ROHC_C_ST_IR))
  {
    sel_pkt = ROHC_IRDYN_PKTTYPE; 
    p_csc->send_irdyn_flag = ROHC_FALSE; 
  }

  *p_pkt_typ = sel_pkt;
  rohc_ed_p1_update_gsw_pkt_typ(p_entity, p_csc, *p_pkt_typ);
  if((sel_pkt == ROHC_R0P1_PKTTYPE) ||
      (sel_pkt >= ROHC_R1P1_PKTTYPE && sel_pkt <=ROHC_R1TSE3P1_PKTTYPE) ||
      (sel_pkt == ROHC_UO1IDE3P1_PKTTYPE ) ||
      (sel_pkt >= ROHC_R1P23_PKTTYPE && sel_pkt <=ROHC_R1E3P23_PKTTYPE) )
  {   
    p_csc->dont_update_csc_flag = ROHC_TRUE;
  }
  else
  {
    p_csc->dont_update_csc_flag = ROHC_FALSE;
  }   

  if((sel_pkt != ROHC_R0P1_PKTTYPE) &&
      (sel_pkt != ROHC_R0CRCP1_PKTTYPE) &&
      (sel_pkt != ROHC_UO0P1_PKTTYPE          ) &&
      (sel_pkt != ROHC_R1P1_PKTTYPE           ) &&
      (sel_pkt != ROHC_R1TSP1_PKTTYPE         ) &&
      (sel_pkt != ROHC_R1IDP1_PKTTYPE         ) &&
      (sel_pkt != ROHC_UO1P1_PKTTYPE          ) &&
      (sel_pkt != ROHC_UO1IDP1_PKTTYPE        ) &&
      (sel_pkt != ROHC_UO1TSP1_PKTTYPE        ) &&
      (sel_pkt != ROHC_UOR2P1_PKTTYPE         ) &&
      (sel_pkt != ROHC_UOR2IDP1_PKTTYPE       ) &&
      (sel_pkt != ROHC_UOR2TSP1_PKTTYPE       ) &&
      (sel_pkt != ROHC_R1P23_PKTTYPE          ) &&
      (sel_pkt != ROHC_UO1P23_PKTTYPE         ) &&
      (sel_pkt != ROHC_UOR2P23_PKTTYPE        ) &&
      (sel_pkt != ROHC_IRP0_PKTYPE            ) &&
      (sel_pkt != ROHC_NORMALP0_PKTYPE        ) &&
      (sel_pkt != ROHC_IR_WITHOUT_DYN_PKTTYPE ) &&
      (sel_pkt != ROHC_IR_WITH_DYN_PKTTYPE    ) &&
      (sel_pkt != ROHC_IRDYN_PKTTYPE          ) )
  {
    p_csc->field_value[ROHC_EXT_FID -ROHC_VAR_FID_BASE]=1;
  }

  else
  {
    p_csc->field_value[ROHC_EXT_FID -ROHC_VAR_FID_BASE]=0;
  }
  if(sel_pkt == ROHC_IRDYN_PKTTYPE)
  {
    /* update the all static +dynamic information */

    en_ip_id.value  = 0;
    if(p_csc->p_cpib->inner_ip4 != ROHC_NULL)
    {
      en_ip_id.value  = p_csc->p_cpib->temp_ipid;
      en_ip_id.bits  = ROHC_P123_IPID_BITS;
    }
    else if(p_csc->p_cpib->outer_ip4 != ROHC_NULL)
    {   
      en_ip_id.value  = p_csc->p_cpib->temp_ipid2;
      en_ip_id.bits  = ROHC_P123_IPID_BITS;
    }   

    en_sn.value  =p_csc->p_cpib->temp_sn;
    en_sn.bits  = ROHC_P1_SN_BITS;
    en_ts.value  =p_csc->p_cpib->temp_ts;
    en_ts.bits  = ROHC_P1_TS_BITS;
    rohc_update_p1_ctx(p_entity, p_csc, &en_sn,&en_ts, &en_ip_id);
  }

  ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"
      "Determining packet type[%u]\n",
      p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
      ROHC_GET_CSC_CID(p_csc),
      sel_pkt);

  ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"
      "SN value in the CSC is [%u]\n",
      p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
      ROHC_GET_CSC_CID(p_csc),
      p_csc->field_value[ROHC_SN_FID -ROHC_VAR_FID_BASE]);

  ROHC_EXIT_FN("rohc_cprm_p1_resolve_pkt_typ");
  return ROHC_SUCCESS;
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_p2_resolve_pkt_typ
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This is the main event processing entry point for C_RPM
 **              module. It determines the type of packet to be set to peer
 **              side compressor.  The possible packet type retunred by this
 **              are defined in the rohc_com_def.h
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **              p_pkt_typ [OUT]   Selected paacket type
 **      RETURN VALUE:
 **              rohc_U8bit_t :ROHC packet type
 **      NOTES:
 **
 *****************************************************************************/
rohc_return_t rohc_cprm_p2_resolve_pkt_typ ROHC_PARAMS(
    (p_entity, p_csc, p_pkt_typ, p_ecode),
    rohc_entity_t     *p_entity  _AND_
    rohc_csc_udp_t    *p_csc     _AND_
    rohc_U8bit_t      *p_pkt_typ _AND_
    rohc_error_code_t *p_ecode)
{
  rohc_U8bit_t sel_pkt = ROHC_UNKNOWN_PKT;
  rohc_encoded_value_t en_sn;
  /*Coverity Fix 32415 Start*/
  /*Coverity Fix 32415 End*/
  rohc_encoded_value_t en_ip_id,en_ip_id2;
  ROHC_ENTER_FN("rohc_cprm_p2_resolve_pkt_typ");

  if(((rohc_csc_common_t *)p_csc)->profile == ROHC_UNCOMP)
  {
      *p_pkt_typ = rohc_cprm_resolve_pkt_typ_uncomp(p_entity, (rohc_csc_uncomp_t*)p_csc);
      ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"
              "Determining the pkt type for profile ROHC_UNCOMP",
              p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
              0);
      return ROHC_SUCCESS;
  }
  /*Coverity Fix 32415 Start*/
  en_sn.value = en_sn.bits = 0;
  /*Coverity Fix 32415 End*/
  en_ip_id.value = en_ip_id.bits = en_ip_id2.value = en_ip_id2.bits = 0;
  p_csc->field_value[ROHC_EXT_FID -ROHC_VAR_FID_BASE]=0;
  rohc_ed_p2_update_window(p_entity, p_csc);

  if( ROHC_CSC_GET_STATE(p_csc) >= ROHC_C_ST_IR      && 
      ROHC_CSC_GET_STATE(p_csc) <= ROHC_C_ST_SO      &&
      ROHC_CSC_GET_PROFILE(p_csc) >= ROHC_IP_UDP_RTP &&
      ROHC_CSC_GET_PROFILE(p_csc) <= ROHC_IP_ESP     && 
      ROHC_CSC_GET_MODE(p_csc)  >=ROHC_MODE_U        &&
      ROHC_CSC_GET_MODE(p_csc)  <= ROHC_MODE_R)
  {
      if(ROHC_FAILURE == (*rohc_cprm_resolve_pkt_typ_fpt_udp 
                             [ROHC_CSC_GET_PROFILE(p_csc) - ROHC_IP_UDP_RTP]
                             [ROHC_CSC_GET_MODE(p_csc) - ROHC_MODE_U]
                             [ROHC_CSC_GET_STATE(p_csc) - ROHC_C_ST_IR])
                             (p_entity, p_csc,&sel_pkt, p_ecode))
      return ROHC_FAILURE;
  }
  else
  {
      /* 
      ** START_SPR_562_FIX : The mode & state variable name  is corrected
      */
      /* SPR 9591 fix start */
      ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"
              "Unsupported profile [%u] OR mode [%u] OR state [%u]" ,
      /* SPR 9591 fix end */
              p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
              ROHC_GET_CSC_CID(p_csc),
              p_csc->profile , p_csc->c_mode , p_csc->c_state);

      /* END_SPR_562_FIX */
   }

  if(sel_pkt == ROHC_UNKNOWN_PKT)
  {
    /* SPR 9591 fix start */
    ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:profile[%u]:"
    /* SPR 9591 fix end */
        "CPRM module failed to determine the pkt typ",
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
        ROHC_GET_CSC_CID(p_csc),
        p_csc->profile);
    *p_ecode = ROHC_PKT_DETERMINATION_FAILURE;
    ROHC_EXIT_FN("rohc_cprm_p2_resolve_pkt_typ");
    return ROHC_FAILURE;
  }

  if(p_csc->send_irdyn_flag == ROHC_TRUE && p_csc->c_state != ROHC_C_ST_IR)
  {
    sel_pkt = ROHC_IRDYN_PKTTYPE; 
    p_csc->send_irdyn_flag = ROHC_FALSE; 
  }

  *p_pkt_typ = sel_pkt;
  rohc_ed_p2_update_gsw_pkt_typ(p_entity, p_csc, *p_pkt_typ);
  if((sel_pkt == ROHC_R0P1_PKTTYPE) ||
      (sel_pkt >= ROHC_R1P1_PKTTYPE && sel_pkt <=ROHC_R1TSE3P1_PKTTYPE) ||
      (sel_pkt == ROHC_UO1IDE3P1_PKTTYPE ) ||
      (sel_pkt >= ROHC_R1P23_PKTTYPE && sel_pkt <=ROHC_R1E3P23_PKTTYPE) )
  {   
    p_csc->dont_update_csc_flag = ROHC_TRUE;
  }
  else
  {
    p_csc->dont_update_csc_flag = ROHC_FALSE;
  }   

  if((sel_pkt != ROHC_R0P1_PKTTYPE) &&
      (sel_pkt != ROHC_R0CRCP1_PKTTYPE) &&
      (sel_pkt != ROHC_UO0P1_PKTTYPE          ) &&
      (sel_pkt != ROHC_R1P1_PKTTYPE           ) &&
      (sel_pkt != ROHC_R1TSP1_PKTTYPE         ) &&
      (sel_pkt != ROHC_R1IDP1_PKTTYPE         ) &&
      (sel_pkt != ROHC_UO1P1_PKTTYPE          ) &&
      (sel_pkt != ROHC_UO1IDP1_PKTTYPE        ) &&
      (sel_pkt != ROHC_UO1TSP1_PKTTYPE        ) &&
      (sel_pkt != ROHC_UOR2P1_PKTTYPE         ) &&
      (sel_pkt != ROHC_UOR2IDP1_PKTTYPE       ) &&
      (sel_pkt != ROHC_UOR2TSP1_PKTTYPE       ) &&
      (sel_pkt != ROHC_R1P23_PKTTYPE          ) &&
      (sel_pkt != ROHC_UO1P23_PKTTYPE         ) &&
      (sel_pkt != ROHC_UOR2P23_PKTTYPE        ) &&
      (sel_pkt != ROHC_IRP0_PKTYPE            ) &&
      (sel_pkt != ROHC_NORMALP0_PKTYPE        ) &&
      (sel_pkt != ROHC_IR_WITHOUT_DYN_PKTTYPE ) &&
      (sel_pkt != ROHC_IR_WITH_DYN_PKTTYPE    ) &&
      (sel_pkt != ROHC_IRDYN_PKTTYPE          ) )
  {
    p_csc->field_value[ROHC_EXT_FID -ROHC_VAR_FID_BASE]=1;
  }

  else
  {
    p_csc->field_value[ROHC_EXT_FID -ROHC_VAR_FID_BASE]=0;
  }
  if(sel_pkt == ROHC_IRDYN_PKTTYPE)
  {
    /* update the all static +dynamic information */

    en_ip_id.value  = 0;
    if(p_csc->p_cpib->inner_ip4 != ROHC_NULL)
    {
      en_ip_id.value  = p_csc->p_cpib->temp_ipid;
      en_ip_id.bits  = ROHC_P123_IPID_BITS;
    }
    else if(p_csc->p_cpib->outer_ip4 != ROHC_NULL)
    {   
      en_ip_id.value  = p_csc->p_cpib->temp_ipid2;
      en_ip_id.bits  = ROHC_P123_IPID_BITS;
    }   

    en_sn.value  =p_csc->p_cpib->temp_sn;
    en_sn.bits  = ROHC_P1_SN_BITS;
      rohc_update_ctx_p2 (p_entity, p_csc, &en_sn,
          &en_ip_id, &en_ip_id2);
  }

  ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"
      "Determining packet type[%u]\n",
      p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
      ROHC_GET_CSC_CID(p_csc),
      sel_pkt);

  ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"
      "SN value in the CSC is [%u]\n",
      p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
      ROHC_GET_CSC_CID(p_csc),
      p_csc->field_value[ROHC_SN_FID -ROHC_VAR_FID_BASE]);

  ROHC_EXIT_FN("rohc_cprm_p2_resolve_pkt_typ");
  return ROHC_SUCCESS;
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_p6_resolve_pkt_typ
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This is the main event processing entry point for C_RPM
 **              module. It determines the type of packet to be set to peer
 **              side compressor.  The possible packet type retunred by this
 **              are defined in the rohc_com_def.h
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **              p_pkt_typ [OUT]   Selected paacket type
 **      RETURN VALUE:
 **              rohc_U8bit_t :ROHC packet type
 **      NOTES:
 **
 *****************************************************************************/
rohc_return_t rohc_cprm_p6_resolve_pkt_typ ROHC_PARAMS(
    (p_entity, p_csc, p_pkt_typ, p_ecode),
    rohc_entity_t     *p_entity  _AND_
    rohc_csc_tcp_t    *p_csc     _AND_
    rohc_U8bit_t      *p_pkt_typ _AND_
    rohc_error_code_t *p_ecode)
{
  rohc_U8bit_t sel_pkt = ROHC_UNKNOWN_PKT;
  rohc_encoded_value_t en_sn;
  rohc_encoded_value_t en_msn;
  rohc_encoded_value_t en_ack;
  rohc_encoded_value_t en_ip_id;
  rohc_U8bit_t crc_res = ROHC_FAILURE;
  ROHC_ENTER_FN("rohc_cprm_p6_resolve_pkt_typ");
  en_sn.value = en_sn.bits =0;
  en_ack.value = en_ack.bits = 0;
  en_ip_id.value = en_ip_id.bits = 0;
 
  rohc_ed_p6_update_window(p_entity, p_csc);
  /* update msn in window */
  en_msn.value = p_csc->tcp_msn;
  en_msn.bits  = ROHC_P6_MSN_BITS;
  rohc_ed_p6_update_gsw_msn(p_entity, p_csc, en_msn.value);

  if( ROHC_CSC_GET_STATE(p_csc) >= ROHC_C_ST_IR      && 
      ROHC_CSC_GET_STATE(p_csc) <= ROHC_C_ST_SO      &&
      ROHC_CSC_GET_PROFILE(p_csc) >= ROHC_IP_UDP_RTP &&
            ROHC_CSC_GET_PROFILE(p_csc) <= ROHC_IP_TCP)
  {
            /* + SPR 17439 */
      if(ROHC_FAILURE == (*rohc_cprm_resolve_pkt_typ_fpt_tcp 
                             [ROHC_CSC_GET_PROFILE(p_csc) - ROHC_IP_UDP_RTP]
                    [0][ROHC_CSC_GET_STATE(p_csc) - ROHC_C_ST_IR])
                             (p_entity, p_csc,&sel_pkt, p_ecode))
            /* - SPR 17439 */
      {    
      return ROHC_FAILURE;
      }
  }
  else
  {
      /* SPR 9591 fix start */ 
      ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"
              "Unsupportet profile [%u] OR mode [%u] OR state [%u] " ,
      /* SPR 9591 fix end */ 
              p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
              ROHC_GET_CSC_CID(p_csc),
              p_csc->profile , p_csc->c_mode , p_csc->c_state);

      return ROHC_FAILURE;
   }

  if(sel_pkt == ROHC_UNKNOWN_PKT)
  {
    /* SPR 9591 fix start */ 
    ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:profile[%u]:"
    /* SPR 9591 fix end */ 
        "CPRM module failed to determine the pkt typ",
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
        ROHC_GET_CSC_CID(p_csc),
        p_csc->profile);
    *p_ecode = ROHC_PKT_DETERMINATION_FAILURE;
    ROHC_EXIT_FN("rohc_cprm_p6_resolve_pkt_typ");
    return ROHC_FAILURE;
  }

  if((p_csc->send_irdyn_flag == ROHC_TRUE) && (p_csc->c_state != ROHC_C_ST_IR))
  {
        sel_pkt = ROHC_IRDYN_PKTTYPE; 
    p_csc->send_irdyn_flag = ROHC_FALSE; 
  }

  *p_pkt_typ = sel_pkt;
    if(sel_pkt == ROHC_IRDYN_PKTTYPE)
  {
    /* update the all static +dynamic information */

    en_ip_id.value  = 0;
    if(p_csc->p_cpib->inner_ip4 != ROHC_NULL)
    {
      en_ip_id.value  = p_csc->p_cpib->temp_ipid;
      en_ip_id.bits  = ROHC_P6_IPID_BITS;
    }
    else if(p_csc->p_cpib->outer_ip4 != ROHC_NULL)
    {   
      en_ip_id.value  = p_csc->p_cpib->temp_ipid2;
            en_ip_id.bits  = ROHC_P6_IPID_BITS;
    }   

    en_sn.value  =p_csc->p_cpib->temp_sn;
      en_sn.bits  = ROHC_P6_SN_BITS;
      en_ack.value = p_csc->p_cpib->temp_ack;
      en_ack.bits  = ROHC_P6_SN_BITS;
      en_msn.value = p_csc->tcp_msn;
      en_msn.bits  = ROHC_P6_MSN_BITS;
      rohc_update_ctx_p6 (p_entity, p_csc, &en_sn, &en_ack, &en_msn, &en_ip_id);
  }

  ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"
      "Determining packet type[%u]\n",
      p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
      ROHC_GET_CSC_CID(p_csc),
      sel_pkt);

  ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"
      "SN value in the CSC is [%u]\n",
      p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
      ROHC_GET_CSC_CID(p_csc),
      p_csc->field_value[ROHC_SN_FID -ROHC_VAR_FID_BASE]);
  /* crc computation for CO pkts*/
  if((ROHC_CO_P6_COMMON_PKTYPE == sel_pkt) || (ROHC_CO_P6_RND8_PKTYPE == 
              sel_pkt))
  {
      /* +- SPR 17777 */
      crc_res = rohc_c_crc_p6_compute(p_csc, ROHC_CRC_TYPE_7);
      /* +- SPR 17777 */
      if(!crc_res)
      {
           *p_pkt_typ = ROHC_IRDYN_PKTTYPE;
      }
  }
  if(sel_pkt >=ROHC_CO_P6_RND1_PKTYPE && sel_pkt <= ROHC_CO_P6_RND7_PKTYPE)
  {
      /* +- SPR 17777 */
      crc_res = rohc_c_crc_p6_compute(p_csc, ROHC_CRC_TYPE_3);
      /* +- SPR 17777 */
      if(!crc_res)
      {
           *p_pkt_typ = ROHC_IRDYN_PKTTYPE;
      }
  }

  ROHC_EXIT_FN("rohc_cprm_p6_resolve_pkt_typ");
  return ROHC_SUCCESS;
}
/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_sel_pkt_p1_uo_ir
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function determines the pkt type for profile 1 when
 **              the compressor is being operated in U or O mode.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **      RETURN VALUE:
 **              rohc_void_t
 **      NOTES:
 **
 *****************************************************************************/
rohc_return_t rohc_cprm_sel_pkt_p1_uo_ir ROHC_PARAMS
((p_entity,p_csc,p_sel_pkt,p_ecode  ),
 rohc_entity_t      * p_entity _AND_
 rohc_csc_rtp_t     *p_csc     _AND_
 rohc_U8bit_t       *p_sel_pkt _AND_
 rohc_error_code_t  *p_ecode
 )
{
  rohc_encoded_value_t en_sn;
  rohc_encoded_value_t en_ts;
  rohc_encoded_value_t en_ip_id;
  //  rohc_U8bit_t crc_res = ROHC_SUCCESS;

  ROHC_ENTER_FN("rohc_cprm_sel_pkt_p1_uo_ir");
  en_sn.value = en_sn.bits = en_ts.value = en_ts.bits = 0;
  en_ip_id.value = en_ip_id.bits = 0;
  *p_sel_pkt  = ROHC_UNKNOWN_PKT;

  /* update the all static +dynamic information */
  en_sn.value  =p_csc->p_cpib->temp_sn;
  en_sn.bits  = ROHC_P1_SN_BITS;
  en_ts.value  =p_csc->p_cpib->temp_ts;
  en_ts.bits  = ROHC_P1_TS_BITS;
  rohc_ed_p1_update_gsw_sn(p_entity, p_csc, en_sn.value);
  rohc_ed_p1_update_gsw_ts(p_entity, p_csc, en_ts.value);
  en_ip_id.value  = 0;
  if(p_csc->p_cpib->inner_ip4 != ROHC_NULL)
  {
    en_ip_id.value  = p_csc->p_cpib->temp_ipid;
    en_ip_id.bits  = ROHC_P123_IPID_BITS;
  }
  else if(p_csc->p_cpib->outer_ip4 != ROHC_NULL)
  {   
    en_ip_id.value  = p_csc->p_cpib->temp_ipid2;
    en_ip_id.bits  = ROHC_P123_IPID_BITS;
  }   
  /*No need to call encoding as IR packet is selected*/
  rohc_update_p1_ctx(p_entity, p_csc, &en_sn,
      &en_ts, &en_ip_id);
  *p_sel_pkt = ROHC_IR_WITH_DYN_PKTTYPE;
  ROHC_EXIT_FN("rohc_cprm_sel_pkt_p1_uo_ir");
  /* +- SPR 17777 */
/* SPR 20636 Changes Start */
  LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
          __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
          ROHC_GET_CSC_CID(p_csc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
  return ROHC_SUCCESS;

}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_sel_pkt_p1_uo_fo
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function determines the pkt type for profile 1 when
 **              the compressor is being operated in U or O mode.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **      RETURN VALUE:
 **              rohc_void_t
 **      NOTES:
 **
 *****************************************************************************/

rohc_return_t rohc_cprm_sel_pkt_p1_uo_fo ROHC_PARAMS
((p_entity,p_csc,p_sel_pkt,p_ecode  ),
 rohc_entity_t      * p_entity  _AND_
 rohc_csc_rtp_t     *p_csc      _AND_
 rohc_U8bit_t       *p_sel_pkt  _AND_
 rohc_error_code_t  *p_ecode
 )
{
  rohc_encoded_value_t en_sn;
  rohc_encoded_value_t en_ts;
  rohc_encoded_value_t en_ip_id;
  //    rohc_U8bit_t crc_res = ROHC_SUCCESS;

  ROHC_ENTER_FN("rohc_cprm_sel_pkt_p1_uo_fo");
  en_sn.value = en_sn.bits = en_ts.value = en_ts.bits = 0;
  en_ip_id.value = en_ip_id.bits = 0;
  *p_sel_pkt  = ROHC_UNKNOWN_PKT;

  /* compute delta bit map */
  if(ROHC_FAILURE == rohc_cprm_p1_call_encode(p_entity, p_csc, &en_sn ,
        &en_ts , &en_ip_id,p_ecode))
    return ROHC_FAILURE;

  /* +- SPR 17777 */
  p_csc->delta_field = rohc_cprm_p1_compute_delta(p_csc,
      &en_sn , &en_ip_id);

  *p_sel_pkt =
  rohc_cprm_p1_type_uo_fo(p_csc, &en_sn, &en_ts, &en_ip_id);
  /* +- SPR 17777 */
  rohc_cprm_p1_type_updt_encd_val(p_entity, p_csc, &en_sn, &en_ts, &en_ip_id);
  ROHC_EXIT_FN("rohc_cprm_sel_pkt_p1_uo_fo");
  return ROHC_SUCCESS;
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_sel_pkt_p1_uo_so
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function determines the pkt type for profile 1 when
 **              the compressor is being operated in U or O mode.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **      RETURN VALUE:
 **              rohc_void_t
 **      NOTES:
 **
 *****************************************************************************/

rohc_return_t rohc_cprm_sel_pkt_p1_uo_so ROHC_PARAMS
((p_entity,p_csc,p_sel_pkt,p_ecode  ),
 rohc_entity_t      * p_entity  _AND_
 rohc_csc_rtp_t     *p_csc      _AND_
 rohc_U8bit_t       *p_sel_pkt  _AND_
 rohc_error_code_t  *p_ecode
 )
{
    rohc_encoded_value_t en_sn;
    rohc_encoded_value_t en_ts;
    rohc_encoded_value_t en_ip_id;

    ROHC_ENTER_FN("rohc_cprm_sel_pkt_p1_so");
    en_sn.value = en_sn.bits = en_ts.value = en_ts.bits = 0;
    en_ip_id.value = en_ip_id.bits = 0;
    *p_sel_pkt  = ROHC_UNKNOWN_PKT;

    if(ROHC_FAILURE == rohc_cprm_p1_call_encode(p_entity, p_csc, &en_sn ,
                &en_ts , &en_ip_id,p_ecode))
        return ROHC_FAILURE;
    /* +- SPR 17777 */
    p_csc->delta_field = rohc_cprm_p1_compute_delta(p_csc, &en_sn , 
             &en_ip_id);
    /*SPR #1392 */
    if(p_csc->delta_field == ROHC_NULL && en_sn.bits <= 4 && 
            en_ts.bits == 0)
    {
        *p_sel_pkt = ROHC_UO0P1_PKTTYPE;
        (void)rohc_c_crc_p1_compute(p_csc, ROHC_CRC_TYPE_3);
        /* if crc is successful */
        /* It will be always succesfull as  delta_field is null */
        /* therefore if (crc_res) not needed */
        rohc_update_p1_ctx(p_entity, p_csc, &en_sn, &en_ts, &en_ip_id);

    }
    else
    {
        p_csc->c_state  = ROHC_C_ST_FO;
        p_csc->dynamic_info_update_counter = 0;
        ROHC_STOP_TIMER(p_csc->fo_ret_timer.tdata, p_csc->fo_ret_timer.tid);
        *p_sel_pkt =
            rohc_cprm_p1_type_uo_fo(p_csc, &en_sn, &en_ts, &en_ip_id);
        /* +- SPR 17777 */
    }

    rohc_cprm_p1_type_updt_encd_val(p_entity, p_csc, &en_sn, &en_ts, &en_ip_id);

    ROHC_EXIT_FN("rohc_cprm_sel_pkt_p1_uo_so");
    return ROHC_SUCCESS;
}

rohc_void_t rohc_cprm_p1_type_updt_encd_val
ROHC_PARAMS((p_entity, p_csc, p_en_sn, p_en_ts, p_en_ip_id),
    rohc_entity_t           *p_entity   _AND_
    rohc_csc_rtp_t          *p_csc      _AND_
    rohc_encoded_value_t    *p_en_sn    _AND_
    rohc_encoded_value_t    *p_en_ts    _AND_
    rohc_encoded_value_t    *p_en_ip_id
    )
{   
  ROHC_ENTER_FN("rohc_cprm_p1_type_updt_encd_val");
  p_csc->field_value[ROHC_SN_FID-ROHC_VAR_FID_BASE]=p_en_sn->value;
  p_csc->field_len[ROHC_SN_FID-ROHC_VAR_FID_BASE]=p_en_sn->bits;
  p_csc->field_value[ROHC_TS_FID-ROHC_VAR_FID_BASE]=p_en_ts->value;
  p_csc->field_len[ROHC_TS_FID-ROHC_VAR_FID_BASE]=p_en_ts->bits;

  if(p_csc->p_cpib->inner_ip4 != ROHC_NULL)
  {
    p_csc->field_value[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE]=
      p_csc->p_cpib->temp_ipid;

    p_csc->field_len[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE]=
      16;
  }

  if(p_csc->p_cpib->outer_ip4 != ROHC_NULL)
  {
    p_csc->field_value[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE]=
      p_csc->p_cpib->temp_ipid2;

    p_csc->field_len[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE]=16;
  }
  /* +- SPR 17777 */
  LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
          __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
          ROHC_GET_CSC_CID(p_csc),sizeof(p_en_ip_id),0,0, __func__,"");
  /* +- SPR 17777 */
  ROHC_EXIT_FN("rohc_cprm_p1_type_updt_encd_val");
}   

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_p2_updt_encd_val
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function updates the encoded value for profile 2.
 **           
 **      ARGUMENTS:
 **              p_entity      Pointer to the rohc entity block
 **              p_csc         Pointer to the CSC block
 **              p_en_sn       Pointer to the encoded_value structure
 **              p_en_ip_id    Pointer to the encoded_value structure
 **              p_en_ip_id2   Pointer to the encoded_value structure
 **      RETURN VALUE:
 **              rohc_void_t
 **      NOTES:
 **
 *****************************************************************************/

rohc_void_t rohc_cprm_p2_updt_encd_val
ROHC_PARAMS((p_entity, p_csc, p_en_sn, p_en_ip_id,p_en_ip_id2),
    rohc_entity_t        *p_entity    _AND_
    rohc_csc_udp_t       *p_csc       _AND_
    rohc_encoded_value_t *p_en_sn     _AND_
    rohc_encoded_value_t *p_en_ip_id  _AND_
    rohc_encoded_value_t *p_en_ip_id2 
    )
{   
  ROHC_ENTER_FN("rohc_cprm_p2_updt_encd_val");
  p_csc->field_value[ROHC_SN_FID-ROHC_VAR_FID_BASE]=p_en_sn->value;
  p_csc->field_len[ROHC_SN_FID-ROHC_VAR_FID_BASE]=p_en_sn->bits;

  if(p_csc->p_cpib->inner_ip4 != ROHC_NULL)
  {
    p_csc->field_value[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE]=
      p_csc->p_cpib->temp_ipid;

    p_csc->field_len[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE]=
      16;
  }

  if(p_csc->p_cpib->outer_ip4 != ROHC_NULL)
  {
    p_csc->field_value[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE]=
      p_csc->p_cpib->temp_ipid2;

    p_csc->field_len[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE]=16;
  }
  /* +- SPR 17777 */
  LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
          __LINE__,p_entity->rohc_entity_id.ueIndex,sizeof(p_en_ip_id2),
          ROHC_GET_CSC_CID(p_csc),sizeof(p_en_ip_id),0,0, __func__,"");
  /* +- SPR 17777 */
  ROHC_EXIT_FN("rohc_cprm_p2_updt_encd_val");
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_sel_pkt_p1_r_ir
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function determines the pkt type for profile 1 when
 **              the compressor is being operated in R mode.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **      RETURN VALUE:
 **              rohc_void_t
 **      NOTES:
 **
 *****************************************************************************/

rohc_return_t rohc_cprm_sel_pkt_p1_r_ir ROHC_PARAMS
((p_entity, p_csc,p_sel_pkt,p_ecode ),
 rohc_entity_t      * p_entity  _AND_
 rohc_csc_rtp_t     *p_csc      _AND_
 rohc_U8bit_t       *p_sel_pkt  _AND_
 rohc_error_code_t  *p_ecode
 )
{
  rohc_encoded_value_t en_sn;
  rohc_encoded_value_t en_ts;
  rohc_encoded_value_t en_ip_id;
  /*rohc_U8bit_t crc_res = ROHC_SUCCESS;*/
  ROHC_ENTER_FN("rohc_cprm_sel_pkt_p1_r_ir");
  en_sn.value = en_sn.bits = en_ts.value = en_ts.bits = 0;
  en_ip_id.value = en_ip_id.bits = 0;
  *p_sel_pkt = ROHC_UNKNOWN_PKT;

  /*Note : CRC computaton will be called by PBM */
  /* update the all static +dynamic information */
  en_sn.value  =p_csc->p_cpib->temp_sn;
  en_sn.bits  = ROHC_P1_SN_BITS;
  en_ts.value  =p_csc->p_cpib->temp_ts;
  en_ts.bits  = ROHC_P1_TS_BITS;
  rohc_ed_p1_update_gsw_sn(p_entity, p_csc, en_sn.value);
  rohc_ed_p1_update_gsw_ts(p_entity, p_csc, en_ts.value);
  en_ip_id.value  = 0;
  if(p_csc->p_cpib->inner_ip4 != ROHC_NULL)
  {
    en_ip_id.value  = p_csc->p_cpib->temp_ipid;
    en_ip_id.bits  = ROHC_P123_IPID_BITS;
  }else if(p_csc->p_cpib->outer_ip4 != ROHC_NULL)
  {
    en_ip_id.value  = p_csc->p_cpib->temp_ipid2;
    en_ip_id.bits  = ROHC_P123_IPID_BITS;
  }
  rohc_update_p1_ctx (p_entity, p_csc, &en_sn,
                        &en_ts, &en_ip_id);
  /* update gsw */
  *p_sel_pkt =  ROHC_IR_WITH_DYN_PKTTYPE;

  /* +- SPR 17777 */
/* SPR 20636 Changes Start */
  LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
          __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
          ROHC_GET_CSC_CID(p_csc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
  /* +- SPR 17777 */
  ROHC_EXIT_FN("rohc_cprm_sel_pkt_p1_r_ir");
  return ROHC_SUCCESS;
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_sel_pkt_p1_r_fo
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function determines the pkt type for profile 1 when
 **              the compressor is being operated in R mode.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **      RETURN VALUE:
 **              rohc_void_t
 **      NOTES:
 **
 *****************************************************************************/

rohc_return_t rohc_cprm_sel_pkt_p1_r_fo ROHC_PARAMS
((p_entity, p_csc,p_sel_pkt,p_ecode ),
 rohc_entity_t      * p_entity  _AND_
 rohc_csc_rtp_t     *p_csc      _AND_
 rohc_U8bit_t       *p_sel_pkt  _AND_
 rohc_error_code_t  *p_ecode
 )
{
  rohc_encoded_value_t en_sn;
  rohc_encoded_value_t en_ts;
  rohc_encoded_value_t en_ip_id;
  /*rohc_U8bit_t crc_res = ROHC_SUCCESS;*/
  ROHC_ENTER_FN("rohc_cprm_sel_pkt_p1_r_fo");
  en_sn.value = en_sn.bits = en_ts.value = en_ts.bits = 0;
  en_ip_id.value = en_ip_id.bits = 0;
  *p_sel_pkt = ROHC_UNKNOWN_PKT;

  if(ROHC_FAILURE == rohc_cprm_p1_call_encode(p_entity, p_csc, &en_sn ,
                                            &en_ts , &en_ip_id,p_ecode))
    return ROHC_FAILURE;
  /* compute delta bit map */

  /* +- SPR 17777 */
  p_csc->delta_field = rohc_cprm_p1_compute_delta(p_csc, &en_sn , 
                                                        &en_ip_id);

      *p_sel_pkt =  rohc_cprm_p1_type_r_fo(p_entity, p_csc, &en_sn, 
                                            &en_ts, &en_ip_id);
      rohc_cprm_p1_type_updt_encd_val(p_entity, p_csc, &en_sn, &en_ts, &en_ip_id);

  ROHC_EXIT_FN("rohc_cprm_sel_pkt_p1_r_fo");
  return ROHC_SUCCESS;
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_sel_pkt_p1_r_so
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function determines the pkt type for profile 1 when
 **              the compressor is being operated in R mode.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **      RETURN VALUE:
 **              rohc_void_t
 **      NOTES:
 **
 *****************************************************************************/

rohc_return_t rohc_cprm_sel_pkt_p1_r_so  ROHC_PARAMS
((p_entity, p_csc,p_sel_pkt,p_ecode ),
 rohc_entity_t      * p_entity  _AND_
 rohc_csc_rtp_t     *p_csc      _AND_
 rohc_U8bit_t       *p_sel_pkt  _AND_
 rohc_error_code_t  *p_ecode
 )
{
  rohc_encoded_value_t en_sn;
  rohc_encoded_value_t en_ts;
  rohc_encoded_value_t en_ip_id;
  /*rohc_U8bit_t crc_res = ROHC_SUCCESS;*/
  ROHC_ENTER_FN("rohc_cprm_sel_pkt_p1_r_so");
  en_sn.value = en_sn.bits = en_ts.value = en_ts.bits = 0;
  en_ip_id.value = en_ip_id.bits = 0;
  *p_sel_pkt = ROHC_UNKNOWN_PKT;

  if(ROHC_FAILURE == rohc_cprm_p1_call_encode(p_entity, p_csc, &en_sn ,
                                            &en_ts , &en_ip_id,p_ecode))
    return ROHC_FAILURE;

  /* +- SPR 17777 */
  p_csc->delta_field = rohc_cprm_p1_compute_delta(p_csc, &en_sn , 
                                                    &en_ip_id);

  if(p_csc->delta_field == 0 && en_sn.bits <=7 && 
      en_ts.bits == 0 && en_ip_id.bits == 0)
  {
    if((p_csc->no_r0crc_m1 == ROHC_CONT_R0CRC) ||
        (p_csc->no_r0_prtt_m1 >= (p_csc->prtt - ROHC_MAX_M_1_VAL)) )
    {
      *p_sel_pkt = ROHC_R0CRCP1_PKTTYPE ;

      if(p_csc->r0_crc_ack_flag) /*must be set when ack is received */
      {
        p_csc->no_r0_prtt_m1 = 0;
        p_csc->no_r0crc_m1 = 0;
        p_csc->r0_crc_ack_flag = ROHC_FALSE;

      }
      else
      {
        p_csc->no_r0crc_m1 = ROHC_CONT_R0CRC;
      }
    }

    if(en_sn.bits == 7 || p_csc->prtt <= ROHC_MAX_M_1_VAL) 
      /*no other choice because r-0 can not be
        selected */
    {   
      *p_sel_pkt = ROHC_R0CRCP1_PKTTYPE ;
    }   
    else if(p_csc->no_r0crc_since_lastack < (ROHC_PRTT_MAX -
          p_csc->prtt))
    {
      p_csc->no_r0crc_since_lastack++;
      *p_sel_pkt = ROHC_R0CRCP1_PKTTYPE ;
    }
    else if(p_csc->no_r0crc_m1 < ROHC_MAX_M_1_VAL)
    {
      p_csc->no_r0crc_m1++;
      *p_sel_pkt = ROHC_R0CRCP1_PKTTYPE ;
    }
    else if(p_csc->no_r0crc_m1!=ROHC_CONT_R0CRC &&
        p_csc->no_r0_prtt_m1 < (p_csc->prtt - ROHC_MAX_M_1_VAL) )
    {
      p_csc->no_r0_prtt_m1++;
      *p_sel_pkt = ROHC_R0P1_PKTTYPE ;
    }

    en_sn.bits = 6;
    if(*p_sel_pkt == ROHC_R0CRCP1_PKTTYPE)
    {
      en_sn.bits = 7;
      /*Coverity Fix 24387 Start */
      (void)rohc_c_crc_p1_compute(p_csc, ROHC_CRC_TYPE_7);
      /*Coverity Fix 24387 End*/
    }

    /* It will be always succesfull as  delta_field is null*/
    /*if(crc_res) not needed*/
    /*rohc_update_ctx_p1 (p_entity, p_csc,p_csc->delta_field,
      &en_sn, &en_ts, &en_ip_id);*/

  }
  /* 
  ** START_SPR_1197_FIX : Introduced the  handling of 
  ** ROHC_R1P1_PKTTYPE type packets in profile-1 (RTP) for 
  ** inner_ip4.
  */
  else if(p_csc->delta_field == 0 && en_sn.bits <= 6 &&
      en_ts.bits <= 6 && en_ip_id.bits == 0)
  {
      *p_sel_pkt = ROHC_R1P1_PKTTYPE;
      en_sn.bits = rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1].sn_bits;
      en_ts.bits = rohc_p1_pkt_typ_info_r[ROHC_RP1_R_1].ts_bits;
  }
  /* END_SPR_1197_FIX */
  else
  {
    /* call ccpm */
    p_csc->c_state  = ROHC_C_ST_FO;
    /*  */
    /* now state in FO therefore call .... */
    /*SPR #1387 Fix */
    /*          p_csc->dynamic_info_update_counter = 0;*/
    /*            ROHC_STOP_TIMER(p_csc->fo_ret_timer.tid);*/
    /*          not required for r mode */

    *p_sel_pkt = rohc_cprm_p1_type_r_fo(p_entity, p_csc, &en_sn, 
        &en_ts, &en_ip_id);
  }
  rohc_cprm_p1_type_updt_encd_val(p_entity, p_csc, &en_sn, &en_ts, &en_ip_id);
  ROHC_EXIT_FN("rohc_cprm_sel_pkt_p1_r_so");
  return ROHC_SUCCESS;
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_update_p1_ctx
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function updates the CSC for profile 1.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **              delta_field[IN]   Delta bit map for changing fields
 **              en_sn   [IN]      Encoded value of sn
 **              en_ts   [IN]      Encoded value of ts
 **              en_ts   [IN]      Encoded value of ip_id
 **
 **      RETURN VALUE:
 **              rohc_void_t
 **      NOTES:
 **
 *****************************************************************************/
rohc_void_t rohc_update_p1_ctx ROHC_PARAMS(
    (p_entity, p_csc, en_sn, en_ts, en_ip_id),
    rohc_entity_t        *p_entity _AND_
    rohc_csc_rtp_t       *p_csc    _AND_
    rohc_encoded_value_t *en_sn    _AND_
    rohc_encoded_value_t *en_ts    _AND_
    rohc_encoded_value_t *en_ip_id
    )
{

  ROHC_ENTER_FN("rohc_update_p1_ctx");
  /* rtp */
  /* +- SPR 17777 */
  rohc_cprm_p1_update_csc_rtp (p_csc,
      en_sn, en_ts);

  /* for IP */
  if(p_csc->p_cpib->inner_ip4 != ROHC_NULL)
  {
  /* +- SPR 17777 */
    rohc_cprm_p1_update_csc_inner_ip4 (p_csc,
        en_ip_id);
  }
  if(p_csc->p_cpib->outer_ip4 != ROHC_NULL)
  {
  /* +- SPR 17777 */
    rohc_cprm_p1_update_csc_outer_ip4 (p_csc);
  }

  if(p_csc->p_cpib->inner_ip6 != ROHC_NULL)
  {
            /* + SPR 17439 */
    rohc_cprm_p1_update_csc_inner_ip6 (p_entity, (rohc_csc_udp_t*)p_csc);
            /* - SPR 17439 */
  }
  if(p_csc->p_cpib->outer_ip6 != ROHC_NULL)
  {
  /* +- SPR 17777 */
      rohc_cprm_p1_update_csc_outer_ip6 (p_csc);
  }

  /* udp */
  /* +- SPR 17777 */
  rohc_cprm_p1_update_csc_udp(p_csc);
  ROHC_EXIT_FN("rohc_update_p1_ctx");
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_p1_call_encode
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function interact with ED module to
 **              encode the value of sn, ipid and ts for profile p1.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **              en_sn   [OUT]      Encoded value of sn
 **              en_ts   [OUT]      Encoded value of ts
 **              en_ts   [OUT]      Encoded value of ip_id

 **      RETURN VALUE:
 **              rohc_void_t
 **      NOTES:
 **
 *****************************************************************************/
rohc_return_t rohc_cprm_p1_call_encode ROHC_PARAMS(
    (p_entity, p_csc, p_en_sn,
     p_en_ts, p_en_ip_id,p_ecode),
    rohc_entity_t           *p_entity   _AND_
    rohc_csc_rtp_t          *p_csc      _AND_
    rohc_encoded_value_t    *p_en_sn    _AND_
    rohc_encoded_value_t    *p_en_ts    _AND_
    rohc_encoded_value_t    *p_en_ip_id _AND_
    rohc_error_code_t       *p_ecode    
    )
{
    ROHC_ENTER_FN("rohc_cprm_p1_call_encode");
    /* call encode and decode */
  /* +- SPR 17777 */
    if(ROHC_FAILURE == rohc_encode_p1_sn(p_entity,  p_csc,
                p_csc->p_cpib->temp_sn ,p_en_sn,p_ecode))
    {
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_CSC_CID(p_csc),sizeof(p_en_ip_id),0,0, __func__,"");
        return ROHC_FAILURE;
    }
  /* +- SPR 17777 */
    /* Start SPR# 6563 Fix*/
    if( (ROHC_CPRM_IS_CH_TSS(p_csc)) || ( ROHC_TRUE == p_csc->ts_wrap_flag))
    {
        p_en_ts->value  =p_csc->p_cpib->temp_ts;
        p_en_ts->bits  = ROHC_P1_TS_BITS;
    }
    else 
    {
        if(ROHC_FAILURE == rohc_encode_p1_ts(p_entity, p_csc,
                    p_csc->p_cpib->temp_ts,p_en_ts,p_ecode))
            return ROHC_FAILURE;

    }
    /* End SPR# 6563 Fix*/
    ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"
            " Encoded values [SN:value(%u),bits(%u)], "\
            " [TS:value(%u),bits(%u)],"\
            " [IP-ID:value(%u),bits(%u)]\n",
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),
            p_en_sn->value, p_en_sn->bits, 
            p_en_ts->value, p_en_ts->bits, 
            p_en_ip_id->value, p_en_ip_id->bits);
    ROHC_EXIT_FN("rohc_cprm_p1_call_encode");
    return ROHC_SUCCESS;
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_p1_type_uo_fo
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function determines the pkt type for profile 1 when
 **              the compressor is operated in U/O mode and state fo.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **              en_sn   [IN]      Encoded value of sn
 **              en_ts   [IN]      Encoded value of ip_ts
 **              en_ip_id[IN]      Encoded value of ip_id
 **
 **      RETURN VALUE:
 **              rohc_U8bit_t
 **      NOTES:
 **
 *****************************************************************************/
  /* +- SPR 17777 */
rohc_U8bit_t rohc_cprm_p1_type_uo_fo ROHC_PARAMS(
    ( p_csc, en_sn,
     en_ts, en_ip_id),
    rohc_csc_rtp_t          *p_csc      _AND_
    rohc_encoded_value_t    *en_sn      _AND_
    rohc_encoded_value_t    *en_ts      _AND_
    rohc_encoded_value_t    *en_ip_id
    )
{

  rohc_U8bit_t sel_pkt = ROHC_UNKNOWN_PKT;
  rohc_U8bit_t crc_res = ROHC_FAILURE;
  rohc_U8bit_t tsn_bits, tts_bits, tip_id_bits;
  /* determine pkt type with crc bit = 3*/
  ROHC_ENTER_FN("rohc_cprm_p1_type_uo_fo");
  tsn_bits = en_sn->bits;
  tts_bits = en_ts->bits;
  tip_id_bits = en_ip_id->bits;

  /* +- SPR 17777 */
  sel_pkt = rohc_cprm_p1_resolve_pkt_typ_uo(
      p_csc, p_csc->delta_field, en_sn,
      en_ts, en_ip_id, ROHC_PRM_CRC_BITS3);

  /* +- SPR 17777 */
  /* if it is unknow, set it as IR-DYN */
  if(sel_pkt == ROHC_UNKNOWN_PKT)
  {
    /*determine pkt type with crc bit = 7 */
    en_sn->bits = tsn_bits;
    en_ts->bits = tts_bits; 
    en_ip_id->bits = tip_id_bits;

  /* +- SPR 17777 */
    sel_pkt = rohc_cprm_p1_resolve_pkt_typ_uo(
        p_csc, p_csc->delta_field, en_sn,
        en_ts,en_ip_id, ROHC_PRM_CRC_BITS7);

  /* +- SPR 17777 */
    if(sel_pkt == ROHC_UNKNOWN_PKT)
    {
      sel_pkt =  ROHC_IRDYN_PKTTYPE;
    }
    else
    {
      crc_res =
        rohc_c_crc_p1_compute(p_csc, ROHC_CRC_TYPE_7);
      /* +- SPR 17777 */
      if(!crc_res)
      {
        sel_pkt =  ROHC_IRDYN_PKTTYPE;
      }
    }
  }
  else
  {
    /* call CRC moudle   */

    crc_res = rohc_c_crc_p1_compute(p_csc, ROHC_CRC_TYPE_3);
    /* +- SPR 17777 */
    if(!crc_res)
    {
      /* crc failure  */
      /*determine pkt type with crc bit = 7 */
      en_sn->bits = tsn_bits;
      en_ts->bits = tts_bits; 
      en_ip_id->bits = tip_id_bits;

  /* +- SPR 17777 */
      sel_pkt = rohc_cprm_p1_resolve_pkt_typ_uo(
          p_csc, p_csc->delta_field, en_sn,
          en_ts, en_ip_id, ROHC_PRM_CRC_BITS7);
  /* +- SPR 17777 */

      if(sel_pkt == ROHC_UNKNOWN_PKT)
      {
        sel_pkt =  ROHC_IRDYN_PKTTYPE;
      }
      else
      {
        crc_res =
          rohc_c_crc_p1_compute(p_csc, ROHC_CRC_TYPE_7);
        /* +- SPR 17777 */
        if(!crc_res)
        {
          sel_pkt =  ROHC_IRDYN_PKTTYPE;
        }
      }
    }
  }

  /* set sn flag of ext3_flag octet for EXT-3 PKT*/
  /*  if( (en_sn->bits - 8) < tsn_bits)
      {
      p_csc->field_value[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE] |=ROHC_ENC_SN_EXT3FLAGP1_MASK;
      } */
  /* set r-ts flag of ext3_flag octet for EXT-3 PKT*/
  /*  if( (en_ts->bits - 7) < tts_bits)
      {
      p_csc->field_value[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE] |=ROHC_ENC_RTS_EXT3FLAGP1_MASK;
      }*/
  /* set I flag of ext3_flag octet for EXT-3 PKT*/
  /*if( (en_ip_id->bits - 7) < tip_id_bits)
    {
    p_csc->field_value[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE] |=ROHC_ENC_I_EXT3FLAGP1_MASK;
    }*/

  ROHC_EXIT_FN("rohc_cprm_p1_type_uo_fo");
  return sel_pkt;
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_p1_type_r_fo
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function determines the pkt type for profile 2 and 3 when
 **              the compressor is operated in r mode and fo state.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **              en_sn   [IN]      Encoded value of sn
 **              en_ts   [IN]      Encoded value of ip_id
 **              en_ts   [IN]      Encoded value of ip_id2
 **
 **      RETURN VALUE:
 **              rohc_U8bit_t
 **      NOTES:
 **
 *****************************************************************************/
rohc_U8bit_t rohc_cprm_p1_type_r_fo ROHC_PARAMS(
    (p_entity, p_csc, en_sn,
     en_ts, en_ip_id),
    rohc_entity_t           *p_entity   _AND_
    rohc_csc_rtp_t          *p_csc      _AND_
    rohc_encoded_value_t    *en_sn      _AND_
    rohc_encoded_value_t    *en_ts      _AND_
    rohc_encoded_value_t    *en_ip_id
    )
{
  /* +- SPR 17777 */
  LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
          __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
          ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
  rohc_U8bit_t sel_pkt = ROHC_UNKNOWN_PKT;
  rohc_U8bit_t crc_res = ROHC_FAILURE;
  rohc_U8bit_t crc_bits = ROHC_PRM_CRC_BITS0;
  /*rohc_U8bit_t tsn_bits, tts_bits;*/
  ROHC_ENTER_FN("rohc_cprm_p1_type_r_fo_p1");

  if ((p_csc->no_uor2_m2 == ROHC_CONT_UOR2) ||
      (p_csc->no_uor1_prtt_m2 >= (p_csc->prtt - ROHC_MAX_M_2_VAL)))
  {
    crc_bits = ROHC_PRM_CRC_BITS7;
    if(p_csc->uor2_ack_flag)
    {
      p_csc->no_uor2_m2 = 0;
      p_csc->no_uor1_prtt_m2 = 0;
      p_csc->uor2_ack_flag = ROHC_FALSE;
    }
    else
    {
      p_csc->no_uor2_m2 = ROHC_CONT_UOR2;
    }   
  }

  if(p_csc->no_uor2_m2 < ROHC_MAX_M_2_VAL ||
      p_csc->no_uor2_m2 == ROHC_CONT_UOR2 || 
      p_csc->prtt <= ROHC_MAX_M_2_VAL)
  {
    p_csc->no_uor2_m2++;
    crc_bits = ROHC_PRM_CRC_BITS7;
  }
  else if((p_csc->no_uor2_m2 != ROHC_CONT_UOR2) &&
      p_csc->no_uor1_prtt_m2 < (p_csc->prtt - ROHC_MAX_M_2_VAL))
  {
    p_csc->no_uor1_prtt_m2++;
    /* crc_bits = 0*/
  }

  /* note:- if ack is received it will move to SO state */

  /* +- SPR 17777 */
  sel_pkt = rohc_cprm_p1_resolve_pkt_typ_r(
      p_csc, p_csc->delta_field, en_sn,
      en_ts, en_ip_id, crc_bits );

  /* +- SPR 17777 */
  /* if it is unknown, set it as IR-DYN */
  if(sel_pkt == ROHC_UNKNOWN_PKT)
  {
    sel_pkt = ROHC_IRDYN_PKTTYPE;
  }
  else if(crc_bits != ROHC_PRM_CRC_BITS0)
  {
    /* call CRC moudle*/
    crc_res = rohc_c_crc_p1_compute(p_csc, ROHC_CRC_TYPE_7);
    /* +- SPR 17777 */
    if(!crc_res) /* crc FAILED */
    {
      sel_pkt = ROHC_IRDYN_PKTTYPE;
    }
  }
  /* set sn flag of ext3_flag octet for EXT-3 PKT*/
  /*      if( (en_sn->bits - 8) < tsn_bits)
          {
          p_csc->field_value[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE] |=ROHC_ENC_SN_EXT3FLAGP1_MASK;
          } */
  /* set r-ts flag of ext3_flag octet for EXT-3 PKT*/
  /*      if( (en_ts->bits - 7) < tts_bits)
          {
          p_csc->field_value[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE] |=ROHC_ENC_RTS_EXT3FLAGP1_MASK;
          }*/
  /* set I flag of ext3_flag octet for EXT-3 PKT*/
  /*if( (en_ip_id->bits - 7) < tip_id_bits)
    {
    p_csc->field_value[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE] |=ROHC_ENC_I_EXT3FLAGP1_MASK;
    } */ 
  ROHC_EXIT_FN("rohc_cprm_p1_type_r_fo");
  return sel_pkt;
}

/*****************************************************************************
 **      FUNCTION: rohc_cprm_sel_pkt_p2_uo_ir
 **              
 *****************************************************************************
 **
 **      DESCRIPTION:
 **              This function determines the pkt type for profile 2 and 3 when
 **              the compressor is being operated in U/O mode.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **      RETURN VALUE:
 **              rohc_U8bit_t
 **      NOTES:
 **
 *****************************************************************************/
rohc_return_t rohc_cprm_sel_pkt_p2_uo_ir  ROHC_PARAMS
((p_entity, p_csc,p_sel_pkt,p_ecode ),
 rohc_entity_t * p_entity _AND_
 rohc_csc_udp_t *p_csc  _AND_
 rohc_U8bit_t    *p_sel_pkt _AND_
 rohc_error_code_t   *p_ecode
 )
{

  /*rohc_U8bit_t crc_res = ROHC_FAILURE;*/
  rohc_encoded_value_t en_sn;
  rohc_encoded_value_t en_ip_id;
  rohc_encoded_value_t en_ip_id2;
  *p_sel_pkt = ROHC_UNKNOWN_PKT;

  ROHC_ENTER_FN("rohc_cprm_sel_pkt_p2_uo_ir");
  /* +- SPR 17777 */
/* SPR 20636 Changes Start */
  LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
          __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
          ROHC_GET_CSC_CID(p_csc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
  /* +- SPR 17777 */
  en_sn.value = en_sn.bits = 0;
  en_ip_id.value = en_ip_id.bits = 0;
  en_ip_id2.value = en_ip_id2.bits = 0;
  /*Note : CRC computaton will be called by PBM */
  /* update the all static +dynamic information */
    en_sn.value =(p_csc->udp_sn);
    en_sn.bits  = ROHC_P1_SN_BITS; /* SN bit for p1 an p2 are same */
  rohc_ed_p2_update_gsw_sn(p_entity, p_csc, en_sn.value);
  en_ip_id.value  = 0;
  if(p_csc->p_cpib->inner_ip4 != ROHC_NULL)
  {
    en_ip_id.value  = p_csc->p_cpib->temp_ipid;
    en_ip_id.bits  = ROHC_P123_IPID_BITS;
  }

  en_ip_id2.value  = 0;
  if(p_csc->p_cpib->outer_ip4 != ROHC_NULL)
  {
    en_ip_id2.value  = p_csc->p_cpib->temp_ipid2;
    en_ip_id2.bits  = ROHC_P123_IPID_BITS;
  }
  rohc_update_ctx_p2 (p_entity, p_csc, &en_sn,
      &en_ip_id, &en_ip_id2);
  *p_sel_pkt = ROHC_IR_WITH_DYN_PKTTYPE;

  ROHC_EXIT_FN("rohc_cprm_sel_pkt_p2_uo_ir");
  return  ROHC_SUCCESS;
}

/*********************************/

/* for p2 profile                  */
/**********************************/
/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_sel_pkt_p2_uo_fo
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function determines the pkt type for profile 2 and 3 when
 **              the compressor is being operated in U/O mode.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **      RETURN VALUE:
 **              rohc_U8bit_t
 **      NOTES:
 **
 *****************************************************************************/
rohc_return_t rohc_cprm_sel_pkt_p2_uo_fo  ROHC_PARAMS
((p_entity, p_csc,p_sel_pkt,p_ecode ),
 rohc_entity_t       * p_entity _AND_
 rohc_csc_udp_t      *p_csc     _AND_
 rohc_U8bit_t        *p_sel_pkt _AND_
 rohc_error_code_t   *p_ecode
 )
{

  /*rohc_U8bit_t crc_res = ROHC_FAILURE;*/
  rohc_encoded_value_t en_sn;
  rohc_encoded_value_t en_ip_id;
  rohc_encoded_value_t en_ip_id2;
  *p_sel_pkt = ROHC_UNKNOWN_PKT;

  ROHC_ENTER_FN("rohc_cprm_sel_pkt_p2_uo");
  en_sn.value = en_sn.bits = 0;
  en_ip_id.value = en_ip_id.bits = 0;
  en_ip_id2.value = en_ip_id2.bits = 0;

  if(ROHC_FAILURE == rohc_cprm_call_encode_p2(p_entity, p_csc, &en_sn , 
        &en_ip_id , &en_ip_id2,p_ecode))
    return ROHC_FAILURE;

  /* +- SPR 17777 */
  /* compute delta bit map */
  p_csc->delta_field = rohc_cprm_compute_p2_delta(p_csc,
      &en_sn , &en_ip_id , &en_ip_id2);

  /* +- SPR 17777 */
  *p_sel_pkt = rohc_cprm_p2_uo_fo(p_entity, p_csc, &en_sn, &en_ip_id,
      &en_ip_id2);
  rohc_cprm_p2_updt_encd_val(p_entity, p_csc, &en_sn, &en_ip_id,
      &en_ip_id2);

  ROHC_EXIT_FN("rohc_cprm_sel_pkt_p2_uo_fo");
  return  ROHC_SUCCESS;
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_sel_pkt_p2_uo_so
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function determines the pkt type for profile 2 and 3 when
 **              the compressor is being operated in U/O mode.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **      RETURN VALUE:
 **              rohc_U8bit_t
 **      NOTES:
 **
 *****************************************************************************/
rohc_return_t rohc_cprm_sel_pkt_p2_uo_so  ROHC_PARAMS
((p_entity, p_csc,p_sel_pkt,p_ecode ),
 rohc_entity_t      * p_entity _AND_
 rohc_csc_udp_t     *p_csc     _AND_
 rohc_U8bit_t       *p_sel_pkt _AND_
 rohc_error_code_t  *p_ecode
 )
{

  /*rohc_U8bit_t crc_res = ROHC_FAILURE;*/
  rohc_encoded_value_t en_sn;
  rohc_encoded_value_t en_ip_id;
  rohc_encoded_value_t en_ip_id2;
  *p_sel_pkt = ROHC_UNKNOWN_PKT;

  ROHC_ENTER_FN("rohc_cprm_sel_pkt_p2_uo_so");
  en_sn.value = en_sn.bits = 0;
  en_ip_id.value = en_ip_id.bits = 0;
  en_ip_id2.value = en_ip_id2.bits = 0;
  if(ROHC_FAILURE == rohc_cprm_call_encode_p2(p_entity, p_csc, &en_sn ,
        &en_ip_id , &en_ip_id2,p_ecode))
    return ROHC_FAILURE;
  /* compute delta bit map */
  /* +- SPR 17777 */
  p_csc->delta_field = rohc_cprm_compute_p2_delta(p_csc,
      &en_sn , &en_ip_id , &en_ip_id2);
  if(p_csc->delta_field == ROHC_NULL)
  {
    *p_sel_pkt = ROHC_UO0P1_PKTTYPE ;
    /* call crc moudle for 3 bit crc*/
    rohc_c_crc_p2_compute(p_csc, ROHC_CRC_TYPE_3);
    /* +- SPR 17777 */
    /* if crc is successful */
    /* It will be always succesfull as  delta_field is null*/
  }
  else
  {
    /* state updation */
    p_csc->c_state  = ROHC_C_ST_FO;
    /* now state in FO therefore call .... */
    p_csc->dynamic_info_update_counter = 0;
    ROHC_STOP_TIMER(p_csc->fo_ret_timer.tdata, p_csc->fo_ret_timer.tid);
    *p_sel_pkt =
      rohc_cprm_p2_uo_fo(p_entity, p_csc, &en_sn, &en_ip_id, &en_ip_id2);
  }
  rohc_cprm_p2_updt_encd_val(p_entity, p_csc, &en_sn, &en_ip_id,
      &en_ip_id2);

  ROHC_EXIT_FN("rohc_cprm_sel_pkt_p2_uo_so");
  return  ROHC_SUCCESS;
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_sel_pkt_p2_r_ir
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function determines the pkt type for profile 2 and 3 when
 **              the compressor is operated in r mode.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **      RETURN VALUE:
 **              rohc_U8bit_t
 **      NOTES:
 **
 *****************************************************************************/
rohc_return_t rohc_cprm_sel_pkt_p2_r_ir  ROHC_PARAMS
((p_entity, p_csc,p_sel_pkt,p_ecode ),
 rohc_entity_t       * p_entity _AND_
 rohc_csc_udp_t      *p_csc     _AND_
 rohc_U8bit_t        *p_sel_pkt _AND_
 rohc_error_code_t   *p_ecode
 )
{


  rohc_encoded_value_t en_sn;
  rohc_encoded_value_t en_ip_id;
  rohc_encoded_value_t en_ip_id2;
  *p_sel_pkt = ROHC_UNKNOWN_PKT;

  ROHC_ENTER_FN("rohc_cprm_sel_pkt_p2_r_ir");
  /* +- SPR 17777 */
/* SPR 20636 Changes Start */
  LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
          __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
          ROHC_GET_CSC_CID(p_csc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
  en_sn.value = en_sn.bits = 0;
  en_ip_id.value = en_ip_id.bits = 0;
  en_ip_id2.value = en_ip_id2.bits = 0;

  /*Note : CRC computaton will be called by PBM */
  /* update the all static +dynamic information */
  en_sn.value =p_csc->udp_sn;
  en_sn.bits  = ROHC_P23_SN_BITS; /* for ip/udp sn bits =32 */
  rohc_ed_p2_update_gsw_sn(p_entity, p_csc, en_sn.value);
  en_ip_id.value  = 0;
  if(p_csc->p_cpib->inner_ip4 != ROHC_NULL)
  {
    en_ip_id.value  = p_csc->p_cpib->temp_ipid;
    en_ip_id.bits  = ROHC_P123_IPID_BITS;
  }

  en_ip_id2.value  = 0;
  if(p_csc->p_cpib->outer_ip4 != ROHC_NULL)
  {
    en_ip_id2.value  = p_csc->p_cpib->temp_ipid2;
    en_ip_id2.bits  = ROHC_P123_IPID_BITS;
  }
  rohc_update_ctx_p2 (p_entity, p_csc,&en_sn,
      &en_ip_id , &en_ip_id2);
  ROHC_EXIT_FN("rohc_cprm_sel_pkt_p2_r_ir");
  return ROHC_IR_WITH_DYN_PKTTYPE;

  ROHC_EXIT_FN("rohc_cprm_sel_pkt_p2_r_ir");
  return ROHC_SUCCESS;
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_sel_pkt_p2_r_fo
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function determines the pkt type for profile 2 and 3 when
 **              the compressor is operated in r mode.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **      RETURN VALUE:
 **              rohc_U8bit_t
 **      NOTES:
 **
 *****************************************************************************/
rohc_return_t rohc_cprm_sel_pkt_p2_r_fo ROHC_PARAMS
((p_entity, p_csc,p_sel_pkt,p_ecode ),
 rohc_entity_t       * p_entity _AND_
 rohc_csc_udp_t      *p_csc     _AND_
 rohc_U8bit_t        *p_sel_pkt _AND_
 rohc_error_code_t   *p_ecode
 )
{


  rohc_encoded_value_t en_sn;
  rohc_encoded_value_t en_ip_id;
  rohc_encoded_value_t en_ip_id2;
  *p_sel_pkt = ROHC_UNKNOWN_PKT;

  ROHC_ENTER_FN("rohc_cprm_sel_pkt_p2_r_fo");
  en_sn.value = en_sn.bits = 0;
  en_ip_id.value = en_ip_id.bits = 0;
  en_ip_id2.value = en_ip_id2.bits = 0;
  if(ROHC_FAILURE == rohc_cprm_call_encode_p2(p_entity, p_csc, &en_sn ,
                                         &en_ip_id , &en_ip_id2,p_ecode))
    return ROHC_FAILURE;

  /* compute delta bit map */
  /* +- SPR 17777 */
  p_csc->delta_field = rohc_cprm_compute_p2_delta(p_csc,
      &en_sn , &en_ip_id , &en_ip_id2);

  *p_sel_pkt = rohc_cprm_p2_r_fo(p_entity, p_csc, &en_sn, &en_ip_id,
      &en_ip_id2);

  rohc_cprm_p2_updt_encd_val(p_entity, p_csc, &en_sn, &en_ip_id,
      &en_ip_id2);

  ROHC_EXIT_FN("rohc_cprm_sel_pkt_p2_r_fo");
  return ROHC_SUCCESS;
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_sel_pkt_p2_r_so
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function determines the pkt type for profile 2 and 3 when
 **              the compressor is operated in r mode.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **      RETURN VALUE:
 **              rohc_U8bit_t
 **      NOTES:
 **
 *****************************************************************************/
rohc_return_t rohc_cprm_sel_pkt_p2_r_so ROHC_PARAMS
((p_entity, p_csc,p_sel_pkt,p_ecode ),
 rohc_entity_t       * p_entity _AND_
 rohc_csc_udp_t      *p_csc     _AND_
 rohc_U8bit_t        *p_sel_pkt _AND_
 rohc_error_code_t   *p_ecode
 )
{


  rohc_encoded_value_t en_sn;
  rohc_encoded_value_t en_ip_id;
  rohc_encoded_value_t en_ip_id2;
  *p_sel_pkt = ROHC_UNKNOWN_PKT;

  ROHC_ENTER_FN("rohc_cprm_sel_pkt_p2_r_so");
  en_sn.value = en_sn.bits = 0;
  en_ip_id.value = en_ip_id.bits = 0;
  en_ip_id2.value = en_ip_id2.bits = 0;

  if(ROHC_FAILURE == rohc_cprm_call_encode_p2(p_entity, p_csc, &en_sn ,
        &en_ip_id , &en_ip_id2,p_ecode))
    return ROHC_FAILURE;
  /* +- SPR 17777 */
  p_csc->delta_field = rohc_cprm_compute_p2_delta(p_csc,
      &en_sn , &en_ip_id , &en_ip_id2);
  if(p_csc->delta_field == 0 && en_sn.bits <=7 && 
      en_ip_id2.bits == 0 && en_ip_id.bits == 0)
  {
    if((p_csc->no_r0crc_m1 == ROHC_CONT_R0CRC) ||
        (p_csc->no_r0_prtt_m1 >= (p_csc->prtt - ROHC_MAX_M_1_VAL)) )
    {
      *p_sel_pkt = ROHC_R0CRCP1_PKTTYPE ;

      if(p_csc->r0_crc_ack_flag) /*must be set when ack is received */
      {
        p_csc->no_r0_prtt_m1 = 0;
        p_csc->no_r0crc_m1 = 0;
        p_csc->r0_crc_ack_flag = ROHC_FALSE;

      }
      else
      {
        p_csc->no_r0crc_m1 = ROHC_CONT_R0CRC;
      }
    }

    if(en_sn.bits == 7 || p_csc->prtt <= ROHC_MAX_M_1_VAL) 
      /*no other choice because r-0 can not be selected */
    {   
      *p_sel_pkt = ROHC_R0CRCP1_PKTTYPE ;
    }   
    else if(p_csc->no_r0crc_since_lastack < (ROHC_PRTT_MAX -
          p_csc->prtt))
    {
      p_csc->no_r0crc_since_lastack++;
      *p_sel_pkt = ROHC_R0CRCP1_PKTTYPE ;
    }
    else if(p_csc->no_r0crc_m1 < ROHC_MAX_M_1_VAL)
    {
      p_csc->no_r0crc_m1++;
      *p_sel_pkt = ROHC_R0CRCP1_PKTTYPE ;
    }
    else if((p_csc->no_r0crc_m1!=ROHC_CONT_R0CRC) && 
        p_csc->no_r0_prtt_m1 < (p_csc->prtt - ROHC_MAX_M_1_VAL) )
    {
      p_csc->no_r0_prtt_m1++;
      *p_sel_pkt = ROHC_R0P1_PKTTYPE ;
    }

    en_sn.bits = 6;
    if(*p_sel_pkt == ROHC_R0CRCP1_PKTTYPE)
    {
      en_sn.bits = 7;
      /*Coverity Fix 24388 Start*/
      (void)rohc_c_crc_p2_compute(p_csc, ROHC_CRC_TYPE_7);
      /* +- SPR 17777 */
      /*Coverity Fix 24388 End*/
    }

  }
  else
  {

    /* call ccpm with update event */
    p_csc->c_state  = ROHC_C_ST_FO;
    /* now state in FO therefore call .... */
    p_csc->dynamic_info_update_counter = 0;
    ROHC_STOP_TIMER(p_csc->fo_ret_timer.tdata, p_csc->fo_ret_timer.tid);

    *p_sel_pkt = 
      rohc_cprm_p2_r_fo(p_entity, p_csc, &en_sn, &en_ip_id, &en_ip_id2);
  }
  rohc_cprm_p2_updt_encd_val(p_entity, p_csc, &en_sn, &en_ip_id,&en_ip_id2);
  ROHC_EXIT_FN("rohc_cprm_sel_pkt_p2_r_so");
  return ROHC_SUCCESS;
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_update_ctx_p2
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function updates the CSC for profile 1.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **              delta_field[IN]   Delta bit map for changing fields
 **              en_sn   [IN]      Encoded value of sn
 **              en_ts   [IN]      Encoded value of ts
 **              en_ip_id[IN]      Encoded value of ip_id
 **              en_ip_id[IN]      Encoded value of ip_id2
 **              update_flag[IN]   Specifies whether to update the static
 **                                context or not.
 **      RETURN VALUE:
 **              rohc_void_t
 **      NOTES:
 **
 *****************************************************************************/
            /* + SPR 17439 */
rohc_void_t rohc_update_ctx_p2 ROHC_PARAMS(
    (p_entity, p_csc, en_sn, en_ip_id, en_ip_id2),
    rohc_entity_t        *p_entity  _AND_
    rohc_csc_udp_t       *p_csc     _AND_
    rohc_encoded_value_t *en_sn     _AND_
    rohc_encoded_value_t *en_ip_id  _AND_
    rohc_encoded_value_t *en_ip_id2 
            /* - SPR 17439 */

    )
{

  ROHC_ENTER_FN("rohc_update_ctx_p2");
  /* rtp */
    rohc_cprm_update_csc_p2_udp (p_entity, p_csc,
        en_sn);
  /* for IP */
  if(p_csc->p_cpib->inner_ip4 != ROHC_NULL)
  {
    rohc_cprm_update_csc_p2_inner_ip4 (p_entity, p_csc,
        en_ip_id );
  }
  if(p_csc->p_cpib->outer_ip4 != ROHC_NULL)
  {
    rohc_cprm_update_csc_p2_outer_ip4 (p_entity, p_csc,
        en_ip_id2);
  }

  if(p_csc->p_cpib->inner_ip6 != ROHC_NULL)
  {
    rohc_cprm_update_csc_p2_inner_ip6 (p_entity, p_csc );
  }
  if(p_csc->p_cpib->outer_ip6 != ROHC_NULL)
  {
    rohc_cprm_update_csc_p2_outer_ip6 (p_entity, p_csc);
  }


  ROHC_EXIT_FN("rohc_update_ctx_p2");
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_update_ctx_p6
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function updates the CSC for profile 6.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **              delta_field[IN]   Delta bit map for changing fields
 **              en_sn   [IN]      Encoded value of sn
 **              en_msn   [IN]      Encoded value of msn
 **              en_ts   [IN]      Encoded value of ip_id
 **
 **      RETURN VALUE:
 **              rohc_void_t
 **      NOTES:
 **
 *****************************************************************************/
rohc_void_t rohc_update_ctx_p6 ROHC_PARAMS(
    (p_entity, p_csc, en_sn, en_ack, en_msn, en_ip_id),
    rohc_entity_t        *p_entity _AND_
    rohc_csc_tcp_t       *p_csc    _AND_
    rohc_encoded_value_t *en_sn    _AND_
    rohc_encoded_value_t *en_ack    _AND_
    rohc_encoded_value_t *en_msn    _AND_
    rohc_encoded_value_t *en_ip_id
    )
{
  /* +- SPR 17777 */
  LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
          __LINE__,p_entity->rohc_entity_id.ueIndex,sizeof(en_ip_id),
          ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");

  ROHC_ENTER_FN("rohc_update_ctx_p6");
  /* tcp */
  /* +- SPR 17777 */
  rohc_cprm_p6_update_csc_tcp (p_csc,
      en_sn, en_ack ,en_msn);

  /* for IP */
  if(p_csc->p_cpib->inner_ip4 != ROHC_NULL)
  {
  /* +- SPR 17777 */
    rohc_cprm_p6_update_csc_inner_ip4 (p_csc,
        en_ip_id);
  }
  /*  
  if(p_csc->p_cpib->outer_ip4 != ROHC_NULL)
  {
    rohc_cprm_p1_update_csc_outer_ip4 (p_entity, p_csc);
  }

  if(p_csc->p_cpib->inner_ip6 != ROHC_NULL)
  {
    rohc_cprm_p1_update_csc_inner_ip6 (p_entity, p_csc);
  }
  if(p_csc->p_cpib->outer_ip6 != ROHC_NULL)
  {
    rohc_cprm_p1_update_csc_outer_ip6 (p_entity, p_csc
        );
  }*/

  ROHC_EXIT_FN("rohc_update_ctx_p6");
}
/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_p2_uo_fo
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function determines the pkt type for profile 2 and 3 when
 **              the compressor is operated in U/O mode and state fo.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **              en_sn   [IN]      Encoded value of sn
 **              en_ip_id [IN]      Encoded value of ip_id
 **              en_ip_id2[IN]      Encoded value of ip_id2
 **
 **      RETURN VALUE:
 **              rohc_U8bit_t
 **      NOTES:
 **
 *****************************************************************************/
rohc_U8bit_t rohc_cprm_p2_uo_fo ROHC_PARAMS(
    (p_entity, p_csc, en_sn,
     en_ip_id, en_ip_id2),
    rohc_entity_t *p_entity _AND_
    rohc_csc_udp_t *p_csc _AND_
    rohc_encoded_value_t *en_sn _AND_
    rohc_encoded_value_t *en_ip_id  _AND_
    rohc_encoded_value_t *en_ip_id2
    )
{

  rohc_U8bit_t sel_pkt = ROHC_UNKNOWN_PKT;
  rohc_U8bit_t crc_res = ROHC_FAILURE;
  rohc_U8bit_t tsn_bits,tip_id_bits,tip_id2_bits;

  /* +- SPR 17777 */
  LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
          __LINE__,p_entity->rohc_entity_id.ueIndex,sizeof(en_ip_id),
          ROHC_GET_CSC_CID(p_csc),sizeof(en_ip_id2),0,0, __func__,"");
  /* determine pkt type with crc bit = 3*/
  ROHC_ENTER_FN("rohc_cprm_p2_uo_fo");
  tsn_bits = en_sn->bits;
  tip_id_bits = en_ip_id->bits;
  tip_id2_bits = en_ip_id2->bits;

  /* +- SPR 17777 */
  sel_pkt = rohc_cprm_resolve_pkt_typ_p2_uo(
      p_csc, p_csc->delta_field, en_sn,
      en_ip_id, en_ip_id2, 3);

  /* +- SPR 17777 */
  /* if it is unknow, set it as IR-DYN */
  if(sel_pkt == ROHC_UNKNOWN_PKT)
  {
    /*determine pkt type with crc bit = 7 */
    en_sn->bits = tsn_bits;
    en_ip_id->bits = tip_id_bits;
    en_ip_id2->bits = tip_id2_bits;

  /* +- SPR 17777 */
    sel_pkt = rohc_cprm_resolve_pkt_typ_p2_uo(
        p_csc, p_csc->delta_field, en_sn,
        en_ip_id, en_ip_id2, ROHC_PRM_CRC_BITS7);

  /* +- SPR 17777 */
    if(sel_pkt == ROHC_UNKNOWN_PKT)
    {
      sel_pkt = ROHC_IRDYN_PKTTYPE;
    }
    else
    {
      crc_res =
        rohc_c_crc_p2_compute(p_csc, ROHC_CRC_TYPE_7);
      /* +- SPR 17777 */
      if(!crc_res)
      {
        sel_pkt = ROHC_IRDYN_PKTTYPE;
      }
    }
  }
  else
  {
    /* call CRC moudle   */

    crc_res = rohc_c_crc_p2_compute(p_csc, ROHC_CRC_TYPE_3);
    /* +- SPR 17777 */
    if(!crc_res)
    {
      /* crc failure  */
      /*determine pkt type with crc bit = 7 */
      en_sn->bits = tsn_bits;
      en_ip_id->bits = tip_id_bits;
      en_ip_id2->bits = tip_id2_bits;

  /* +- SPR 17777 */
      sel_pkt = rohc_cprm_resolve_pkt_typ_p2_uo(
          p_csc, p_csc->delta_field, en_sn,
          en_ip_id, en_ip_id2, ROHC_PRM_CRC_BITS7);

  /* +- SPR 17777 */
      if(sel_pkt == ROHC_UNKNOWN_PKT)
      {
        sel_pkt =  ROHC_IRDYN_PKTTYPE;
      }
      else
      {
        crc_res =
          rohc_c_crc_p2_compute(p_csc, ROHC_CRC_TYPE_7);
        /* +- SPR 17777 */
        if(!crc_res)
        {
          sel_pkt =  ROHC_IRDYN_PKTTYPE;
        }
      }
    }

  }

  ROHC_EXIT_FN("rohc_cprm_p2_uo_fo");
  return sel_pkt;
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_p2_r_fo
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function determines the pkt type for profile 2 and 3 when
 **              the compressor is operated in r mode and fo state.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **              en_sn   [IN]      Encoded value of sn
 **              en_ip_id [IN]      Encoded value of ip_id
 **              en_ip_id2[IN]      Encoded value of ip_id2
 **
 **      RETURN VALUE:
 **              rohc_U8bit_t
 **      NOTES:
 **
 *****************************************************************************/
rohc_U8bit_t rohc_cprm_p2_r_fo ROHC_PARAMS(
    (p_entity, p_csc, en_sn,
     en_ip_id, en_ip_id2),
    rohc_entity_t *p_entity _AND_
    rohc_csc_udp_t *p_csc _AND_
    rohc_encoded_value_t *en_sn _AND_
    rohc_encoded_value_t *en_ip_id  _AND_
    rohc_encoded_value_t *en_ip_id2
    )
{

  rohc_U32bit_t crc_bits  = 0;
  rohc_U8bit_t sel_pkt  = ROHC_UNKNOWN_PKT;
  rohc_U8bit_t crc_res = ROHC_FAILURE;
  /*rohc_U8bit_t tsn_bits;*/
  ROHC_ENTER_FN("rohc_cprm_p2_r_fo");

  /* +- SPR 17777 */
  LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
          __LINE__,p_entity->rohc_entity_id.ueIndex,sizeof(en_ip_id),
          ROHC_GET_CSC_CID(p_csc),sizeof(en_ip_id2),0,0, __func__,"");
  if ((p_csc->no_uor2_m2 == ROHC_CONT_UOR2) ||
      (p_csc->no_uor1_prtt_m2 >= (p_csc->prtt - ROHC_MAX_M_2_VAL)))
  {
    crc_bits = ROHC_PRM_CRC_BITS7;
    if(p_csc->uor2_ack_flag)
    {
      p_csc->no_uor2_m2 = 0;
      p_csc->no_uor1_prtt_m2 = 0;
      p_csc->uor2_ack_flag = ROHC_FALSE;
    }
    else
    {
      p_csc->no_uor2_m2 = ROHC_CONT_UOR2;
    }   
  }

  if(p_csc->no_uor2_m2 < ROHC_MAX_M_2_VAL || 
      p_csc->no_uor2_m2 == ROHC_CONT_UOR2  ||
      p_csc->prtt <= ROHC_MAX_M_2_VAL)
  {
    p_csc->no_uor2_m2++;
    crc_bits = ROHC_PRM_CRC_BITS7;
  }
  else if((p_csc->no_uor2_m2 != ROHC_CONT_UOR2) &&
      p_csc->no_uor1_prtt_m2 < (p_csc->prtt - ROHC_MAX_M_2_VAL) )
  {
    p_csc->no_uor1_prtt_m2++;
    /* crc_bits = 0*/
  }


  /* note:- if ack is received it will move to SO state */

  /* +- SPR 17777 */
  sel_pkt = rohc_cprm_resolve_pkt_typ_p2_r(
      p_csc, p_csc->delta_field, en_sn,
      en_ip_id, en_ip_id2, (rohc_U8bit_t)crc_bits );

  /* +- SPR 17777 */

  /* if it is unknown, set it as IR-DYN */
  if(sel_pkt == ROHC_UNKNOWN_PKT)
  {
    sel_pkt = ROHC_IRDYN_PKTTYPE;
  }
  else if(crc_bits != ROHC_PRM_CRC_BITS0)
  {
    /* call CRC moudle*/
    crc_res = rohc_c_crc_p2_compute( p_csc, ROHC_CRC_TYPE_7);
    /* +- SPR 17777 */
    if(crc_res) /* crc FAILED */
    {
      sel_pkt = ROHC_IRDYN_PKTTYPE;
    }
  }

  /* set sn flag of ext3_flag octet for EXT-3 PKT*/
  /*      if( (en_sn->bits - 8) < tsn_bits)
          {
          p_csc->field_value[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE] |=ROHC_ENC_SN_EXT3FLAGP1_MASK;
          } 
   */
  /* set I flag of ext3_flag octet for EXT-3 PKT*/
  /*    if( (en_ip_id->bits - 7) < tip_id_bits)
        {
        p_csc->field_value[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE] |=ROHC_ENC_I_EXT3FLAGP1_MASK;
        } */
  ROHC_EXIT_FN("rohc_cprm_p2_r_fo");
  return sel_pkt;
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_call_encode_p2
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function interact with ED module to
 **              encode the value of sn, ipid and ts for profile p1.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **              en_sn   [OUT]     Encoded value of sn
 **              en_ip_id [OUT]     Encoded value of ip_id(inner)
 **              en_ip_id2[OUT]     Encoded value of ip_id2(outer)
 **
 **      RETURN VALUE:
 **              rohc_U8bit_t
 **      NOTES:
 **
 *****************************************************************************/
rohc_return_t rohc_cprm_call_encode_p2 ROHC_PARAMS(
    (p_entity, p_csc, p_en_sn,
     p_en_ip_id,p_en_ip_id2,p_ecode),
    rohc_entity_t           *p_entity    _AND_
    rohc_csc_udp_t          *p_csc       _AND_
    rohc_encoded_value_t    *p_en_sn     _AND_
    rohc_encoded_value_t    *p_en_ip_id  _AND_
    rohc_encoded_value_t    *p_en_ip_id2 _AND_
    rohc_error_code_t       *p_ecode
    )
{
  ROHC_ENTER_FN("rohc_cprm_call_encode_p2");
  /* call encode for sn, ts , ip , ip2 */
  /* +- SPR 17777 */
    if(ROHC_FAILURE == rohc_encode_p2_sn(p_entity, p_csc, p_csc->udp_sn,p_en_sn,
          p_ecode))
    {
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_entity->rohc_entity_id.ueIndex,sizeof(p_en_ip_id),
                ROHC_GET_CSC_CID(p_csc),sizeof(p_en_ip_id2),0,0, __func__,"");
        return ROHC_FAILURE;
    }
  /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"
      " Encoded values [SN:value(%u),bits(%u)], "\
      " [IP-ID:value(%u),bits(%u)]"\
      " [IP-ID2:value(%u),bits(%u)]\n",
      p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
      ROHC_GET_CSC_CID(p_csc),
      p_en_sn->value, p_en_sn->bits, 
      p_en_ip_id->value, p_en_ip_id->bits,
      p_en_ip_id2->value, p_en_ip_id2->bits
      );

  ROHC_EXIT_FN("rohc_cprm_call_encode_p2");
  return ROHC_SUCCESS;
}
/* SPR 9591 fix start */
/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_p0_process_feedback
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              Provides the handling for received feedback packet at the
 **              compressor side for profile 0.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **              p_fp_buffer[IN]   Pointer to the feedback packet received
 **              len     [IN]      Length of p_buff
 **
 **      RETURN VALUE:
 **              rohc_void_t
 **      NOTES:
 **
 *****************************************************************************/
            /* + SPR 17439 */
rohc_return_t rohc_cprm_p0_process_feedback ROHC_PARAMS(
    (p_entity, p_csc_ctx, p_fp_buffer,
     len, p_ecode),
    rohc_entity_t       * p_entity      _AND_
    rohc_csc_common_t      *p_csc_ctx          _AND_
    rohc_U8bit_t         *p_fp_buffer    _AND_
    rohc_U32bit_t        len             _AND_
            /* - SPR 17439 */
    rohc_error_code_t   *p_ecode)
{
    ROHC_ENTER_FN("rohc_cprm_p0_process_feedback");
    rohc_U8bit_t feedback_size = 0;
    rohc_U8bit_t code = 0;

    code =(*((rohc_U8bit_t*)p_fp_buffer) & ROHC_FB_CODE_MASK);
    /* CID for profile 0 is 0, Add CID octet is not present */
    /* Non zero code field indicates size= code*/
    if (code)
    {
        feedback_size = code;
    }
    /* If code field is 0, then size field is present */
    else 
    {
        feedback_size = (*((rohc_U8bit_t*)p_fp_buffer + 1));
    }
    /* Length of the feedback packet should be >= feedback_data size */
    if (len < feedback_size)
    {
        ROHC_UT_TRACE((stderr, "Unexpected feedback received\n"));  
    }
    /* Feedback for profile 0 is always ACK */
    if (1 == feedback_size)
    {
        ROHC_UT_TRACE((stderr, "Feedback type 1 received\n"));  
    }
    else
    {
        ROHC_UT_TRACE((stderr, "Unexpected feedback received\n"));  
    }

  /* +- SPR 17777 */
/* SPR 20636 Changes Start */
  LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
          __LINE__,p_entity->rohc_entity_id.ueIndex,sizeof(p_csc_ctx),
                (ADDR)p_ecode,0,0,0, __func__,"");
/* SPR 20636 Changes End */
  /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_cprm_p0_process_feedback");

    return ROHC_SUCCESS;
}
/* SPR 9591 fix end */

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_p1_process_feedback
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              Provides the handling for received feedback packet at the
 **              compressor side.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **              p_fp_buffer[IN]   Pointer to the feedback packet received
 **              len     [IN]      Length of p_buff
 **
 **      RETURN VALUE:
 **              rohc_void_t
 **      NOTES:
 **
 *****************************************************************************/
    /* + SPR 17439 */
rohc_return_t rohc_cprm_p1_process_feedback ROHC_PARAMS(
    (p_entity, p_csc_ctx, p_fp_buffer,
     len, p_ecode),
    rohc_entity_t       * p_entity      _AND_
    rohc_csc_common_t      *p_csc_ctx          _AND_
    rohc_U8bit_t         *p_fp_buffer    _AND_
    rohc_U32bit_t        len             _AND_
    rohc_error_code_t   *p_ecode)
{
    rohc_csc_rtp_t *p_csc = NULL;
    p_csc = (rohc_csc_rtp_t*)p_csc_ctx;
    /* - SPR 17439 */
  ROHC_ENTER_FN("rohc_cprm_p1_process_feedback");
  if(!rohc_cprm_p1_pr_fb(p_entity, p_csc, (rohc_U8bit_t *)p_fp_buffer, 
                                                    len, p_ecode))
  {
    ROHC_EXIT_FN("rohc_cprm_p1_process_feedback");
    return ROHC_FAILURE;
  }
  ROHC_EXIT_FN("rohc_cprm_p1_process_feedback");
  return ROHC_SUCCESS;
}
/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_p2_process_feedback
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              Provides the handling for received feedback packet at the
 **              compressor side.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **              p_fp_buffer[IN]   Pointer to the feedback packet received
 **              len     [IN]      Length of p_buff
 **
 **      RETURN VALUE:
 **              rohc_void_t
 **      NOTES:
 **
 *****************************************************************************/
    /* + SPR 17439 */
rohc_return_t rohc_cprm_p2_process_feedback ROHC_PARAMS(
    (p_entity, p_csc_ctx, p_fp_buffer,
     len, p_ecode),
    rohc_entity_t * p_entity _AND_
    rohc_csc_common_t  *p_csc_ctx   _AND_
    rohc_U8bit_t *p_fp_buffer _AND_
    rohc_U32bit_t len _AND_
    rohc_error_code_t    *p_ecode)
{
    rohc_csc_udp_t *p_csc = NULL;
    p_csc = (rohc_csc_udp_t*)p_csc_ctx;
    /* - SPR 17439 */
  ROHC_ENTER_FN("rohc_cprm_p2_process_feedback");
  if(!rohc_cprm_p2_pr_fb(p_entity, p_csc, 
                    (rohc_U8bit_t *)p_fp_buffer, len, p_ecode))
  {
    ROHC_EXIT_FN("rohc_cprm_p2_process_feedback");
    return ROHC_FAILURE;
  }
  ROHC_EXIT_FN("rohc_cprm_p2_process_feedback");
  return ROHC_SUCCESS;
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_p6_process_feedback
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              Provides the handling for received feedback packet at the
 **              compressor side.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **              p_fp_buffer[IN]   Pointer to the feedback packet received
 **              len     [IN]      Length of p_buff
 **
 **      RETURN VALUE:
 **              rohc_void_t
 **      NOTES:
 **
 *****************************************************************************/
    /* + SPR 17439 */
rohc_return_t rohc_cprm_p6_process_feedback ROHC_PARAMS(
    (p_entity, p_csc_ctx, p_fp_buffer,
     len, p_ecode),
    rohc_entity_t       * p_entity      _AND_
    rohc_csc_common_t      *p_csc_ctx          _AND_
    rohc_U8bit_t         *p_fp_buffer    _AND_
    rohc_U32bit_t        len             _AND_
    rohc_error_code_t   *p_ecode)
{
    rohc_csc_tcp_t *p_csc = NULL;
    p_csc = (rohc_csc_tcp_t*)p_csc_ctx;
    /* - SPR 17439 */
  ROHC_ENTER_FN("rohc_cprm_p6_process_feedback");
  if(!rohc_cprm_p6_pr_fb(p_entity, p_csc, (rohc_U8bit_t *)p_fp_buffer, 
                                                    len, p_ecode))
  {
    ROHC_EXIT_FN("rohc_cprm_p6_process_feedback");
    return ROHC_FAILURE;
  }
  ROHC_EXIT_FN("rohc_cprm_p6_process_feedback");
  return ROHC_SUCCESS;
}

/*****************************************************************************
 **      FUNCTION: rohc_cprm_sel_pkt_p6_ir
 **              
 *****************************************************************************
 **
 **      DESCRIPTION:
 **              This function determines the pkt type for profile 6 when
 **              the compressor is being operated in U/O mode.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **      RETURN VALUE:
 **              rohc_U8bit_t
 **      NOTES:
 **
 *****************************************************************************/
rohc_return_t rohc_cprm_sel_pkt_p6_ir  ROHC_PARAMS
((p_entity, p_csc,p_sel_pkt,p_ecode ),
 rohc_entity_t * p_entity _AND_
 rohc_csc_tcp_t *p_csc  _AND_
 rohc_U8bit_t    *p_sel_pkt _AND_
 rohc_error_code_t   *p_ecode
 )
{

  
  rohc_encoded_value_t en_sn;
  rohc_encoded_value_t en_ack;
  rohc_encoded_value_t en_msn;
  rohc_encoded_value_t en_ip_id;
  *p_sel_pkt = ROHC_UNKNOWN_PKT;

  ROHC_ENTER_FN("rohc_cprm_sel_pkt_p6_ir");
  en_sn.value = en_sn.bits = 0;
  en_ack.value = en_ack.bits = 0;
  en_ip_id.value = en_ip_id.bits = 0;
  en_msn.value = en_msn.bits = 0;
  /*Note : CRC computaton will be called by PBM */
  /* update the all static +dynamic information */
    en_sn.value = p_csc->p_cpib->temp_sn;
    en_sn.bits  = ROHC_P6_SN_BITS;
    en_ack.value = p_csc->p_cpib->temp_ack;
    en_ack.bits  = ROHC_P6_SN_BITS;
 

    en_msn.value = p_csc->tcp_msn;
    en_msn.bits  = ROHC_P6_MSN_BITS;
    rohc_ed_p6_update_gsw_sn(p_entity, p_csc, en_sn.value);
    rohc_ed_p6_update_gsw_ack(p_entity, p_csc, en_ack.value);
 
  en_ip_id.value  = 0;
  if(p_csc->p_cpib->inner_ip4 != ROHC_NULL)
  {
    en_ip_id.value  = p_csc->p_cpib->temp_ipid;
    en_ip_id.bits  = ROHC_P6_IPID_BITS;
  }
  rohc_update_ctx_p6 (p_entity, p_csc, &en_sn, &en_ack, &en_msn, &en_ip_id);
  *p_sel_pkt = ROHC_IR_WITH_DYN_PKTTYPE;

  /* +- SPR 17777 */
/* SPR 20636 Changes Start */
  LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
          __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_CSC_CID(p_csc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
  /* +- SPR 17777 */
  ROHC_EXIT_FN("rohc_cprm_sel_pkt_p6_ir");
  return  ROHC_SUCCESS;
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_sel_p6_co_pkt
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function determines the pkt type for profile 6 when
 **              the compressor is being operated in U or O mode.
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **      RETURN VALUE:
 **              rohc_void_t
 **      NOTES:
 **
 *****************************************************************************/

rohc_return_t rohc_cprm_sel_p6_co_pkt ROHC_PARAMS
((p_entity,p_csc,p_sel_pkt,p_ecode  ),
 rohc_entity_t      * p_entity  _AND_
 rohc_csc_tcp_t     *p_csc      _AND_
 rohc_U8bit_t       *p_sel_pkt  _AND_
 rohc_error_code_t  *p_ecode
 )
{
  rohc_encoded_value_t en_sn, en_scaled_sn;
  rohc_encoded_value_t en_ack, en_scaled_ack;
    rohc_encoded_value_t en_ip_id;
    ROHC_ENTER_FN("rohc_cprm_sel_p6_co_pkt");
  en_sn.value = en_sn.bits = en_ack.value = en_ack.bits = 0;
  en_scaled_sn.value = en_scaled_sn.bits = en_scaled_ack.value =
      en_scaled_ack.bits = 0;
    en_ip_id.value = en_ip_id.bits = 0;
  *p_sel_pkt  = ROHC_UNKNOWN_PKT;

  /* +- SPR 17777 */
  p_csc->delta_field = rohc_cprm_p6_compute_delta(p_csc);

  
    if(ROHC_FAILURE == rohc_cprm_p6_pkt_det(p_entity, p_csc, p_csc->delta_field,
                &en_sn,&en_scaled_sn, &en_ack, &en_scaled_ack,&en_ip_id,
                p_sel_pkt))
    {
/* SPR 20636 Changes Start */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_CSC_CID(p_csc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
        return ROHC_FAILURE;
    }
  /* +- SPR 17777 */
    rohc_cprm_p6_type_updt_encd_val(p_entity, p_csc, &en_sn, &en_scaled_sn, 
            &en_ack, &en_scaled_ack, &en_ip_id,p_sel_pkt);
    /* update min and max values in sliding window */
   rohc_ed_p6_update_gsw_sn(p_entity,p_csc,
           p_csc->field_value[ROHC_SN_FID-ROHC_VAR_FID_BASE]);
  /*rohc_ed_p6_update_gsw_sn_scaled(p_entity, p_csc,
          p_csc->field_value[ROHC_TCP_SCALED_SN_FID-ROHC_VAR_FID_BASE]);*/
  rohc_ed_p6_update_gsw_ack(p_entity,p_csc,
          p_csc->field_value[ROHC_TCP_ACKNO_FID-ROHC_VAR_FID_BASE]);
 /* rohc_ed_p6_update_gsw_ack_scaled(p_entity, p_csc,
          p_csc->field_value[ROHC_TCP_SCALED_ACKNO_FID-ROHC_VAR_FID_BASE]);*/
   
  return ROHC_SUCCESS;
    ROHC_EXIT_FN("rohc_cprm_sel_p6_co_pkt");
}

/*****************************************************************************
 **      FUNCTION:
 **              rohc_cprm_p6_type_updt_encd_val
 ******************************************************************************
 **
 **      DESCRIPTION:
 **              This function updated the encoded values for prof 6
 **              
 **      ARGUMENTS:
 **              p_entity[IN]      Pointer to the rohc entity block
 **              p_csc   [IN]      Pointer to the CSC block
 **              p_en_sn   [IN]    Encoded value of sn
 **              p_en_scaled_sn[IN]Encoded value of scaled sn
 **              p_en_ack[IN]      Encoded value of ack
 **              p_en_scaled_ack[IN] Encoded value of scaled ack
 **              p_en_ip_id[IN] Encoded value of ip id
 **
 **      RETURN VALUE:
 **              void
 **      NOTES:
 **
 *****************************************************************************/
rohc_void_t rohc_cprm_p6_type_updt_encd_val
ROHC_PARAMS((p_entity, p_csc, p_en_sn, p_en_scaled_sn, p_en_ack, 
            p_en_scaled_ack, p_en_ip_id, p_sel_pkt),
    rohc_entity_t           *p_entity         _AND_
    rohc_csc_tcp_t          *p_csc            _AND_
    rohc_encoded_value_t    *p_en_sn          _AND_
    rohc_encoded_value_t    *p_en_scaled_sn   _AND_
    rohc_encoded_value_t    *p_en_ack         _AND_
    rohc_encoded_value_t    *p_en_scaled_ack  _AND_
    rohc_encoded_value_t    *p_en_ip_id       _AND_ /* for seq pkts to be done later*/
    rohc_U8bit_t       *p_sel_pkt
    )
{   
    ROHC_ENTER_FN("rohc_cprm_p6_type_updt_encd_val");
    /* Field val of SN, ACK and Scaled SN, ACK is already updated in  compute_delta func.
     * Here we will update field len of these fields obtained after encoding
     * Note-  If field_len is 0 means value of that field is not changed*/
    switch(*p_sel_pkt)
    {
        case ROHC_CO_P6_RND1_PKTYPE:
            p_en_sn->bits = 18;
            break;

        case ROHC_CO_P6_RND2_PKTYPE:
            p_en_scaled_sn->bits = 4;
            break;

        case ROHC_CO_P6_RND3_PKTYPE:
            p_en_ack->bits = 15;
            break;

        case ROHC_CO_P6_RND4_PKTYPE:
             p_en_scaled_ack->bits = 4;
            break;

        case ROHC_CO_P6_RND5_PKTYPE:
            p_en_sn->bits = 14;
            p_en_ack->bits = 15;
            break;

        case ROHC_CO_P6_RND6_PKTYPE:
            p_en_scaled_sn->bits = 4;
            p_en_ack->bits = 16;
            break;

        case ROHC_CO_P6_RND7_PKTYPE:
            p_en_ack->bits = 18;
            break;

        case ROHC_CO_P6_RND8_PKTYPE:
            p_en_sn->bits = 16;
            p_en_ack->bits = 16;
            break;

        default:
  /* +- SPR 17777 */
            LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                    ROHC_GET_CSC_CID(p_csc),sizeof(p_en_ip_id),0,0, __func__,"");
            break;
    }
    p_csc->field_len[ROHC_SN_FID-ROHC_VAR_FID_BASE] = p_en_sn->bits;
    p_csc->field_len[ROHC_TCP_SCALED_SN_FID-ROHC_VAR_FID_BASE] = p_en_scaled_sn->bits;
    p_csc->field_len[ROHC_TCP_ACKNO_FID-ROHC_VAR_FID_BASE] = p_en_ack->bits;
    p_csc->field_len[ROHC_TCP_SCALED_ACKNO_FID-ROHC_VAR_FID_BASE] = p_en_scaled_ack->bits;
    p_csc->field_len[ROHC_TCP_MSN_FID-ROHC_VAR_FID_BASE] = 4;

    if(p_csc->p_cpib->inner_ip4 != ROHC_NULL)
    {
        /* currently supporting rnd behavior only, so filling ip id 
         * as received in uncompressed header*/
        p_csc->field_value[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE]=
            p_csc->p_cpib->temp_ipid;

        p_csc->field_len[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE]=
            ROHC_P6_IPID_BITS;
        p_csc->ctrl_field_val_p6[ROHC_TCP_IPID_IND_FID
            -ROHC_PROF6_VAR_CTRL_FID_BASE] = 0;
    }

    if(p_csc->p_cpib->outer_ip4 != ROHC_NULL)
    {
        p_csc->field_value[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE]=
            p_csc->p_cpib->temp_ipid2;

        p_csc->field_len[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE]=16;
    }
    ROHC_EXIT_FN("rohc_cprm_p6_type_updt_encd_val");
}   
