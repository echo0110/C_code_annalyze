/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_cprm_update_p23.c,v 1.1.1.1.16.1 2010/11/03 07:06:19 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the function definition for updating
 *                     the context (CSC) for profile p2(IP/UDP) and p3(IP/ESP)
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_cprm_update_p23.c,v $
 * Revision 1.1.1.1.16.1  2010/11/03 07:06:19  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.3.36.2  2010/10/13 03:58:12  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.3.36.1  2010/09/23 05:20:49  gur22059
 * Reduction in the size of context block w.r.t profile2
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
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/



/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_update_csc_p2_udp
******************************************************************************
**
**      DESCRIPTION:
**              This funtion updates from CPIB to csc for udp 
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**              
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/
rohc_void_t rohc_cprm_update_csc_p2_udp ROHC_PARAMS(
            (p_entity, p_csc, encoded_sn ),
            rohc_entity_t *p_entity _AND_   
            rohc_csc_udp_t *p_csc _AND_
            rohc_encoded_value_t *encoded_sn )
{
    ROHC_ENTER_FN("rohc_cprm_update_csc_p2_udp");
    /* generated SN */
    p_csc->field_value[ROHC_SN_FID-ROHC_VAR_FID_BASE] = encoded_sn->value;
    p_csc->field_len[ROHC_SN_FID-ROHC_VAR_FID_BASE] = encoded_sn->bits;
    /* UDP static */
    p_csc->field_value[ROHC_UDP_SRC_FID-ROHC_VAR_FID_BASE] = 
                      ROHC_NTOHS(((rohc_udp_t *)p_csc->p_cpib->udp)->sport);
    p_csc->field_value[ROHC_UDP_DST_FID-ROHC_VAR_FID_BASE] = 
                      ROHC_NTOHS(((rohc_udp_t *)p_csc->p_cpib->udp)->dport);
    /* UDP dyn */ 
    p_csc->field_value[ROHC_UDP_CKSUM_FID-ROHC_VAR_FID_BASE] = 
                      ROHC_NTOHS(((rohc_udp_t *)p_csc->p_cpib->udp)->cksum);
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_cprm_update_csc_p2_udp");
}

/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_update_csc_p2_inner_ip4
******************************************************************************
**
**      DESCRIPTION:
**              This funtion updates from CPIB to csc for inner_ip4 
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**              
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/
rohc_void_t rohc_cprm_update_csc_p2_inner_ip4 ROHC_PARAMS(
            (p_entity, p_csc,  en_ip_id),
            rohc_entity_t *p_entity _AND_   
            rohc_csc_udp_t *p_csc _AND_
            rohc_encoded_value_t *en_ip_id 
            )
{
    ROHC_ENTER_FN("rohc_cprm_update_csc_p2_inner_ip4");
    /* Inner ipv4 static param */
    p_csc->field_value[ROHC_INNERIP4_SRC_FID-ROHC_VAR_FID_BASE] = 
               ROHC_NTOHL(((rohc_ip4_t *)p_csc->p_cpib->inner_ip4)->src);
    p_csc->field_value[ROHC_INNERIP4_DST_FID-ROHC_VAR_FID_BASE] = 
              ROHC_NTOHL(((rohc_ip4_t *)p_csc->p_cpib->inner_ip4)->dst);  
    /* Inner ipv4 dynamic param
     proto field is to be verified */
    p_csc->field_value[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE] = 
                            en_ip_id->value;
    p_csc->field_len[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE] = 
                        en_ip_id->bits;
    p_csc->field_value[ROHC_INNERIP4_TOS_FID-ROHC_VAR_FID_BASE] = 
               ((rohc_ip4_t *)p_csc->p_cpib->inner_ip4)->tos;
    p_csc->field_value[ROHC_INNERIP4_TTL_FID-ROHC_VAR_FID_BASE] = 
               ((rohc_ip4_t *)p_csc->p_cpib->inner_ip4)->ttl;
    p_csc->field_value[ROHC_INNERIP4_PROTO_FID-ROHC_VAR_FID_BASE] = 
               ((rohc_ip4_t *)p_csc->p_cpib->inner_ip4)->proto;
    p_csc->field_value[ROHC_INNERIP_DF_FID-ROHC_VAR_FID_BASE ] = 
                (ROHC_NTOHS(((rohc_ip4_t *)p_csc->p_cpib->inner_ip4)->off) & 
                 ROHC_IP4_DF)>>14;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_cprm_update_csc_p2_inner_ip4");
}

