/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_d_crc_main.c,v 1.3.36.4 2010/11/24 03:36:25 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file provides implementation function pointers table
 *                     initialization and main entry point function for 
 *                     decompressor specific CRC computation.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_d_crc_main.c,v $
 * Revision 1.3.36.4  2010/11/24 03:36:25  gur22059
 * Removal of conditional checks w.r.t profile1
 *
 * Revision 1.3.36.3  2010/11/09 05:10:22  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.3.36.2  2010/10/13 03:43:28  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.3.36.1  2010/10/01 14:11:17  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.3  2009/05/27 13:50:15  gur19836
 * File Header and Function Headers updated
 *
 *
 *****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "rohc_d_typ.h"
#include "rohc_g_typ.h"
#include "rohc_com_def.h"
#include "rohc_protocol_header.h"
#include "rohc_dcom_mcr.h"
#include "rohc_d_crc_proto.h"
#include "rohc_db_ext_if.h"
#include "rohc_trace_mcr.h"

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
 * Function Name  : rohc_d_crc_p1_compute_comp_pkt
 * Inputs         : p_entity - Pointer to entity
 *                  p_dsc - Pointer to DSC stream
 *                  p_blk
 *                  data - Data
 *                  len - Length
 * Outputs        : None
 * Returns        : ROHC_SUCCESS or ROHC_FAILURE
 * Description    : This function computes crc
 *****************************************************************************/
/* +- SPR 17777 */
rohc_return_t rohc_d_crc_p1_compute_comp_pkt ROHC_PARAMS
((p_entity,p_dsc,p_blk),
    rohc_entity_t           *p_entity   _AND_ 
    rohc_dsc_rtp_t          *p_dsc      _AND_
    rohc_dprm_update_blk_t  *p_blk
    )
{
    /* SPR5816 Fix Start */
    rohc_U8bit_t com_crc=0xFF;
    /* SPR5816 Fix End */

    ROHC_ENTER_FN("rohc_d_crc_p1_compute_comp_pkt");
    
    if(p_dsc->dpib->field_len[ROHC_CRC_FID-ROHC_VAR_FID_BASE]==3)
    {   
        com_crc=rohc_d_compute_p1_crc_static(p_dsc,p_blk,ROHC_CRC_TYPE_3, com_crc);
        com_crc = rohc_d_compute_p1_crc_dynamic(p_dsc,p_blk,
                                             ROHC_CRC_TYPE_3, com_crc);
    }
    else if (p_dsc->dpib->field_len[ROHC_CRC_FID-ROHC_VAR_FID_BASE]==7)
    {   
        com_crc=rohc_d_compute_p1_crc_static(p_dsc,p_blk,
                                          ROHC_CRC_TYPE_7, com_crc);
        com_crc = rohc_d_compute_p1_crc_dynamic(p_dsc,p_blk,
                                             ROHC_CRC_TYPE_7 , com_crc);
    }
    else
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),p_dsc->dpib->rcv_pkt_typ,
                0,0, __func__,
                "");
        /* +- SPR 17777 */
        ROHC_LOG(LOG_INFO,"DCRC:EID[%u:%u]:CID[%u]:"\
                                  "CRC-bits in the received Packet[%u]"\
                                  " are not equal to 3 or 7\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc),
                                  p_dsc->dpib->rcv_pkt_typ);
        return ROHC_FAILURE;
    }
    if( com_crc==p_dsc->dpib->field_val[ROHC_CRC_FID-ROHC_VAR_FID_BASE])
    {
        
        ROHC_LOG(LOG_INFO,"DCRC:EID[%u:%u]:CID[%u]:"\
                                  "CRC-check is successful for the received"\
                                  " packet [%u]\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc),
                                  p_dsc->dpib->rcv_pkt_typ);
    
        ROHC_EXIT_FN("rohc_d_crc_p1_compute_comp_pkt");
        return ROHC_SUCCESS;
    }
    else
    {
        ROHC_LOG(LOG_INFO,"DCRC:EID[%u:%u]:CID[%u]:"\
                                  "CRC-check is unsuccessful for the received"\
                                  " packet [%u]\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc),
                                  p_dsc->dpib->rcv_pkt_typ);

        ROHC_EXIT_FN("rohc_d_crc_p1_compute_comp_pkt");
        return ROHC_FAILURE;
    }
}

