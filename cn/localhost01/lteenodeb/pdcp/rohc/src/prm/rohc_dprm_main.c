/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_dprm_main.c,v 1.5.30.9 2010/12/02 08:58:02 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the functions to initialize the DPRM
 *                     function-pointer table. This also provides the functionality
 *                     to update the DSC and, to create and sent the clock resolution,
 *                     jitter and mode transition feedback packets to compressor.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_dprm_main.c,v $
 * Revision 1.5.30.9  2010/12/02 08:58:02  gur22059
 * Conflicting profile 1 functions were renamed acc to new Naming Convention
 *
 * Revision 1.5.30.8  2010/11/24 03:45:15  gur22059
 * Removal of conditional checks w.r.t profile1
 *
 * Revision 1.5.30.7  2010/11/09 05:25:56  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.5.30.6  2010/10/13 03:59:28  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.5.30.5  2010/10/01 14:08:08  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.5.30.4  2010/07/08 09:28:21  gur21145
 * code review comments for prof0,2 enhancement activity
 *
 * Revision 1.5  2009/05/28 13:51:25  gur19140
 * *** empty log message ***
 *
 * Revision 1.4  2009/05/28 04:26:50  gur19836
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
#include "rohc_dprm_proto.h"
#include "rohc_pbm_ext_if.h"
#include "rohc_pedcm_ext_if.h"
#include "rohc_stats_def.h"
#include "rohc_opt_proto.h"
#include "rohc_db_ext_if.h"
/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static rohc_return_t rohcDecodeP6SeqNumber( 
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode ,
                rohc_U32bit_t      interpVal);

static rohc_return_t rohcDecodeP6AckNumber( 
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode ,
                rohc_U32bit_t      interpVal);
static rohc_return_t rohcDecodeP6TtlHopl( 
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode );
static rohc_return_t rohcDecodeP6MSeqNumber( 
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode );
static rohc_return_t rohcDecodeP6Window( 
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode );
static rohc_return_t rohcDecodeSeqNumScaled( 
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode );
static rohc_return_t rohcDecodeP6IpId( 
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode );
static rohc_return_t rohcDecodeP6TcpOptions( 
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode );
static rohc_return_t rohcDecodeP6AckNumberScaled( 
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode );
static rohc_return_t rohcDecodeIrCrPkt (
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode );
rohc_return_t rohcDecodeP6SackBlock (
                rohc_dsc_tcp_t          *p_dsc,
                rohc_encoded_value_t    *p_en_val,
                rohc_U32bit_t           *p_dec_ts,
                rohc_error_code_t       *p_ecode,
                rohc_U32bit_t           *pBaseVal );

rohc_return_t rohcDecodeP6TSEchoVal (
                rohc_entity_t           *p_rohc_entity,
                rohc_dsc_tcp_t          *p_dsc,
                rohc_encoded_value_t    *p_en_val,
                rohc_U32bit_t           *p_dec_ts,
                rohc_error_code_t       *p_ecode );

rohc_return_t rohcDecodeP6SNScaled (
                rohc_entity_t           *p_rohc_entity,
                rohc_dsc_tcp_t          *p_dsc,
                rohc_encoded_value_t    *p_en_val,
                rohc_U32bit_t           *p_dec_sn,
                rohc_error_code_t       *p_ecode );

rohc_return_t rohcDecodeP6AckNumScaled (
                rohc_entity_t           *p_rohc_entity,
                rohc_dsc_tcp_t          *p_dsc,
                rohc_encoded_value_t    *p_en_val,
                rohc_U32bit_t           *p_dec_ack_num,
                rohc_error_code_t       *p_ecode );

STATIC rohc_void_t rohcDecodeP6CoCommonSeqNoAckNo(
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_entity_t     *p_entity ,
                rohc_error_code_t *p_ecode
                );


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
/* assumed ROHC_MODE_U is minimum */
#ifdef ROHC_MODE_TRIGGER
static const rohc_U8bit_t 
rohc_dprm_mt_matix[ROHC_MODE_R - ROHC_MODE_U +1 ][ROHC_MODE_R - ROHC_MODE_U +1] =
{
    {ROHC_MT_NOT_REQ,       ROHC_D_U_MT_U_TO_O_ET,  ROHC_D_U_MT_U_TO_R_ET},
    {ROHC_D_O_MT_O_TO_U_ET, ROHC_MT_NOT_REQ,        ROHC_D_O_MT_O_TO_R_ET},
    {ROHC_D_R_MT_R_TO_U_ET, ROHC_D_R_MT_R_TO_U_ET,  ROHC_MT_NOT_REQ}
};

static const rohc_U8bit_t
rohc_d_mt_stats_matrix[ROHC_MODE_R - ROHC_MODE_U +1 ][ROHC_MODE_R - ROHC_MODE_U +1] =
{
    {0, ROHC_NO_OF_MODE_TRANS_U_TO_O_INIT,  ROHC_NO_OF_MODE_TRANS_U_TO_R_INIT},
    {ROHC_NO_OF_MODE_TRANS_O_TO_U_INIT,0,   ROHC_NO_OF_MODE_TRANS_O_TO_R_INIT},
    {ROHC_NO_OF_MODE_TRANS_R_TO_U_INIT,ROHC_NO_OF_MODE_TRANS_R_TO_O_INIT,0}
};  
#endif

/*Coverity Fix 32165 start*/
#ifdef ROHC_UT_FLAG
static const rohc_U8bit_t rohc_dprm_mode[3]={"UOR"};
#endif
/*Coverity Fix 32165 end*/



/*****************************************************************************
**      FUNCTION:
**              rohc_dprm_resolve_pkt_typ
******************************************************************************
**
**      DESCRIPTION:
**          Initializes the function PRM function pointer table.
**      ARGUMENTS:

**      RETURN VALUE:
**              Rohc packet type
**      NOTES:
**               
*****************************************************************************/
rohc_void_t rohc_dprm_init ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_dprm_init");
    rohc_dprm_init_fpt();
    ROHC_EXIT_FN("rohc_dprm_init");
}

/*****************************************************************************
**      FUNCTION: rohc_dprm_p1_exec
******************************************************************************
**
**      DESCRIPTION:
**              This funciton invokes the encoding and decoding module, then
**              verifies the CRC and then update the DSC
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**      RETURN VALUE:
**              rohc_return_t
**               
*****************************************************************************/

rohc_return_t rohc_dprm_p1_exec ROHC_PARAMS(
                    (p_entity, p_dsc, p_in_buff,p_len, p_ecode),
                    rohc_entity_t       *p_entity   _AND_
                    rohc_dsc_rtp_t      *p_dsc      _AND_
                    rohc_U8bit_t        *p_in_buff  _AND_
                    rohc_U32bit_t       *p_len      _AND_
                    rohc_error_code_t   *p_ecode)
{
    rohc_dprm_update_blk_t dprm_update_blk;
    register rohc_U8bit_t rcv_pkt_typ;
    ROHC_ENTER_FN("rohc_dprm_p1_exec");
    ROHC_MEMSET(&dprm_update_blk,0,sizeof(rohc_dprm_update_blk_t));
    dprm_update_blk.inner_ip4 = p_dsc->inner_ip4;
    dprm_update_blk.inner_ip6 = p_dsc->inner_ip6;
    dprm_update_blk.outer_ip4 = p_dsc->outer_ip4;
    dprm_update_blk.outer_ip6 = p_dsc->outer_ip6;
    dprm_update_blk.rtp = p_dsc->rtp;
    dprm_update_blk.udp = p_dsc->udp;
    p_dsc->decom_res = ROHC_FAILURE; 
    /* It seems this switch-case on profile basis is not required, so for now moving the
     * code for profile ROHC_IP_UDP_RTP out of the switch-case.*/
#ifdef ROHC_FUNC_ENHANCEMENT
    switch(p_dsc->profile)
    {
      case ROHC_UNCOMP: 
             ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"\
                                    "Uncompressed profile"\
                                    " nothing needs to be updated in dsc\n",
                                    p_entity->rohc_entity_id.ueIndex,
                                    p_entity->rohc_entity_id.lcId,
                                    ROHC_GET_DSC_CID(p_dsc));
             p_dsc->decom_res = ROHC_SUCCESS;
            break;
                                 
      case ROHC_IP_UDP_RTP:
            break;                          

        default: 
            *p_ecode = ROHC_PROFILE_NOT_SUPPORTED;
            ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"\
                    "Received profile is not supported\n", 
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,
                    ROHC_GET_DSC_CID(p_dsc));
            return ROHC_FAILURE;                                    
    }
#endif
            ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"\
                                    " DSC is to be updated "\
                                    " for profile ROHC_IP_UDP_RTP"\
                                    " and packet type[%u]\n",
                                    p_entity->rohc_entity_id.ueIndex,
                                    p_entity->rohc_entity_id.lcId,
                                    ROHC_GET_DSC_CID(p_dsc),
                                    p_dsc->dpib->rcv_pkt_typ);
            rcv_pkt_typ = p_dsc->dpib->rcv_pkt_typ;

            if (rcv_pkt_typ < ROHC_IR_WITHOUT_DYN_PKTTYPE)

            {           
                if(ROHC_FAILURE==rohc_dprm_p1_type_decode(p_entity,p_dsc,p_ecode))
                {
                    return  ROHC_FAILURE;
                }   
            }
            if(ROHC_SUCCESS ==
               rohc_dprm_p1_is_updation_req(p_entity, p_dsc, p_in_buff,p_len,
                                         p_ecode))
            {
                if(rcv_pkt_typ >=ROHC_R0P1_PKTTYPE &&
                   rcv_pkt_typ < ROHC_MAX_PKT_TYPES)
                {
                    p_dsc->bit_mask|=ROHC_UPDATE_DSC_FOR_ED;
                    rohc_dprm_p1_type_fpt[rohc_dprm_p1_pkt_cat
                            [rcv_pkt_typ]](p_entity, p_dsc, 
                                                        &dprm_update_blk);
                    p_dsc->bit_mask&=(~ROHC_UPDATE_DSC_FOR_ED);
                    /* +- SPR 17777 */
                    if(ROHC_FAILURE == 
                       rohc_dled_p1_update_dsc(p_dsc, p_dsc->dpib))
                    {
                        ROHC_EXIT_FN("rohc_dprm_p1_exec");
                        return ROHC_FAILURE;
                    }
                    if (rcv_pkt_typ > ROHC_IR_WITHOUT_DYN_PKTTYPE)
                    {
                        
                        /*call pbm to create packet buffer */
                        if(ROHC_FAILURE==
                            rohc_d_make_p1_ip_pkt(p_in_buff,p_len,p_dsc,
                              &dprm_update_blk))
                            /* +- SPR 17777 */
                        {
                            /* +- SPR 17777 */
                            LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                                    __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                    ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
                            ROHC_EXIT_FN("rohc_dprm_p1_exec");
                            return ROHC_FAILURE;
                        }
                    }   
       
                    p_dsc->decom_res = ROHC_SUCCESS; 
                }
                else
                {
                    ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"\
                              "Reveived packet type[%d] is not supported for"\
                              "IP/UDP/RTP profile\n",
                              p_entity->rohc_entity_id.ueIndex,
                              p_entity->rohc_entity_id.lcId,
                              ROHC_GET_DSC_CID(p_dsc),
                              p_dsc->dpib->rcv_pkt_typ);
                }  
                 
            }
            else
            {
                return (*p_ecode != ROHC_CRC_CHECK_FAILED);
            }   
    /* SPR 9591 fix start */        
    ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"\
    /* SPR 9591 fix end */        
                                    "SN value in the DSC is %u\n", 
                                    p_entity->rohc_entity_id.ueIndex,
                                    p_entity->rohc_entity_id.lcId,
                                    ROHC_GET_DSC_CID(p_dsc), 
                                    p_dsc->sn_prev);
   
    ROHC_EXIT_FN("rohc_dprm_p1_exec");
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**              rohc_dprm_p2_exec
******************************************************************************
**
**      DESCRIPTION:
**              This funciton invokes the encoding and decoding module, then
**              verifies the CRC and then update the DSC
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**      RETURN VALUE:
**              rohc_return_t
**               
*****************************************************************************/