/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_update_csc_p2_inner_ip6
******************************************************************************
**
**      DESCRIPTION:
**              This funtion updates from CPIB to csc for inner_ip6 
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**              
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/
rohc_void_t rohc_cprm_update_csc_p2_inner_ip6 ROHC_PARAMS(
            (p_entity, p_csc),
            rohc_entity_t *p_entity _AND_   
            rohc_csc_udp_t *p_csc )
{
        ROHC_ENTER_FN("rohc_cprm_update_csc_p2_inner_ip6");
        rohc_cprm_p1_update_csc_inner_ip6 
            (p_entity, p_csc);
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
        ROHC_EXIT_FN("rohc_cprm_update_csc_p2_inner_ip6");
            
}

/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_update_csc_p2_outer_ip4
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
rohc_void_t rohc_cprm_update_csc_p2_outer_ip4 ROHC_PARAMS(
            (p_entity, p_csc, encoded_ip_id),
            rohc_entity_t *p_entity _AND_   
            rohc_csc_udp_t *p_csc _AND_
            rohc_encoded_value_t * encoded_ip_id 
            )
{
    ROHC_ENTER_FN("rohc_cprm_update_csc_p2_outer_ip4");
    /* outer ipv4 static param */
    p_csc->field_value[ROHC_OUTERIP4_SRC_FID-ROHC_VAR_FID_BASE] = 
                ROHC_NTOHL(((rohc_ip4_t *)p_csc->p_cpib->outer_ip4)->src);
    p_csc->field_value[ROHC_OUTERIP4_DST_FID-ROHC_VAR_FID_BASE] = 
                ROHC_NTOHL(((rohc_ip4_t *)p_csc->p_cpib->outer_ip4)->dst);  
    /* outer ipv4 dynamic param
     proto field is to be verified */
    p_csc->field_value[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE] = 
                            encoded_ip_id->value;
    p_csc->field_len[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE] = 
                        encoded_ip_id->bits;
    p_csc->field_value[ROHC_OUTERIP4_TOS_FID-ROHC_VAR_FID_BASE] = 
                            ((rohc_ip4_t *)p_csc->p_cpib->outer_ip4)->tos;
    p_csc->field_value[ROHC_OUTERIP4_TTL_FID-ROHC_VAR_FID_BASE] = 
                            ((rohc_ip4_t *)p_csc->p_cpib->outer_ip4)->ttl;
    p_csc->field_value[ROHC_OUTERIP4_PROTO_FID-ROHC_VAR_FID_BASE] = 
                            ((rohc_ip4_t *)p_csc->p_cpib->outer_ip4)->proto;
    p_csc->field_value[ROHC_OUTERIP_DF_FID-ROHC_VAR_FID_BASE ] = 
                (ROHC_NTOHS(((rohc_ip4_t *)p_csc->p_cpib->outer_ip4)->off) & 
                 ROHC_IP4_DF)>>14;
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
                
    ROHC_EXIT_FN("rohc_cprm_update_csc_p2_outer_ip4");
}


/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_update_csc_p2_outer_ip6
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
rohc_void_t rohc_cprm_update_csc_p2_outer_ip6 ROHC_PARAMS(
            (p_entity, p_csc ),
            rohc_entity_t *p_entity _AND_   
            rohc_csc_udp_t *p_csc )
{
    ROHC_ENTER_FN("rohc_cprm_update_csc_p2_outer_ip6");
    /* + SPR 17439 */
    /* +- SPR 17777 */
    rohc_cprm_p1_update_csc_outer_ip6
            ((rohc_csc_rtp_t*)p_csc);
    /* - SPR 17439 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_cprm_update_csc_p2_outer_ip6");
}