/*****************************************************************************
 * Function Name  : rohc_d_crc_p2_compute_comp_pkt
 * Inputs         : p_entity - Pointer to entity
 *                  p_dsc - Pointer to DSC stream
 *                  p_blk
 *                  data - Data
 *                  len - Length
 * Outputs        : None
 * Returns        : ROHC_SUCCESS or ROHC_FAILURE
 * Description    : This function computes crc
 *****************************************************************************/
/* +- SPR 17777 */
rohc_return_t rohc_d_crc_p2_compute_comp_pkt ROHC_PARAMS
((p_entity,p_dsc,p_blk),
    rohc_entity_t           *p_entity _AND_ 
    rohc_dsc_udp_t          *p_dsc    _AND_
    rohc_dprm_update_blk_t  *p_blk
    )
{
    /* SPR5816 Fix Start */
    rohc_U8bit_t com_crc=0xFF;
    /* SPR5816 Fix End */

    ROHC_ENTER_FN("rohc_d_crc_p2_compute_comp_pkt");

    if(p_dsc->dpib->field_len[ROHC_CRC_FID-ROHC_VAR_FID_BASE]==3)
    {   
        com_crc=rohc_d_compute_p2_crc_static(p_dsc,p_blk,
                                             ROHC_CRC_TYPE_3, com_crc);
        com_crc = rohc_d_compute_p2_crc_dynamic(p_dsc,p_blk,
                                             ROHC_CRC_TYPE_3, com_crc);
    }
    else if (p_dsc->dpib->field_len[ROHC_CRC_FID-ROHC_VAR_FID_BASE]==7)
    {   
        com_crc=rohc_d_compute_p2_crc_static(p_dsc,p_blk,
                                          ROHC_CRC_TYPE_7, com_crc);
        com_crc = rohc_d_compute_p2_crc_dynamic(p_dsc,p_blk,
                                             ROHC_CRC_TYPE_7 , com_crc);
    }
    else
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),p_dsc->dpib->rcv_pkt_typ,
                0,0, __func__,
                "");
        /* +- SPR 17777 */
        ROHC_LOG(LOG_INFO,"DCRC:EID[%u:%u]:CID[%u]:"\
                                  "CRC-bits in the received Packet[%u]"\
                                  " are not equal to 3 or 7\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc),
                                  p_dsc->dpib->rcv_pkt_typ);
        return ROHC_FAILURE;
    }
    if( com_crc==p_dsc->dpib->field_val[ROHC_CRC_FID-ROHC_VAR_FID_BASE])
    {
        
        ROHC_LOG(LOG_INFO,"DCRC:EID[%u:%u]:CID[%u]:"\
                                  "CRC-check is successful for the received"\
                                  " packet [%u]\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc),
                                  p_dsc->dpib->rcv_pkt_typ);
    
        ROHC_EXIT_FN("rohc_d_crc_p2_compute_comp_pkt");
        return ROHC_SUCCESS;
    }
    else
    {
        ROHC_LOG(LOG_INFO,"DCRC:EID[%u:%u]:CID[%u]:"\
                                  "CRC-check is unsuccessful for the received"\
                                  " packet [%u]\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc),
                                  p_dsc->dpib->rcv_pkt_typ);

        ROHC_EXIT_FN("rohc_d_crc_p2_compute_comp_pkt");
        return ROHC_FAILURE;
    }
}

/*****************************************************************************
 * Function Name  : rohc_d_crc_p6_compute_comp_pkt
 * Inputs         : p_entity - Pointer to entity
 *                  p_dsc - Pointer to DSC stream
 *                  p_blk
 *                  data - Data
 *                  len - Length
 * Outputs        : None
 * Returns        : ROHC_SUCCESS or ROHC_FAILURE
 * Description    : This function computes crc
 *****************************************************************************/
