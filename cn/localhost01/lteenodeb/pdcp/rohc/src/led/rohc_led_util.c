/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_led_util.c,v 1.3.36.7 2010/11/26 05:35:23 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the functions for Compression of IP
 *                     Extension header list.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_led_util.c,v $
 * Revision 1.3.36.7  2010/11/26 05:35:23  gur22059
 * Use of register variables for profile 1
 *
 * Revision 1.3.36.6  2010/11/24 03:41:01  gur22059
 * Removal of conditional checks w.r.t profile1
 *
 * Revision 1.3.36.5  2010/11/09 05:20:44  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.3.36.4  2010/10/13 03:52:39  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.3.36.3  2010/10/04 03:42:27  gur22059
 * Rename function rohc_udp_led_init_inneripext to rohc_led_p2_init_inneripext
 *
 * Revision 1.3.36.2  2010/10/01 14:10:06  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.3.36.1  2010/09/23 05:17:45  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.3  2009/05/27 16:14:06  gur19836
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
#include "rohc_def.h"
#include "rohc_typ.h"
#include "rohc_g_typ.h"
#include "rohc_gcom_mcr.h"
#include "rohc_com_def.h"
#include "rohc_protocol_header.h"
#include "rohc_c_typ.h"
#include "rohc_ccom_mcr.h"
#include "rohc_d_typ.h"
#include "rohc_dcom_mcr.h"
#include "rohc_trace_mcr.h"
#include "rohc_sl_windows.h"
#include "rohc_db_ext_if.h"
#include "rohc_led_proto.h"
#include "rohc_led_mcr.h"
#include "rohc_led_def.h"
#include "rohc_led_ext_if.h"
#include "rohc_ed_ext_if.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* + SPR 17439 */
static rohc_void_t   rohc_cled_p1_r_ack_handling  ROHC_PROTO((
    rohc_csc_rtp_t      *p_csc  ,   
    rohc_U16bit_t       rtp_sn ));

STATIC rohc_return_t  rohc_c_compare_n_encode_p1_gre_ext_hdr_item    ROHC_PROTO((
 rohc_csc_rtp_t      *p_csc      ,
 rohc_U8bit_t        *p_item     ,
 rohc_list_node_t    *p_node     ,
 rohc_U32bit_t       *p_gseq_no));

STATIC rohc_return_t  rohc_c_compare_n_encode_p2_gre_ext_hdr_item    ROHC_PROTO((
 rohc_csc_udp_t      *p_csc      ,
 rohc_U8bit_t        *p_item     ,
 rohc_list_node_t    *p_node     ,
 rohc_U32bit_t       *p_gseq_no));

STATIC rohc_return_t   rohc_d_store_item_gre_in_list   ROHC_PROTO((
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt   ,
    rohc_U8bit_t                *p_buf          ,
    rohc_U32bit_t               *p_length       ,
    rohc_gre_t                  **p_p_gre       ,
    rohc_error_code_t           *p_ecode));
/* - SPR 17439 */

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
**          rohc_c_convert_p1_in_buffer
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to convert the items in buffer format, when
**          items go as uncompressed items
**      ARGUMENTS:
**          The item to be converted is taken as input
**      RETURN VALUE:
**          Pointer to the buffer
**      NOTES:
*****************************************************************************/
rohc_return_t    rohc_c_convert_p1_in_buffer ROHC_PARAMS
    (
        (p_entity, p_csc, p_node, p_buf, p_size, p_ecode),
            rohc_entity_t       *p_entity   _AND_
            rohc_csc_rtp_t      *p_csc      _AND_
            rohc_U8bit_t        *p_node     _AND_
            rohc_U8bit_t        *p_buf      _AND_
            rohc_U32bit_t       *p_size     _AND_
            rohc_error_code_t   *p_ecode
    )
{
    rohc_U8bit_t    length = 0, *p_gre_buf = ROHC_NULL;
    rohc_U16bit_t   gre_flags;
    rohc_return_t   ret = ROHC_SUCCESS;

    ROHC_ENTER_FN("rohc_c_convert_p1_in_buffer");

    p_buf = p_buf + *p_size; /* Start writing the buffer from this point */

    switch(((rohc_protocol_node_t *)p_node)->protocol)
    {   
        case ROHC_IPPROTO_AH:
        {
            *p_size += ((rohc_protocol_node_t *)p_node)->length;
            ROHC_MEMCPY(p_buf, ((rohc_protocol_node_t *)p_node)->ptr, *p_size);
            *p_buf = ((rohc_protocol_node_t *)p_node)->protocol;
            p_csc->ah_seq = ((rohc_ah_t *)(((rohc_protocol_node_t *)
                            p_node)->ptr))->seq;
            /* +- SPR 17777 */
            LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
            /* +- SPR 17777 */
            ROHC_LOG(LOG_INFO,
            "CLED:EID[%d:%d]:CID[%d]: " \
            " The calculated length of AH header [%d] \n", \
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
            ROHC_CSC_GET_CID(p_csc), ((rohc_protocol_node_t *)p_node)->length);
            break;
        }
        case ROHC_IPPROTO_MINENC:
        case ROHC_IPPROTO_HOPOPTS:
        case ROHC_IPPROTO_ROUTOPTS:
        case ROHC_IPPROTO_FRAGOPTS:
        case ROHC_IPPROTO_DSTOPTS:
        {
            *p_size += ((rohc_protocol_node_t *)p_node)->length;
            ROHC_MEMCPY(p_buf, ((rohc_protocol_node_t *)p_node)->ptr, *p_size);
            *p_buf = ((rohc_protocol_node_t *)p_node)->protocol;
            /* +- SPR 17777 */
            LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
            /* +- SPR 17777 */
            ROHC_LOG(LOG_INFO,
            "CLED:EID[%d:%d]:CID[%d]: " \
            " The calculated length of IPv6 extension header[%d] is [%d] \n", \
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
            ROHC_CSC_GET_CID(p_csc), ((rohc_protocol_node_t *)p_node)->protocol, 
            ((rohc_protocol_node_t *)p_node)->length);
            break;
        }
        case ROHC_IPPROTO_GRE:
        {
            length = 2; /* This is length of buffer to be constructed */
            p_gre_buf = ((rohc_protocol_node_t *)p_node)->ptr;
            gre_flags = ((rohc_gre_t *)p_gre_buf)->flags;
            *p_buf = ROHC_IPPROTO_GRE;
            p_buf++;
            *p_buf = 0;
            *p_buf |= (gre_flags & ROHC_GRE_VER_MASK);
            p_gre_buf += sizeof(rohc_gre_t);
            if(gre_flags & ROHC_GRE_CP)
            {
                /* Checksum present */
                *p_buf = (rohc_U8bit_t)ROHC_SET_BIT(*p_buf, 8);
                *(p_buf + length) = *p_gre_buf;
                *(p_buf + length + 1) = *(p_gre_buf + 1);
                length += 2;
                p_gre_buf += 4; 
            }
            if(gre_flags & ROHC_GRE_KP)
            {
                /* Key present */
                *p_buf = (rohc_U8bit_t)ROHC_SET_BIT(*p_buf, 6);
                *(p_buf + length) = *p_gre_buf;
                *(p_buf + length + 1) = *(p_gre_buf + 1);
                *(p_buf + length + 2) = *(p_gre_buf + 2);
                *(p_buf + length + 3) = *(p_gre_buf + 3);
                p_gre_buf += 4;
                length += 4;
            }
            if(gre_flags & ROHC_GRE_SP)
            {
                /* Sequence Present */
                *p_buf = (rohc_U8bit_t)ROHC_SET_BIT(*p_buf, 5);
                p_csc->gre_seq = ROHC_GET_4BYTES(p_gre_buf);
                *(p_buf + length) = *p_gre_buf;
                *(p_buf + length + 1) = *(p_gre_buf + 1);
                *(p_buf + length + 2) = *(p_gre_buf + 2);
                *(p_buf + length + 3) = *(p_gre_buf + 3);
                length += 4;
            }
            *p_size += length;
            ROHC_LOG(LOG_INFO,
            "CLED:EID[%d:%d]:CID[%d]: " \
            " The calculated length of GRE header [%d] \n", \
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
            ROHC_CSC_GET_CID(p_csc), length);
            break;
        }
        default:
        {
            *p_ecode = ROHC_INVALID_PROTOCOL_TYPE;
            ret = ROHC_FAILURE;
        }
    } /* end of switch */

    ROHC_EXIT_FN("rohc_c_convert_p1_in_buffer");
    return ret;
}
/*****************************************************************************
**      FUNCTION:
**          rohc_c_convert_p2_in_buffer
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to convert the items in buffer format, when
**          items go as uncompressed items
**      ARGUMENTS:
**          The item to be converted is taken as input
**      RETURN VALUE:
**          Pointer to the buffer
**      NOTES:
*****************************************************************************/
rohc_return_t    rohc_c_convert_p2_in_buffer ROHC_PARAMS
    (
        (p_entity, p_csc, p_node, p_buf, p_size, p_ecode),
            rohc_entity_t   *p_entity   _AND_
            rohc_csc_udp_t  *p_csc  _AND_
            rohc_U8bit_t    *p_node _AND_
            rohc_U8bit_t    *p_buf  _AND_
            rohc_U32bit_t   *p_size _AND_
            rohc_error_code_t   *p_ecode
    )
{
    rohc_U8bit_t    length = 0, *p_gre_buf = ROHC_NULL;
    rohc_U16bit_t   gre_flags;
    rohc_return_t   ret = ROHC_SUCCESS;

    ROHC_ENTER_FN("rohc_c_convert_p2_in_buffer");

    p_buf = p_buf + *p_size; /* Start writing the buffer from this point */

    switch(((rohc_protocol_node_t *)p_node)->protocol)
    {   
        case ROHC_IPPROTO_AH:
        {
            *p_size += ((rohc_protocol_node_t *)p_node)->length;
            ROHC_MEMCPY(p_buf, ((rohc_protocol_node_t *)p_node)->ptr, *p_size);
            *p_buf = ((rohc_protocol_node_t *)p_node)->protocol;
            p_csc->ah_seq = ((rohc_ah_t *)(((rohc_protocol_node_t *)
                            p_node)->ptr))->seq;
            /* +- SPR 17777 */
            LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
            /* +- SPR 17777 */
            ROHC_LOG(LOG_INFO,
            "CLED:EID[%d:%d]:CID[%d]: " \
            " The calculated length of AH header [%d] \n", \
            p_entity->rohc_entity_id.ueIndex,
            p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),  \
            ((rohc_protocol_node_t *)p_node)->length);
            break;
        }
        case ROHC_IPPROTO_MINENC:
        case ROHC_IPPROTO_HOPOPTS:
        case ROHC_IPPROTO_ROUTOPTS:
        case ROHC_IPPROTO_FRAGOPTS:
        case ROHC_IPPROTO_DSTOPTS:
        {
            *p_size += ((rohc_protocol_node_t *)p_node)->length;
            ROHC_MEMCPY(p_buf, ((rohc_protocol_node_t *)p_node)->ptr, *p_size);
            *p_buf = ((rohc_protocol_node_t *)p_node)->protocol;
            /* +- SPR 17777 */
            LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),0,0,0, __func__,"");
            /* +- SPR 17777 */
            ROHC_LOG(LOG_INFO,
            "CLED:EID[%d:%d]:CID[%d]: " \
            " The calculated length of IPv6 extension header[%d] is [%d] \n", \
            p_entity->rohc_entity_id.ueIndex,
            p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),  \
            ((rohc_protocol_node_t *)p_node)->protocol, 
            ((rohc_protocol_node_t *)p_node)->length);
            break;
        }
        case ROHC_IPPROTO_GRE:
        {
            length = 2; /* This is length of buffer to be constructed */
            p_gre_buf = ((rohc_protocol_node_t *)p_node)->ptr;
            gre_flags = ((rohc_gre_t *)p_gre_buf)->flags;
            *p_buf = ROHC_IPPROTO_GRE;
            p_buf++;
            *p_buf = 0;
            *p_buf |= (gre_flags & ROHC_GRE_VER_MASK);
            p_gre_buf += sizeof(rohc_gre_t);
            if(gre_flags & ROHC_GRE_CP)
            {
                /* Checksum present */
                *p_buf = (rohc_U8bit_t)ROHC_SET_BIT(*p_buf, 8);
                *(p_buf + length) = *p_gre_buf;
                *(p_buf + length + 1) = *(p_gre_buf + 1);
                length += 2;
                p_gre_buf += 4; 
            }
            if(gre_flags & ROHC_GRE_KP)
            {
                /* Key present */
                *p_buf = (rohc_U8bit_t)ROHC_SET_BIT(*p_buf, 6);
                *(p_buf + length) = *p_gre_buf;
                *(p_buf + length + 1) = *(p_gre_buf + 1);
                *(p_buf + length + 2) = *(p_gre_buf + 2);
                *(p_buf + length + 3) = *(p_gre_buf + 3);
                p_gre_buf += 4;
                length += 4;
            }
            if(gre_flags & ROHC_GRE_SP)
            {
                /* Sequence Present */
                *p_buf = (rohc_U8bit_t)ROHC_SET_BIT(*p_buf, 5);
                p_csc->gre_seq = ROHC_GET_4BYTES(p_gre_buf);
                *(p_buf + length) = *p_gre_buf;
                *(p_buf + length + 1) = *(p_gre_buf + 1);
                *(p_buf + length + 2) = *(p_gre_buf + 2);
                *(p_buf + length + 3) = *(p_gre_buf + 3);
                length += 4;
            }
            *p_size += length;
            ROHC_LOG(LOG_INFO,
            "CLED:EID[%d:%d]:CID[%d]: " \
            " The calculated length of GRE header [%d] \n", \
            p_entity->rohc_entity_id.ueIndex,
            p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),  \
            length);
            break;
        }
        default:
        {
            *p_ecode = ROHC_INVALID_PROTOCOL_TYPE;
            ret = ROHC_FAILURE;
        }
    } /* end of switch */

    ROHC_EXIT_FN("rohc_c_convert_p2_in_buffer");
    return ret;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_c_compare_p1_ext_hdr_item
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to compare two EXT HDR items.
**      ARGUMENTS:
**          The items to be compared is taken as input
**      RETURN VALUE:
**          SUCCESS/FAILURE
**      NOTES:
*****************************************************************************/
            /* +- SPR 17777 */
