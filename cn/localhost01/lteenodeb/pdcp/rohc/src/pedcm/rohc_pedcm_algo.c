/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_pedcm_algo.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the implementation for
 *                     Algorithm-1 : Repair of incorrect SN updates 
 *                                            and
 *                     Algorithm-2 : Correction of SN LSB wraparound.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_pedcm_algo.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/28 03:39:33  gur19836
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
#include "rohc_pedcm_proto.h"

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
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/



/*****************************************************************************
**      FUNCTION:
**              rohc_pedcm_incor_sn_update_algo
******************************************************************************
**
**      DESCRIPTION:
**          This function provides the mechanism that allows the decompressor
**          to judge if the context was updated incorrectly by an earlier 
**          packet and, if so, to attempt a repair.
**
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**      RETURN VALUE:
**              rohc_return_t (ROHC_SUCCESS/ROHC_FAILURE)
**      NOTES:
**               
*****************************************************************************/
rohc_return_t rohc_pedcm_incor_sn_update_algo ROHC_PARAMS(
            (p_entity, p_dsc ),
            rohc_entity_t *p_entity _AND_   
            rohc_dsc_t *p_dsc)
{
    rohc_U32bit_t sn_new = 0;   

    ROHC_ENTER_FN("rohc_pedcm_incor_sn_update_algo");
    /* generate new sn using the previous sn_ref value)
       p_dsc->pre_to_pre_sn;
       sn_new = rohc_edm_decode_sn();  */              
    if (sn_new != p_dsc->dpib->decoded_sn)
    {

        p_dsc->dpib->decoded_sn = sn_new;
        /* call the pbm to create the new IP packet with the  */
        /* new decoded SN   */
        /* call CRC moudle for CRC validation */
        /* SPR 4808 Fix Start */
        /* SPR 4808 Fix End */
    }
    else
    {
        p_dsc->incor_update_sn_succeed_count = 0;
    }
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,
            "PdcpRxStateMachine");
    ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
            "ALGO - Repair of incorrect SN updates "
            "failed to correct the error\n",
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc))
        ;

    ROHC_EXIT_FN("rohc_pedcm_incor_sn_update_algo");

    return ROHC_FAILURE;
}


/*****************************************************************************
**      FUNCTION:
**              rohc_pedcm_sn_lsb_algo
******************************************************************************
**
**      DESCRIPTION:
**              This function provides the mechanism to detect the situation
**              when the SN LSBs being interpreted  wrongly because the
**              interpretation interval has not moved for lack of input and 
**              to avoid the context damage by using local clock.
**   
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**      RETURN VALUE:
**              rohc_void_t (ROHC_SUCCESS/ROHC_FAILURE)
**      NOTES:
**               
*************************************************************************** */
rohc_return_t rohc_pedcm_sn_lsb_algo ROHC_PARAMS(
                        (p_entity, p_dsc ),
                            rohc_entity_t *p_entity _AND_   
                            rohc_dsc_t *p_dsc)
{
    /* arrival time of previous successfully  
       decompressed packet and arrival time of current 
       packet are fetched from p_dsc */

    rohc_U32bit_t tmp_avg_time = 0;
    rohc_U32bit_t ref_sn = 0; 
    rohc_U32bit_t interval = 0;
    rohc_U32bit_t tmp_add_sn =0;

    ROHC_ENTER_FN("rohc_pedcm_sn_lsb_algo");
    /* SPR 3532 - klocowrk fix */
    ref_sn = p_dsc->dpib->field_val[ROHC_SN_FID - ROHC_VAR_FID_BASE]; 
    interval = (p_dsc->arv_tm_cur_pkt - 
            p_dsc->arv_tm_pre_pkt );
    /* SPR 3532 - klocowrk fix */
    tmp_add_sn = ( 1 << (p_dsc->dpib->field_len[ROHC_SN_FID - ROHC_VAR_FID_BASE]) );         
    if (p_dsc->profile == ROHC_IP_UDP_RTP)
    {
        tmp_avg_time = p_dsc->tss;  
    }
    else
    {
        tmp_avg_time = p_dsc->avg_inter_pkt_arv_tm;
    }

    if(interval == tmp_add_sn * tmp_avg_time )
    {
        ref_sn += tmp_add_sn;
        /* call the pbm to create the new IP packet with the  */
        /* new SN       */
        /* call CRC moudle for CRC validation */
        /* SPR 4808 Fix Start */
        /* SPR 4808 Fix End */
    }
    else
    {
        p_dsc->sn_lsb_succeed_count = 0;
    }
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,
            "PdcpRxStateMachine");

    ROHC_LOG(LOG_INFO,"CCPM:EID[%u:%u]:CID[%u]:"
            "ALGO - Correction of SN LSB wraparound "
            "failed to correct the error.\n",
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc));

    ROHC_EXIT_FN("rohc_pedcm_sn_lsb_algo");
    return ROHC_FAILURE;
}