/* +- SPR 17777 */
rohc_return_t rohc_d_crc_p6_compute_comp_pkt(
    rohc_entity_t           *p_entity, 
    rohc_dsc_tcp_t          *p_dsc,
    rohc_dprm_update_blk_t  *p_blk
    )
{
    /* SPR5816 Fix Start */
    rohc_U8bit_t com_crc=0xFF;
    /* SPR5816 Fix End */

    ROHC_ENTER_FN("rohc_d_crc_p6_compute_comp_pkt");
    
    if(p_dsc->dpib->field_len[ROHC_CRC_FID-ROHC_VAR_FID_BASE]==3)
    {   
        com_crc = rohc_d_calculate_crc ( p_blk->inner_ip4 , 1 ,20 , com_crc , 
                    ROHC_CRC_TYPE_3  ); 

        com_crc = rohc_d_calculate_crc ( p_blk->tcp , 1 ,(p_blk->tcp->offset>>4)*4 , 
                    com_crc , ROHC_CRC_TYPE_3  ); 
        
    }
    else if (p_dsc->dpib->field_len[ROHC_CRC_FID-ROHC_VAR_FID_BASE]==7 ||
        (p_dsc->dpib->rcv_pkt_typ == ROHC_IRP6CR_PKTYPE))
    {   
        com_crc = rohc_d_calculate_crc ( p_blk->inner_ip4 , 1 ,20 , com_crc , 
                    ROHC_CRC_TYPE_7  ); 

        com_crc = rohc_d_calculate_crc ( p_blk->tcp , 1 ,(p_blk->tcp->offset>>4)*4 , 
                    com_crc , ROHC_CRC_TYPE_7); 
    
    }
    else
    {

        /* +- SPR 17777 */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),p_dsc->dpib->rcv_pkt_typ
                ,0,0, __func__,
                "");
        /* +- SPR 17777 */
        ROHC_LOG(LOG_INFO,"DCRC:EID[%u:%u]:CID[%u]:"\
                                  "CRC-bits in the received Packet[%u]"\
                                  " are not equal to 3 or 7\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc),
                                  p_dsc->dpib->rcv_pkt_typ);
        return ROHC_FAILURE;
    }
    if((com_crc==p_dsc->dpib->field_val[ROHC_CRC_FID-ROHC_VAR_FID_BASE]) ||
        (com_crc==p_dsc->dpib->rohc_tcp_baseCID_CRC7))
    {
        
        ROHC_LOG(LOG_INFO,"DCRC:EID[%u:%u]:CID[%u]:"\
                                  "CRC-check is successful for the received"\
                                  " packet [%u]\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc),
                                  p_dsc->dpib->rcv_pkt_typ);
    
        ROHC_EXIT_FN("rohc_d_crc_p6_compute_comp_pkt");
        return ROHC_SUCCESS;
    }
    else
    {
        ROHC_LOG(LOG_INFO,"DCRC:EID[%u:%u]:CID[%u]:"\
                                  "CRC-check is unsuccessful for the received"\
                                  " packet [%u]\n",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(p_dsc),
                                  p_dsc->dpib->rcv_pkt_typ);

        ROHC_EXIT_FN("rohc_d_crc_p6_compute_comp_pkt");
        return ROHC_FAILURE;
    }
}
/*****************************************************************************
 * Function Name  : rohc_d_calculate_crc
 * Inputs         : data - Data ptr
 *                  start -
 *                  end -
 *                  previous_crc
 *                  crc_type
 * Outputs        : None
 * Returns        : 
 * Description    : This function calculated CRC
 *****************************************************************************/
rohc_U8bit_t    rohc_d_calculate_crc(rohc_void_t *data,
                                     rohc_U32bit_t start,
                                     rohc_U32bit_t end, 
                                     rohc_U8bit_t previous_crc, 
                                     rohc_U8bit_t crc_type)
{
    return rohc_compute_crc(crc_type,(rohc_U8bit_t*)data+start-1,end-start+1,previous_crc);
}

/*****************************************************************************
 * Function Name  : rohc_d_compute_p1_csrc_list_crc
 * Inputs         : p_dsc - Pointer to DSC Stream
 *                  p_blk - 
 *                  crc_type -
 *                  crc_val -
 * Outputs        : None
 * Returns        : 
 * Description    : 
 *****************************************************************************/
rohc_U8bit_t  rohc_d_compute_p1_csrc_list_crc(/* +- SPR 17777 */
                                              rohc_dprm_update_blk_t *p_blk,
                                              rohc_U8bit_t           crc_type,
                                              rohc_U8bit_t           crc_val)
{
    rohc_U32bit_t       *p_csrc_node;
    rohc_U32bit_t       tot_count = 0;

    /* SPR 4808 Fix Start */
    p_csrc_node = (rohc_U32bit_t *)((p_blk->rtp) + 1);
    /* SPR 4808 Fix End */
    tot_count = (((rohc_rtp_t *)(p_blk->rtp))->v_p_x_cc & ROHC_CC_MASK);

    return rohc_d_calculate_crc(p_csrc_node,1,(4*tot_count),crc_val,crc_type);

}

/*****************************************************************************
 * Function Name  : rohc_d_compute_p1_ip_static_crc
 * Inputs         : p_dsc, p_blk, crc_type, crc_val
 * Outputs        : None
 * Returns        : 
 * Description    : 
 *****************************************************************************/