rohc_return_t rohc_dprm_p2_exec ROHC_PARAMS(
                    (p_entity, p_dsc, p_in_buff,p_len, p_ecode),
                    rohc_entity_t     *p_entity   _AND_
                    rohc_dsc_udp_t    *p_dsc      _AND_
                    rohc_U8bit_t      *p_in_buff  _AND_
                    rohc_U32bit_t     *p_len      _AND_
                    rohc_error_code_t *p_ecode)
{
    rohc_dprm_update_blk_t dprm_update_blk;
    register rohc_U8bit_t rcv_pkt_typ;
    ROHC_ENTER_FN("rohc_dprm_p2_exec");
    ROHC_MEMSET(&dprm_update_blk,0,sizeof(rohc_dprm_update_blk_t));
    dprm_update_blk.inner_ip4 = p_dsc->inner_ip4;
    dprm_update_blk.inner_ip6 = p_dsc->inner_ip6;
    dprm_update_blk.outer_ip4 = p_dsc->outer_ip4;
    dprm_update_blk.outer_ip6 = p_dsc->outer_ip6;
    dprm_update_blk.udp = p_dsc->udp;
    p_dsc->decom_res = ROHC_FAILURE; 

    /* It seems this switch-case on profile basis is not required, so for now moving the
     * code for profile ROHC_IP_UDP out of the switch-case.*/
#ifdef ROHC_FUNC_ENHANCEMENT
    switch(p_dsc->profile)
    {
      case ROHC_UNCOMP: 
             ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"\
                                    "Uncompressed profile"\
                                    " nothing needs to be updated in dsc\n",
                                    p_entity->rohc_entity_id.ueIndex,
                                    p_entity->rohc_entity_id.lcId,
                                    ROHC_GET_DSC_CID(p_dsc));
            p_dsc->decom_res = ROHC_SUCCESS; 
            break;
                                 
      case ROHC_IP_UDP:     
            break;
        default: 
            *p_ecode = ROHC_PROFILE_NOT_SUPPORTED;
            ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"\
                    "Received profile is not supported\n", 
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,
                    ROHC_GET_DSC_CID(p_dsc));
            return ROHC_FAILURE;                                    
    }
#endif
            ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"\
                                    " DSC is to be updated"\
                                    " for profile ROHC_IP_UDP"\
                                    " and packet type[%u]\n",
                                    p_entity->rohc_entity_id.ueIndex,
                                    p_entity->rohc_entity_id.lcId,
                                    ROHC_GET_DSC_CID(p_dsc),
                                    p_dsc->dpib->rcv_pkt_typ);
            rcv_pkt_typ = p_dsc->dpib->rcv_pkt_typ;
            /* call ed to decode SN, IPID, IPID2*/
            if (rcv_pkt_typ < ROHC_IR_WITHOUT_DYN_PKTTYPE)
            {           
                if(ROHC_FAILURE==rohc_dprm_p2_decode(p_entity,p_dsc,p_ecode))
                {
                    
                    return  ROHC_FAILURE;
                }
            }
            if(ROHC_SUCCESS ==
               rohc_dprm_p2_is_updation_req(p_entity, p_dsc,p_in_buff,p_len,
                                         p_ecode))
            {
                if(rcv_pkt_typ >=ROHC_R0P1_PKTTYPE &&
                rcv_pkt_typ < ROHC_MAX_PKT_TYPES)
                {
                    p_dsc->bit_mask|=ROHC_UPDATE_DSC_FOR_ED;
                    rohc_dprm_p2_fpt[rohc_dprm_p23_pkt_cat[
                                rcv_pkt_typ]](p_entity, p_dsc,
                                                           &dprm_update_blk);
                    /* +- SPR 17777 */
                    p_dsc->bit_mask&=(~ROHC_UPDATE_DSC_FOR_ED);
                    if(ROHC_FAILURE ==
                       rohc_dled_p2_update_dsc(p_dsc, p_dsc->dpib))
                    {       
                        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                                __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
                        /* +- SPR 17777 */
                        ROHC_EXIT_FN("rohc_dprm_p2_exec");
                        return ROHC_FAILURE;
                    }
                    if (rcv_pkt_typ > ROHC_IR_WITHOUT_DYN_PKTTYPE)
                    {
                        /*call pbm to create packet buffer */
                        if(ROHC_FAILURE==
                            rohc_d_make_p2_ip_pkt(p_in_buff,p_len,p_dsc,
                              &dprm_update_blk))
                            /* +- SPR 17777 */
                        {
                            ROHC_EXIT_FN("rohc_dprm_p2_exec");
                            return ROHC_FAILURE;
                        }
                    }   
                     p_dsc->decom_res = ROHC_SUCCESS; 
                }
                else
                {
                    ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                               "Reveived packet type[%d] is not supported for"
                               "IP/UDP profile\n",
                               p_entity->rohc_entity_id.ueIndex,
                               p_entity->rohc_entity_id.lcId,
                               ROHC_GET_DSC_CID(p_dsc),
                               p_dsc->dpib->rcv_pkt_typ);
                }         
                
            }else
            {

                return (*p_ecode != ROHC_CRC_CHECK_FAILED);
            }
    /* SPR 9591 fix start */        
    ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"\
    /* SPR 9591 fix end */        
                                    "SN value in the DSC is %u\n", 
                                    p_entity->rohc_entity_id.ueIndex,
                                    p_entity->rohc_entity_id.lcId,
                                    ROHC_GET_DSC_CID(p_dsc), 
                                    p_dsc->sn_prev);
   
    ROHC_EXIT_FN("rohc_dprm_p2_exec");
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**              rohcDprmP6Exec
******************************************************************************
**
**      DESCRIPTION:
**              This funciton invokes the encoding and decoding module, then
**              verifies the CRC and then update the DSC
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**              p_in_buff [IN]    Pointer to the final out going buffer
**              p_len   [IN]      Pointer to the packet length
**              p_ecode [IN]      Pointer to error code
**      RETURN VALUE:
**              rohc_return_t
**               
*****************************************************************************/

rohc_return_t rohcDprmP6Exec(
                    rohc_entity_t     *p_entity  ,
                    rohc_dsc_tcp_t    *p_dsc     ,
                    rohc_U8bit_t      *p_in_buff ,
                    rohc_U32bit_t     *p_len     ,
                    rohc_error_code_t *p_ecode)
{
    rohc_dprm_update_blk_t dprm_update_blk;
    register rohc_U8bit_t rcv_pkt_typ;
    ROHC_ENTER_FN("rohcDprmP6Exec");
    ROHC_MEMSET(&dprm_update_blk,0,sizeof(rohc_dprm_update_blk_t));
    dprm_update_blk.inner_ip4 = p_dsc->inner_ip4;
    dprm_update_blk.inner_ip6 = p_dsc->inner_ip6;
    dprm_update_blk.outer_ip4 = p_dsc->outer_ip4;
    dprm_update_blk.outer_ip6 = p_dsc->outer_ip6;
    dprm_update_blk.tcp = p_dsc->tcp;
    p_dsc->decom_res = ROHC_FAILURE; 

    /* It seems this switch-case on profile basis is not required, so for now
     * moving the code for profile ROHC_IP_UDP out of the switch-case.*/
    ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"\
            " DSC is to be updated"\
            " for profile ROHC_IP_UDP"\
            " and packet type[%u]\n",
            p_entity->rohc_entity_id.ueIndex,
            p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),
            p_dsc->dpib->rcv_pkt_typ);
    rcv_pkt_typ = p_dsc->dpib->rcv_pkt_typ;
    /* call ed to decode SN, IPID, IPID2*/
    if (ROHC_IRP6CR_PKTYPE >= rcv_pkt_typ)
    {           
        if(ROHC_FAILURE==rohcDprmP6Decode(p_entity,p_dsc,p_ecode))
        {

            return  ROHC_FAILURE;
        }

        /*updation req is used to find whether the Packet to be formed is
        correct or not based on CRC check,if SUCCESS is found context can
        be updated or the packet will be discarded without saving it
        to the decompressor context*/
        if(ROHC_SUCCESS ==
                rohcDprmP6IsUpdationReq(p_entity, p_dsc,p_in_buff,p_len,
                    p_ecode))
        {
            if(ROHC_CO_P6_COMMON_PKTYPE <=rcv_pkt_typ &&
                    ROHC_MAX_PKT_TYPES > rcv_pkt_typ)
            {
                p_dsc->bit_mask|=ROHC_UPDATE_DSC_FOR_ED;
                rohcDprmP6FptG[rohc_dprm_p6_pkt_cat_g[
                    rcv_pkt_typ]](p_entity, p_dsc,
                            &dprm_update_blk);
                p_dsc->bit_mask&=(~ROHC_UPDATE_DSC_FOR_ED);
                if (ROHC_IR_WITHOUT_DYN_PKTTYPE < rcv_pkt_typ)
                {
                    /*call pbm to create packet buffer */
                    /*Coverity Fix 42164 Start*/
                    /* +- SPR 17777 */
                    if(ROHC_FAILURE==
                            rohcDMakeP6IpPkt(p_in_buff,p_len,p_dsc,
                                &dprm_update_blk))
                        /* +- SPR 17777 */
                    {
                        /* +- SPR 17777 */
                        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                                __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                                ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
                        /* +- SPR 17777 */
                        ROHC_EXIT_FN("rohcDprmP6Exec");
                        return ROHC_FAILURE;
                    }
                    /*Coverity Fix 42164 Stop*/
                }   
                p_dsc->decom_res = ROHC_SUCCESS; 
            }
            else
            {
                ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                        "Reveived packet type[%d] is not supported for"
                        "IP/UDP profile\n",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,
                        ROHC_GET_DSC_CID(p_dsc),
                        p_dsc->dpib->rcv_pkt_typ);
            }         

        }else
        {

            return (*p_ecode != ROHC_CRC_CHECK_FAILED);
        }   
    }
    /* SPR 9591 fix start */
    ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"\
    /* SPR 9591 fix end */
            "SN value in the DSC is %u\n", 
            p_entity->rohc_entity_id.ueIndex,
            p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc), 
            p_dsc->sn_prev);

    ROHC_EXIT_FN("rohcDprmP6Exec");
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**              rohc_dprm_p1_is_updation_req
******************************************************************************
**
**      DESCRIPTION:
**              This function determines whether the updation form DPIB to 
**              DSC is required or not. 
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/
 
