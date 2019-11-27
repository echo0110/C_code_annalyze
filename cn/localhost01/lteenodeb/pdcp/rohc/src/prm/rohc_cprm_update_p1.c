/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_cprm_update_p1.c,v 1.3.36.1 2010/11/09 05:25:05 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the function definition for updating
 *                     the context (CSC) from DPIB for profile p1(IP/UDP/RTP).
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_cprm_update_p1.c,v $
 * Revision 1.3.36.1  2010/11/09 05:25:05  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.3  2009/05/28 04:26:50  gur19836
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
/* +- SPR 17777 */
extern rohc_p1_pkt_typ_info_t 
rohc_p1_pkt_typ_info_uo[ROHC_P1_UO_MAX_PACKETS];

extern rohc_p1_pkt_typ_info_t
rohc_p1_pkt_typ_info_r[ROHC_P1_R_MAX_PACKETS];

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/



/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_p1_update_csc_rtp
******************************************************************************
**
**      DESCRIPTION:
**              This funtion updates from CPIB to csc for rtp 
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**              delta_field[IN]   Computed Delta field 
**              en_sn[IN]         Encoded value of sn
**              st_update_flag[IN]   Specifies whether to update the static 
**                                context or not.
**              
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/
/* +- SPR 17777 */
rohc_void_t rohc_cprm_p1_update_csc_rtp ROHC_PARAMS(
            ( p_csc,  en_sn, en_ts),
            rohc_csc_rtp_t       *p_csc    _AND_
            rohc_encoded_value_t *en_sn    _AND_
            rohc_encoded_value_t *en_ts 
            )
{
 
    ROHC_ENTER_FN("rohc_cprm_p1_update_csc_rtp");
    p_csc->field_value[ROHC_SN_FID-ROHC_VAR_FID_BASE] = en_sn->value;;    
    p_csc->field_len[ROHC_SN_FID-ROHC_VAR_FID_BASE] = en_sn->bits;
    /* RTP static param */
    p_csc->field_value[ROHC_RTP_SSRC_FID-ROHC_VAR_FID_BASE] = 
            ROHC_NTOHL((((rohc_rtp_t*)p_csc->p_cpib->rtp)->ssrc));
    /* RTP Dyn param bitmask based updation */
    p_csc->field_value[ROHC_TS_FID-ROHC_VAR_FID_BASE] = en_ts->value;
    p_csc->field_len[ROHC_TS_FID-ROHC_VAR_FID_BASE] = en_ts->bits;
    
    p_csc->field_value[ROHC_RTP_PT_FID-ROHC_VAR_FID_BASE] =  
         ( ((rohc_rtp_t*)p_csc->p_cpib->rtp)->m_pt & ROHC_PT_MASK );
    
    p_csc->field_value[ROHC_RTP_X_FID-ROHC_VAR_FID_BASE] = 
          (((rohc_rtp_t*)p_csc->p_cpib->rtp)->v_p_x_cc & ROHC_X_MASK)>>4;
    p_csc->field_value[ROHC_RTP_M_FID-ROHC_VAR_FID_BASE] = 
         (((rohc_rtp_t*)p_csc->p_cpib->rtp)->m_pt & ROHC_M_MASK )>>7;
    p_csc->field_value[ROHC_RTP_R_P_FID-ROHC_VAR_FID_BASE] = 
        (((rohc_rtp_t*)p_csc->p_cpib->rtp)->v_p_x_cc & ROHC_PAD_MASK )>>5;
    p_csc->field_value[ROHC_RTP_CC_FID-ROHC_VAR_FID_BASE] = 
        (((rohc_rtp_t*)p_csc->p_cpib->rtp)->v_p_x_cc & ROHC_CC_MASK );
    
    p_csc->field_value[ROHC_RTP_TSS_FID-ROHC_VAR_FID_BASE] = 1;
    p_csc->field_value[ROHC_RTP_TS_STRIDE_FID-ROHC_VAR_FID_BASE] = 
                            p_csc->ts_stride;
    /* Fix for PR 6134 Start */  
    p_csc->field_value[ROHC_RTP_TIS_FID-ROHC_VAR_FID_BASE] = 0;
    /*p_csc->field_value[ROHC_RTP_TIME_STRIDE_FID-ROHC_VAR_FID_BASE] = 
                            p_csc->time_stride; */
    /* Fix for PR 6134 Ends*/
    ROHC_EXIT_FN("rohc_cprm_p1_update_csc_rtp");
}   
    