rohc_return_t   rohc_c_compare_p1_ext_hdr_item    ROHC_PARAMS
    (
        (p_item, p_node),
        rohc_U8bit_t        *p_item     _AND_
        rohc_list_node_t    *p_node
    )
            /* +- SPR 17777 */
{
    rohc_U32bit_t   curr_key = 0, ref_key = 0;
    rohc_U8bit_t    l_length = 0;

    ROHC_ENTER_FN("rohc_c_compare_p1_ext_hdr_item");

    if(((rohc_protocol_node_t *)p_item)->protocol !=
        ((rohc_protocol_node_t *)p_node)->protocol)
    {
        ROHC_EXIT_FN("rohc_c_compare_p1_ext_hdr_item");
        return ROHC_FAILURE;
    }
    /* This implies that they are of same type
     *  now respective fields need to be compared */

    switch(((rohc_protocol_node_t *)p_node)->protocol)
    {
        case ROHC_IPPROTO_AH:
        {
            if(((rohc_ah_t *)(((rohc_protocol_node_t *)p_item)->ptr))->spi ==
            ((rohc_ah_t *)(((rohc_protocol_node_t *)p_node)->ptr))->spi)
            {
                /* The AH header is considered as changed if SPI field changes
                 * Rest all fields are sent as-is */
                ROHC_EXIT_FN("rohc_c_compare_p1_ext_hdr_item");
                return ROHC_SUCCESS;
            }
            break;
        }
        case ROHC_IPPROTO_MINENC:
        case ROHC_IPPROTO_HOPOPTS:
        case ROHC_IPPROTO_ROUTOPTS:
        case ROHC_IPPROTO_FRAGOPTS:
        case ROHC_IPPROTO_DSTOPTS:
        {
            if(ROHC_MEMCMP(
                (((rohc_U8bit_t *)((rohc_protocol_node_t *)p_item)->ptr) + 1),
                (((rohc_U8bit_t *)((rohc_protocol_node_t *)p_node)->ptr) + 1),
                ((rohc_protocol_node_t *)p_node)->length - 1) == 0)
            {
                /* This implies items are same */
                ROHC_EXIT_FN("rohc_c_compare_p1_ext_hdr_item");
                return ROHC_SUCCESS;
            }
            break;
        }
        case ROHC_IPPROTO_GRE:
        {
            if(((rohc_gre_t *)(((rohc_protocol_node_t *)p_item)->ptr))->flags ==
            ((rohc_gre_t *)(((rohc_protocol_node_t *)p_node)->ptr))->flags)
            {
                /* This implies that items have same flags */

                /* Compare the KEY field */
                if(((rohc_gre_t *)
                  (((rohc_protocol_node_t *)p_item)->ptr))->flags & ROHC_GRE_KP)
                { /* Key present */
                    if(((rohc_gre_t *)((
                    (rohc_protocol_node_t *)p_item)->ptr))->flags & ROHC_GRE_CP)
                    {
                        l_length = sizeof(rohc_gre_t) + 4;
                    }
                    else
                    {
                        l_length = sizeof(rohc_gre_t);
                    }
                    ref_key = ROHC_GET_4BYTES((rohc_U8bit_t *)
                            (((rohc_protocol_node_t *)p_item)->ptr) + l_length);
                    curr_key = ROHC_GET_4BYTES((rohc_U8bit_t *)
                            (((rohc_protocol_node_t *)p_node)->ptr) + l_length);
                    if(ref_key == curr_key)
                    {
                        /* Key is same, implies items are same as rest of the
                         * fields go as-is, if present */ 
                        ROHC_EXIT_FN("rohc_c_compare_p1_ext_hdr_item");
                        return ROHC_SUCCESS;
                    }
                }
                else
                {   /* Key not present, hence GRE can be sent as compressed header */
                    ROHC_EXIT_FN("rohc_c_compare_p1_ext_hdr_item");
                    return ROHC_SUCCESS;
                }
            }
            else
            {
                /* Flags are not same */
            }
        } /* end of GRE case */
        break;
        default:
        {
            ROHC_EXIT_FN("rohc_c_compare_p1_ext_hdr_item");
            return ROHC_FAILURE;
        }
    } /* end of switch */
    
    ROHC_EXIT_FN("rohc_c_compare_p1_ext_hdr_item");
    return ROHC_FAILURE;
}
/*****************************************************************************
**      FUNCTION:
**          rohc_c_compare_p2_ext_hdr_item
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to compare two EXT HDR items.
**      ARGUMENTS:
**          The items to be compared is taken as input
**      RETURN VALUE:
**          SUCCESS/FAILURE
**      NOTES:
*****************************************************************************/
            /* +- SPR 17777 */