rohc_U8bit_t rohc_dprm_p1_is_updation_req 
            ROHC_PARAMS(
            (p_entity, p_dsc, p_in_buff,p_len,p_ecode) ,
            rohc_entity_t       * p_entity  _AND_
            rohc_dsc_rtp_t      *p_dsc      _AND_
            rohc_U8bit_t        *p_in_buff  _AND_
            rohc_U32bit_t       *p_len      _AND_
            rohc_error_code_t   *p_ecode)
{
    rohc_U8bit_t crc_res = ROHC_FALSE;
    rohc_U8bit_t update_flag = ROHC_TRUE;
    //rohc_U8bit_t pedcm_res = ROHC_FALSE;
    rohc_dprm_update_blk_t dprm_update_blk;

    register rohc_U8bit_t rcv_pkt_typ;
    rcv_pkt_typ = p_dsc->dpib->rcv_pkt_typ;

    ROHC_ENTER_FN("rohc_dprm_p1_is_updation_req");
    ROHC_MEMSET(&dprm_update_blk,0,sizeof(rohc_dprm_update_blk_t));

    if (rcv_pkt_typ < ROHC_IR_WITHOUT_DYN_PKTTYPE)
    {
         /*call pbm to create packet buffer */
        if(ROHC_FAILURE==
           rohc_d_make_p1_ip_pkt(p_in_buff,p_len,p_dsc,
                              &dprm_update_blk))
            /* +- SPR 17777 */
        {
            /* +- SPR 17777 */
            LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                    ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
            /* +- SPR 17777 */
            ROHC_EXIT_FN("rohc_dprm_p1_is_updation_req");
            return ROHC_FAILURE;
        }   
        /* update the in temp output buffer then apply the CRC check... */
       
        rohc_dprm_p1_type_fpt[rohc_dprm_p1_pkt_cat[rcv_pkt_typ]]
                        (p_entity, p_dsc,&dprm_update_blk);
        /* call CRC Module for CRC validation */
        if(rohc_dprm_pkt_crc_bit[rcv_pkt_typ])
        {
            ROHC_UT_TRACE((stderr," IP pkt len - 0x%x id=0x%x cksum=0x%x ttl=0x%x\n"
                        "udp len = 0x%x cksum=0x%x,rtp sn=0x%x TS=0x%x vXcc=0x%x PT = 0x%x\n"
                        ,dprm_update_blk.inner_ip4->len,dprm_update_blk.inner_ip4->id,
                        dprm_update_blk.inner_ip4->cksum,dprm_update_blk.inner_ip4->ttl,
                        dprm_update_blk.udp->len,dprm_update_blk.udp->cksum,dprm_update_blk.rtp->sn,
                        dprm_update_blk.rtp->ts,dprm_update_blk.rtp->v_p_x_cc,dprm_update_blk.rtp->m_pt));
            /*   crc_res = call CRC module */
            crc_res = rohc_d_crc_p1_compute_comp_pkt(p_entity, p_dsc,
                                &dprm_update_blk);
            /* +- SPR 17777 */
            if(!crc_res)
            {
                ROHC_UT_TRACE((stderr,"CRC check failed in updatingReq\n"));

                 /* Incrementing Stats */
                 pdcpStats.statsRohc.numberOfTimesCrcFailed++;

                    /* call PEDCM module for error recover */
            update_flag = ROHC_FALSE;
            *p_ecode = ROHC_CRC_CHECK_FAILED; 
            }
        }   
    }
    ROHC_EXIT_FN("rohc_dprm_p1_is_updation_req");
    return update_flag;
 }

/*****************************************************************************
**      FUNCTION:
**              rohc_dprm_p2_is_updation_req
******************************************************************************
**
**      DESCRIPTION:
**              This function determines whether the updation form DPIB to 
**              DSC is required or not. 
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/
 
rohc_U8bit_t rohc_dprm_p2_is_updation_req 
            ROHC_PARAMS(
            (p_entity, p_dsc, p_in_buff,p_len,p_ecode) ,
            rohc_entity_t     * p_entity  _AND_
            rohc_dsc_udp_t    *p_dsc      _AND_
            rohc_U8bit_t      *p_in_buff  _AND_
            rohc_U32bit_t     *p_len      _AND_
            rohc_error_code_t *p_ecode)
{
    rohc_U8bit_t crc_res = ROHC_FALSE;
    rohc_U8bit_t update_flag = ROHC_TRUE;
    rohc_dprm_update_blk_t dprm_update_blk;

    register rohc_U8bit_t rcv_pkt_typ;
    rcv_pkt_typ = p_dsc->dpib->rcv_pkt_typ;

    ROHC_ENTER_FN("rohc_dprm_p2_is_updation_req");
    ROHC_MEMSET(&dprm_update_blk,0,sizeof(rohc_dprm_update_blk_t));

    if (rcv_pkt_typ < ROHC_IR_WITHOUT_DYN_PKTTYPE)
    {
         /*call pbm to create packet buffer */
        if(ROHC_FAILURE==
           rohc_d_make_p2_ip_pkt(p_in_buff,p_len,p_dsc,
                              &dprm_update_blk))
            /* +- SPR 17777 */
        {
            LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                    ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
            ROHC_EXIT_FN("rohc_dprm_p2_is_updation_req");
            return ROHC_FAILURE;
        }   
        /* update the in temp output buffer then apply the CRC check... */
        rohc_dprm_p2_fpt[rohc_dprm_p23_pkt_cat[rcv_pkt_typ]]
                        (p_entity, p_dsc,&dprm_update_blk);

        /* call CRC Module for CRC validation */
        if(rohc_dprm_pkt_crc_bit[rcv_pkt_typ])
        {
            ROHC_UT_TRACE((stderr," IP pkt len - 0x%x id=0x%x cksum=0x%x ttl=0x%x\n"
                        "udp len = 0x%x cksum=0x%x\n"
                        ,dprm_update_blk.inner_ip4->len,dprm_update_blk.inner_ip4->id,
                        dprm_update_blk.inner_ip4->cksum,dprm_update_blk.inner_ip4->ttl,
                        dprm_update_blk.udp->len,dprm_update_blk.udp->cksum));
            /*   crc_res = call CRC module */
            crc_res = rohc_d_crc_p2_compute_comp_pkt(p_entity, p_dsc,
                                &dprm_update_blk);
            /* +- SPR 17777 */
            if(!crc_res)
            {
                ROHC_UT_TRACE((stderr,"CRC check failed in updatingReq\n"));
                /* Incrementing Stats */
                pdcpStats.statsRohc.numberOfTimesCrcFailed++;

                    /* call PEDCM module for error recover */
            update_flag = ROHC_FALSE;
            *p_ecode = ROHC_CRC_CHECK_FAILED; 
            }
        }   
    }
    ROHC_EXIT_FN("rohc_dprm_p2_is_updation_req");
    return update_flag;
 }

/*****************************************************************************
**      FUNCTION:
**              rohcDprmP6IsUpdationReq
******************************************************************************
**
**      DESCRIPTION:
**              This function determines whether the updation form DPIB to 
**              DSC is required or not. 
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/
 
rohc_U8bit_t rohcDprmP6IsUpdationReq(
            rohc_entity_t     * p_entity  ,
            rohc_dsc_tcp_t    *p_dsc      ,
            rohc_U8bit_t      *p_in_buff  ,
            rohc_U32bit_t     *p_len      ,
            rohc_error_code_t *p_ecode)
{
    rohc_U8bit_t crc_res = ROHC_FALSE;
    rohc_U8bit_t update_flag = ROHC_TRUE;
    rohc_dprm_update_blk_t dprm_update_blk;

    register rohc_U8bit_t rcv_pkt_typ;
    rcv_pkt_typ = p_dsc->dpib->rcv_pkt_typ;

    ROHC_ENTER_FN("rohcDprmP6IsUpdationReq");
    ROHC_MEMSET(&dprm_update_blk,0,sizeof(rohc_dprm_update_blk_t));

    if (ROHC_IR_WITHOUT_DYN_PKTTYPE > rcv_pkt_typ || ROHC_IRP6CR_PKTYPE 
        == rcv_pkt_typ)
    {
         /*call pbm to create packet buffer */
         /*COVERITY FIX for 40893 START*/
        /* +- SPR 17777 */
        if(ROHC_FAILURE==
           rohcDMakeP6IpPkt(p_in_buff,p_len,p_dsc,
                              &dprm_update_blk))
            /* +- SPR 17777 */
        {
            LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                    ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
            ROHC_EXIT_FN("rohcDprmP6IsUpdationReq");
            return ROHC_FAILURE;
        }   
         /*COVERITY FIX for 40893 Stops*/
        /* update the in temp output buffer then apply the CRC check... */
        rohcDprmP6FptG[rohc_dprm_p6_pkt_cat_g[rcv_pkt_typ]]
                        (p_entity, p_dsc,&dprm_update_blk);

        /* call CRC Module for CRC validation */
        if(rohc_dprm_pkt_crc_bit[rcv_pkt_typ] || rcv_pkt_typ == ROHC_IRP6CR_PKTYPE)
        {
            /*   crc_res = call CRC module */
            crc_res = rohc_d_crc_p6_compute_comp_pkt(p_entity, p_dsc,
                                &dprm_update_blk);
            /* +- SPR 17777 */
            if(!crc_res)
            {
                /* Incrementing Stats */
                pdcpStats.statsRohc.numberOfTimesCrcFailed++;

                    /* call PEDCM module for error recover */
            update_flag = ROHC_FALSE;
            *p_ecode = ROHC_CRC_CHECK_FAILED; 
            }
        }   
    }
    ROHC_EXIT_FN("rohcDprmP6IsUpdationReq");
    return update_flag;
 }
/*****************************************************************************
**      FUNCTION:
**              rohc_exec_dprm_jitter
******************************************************************************
**
**      DESCRIPTION:
**              This function create the feedback with Jitter option.
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**              jitter  [IN}      Jitter value
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/   
 rohc_return_t rohc_exec_dprm_jitter 
                 ROHC_PARAMS((p_entity, p_dsc, jitter,p_ecode ),
                 rohc_entity_t * p_entity _AND_
                 rohc_dsc_t *p_dsc _AND_
                rohc_U8bit_t jitter _AND_
                rohc_error_code_t   *p_ecode)
{
    rohc_feedback_t fb;
    ROHC_ENTER_FN("rohc_exec_dprm_jitter");
    fb.feedback_type = ROHC_FEEDBACK_TYPE2;          
    fb.sn = p_dsc->sn_prev;                     
    fb.ack_type = ROHC_ACK;               
    fb.mode = p_dsc->d_mode;    
    fb.cid =p_dsc->cid;
    fb.opt_type[0] = ROHC_FBO_JITTER;
    fb.opt_val[0] = jitter;
    fb.opt_type[1] = ROHC_FBO_CRC;
    fb.opt_val[1] = 0;
    fb.opt_type[2] = ROHC_INVALID_OPT_TYPE;

    /*call PBM to create feedback */
    if(ROHC_FAILURE == rohc_d_make_feedback_packet(p_entity,&fb,p_ecode))
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
        /* +- SPR 17777 */
        return ROHC_FAILURE;
    }   
    ROHC_EXIT_FN("rohc_exec_dprm_jitter");
    return ROHC_SUCCESS;
}
#ifdef ROHC_FUNC_ENHANCEMENT
/*****************************************************************************
**      FUNCTION:
**              rohc_exec_dprm_cl_resol
******************************************************************************
**
**      DESCRIPTION:
**              This function create the feedback with Clock-resolution option.
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**              jitter  [IN}      Clock-resolution value
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
rohc_return_t rohc_exec_dprm_cl_resol  
                ROHC_PARAMS((p_entity, p_dsc, clck_resol,p_ecode) ,
                rohc_entity_t * p_entity _AND_
                rohc_dsc_t *p_dsc _AND_
                rohc_U8bit_t clck_resol _AND_
                rohc_error_code_t *p_ecode)
{
    rohc_feedback_t fb;
    ROHC_ENTER_FN("rohc_exec_dprm_cl_resol");
    fb.feedback_type = ROHC_FEEDBACK_TYPE2;          
    fb.sn = p_dsc->sn_prev;                     
    fb.ack_type = ROHC_ACK;               
    fb.mode = p_dsc->d_mode;    
    fb.cid =p_dsc->cid;
    fb.opt_type[0] = ROHC_FBO_CLOCK;
    fb.opt_val[0] = clck_resol;
    fb.opt_type[1] = ROHC_FBO_CRC;
    fb.opt_val[1] = 0;
    fb.opt_type[2] = ROHC_INVALID_OPT_TYPE;
    /* call PBM to create feedback */
    if(ROHC_FAILURE ==rohc_d_make_feedback_packet(p_entity,&fb,p_ecode))
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
        /* +- SPR 17777 */
        return ROHC_FAILURE;
    }   
    ROHC_EXIT_FN("rohc_exec_dprm_cl_resol");
    return ROHC_SUCCESS;
    
}
#endif