/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_p1_update_csc_udp
******************************************************************************
**
**      DESCRIPTION:
**              This funtion updates from CPIB to csc for udp 
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**              delta_field[IN]   Computed Delta field 
**              update_flag[IN]   Specifies whether to update the static 
**                                context or not.
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/
/* +- SPR 17777 */
rohc_void_t rohc_cprm_p1_update_csc_udp (
            rohc_csc_rtp_t *p_csc 
            )
{

    ROHC_ENTER_FN("rohc_cprm_p1_update_csc_udp");
    /* UDP static */
    
    p_csc->field_value[ROHC_UDP_SRC_FID-ROHC_VAR_FID_BASE] = 
                    ROHC_NTOHS(((rohc_udp_t*)p_csc->p_cpib->udp)->sport);
        
    p_csc->field_value[ROHC_UDP_DST_FID-ROHC_VAR_FID_BASE] = 
                    ROHC_NTOHS(((rohc_udp_t*)p_csc->p_cpib->udp)->dport);
    
    p_csc->field_value[ROHC_UDP_CKSUM_FID-ROHC_VAR_FID_BASE] = 
                    ROHC_NTOHS(((rohc_udp_t*)p_csc->p_cpib->udp)->cksum);
    
    ROHC_EXIT_FN("rohc_cprm_p1_update_csc_udp");
}