rohc_U8bit_t rohc_d_compute_p1_ip_static_crc (rohc_dsc_rtp_t         *p_dsc,
                                              rohc_dprm_update_blk_t *p_blk,
                                              rohc_U8bit_t           crc_type, 
                                              rohc_U8bit_t           crc_val)
{
    int list_count,i;
    rohc_list_node_t *node; 

    if(p_blk->inner_ip4 != ROHC_NULL)
    {
        crc_val = rohc_d_calculate_crc(p_blk->inner_ip4,1,2,crc_val,crc_type);
        crc_val = rohc_d_calculate_crc(p_blk->inner_ip4,7,10,crc_val,crc_type);
        crc_val = rohc_d_calculate_crc(p_blk->inner_ip4,13,20,crc_val,crc_type);

    }
    else
    {
        crc_val = rohc_d_calculate_crc(p_blk->inner_ip6,1,4,crc_val,crc_type);
        crc_val = rohc_d_calculate_crc(p_blk->inner_ip6,7,40,crc_val,crc_type);
    }
    if(p_blk->outer_ip4 != ROHC_NULL)
    {
        crc_val = rohc_d_calculate_crc(p_blk->outer_ip4,1,2,crc_val,crc_type);
        crc_val = rohc_d_calculate_crc(p_blk->outer_ip4,7,10,crc_val,crc_type);
        crc_val = rohc_d_calculate_crc(p_blk->outer_ip4,13,20,crc_val,crc_type);

    }
    else if(p_blk->outer_ip6 != ROHC_NULL)
    {
        crc_val = rohc_d_calculate_crc(p_blk->outer_ip6,1,4,crc_val,crc_type);
        crc_val = rohc_d_calculate_crc(p_blk->outer_ip6,7,40,crc_val,crc_type);
    }

    list_count = rohc_db_count_of_list(&(p_dsc->dpib->inneripextlist));
     node = rohc_db_get_first_node(&(p_dsc->dpib->inneripextlist));
    if(node == ROHC_NULL)
    {
        /*empty list*/
        ROHC_LOG(LOG_INFO,
                         "Empty Extention header list found");
        /* SPR 3444 changes starts */
        return crc_val;
        /* SPR 3444 changes ends */
    }

    for(i=0; i < list_count; i++)
    {
        rohc_protocol_node_t *rohc_proto_node = (rohc_protocol_node_t *)node;
        switch(rohc_proto_node->protocol)
        {

            case ROHC_IPPROTO_GRE:
            {
                crc_val = rohc_d_calculate_crc(rohc_proto_node->ptr,1,
                                rohc_proto_node->length,crc_val,crc_type);
                break;
            }
            case ROHC_IPPROTO_MINENC:
            {
                crc_val = rohc_d_calculate_crc(rohc_proto_node->ptr,1,
                                rohc_proto_node->length,crc_val,crc_type);
                break;
            }
            case ROHC_IPPROTO_HOPOPTS:
            {
                crc_val = rohc_d_calculate_crc(rohc_proto_node->ptr,
                                1,rohc_proto_node->length,crc_val,crc_type);
                break;
            }
            case ROHC_IPPROTO_DSTOPTS:
            {
                crc_val = rohc_d_calculate_crc(rohc_proto_node->ptr,
                                1,rohc_proto_node->length,crc_val,crc_type);
                break;
            }
                    case ROHC_IPPROTO_FRAGOPTS:
            {
                crc_val = rohc_d_calculate_crc(rohc_proto_node->ptr,
                                1,rohc_proto_node->length,crc_val,crc_type);
                break;
            }
            case ROHC_IPPROTO_ROUTOPTS:
            {
                crc_val = rohc_d_calculate_crc(rohc_proto_node->ptr,1,
                                rohc_proto_node->length,crc_val,crc_type);
                break;
            }
            case ROHC_IPPROTO_AH:
            {
                /* no static crc req. all fields are crc dynamic*/
                break;

            }
            case ROHC_IPPROTO_ESP:
            {
                /*  no static crc req. all fields are crc dynamic */
                break;

            }
            default:
            ROHC_LOG(LOG_INFO,
                            "Unknown IP Extention header found");

        }
        node = rohc_db_get_next_node(node);
    }

    return crc_val;

}