/*****************************************************************************
**      FUNCTION:
**              rohc_exec_dprm_p1_md_transl
******************************************************************************
**
**      DESCRIPTION:
**              This function decides the mode transitioning.
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**              new_mode[IN}      New mode
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
#ifdef ROHC_MODE_TRIGGER
rohc_return_t rohc_exec_dprm_p1_md_transl 
                ROHC_PARAMS((p_entity, p_dsc , new_mode, p_ecode) ,
                rohc_entity_t       * p_entity  _AND_
                rohc_dsc_rtp_t      *p_dsc      _AND_
                rohc_U8bit_t        new_mode    _AND_
                rohc_error_code_t   *p_ecode)
{
    
    rohc_feedback_t fb;
    ROHC_ENTER_FN("rohc_exec_dprm_p1_md_transl");
    if(p_dsc->profile == ROHC_UNCOMP)
    {
        ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                          "For uncompressed profile mode "
                          " transition is not required.\n",
                          p_entity->rohc_entity_id.ueIndex,
                          p_entity->rohc_entity_id.lcId,
                          ROHC_GET_DSC_CID(p_dsc));
        return  ROHC_SUCCESS;
    }
    
    if(new_mode == p_dsc->d_mode)
    {
        
        ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                          "New mode and old mode are same.\n",
                          p_entity->rohc_entity_id.ueIndex,
                          p_entity->rohc_entity_id.lcId,
                          ROHC_GET_DSC_CID(p_dsc));
        
        return ROHC_FAILURE;
    }
    if(p_dsc->d_trans_state != ROHC_D_TST_DONE)
    {
        ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                          "Mode transition in not allowed as another"
                          " mode transition  is in progress\n",
                          p_entity->rohc_entity_id.ueIndex,
                          p_entity->rohc_entity_id.lcId,
                          ROHC_GET_DSC_CID(p_dsc));
        return ROHC_FAILURE;
    }
    
    fb.feedback_type = ROHC_FEEDBACK_TYPE2;          
    fb.sn = p_dsc->sn_prev;                     
    fb.ack_type = ROHC_ACK;               
    fb.mode = new_mode;                   
    fb.cid =p_dsc->cid;
    fb.opt_type[0] = ROHC_FBO_CRC;
    fb.opt_val[0] = 0;
    /* call PBM to create feedback */
    fb.opt_type[1] = ROHC_INVALID_OPT_TYPE;
    if(ROHC_FAILURE == rohc_d_make_feedback_packet(p_entity,&fb,p_ecode))
    {
        /* +- SPR 17777 */
        ROHC_EXIT_FN("rohc_exec_dprm_p1_md_transl");
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
        /* +- SPR 17777 */
        return ROHC_FAILURE;
    }   

/*Coverity Fix 32165 start*/
    ROHC_UT_TRACE(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"
                      "Initiating mode transition "\
                      "%c->%c\n",
                      p_entity->rohc_entity_id.ueIndex,
                      p_entity->rohc_entity_id.lcId,
                      ROHC_GET_DSC_CID(p_dsc),
                      rohc_dprm_mode[p_dsc->d_mode - ROHC_MODE_U],
                      rohc_dprm_mode[new_mode - ROHC_MODE_U]);

    /*Coverity Fix 32165 end */

    /* Incrementing Stats */
    rohcStatsModeTransUpdate(rohc_d_mt_stats_matrix[p_dsc->d_mode -ROHC_MODE_U]
                                        [new_mode-ROHC_MODE_U],ROHC_DIR_RX);

    rohc_dcpm_p1_exec_direct_evt(p_entity, p_dsc,
            rohc_dprm_mt_matix[p_dsc->d_mode -ROHC_MODE_U]
            [new_mode-ROHC_MODE_U], p_ecode);

    ROHC_EXIT_FN("rohc_exec_dprm_p1_md_transl");
      return ROHC_SUCCESS;
}
#endif

/*****************************************************************************
**      FUNCTION:
**              rohc_exec_dprm_p2_md_transl
******************************************************************************
**
**      DESCRIPTION:
**              This function decides the mode transitioning.
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**              new_mode[IN}      New mode
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
#ifdef ROHC_MODE_TRIGGER
rohc_return_t rohc_exec_dprm_p2_md_transl 
                ROHC_PARAMS((p_entity, p_dsc , new_mode, p_ecode) ,
                rohc_entity_t     * p_entity _AND_
                rohc_dsc_udp_t    *p_dsc     _AND_
                rohc_U8bit_t      new_mode   _AND_
                rohc_error_code_t *p_ecode)
{
    
    rohc_feedback_t fb;
    ROHC_ENTER_FN("rohc_exec_dprm_p2_md_transl");
    if(p_dsc->profile == ROHC_UNCOMP)
    {
        ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                                      "For uncompressed profile mode "
                                    " transition is not required.\n",
                                      p_entity->rohc_entity_id.ueIndex,
                                      p_entity->rohc_entity_id.lcId,
                                      ROHC_GET_DSC_CID(p_dsc));
        return  ROHC_SUCCESS;
    }
    if(new_mode == p_dsc->d_mode)
    {
        
        ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                          "New mode and old mode are same.\n",
                          p_entity->rohc_entity_id.ueIndex,
                          p_entity->rohc_entity_id.lcId,
                          ROHC_GET_DSC_CID(p_dsc));
        
        return ROHC_FAILURE;
    }
    if(p_dsc->d_trans_state != ROHC_D_TST_DONE)
    {
        ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                          "Mode transition in not allowed as another"
                          " mode transition  is in progress\n",
                          p_entity->rohc_entity_id.ueIndex,
                          p_entity->rohc_entity_id.lcId,
                          ROHC_GET_DSC_CID(p_dsc));

        return ROHC_FAILURE;
    }
    
    fb.feedback_type = ROHC_FEEDBACK_TYPE2;          
    fb.sn = p_dsc->sn_prev;                     
    fb.ack_type = ROHC_ACK;               
    fb.mode = new_mode;                   
    fb.cid =p_dsc->cid;
    fb.opt_type[0] = ROHC_FBO_CRC;
    fb.opt_val[0] = 0;
    /* call PBM to create feedback */
    fb.opt_type[1] = ROHC_INVALID_OPT_TYPE;
    if(ROHC_FAILURE == rohc_d_make_feedback_packet(p_entity,&fb,p_ecode))
    {
        ROHC_EXIT_FN("rohc_exec_dprm_p2_md_transl");
        return ROHC_FAILURE;
    }   

    /*Coverity Fix 32165 start*/
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_UT_TRACE(LOG_INFO,"CPRM:EID[%u:%u]:CID[%u]:"
                      "Initiating mode transition "\
                      "%c->%c\n", p_entity->rohc_entity_id.ueIndex,
                      p_entity->rohc_entity_id.lcId, ROHC_GET_DSC_CID(p_dsc),
                      rohc_dprm_mode[p_dsc->d_mode - ROHC_MODE_U],
                      rohc_dprm_mode[new_mode - ROHC_MODE_U]
                      );
    /*Coverity Fix 32165 end*/

    /* Incrementing Stats */
    rohcStatsModeTransUpdate(rohc_d_mt_stats_matrix[p_dsc->d_mode -ROHC_MODE_U]
                                        [new_mode-ROHC_MODE_U],ROHC_DIR_RX);

    rohc_dcpm_p2_exec_direct_evt(p_entity, p_dsc,
            rohc_dprm_mt_matix[p_dsc->d_mode -ROHC_MODE_U]
                              [new_mode-ROHC_MODE_U], p_ecode);

    ROHC_EXIT_FN("rohc_exec_dprm_p2_md_transl");
      return ROHC_SUCCESS;
}

#endif
/*****************************************************************************
**      FUNCTION:
**              rohc_dprm_p1_none_updating_pkt
******************************************************************************
**
**      DESCRIPTION:
**              This function for error handling(none updating pkt).
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**              new_mode[IN}      New mode
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
rohc_void_t rohc_dprm_p1_none_updating_pkt
         ROHC_PARAMS((p_entity, p_dsc, p_dprm_update_blk),
                    rohc_entity_t           *p_entity   _AND_
                    rohc_dsc_rtp_t          *p_dsc      _AND_
                    rohc_dprm_update_blk_t  *p_dprm_update_blk)
{
    ROHC_ENTER_FN("rohc_dprm_p1_none_updating_pkt");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),sizeof(p_dprm_update_blk),0,0, __func__,"");
            /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:PROFILE[%u]:"
                        "Received packet type[%u] is none updating packet\n",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,
                        ROHC_GET_DSC_CID(p_dsc),
                        p_dsc->profile,
                        p_dsc->dpib->rcv_pkt_typ);
    ROHC_EXIT_FN("rohc_dprm_p1_none_updating_pkt");
}

/*****************************************************************************
**      FUNCTION:
**              rohc_dprm_p2_none_updating_pkt
******************************************************************************
**
**      DESCRIPTION:
**              This function for error handling(none updating pkt).
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**              new_mode[IN}      New mode
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
rohc_void_t rohc_dprm_p2_none_updating_pkt
         ROHC_PARAMS((p_entity, p_dsc, p_dprm_update_blk),
                    rohc_entity_t *p_entity _AND_
                    rohc_dsc_udp_t *p_dsc _AND_
                    rohc_dprm_update_blk_t *p_dprm_update_blk)
{
    ROHC_ENTER_FN("rohc_dprm_p2_none_updating_pkt");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),sizeof(p_dprm_update_blk),0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:PROFILE[%u]:"
                        "Received packet type[%u] is none updating packet\n",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,
                        ROHC_GET_DSC_CID(p_dsc),
                        p_dsc->profile,
                        p_dsc->dpib->rcv_pkt_typ);
    ROHC_EXIT_FN("rohc_dprm_p2_none_updating_pkt");
}

/*****************************************************************************
**      FUNCTION:
**              rohc_dprm_p6_none_updating_pkt
******************************************************************************
**
**      DESCRIPTION:
**              This function for error handling(none updating pkt).
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**              new_mode[IN}      New mode
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
rohc_void_t rohc_dprm_p6_none_updating_pkt(
                    rohc_entity_t *p_entity ,
                    /* + SPR 17439 */
                    rohc_dsc_tcp_t *p_dsc ,
                    /* - SPR 17439 */
                    rohc_dprm_update_blk_t *p_dprm_update_blk)
{
    ROHC_ENTER_FN("rohc_dprm_p6_none_updating_pkt");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),sizeof(p_dprm_update_blk),0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:PROFILE[%u]:"
                        "Received packet type[%u] is none updating packet\n",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,
                        ROHC_GET_DSC_CID(p_dsc),
                        p_dsc->profile,
                        p_dsc->dpib->rcv_pkt_typ);
    ROHC_EXIT_FN("rohc_dprm_p6_none_updating_pkt");
}