/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_p1_update_csc_inner_ip4
******************************************************************************
**
**      DESCRIPTION:
**              This funtion updates from CPIB to csc for inner_ip4 
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**              delta_field[IN]   Computed Delta field    
**              st_update_flag[IN]Specifies whether to update the static 
**                                context or not.
**
**    Static part:
**
**      Version, Protocol, Source Address, Destination Address.
**
**   Dynamic part:
**
**      Type of Service, Time to Live, Identification, DF, RND, NBO,
**      extension header list.
**
**   
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/
/* +- SPR 17777 */
rohc_void_t rohc_cprm_p1_update_csc_inner_ip4 ROHC_PARAMS(
            (p_csc, en_ip_id),
            rohc_csc_rtp_t        *p_csc    _AND_
            rohc_encoded_value_t  *en_ip_id 
            )
{
    ROHC_ENTER_FN("rohc_cprm_p1_update_csc_inner_ip4");
    /* Inner ipv4 static param */
    p_csc->field_value[ROHC_INNERIP4_SRC_FID-ROHC_VAR_FID_BASE] = 
                ROHC_NTOHL(((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->src);
                                    
    p_csc->field_value[ROHC_INNERIP4_DST_FID-ROHC_VAR_FID_BASE] = 
                ROHC_NTOHL(((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->dst); 
    
    /* Inner ipv4 dynamic param
     proto field is to be verified */
    p_csc->field_value[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE] = 
                            en_ip_id->value;
    p_csc->field_len[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE] = en_ip_id->bits;
    
    p_csc->field_value[ROHC_INNERIP4_TOS_FID-ROHC_VAR_FID_BASE] = 
                            ((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->tos;
    
    p_csc->field_value[ROHC_INNERIP4_TTL_FID-ROHC_VAR_FID_BASE] = 
                            ((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->ttl;
    
    p_csc->field_value[ROHC_INNERIP4_PROTO_FID-ROHC_VAR_FID_BASE] = 
                            ((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->proto;
    p_csc->field_value[ROHC_INNERIP_DF_FID-ROHC_VAR_FID_BASE ] = 
                (ROHC_NTOHS(((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->off) &
                 ROHC_IP4_DF)>>14;
                
    ROHC_EXIT_FN("rohc_cprm_p1_update_csc_inner_ip4");
}

/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_p1_update_csc_inner_ip6
******************************************************************************
**
**      DESCRIPTION:
**              This funtion updates from CPIB to csc for inner_ip6 
**          
**              Static part:Flow Label, Next Header, 
**                  Source Address , Destination Address  
**              Dynamic part:   Traffic class, Hop Limit,
**                  generic extension header list 
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**              delta_field[IN]   Computed Delta field 
**              st_update_flag[IN]Specifies whether to update the static 
**                                context or not.
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/
rohc_void_t rohc_cprm_p1_update_csc_inner_ip6 ROHC_PARAMS(
            (p_entity, p_csc),
            rohc_entity_t  *p_entity _AND_   
            /* + SPR 17439 */
            rohc_csc_udp_t *p_csc )
            /* - SPR 17439 */
            
{
    rohc_U32bit_t temp_32;
    ROHC_ENTER_FN("rohc_cprm_p1_update_csc_inner_ip6");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    
    ROHC_MEMCPY(p_csc->innerip6_src,
        ((rohc_ip6_t *)p_csc->p_cpib->inner_ip6)->src,16); 
    
    ROHC_MEMCPY(p_csc->innerip6_dst,
        ((rohc_ip6_t *)p_csc->p_cpib->inner_ip6)->dst,16); 
    
    temp_32 = ROHC_NTOHL(((rohc_ip6_t *)p_csc->p_cpib->inner_ip6)
                            ->ip6_ctlun.ip6_un1.un1_flow);
    p_csc->field_value[ROHC_INNERIP6_FLOWLABEL_FID-ROHC_VAR_FID_BASE ] = 
                                    (temp_32 & ROHC_IP6_FLOWLABEL_MASK);          
    p_csc->field_value[ROHC_INNERIP6_TCLASS_FID-ROHC_VAR_FID_BASE ] = 
        (temp_32 & ROHC_IP6_TRAFFICCLASS_MASK)>>20;
    
    p_csc->field_value[ROHC_INNERIP6_HLIM_FID-ROHC_VAR_FID_BASE ] = 
        ((rohc_ip6_t *)p_csc->p_cpib->inner_ip6)->ip6_ctlun.ip6_un1.un1_hlim;
    
    p_csc->field_value[ROHC_INNERIP6_NHEADER_FID-ROHC_VAR_FID_BASE] = 
         ((rohc_ip6_t *)p_csc->p_cpib->inner_ip6)->ip6_ctlun.ip6_un1.un1_nxt;
    
    ROHC_EXIT_FN("rohc_cprm_p1_update_csc_inner_ip6");
}

/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_p1_update_csc_outer_ip4
******************************************************************************
**
**      DESCRIPTION:
**              This funtion updates from CPIB to csc for rtp 
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**              delta_field[IN]   Computed Delta field 
**              st_update_flag[IN]Specifies whether to update the static 
**                                context or not.
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/
/* +- SPR 17777 */
rohc_void_t rohc_cprm_p1_update_csc_outer_ip4(
            rohc_csc_rtp_t *p_csc 
            )
{
    ROHC_ENTER_FN("rohc_cprm_p1_update_csc_outer_ip4");
    /* outer ipv4 static param */
    
    p_csc->field_value[ROHC_OUTERIP4_SRC_FID-ROHC_VAR_FID_BASE] = 
          ROHC_NTOHL(((rohc_ip4_t *)p_csc->p_cpib->outer_ip4)->src);
                                    
    p_csc->field_value[ROHC_OUTERIP4_DST_FID-ROHC_VAR_FID_BASE] = 
          ROHC_NTOHL(((rohc_ip4_t *)p_csc->p_cpib->outer_ip4)->dst);  
    
    /* outer ipv4 dynamic param
     proto field is to be verified */
    p_csc->field_value[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE] = 
                            p_csc->p_cpib->temp_ipid;
	/*((rohc_ip4_t *)p_csc->p_cpib->outer_ip4)->id;*/
    p_csc->field_len[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE] = 16;
        
    
    p_csc->field_value[ROHC_OUTERIP4_TOS_FID-ROHC_VAR_FID_BASE] = 
                            ((rohc_ip4_t *)p_csc->p_cpib->outer_ip4)->tos;
    p_csc->field_value[ROHC_OUTERIP4_TTL_FID-ROHC_VAR_FID_BASE] = 
                            ((rohc_ip4_t *)p_csc->p_cpib->outer_ip4)->ttl;
    p_csc->field_value[ROHC_OUTERIP4_PROTO_FID-ROHC_VAR_FID_BASE] = 
                            ((rohc_ip4_t *)p_csc->p_cpib->outer_ip4)->proto;
    p_csc->field_value[ROHC_OUTERIP_DF_FID-ROHC_VAR_FID_BASE ] = 
                (ROHC_NTOHS(((rohc_ip4_t *)p_csc->p_cpib->outer_ip4)->off) & 
                    ROHC_IP4_DF)>>14;
    ROHC_EXIT_FN("rohc_cprm_p1_update_csc_outer_ip4");
}

/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_p1_update_csc_outer_ip6
******************************************************************************
**
**      DESCRIPTION:
**              This funtion updates from CPIB to csc for outer ip_6
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**              delta_field[IN]   Computed Delta field 
**              st_update_flag[IN]Specifies whether to update the static 
**                                context or not.
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/
/* +- SPR 17777 */
rohc_void_t rohc_cprm_p1_update_csc_outer_ip6(rohc_csc_rtp_t *p_csc )
            
{
    rohc_U32bit_t temp_32;
    ROHC_ENTER_FN("rohc_cprm_p1_update_csc_outer_ip6");
    ROHC_MEMCPY(p_csc->outerip6_src,
        ((rohc_ip6_t*)p_csc->p_cpib->outer_ip6)->src,16);
        
    ROHC_MEMCPY(p_csc->outerip6_dst,
        ((rohc_ip6_t*)p_csc->p_cpib->outer_ip6)->dst,16);
    temp_32 = ROHC_NTOHL(((rohc_ip6_t *)p_csc->p_cpib->outer_ip6)->
                         ip6_ctlun.ip6_un1.un1_flow); 
    p_csc->field_value[ROHC_OUTERIP6_FLOWLABEL_FID-ROHC_VAR_FID_BASE ] = 
                    (temp_32 & ROHC_IP6_FLOWLABEL_MASK);           
    p_csc->field_value[ROHC_OUTERIP6_TCLASS_FID-ROHC_VAR_FID_BASE ] = 
            (temp_32 & ROHC_IP6_TRAFFICCLASS_MASK)>>20;
    
    p_csc->field_value[ROHC_OUTERIP6_HLIM_FID-ROHC_VAR_FID_BASE] = 
           ((rohc_ip6_t *)p_csc->p_cpib->outer_ip6)->ip6_ctlun.ip6_un1.un1_hlim;
    
    p_csc->field_value[ROHC_OUTERIP6_NHEADER_FID-ROHC_VAR_FID_BASE] = 
           ((rohc_ip6_t *)p_csc->p_cpib->outer_ip6)->ip6_ctlun.ip6_un1.un1_nxt;
    
    ROHC_EXIT_FN("rohc_cprm_p1_update_csc_outer_ip6");
}