/*****************************************************************************
 * Function Name  : rohc_d_compute_p2_ip_static_crc
 * Inputs         : p_dsc, p_blk, crc_type, crc_val
 * Outputs        : None
 * Returns        : 
 * Description    : 
 *****************************************************************************/
rohc_U8bit_t rohc_d_compute_p2_ip_static_crc (rohc_dsc_udp_t *p_dsc,
                                           rohc_dprm_update_blk_t *p_blk,
                                           rohc_U8bit_t crc_type, 
                                           rohc_U8bit_t crc_val)
{
    int list_count,i;
    rohc_list_node_t *node; 

    if(p_blk->inner_ip4 != ROHC_NULL)
    {
        crc_val = rohc_d_calculate_crc(p_blk->inner_ip4,1,2,crc_val,crc_type);
        crc_val = rohc_d_calculate_crc(p_blk->inner_ip4,7,10,crc_val,crc_type);
        crc_val = rohc_d_calculate_crc(p_blk->inner_ip4,13,20,crc_val,crc_type);

    }
    else
    {
        crc_val = rohc_d_calculate_crc(p_blk->inner_ip6,1,4,crc_val,crc_type);
        crc_val = rohc_d_calculate_crc(p_blk->inner_ip6,7,40,crc_val,crc_type);
    }
    if(p_blk->outer_ip4 != ROHC_NULL)
    {
        crc_val = rohc_d_calculate_crc(p_blk->outer_ip4,1,2,crc_val,crc_type);
        crc_val = rohc_d_calculate_crc(p_blk->outer_ip4,7,10,crc_val,crc_type);
        crc_val = rohc_d_calculate_crc(p_blk->outer_ip4,13,20,crc_val,crc_type);

    }
    else if(p_blk->outer_ip6 != ROHC_NULL)
    {
        crc_val = rohc_d_calculate_crc(p_blk->outer_ip6,1,4,crc_val,crc_type);
        crc_val = rohc_d_calculate_crc(p_blk->outer_ip6,7,40,crc_val,crc_type);
    }

    list_count = rohc_db_count_of_list(&(p_dsc->dpib->inneripextlist));
     node = rohc_db_get_first_node(&(p_dsc->dpib->inneripextlist));
    if(node == ROHC_NULL)
    {
        /*empty list*/
        ROHC_LOG(LOG_INFO,
                         "static Empty Extention header list found");
        /* SPR 3444 changes starts */
        return crc_val;
        /* SPR 3444 changes ends */
    }

    for(i=0; i < list_count; i++)
    {
        rohc_protocol_node_t *rohc_proto_node = (rohc_protocol_node_t *)node;
        switch(rohc_proto_node->protocol)
        {

            case ROHC_IPPROTO_GRE:
            {
                crc_val = rohc_d_calculate_crc(rohc_proto_node->ptr,1,
                                rohc_proto_node->length,crc_val,crc_type);
                break;
            }
            case ROHC_IPPROTO_MINENC:
            {
                crc_val = rohc_d_calculate_crc(rohc_proto_node->ptr,1,
                                rohc_proto_node->length,crc_val,crc_type);
                break;
            }
            case ROHC_IPPROTO_HOPOPTS:
            {
                crc_val = rohc_d_calculate_crc(rohc_proto_node->ptr,
                                1,rohc_proto_node->length,crc_val,crc_type);
                break;
            }
            case ROHC_IPPROTO_DSTOPTS:
            {
                crc_val = rohc_d_calculate_crc(rohc_proto_node->ptr,
                                1,rohc_proto_node->length,crc_val,crc_type);
                break;
            }
                    case ROHC_IPPROTO_FRAGOPTS:
            {
                crc_val = rohc_d_calculate_crc(rohc_proto_node->ptr,
                                1,rohc_proto_node->length,crc_val,crc_type);
                break;
            }
            case ROHC_IPPROTO_ROUTOPTS:
            {
                crc_val = rohc_d_calculate_crc(rohc_proto_node->ptr,1,
                                rohc_proto_node->length,crc_val,crc_type);
                break;
            }
            case ROHC_IPPROTO_AH:
            {
                /* no static crc req. all fields are crc dynamic*/
                break;

            }
            case ROHC_IPPROTO_ESP:
            {
                /*  no static crc req. all fields are crc dynamic */
                break;

            }
            default:
            ROHC_LOG(LOG_INFO,
                            "Unknown IP Extention header found");

        }
        node = rohc_db_get_next_node(node);
    }

    return crc_val;

}