/*****************************************************************************
**      FUNCTION:
**              rohc_dprm_p1_not_supported_pkt
******************************************************************************
**
**      DESCRIPTION:
**              This function for error handling(not supoorted pkt).
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**              new_mode[IN}      New mode
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
rohc_void_t rohc_dprm_p1_not_supported_pkt
         ROHC_PARAMS((p_entity, p_dsc,p_dprm_update_blk ),
                rohc_entity_t   *p_entity   _AND_
                rohc_dsc_rtp_t  *p_dsc      _AND_
                rohc_dprm_update_blk_t *p_dprm_update_blk)
{
    ROHC_ENTER_FN("rohc_dprm_p1_not_supported_pkt");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),sizeof(p_dprm_update_blk),0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:PROFILE[%u]:"
            "Received packet type[%d] is not a valid packet-type.\n",
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,
                    ROHC_GET_DSC_CID(p_dsc),
                    p_dsc->profile,
                    p_dsc->dpib->rcv_pkt_typ);
ROHC_EXIT_FN("rohc_dprm_p1_not_supported_pkt");
}                   

/*****************************************************************************
**      FUNCTION:
**              rohc_dprm_p2_not_supported_pkt
******************************************************************************
**
**      DESCRIPTION:
**              This function for error handling(not supoorted pkt).
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**              new_mode[IN}      New mode
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
rohc_void_t rohc_dprm_p2_not_supported_pkt
         ROHC_PARAMS((p_entity, p_dsc,p_dprm_update_blk ),
                rohc_entity_t *p_entity _AND_
                rohc_dsc_udp_t *p_dsc _AND_
                rohc_dprm_update_blk_t *p_dprm_update_blk)
{
    ROHC_ENTER_FN("rohc_dprm_p2_not_supported_pkt");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),sizeof(p_dprm_update_blk),0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:PROFILE[%u]:"
                    "Received packet type[%d] is not a valid packet-type.\n",
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,
                    ROHC_GET_DSC_CID(p_dsc),
                    p_dsc->profile,
                    p_dsc->dpib->rcv_pkt_typ);
ROHC_EXIT_FN("rohc_dprm_p2_not_supported_pkt");
}                   

/*****************************************************************************
**      FUNCTION:
**              rohc_dprm_p6_not_supported_pkt
******************************************************************************
**
**      DESCRIPTION:
**              This function for error handling(not supoorted pkt).
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**              new_mode[IN}      New mode
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
rohc_void_t rohc_dprm_p6_not_supported_pkt(
                rohc_entity_t *p_entity ,
                rohc_dsc_tcp_t *p_dsc ,
                rohc_dprm_update_blk_t *p_dprm_update_blk)
{
    ROHC_ENTER_FN("rohc_dprm_p6_not_supported_pkt");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),sizeof(p_dprm_update_blk),0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:PROFILE[%u]:"
            "Received packet type[%d] is not a valid packet-type.\n",
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,
                    ROHC_GET_DSC_CID(p_dsc),
                    p_dsc->profile,
                    p_dsc->dpib->rcv_pkt_typ);
ROHC_EXIT_FN("rohc_dprm_p6_not_supported_pkt");
}

/*****************************************************************************
**      FUNCTION:
**            rohc_dprm_p1_type_decode
******************************************************************************
**
**      DESCRIPTION:
**             Calls the decoding module for profile 1 
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**              new_mode[IN}      New mode
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
rohc_return_t rohc_dprm_p1_type_decode
         ROHC_PARAMS((p_entity, p_dsc,p_ecode),
                rohc_entity_t       *p_entity   _AND_
                rohc_dsc_rtp_t      *p_dsc      _AND_
                rohc_error_code_t   *p_ecode
                )
{
    rohc_encoded_value_t    en_tmp;
    /*Start SPR 5818*/
    rohc_U32bit_t sn_delta = 0;  /* holds difference of sn_prev and decoded_sn values*/
    /*End SPR 5818*/
    ROHC_ENTER_FN("rohc_dprm_p1_type_decode");
    en_tmp.value = p_dsc->dpib->field_val[ROHC_SN_FID-ROHC_VAR_FID_BASE];
    en_tmp.bits  =  p_dsc->dpib->field_len[ROHC_SN_FID-ROHC_VAR_FID_BASE];
    if(ROHC_FAILURE== rohc_decode_p1_sn(p_entity, p_dsc, &en_tmp, 
                                &p_dsc->dpib->decoded_sn,p_ecode))
    {   
        ROHC_EXIT_FN("rohc_dprm_p1_type_decode");
        return ROHC_FAILURE;
    }       
    /*Start SPR 5818*/
    sn_delta = p_dsc->dpib->decoded_sn - p_dsc->sn_prev;
    ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
            " SN [received bits(%u), received values(%u), "\
            "decoded values(%u)]\n",
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),
            en_tmp.bits,en_tmp.value,p_dsc->dpib->decoded_sn);
    
    /*End SPR 5818*/ 
    en_tmp.value = p_dsc->dpib->field_val[ROHC_TS_FID-ROHC_VAR_FID_BASE];
    en_tmp.bits  = p_dsc->dpib->field_len[ROHC_TS_FID-ROHC_VAR_FID_BASE];
    /* Start SPR 5818*/
    /* When tmp bits set to 0, TS is calculated based on sn */
    if(en_tmp.bits == 0)
    {
        if(ROHC_FAILURE == rohc_decode_p1_ts(p_entity, p_dsc, &en_tmp, 
                    &p_dsc->dpib->decoded_ts,p_ecode, sn_delta))
        {   
            ROHC_EXIT_FN("rohc_dprm_p1_type_decode");
            return ROHC_FAILURE;
        }
    }
    else
    {
        if(ROHC_FAILURE == rohc_decode_p1_ts(p_entity, p_dsc, &en_tmp, 
                    &p_dsc->dpib->decoded_ts,p_ecode, 0))
        {   
            ROHC_EXIT_FN("rohc_dprm_p1_type_decode");
            return ROHC_FAILURE;
        }
    }
    /*End Start SPR 5818*/
    ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
            " TS [received bits(%u), received values(%u), "\
            "decoded values(%u)]\n",
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),
            en_tmp.bits,en_tmp.value,p_dsc->dpib->decoded_ts);


   /* Fix for IP -Id Decoding */
    if(p_dsc->inner_ip4 != ROHC_NULL && ( 
       ((0 == (((p_dsc)->dpib->field_val[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE]) & ROHC_ENC_INNERIPFLAG_EXT3FLAGP1_MASK)) &&
               ((p_dsc)->rnd == ROHC_RND_ABSENT))
              || ((((p_dsc)->dpib->field_val[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE]) & ROHC_ENC_INNERIPFLAG_EXT3FLAGP1_MASK) &&
               (ROHC_RND_ABSENT  ==(((p_dsc)->dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE]) & ROHC_ENC_RND_INNERIPFLAGP1_MASK)) ))) 
    {
        en_tmp.value = 
            p_dsc->dpib->field_val[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE];
        en_tmp.bits = 
            p_dsc->dpib->field_len[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE];
        if(ROHC_FAILURE == rohc_decode_p1_inner_ip_id(p_entity, p_dsc, 
            &en_tmp,&p_dsc->dpib->decoded_ipid,p_ecode))
        {   
                        ROHC_EXIT_FN("rohc_dprm_p1_type_decode");
                        return ROHC_FAILURE;
        }
        ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
            " INNER-IP4 [received bits(%u), received values(%u), "\
            "decoded values(%u)]\n",
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),
            en_tmp.bits,en_tmp.value,p_dsc->dpib->decoded_ipid);
    }
    else if(p_dsc->outer_ip4 != ROHC_NULL && (!p_dsc->rnd2))
    {
            en_tmp.value =
                p_dsc->dpib->field_val[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE];
            en_tmp.bits =  
                p_dsc->dpib->field_len[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE];
            if(ROHC_FAILURE == rohc_decode_p1_outer_ip_id(p_entity, p_dsc, 
                    &en_tmp,&p_dsc->dpib->decoded_ipid,p_ecode))
            {   
                        ROHC_EXIT_FN("rohc_dprm_p1_type_decode");
                        return ROHC_FAILURE;
            }           
            ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                " OUTER-IP4 [received bits(%u), received values(%u), "\
                "decoded values(%u)]\n",
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),
                en_tmp.bits,en_tmp.value,p_dsc->dpib->decoded_ipid);
    }

    return ROHC_SUCCESS;
    ROHC_EXIT_FN("rohc_dprm_p1_type_decode");
}    