rohc_return_t   rohc_c_compare_p2_ext_hdr_item    ROHC_PARAMS
    (
        (p_item, p_node),
        rohc_U8bit_t        *p_item     _AND_
        rohc_list_node_t    *p_node
    )
            /* +- SPR 17777 */
{
    rohc_U32bit_t   curr_key = 0, ref_key = 0;
    rohc_U8bit_t    l_length = 0;

    ROHC_ENTER_FN("rohc_c_compare_p2_ext_hdr_item");

    if(((rohc_protocol_node_t *)p_item)->protocol !=
        ((rohc_protocol_node_t *)p_node)->protocol)
    {
        ROHC_EXIT_FN("rohc_c_compare_p2_ext_hdr_item");
        return ROHC_FAILURE;
    }
    /* This implies that they are of same type
     *  now respective fields need to be compared */

    switch(((rohc_protocol_node_t *)p_node)->protocol)
    {
        case ROHC_IPPROTO_AH:
        {
            if(((rohc_ah_t *)(((rohc_protocol_node_t *)p_item)->ptr))->spi ==
            ((rohc_ah_t *)(((rohc_protocol_node_t *)p_node)->ptr))->spi)
            {
                /* The AH header is considered as changed if SPI field changes
                 * Rest all fields are sent as-is */
                ROHC_EXIT_FN("rohc_c_compare_p2_ext_hdr_item");
                return ROHC_SUCCESS;
            }
            break;
        }
        case ROHC_IPPROTO_MINENC:
        case ROHC_IPPROTO_HOPOPTS:
        case ROHC_IPPROTO_ROUTOPTS:
        case ROHC_IPPROTO_FRAGOPTS:
        case ROHC_IPPROTO_DSTOPTS:
        {
            if(ROHC_MEMCMP(
                (((rohc_U8bit_t *)((rohc_protocol_node_t *)p_item)->ptr) + 1),
                (((rohc_U8bit_t *)((rohc_protocol_node_t *)p_node)->ptr) + 1),
                ((rohc_protocol_node_t *)p_node)->length - 1) == 0)
            {
                /* This implies items are same */
                ROHC_EXIT_FN("rohc_c_compare_p2_ext_hdr_item");
                return ROHC_SUCCESS;
            }
            break;
        }

        case ROHC_IPPROTO_ESP:
        {
            if(((rohc_esp_t *)(((rohc_protocol_node_t *)p_item)->ptr))->spi ==
            ((rohc_esp_t *)(((rohc_protocol_node_t *)p_node)->ptr))->spi)
            {
                /* This implies SPI field is same */
                ROHC_EXIT_FN("rohc_c_compare_p2_ext_hdr_item");
                return ROHC_SUCCESS;
            }
            break;
        }
        case ROHC_IPPROTO_GRE:
        {
            if(((rohc_gre_t *)(((rohc_protocol_node_t *)p_item)->ptr))->flags ==
            ((rohc_gre_t *)(((rohc_protocol_node_t *)p_node)->ptr))->flags)
            {
                /* This implies that items have same flags */

                /* Compare the KEY field */
                if(((rohc_gre_t *)
                 (((rohc_protocol_node_t *)p_item)->ptr))->flags & ROHC_GRE_KP)
                { /* Key present */
                    if(((rohc_gre_t *)
                    (((rohc_protocol_node_t *)p_item)->ptr))->flags & ROHC_GRE_CP)
                    {
                        l_length = sizeof(rohc_gre_t) + 4;
                    }
                    else
                    {
                        l_length = sizeof(rohc_gre_t);
                    }
                    ref_key = ROHC_GET_4BYTES((rohc_U8bit_t *)
                           (((rohc_protocol_node_t *)p_item)->ptr) + l_length);
                    curr_key = ROHC_GET_4BYTES((rohc_U8bit_t *)
                           (((rohc_protocol_node_t *)p_node)->ptr) + l_length);
                    if(ref_key == curr_key)
                    {
                        /* Key is same, implies items are same as rest of the
                         * fields go as-is, if present */ 
                        ROHC_EXIT_FN("rohc_c_compare_p2_ext_hdr_item");
                        return ROHC_SUCCESS;
                    }
                }
                else
                {   /* Key not present, hence GRE can be sent as compressed header */
                    ROHC_EXIT_FN("rohc_c_compare_p2_ext_hdr_item");
                    return ROHC_SUCCESS;
                }
            }
            else
            {
                /* Flags are not same */
            }
        } /* end of GRE case */
        break;
        default:
        {
            ROHC_EXIT_FN("rohc_c_compare_p2_ext_hdr_item");
            return ROHC_FAILURE;
        }
    } /* end of switch */
    
    ROHC_EXIT_FN("rohc_c_compare_p2_ext_hdr_item");
    return ROHC_FAILURE;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_c_compare_n_encode_p1_ext_hdr_item
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to compare two EXT HDR items and also
**          encodes Sequence number field in case of AH, GRE and ESP.
**      ARGUMENTS:
**          The items to be compared is taken as input
**      RETURN VALUE:
**          SUCCESS/FAILURE
**      NOTES:
*****************************************************************************/
            /* +- SPR 17777 */
rohc_return_t   rohc_c_compare_n_encode_p1_ext_hdr_item    ROHC_PARAMS
    (
        (p_item, p_node, p_csc, p_aseq_no,p_gseq_no),
        rohc_U8bit_t        *p_item     _AND_
        rohc_list_node_t    *p_node     _AND_
        rohc_csc_rtp_t      *p_csc      _AND_
        rohc_U32bit_t       *p_aseq_no  _AND_
        rohc_U32bit_t       *p_gseq_no
    )
            /* +- SPR 17777 */
{
    rohc_U8bit_t    len = 0;
    rohc_cpib_t     *p_cpib = ROHC_CSC_GET_CPIB(p_csc);
    rohc_U8bit_t    ret_val = ROHC_SUCCESS;

    ROHC_ENTER_FN("rohc_c_compare_n_encode_p1_ext_hdr_item");

    if(((rohc_protocol_node_t *)p_item)->protocol !=
        ((rohc_protocol_node_t *)p_node)->protocol)
    {
        ROHC_EXIT_FN("rohc_c_compare_n_encode_p1_ext_hdr_item");
        return ROHC_FAILURE;
    }
    
    /* This implies that they are of same type
     *  now respective fields need to be compared */

    switch(((rohc_protocol_node_t *)p_node)->protocol)
    {
        case ROHC_IPPROTO_AH:
        {
            if(((rohc_ah_t *)(((rohc_protocol_node_t *)p_item)->ptr))->spi ==
            ((rohc_ah_t *)(((rohc_protocol_node_t *)p_node)->ptr))->spi)
            {
                /* This implies SPI field is same
                 * Hence only the seq no needs to be compressed and sent */
                if(*p_aseq_no == 0)
                {
                    *p_aseq_no = ((rohc_ah_t *)
                        (((rohc_protocol_node_t *)p_node)->ptr))->seq;
                    rohc_led_encode_sn(&(p_csc->ah_seq), p_aseq_no);
                }
                /* Copy the AH data in CSC to be sent in the final pkt */
                len=((rohc_protocol_node_t *)p_node)->length-sizeof(rohc_ah_t);
                p_cpib->ah_data_len = len;
                p_cpib->p_ah_data = (rohc_U8bit_t *)
                    (((rohc_protocol_node_t *)p_node)->ptr) + sizeof(rohc_ah_t);
                ROHC_EXIT_FN("rohc_c_compare_n_encode_p1_ext_hdr_item");
                return ROHC_SUCCESS;
            }
            break;
        }
        case ROHC_IPPROTO_MINENC:
        case ROHC_IPPROTO_HOPOPTS:
        case ROHC_IPPROTO_ROUTOPTS:
        case ROHC_IPPROTO_FRAGOPTS:
        case ROHC_IPPROTO_DSTOPTS:
        {
            /* Start memcmp after leaving one byte of next header field */
            if(ROHC_MEMCMP(((rohc_U8bit_t *)
                (((rohc_protocol_node_t *)p_item)->ptr) + 1),
                ((rohc_U8bit_t *)(((rohc_protocol_node_t *)p_node)->ptr) + 1),
                (((rohc_protocol_node_t *)p_node)->length - 1)) == 0)
            {
                /* This implies items are same */
                ROHC_EXIT_FN("rohc_c_compare_n_encode_p1_ext_hdr_item");
                return ROHC_SUCCESS;
            }
            break;
        }
        case ROHC_IPPROTO_GRE:
        {
            ret_val = rohc_c_compare_n_encode_p1_gre_ext_hdr_item(p_csc,p_item,p_node,p_gseq_no);
            return ret_val;
        } /* end of GRE case */
        break;
    } /* end of switch */
    
    ROHC_EXIT_FN("rohc_c_compare_n_encode_p1_ext_hdr_item");
    return ROHC_FAILURE;
}
/*****************************************************************************
**      FUNCTION:
**          rohc_c_compare_n_encode_p2_ext_hdr_item
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to compare two EXT HDR items and also
**          encodes Sequence number field in case of AH, GRE and ESP.
**      ARGUMENTS:
**          The items to be compared is taken as input
**      RETURN VALUE:
**          SUCCESS/FAILURE
**      NOTES:
*****************************************************************************/
            /* +- SPR 17777 */
rohc_return_t   rohc_c_compare_n_encode_p2_ext_hdr_item    ROHC_PARAMS
    (
        (p_item, p_node, p_csc, p_aseq_no, p_gseq_no),
        rohc_U8bit_t        *p_item     _AND_
        rohc_list_node_t    *p_node     _AND_
        rohc_csc_udp_t      *p_csc      _AND_
        rohc_U32bit_t       *p_aseq_no  _AND_
        rohc_U32bit_t       *p_gseq_no 
    )
            /* +- SPR 17777 */
{
    rohc_U8bit_t    len = 0 ;
    rohc_U8bit_t    ret_val = ROHC_FAILURE;
    rohc_cpib_t     *p_cpib = ROHC_CSC_GET_CPIB(p_csc);

    ROHC_ENTER_FN("rohc_c_compare_n_encode_p2_ext_hdr_item");

    if(((rohc_protocol_node_t *)p_item)->protocol !=
        ((rohc_protocol_node_t *)p_node)->protocol)
    {
        ROHC_EXIT_FN("rohc_c_compare_n_encode_p2_ext_hdr_item");
        return ROHC_FAILURE;
    }
    
    /* This implies that they are of same type
     *  now respective fields need to be compared */

    switch(((rohc_protocol_node_t *)p_node)->protocol)
    {
        case ROHC_IPPROTO_AH:
        {
            if(((rohc_ah_t *)(((rohc_protocol_node_t *)p_item)->ptr))->spi ==
            ((rohc_ah_t *)(((rohc_protocol_node_t *)p_node)->ptr))->spi)
            {
                /* This implies SPI field is same
                 * Hence only the seq no needs to be compressed and sent */
                if(*p_aseq_no == 0)
                {
                    *p_aseq_no = ((rohc_ah_t *)
                        (((rohc_protocol_node_t *)p_node)->ptr))->seq;
                    rohc_led_encode_sn(&(p_csc->ah_seq), p_aseq_no);
                }
                /* Copy the AH data in CSC to be sent in the final pkt */
                len=((rohc_protocol_node_t *)p_node)->length-sizeof(rohc_ah_t);
                p_cpib->ah_data_len = len;
                p_cpib->p_ah_data = (rohc_U8bit_t *)
                   (((rohc_protocol_node_t *)p_node)->ptr) + sizeof(rohc_ah_t);
                ROHC_EXIT_FN("rohc_c_compare_n_encode_p2_ext_hdr_item");
                return ROHC_SUCCESS;
            }
            break;
        }
        case ROHC_IPPROTO_MINENC:
        case ROHC_IPPROTO_HOPOPTS:
        case ROHC_IPPROTO_ROUTOPTS:
        case ROHC_IPPROTO_FRAGOPTS:
        case ROHC_IPPROTO_DSTOPTS:
        {
            /* Start memcmp after leaving one byte of next header field */
            if(ROHC_MEMCMP(
                ((rohc_U8bit_t *)(((rohc_protocol_node_t *)p_item)->ptr) + 1),
                ((rohc_U8bit_t *)(((rohc_protocol_node_t *)p_node)->ptr) + 1),
                (((rohc_protocol_node_t *)p_node)->length - 1)) == 0)
            {
                /* This implies items are same */
                ROHC_EXIT_FN("rohc_c_compare_n_encode_p2_ext_hdr_item");
                return ROHC_SUCCESS;
            }
            break;
        }
        case ROHC_IPPROTO_GRE:
        {
            ret_val = rohc_c_compare_n_encode_p2_gre_ext_hdr_item(p_csc,p_item,p_node,p_gseq_no);
            return ret_val;
        } /* end of GRE case */
        break;
    } /* end of switch */
    
    ROHC_EXIT_FN("rohc_c_compare_n_encode_p2_ext_hdr_item");
    return ROHC_FAILURE;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_update_XI_list
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to update the XI List of the format
**           0   1   2   3   4   5   6   7
**           +---+---+---+---+---+---+---+---+
**           | X |           Index           |
**           +---+---+---+---+---+---+---+---+
**      ARGUMENTS:
**          The items to be compared is taken as input
**      RETURN VALUE:
**          SUCCESS/FAILURE
**      NOTES:
*****************************************************************************/

rohc_void_t rohc_update_XI_list ROHC_PARAMS
    (
    (p_XI_byte, I_value, X_value),
        rohc_U8bit_t    *p_XI_byte  _AND_
        rohc_U8bit_t    I_value     _AND_
        rohc_U8bit_t    X_value
    )
{
    ROHC_ENTER_FN("rohc_update_XI_list");
    *p_XI_byte = 0;
    *p_XI_byte = I_value;
    if(I_value > 7)
    {
        if(X_value)
            *p_XI_byte = (rohc_U8bit_t)ROHC_SET_BIT(*p_XI_byte, 8);
    }
    else
    {
        if(X_value)
            *p_XI_byte = (rohc_U8bit_t)ROHC_SET_BIT(*p_XI_byte, 4);
    }
    ROHC_EXIT_FN("rohc_update_XI_list");
}   

/*****************************************************************************
**      FUNCTION:
**          rohc_p6_update_XI_list
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to update the XI List of the format
**               +---+---+---+---+
         PS = 0: | X |   Index   |
                 +---+---+---+---+

                   0   1   2   3   4   5   6   7
                 +---+---+---+---+---+---+---+---+
         PS = 1: | X | Reserved  |     Index     |
                 +---+---+---+---+---+---+---+---+

         X: Indicates whether the item is present in the list:
**      ARGUMENTS:
**          The items to be compared is taken as input
**      RETURN VALUE:
**          SUCCESS/FAILURE
**      NOTES:
*****************************************************************************/

rohc_void_t rohc_p6_update_XI_list ROHC_PARAMS
    (
    (p_XI_byte, I_value, X_value, PS_flag),
        rohc_U8bit_t    *p_XI_byte  _AND_
        rohc_U8bit_t    I_value     _AND_
        rohc_U8bit_t    X_value     _AND_
        rohc_bool_t     PS_flag
    )
{
    ROHC_ENTER_FN("rohc_p6_update_XI_list");
    *p_XI_byte = 0;
    *p_XI_byte = I_value;
    if(PS_flag)
    {
        if(X_value)
            *p_XI_byte = (rohc_U8bit_t)ROHC_SET_BIT(*p_XI_byte, 8);
    }
    else
    {
        if(X_value)
            *p_XI_byte = (rohc_U8bit_t)ROHC_SET_BIT(*p_XI_byte, 4);
    }
    ROHC_EXIT_FN("rohc_p6_update_XI_list");
}

/*****************************************************************************
**      FUNCTION:
**          rohc_compare_prev_list
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to 2 entries in the Sliding Window
**      ARGUMENTS:
**          The entry in SW and new item list are inputs
**      RETURN VALUE:
**          SUCCESS/FAILURE
**      NOTES:
*****************************************************************************/

rohc_return_t   rohc_compare_prev_list  ROHC_PARAMS
    (
    (sw_entry, index_count, index_list),
    rohc_uo_sw_t    sw_entry    _AND_
    rohc_U8bit_t    index_count _AND_
    rohc_U8bit_t    *index_list
    )
{
    rohc_U32bit_t       i = 0;
    ROHC_ENTER_FN("rohc_compare_prev_list");
    if(index_count != sw_entry.icount)  return ROHC_FAILURE;    
    for(i=0; i<index_count; i++)
    {
        if(index_list[i] != sw_entry.index_list[i])
        {
            /* Items are not same, hence return failure */
            ROHC_EXIT_FN("rohc_compare_prev_list");
            return ROHC_FAILURE;
        }
    }
    ROHC_EXIT_FN("rohc_compare_prev_list");
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_generate_gen_id
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to generate gen_id in U/O Mode
**          in case it is an context updating packet.
**      ARGUMENTS:
**          The previous entry in SW is taken as input
**      RETURN VALUE:
**          new gen_id
**      NOTES:
*****************************************************************************/

rohc_U8bit_t    rohc_generate_gen_id    ROHC_PARAMS
    (
    (p_sw_prev_entry),
    rohc_uo_sw_t    *p_sw_prev_entry
    )
{
    rohc_U8bit_t    gen_id = p_sw_prev_entry->gen_id;
    gen_id++;
    
    return gen_id;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_cled_p1_ack_handling
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to handle the ACK message
**      ARGUMENTS:
**          The CSC block is taken as input
**      RETURN VALUE:
**          SUCCESS/FAILURE
**      NOTES:
*****************************************************************************/
            /* +- SPR 17777 */
rohc_return_t   rohc_cled_p1_ack_handling  ROHC_PARAMS
(   (p_csc, rtp_sn),   
    rohc_csc_rtp_t      *p_csc  _AND_   
    rohc_U16bit_t       rtp_sn
)
            /* +- SPR 17777 */
{
    ROHC_ENTER_FN("rohc_cled_p1_ack_handling");
    if(p_csc->c_mode == ROHC_MODE_R)
    {
        rohc_cled_p1_r_ack_handling(p_csc,rtp_sn);
    }
    else
    {   /* U/O Mode */
        rohc_U8bit_t    count = 0, sn_count = 0;
        rohc_U32bit_t i=0;
        /* In U/O mode also ACK can be received and can make a list as 
         * reference list. But to identify that list, gen_id is required 
         * and there is no placeholder in the Feedback message to communicate
         * the gen_id of the list. */

        /* Solution -- 
         * Keep Sequence number list along with gen_id in the SW. Kindly
         * note that specs does not convey anything like that. Also it will
         * have a huge impact on the performance as the SN list in whole SW 
         * needs to be checked for the right entry. */

        for(count=p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_ref_index;
                count <= p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_next_free_index;
                count++)
        {
            for(sn_count=0; sn_count<p_csc->rohc_c_uo_ext_hdr_sw.
                    uo_ext_hdr_sw[count].gen_count; sn_count++)
            {
                if(rtp_sn == p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw[count].
                        sn_no_list[sn_count])
                {
                    break;
                }
            } /* end of SN loop */

            if(sn_count != p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw[count].
                    gen_count)
            {
                /* Indicates SN is found */
                /* Set the known field to 1 of the items in the TT */
                for(i=0; 
                        i<p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw[count].icount;
                        i++)
                {
                    p_csc->rohc_c_uo_ext_hdr_tt.uo_ext_hdr_tt[i].known = 
                        ROHC_TRUE;
                }
                p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_ref_index = count;
                p_csc->rohc_c_uo_ext_hdr_sw.ref_id_present = ROHC_TRUE;
                break;
            }
        }   /* end of for */

        /* CSRC list might be present */
        for(count=p_csc->rohc_c_uo_csrc_sw.uo_csrc_ref_index;
                count <= p_csc->rohc_c_uo_csrc_sw.uo_csrc_next_free_index;
                count++)
        {
            for(sn_count=0; 
                    sn_count<p_csc->rohc_c_uo_csrc_sw.uo_csrc_sw[count].
                    gen_count; sn_count++)
            {
                if(rtp_sn == p_csc->rohc_c_uo_csrc_sw.uo_csrc_sw[count].
                        sn_no_list[sn_count])
                {
                    break;
                }
            } /* end of SN loop */

            if(sn_count != p_csc->rohc_c_uo_csrc_sw.uo_csrc_sw[count].gen_count)
            {
                /* Indicates SN is found */
                /* Set the known field to 1 of the items in the TT */
                for(i=0;
                        i<p_csc->rohc_c_uo_csrc_sw.uo_csrc_sw[count].icount;
                        i++)
                {
                    p_csc->rohc_c_uo_csrc_tt.uo_csrc_tt[i].known =
                        ROHC_TRUE;
                }
                p_csc->rohc_c_uo_csrc_sw.uo_csrc_ref_index = count;
                p_csc->rohc_c_uo_csrc_sw.ref_id_present = ROHC_TRUE;
                break;
            }
        }   /* end of for */
    }   /* end of U/O Mode */
    ROHC_EXIT_FN("rohc_cled_p1_ack_handling");
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_cled_p1_r_ack_handling
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to handle the ACK message in R mode
**      ARGUMENTS:
**          The CSC block is taken as input
**      RETURN VALUE:
**          SUCCESS/FAILURE
**      NOTES:
*****************************************************************************/
static rohc_void_t   rohc_cled_p1_r_ack_handling  ROHC_PARAMS
(   (p_csc, rtp_sn),   
    rohc_csc_rtp_t      *p_csc  _AND_   
    rohc_U16bit_t       rtp_sn 
)
{
    rohc_U8bit_t    count = 0 ;
	rohc_U32bit_t i=0;
    ROHC_ENTER_FN("rohc_cled_p1_r_ack_handling");
        /* Make the entry in the SW corrosponding to that SN as ref list
         * for both extension header SW and CSRC list SW 
         * Also the known field of all the items in that ref list will
         * be set as 1 */
        
        /* Extension header list */
        for(count = p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_ref_index;
            count <= p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_next_free_index;
            count++)
        {
            if(rtp_sn==p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_sw[count].rtp_sn_no)
            {
                /* Entry found for the SN */
                break;
            }
        }   /* end of for */
        if(count == p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_next_free_index)
        {
            /* May be extension list was not present in that header */
        }
        else
        {
            p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_ref_index = count;
            p_csc->rohc_c_r_ext_hdr_sw.ref_id_present = ROHC_TRUE;
            /* Setting all known fields to 1 */
            for(i = 0; 
                i<p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_sw[count].icount; 
                i++)
            {
                p_csc->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt
                [p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_sw[count].
                index_list[i]].r_ext_hdr_tt.known = ROHC_TRUE;
            }
        }

        /* CSRC list can be present */
        for(count = p_csc->rohc_c_r_csrc_sw.r_csrc_ref_index;
            count <= p_csc->rohc_c_r_csrc_sw.r_csrc_next_free_index;
            count++)
        {
            if(rtp_sn==p_csc->rohc_c_r_csrc_sw.r_csrc_sw[count].rtp_sn_no)
            {
                /* Entry found for the SN */
                break;
            }
        } /* end of for */
        if(count == p_csc->rohc_c_r_csrc_sw.r_csrc_next_free_index)
        {
            /* May be extension list was not present in that header */
        }
        else
        {
            p_csc->rohc_c_r_csrc_sw.r_csrc_ref_index = count;
            p_csc->rohc_c_r_csrc_sw.ref_id_present = ROHC_TRUE;
            /* Setting all known fields to 1 */
            for(i = 0;i<p_csc->rohc_c_r_csrc_sw.r_csrc_sw[count].icount;
                 i++)
            {
                p_csc->rohc_c_r_csrc_tt.r_csrc_tt
                [p_csc->rohc_c_r_csrc_sw.r_csrc_sw
                [count].index_list[i]].r_csrc_tt.known = ROHC_TRUE;
            }
        }
}

/*****************************************************************************
**      FUNCTION:
**          rohc_cled_p2_ack_handling
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to handle the ACK message
**      ARGUMENTS:
**          The CSC block is taken as input
**      RETURN VALUE:
**          SUCCESS/FAILURE
**      NOTES:
*****************************************************************************/
            /* +- SPR 17777 */
rohc_return_t   rohc_cled_p2_ack_handling  ROHC_PARAMS
(   (p_csc, rtp_sn),   
    rohc_csc_udp_t  *p_csc  _AND_   
    rohc_U16bit_t   rtp_sn
)
            /* +- SPR 17777 */
{
    rohc_U8bit_t    count = 0, sn_count = 0;
	rohc_U32bit_t i=0;
    ROHC_ENTER_FN("rohc_cled_p2_ack_handling");
    if(p_csc->c_mode == ROHC_MODE_R)
    {
        /* Make the entry in the SW corrosponding to that SN as ref list
         * for both extension header SW and CSRC list SW 
         * Also the known field of all the items in that ref list will
         * be set as 1 */
        
        /* Extension header list */
        /* Fix start SPR#2179 2191 */
        for(count = p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_ref_index;
            count < p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_next_free_index;
            count++) /* Fix End SPR#2179 2191 */
        {
            if(rtp_sn==p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_sw[count].rtp_sn_no)
            {
                /* Entry found for the SN */
                break;
            }
        }   /* end of for */
        if(count == p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_next_free_index)
        {
            /* May be extension list was not present in that header */
        }
        else
        {
            p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_ref_index = count;
            p_csc->rohc_c_r_ext_hdr_sw.ref_id_present = ROHC_TRUE;
            /* Setting all known fields to 1 */
            for(i = 0; 
                i<p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_sw[count].icount; 
                i++)
            {
                p_csc->rohc_c_r_ext_hdr_tt.r_ext_hdr_tt
                [p_csc->rohc_c_r_ext_hdr_sw.r_ext_hdr_sw[count].
                index_list[i]].r_ext_hdr_tt.known = ROHC_TRUE;
            }
        }
    }
    else
    {   /* U/O Mode */
        /* In U/O mode also ACK can be received and can make a list as 
         * reference list. But to identify that list, gen_id is required 
         * and there is no placeholder in the Feedback message to communicate
         * the gen_id of the list. */

        /* Solution -- 
         * Keep Sequence number list along with gen_id in the SW. Kindly
         * note that specs does not convey anything like that. Also it will
         * have a huge impact on the performance as the SN list in whole SW 
         * needs to be checked for the right entry. */
        /* Fix start SPR#2179 2191 */
        for(count=p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_ref_index;
            count < p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_next_free_index;
            count++) /* Fix End SPR#2179 2191 */
        {
            for(sn_count=0; sn_count<p_csc->rohc_c_uo_ext_hdr_sw.
                    uo_ext_hdr_sw[count].gen_count; sn_count++)
            {
                if(rtp_sn == p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw[count].
                        sn_no_list[sn_count])
                {
                    break;
                }
            } /* end of SN loop */

            if(sn_count != p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw[count].
                         gen_count)
            {
                /* Indicates SN is found */
                /* Set the known field to 1 of the items in the TT */
                for(i=0; 
                    i<p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw[count].icount;
                    i++)
                {
                    p_csc->rohc_c_uo_ext_hdr_tt.uo_ext_hdr_tt[i].known = 
                        ROHC_TRUE;
                }
                p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_ref_index = count;
                p_csc->rohc_c_uo_ext_hdr_sw.ref_id_present = ROHC_TRUE;
                break;
            }
        }   /* end of for */

    }   /* end of U/O Mode */
    ROHC_EXIT_FN("rohc_cled_p2_ack_handling");
    return ROHC_SUCCESS;
}

/* Decompressor utility functions */
/*****************************************************************************
**      FUNCTION:
**          rohc_led_p1_init_inneripext
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to initialize elements for Inner IP Extension
**          header in DPIB
**      ARGUMENTS:
**          The DPIB block is taken as input
**      RETURN VALUE:
**          void
**      NOTES:
*****************************************************************************/

rohc_void_t     rohc_led_p1_init_inneripext        ROHC_PARAMS
((p_dpib), rohc_dpib_t  *p_dpib)
{

    ROHC_ENTER_FN("rohc_led_p1_init_inneripext");

    /* Initialize Inner IP ext List */
    rohc_db_create_list(&(p_dpib->inneripextlist));
    p_dpib->inneripext_new_item_count = 0;
    p_dpib->inneripext_r_sw.icount = 0;
    p_dpib->inneripext_uo_sw.icount = 0;
    ROHC_MEMSET(p_dpib->inneripext_tt, 0, 
        (ROHC_MAX_ENTRIES_IN_TT * sizeof(rohc_decomp_ext_hdr_tt_t)));
    ROHC_MEMSET(p_dpib->inneripext_new_index_list, 0, ROHC_MAX_ENTRIES_IN_TT);

    ROHC_EXIT_FN("rohc_led_p1_init_inneripext");
}

/* Decompressor utility functions */
/*****************************************************************************
**      FUNCTION:
**          rohc_led_p2_init_inneripext
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to initialize elements for Inner IP Extension
**          header in DPIB
**      ARGUMENTS:
**          The DPIB block is taken as input
**      RETURN VALUE:
**          void
**      NOTES:
*****************************************************************************/

rohc_void_t     rohc_led_p2_init_inneripext        ROHC_PARAMS
((p_dpib), rohc_dpib_t  *p_dpib)
{

    ROHC_ENTER_FN("rohc_led_p2_init_inneripext");

    /* Initialize Inner IP ext List */
    rohc_db_create_list(&(p_dpib->inneripextlist));
    p_dpib->inneripext_new_item_count = 0;
    p_dpib->inneripext_r_sw.icount = 0;
    p_dpib->inneripext_uo_sw.icount = 0;
    ROHC_MEMSET(p_dpib->inneripext_tt, 0, 
        (ROHC_MAX_ENTRIES_IN_TT * sizeof(rohc_decomp_ext_hdr_tt_t)));

    ROHC_MEMSET(p_dpib->inneripext_new_index_list, 0, 
        ROHC_MAX_ENTRIES_IN_TT);

    ROHC_EXIT_FN("rohc_led_p2_init_inneripext");
}

/*****************************************************************************
**      FUNCTION:
**          rohc_led_p6_init_inneripext
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to initialize elements for Inner IP Extension
**          header in DPIB
**      ARGUMENTS:
**          The DPIB block is taken as input
**      RETURN VALUE:
**          void
**      NOTES:
*****************************************************************************/
#ifdef ROHC_FUNC_ENHANCEMENT  
rohc_void_t rohc_led_p6_init_inneripext(rohc_dpib_t  *p_dpib)
{
    ROHC_ENTER_FN("rohc_led_p6_init_inneripext");

    /* Initialize Inner IP ext List */
    rohc_db_create_list(&(p_dpib->inneripextlist));
    p_dpib->inneripext_new_item_count = 0;
    p_dpib->inneripext_r_sw.icount = 0;
    p_dpib->inneripext_uo_sw.icount = 0;
    ROHC_MEMSET(p_dpib->inneripext_tt, 0, 
        (ROHC_MAX_ENTRIES_IN_TT * sizeof(rohc_decomp_ext_hdr_tt_t)));

    ROHC_MEMSET(p_dpib->inneripext_new_index_list, 0, 
        ROHC_MAX_ENTRIES_IN_TT);

    ROHC_EXIT_FN("rohc_led_p6_init_inneripext");
}
#endif

/*****************************************************************************
**      FUNCTION:
**          rohc_led_p1_init_outeripext
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to initialize elements for OUTER IP Extension
**          header in DPIB
**      ARGUMENTS:
**          The DPIB block is taken as input
**      RETURN VALUE:
**         void
**      NOTES:
*****************************************************************************/

rohc_void_t     rohc_led_p1_init_outeripext        ROHC_PARAMS
((p_dpib), rohc_dpib_t  *p_dpib)
{
    ROHC_ENTER_FN("rohc_led_p1_init_outeripext");

    /* Initialize Outer IP ext List */
    rohc_db_create_list(&(p_dpib->outeripextlist));
    p_dpib->outeripext_new_item_count = 0;
    p_dpib->outeripext_r_sw.icount = 0;
    p_dpib->outeripext_uo_sw.icount = 0;
    ROHC_MEMSET(p_dpib->outeripext_tt, 0, 
    (ROHC_MAX_ENTRIES_IN_TT * sizeof(rohc_decomp_ext_hdr_tt_t)));
    ROHC_MEMSET(p_dpib->outeripext_new_index_list, 0, ROHC_MAX_ENTRIES_IN_TT);

    ROHC_EXIT_FN("rohc_led_p1_init_outeripext");
}

/*****************************************************************************
**      FUNCTION:
**          rohc_led_p2_init_outeripext
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to initialize elements for OUTER IP Extension
**          header in DPIB
**      ARGUMENTS:
**          The DPIB block is taken as input
**      RETURN VALUE:
**         void
**      NOTES:
*****************************************************************************/

rohc_void_t     rohc_led_p2_init_outeripext        ROHC_PARAMS
((p_dpib), rohc_dpib_t  *p_dpib)
{
    ROHC_ENTER_FN("rohc_led_p2_init_outeripext");

    /* Initialize Outer IP ext List */
    rohc_db_create_list(&(p_dpib->outeripextlist));
    p_dpib->outeripext_new_item_count = 0;
    p_dpib->outeripext_r_sw.icount = 0;
    p_dpib->outeripext_uo_sw.icount = 0;
    ROHC_MEMSET(p_dpib->outeripext_tt, 0, 
        (ROHC_MAX_ENTRIES_IN_TT * sizeof(rohc_decomp_ext_hdr_tt_t)));
    ROHC_MEMSET(p_dpib->outeripext_new_index_list, 0, 
        ROHC_MAX_ENTRIES_IN_TT);

    ROHC_EXIT_FN("rohc_led_p2_init_outeripext");
}

/*****************************************************************************
**      FUNCTION:
**          rohc_led_p1_init_csrc
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to initialize elements for CSRC List in DPIB
**      ARGUMENTS:
**          The DPIB block is taken as input
**      RETURN VALUE:
**          void
**      NOTES:
*****************************************************************************/

rohc_void_t     rohc_led_p1_init_csrc      ROHC_PARAMS
((p_dpib), rohc_dpib_t  *p_dpib)
{
    ROHC_ENTER_FN("rohc_led_p1_init_csrc");
    p_dpib->csrc_new_item_count = 0;
    p_dpib->csrc_r_sw.icount = 0;
    p_dpib->csrc_uo_sw.icount = 0;
    ROHC_MEMSET(p_dpib->csrc_tt, 0, 
                ROHC_MAX_ENTRIES_IN_CSRC_TT * sizeof(rohc_decomp_csrc_tt_t));
    ROHC_MEMSET(p_dpib->final_csrc_list, 0, ROHC_MAX_ENTRIES_IN_CSRC_TT * 4);
    ROHC_MEMSET(p_dpib->csrc_new_index_list, 0, ROHC_MAX_ENTRIES_IN_CSRC_TT);
    ROHC_EXIT_FN("rohc_led_p1_init_csrc");
}

/*****************************************************************************
**      FUNCTION:
**          rohc_d_store_item_p1_in_list
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to store the item in item list
**          The first octet of p_buf is always next header field which tells the
**          protocol type and accordingly the length can be calculated. This
**          functions hold true only when uncompressed item is received in a 
**          list. If the item is AH or GRE then their pointers also 
**          needs to be modified. 
**
**          The ptr field in rohc_protocol_node_t should be the uncompressed IP
**          datagram packet which needs to be sent to the upper layer and 
**          not the one which is received from the compressor
**      ARGUMENTS:
**          The item, buffer and AH, GRE pointers are input
**      RETURN VALUE:
**          void
**      NOTES:
**          p_ext_hdr_tt->p_item is of type rohc_protocol_node_t 
**          p_length is length of compressed buffer
*****************************************************************************/

rohc_return_t   rohc_d_store_item_p1_in_list   ROHC_PARAMS
(   (p_dsc, p_ext_hdr_tt, p_buf, p_length, p_p_ah, p_p_gre, p_ecode),
    rohc_dsc_rtp_t              *p_dsc          _AND_
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt   _AND_
    rohc_U8bit_t                *p_buf          _AND_
    rohc_U32bit_t               *p_length       _AND_
    rohc_ah_t                   **p_p_ah        _AND_
    rohc_gre_t                  **p_p_gre       _AND_
    rohc_error_code_t           *p_ecode
)
{
    rohc_U8bit_t    protocol; 
    rohc_U32bit_t   length = 0;
    rohc_dpib_t     *p_dpib = ROHC_DSC_GET_DPIB(p_dsc);
    rohc_return_t   ret = ROHC_SUCCESS;
    rohc_minenc_t   *p_minenc;

    ROHC_ENTER_FN("rohc_d_store_item_p1_in_list");
    protocol = *p_buf;
    
    /* Check if this entry of TT already contains an entry
     * If yes, then that pointer itself can be utilized and need not be 
     * malloced again */

    if(p_ext_hdr_tt->p_item == ROHC_NULL)
    {
        p_ext_hdr_tt->p_item = 
            (rohc_U8bit_t *)ROHC_MALLOC(sizeof(rohc_protocol_node_t));

        if(ROHC_NULL == p_ext_hdr_tt->p_item)
        {
            *p_ecode = ROHC_MEMORY_FAILURE;
            ROHC_EXIT_FN("rohc_d_store_item_p1_in_list");
            return ROHC_FAILURE;
        }
    }
    else
    {
        /* If ptr points to any buffer then free it first and then
         * malloc again */
        if(((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr !=
            ROHC_NULL)
        {
           ROHC_FREE(((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr);
        }
    }


    switch(protocol)
    {
        case ROHC_IPPROTO_AH:
        {
            length = (*(p_buf + 1) + 2) * 4;
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->length = length;
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->protocol = 
                ROHC_IPPROTO_AH;
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr = 
                ROHC_MALLOC(length);
            if(((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr ==
                ROHC_NULL)
            {
                *p_ecode = ROHC_MEMORY_FAILURE;
                ROHC_EXIT_FN("rohc_d_store_item_p1_in_list");
                return ROHC_FAILURE;
            }
            ROHC_MEMCPY(((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr, 
                p_buf, length);
            *p_p_ah = (rohc_ah_t *)
                        (((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr);
            p_dpib->decoded_ah_seq = ((rohc_ah_t *)(*p_p_ah))->seq;
            *p_length = length;
        }       
        break;
        case ROHC_IPPROTO_GRE:
        {
            if(ROHC_FAILURE == rohc_d_store_item_gre_in_list(p_ext_hdr_tt,
                p_buf,p_length,p_p_gre,p_ecode))
            {
                return ROHC_FAILURE;
            }
        }
        break;
        case ROHC_IPPROTO_HOPOPTS:
        case ROHC_IPPROTO_DSTOPTS:
        case ROHC_IPPROTO_ROUTOPTS:
        {
            length = (*(p_buf+1) + 1) << 3;
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->length = length;
            ((rohc_protocol_node_t *)
                                (p_ext_hdr_tt->p_item))->protocol = protocol;
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr =
                ROHC_MALLOC(length);
            if(((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr == ROHC_NULL)
            {
                *p_ecode = ROHC_MEMORY_FAILURE;
                ROHC_EXIT_FN("rohc_d_store_item_p1_in_list");
                return ROHC_FAILURE;
            }
            ROHC_MEMCPY(((rohc_protocol_node_t *)
                                (p_ext_hdr_tt->p_item))->ptr, p_buf, length);
            *p_length = length;
        }
        break;
        case ROHC_IPPROTO_MINENC:
        {
            p_minenc = (rohc_minenc_t *)p_buf;
            if(p_minenc->osrc_flag & ROHC_OSRC_PRES)
                length = sizeof(rohc_minenc_t);
            else
                length = sizeof(rohc_minenc_t) - 4;
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->length = length;
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->protocol = 
                                                                     protocol;
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr =
                ROHC_MALLOC(length);
            if(((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr == 
                                                                    ROHC_NULL)
            {
                *p_ecode = ROHC_MEMORY_FAILURE;
                ROHC_EXIT_FN("rohc_d_store_item_p1_in_list");
                return ROHC_FAILURE;
            }
            ROHC_MEMCPY(((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr, 
                                                                p_buf, length);
            *p_length = length;
        }
        break;
        case ROHC_IPPROTO_FRAGOPTS:
        {
            length = sizeof(rohc_fragopts_t);
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->length = length;
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->protocol = 
                                                                      protocol;
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr =
                ROHC_MALLOC(length);
            if(((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr == 
                                                                     ROHC_NULL)
            {
                *p_ecode = ROHC_MEMORY_FAILURE;
                ROHC_EXIT_FN("rohc_d_store_item_p1_in_list");
                return ROHC_FAILURE;
            }
            ROHC_MEMCPY(((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr, 
                                                                p_buf, length);
            *p_length = length;
        }
        break;
        default:
        {
            *p_ecode = ROHC_INVALID_PROTOCOL_TYPE;
            ret = ROHC_FAILURE;
        }
    } /* end of switch */
    ROHC_EXIT_FN("rohc_d_store_item_p1_in_list");
    return ret;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_d_store_item_p2_in_list
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to store the item in item list
**          The first octet of p_buf is always next header field which tells the
**          protocol type and accordingly the length can be calculated. This
**          functions hold true only when uncompressed item is received in a 
**          list. If the item is AH or GRE then their pointers also 
**          needs to be modified. 
**
**          The ptr field in rohc_protocol_node_t should be the uncompressed IP
**          datagram packet which needs to be sent to the upper layer and 
**          not the one which is received from the compressor
**      ARGUMENTS:
**          The item, buffer and AH, GRE pointers are input
**      RETURN VALUE:
**          void
**      NOTES:
**          p_ext_hdr_tt->p_item is of type rohc_protocol_node_t 
**          p_length is length of compressed buffer
*****************************************************************************/

rohc_return_t   rohc_d_store_item_p2_in_list   ROHC_PARAMS
(   (p_dsc, p_ext_hdr_tt, p_buf, p_length, p_p_ah, p_p_gre, p_ecode),
    rohc_dsc_udp_t              *p_dsc          _AND_
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt   _AND_
    rohc_U8bit_t                *p_buf          _AND_
    rohc_U32bit_t               *p_length       _AND_
    rohc_ah_t                   **p_p_ah        _AND_
    rohc_gre_t                  **p_p_gre       _AND_
    rohc_error_code_t           *p_ecode
)
{
    rohc_U8bit_t    protocol; 
    rohc_U32bit_t   length = 0;
    rohc_esp_t      *p_esp;
    rohc_dpib_t *p_dpib = ROHC_DSC_GET_DPIB(p_dsc);
    rohc_return_t   ret = ROHC_SUCCESS;
    rohc_minenc_t   *p_minenc;

    ROHC_ENTER_FN("rohc_d_store_item_p2_in_list");
    protocol = *p_buf;
    
    /* Check if this entry of TT already contains an entry
     * If yes, then that pointer itself can be utilized and need not be 
     * malloced again */

    if(p_ext_hdr_tt->p_item == ROHC_NULL)
    {
        p_ext_hdr_tt->p_item = 
            (rohc_U8bit_t *)ROHC_MALLOC(sizeof(rohc_protocol_node_t));

        if(ROHC_NULL == p_ext_hdr_tt->p_item)
        {
            *p_ecode = ROHC_MEMORY_FAILURE;
            ROHC_EXIT_FN("rohc_d_store_item_p2_in_list");
            return ROHC_FAILURE;
        }
    }
    else
    {
        /* If ptr points to any buffer then free it first and then
         * malloc again */
        if(((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr !=
            ROHC_NULL)
        {
           ROHC_FREE(((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr);
        }
    }


    switch(protocol)
    {
        case ROHC_IPPROTO_AH:
        {
            length = (*(p_buf + 1) + 2) * 4;
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->length = length;
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->protocol = 
                ROHC_IPPROTO_AH;
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr = 
                ROHC_MALLOC(length);
            if(((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr ==
                ROHC_NULL)
            {
                *p_ecode = ROHC_MEMORY_FAILURE;
                ROHC_EXIT_FN("rohc_d_store_item_p2_in_list");
                return ROHC_FAILURE;
            }
            ROHC_MEMCPY(((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr, 
                p_buf, length);
            *p_p_ah = (rohc_ah_t *)
                        (((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr);
            p_dpib->decoded_ah_seq = ((rohc_ah_t *)(*p_p_ah))->seq;
            *p_length = length;
        }       
        break;
        case ROHC_IPPROTO_ESP:
        {
            length = sizeof(rohc_esp_t); /* ??????????? */
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->length = length;
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->protocol = 
                    ROHC_IPPROTO_ESP;
            p_esp = ROHC_MALLOC(length);
            if(p_esp == ROHC_NULL)
            {
                *p_ecode = ROHC_MEMORY_FAILURE;
                ROHC_EXIT_FN("rohc_d_store_item_p2_in_list");
                return ROHC_FAILURE;
            }
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr = p_esp;
            p_esp->spi = ROHC_GET_4BYTES(p_buf + 1);
            p_esp->seq = ROHC_GET_4BYTES(p_buf + 5);
            p_dpib->decoded_esp_seq = p_esp->seq;
            *p_length = length + 1;
        }
        break;
        case ROHC_IPPROTO_GRE:
        {
            if(ROHC_FAILURE == rohc_d_store_item_gre_in_list(p_ext_hdr_tt,
                p_buf,p_length,p_p_gre,p_ecode))
            {
                return ROHC_FAILURE;
            }
        }
        break;
        case ROHC_IPPROTO_HOPOPTS:
        case ROHC_IPPROTO_DSTOPTS:
        case ROHC_IPPROTO_ROUTOPTS:
        {
            length = (*(p_buf+1) + 1) << 3;
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->length = length;
            ((rohc_protocol_node_t *)
                                (p_ext_hdr_tt->p_item))->protocol = protocol;
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr =
                ROHC_MALLOC(length);
            if(((rohc_protocol_node_t *)
                    (p_ext_hdr_tt->p_item))->ptr == ROHC_NULL)
            {
                *p_ecode = ROHC_MEMORY_FAILURE;
                ROHC_EXIT_FN("rohc_d_store_item_p2_in_list");
                return ROHC_FAILURE;
            }
            ROHC_MEMCPY(((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr,
                                                              p_buf, length);
            *p_length = length;
        }
        break;
        case ROHC_IPPROTO_MINENC:
        {
            p_minenc = (rohc_minenc_t *)p_buf;
            if(p_minenc->osrc_flag & ROHC_OSRC_PRES)
                length = sizeof(rohc_minenc_t);
            else
                length = sizeof(rohc_minenc_t) - 4;
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->length = length;
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->protocol = 
                                                                    protocol;
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr =
                ROHC_MALLOC(length);
            if(((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr == 
                                                                    ROHC_NULL)
            {
                *p_ecode = ROHC_MEMORY_FAILURE;
                ROHC_EXIT_FN("rohc_d_store_item_p2_in_list");
                return ROHC_FAILURE;
            }
            ROHC_MEMCPY(((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr,
                                                                p_buf, length);
            *p_length = length;
        }
        break;
        case ROHC_IPPROTO_FRAGOPTS:
        {
            length = sizeof(rohc_fragopts_t);
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->length = length;
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->protocol = 
                                                                    protocol;
            ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr =
                ROHC_MALLOC(length);
            if(((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr == 
                                                                    ROHC_NULL)
            {
                *p_ecode = ROHC_MEMORY_FAILURE;
                ROHC_EXIT_FN("rohc_d_store_item_p2_in_list");
                return ROHC_FAILURE;
            }
            ROHC_MEMCPY(((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr,
                                                                p_buf, length);
            *p_length = length;
        }
        break;
        default:
        {
            *p_ecode = ROHC_INVALID_PROTOCOL_TYPE;
            ret = ROHC_FAILURE;
        }
    } /* end of switch */
    ROHC_EXIT_FN("rohc_d_store_item_p2_in_list");
    return ret;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_d_add_node_in_list
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to add an item in the item list
**          In this function we set the next header field of the last node
**          in the list to the protocol value passed and also adds the node
**          at the end of the list
**      ARGUMENTS:
**          The list and the node to be added are taken as input
**      RETURN VALUE:
**          void
**      NOTES:
*****************************************************************************/

rohc_return_t       rohc_d_add_node_in_list     ROHC_PARAMS
(   (p_ext_hdr_list, p_ext_hdr_node, p_ecode),
    rohc_list_t             *p_ext_hdr_list     _AND_
    rohc_protocol_node_t    *p_ext_hdr_node     _AND_
    rohc_error_code_t       *p_ecode
)
{
    rohc_U8bit_t            protocol = p_ext_hdr_node->protocol;
    rohc_protocol_node_t    *p_last_node;

    ROHC_ENTER_FN("rohc_d_add_node_in_list");
    p_last_node = (rohc_protocol_node_t *)rohc_db_get_last_node(p_ext_hdr_list);
    
    if(p_last_node != ROHC_NULL)
    {
        switch(p_last_node->protocol)
        {
            case ROHC_IPPROTO_AH:
            case ROHC_IPPROTO_MINENC:
            case ROHC_IPPROTO_HOPOPTS:
            case ROHC_IPPROTO_DSTOPTS:
            case ROHC_IPPROTO_FRAGOPTS:
            case ROHC_IPPROTO_ROUTOPTS:
            {   
                /* Point the next header field to the protocol field passed 
                 * In these headers next header field is always first byte */
                *((rohc_U8bit_t *)(p_last_node->ptr)) = protocol;
            }
            break;
            case ROHC_IPPROTO_GRE:
            {
                ((rohc_gre_t *)(p_last_node->ptr))->proto = protocol;
                ((rohc_gre_t *)(p_last_node->ptr))->proto = 
                        ROHC_HTONS(((rohc_gre_t *)(p_last_node->ptr))->proto);
            }
            /* ESP does not have a next header field */
            break;
            default:
            {
                *p_ecode = ROHC_INVALID_PROTOCOL_TYPE;
                ROHC_EXIT_FN("rohc_d_add_node_in_list");
                return ROHC_FAILURE;
            }
        } /* end of switch */
    }

    /* Now add the node in the list */
    rohc_db_add_last_node(p_ext_hdr_list,(rohc_list_node_t *)p_ext_hdr_node);
    ROHC_EXIT_FN("rohc_d_add_node_in_list");

    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_d_add_n_update_list
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to add an item in the list and update the list
**          accordingly
**          This function is applicable only when AH,ESP and GRE comes as 
**          compressed packet 
**      ARGUMENTS:
**          The item list, node, uncompressed esp sequence number and pointers
**          to AH and GRE are taken as input
**      RETURN VALUE:
**          void
**      NOTES:
*****************************************************************************/

rohc_return_t   rohc_d_add_n_update_list    ROHC_PARAMS
(   (p_list, p_node, esp_seq, p_p_ah, p_p_gre, p_ecode),
    rohc_list_t             *p_list     _AND_
    rohc_protocol_node_t    *p_node     _AND_
    rohc_U32bit_t           esp_seq     _AND_
    rohc_ah_t               **p_p_ah    _AND_
    rohc_gre_t              **p_p_gre   _AND_
    rohc_error_code_t       *p_ecode
)
{
    ROHC_ENTER_FN("rohc_d_add_n_update_list");

    /*
    ** START_SPR_1306_FIX : Delayed packets are received from MAC and 
    ** due to HFN mismatch at PDCP, ciphering module produces a garble data 
    ** which is not handled at ROHC and it crashes.
    */
    if (p_node == ROHC_NULL)
    {
        ROHC_EXIT_FN("rohc_d_add_n_update_list");
        return ROHC_FAILURE;
    }
    /* END_SPR_1306_FIX */

    if(ROHC_FAILURE == rohc_d_add_node_in_list(p_list, p_node, p_ecode))
    {
        ROHC_EXIT_FN("rohc_d_add_n_update_list");
        return ROHC_FAILURE;
    }
    switch(p_node->protocol)
    {
        case ROHC_IPPROTO_AH:
        {
            *p_p_ah = (rohc_ah_t *)p_node->ptr;
        }
        break;
        case ROHC_IPPROTO_GRE:
        {
            *p_p_gre = (rohc_gre_t *)(p_node->ptr);
        }
        break;
        case ROHC_IPPROTO_ESP:
        {
            ((rohc_esp_t *)(p_node->ptr))->seq = esp_seq;
        }
        break;
    }
    ROHC_EXIT_FN("rohc_d_add_n_update_list");
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_dled_p1_update_dsc
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to update the DSC after CRC check is
**          successful.
**      ARGUMENTS:
**          The DSC and DPIB block are taken as input
**      RETURN VALUE:
**          SUCCESS/FAILURE
**      NOTES:
*****************************************************************************/

            /* +- SPR 17777 */
rohc_return_t   rohc_dled_p1_update_dsc    ROHC_PARAMS
(   (p_dsc, p_dpib),
    rohc_dsc_rtp_t      *p_dsc  _AND_
    rohc_dpib_t     *p_dpib
)
            /* +- SPR 17777 */
{
    rohc_U8bit_t    count = 0;

    ROHC_ENTER_FN("rohc_dled_p1_update_dsc");
    if(p_dpib->csrc_r_sw.icount != 0)
    {
        /* Update the CSRC information in the DSC */
        /* Update the translation table */
        for(count=0; count<p_dpib->csrc_new_item_count; count++)
        {
            if(ROHC_DSC_GET_R_CSRC_TT_NIDX(p_dsc) == 
                                    p_dpib->csrc_new_index_list[count])
            {
                p_dsc->rohc_d_r_csrc_tt.d_r_csrc_tt
                [p_dpib->csrc_new_index_list[count]] = 
                    p_dpib->csrc_tt[p_dpib->csrc_new_index_list[count]];
                p_dsc->rohc_d_r_csrc_tt.d_r_csrc_tt
                [p_dpib->csrc_new_index_list[count]].known = ROHC_TRUE;
                ROHC_DSC_SET_R_CSRC_TT_NIDX(p_dsc, 
                              (ROHC_DSC_GET_R_CSRC_TT_NIDX(p_dsc) + 1));
            }
        }
        /* Updating the SW */
        rohc_d_update_p1_r_csrc_sw(p_dsc, &p_dpib->csrc_r_sw, 
                                    p_dpib->csrc_ref_index); 
        /* Copy the final list after the RTP header */
        /* SPR 4808 Fix Start */
        ROHC_MEMCPY(p_dsc->rtp + 1, p_dpib->final_csrc_list, 
                                        (p_dpib->csrc_r_sw.icount) * 4);
        /* SPR 4808 Fix End */
    }
    else if(p_dpib->csrc_uo_sw.icount != 0)
    {   /* U/O Mode */
        /* Update the translation table */
        for(count=0; count<p_dpib->csrc_new_item_count; count++)
        {
            if(ROHC_DSC_GET_UO_CSRC_TT_NIDX(p_dsc) == 
                                    p_dpib->csrc_new_index_list[count])
            {
                p_dsc->rohc_d_uo_csrc_tt.d_uo_csrc_tt
                [p_dpib->csrc_new_index_list[count]] = 
                    p_dpib->csrc_tt[p_dpib->csrc_new_index_list[count]];
                p_dsc->rohc_d_uo_csrc_tt.d_uo_csrc_tt
                [p_dpib->csrc_new_index_list[count]].known = ROHC_TRUE;
                ROHC_DSC_SET_UO_CSRC_TT_NIDX(p_dsc, 
                                (ROHC_DSC_GET_UO_CSRC_TT_NIDX(p_dsc)+1));
            }
        }
        /* In this mode the SW is updated in case the compressed list 
         * has a gen_id and the SW does not contain a list
         * with that generation id */
        /* gen_id = 0xFF implies that list did not had any gen_id 
         * hence the context need not be updated */
        rohc_d_update_p1_uo_csrc_sw(p_dsc);

        /* Copy the final list after the RTP header */
        /* SPR 4808 Fix Start */
        ROHC_MEMCPY(p_dsc->rtp + 1, p_dpib->final_csrc_list, 
                                            (p_dpib->csrc_uo_sw.icount) * 4);
        /* SPR 4808 Fix End */

    } /* end of CSRC list updation */

    if(p_dpib->outeripext_r_sw.icount != 0)
    {
        /* Update the Outer IP ext list in the DSC */
        /* Update the Translation Table */
        for(count=0; count<p_dpib->outeripext_new_item_count; count++)
        {
            if(ROHC_DSC_GET_R_EXT_TT_NIDX(p_dsc) == 
                                    p_dpib->outeripext_new_index_list[count])
            {
                p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt[
                p_dpib->outeripext_new_index_list[count]] = 
                p_dpib->outeripext_tt[p_dpib->outeripext_new_index_list[
                count]];
                p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt
                [p_dpib->outeripext_new_index_list[count]].known = ROHC_TRUE;
                ROHC_DSC_SET_R_EXT_TT_NIDX(p_dsc, 
                                    (ROHC_DSC_GET_R_EXT_TT_NIDX(p_dsc) + 1));
            }
        } /* end of for */

        /* Updating the Outer IP Ext list */
        p_dsc->outeripextlist = p_dpib->outeripextlist;

        /* Updating the SW */
        rohc_d_update_p1_r_ext_sw(p_dsc,&(p_dpib->outeripext_r_sw),
                        p_dpib->ext_hdr_ref_index);
    }
    else if(p_dpib->outeripext_uo_sw.icount != 0)
    {   /* U/O Mode */
        for(count = 0; count<p_dpib->outeripext_new_item_count; count++)
        {
            if(ROHC_DSC_GET_UO_EXT_TT_NIDX(p_dsc) == 
                                p_dpib->outeripext_new_index_list[count])
            {
                p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt
                [p_dpib->outeripext_new_index_list[count]] = 
                p_dpib->outeripext_tt[p_dpib->outeripext_new_index_list[count]];
                p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt
                [p_dpib->outeripext_new_index_list[count]].known = ROHC_TRUE;
                ROHC_DSC_SET_UO_EXT_TT_NIDX(p_dsc, 
                                    (ROHC_DSC_GET_UO_EXT_TT_NIDX(p_dsc) + 1));
            }
        }
        /* Updating the Outer IP Ext list */
        p_dsc->outeripextlist = p_dpib->outeripextlist;

        /* Updating the SW */
        rohc_d_update_p1_uo_ext_sw(p_dsc, &(p_dpib->outeripext_uo_sw));
    }

    if(p_dpib->inneripext_r_sw.icount != 0)
    {
        /* Update the Inner IP Ext list in the DSC */
        /* Updating the Translation table */
        for(count=0; count<p_dpib->inneripext_new_item_count; count++)
        {
            if(ROHC_DSC_GET_R_EXT_TT_NIDX(p_dsc) == 
                                    p_dpib->inneripext_new_index_list[count])
            {
                p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt
                [p_dpib->inneripext_new_index_list[count]] = 
                p_dpib->inneripext_tt[p_dpib->inneripext_new_index_list[count]];
                p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt
                [p_dpib->inneripext_new_index_list[count]].known = ROHC_TRUE;
                ROHC_DSC_SET_R_EXT_TT_NIDX(p_dsc,
                (ROHC_DSC_GET_R_EXT_TT_NIDX(p_dsc) + 1));
            }
        }

        /* Updating the Inner IP Ext list */
        p_dsc->inneripextlist = p_dpib->inneripextlist;

        /* Updating the SW */
        rohc_d_update_p1_r_ext_sw(p_dsc, 
            &(p_dpib->inneripext_r_sw), p_dpib->ext_hdr_ref_index);
    }
    else if(p_dpib->inneripext_uo_sw.icount != 0)
    {
        for(count=0; count<p_dpib->inneripext_new_item_count; count++)
        {
            if(ROHC_DSC_GET_UO_EXT_TT_NIDX(p_dsc) == 
                                p_dpib->inneripext_new_index_list[count])
            {
                p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt
                [p_dpib->inneripext_new_index_list[count]] = 
                p_dpib->inneripext_tt[p_dpib->inneripext_new_index_list[count]];
                p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt
                [p_dpib->inneripext_new_index_list[count]].known = ROHC_TRUE;
                ROHC_DSC_SET_UO_EXT_TT_NIDX(p_dsc, 
                    (ROHC_DSC_GET_UO_EXT_TT_NIDX(p_dsc) + 1));
            }
        }

        /* Updating the Inner IP Ext list */
        p_dsc->inneripextlist = p_dpib->inneripextlist;

        /* Updating the SW */
        rohc_d_update_p1_uo_ext_sw(p_dsc, &(p_dpib->inneripext_uo_sw));
    }

    /* Update the Decoded values of Sequence numbers in the DSC */
    p_dsc->ah_seq = p_dpib->decoded_ah_seq;
    p_dsc->esp_seq = p_dpib->decoded_esp_seq;
    p_dsc->gre_seq = p_dpib->decoded_gre_seq;

    ROHC_EXIT_FN("rohc_dled_p1_update_dsc");

    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_dled_p2_update_dsc
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to update the DSC after CRC check is
**          successful.
**      ARGUMENTS:
**          The DSC and DPIB block are taken as input
**      RETURN VALUE:
**          SUCCESS/FAILURE
**      NOTES:
*****************************************************************************/

            /* +- SPR 17777 */
rohc_return_t   rohc_dled_p2_update_dsc    ROHC_PARAMS
(   (p_dsc, p_dpib),
    rohc_dsc_udp_t      *p_dsc  _AND_
    rohc_dpib_t         *p_dpib
)
            /* +- SPR 17777 */
{
    rohc_U8bit_t    count = 0;

    ROHC_ENTER_FN("rohc_dled_p2_update_dsc");

    if(p_dpib->outeripext_r_sw.icount != 0)
    {
        /* Update the Outer IP ext list in the DSC */
        /* Update the Translation Table */
        for(count=0; count<p_dpib->outeripext_new_item_count; count++)
        {
            if(ROHC_DSC_GET_R_EXT_TT_NIDX(p_dsc) == 
                                    p_dpib->outeripext_new_index_list[count])
            {
                p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt[
                p_dpib->outeripext_new_index_list[count]] = 
                p_dpib->outeripext_tt[p_dpib->outeripext_new_index_list[
                count]];

                p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt[
                p_dpib->outeripext_new_index_list[count]].known = ROHC_TRUE;

                ROHC_DSC_SET_R_EXT_TT_NIDX(p_dsc, 
                                    (ROHC_DSC_GET_R_EXT_TT_NIDX(p_dsc) + 1));
            }
        } /* end of for */

        /* Updating the Outer IP Ext list */
        p_dsc->outeripextlist = p_dpib->outeripextlist;

        /* Updating the SW */
        rohc_d_update_p2_r_ext_sw(p_dsc,&(p_dpib->outeripext_r_sw),
                        p_dpib->ext_hdr_ref_index);
    }
    else if(p_dpib->outeripext_uo_sw.icount != 0)
    {   /* U/O Mode */
        for(count = 0; count<p_dpib->outeripext_new_item_count; count++)
        {
            if(ROHC_DSC_GET_UO_EXT_TT_NIDX(p_dsc) == 
                                    p_dpib->outeripext_new_index_list[count])
            {
                p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt[
                p_dpib->outeripext_new_index_list[count]] = 
                p_dpib->outeripext_tt[p_dpib->outeripext_new_index_list[count]];

                p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt[
                p_dpib->outeripext_new_index_list[count]].known = ROHC_TRUE;

                ROHC_DSC_SET_UO_EXT_TT_NIDX(p_dsc,
                                     (ROHC_DSC_GET_UO_EXT_TT_NIDX(p_dsc) + 1));
            }
        }
        /* Updating the Outer IP Ext list */
        p_dsc->outeripextlist = p_dpib->outeripextlist;

        /* Updating the SW */
        rohc_d_update_p2_uo_ext_sw(p_dsc, &(p_dpib->outeripext_uo_sw));
    }

    if(p_dpib->inneripext_r_sw.icount != 0)
    {
        /* Update the Inner IP Ext list in the DSC */
        /* Updating the Translation table */
        for(count=0; count<p_dpib->inneripext_new_item_count; count++)
        {
            if(ROHC_DSC_GET_R_EXT_TT_NIDX(p_dsc) ==
                             p_dpib->inneripext_new_index_list[count])
            {
                p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt[
                p_dpib->inneripext_new_index_list[count]] =
                p_dpib->inneripext_tt[p_dpib->inneripext_new_index_list[count]];

                p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt[
                p_dpib->inneripext_new_index_list[count]].known = ROHC_TRUE;

                ROHC_DSC_SET_R_EXT_TT_NIDX(p_dsc,
                (ROHC_DSC_GET_R_EXT_TT_NIDX(p_dsc) + 1));
            }
        }

        /* Updating the Inner IP Ext list */
        p_dsc->inneripextlist = p_dpib->inneripextlist;

        /* Updating the SW */
        rohc_d_update_p2_r_ext_sw(p_dsc, 
            &(p_dpib->inneripext_r_sw), p_dpib->ext_hdr_ref_index);
    }
    else if(p_dpib->inneripext_uo_sw.icount != 0)
    {
        for(count=0; count<p_dpib->inneripext_new_item_count; count++)
        {
            if(ROHC_DSC_GET_UO_EXT_TT_NIDX(p_dsc) ==
                             p_dpib->inneripext_new_index_list[count])
            {
                p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt[
                p_dpib->inneripext_new_index_list[count]] = 
                p_dpib->inneripext_tt[p_dpib->inneripext_new_index_list[count]];

                p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt[
                p_dpib->inneripext_new_index_list[count]].known = ROHC_TRUE;

                ROHC_DSC_SET_UO_EXT_TT_NIDX(p_dsc,
                         (ROHC_DSC_GET_UO_EXT_TT_NIDX(p_dsc) + 1));
            }
        }

        /* Updating the Inner IP Ext list */
        p_dsc->inneripextlist = p_dpib->inneripextlist;

        /* Updating the SW */
        rohc_d_update_p2_uo_ext_sw(p_dsc, &(p_dpib->inneripext_uo_sw));
    }

    /* Update the Decoded values of Sequence numbers in the DSC */
    p_dsc->ah_seq = p_dpib->decoded_ah_seq;
    p_dsc->esp_seq = p_dpib->decoded_esp_seq;
    p_dsc->gre_seq = p_dpib->decoded_gre_seq;

    ROHC_EXIT_FN("rohc_dled_p2_update_dsc");

    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_led_encode_sn
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to encode AH, GRE and ESP seq number
**          compare the last 3 bytes (MSB) of both seq no. 
**          In case they are same send just 
**          1 byte of Seq no else send the whole Seq no  
**          It also copies the uncompressed new seq in the old one.
**      ARGUMENTS:
**          The old and new sequence number are taken as input.
**      RETURN VALUE:
**          void
**      NOTES:
*****************************************************************************/


rohc_void_t rohc_led_encode_sn  ROHC_PARAMS
(   (p_old_seq, p_seq),
    rohc_U32bit_t   *p_old_seq  _AND_
    rohc_U32bit_t   *p_seq
)
{
    rohc_U32bit_t   seq = *p_seq;
    ROHC_ENTER_FN("rohc_led_encode_sn");
    if((*p_old_seq - *p_seq) < 128)
    {
        /* Send just 1 byte and set the 8th bit as 0 */
        *p_seq = (*p_seq) & 0xFF;
        *p_seq = *p_seq & 0x7F;
    }
    else
    {
        /* Send 4 bytes and set the 8th bit as 1 */
        *p_seq = ROHC_SET_BIT(((rohc_U8bit_t)(*p_seq)), 8); 
    }

    *p_old_seq = seq;
    ROHC_EXIT_FN("rohc_led_encode_sn");
}

/*****************************************************************************
**      FUNCTION:
**          rohc_led_decode_sn
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to decode AH, GRE and ESP Sequence number
**          This function is called to decode SN when just 1 byte of SN is 
**          received. Then the old value of SN is picked and
**          LSB received is appended to the MSB's
**      ARGUMENTS:
**          The old sequence number and LSB of new seq no. are inputs
**      RETURN VALUE:
**          void
**      NOTES:
*****************************************************************************/

rohc_void_t     rohc_led_decode_sn  ROHC_PARAMS
(   (old_seq, p_seq, len),
    rohc_U32bit_t   old_seq _AND_
    rohc_U32bit_t   *p_seq  _AND_
    rohc_U8bit_t    len
)
{
    ROHC_ENTER_FN("rohc_led_decode_sn");
    if(len == 1)
    {
        /* Copy the 3 bytes from old_seq to p_seq */
        *p_seq |= (old_seq & 0xFFFFFF00) ;
    }
    /* Replace the 8th bit of 1st byte */
    *p_seq = (*p_seq & 0x7F) | (old_seq & 0x80000000);

    ROHC_EXIT_FN("rohc_led_decode_sn");
}

/*****************************************************************************
**      FUNCTION:
**          rohc_led_p1_generate_ext_gen_id
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to generate a gen_id for the extension
**          header list.
**      ARGUMENTS:
**      RETURN VALUE:
**          gen_id
**      NOTES:
*****************************************************************************/
rohc_U8bit_t    rohc_led_p1_generate_ext_gen_id    ROHC_PARAMS
(   (p_entity, p_csc, count, index_list, p_gp_flag),
    rohc_entity_t   *p_entity   _AND_
    rohc_csc_rtp_t  *p_csc      _AND_
    rohc_U8bit_t    count       _AND_
    rohc_U8bit_t    *index_list _AND_
    rohc_bool_t     *p_gp_flag
)
{
    rohc_uo_sw_t    sw_prev_entry;
    rohc_U8bit_t    gen_id;
    register rohc_U8bit_t    gen_count;
    rohc_U16bit_t   sn_no = ROHC_CSC_GET_SN_NO(p_csc);

    ROHC_ENTER_FN("rohc_led_p1_generate_ext_gen_id");

    sw_prev_entry = ROHC_CSC_GET_UO_EXT_SW(p_csc,
               ROHC_CSC_GET_UO_EXT_SW_NIDX(p_csc)-1);

    if(ROHC_SUCCESS ==
         rohc_compare_prev_list(sw_prev_entry, count, index_list))
    {
        /* Since lists are same, hence increment the counter
         * Also since list same as previous hence no need to send
         * gen_id as this list would not be added in the SW 
         * at the decompressor side */
        gen_id = sw_prev_entry.gen_id;
        *p_gp_flag = ROHC_FALSE;

        /* Increment gen count */
        (p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw
        [ROHC_CSC_GET_UO_EXT_SW_NIDX(p_csc)- 1].gen_count)++;

        gen_count = p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw
                    [ROHC_CSC_GET_UO_EXT_SW_NIDX(p_csc)- 1].gen_count;

        ROHC_LOG(LOG_INFO,
        "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
        "The list is same as previous list with gen_id[%d] and count[%d]\n",\
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc), gen_id, gen_count);

        if(gen_count == p_entity->max_dynamic_info_update_counter)
        {
           /* Implies gen_id has been repeated L times
            *  hence curr_list can be made ref_list */
           ROHC_CSC_SET_UO_EXT_SW_RIDX(p_csc,
                   ROHC_CSC_GET_UO_EXT_SW_NIDX(p_csc)-1);
           ROHC_CSC_SET_UO_EXT_SW_ACKED(p_csc);

           /* Also there is no need to send gen_id */
           *p_gp_flag = ROHC_FALSE;
           ROHC_LOG(LOG_INFO,
           "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
           "Gen_id [%d] has been repeated [%d]"
           " times,hence making it as ref_list \n", \
           p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
           ROHC_CSC_GET_CID(p_csc),ROHC_CSC_GET_MODE(p_csc), gen_id, gen_count);
        }
        /* Also add the new SN in the SN list */
        p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw[
            ROHC_CSC_GET_UO_EXT_SW_NIDX(p_csc)-1].sn_no_list[gen_count]=sn_no;
    }
    else
    {
        /* List different then the previous list */
        gen_id = rohc_generate_gen_id(&sw_prev_entry);
        ROHC_LOG(LOG_INFO,
        "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
        "Generating new gen_id [%d] for the list\n", \
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc), gen_id);
        /* Add the entry in the sliding window */
        rohc_c_add_uo_mode_ext_hdr_p1_sw(p_csc, gen_id, count,index_list);
        *p_gp_flag = ROHC_TRUE;
    }

    ROHC_EXIT_FN("rohc_led_p1_generate_ext_gen_id");
    return gen_id;
}
/*****************************************************************************
**      FUNCTION:
**          rohc_led_p2_generate_ext_gen_id
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to generate a gen_id for the extension
**          header list.
**      ARGUMENTS:
**      RETURN VALUE:
**          gen_id
**      NOTES:
*****************************************************************************/
rohc_U8bit_t    rohc_led_p2_generate_ext_gen_id    ROHC_PARAMS
(   (p_entity, p_csc, count, index_list, p_gp_flag),
    rohc_entity_t       *p_entity   _AND_
    rohc_csc_udp_t      *p_csc      _AND_
    rohc_U8bit_t        count       _AND_
    rohc_U8bit_t        *index_list _AND_
    rohc_bool_t         *p_gp_flag
)
{
    rohc_uo_sw_t    sw_prev_entry;
    rohc_U8bit_t    gen_id;
    rohc_U8bit_t    gen_count;
    rohc_U16bit_t   sn_no = ROHC_CSC_GET_SN_NO(p_csc);

    ROHC_ENTER_FN("rohc_led_p2_generate_ext_gen_id");

    sw_prev_entry = ROHC_CSC_GET_UO_EXT_SW(p_csc,
               ROHC_CSC_GET_UO_EXT_SW_NIDX(p_csc)-1);

    if(ROHC_SUCCESS ==
         rohc_compare_prev_list(sw_prev_entry, count, index_list))
    {
        /* Since lists are same, hence increment the counter
         * Also since list same as previous hence no need to send
         * gen_id as this list would not be added in the SW 
         * at the decompressor side */
        gen_id = sw_prev_entry.gen_id;
        *p_gp_flag = ROHC_FALSE;

        /* Increment gen count */
        (p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw
                 [ROHC_CSC_GET_UO_EXT_SW_NIDX(p_csc)- 1].gen_count)++;
        gen_count = p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw
                    [ROHC_CSC_GET_UO_EXT_SW_NIDX(p_csc)- 1].gen_count;

        ROHC_LOG(LOG_INFO,
        "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
        "The list is same as previous list with gen_id[%d] and count[%d]\n",\
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc),  \
        ROHC_CSC_GET_MODE(p_csc), gen_id, gen_count);

        if(gen_count == p_entity->max_dynamic_info_update_counter)
        {
           /* Implies gen_id has been repeated L times
            *  hence curr_list can be made ref_list */
           ROHC_CSC_SET_UO_EXT_SW_RIDX(p_csc,
                   ROHC_CSC_GET_UO_EXT_SW_NIDX(p_csc)-1);
           ROHC_CSC_SET_UO_EXT_SW_ACKED(p_csc);

           /* Also there is no need to send gen_id */
           *p_gp_flag = ROHC_FALSE;
           ROHC_LOG(LOG_INFO,
           "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
           "Gen_id [%d] has been repeated [%d] times,"
           "hence making it as ref_list \n", \
           p_entity->rohc_entity_id.ueIndex,
           p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
           ROHC_CSC_GET_MODE(p_csc), gen_id, gen_count);
        }
        /* Also add the new SN in the SN list */
        p_csc->rohc_c_uo_ext_hdr_sw.uo_ext_hdr_sw[
            ROHC_CSC_GET_UO_EXT_SW_NIDX(p_csc)-1].sn_no_list[gen_count]=sn_no;
    }
    else
    {
        /* List different then the previous list */
        gen_id = rohc_generate_gen_id(&sw_prev_entry);
        ROHC_LOG(LOG_INFO,
        "CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
        "Generating new gen_id [%d] for the list\n", \
        p_entity->rohc_entity_id.ueIndex,
        p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),  \
        ROHC_CSC_GET_MODE(p_csc), gen_id);
        /* Add the entry in the sliding window */
        rohc_c_add_uo_mode_ext_hdr_p2_sw(p_csc, gen_id, count,index_list);
        *p_gp_flag = ROHC_TRUE;
    }

    ROHC_EXIT_FN("rohc_led_p2_generate_ext_gen_id");
    return gen_id;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_led_p1_generate_csrc_gen_id
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to generate a gen id for the CSRC list.
**      ARGUMENTS:
**      RETURN VALUE:
**          gen_id
**      NOTES:
*****************************************************************************/

rohc_U8bit_t    rohc_led_p1_generate_csrc_gen_id   ROHC_PARAMS
(   (p_entity, p_csc, count, index_list, p_gp_flag),
    rohc_entity_t   *p_entity   _AND_
    rohc_csc_rtp_t  *p_csc      _AND_
    rohc_U8bit_t    count       _AND_
    rohc_U8bit_t    *index_list _AND_
    rohc_bool_t     *p_gp_flag
)
{
    rohc_uo_sw_t    sw_prev_entry;
    rohc_U8bit_t    gen_id;
    rohc_U8bit_t    gen_count;

    ROHC_ENTER_FN("rohc_led_p1_generate_csrc_gen_id");
    sw_prev_entry = ROHC_CSC_GET_UO_CSRC_SW(p_csc,
        ROHC_CSC_GET_UO_CSRC_SW_NIDX(p_csc) -1);

    if(ROHC_SUCCESS ==
        rohc_compare_prev_list(sw_prev_entry, count, index_list))
    {
        /* Since lists are same, hence send same gen_id
         * and increment the counter */
        gen_id = sw_prev_entry.gen_id;

        /* Increment gen counter */
        p_csc->rohc_c_uo_csrc_sw.uo_csrc_sw
            [ROHC_CSC_GET_UO_CSRC_SW_NIDX(p_csc)- 1].gen_count++;

        gen_count = p_csc->rohc_c_uo_csrc_sw.uo_csrc_sw
                        [ROHC_CSC_GET_UO_CSRC_SW_NIDX(p_csc)- 1].gen_count;

        ROHC_LOG(LOG_INFO,
        "CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
        "The list is same as previous list with gen_id[%d] and count[%d]\n", \
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc), gen_id, gen_count);
        *p_gp_flag = ROHC_TRUE;

        if(gen_count == p_entity->max_dynamic_info_update_counter)
        {
            ROHC_LOG(LOG_INFO,
            "CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
            "Gen_id [%d] has been repeated [%d] times,"
            "hence making it as ref_list\n",
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
            ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc), 
            gen_id, gen_count);

            /* Implies gen_id has been repeated L times
             *  hence curr_list can be made ref_list */
            ROHC_CSC_SET_UO_CSRC_SW_RIDX(p_csc,
                    ROHC_CSC_GET_UO_CSRC_SW_NIDX(p_csc)-1);
            ROHC_CSC_SET_UO_CSRC_SW_ACKED(p_csc);

            /* Also there is no need to send gen_id */
            *p_gp_flag = ROHC_FALSE;
        }
   }
   else
   {
        gen_id = rohc_generate_gen_id(&sw_prev_entry);
        ROHC_LOG(LOG_INFO,
        "CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
        "Generating new gen_id [%d] for the list\n", \
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc), gen_id);

        /* Add the entry in the sliding window */
        rohc_c_add_p1_uo_mode_csrc_sw(p_csc, gen_id, count,index_list);
        *p_gp_flag = ROHC_TRUE;
    }

    ROHC_EXIT_FN("rohc_led_p1_generate_csrc_gen_id");
    return gen_id;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_d_pick_item_from_tt
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to pick the item from the TT
**      ARGUMENTS:
**          Pointer to TT, YLIST are input and pointers to AH, GRE are
**          returned.
**      RETURN VALUE:
**          SUCCESS/FAILURE
**      NOTES:
*****************************************************************************/

rohc_return_t   rohc_d_pick_item_from_tt  ROHC_PARAMS
(
    (p_tt, p_ext_hdr_list, esp_seq, p_p_ah, p_p_gre, p_ecode),
    rohc_decomp_ext_hdr_tt_t    *p_tt           _AND_
    rohc_list_t                 *p_ext_hdr_list _AND_
    rohc_U32bit_t               esp_seq         _AND_
    rohc_ah_t                   **p_p_ah        _AND_
    rohc_gre_t                  **p_p_gre       _AND_
    rohc_error_code_t           *p_ecode
)
{
    rohc_protocol_node_t        *p_ext_hdr_node;

    ROHC_ENTER_FN("rohc_d_pick_item_from_tt");

    /* The YLIST contains the same pointer as stored in TT and
     * hence its not supposed to be freed */

    p_ext_hdr_node = (rohc_protocol_node_t *)(p_tt->p_item);

    if(ROHC_FAILURE == rohc_d_add_n_update_list(p_ext_hdr_list,
        p_ext_hdr_node, esp_seq, p_p_ah, p_p_gre, p_ecode))
    {
        ROHC_EXIT_FN("rohc_d_pick_item_from_tt");
        return ROHC_FAILURE;
    }

    ROHC_EXIT_FN("rohc_d_pick_item_from_tt");
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_d_process_p1_new_item
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to process a new extension header.
**      ARGUMENTS:
**          Pointer to entity, DSC, TT and YLIST. Also length, pointers to AH
**          GRE and pointer pointing to new item in the item list are
**          returned.
**      RETURN VALUE:
**          void
**      NOTES:
**          The item pointer in Translation table, YLIST and DPIB/DSC all point
**          to the same pointers.
**          p_length is the length of the compressed buffer
*****************************************************************************/

rohc_return_t   rohc_d_process_p1_new_item     ROHC_PARAMS
(
    (p_entity, p_dsc, p_tt, p_length, p_ext_hdr_list, p_p_ah, p_p_gre, 
    p_p_item_start, p_ecode),
    rohc_entity_t               *p_entity           _AND_
    rohc_dsc_rtp_t              *p_dsc              _AND_
    rohc_decomp_ext_hdr_tt_t    *p_tt               _AND_
    rohc_U32bit_t               *p_length           _AND_
    rohc_list_t                 *p_ext_hdr_list     _AND_
    rohc_ah_t                   **p_p_ah            _AND_
    rohc_gre_t                  **p_p_gre           _AND_
    rohc_U8bit_t                **p_p_item_start    _AND_
    rohc_error_code_t           *p_ecode
)
{
    rohc_protocol_node_t    *p_ext_hdr_node = ROHC_NULL;
    rohc_U32bit_t            item_length = 0;

    ROHC_ENTER_FN("rohc_d_process_p1_new_item");
    p_tt->known = 0;
    if(ROHC_FAILURE == rohc_d_store_item_p1_in_list(p_dsc, p_tt, 
                            *p_p_item_start, &item_length, p_p_ah, 
                            p_p_gre, p_ecode))
    {
        ROHC_EXIT_FN("rohc_d_process_p1_new_item");
        return ROHC_FAILURE;
    }

    p_ext_hdr_node = (rohc_protocol_node_t *)p_tt->p_item;
    if(ROHC_FAILURE == rohc_d_add_node_in_list(p_ext_hdr_list, p_ext_hdr_node,
        p_ecode))
    {
        ROHC_EXIT_FN("rohc_d_process_p1_new_item");
        return ROHC_FAILURE;
    }

    *p_p_item_start = *p_p_item_start + item_length;
    *p_length = *p_length + item_length;

            /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
            /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "New item [%d] stored in TT and item list \n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    ((rohc_protocol_node_t *)(p_tt->p_item))->protocol);

    ROHC_EXIT_FN("rohc_d_process_p1_new_item");
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_d_process_p2_new_item
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to process a new extension header.
**      ARGUMENTS:
**          Pointer to entity, DSC, TT and YLIST. Also length, pointers to AH
**          GRE and pointer pointing to new item in the item list are
**          returned.
**      RETURN VALUE:
**          void
**      NOTES:
**          The item pointer in Translation table, YLIST and DPIB/DSC all point
**          to the same pointers.
**          p_length is the length of the compressed buffer
*****************************************************************************/

rohc_return_t   rohc_d_process_p2_new_item     ROHC_PARAMS
(
    (p_entity, p_dsc, p_tt, p_length, p_ext_hdr_list, p_p_ah, p_p_gre, 
    p_p_item_start, p_ecode),
    rohc_entity_t               *p_entity       _AND_
    rohc_dsc_udp_t              *p_dsc          _AND_
    rohc_decomp_ext_hdr_tt_t    *p_tt           _AND_
    rohc_U32bit_t               *p_length       _AND_
    rohc_list_t                 *p_ext_hdr_list _AND_
    rohc_ah_t                   **p_p_ah        _AND_
    rohc_gre_t                  **p_p_gre       _AND_
    rohc_U8bit_t                **p_p_item_start    _AND_
    rohc_error_code_t           *p_ecode
)
{
    rohc_protocol_node_t    *p_ext_hdr_node = ROHC_NULL;
    rohc_U32bit_t            item_length = 0;

    ROHC_ENTER_FN("rohc_d_process_p2_new_item");
    p_tt->known = 0;
    if(ROHC_FAILURE == rohc_d_store_item_p2_in_list(p_dsc, p_tt,
                *p_p_item_start, &item_length, p_p_ah, p_p_gre, p_ecode))
    {
        ROHC_EXIT_FN("rohc_d_process_p2_new_item");
        return ROHC_FAILURE;
    }

    p_ext_hdr_node = (rohc_protocol_node_t *)p_tt->p_item;
    if(ROHC_FAILURE == rohc_d_add_node_in_list(p_ext_hdr_list, p_ext_hdr_node,
        p_ecode))
    {
        ROHC_EXIT_FN("rohc_d_process_p2_new_item");
        return ROHC_FAILURE;
    }

    *p_p_item_start = *p_p_item_start + item_length;
    *p_length = *p_length + item_length;

            /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
            /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "New item [%d] stored in TT and item list \n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    ((rohc_protocol_node_t *)(p_tt->p_item))->protocol);

    ROHC_EXIT_FN("rohc_d_process_p2_new_item");
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_d_process_p1_csrc_new_item
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to process a new CSRC item
**      ARGUMENTS:
**          Pointer to entity, DSC, index where item was present in the item
**          list, the final index where item needs to be stored and pointer to
**          start of the item.
**      RETURN VALUE:
**          void
**      NOTES:
*****************************************************************************/

rohc_void_t     rohc_d_process_p1_csrc_new_item        ROHC_PARAMS
(   (p_entity, p_dsc, index, final_index, p_item_start),
    rohc_entity_t   *p_entity       _AND_
    rohc_dsc_rtp_t  *p_dsc          _AND_
    rohc_U8bit_t    index           _AND_
    rohc_U8bit_t    final_index     _AND_
    rohc_U8bit_t    *p_item_start
)
{
    rohc_dpib_t     *p_dpib = ROHC_DSC_GET_DPIB(p_dsc);

    ROHC_ENTER_FN("rohc_d_process_p1_csrc_new_item");
    
    /* Update the temporary CSRC TT in the DPIB */
    p_dpib->csrc_tt[index].known = 0;
    p_dpib->csrc_tt[index].item = ROHC_GET_4BYTES(p_item_start);

    /* Update the new item index list in the DPIB */
    p_dpib->csrc_new_index_list[p_dpib->csrc_new_item_count]=index;

    /* Update the new item count in DPIB */
    p_dpib->csrc_new_item_count++;

    /* Update the final CSRC list in DPIB */ 
    p_dpib->final_csrc_list[final_index] = p_dpib->csrc_tt[index].item;

    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    " Received new item [%d] at index[%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    p_dpib->csrc_tt[index].item, index);

    ROHC_EXIT_FN("rohc_d_process_p1_csrc_new_item");
}

/*****************************************************************************
**      FUNCTION:
**          rohc_c_compare_n_encode_p1_gre_ext_hdr_item
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to compare two EXT HDR items and also
**          encodes Sequence number field in case of GRE .
**      ARGUMENTS:
**          The items to be compared is taken as input
**      RETURN VALUE:
**          SUCCESS/FAILURE
**      NOTES:
*****************************************************************************/
STATIC rohc_return_t   rohc_c_compare_n_encode_p1_gre_ext_hdr_item    ROHC_PARAMS
(
 (p_csc, p_item, p_node, p_gseq_no),
 rohc_csc_rtp_t      *p_csc      _AND_
 rohc_U8bit_t        *p_item     _AND_
 rohc_list_node_t    *p_node     _AND_
 rohc_U32bit_t       *p_gseq_no
 )
{
    rohc_U8bit_t l_length = 0;
    rohc_U32bit_t   curr_key = 0, ref_key = 0;
    rohc_cpib_t     *p_cpib = ROHC_CSC_GET_CPIB(p_csc);
    if(((rohc_gre_t *)(((rohc_protocol_node_t *)p_item)->ptr))->flags ==
            ((rohc_gre_t *)(((rohc_protocol_node_t *)p_node)->ptr))->flags)
    {
        /* This implies that items have same flags */

        /* Compare the KEY field */
        if(((rohc_gre_t *)
                    (((rohc_protocol_node_t *)p_item)->ptr))->flags & ROHC_GRE_KP)
        { /* Key present */
            if(((rohc_gre_t *)((
                                (rohc_protocol_node_t *)
                                p_item)->ptr))->flags & ROHC_GRE_CP)
            {   /* Checksum present */
                l_length = sizeof(rohc_gre_t) + 4;
                ref_key = ROHC_GET_4BYTES((rohc_U8bit_t *)
                        (((rohc_protocol_node_t *)p_item)->ptr) + l_length);
                curr_key = ROHC_GET_4BYTES((rohc_U8bit_t *)
                        (((rohc_protocol_node_t *)p_node)->ptr) + l_length);
                if(ref_key == curr_key)
                {
                    /* Checksum field has a place holder in Extension 3  */
                    /* Set a flag in CPIB for sending CHK */
                    p_cpib->gre_chksum_present = ROHC_TRUE;
                    p_cpib->gre_chksum = ROHC_GET_2BYTES(
                            (rohc_U8bit_t *)(((rohc_protocol_node_t *)
                                    p_node)->ptr) + sizeof(rohc_gre_t));

                    /* Encode the GRE seq no, if present */
                    if(((rohc_gre_t *)((
                                        (rohc_protocol_node_t *)
                                        p_item)->ptr))->flags & ROHC_GRE_SP)
                    { /* Seq no present */
                        if(*p_gseq_no == 0)
                        {
                            *p_gseq_no = ROHC_GET_4BYTES(
                                    (rohc_U8bit_t *)(((rohc_protocol_node_t *)
                                            p_node)->ptr) + l_length + 4);   
                            rohc_led_encode_sn(&(p_csc->gre_seq), 
                                    p_gseq_no);
                        }
                        ROHC_EXIT_FN("rohc_c_compare_n_encode_p1_gre_ext_hdr_item");
                        return ROHC_SUCCESS;
                    }
                }
                else
                {   /* Key field different */
                    ROHC_EXIT_FN("rohc_c_compare_n_encode_p1_gre_ext_hdr_item");
                    return ROHC_FAILURE;
                }
            }
            else
            {   /* Checksum not present */
                l_length = sizeof(rohc_gre_t);
                ref_key = ROHC_GET_4BYTES((rohc_U8bit_t *)
                        (((rohc_protocol_node_t *)p_item)->ptr) + l_length);
                curr_key = ROHC_GET_4BYTES((rohc_U8bit_t *)
                        (((rohc_protocol_node_t *)p_node)->ptr) + l_length);
                if(ref_key == curr_key)
                {   /* Key same */
                    /* Encode the GRE seq no, if present */
                    if(((rohc_gre_t *)((
                                        (rohc_protocol_node_t *)
                                        p_item)->ptr))->flags & ROHC_GRE_SP)
                    { /* Seq no present */
                        if(*p_gseq_no == 0)
                        {
                            *p_gseq_no = ROHC_GET_4BYTES(
                                    (rohc_U8bit_t *)((
                                            (rohc_protocol_node_t *)
                                            p_node)->ptr) + l_length + 4);   
                            rohc_led_encode_sn(&(p_csc->gre_seq), 
                                    p_gseq_no);
                        }
                        ROHC_EXIT_FN("rohc_c_compare_n_encode_p1_ext_hdr_item");
                        return ROHC_SUCCESS;
                    }
                    p_cpib->gre_chksum_present = ROHC_FALSE;
                }
                else
                {   /* Key field different */
                    ROHC_EXIT_FN("rohc_c_compare_n_encode_p1_ext_hdr_item");
                    return ROHC_FAILURE;
                }
            }
        }
        else
        {   /* Key is not present */
            /* Checksum field always goes, if present */
            if(((rohc_gre_t *)((
                                (rohc_protocol_node_t *)
                                p_item)->ptr))->flags & ROHC_GRE_CP)
            {   /* Checksum present */
                /* Set a flag in CPIB for sending CHK */
                p_cpib->gre_chksum_present = ROHC_TRUE;
                p_cpib->gre_chksum = ROHC_GET_2BYTES((rohc_U8bit_t *)
                        (((rohc_protocol_node_t *)
                          p_node)->ptr) + sizeof(rohc_gre_t));

                /* Encode the GRE seq no, if present */
                if(((rohc_gre_t *)((
                                    (rohc_protocol_node_t *)
                                    p_item)->ptr))->flags & ROHC_GRE_SP)
                { /* Seq no present */
                    if(*p_gseq_no == 0)
                    {
                        *p_gseq_no = ROHC_GET_4BYTES((rohc_U8bit_t *)((
                                        (rohc_protocol_node_t *)p_node)->ptr) + 
                                sizeof(rohc_gre_t) + 4);         
                        rohc_led_encode_sn(&(p_csc->gre_seq),p_gseq_no);
                    }
                    ROHC_EXIT_FN("rohc_c_compare_n_encode_p1_gre_ext_hdr_item");
                    return ROHC_SUCCESS;
                }
            }
            else
            {   /* Checksum and Key both are not present */
                /* Encode the GRE seq no, if present */
                if(((rohc_gre_t *)((
                                    (rohc_protocol_node_t *)
                                    p_item)->ptr))->flags & ROHC_GRE_SP)
                { /* Seq no present */
                    if(*p_gseq_no == 0)
                    {
                        *p_gseq_no = ROHC_GET_4BYTES((rohc_U8bit_t *)((
                                        (rohc_protocol_node_t *)p_node)->ptr) + 
                                sizeof(rohc_gre_t)); 
                        rohc_led_encode_sn(&(p_csc->gre_seq),p_gseq_no);
                    }
                    ROHC_EXIT_FN("rohc_c_compare_n_encode_p1_gre_ext_hdr_item");
                    return ROHC_SUCCESS;
                }
            }
            p_cpib->gre_chksum_present = ROHC_FALSE;
        }
    }
    return ROHC_FAILURE;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_c_compare_n_encode_p2_gre_ext_hdr_item
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to compare two EXT HDR items and also
**          encodes Sequence number field in case of GRE .
**      ARGUMENTS:
**          The items to be compared is taken as input
**      RETURN VALUE:
**          SUCCESS/FAILURE
**      NOTES:
*****************************************************************************/
STATIC rohc_return_t   rohc_c_compare_n_encode_p2_gre_ext_hdr_item    ROHC_PARAMS
(
 (p_csc, p_item, p_node, p_gseq_no),
 rohc_csc_udp_t      *p_csc      _AND_
 rohc_U8bit_t        *p_item     _AND_
 rohc_list_node_t    *p_node     _AND_
 rohc_U32bit_t       *p_gseq_no)
{
    rohc_U8bit_t l_length = 0;
    rohc_U32bit_t   curr_key = 0, ref_key = 0;
    rohc_cpib_t     *p_cpib = ROHC_CSC_GET_CPIB(p_csc);
    if(((rohc_gre_t *)(((rohc_protocol_node_t *)p_item)->ptr))->flags ==
            ((rohc_gre_t *)(((rohc_protocol_node_t *)p_node)->ptr))->flags)
    {
        /* This implies that items have same flags */

        /* Compare the KEY field */
        if(((rohc_gre_t *)
                    (((rohc_protocol_node_t *)p_item)->ptr))->flags & ROHC_GRE_KP)
        { /* Key present */
            if(((rohc_gre_t *)
                        (((rohc_protocol_node_t *)p_item)->ptr))->flags & ROHC_GRE_CP)
            {   /* Checksum present */
                l_length = sizeof(rohc_gre_t) + 4;
                ref_key = ROHC_GET_4BYTES((rohc_U8bit_t *)
                        (((rohc_protocol_node_t *)p_item)->ptr) + l_length);

                curr_key = ROHC_GET_4BYTES((rohc_U8bit_t *)
                        (((rohc_protocol_node_t *)p_node)->ptr) + l_length);
                if(ref_key == curr_key)
                {
                    /* Checksum field has a place holder in Extension 3  */
                    /* Set a flag in CPIB for sending CHK */
                    p_cpib->gre_chksum_present = ROHC_TRUE;
                    p_cpib->gre_chksum = ROHC_GET_2BYTES((rohc_U8bit_t *)
                            (((rohc_protocol_node_t *)p_node)->ptr) + sizeof(rohc_gre_t));

                    /* Encode the GRE seq no, if present */
                    if(((rohc_gre_t *)
                                (((rohc_protocol_node_t *)p_item)->ptr))->flags & ROHC_GRE_SP)
                    { /* Seq no present */
                        if(*p_gseq_no == 0)
                        {
                            *p_gseq_no = ROHC_GET_4BYTES((rohc_U8bit_t *)
                                    (((rohc_protocol_node_t *)p_node)->ptr) + l_length + 4);   
                            rohc_led_encode_sn(&(p_csc->gre_seq), p_gseq_no);
                        }
                        ROHC_EXIT_FN("rohc_c_compare_n_encode_p2_gre_ext_hdr_item");
                        return ROHC_SUCCESS;
                    }
                }
                else
                {   /* Key field different */
                    ROHC_EXIT_FN("rohc_c_compare_n_encode_p2_gre_ext_hdr_item");
                    return ROHC_FAILURE;
                }
            }
            else
            {   /* Checksum not present */
                l_length = sizeof(rohc_gre_t);
                ref_key = ROHC_GET_4BYTES((rohc_U8bit_t *)
                        (((rohc_protocol_node_t *)p_item)->ptr) + l_length);
                curr_key = ROHC_GET_4BYTES((rohc_U8bit_t *)
                        (((rohc_protocol_node_t *)p_node)->ptr) + l_length);
                if(ref_key == curr_key)
                {   /* Key same */
                    /* Encode the GRE seq no, if present */
                    if(((rohc_gre_t *)(
                                    ((rohc_protocol_node_t *)p_item)->ptr))->flags & ROHC_GRE_SP)
                    { /* Seq no present */
                        if(*p_gseq_no == 0)
                        {
                            *p_gseq_no = ROHC_GET_4BYTES((rohc_U8bit_t *)
                                    (((rohc_protocol_node_t *)p_node)->ptr) + l_length + 4);   
                            rohc_led_encode_sn(&(p_csc->gre_seq), p_gseq_no);
                        }
                        ROHC_EXIT_FN("rohc_c_compare_n_encode_p2_ext_hdr_item");
                        return ROHC_SUCCESS;
                    }
                    p_cpib->gre_chksum_present = ROHC_FALSE;
                }
                else
                {   /* Key field different */
                    ROHC_EXIT_FN("rohc_c_compare_n_encode_p2_gre_ext_hdr_item");
                    return ROHC_FAILURE;
                }
            }
        }
        else
        {   /* Key is not present */
            /* Checksum field always goes, if present */
            if(((rohc_gre_t *)
                        (((rohc_protocol_node_t *)p_item)->ptr))->flags & ROHC_GRE_CP)
            {   /* Checksum present */
                /* Set a flag in CPIB for sending CHK */
                p_cpib->gre_chksum_present = ROHC_TRUE;
                p_cpib->gre_chksum = ROHC_GET_2BYTES((rohc_U8bit_t *)
                        (((rohc_protocol_node_t *)p_node)->ptr) + sizeof(rohc_gre_t));

                /* Encode the GRE seq no, if present */
                if(((rohc_gre_t *)
                            (((rohc_protocol_node_t *)p_item)->ptr))->flags & ROHC_GRE_SP)
                { /* Seq no present */
                    if(*p_gseq_no == 0)
                    {
                        *p_gseq_no = ROHC_GET_4BYTES((rohc_U8bit_t *)
                                (((rohc_protocol_node_t *)p_node)->ptr) + sizeof(rohc_gre_t) + 4);         
                        rohc_led_encode_sn(&(p_csc->gre_seq),p_gseq_no);
                    }
                    ROHC_EXIT_FN("rohc_c_compare_n_encode_p2_gre_ext_hdr_item");
                    return ROHC_SUCCESS;
                }
            }
            else
            {   /* Checksum and Key both are not present */
                /* Encode the GRE seq no, if present */
                if(((rohc_gre_t *)
                            (((rohc_protocol_node_t *)p_item)->ptr))->flags & ROHC_GRE_SP)
                { /* Seq no present */
                    if(*p_gseq_no == 0)
                    {
                        *p_gseq_no = ROHC_GET_4BYTES((rohc_U8bit_t *)
                                (((rohc_protocol_node_t *)p_node)->ptr) + sizeof(rohc_gre_t)); 
                        rohc_led_encode_sn(&(p_csc->gre_seq),p_gseq_no);
                    }
                    ROHC_EXIT_FN("rohc_c_compare_n_encode_p2_gre_ext_hdr_item");
                    return ROHC_SUCCESS;
                }
            }
            p_cpib->gre_chksum_present = ROHC_FALSE;
        }
    }

    ROHC_EXIT_FN("rohc_c_compare_n_encode_p2_gre_ext_hdr_item");
    return ROHC_FAILURE;
}

/*****************************************************************************
**      FUNCTION:
**          rohc_d_store_item_gre_in_list
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to store the item in item list
**          The first octet of p_buf is always next header field which tells the
**          protocol type and accordingly the length can be calculated. This
**          functions hold true only when uncompressed item is received in a 
**          list. If the item is GRE then their pointers also 
**          needs to be modified. 
**
**          The ptr field in rohc_protocol_node_t should be the uncompressed IP
**          datagram packet which needs to be sent to the upper layer and 
**          not the one which is received from the compressor
**      ARGUMENTS:
**          The p_ext_hdr_tt, buffer,length, GRE pointers, ecode are input
**      RETURN VALUE:
**          return SUCCESS or FAILURE
**      NOTES:
**          p_ext_hdr_tt->p_item is of type rohc_protocol_node_t 
**          p_length is length of compressed buffer
*****************************************************************************/

STATIC rohc_return_t   rohc_d_store_item_gre_in_list   ROHC_PARAMS
(   (p_ext_hdr_tt, p_buf, p_length, p_p_gre, p_ecode),
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt   _AND_
    rohc_U8bit_t                *p_buf          _AND_
    rohc_U32bit_t               *p_length       _AND_
    rohc_gre_t                  **p_p_gre       _AND_
    rohc_error_code_t           *p_ecode
)
{
    rohc_U8bit_t    flags = 0, *p_gre = ROHC_NULL; 
    rohc_U32bit_t   length = 0;
    /* First calculate length of gre */
    flags = *(p_buf + 1);
    length = sizeof(rohc_gre_t);
    if(flags & ROHC_GRE_C_BIT)
    {
        length += 4;
    }
    if(flags & ROHC_GRE_K_BIT)
    {
        length += 4;
    }
    if(flags & ROHC_GRE_S_BIT)
    {
        length += 4;
    }

    /* Now malloc and fill the buffer */
    p_gre = ROHC_MALLOC(length);
    if(p_gre == ROHC_NULL)
    {
        *p_ecode = ROHC_MEMORY_FAILURE;
        ROHC_EXIT_FN("rohc_d_store_item_gre_in_list");
        return ROHC_FAILURE;
    }
    ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->length = length;
    ((rohc_protocol_node_t *)
     (p_ext_hdr_tt->p_item))->protocol = ROHC_IPPROTO_GRE;
    ((rohc_protocol_node_t *)(p_ext_hdr_tt->p_item))->ptr = p_gre;
    *p_p_gre = (rohc_gre_t *)p_gre;
    (*p_p_gre)->flags = 0;
    p_gre += sizeof(rohc_gre_t);
    *p_length = 2;
    if(flags & ROHC_GRE_C_BIT)
    {
        (*p_p_gre)->flags |= ROHC_GRE_CP;
        *p_gre = *(p_buf + *p_length);
        p_gre += 4;
        *p_length += 2;
    }
    if(flags & ROHC_GRE_K_BIT)
    {
        (*p_p_gre)->flags |= ROHC_GRE_KP;
        *p_gre = *(p_buf + *p_length);
        p_gre += 4;
        *p_length += 4;
    }
    if(flags & ROHC_GRE_S_BIT)
    {
        (*p_p_gre)->flags |= ROHC_GRE_SP;
        *p_gre = *(p_buf + *p_length);
        *p_length += 4;
        p_gre += 4;
    }
    return ROHC_SUCCESS;
}

#ifdef ROHC_UT_FLAG
rohc_void_t node_walk(YLNODE * x,rohc_void_tp a)
{
    ROHC_UT_TRACE((stderr,
           "PROTOCOL = %d LENGTH = %d POINTER =%p\n",
           ((rohc_protocol_node_t *)x)->protocol,
           ((rohc_protocol_node_t *)x)->length,
           ((rohc_protocol_node_t *)x)->ptr));
}
#endif