/*****************************************************************************
 * Function Name  : rohc_d_compute_p1_ip_dynamic_crc
 * Inputs         : p_dsc, p_blk, crc_type, crc_val
 * Outputs        : None
 * Returns        : 
 * Description    : 
 *****************************************************************************/
rohc_U8bit_t rohc_d_compute_p1_ip_dynamic_crc (rohc_dsc_rtp_t         *p_dsc, 
                                               rohc_dprm_update_blk_t *p_blk,
                                               rohc_U8bit_t           crc_type, 
                                               rohc_U8bit_t           crc_val)
{
    rohc_U8bit_t    *p_buf = ROHC_NULL, *p_temp = ROHC_NULL;
    rohc_U8bit_t    list_count;
    rohc_U16bit_t   gre_flags;
    rohc_list_node_t *p_node;
    rohc_U32bit_t i;

    if(p_blk->inner_ip4 != ROHC_NULL)
    {
        crc_val = rohc_d_calculate_crc(p_blk->inner_ip4,3,4,crc_val,crc_type);
        crc_val = rohc_d_calculate_crc(p_blk->inner_ip4,5,6,crc_val,crc_type);
        crc_val = rohc_d_calculate_crc(p_blk->inner_ip4,11,12,crc_val,crc_type);

    }
    else
    {
        crc_val = rohc_d_calculate_crc(p_blk->inner_ip6,5,6,crc_val,crc_type);
    }
    if(p_blk->outer_ip4 != ROHC_NULL)
    {
        crc_val = rohc_d_calculate_crc(p_blk->outer_ip4,3,4,crc_val,crc_type);
        
        crc_val = rohc_d_calculate_crc(p_blk->outer_ip4,5,6,crc_val,crc_type);
        crc_val = rohc_d_calculate_crc(p_blk->outer_ip4,11,12,crc_val,crc_type);

    }
    else if(p_blk->outer_ip6 != ROHC_NULL)
    {
        crc_val = rohc_d_calculate_crc(p_blk->outer_ip6,5,6,crc_val,crc_type);
    }

    list_count = rohc_db_count_of_list(&(p_dsc->dpib->inneripextlist));
    p_node  = (rohc_list_node_t *)rohc_db_get_first_node
                            (&(p_dsc->dpib->inneripextlist)) ;
    
    if(p_node == ROHC_NULL)
    {
        /*empty list*/
        ROHC_LOG(LOG_INFO,
                         "Empty Extention header list found");
        /* SPR 3444 changes starts */
        return crc_val; 
        /* SPR 3444 changes ends */
    }

    for(i=0; i < list_count; i++)
    {
        rohc_protocol_node_t *rohc_proto_node = (rohc_protocol_node_t *)p_node;
        switch(rohc_proto_node->protocol)
        {

            case ROHC_IPPROTO_AH:
            {
             /*   // no static crc req. all fields are crc dynamic*/
                crc_val = rohc_d_calculate_crc(rohc_proto_node->ptr,1,
                                               rohc_proto_node->length,
                                               crc_val,crc_type);
                break;

            }
            case ROHC_IPPROTO_GRE:
            {
                p_buf = ((rohc_protocol_node_t *)p_node)->ptr;
                gre_flags = ((rohc_gre_t *)p_buf)->flags;
                p_temp = p_buf + sizeof(rohc_gre_t);
                if(gre_flags & ROHC_GRE_CP)
                {
                    p_temp = p_buf + 4;
                }
                if(gre_flags & ROHC_GRE_KP)
                {
                    /* Key present */
                    p_temp = p_buf + 4;
                }
                if(gre_flags & ROHC_GRE_SP)
                {
                    crc_val = rohc_d_calculate_crc(p_temp,1,4,crc_val,crc_type);
                }
                break;
            }
            case ROHC_IPPROTO_MINENC:
            {
/*                //  no dynamic crc req. all fields are crc crc-static*/
                break;
            }
            case ROHC_IPPROTO_HOPOPTS:
            {
/*                //  no dynamic crc req. all fields are crc crc-static*/
                break;
            }
            case ROHC_IPPROTO_DSTOPTS:
            {
/*                //  no dynamic crc req. all fields are crc crc-static*/
                break;
            }
            case ROHC_IPPROTO_FRAGOPTS:
            {
 /*               //  no dynamic crc req. all fields are crc crc-static*/
                break;
            }
            case ROHC_IPPROTO_ROUTOPTS:
            {
  /*              //  no dynamic crc req. all fields are crc crc-static*/
                break;
            }
            default:
            ROHC_LOG(LOG_INFO,
                             "Unknown IP Extention header found");

        }
        p_node = (rohc_list_node_t *)rohc_db_get_next_node(p_node);
    }

    return crc_val;
}