/*****************************************************************************
**      FUNCTION:
**            rohc_dprm_p2_decode 
******************************************************************************
**
**      DESCRIPTION:
**             Calls the decoding module for profile 2
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**              new_mode[IN}      New mode
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
rohc_return_t rohc_dprm_p2_decode
         ROHC_PARAMS((p_entity, p_dsc,p_ecode),
                rohc_entity_t     *p_entity _AND_
                rohc_dsc_udp_t    *p_dsc    _AND_
                rohc_error_code_t *p_ecode
                )
{
    rohc_encoded_value_t    en_tmp;
    rohc_U8bit_t flag = ROHC_FALSE;
    ROHC_ENTER_FN("rohc_dprm_p2_decode");
    en_tmp.value = p_dsc->dpib->field_val[ROHC_SN_FID-ROHC_VAR_FID_BASE];
    en_tmp.bits  =  p_dsc->dpib->field_len[ROHC_SN_FID-ROHC_VAR_FID_BASE];
    if(ROHC_FAILURE == rohc_decode_p2_sn(p_entity, p_dsc, &en_tmp, 
                                &p_dsc->dpib->decoded_sn,p_ecode))
    {   
            return ROHC_FAILURE;
    }       

    ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                " SN [received bits(%u), received values(%u), "\
                "decoded values(%u)]\n",
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),
                en_tmp.bits,en_tmp.value,p_dsc->dpib->decoded_sn);
   /* Fix for IP -Id Decoding */
   if((( p_dsc->inner_ip4 != ROHC_NULL ) &&  
      ((0 == (((p_dsc)->dpib->field_val[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE]) & ROHC_ENC_INNERIPFLAG_EXT3FLAGP1_MASK)) &&
               ((p_dsc)->rnd == ROHC_RND_ABSENT)))
              || ((((p_dsc)->dpib->field_val[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE]) & ROHC_ENC_INNERIPFLAG_EXT3FLAGP1_MASK) &&
               (ROHC_RND_ABSENT  ==(((p_dsc)->dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE]) & ROHC_ENC_RND_INNERIPFLAGP1_MASK)) )) 
    {
        en_tmp.value = 
            p_dsc->dpib->field_val[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE];
        en_tmp.bits = 
            p_dsc->dpib->field_len[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE];
        if(ROHC_FAILURE == rohc_decode_p2_inner_ip_id(p_entity, p_dsc, 
            &en_tmp,&p_dsc->dpib->decoded_ipid,p_ecode))
        {   
                        ROHC_EXIT_FN("rohc_dprm_p2_decode");
                        return ROHC_FAILURE;
        }

        ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                " INNER-IP4 [received bits(%u), received values(%u), "\
                "decoded values(%u)]\n",
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),
                en_tmp.bits,en_tmp.value,p_dsc->dpib->decoded_ipid);
        if(p_dsc->outer_ip4 != ROHC_NULL && (!p_dsc->rnd2))
        {
            en_tmp.value =
                p_dsc->dpib->field_val[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE];
            en_tmp.bits =  
                p_dsc->dpib->field_len[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE];
            if(ROHC_FAILURE == rohc_decode_p2_outer_ip_id(p_entity, p_dsc, 
                    &en_tmp,&p_dsc->dpib->decoded_ipid2,p_ecode))
            {   
                        ROHC_EXIT_FN("rohc_dprm_p23_decode");
                        return ROHC_FAILURE;
            }           

            ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                " OUTER-IP4 [received bits(%u), received values(%u), "\
                "decoded values(%u)]\n",
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),
                en_tmp.bits,en_tmp.value,p_dsc->dpib->decoded_ipid2);
        }
    }
    else if(p_dsc->outer_ip4 != ROHC_NULL && (!p_dsc->rnd2) && 
            (flag == ROHC_FALSE))
    {
            en_tmp.value =
                p_dsc->dpib->field_val[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE];
            en_tmp.bits =  
                p_dsc->dpib->field_len[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE];
            if(ROHC_FAILURE == rohc_decode_p2_outer_ip_id(p_entity, p_dsc, 
                    &en_tmp,&p_dsc->dpib->decoded_ipid,p_ecode))
            {   
                        ROHC_EXIT_FN("rohc_dprm_p2_decode");
                        return ROHC_FAILURE;
            }           
            ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                " INNER-IP4 [received bits(%u), received values(%u), "\
                "decoded values(%u)]\n",
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),
                en_tmp.bits,en_tmp.value,p_dsc->dpib->decoded_ipid);

    }
    ROHC_EXIT_FN("rohc_dprm_p2_decode");
    return ROHC_SUCCESS;
}    

/*****************************************************************************
**      FUNCTION:
**            rohcDprmP6Decode 
******************************************************************************
**
**      DESCRIPTION:
**             Calls the decoding module for profile 6
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**              p_ecode [IN]      Error Code pointer
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
rohc_return_t rohcDprmP6Decode(
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode
                )
{
    ROHC_ENTER_FN("rohcDprmP6Decode");
    
    switch ( p_dsc->dpib->rcv_pkt_typ )
    {
        case ROHC_IR_WITHOUT_DYN_PKTTYPE:
        case ROHC_IR_WITH_DYN_PKTTYPE:
        case ROHC_IRDYN_PKTTYPE:
            return ROHC_SUCCESS;    
        
        /*FOR IR CR P6 packet SRC and DST are LSB Encoded so we need to decode them 
        before creating the uncompressed packet*/
        case ROHC_IRP6CR_PKTYPE:
            return rohcDecodeIrCrPkt(p_entity , p_dsc , p_ecode);
        
        case ROHC_CO_P6_COMMON_PKTYPE:
              /* Decode LSB Encoding for Common Compressed Buffer*/   
                rohcDecodeP6CoCommonSeqNoAckNo(p_dsc,p_entity,p_ecode);
                rohcDecodeP6MSeqNumber( p_entity , p_dsc , p_ecode );
                rohcDecodeP6IpId( p_entity , p_dsc , p_ecode);
                rohcDecodeP6TcpOptions( p_entity , p_dsc,p_ecode);
                break;        
        case ROHC_CO_P6_RND1_PKTYPE: 
                rohcDecodeP6SeqNumber( p_entity , p_dsc , p_ecode , 65535); 
                rohcDecodeP6MSeqNumber( p_entity , p_dsc , p_ecode );
                rohcDecodeP6TcpOptions( p_entity , p_dsc,p_ecode);
                break;
  
        case ROHC_CO_P6_SEQ1_PKTYPE:
                rohcDecodeP6SeqNumber( p_entity , p_dsc , p_ecode , 32767); 
                rohcDecodeP6MSeqNumber( p_entity , p_dsc , p_ecode );
                rohcDecodeP6IpId( p_entity , p_dsc , p_ecode);
                rohcDecodeP6TcpOptions( p_entity , p_dsc,p_ecode);
                break;  
        case ROHC_CO_P6_RND2_PKTYPE:
                rohcDecodeP6MSeqNumber( p_entity ,p_dsc,p_ecode );
                rohcDecodeSeqNumScaled( p_entity ,p_dsc,p_ecode); 
                rohcDecodeP6TcpOptions( p_entity ,p_dsc,p_ecode);
            break;          
        case ROHC_CO_P6_SEQ2_PKTYPE:
                rohcDecodeP6MSeqNumber( p_entity ,p_dsc,p_ecode );
                rohcDecodeSeqNumScaled( p_entity ,p_dsc,p_ecode); 
                rohcDecodeP6IpId( p_entity , p_dsc , p_ecode);
                rohcDecodeP6TcpOptions( p_entity ,p_dsc,p_ecode);
            break;          
        case ROHC_CO_P6_RND3_PKTYPE:
                rohcDecodeP6AckNumber( p_entity ,p_dsc , p_ecode , 8191);
                rohcDecodeP6MSeqNumber( p_entity ,p_dsc , p_ecode );
                rohcDecodeP6TcpOptions( p_entity , p_dsc,p_ecode);
            break;
        case ROHC_CO_P6_SEQ3_PKTYPE:
                rohcDecodeP6AckNumber( p_entity ,p_dsc , p_ecode , 16383);
                rohcDecodeP6MSeqNumber( p_entity ,p_dsc , p_ecode );
                rohcDecodeP6IpId( p_entity , p_dsc , p_ecode);
                rohcDecodeP6TcpOptions( p_entity , p_dsc,p_ecode);
            break;          
        case ROHC_CO_P6_RND4_PKTYPE:
                rohcDecodeP6MSeqNumber( p_entity ,p_dsc , p_ecode );
                rohcDecodeP6AckNumberScaled( p_entity ,p_dsc , p_ecode);
                rohcDecodeP6TcpOptions( p_entity , p_dsc,p_ecode);
            break;          
        case ROHC_CO_P6_SEQ4_PKTYPE:
                rohcDecodeP6MSeqNumber( p_entity ,p_dsc , p_ecode );
                rohcDecodeP6AckNumberScaled( p_entity ,p_dsc , p_ecode);
                rohcDecodeP6IpId( p_entity , p_dsc , p_ecode);
                rohcDecodeP6TcpOptions( p_entity , p_dsc,p_ecode);
            break;          
        case ROHC_CO_P6_RND5_PKTYPE:
                rohcDecodeP6MSeqNumber(p_entity , p_dsc , p_ecode );
                rohcDecodeP6SeqNumber( p_entity ,p_dsc , p_ecode ,8191); 
                rohcDecodeP6AckNumber( p_entity ,p_dsc , p_ecode , 8191);
                rohcDecodeP6TcpOptions( p_entity , p_dsc,p_ecode);
            break;          
        case ROHC_CO_P6_SEQ5_PKTYPE:
                rohcDecodeP6MSeqNumber(p_entity , p_dsc , p_ecode );
                rohcDecodeP6SeqNumber( p_entity ,p_dsc , p_ecode ,32767); 
                rohcDecodeP6AckNumber( p_entity ,p_dsc , p_ecode , 16383);
                rohcDecodeP6IpId( p_entity , p_dsc , p_ecode);
                rohcDecodeP6TcpOptions( p_entity , p_dsc,p_ecode);
            break;          
        case ROHC_CO_P6_RND6_PKTYPE:
                rohcDecodeP6MSeqNumber( p_entity ,p_dsc , p_ecode );
                rohcDecodeP6AckNumber( p_entity ,p_dsc , p_ecode , 16383);
                rohcDecodeSeqNumScaled( p_entity , p_dsc,p_ecode); 
                rohcDecodeP6TcpOptions( p_entity , p_dsc,p_ecode);
            break;          
        case ROHC_CO_P6_SEQ6_PKTYPE:
                rohcDecodeP6MSeqNumber( p_entity ,p_dsc , p_ecode );
                rohcDecodeP6AckNumber( p_entity ,p_dsc , p_ecode , 16383);
                rohcDecodeSeqNumScaled( p_entity , p_dsc,p_ecode); 
                rohcDecodeP6IpId( p_entity , p_dsc , p_ecode);
                rohcDecodeP6TcpOptions( p_entity , p_dsc,p_ecode);
            break;          
        case ROHC_CO_P6_RND7_PKTYPE:
                rohcDecodeP6MSeqNumber( p_entity ,p_dsc , p_ecode );
                rohcDecodeP6AckNumber( p_entity ,p_dsc , p_ecode ,65535);
                rohcDecodeP6TcpOptions( p_entity ,p_dsc,p_ecode);
            break;          
        case ROHC_CO_P6_SEQ7_PKTYPE:
                rohcDecodeP6MSeqNumber( p_entity ,p_dsc , p_ecode );
                rohcDecodeP6AckNumber( p_entity ,p_dsc , p_ecode ,65535);
		rohcDecodeP6Window( p_entity , p_dsc , p_ecode );
                rohcDecodeP6IpId( p_entity , p_dsc , p_ecode);
                rohcDecodeP6TcpOptions( p_entity ,p_dsc,p_ecode);
            break;          
        case ROHC_CO_P6_RND8_PKTYPE:
                rohcDecodeP6MSeqNumber(p_entity , p_dsc , p_ecode );
                rohcDecodeP6SeqNumber( p_entity ,p_dsc , p_ecode , 65535); 
                rohcDecodeP6AckNumber( p_entity ,p_dsc , p_ecode, 16383);
                rohcDecodeP6TtlHopl(p_entity, p_dsc , p_ecode );
                rohcDecodeP6TcpOptions( p_entity , p_dsc,p_ecode);
                break;
                
        case ROHC_CO_P6_SEQ8_PKTYPE:
                rohcDecodeP6MSeqNumber(p_entity , p_dsc , p_ecode );
                rohcDecodeP6SeqNumber( p_entity ,p_dsc , p_ecode , 8191); 
                rohcDecodeP6AckNumber( p_entity ,p_dsc , p_ecode , 16383);
                rohcDecodeP6TtlHopl(p_entity, p_dsc , p_ecode );
                rohcDecodeP6IpId( p_entity , p_dsc , p_ecode);
                rohcDecodeP6TcpOptions( p_entity , p_dsc,p_ecode);
            break;          

    }
    ROHC_EXIT_FN("rohcDprmP6Decode");
    return ROHC_SUCCESS;
}


/*****************************************************************************
**      FUNCTION: rohcDecodeIrCrPkt
**            
******************************************************************************
**
**      DESCRIPTION:
**             Calls the decoding module for profile 6
**      ARGUMENTS:
**              p_dsc   [IN]      Pointer to the DSC block
**              p_ecode [IN]      Error Code pointer
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
static rohc_return_t rohcDecodeIrCrPkt (
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode )
{
    rohc_encoded_value_t    en_tmp;
    /*Coverity Fix 40437 Start*/
    if(0x01 == (p_dsc->dpib->tcp_replicate_partial->
        winp_listp_srcprt_dstprt_asf & ROHC_REP_SRC_PORT_PRES_MASK)>>3)
    {
        en_tmp.value = *p_dsc->dpib->tcp_replicate_srcptr->src_port;
        en_tmp.bits  = 8;

        if(ROHC_FAILURE == rohc_decode_p6_src(p_entity, p_dsc, &en_tmp, 
                   (rohc_U16bit_t *)
                   &p_dsc->dpib->decoded_sport,
                   p_ecode))
        {   
            return ROHC_FAILURE;
        }
        ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                " SRC [received bits(%u), received values(%u), "\
                "decoded values(%u)]\n",
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),
                en_tmp.bits,en_tmp.value, 
                p_dsc->dpib->decoded_sport);
    }
    
    if(0x01 == (p_dsc->dpib->tcp_replicate_partial->
        winp_listp_srcprt_dstprt_asf & ROHC_REP_DST_PORT_PRES_MASK)>>1)
    {
        en_tmp.value = *p_dsc->dpib->tcp_replicate_dstptr->dst_port;
        en_tmp.bits  = 8;
        if(ROHC_FAILURE == rohc_decode_p6_dst(p_entity, p_dsc, &en_tmp, 
                 (rohc_U16bit_t *)
                 &p_dsc->dpib->decoded_dport,p_ecode))
        {   
            return ROHC_FAILURE;
        }
        ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                " DST [received bits(%u), received values(%u), "\
                "decoded values(%u)]\n",
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),
                en_tmp.bits,en_tmp.value,
                /* SPR 9591 fix start */
                p_dsc->dpib->decoded_dport);
                /* SPR 9591 fix end */
    }
    /*Coverity Fix 40437 Stop*/
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**            rohcDecodeP6SeqNumber 
******************************************************************************
**
**      DESCRIPTION:
**             Calls the decoding module for profile 6
**      ARGUMENTS:
**              p_dsc   [IN]      Pointer to the DSC block
**              p_ecode [IN]      Error Code pointer
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
static rohc_return_t rohcDecodeP6SeqNumber( 
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode  , 
                rohc_U32bit_t      interpVal)
{
    rohc_encoded_value_t    en_tmp;
    if((p_dsc->dpib->field_len[ROHC_SN_FID-ROHC_VAR_FID_BASE]))
    {
        en_tmp.value = p_dsc->dpib->field_val[ROHC_SN_FID-ROHC_VAR_FID_BASE];
        en_tmp.bits  = p_dsc->dpib->field_len[ROHC_SN_FID-ROHC_VAR_FID_BASE];
        if ( 32 != en_tmp.bits   )
        {
            if(ROHC_FAILURE == rohcDecodeP6SN(p_entity, p_dsc, &en_tmp, 
                    &p_dsc->dpib->decoded_sn,p_ecode , interpVal))
            {   
                return ROHC_FAILURE;
            }
        }
        else
        {
          p_dsc->dpib->decoded_sn = p_dsc->dpib->
                field_val[ROHC_SN_FID-ROHC_VAR_FID_BASE];  
        }
        ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                " SN [received bits(%u), received values(%u), "\
                "decoded values(%u)]\n",
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),
                en_tmp.bits,en_tmp.value,p_dsc->dpib->decoded_sn);
    }
    return ROHC_SUCCESS;
       
}    