/*****************************************************************************
 * Function Name  : rohc_d_compute_p2_ip_dynamic_crc
 * Inputs         : p_dsc, p_blk, crc_type, crc_val
 * Outputs        : None
 * Returns        : 
 * Description    : 
 *****************************************************************************/
rohc_U8bit_t rohc_d_compute_p2_ip_dynamic_crc (rohc_dsc_udp_t *p_dsc, 
                                    rohc_dprm_update_blk_t     *p_blk,
                                    rohc_U8bit_t               crc_type, 
                                    rohc_U8bit_t               crc_val)
{
    rohc_U8bit_t    *p_buf = ROHC_NULL, *p_temp = ROHC_NULL;
    rohc_U8bit_t    list_count;
    rohc_U16bit_t   gre_flags;
    rohc_list_node_t *p_node;
    rohc_U32bit_t i;

    if(p_blk->inner_ip4 != ROHC_NULL)
    {
        crc_val = rohc_d_calculate_crc(p_blk->inner_ip4,3,4,crc_val,crc_type);
        crc_val = rohc_d_calculate_crc(p_blk->inner_ip4,5,6,crc_val,crc_type);
        crc_val = rohc_d_calculate_crc(p_blk->inner_ip4,11,12,crc_val,crc_type);

    }
    else
    {
        crc_val = rohc_d_calculate_crc(p_blk->inner_ip6,5,6,crc_val,crc_type);
    }
    if(p_blk->outer_ip4 != ROHC_NULL)
    {
        crc_val = rohc_d_calculate_crc(p_blk->outer_ip4,3,4,crc_val,crc_type);
        
        crc_val = rohc_d_calculate_crc(p_blk->outer_ip4,5,6,crc_val,crc_type);
        crc_val = rohc_d_calculate_crc(p_blk->outer_ip4,11,12,crc_val,crc_type);

    }
    else if(p_blk->outer_ip6 != ROHC_NULL)
    {
        crc_val = rohc_d_calculate_crc(p_blk->outer_ip6,5,6,crc_val,crc_type);
    }

    list_count = rohc_db_count_of_list(&(p_dsc->dpib->inneripextlist));
    p_node  = (rohc_list_node_t *)rohc_db_get_first_node
                            (&(p_dsc->dpib->inneripextlist)) ;
    
    if(p_node == ROHC_NULL)
    {
        /*empty list*/
        ROHC_LOG(LOG_INFO,
                         "Empty Extention header list found");
        /* SPR 3444 changes starts */
        return crc_val;
        /* SPR 3444 changes ends */
    }

    for(i=0; i < list_count; i++)
    {
        rohc_protocol_node_t *rohc_proto_node = (rohc_protocol_node_t *)p_node;
        switch(rohc_proto_node->protocol)
        {

            case ROHC_IPPROTO_AH:
            {
             /*   // no static crc req. all fields are crc dynamic*/
                crc_val = rohc_d_calculate_crc(rohc_proto_node->ptr,1,
                                               rohc_proto_node->length,
                                               crc_val,crc_type);
                break;

            }
            case ROHC_IPPROTO_GRE:
            {
                p_buf = ((rohc_protocol_node_t *)p_node)->ptr;
                gre_flags = ((rohc_gre_t *)p_buf)->flags;
                p_temp = p_buf + sizeof(rohc_gre_t);
                if(gre_flags & ROHC_GRE_CP)
                {
                    p_temp = p_buf + 4;
                }
                if(gre_flags & ROHC_GRE_KP)
                {
                    /* Key present */
                    p_temp = p_buf + 4;
                }
                if(gre_flags & ROHC_GRE_SP)
                {
                    crc_val = rohc_d_calculate_crc(p_temp,1,4,crc_val,crc_type);
                }
                break;
            }
            case ROHC_IPPROTO_MINENC:
            {
/*                //  no dynamic crc req. all fields are crc crc-static*/
                break;
            }
            case ROHC_IPPROTO_HOPOPTS:
            {
/*                //  no dynamic crc req. all fields are crc crc-static*/
                break;
            }
            case ROHC_IPPROTO_DSTOPTS:
            {
/*                //  no dynamic crc req. all fields are crc crc-static*/
                break;
            }
            case ROHC_IPPROTO_FRAGOPTS:
            {
 /*               //  no dynamic crc req. all fields are crc crc-static*/
                break;
            }
            case ROHC_IPPROTO_ROUTOPTS:
            {
  /*              //  no dynamic crc req. all fields are crc crc-static*/
                break;
            }
            default:
            ROHC_LOG(LOG_INFO,
                             "Unknown IP Extention header found");

        }
        p_node = (rohc_list_node_t *)rohc_db_get_next_node(p_node);
    }

    return crc_val;
}