/*****************************************************************************
**      FUNCTION:
**            rohcDecodeP6AckNumber 
******************************************************************************
**
**      DESCRIPTION:
**             Calls the decoding module for profile 6
**      ARGUMENTS:
**              p_dsc   [IN]      Pointer to the DSC block
**              p_ecode [IN]      Error Code pointer
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
static rohc_return_t rohcDecodeP6AckNumber( 
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode ,
                rohc_U32bit_t      interpVal)
{
    rohc_encoded_value_t    en_tmp;
    if( p_dsc->dpib->field_len[ROHC_TCP_ACKNO_FID-ROHC_VAR_FID_BASE] )  
    {
	    en_tmp.value = p_dsc->dpib->field_val[ROHC_TCP_ACKNO_FID-ROHC_VAR_FID_BASE];
	    en_tmp.bits  = p_dsc->dpib->field_len[ROHC_TCP_ACKNO_FID-ROHC_VAR_FID_BASE];
        if ( 32 != en_tmp.bits   )
        {
            if(ROHC_FAILURE == rohcDecodeP6AckNum(p_entity, p_dsc, &en_tmp, 	
                    &p_dsc->dpib->decoded_ack_num,p_ecode, interpVal))
            {   
                return ROHC_FAILURE;
            } 
        }      
        else
        {
          p_dsc->dpib->decoded_ack_num = p_dsc->dpib->
                field_val[ROHC_TCP_ACKNO_FID-ROHC_VAR_FID_BASE];  
        }
        ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                " ACK NO. [received bits(%u), received values(%u), "\
                "decoded values(%u)]\n",
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),
                en_tmp.bits,en_tmp.value,p_dsc->dpib->decoded_ack_num);
    }
    return ROHC_SUCCESS;
}    


/*****************************************************************************
**      FUNCTION:
**            rohcDecodeP6AckNumberScaled 
******************************************************************************
**
**      DESCRIPTION:
**             Calls the decoding module for profile 6
**      ARGUMENTS:
**              p_dsc   [IN]      Pointer to the DSC block
**              p_ecode [IN]      Error Code pointer
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
static rohc_return_t rohcDecodeP6AckNumberScaled( 
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode )
{
    rohc_encoded_value_t    en_tmp;
    if( p_dsc->dpib->field_len[ROHC_TCP_SCALED_ACKNO_FID-ROHC_VAR_FID_BASE] )  
    {
	    en_tmp.value = p_dsc->dpib->field_val[ROHC_TCP_SCALED_ACKNO_FID-ROHC_VAR_FID_BASE];
	    en_tmp.bits  = p_dsc->dpib->field_len[ROHC_TCP_SCALED_ACKNO_FID-ROHC_VAR_FID_BASE];
        if(ROHC_FAILURE == rohcDecodeP6AckNumScaled(p_entity, p_dsc, &en_tmp, 	
                    &p_dsc->dpib->decoded_ack_num,p_ecode))
        {   
            return ROHC_FAILURE;
        }       
        ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                " ACK NO. [received bits(%u), received values(%u), "\
                "decoded values(%u)]\n",
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),
                en_tmp.bits,en_tmp.value,p_dsc->dpib->decoded_ack_num);
    }
    return ROHC_SUCCESS;
}    

/*****************************************************************************
**      FUNCTION:
**            rohcDecodeP6TtlHopl 
******************************************************************************
**
**      DESCRIPTION:
**             Calls the decoding module for profile 6
**      ARGUMENTS:
**              p_dsc   [IN]      Pointer to the DSC block
**              p_ecode [IN]      Error Code pointer
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
static rohc_return_t rohcDecodeP6TtlHopl( 
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode )
{
    rohc_encoded_value_t    en_tmp;
    en_tmp.value = p_dsc->dpib->field_val[ROHC_INNERIP4_TTL_FID-ROHC_VAR_FID_BASE];
    en_tmp.bits  = p_dsc->dpib->field_len[ROHC_INNERIP4_TTL_FID-ROHC_VAR_FID_BASE];
    if(ROHC_FAILURE == rohcDecodeP6TTL(p_entity, p_dsc, &en_tmp, 	
                                &p_dsc->dpib->decoded_ttl,p_ecode))
    {   
        return ROHC_FAILURE;
    }       
    return ROHC_SUCCESS;
}  


/*****************************************************************************
**      FUNCTION:
**            rohcDecodeP6MSeqNumber 
******************************************************************************
**
**      DESCRIPTION:
**             Calls the decoding module for profile 6
**      ARGUMENTS:
**              p_dsc   [IN]      Pointer to the DSC block
**              p_ecode [IN]      Error Code pointer
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
static rohc_return_t rohcDecodeP6MSeqNumber( 
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode )
{
    rohc_encoded_value_t    en_tmp;
    en_tmp.value = p_dsc->dpib->field_val[ROHC_TCP_MSN_FID-ROHC_VAR_FID_BASE];
    en_tmp.bits  = p_dsc->dpib->field_len[ROHC_TCP_MSN_FID-ROHC_VAR_FID_BASE];
    if(ROHC_FAILURE == rohcDecodeP6MSN(p_entity, p_dsc, &en_tmp, 	
                                &p_dsc->dpib->decoded_msn,p_ecode))
    {   
        return ROHC_FAILURE;
    }       
    return ROHC_SUCCESS;
}  

/*****************************************************************************
**      FUNCTION:
**            rohcDecodeP6Window 
******************************************************************************
**
**      DESCRIPTION:
**             Calls the decoding module for profile 6
**      ARGUMENTS:
**              p_dsc   [IN]      Pointer to the DSC block
**              p_ecode [IN]      Error Code pointer
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
static rohc_return_t rohcDecodeP6Window( 
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode )
{
    rohc_encoded_value_t    en_tmp;
    en_tmp.value = p_dsc->dpib->field_val[ROHC_TCP_WINDOW_FID-ROHC_VAR_FID_BASE];
    en_tmp.bits  = p_dsc->dpib->field_len[ROHC_TCP_WINDOW_FID-ROHC_VAR_FID_BASE];
    if(ROHC_FAILURE == rohcDecodeP6Win(p_entity, p_dsc, &en_tmp, 	
                                &p_dsc->dpib->decoded_win,p_ecode))
    {   
        return ROHC_FAILURE;
    }       
    return ROHC_SUCCESS;
}  


/*****************************************************************************
**      FUNCTION:
**            rohcDecodeSeqNumScaled 
******************************************************************************
**
**      DESCRIPTION:
**             Calls the decoding module for profile 6
**      ARGUMENTS:
**              p_dsc   [IN]      Pointer to the DSC block
**              p_ecode [IN]      Error Code pointer
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
static rohc_return_t rohcDecodeSeqNumScaled( 
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode )
{
    rohc_encoded_value_t    en_tmp;
    en_tmp.value = p_dsc->dpib->field_val[ROHC_TCP_SCALED_SN_FID-ROHC_VAR_FID_BASE];
	en_tmp.bits  = p_dsc->dpib->field_len[ROHC_TCP_SCALED_SN_FID-ROHC_VAR_FID_BASE];
    
    if(ROHC_FAILURE == rohcDecodeP6SNScaled(p_entity, p_dsc, &en_tmp, 
                    &p_dsc->dpib->decoded_sn,p_ecode))
    {   
            return ROHC_FAILURE;
    }
    ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                " SN [received bits(%u), received values(%u), "\
                "decoded values(%u)]\n",
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),
                en_tmp.bits,en_tmp.value,p_dsc->dpib->decoded_sn);

    return ROHC_SUCCESS;

}   
 


/*****************************************************************************
**      FUNCTION:
**            rohcDecodeP6IpId 
******************************************************************************
**
**      DESCRIPTION:
**             Calls the decoding module for profile 6
**      ARGUMENTS:
**              p_dsc   [IN]      Pointer to the DSC block
**              p_ecode [IN]      Error Code pointer
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
static rohc_return_t rohcDecodeP6IpId( 
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode )
{
    rohc_encoded_value_t    en_tmp;

    if(!p_dsc->dpib->field_val[ROHC_TCP_IPID_IND_FID-ROHC_TCP_CTRL_FID_BASE])
    {
        en_tmp.value = 
            p_dsc->dpib->field_val[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE];
        en_tmp.bits = 
            p_dsc->dpib->field_len[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE];
        if(ROHC_FAILURE == rohcDecodeP6InnerIpId(p_entity, p_dsc, 
                    &en_tmp,&p_dsc->dpib->decoded_ipid,p_ecode))
        {   
            return ROHC_FAILURE;
        }
    }
    else
    {
        p_dsc->dpib->decoded_ipid =
            p_dsc->dpib->field_val[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE];
    }
    
    return ROHC_SUCCESS;
}    


/*****************************************************************************
**      FUNCTION:
**            rohcDecodeP6TcpOptions 
******************************************************************************
**
**      DESCRIPTION:
**             Calls the decoding module for profile 6
**      ARGUMENTS:
**              p_dsc   [IN]      Pointer to the DSC block
**              p_ecode [IN]      Error Code pointer
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
static rohc_return_t rohcDecodeP6TcpOptions( 
                rohc_entity_t     *p_entity ,
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_error_code_t *p_ecode )
{
    rohc_encoded_value_t    en_tmp;
    /*Coverity Fix 40443 start */
    rohc_U32bit_t           ackNum = 0;    
    /*Coverity Fix 40443 end */
    /* Check for TS Val is updated for LSB decoding*/
    if (p_dsc->dpib->field_len[ROHC_TS_FID-ROHC_VAR_FID_BASE])
    {
	    en_tmp.value = p_dsc->dpib->field_val[ROHC_TS_FID-ROHC_VAR_FID_BASE];
	    en_tmp.bits  = p_dsc->dpib->field_len[ROHC_TS_FID-ROHC_VAR_FID_BASE];
        if(ROHC_FAILURE == rohcDecodeP6TS(p_entity, p_dsc, &en_tmp, 	
                    &p_dsc->dpib->decoded_ts,p_ecode))
        {   
            return ROHC_FAILURE;
        }       
        ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                " TS [received bits(%u), received values(%u), "\
                "decoded values(%u)]\n",
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),
                en_tmp.bits,en_tmp.value,p_dsc->dpib->decoded_ts);
    }

    /* Check for TS Echo Lsb Decoding*/
    if (p_dsc->dpib->field_len[ ROHC_TCP_TS_ECHO_FID - ROHC_VAR_FID_BASE])
    {
	    en_tmp.value = p_dsc->dpib->field_val[ ROHC_TCP_TS_ECHO_FID -ROHC_VAR_FID_BASE];
	    en_tmp.bits  = p_dsc->dpib->field_len[ ROHC_TCP_TS_ECHO_FID - ROHC_VAR_FID_BASE];
        if(ROHC_FAILURE == rohcDecodeP6TSEchoVal(p_entity, p_dsc, &en_tmp, 	
                    &p_dsc->dpib->decodedTsEchoVal ,p_ecode))
        {   
            return ROHC_FAILURE;
        }       
        ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                " TS [received bits(%u), received values(%u), "\
                "decoded values(%u)]\n",
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),
                en_tmp.bits,en_tmp.value,p_dsc->dpib->decoded_ts);
    }

    /* Check for SACK Blocks  */
    if (p_dsc->dpib->field_len[ ROHC_TCP_SACK_BLOCK_START_1 - ROHC_VAR_FID_BASE])
    {
        if (p_dsc->dpib->field_val[ ROHC_TCP_ACKNO_FID - ROHC_VAR_FID_BASE])
        {
	    ackNum = p_dsc->dpib->decoded_ack_num;
        }
        else
        {
            ackNum = p_dsc->ack_recvd_prev;
        }
        en_tmp.value = p_dsc->dpib->field_val[ ROHC_TCP_SACK_BLOCK_START_1 -ROHC_VAR_FID_BASE];
	    en_tmp.bits  = p_dsc->dpib->field_len[ ROHC_TCP_SACK_BLOCK_START_1 - ROHC_VAR_FID_BASE];
        rohcDecodeP6SackBlock(p_dsc, &en_tmp, &p_dsc->dpib->decodedSackBlockStart1 ,p_ecode , 
                    &ackNum);
        en_tmp.value = p_dsc->dpib->field_val[ ROHC_TCP_SACK_BLOCK_END_1 -ROHC_VAR_FID_BASE];
	    en_tmp.bits  = p_dsc->dpib->field_len[ ROHC_TCP_SACK_BLOCK_END_1 - ROHC_VAR_FID_BASE];
        rohcDecodeP6SackBlock(p_dsc, &en_tmp, 	
                    &p_dsc->dpib->decodedSackBlockEnd1 ,p_ecode , &ackNum);
      
        ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                " TS [received bits(%u), received values(%u), "\
                "decoded values(%u)]\n",
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),
                en_tmp.bits,en_tmp.value,p_dsc->dpib->decoded_ts);
    }

    if (p_dsc->dpib->field_len[ ROHC_TCP_SACK_BLOCK_START_2 - ROHC_VAR_FID_BASE])
    {
	    en_tmp.value = p_dsc->dpib->field_val[ ROHC_TCP_SACK_BLOCK_START_2 -ROHC_VAR_FID_BASE];
	    en_tmp.bits  = p_dsc->dpib->field_len[ ROHC_TCP_SACK_BLOCK_START_2 - ROHC_VAR_FID_BASE];
        rohcDecodeP6SackBlock(p_dsc, &en_tmp, 	
                    &p_dsc->dpib->decodedSackBlockStart2 ,p_ecode , &ackNum);
        en_tmp.value = p_dsc->dpib->field_val[ ROHC_TCP_SACK_BLOCK_END_2 -ROHC_VAR_FID_BASE];
	    en_tmp.bits  = p_dsc->dpib->field_len[ ROHC_TCP_SACK_BLOCK_END_2 - ROHC_VAR_FID_BASE];
        rohcDecodeP6SackBlock(p_dsc, &en_tmp, 	
                    &p_dsc->dpib->decodedSackBlockEnd2 ,p_ecode , &ackNum);
      
        ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                " TS [received bits(%u), received values(%u), "\
                "decoded values(%u)]\n",
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),
                en_tmp.bits,en_tmp.value,p_dsc->dpib->decoded_ts);
    }

    if (p_dsc->dpib->field_len[ ROHC_TCP_SACK_BLOCK_START_3 - ROHC_VAR_FID_BASE])
    {
	    en_tmp.value = p_dsc->dpib->field_val[ ROHC_TCP_SACK_BLOCK_START_3 -ROHC_VAR_FID_BASE];
	    en_tmp.bits  = p_dsc->dpib->field_len[ ROHC_TCP_SACK_BLOCK_START_3 - ROHC_VAR_FID_BASE];
        rohcDecodeP6SackBlock(p_dsc, &en_tmp, 	
                    &p_dsc->dpib->decodedSackBlockStart3 ,p_ecode , &ackNum);
        en_tmp.value = p_dsc->dpib->field_val[ ROHC_TCP_SACK_BLOCK_END_3 -ROHC_VAR_FID_BASE];
	    en_tmp.bits  = p_dsc->dpib->field_len[ ROHC_TCP_SACK_BLOCK_END_3 - ROHC_VAR_FID_BASE];
        rohcDecodeP6SackBlock(p_dsc, &en_tmp, 	
                    &p_dsc->dpib->decodedSackBlockEnd3 ,p_ecode , &ackNum);
      
        ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                " TS [received bits(%u), received values(%u), "\
                "decoded values(%u)]\n",
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),
                en_tmp.bits,en_tmp.value,p_dsc->dpib->decoded_ts);
    }

    if (p_dsc->dpib->field_len[ ROHC_TCP_SACK_BLOCK_START_4 - ROHC_VAR_FID_BASE])
    {
	    en_tmp.value = p_dsc->dpib->field_val[ ROHC_TCP_SACK_BLOCK_START_4 -ROHC_VAR_FID_BASE];
	    en_tmp.bits  = p_dsc->dpib->field_len[ ROHC_TCP_SACK_BLOCK_START_4 - ROHC_VAR_FID_BASE];
        rohcDecodeP6SackBlock(p_dsc, &en_tmp, 	
                    &p_dsc->dpib->decodedSackBlockStart4 ,p_ecode , &ackNum);
        en_tmp.value = p_dsc->dpib->field_val[ ROHC_TCP_SACK_BLOCK_END_4 -ROHC_VAR_FID_BASE];
	    en_tmp.bits  = p_dsc->dpib->field_len[ ROHC_TCP_SACK_BLOCK_END_4 - ROHC_VAR_FID_BASE];
        rohcDecodeP6SackBlock(p_dsc, &en_tmp, 	
                    &p_dsc->dpib->decodedSackBlockEnd4 ,p_ecode , &ackNum);
      
        ROHC_LOG(LOG_INFO,"DPRM:EID[%u:%u]:CID[%u]:"
                " TS [received bits(%u), received values(%u), "\
                "decoded values(%u)]\n",
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),
                en_tmp.bits,en_tmp.value,p_dsc->dpib->decoded_ts);
    }
    return ROHC_SUCCESS;
} 

/*****************************************************************************
**      FUNCTION:
**            rohcDecodeP6CoCommonSeqNoAckNo
******************************************************************************
**
**      DESCRIPTION:
**             Calls the decoding module for profile 6 CoCommon pkt
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_dsc   [IN]      Pointer to the DSC block
**              p_ecode [IN]      Error Code pointer
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/  
STATIC rohc_void_t rohcDecodeP6CoCommonSeqNoAckNo(
                rohc_dsc_tcp_t    *p_dsc    ,
                rohc_entity_t     *p_entity ,
                rohc_error_code_t *p_ecode
                )
{
    ROHC_ENTER_FN("rohcDecodeP6CoCommonSeqNoAckNo");
    if ( 0 != p_dsc->dpib->field_len[ROHC_SN_FID-ROHC_VAR_FID_BASE] )
    {    
        rohcDecodeP6SeqNumber( p_entity , p_dsc , p_ecode , 
                ((( 8== p_dsc->dpib->field_len[ROHC_SN_FID-ROHC_VAR_FID_BASE]))
                 ? 63 :16383));
    } 
    if ( 0 != p_dsc->dpib->field_len[ROHC_TCP_ACKNO_FID - ROHC_VAR_FID_BASE] )
    {    
        rohcDecodeP6AckNumber( p_entity ,  p_dsc , p_ecode ,    
                ((( 8== p_dsc->dpib->field_len[ ROHC_TCP_ACKNO_FID -ROHC_VAR_FID_BASE]))
                 ? 63 :16383));
    }
}
    