/*****************************************************************************
 * Function Name  : rohc_d_compute_p1_crc_static
 * Inputs         : p_dsc, p_blk, crc_type, crc_val
 * Outputs        : None
 * Returns        : 
 * Description    : 
 *****************************************************************************/
rohc_U8bit_t rohc_d_compute_p1_crc_static (rohc_dsc_rtp_t         *p_dsc, 
                                           rohc_dprm_update_blk_t *p_blk,
                                           rohc_U8bit_t           crc_type,
                                           rohc_U8bit_t           crc_val)
{
    /*START - static crc calc */
    crc_val = rohc_d_compute_p1_ip_static_crc(p_dsc,p_blk,crc_type,crc_val);
    crc_val = rohc_d_calculate_crc(p_blk->udp,1,4,crc_val,crc_type);
    crc_val = rohc_d_calculate_crc(p_blk->rtp,1,1,crc_val,crc_type);
    crc_val = rohc_d_calculate_crc(p_blk->rtp,9,12,crc_val,crc_type);
    /* +- SPR 17777 */
    crc_val = rohc_d_compute_p1_csrc_list_crc(p_blk,crc_type,crc_val);
    /*END - static crc calc */

    return crc_val;
}

/*****************************************************************************
 * Function Name  : rohc_d_compute_p2_crc_static
 * Inputs         : p_dsc, p_blk, crc_type, crc_val
 * Outputs        : None
 * Returns        : 
 * Description    : 
 *****************************************************************************/
rohc_U8bit_t rohc_d_compute_p2_crc_static (rohc_dsc_udp_t *p_dsc, 
                                        rohc_dprm_update_blk_t *p_blk,
                                        rohc_U8bit_t crc_type,
                                        rohc_U8bit_t crc_val)
{
    /*START - static crc calc */
    crc_val = rohc_d_compute_p2_ip_static_crc(p_dsc,p_blk,
                                           crc_type,crc_val);
    crc_val = rohc_d_calculate_crc(p_blk->udp,1,4,crc_val,crc_type);
    /*END - static crc calc */
    return crc_val;
}

/*****************************************************************************
 * Function Name  : rohc_d_compute_p1_crc_dynamic
 * Inputs         : p_dsc, p_blk, crc_type, crc_val
 * Outputs        : None
 * Returns        : 
 * Description    : 
 *****************************************************************************/
rohc_U8bit_t rohc_d_compute_p1_crc_dynamic (rohc_dsc_rtp_t         *p_dsc,
                                            rohc_dprm_update_blk_t *p_blk,
                                            rohc_U8bit_t           crc_type, 
                                            rohc_U8bit_t           crc_val)
{

    /*START - dyanamic crc calc */
    crc_val = rohc_d_compute_p1_ip_dynamic_crc (p_dsc,p_blk,crc_type,crc_val);
    crc_val = rohc_d_calculate_crc(p_blk->udp,5,8,crc_val,crc_type);
    crc_val = rohc_d_calculate_crc(p_blk->rtp,2,8,crc_val,crc_type);

    /*END - dyanamic crc calc */
    return crc_val;
}

/*****************************************************************************
 * Function Name  : rohc_d_compute_p2_crc_dynamic
 * Inputs         : p_dsc, p_blk, crc_type, crc_val
 * Outputs        : None
 * Returns        : 
 * Description    : 
 *****************************************************************************/
rohc_U8bit_t rohc_d_compute_p2_crc_dynamic (rohc_dsc_udp_t *p_dsc,
                                         rohc_dprm_update_blk_t *p_blk,
                                         rohc_U8bit_t crc_type, 
                                         rohc_U8bit_t crc_val)
{
    /*START - dyanamic crc calc */
    crc_val = rohc_d_compute_p2_ip_dynamic_crc (p_dsc,p_blk,
                                             crc_type,crc_val);
    crc_val = rohc_d_calculate_crc(p_blk->udp,5,8,crc_val,crc_type);
    /*END - dyanamic crc calc */
    return crc_val;
}
