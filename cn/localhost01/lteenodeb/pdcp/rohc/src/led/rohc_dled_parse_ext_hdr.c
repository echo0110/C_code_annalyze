/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_dled_parse_ext_hdr.c,v 1.4.34.2 2010/11/09 05:19:45 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file parse the buffer received at decompressor side
 *                     in case of extension header list
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_dled_parse_ext_hdr.c,v $
 * Revision 1.4.34.2  2010/11/09 05:19:45  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.4.34.1  2010/10/04 07:06:28  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.4  2009/05/27 16:14:06  gur19836
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
#include "rohc_typ.h"
#include "rohc_def.h"
#include "rohc_stats_def.h"
#include "rohc_d_typ.h"
#include "rohc_dcom_mcr.h"
#include "rohc_gcom_mcr.h"
#include "rohc_trace_mcr.h"
#include "rohc_sl_windows.h"
#include "rohc_led_mcr.h"
#include "rohc_led_proto.h"
#include "rohc_led_def.h"
#include "rohc_opt_proto.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/
        /* +- SPR 17777 */
extern tickType_t pdcpCurrentTime_g;
        /* +- SPR 17777 */
/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* + SPR 17439 */
static rohc_return_t rohcParseP1ExtHdrEncType1PSflagOn   ROHC_PROTO
((
    rohc_entity_t               *p_entity           ,
    rohc_dsc_rtp_t              *p_dsc              ,
    rohc_U8bit_t                *p_compressed_pkt   ,
    rohc_U32bit_t               *p_length           ,
    rohc_U8bit_t                *p_new_item_count   ,
    rohc_list_t                 *p_ext_hdr_list     ,
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       ,
    rohc_U8bit_t                *p_new_index_list   ,
    rohc_U32bit_t               esp_seq             ,
    rohc_ah_t                   **p_p_ah            ,
    rohc_gre_t                  **p_p_gre           ,
    rohc_r_sw_t                 *p_ext_hdr_ref_sw   ,
    rohc_decomp_ext_hdr_tt_t    *p_tt               ,
    rohc_U8bit_t                count               ,
    rohc_U8bit_t                l_insert            ,
    /* Coverity 54507 */
    rohc_U16bit_t               insert_bit_mask     ,
    /* Coverity 54507 */
    rohc_error_code_t           *p_ecode
 ));

static rohc_return_t rohcParseP1ExtHdrEncType1PSflagOff   ROHC_PROTO
((
    rohc_entity_t               *p_entity           ,
    rohc_dsc_rtp_t              *p_dsc              ,
    rohc_U8bit_t                *p_compressed_pkt   ,
    rohc_U32bit_t               *p_length           ,
    rohc_U8bit_t                *p_new_item_count   ,
    rohc_list_t                 *p_ext_hdr_list     ,
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       ,
    rohc_U8bit_t                *p_new_index_list   ,
    rohc_U32bit_t               esp_seq             ,
    rohc_ah_t                   **p_p_ah            ,
    rohc_gre_t                  **p_p_gre           ,
    rohc_r_sw_t                 *p_ext_hdr_ref_sw   ,
    rohc_decomp_ext_hdr_tt_t    *p_tt               ,
    rohc_U8bit_t                count               ,
    rohc_U8bit_t                l_insert            ,
    rohc_U16bit_t               insert_bit_mask     ,
    rohc_error_code_t           *p_ecode
 ));

static rohc_return_t rohcParseP2ExtHdrEncType1PSflagOn   ROHC_PROTO
((
    rohc_entity_t               *p_entity           ,
    rohc_dsc_udp_t              *p_dsc              ,
    rohc_U8bit_t                *p_compressed_pkt   ,
    rohc_U32bit_t               *p_length           ,
    rohc_U8bit_t                *p_new_item_count   ,
    rohc_list_t                 *p_ext_hdr_list     ,
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       ,
    rohc_U8bit_t                *p_new_index_list   ,
    rohc_U32bit_t               esp_seq             ,
    rohc_ah_t                   **p_p_ah            ,
    rohc_gre_t                  **p_p_gre           ,
    rohc_r_sw_t                 *p_ext_hdr_ref_sw   ,
    rohc_decomp_ext_hdr_tt_t   *p_tt                ,
    rohc_U8bit_t                count               ,
    rohc_U8bit_t                l_insert            ,
    /* Coverity 54510 */
    rohc_U16bit_t               insert_bit_mask     ,
    /* Coverity 54510 */
    rohc_error_code_t           *p_ecode
 ));

static rohc_return_t rohcParseP2ExtHdrEncType1PSflagOff   ROHC_PROTO
((
    rohc_entity_t               *p_entity           ,
    rohc_dsc_udp_t              *p_dsc              ,
    rohc_U8bit_t                *p_compressed_pkt   ,
    rohc_U32bit_t               *p_length           ,
    rohc_U8bit_t                *p_new_item_count   ,
    rohc_list_t                 *p_ext_hdr_list     ,
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       ,
    rohc_U8bit_t                *p_new_index_list   ,
    rohc_U32bit_t               esp_seq             ,
    rohc_ah_t                   **p_p_ah            ,
    rohc_gre_t                  **p_p_gre           ,
    rohc_r_sw_t                 *p_ext_hdr_ref_sw   ,
    rohc_decomp_ext_hdr_tt_t    *p_tt               ,
    rohc_U8bit_t                count               ,
    rohc_U8bit_t                l_insert            ,
    rohc_U16bit_t               insert_bit_mask     ,
    rohc_error_code_t           *p_ecode
 ));

static rohc_return_t rohcParseP1ExtHdrEncType3PSflagOn   ROHC_PROTO
((
    rohc_entity_t               *p_entity           ,
    rohc_dsc_rtp_t              *p_dsc              ,
    rohc_U8bit_t                *p_compressed_pkt   ,
    rohc_U32bit_t               *p_length           ,
    rohc_U8bit_t                *p_new_item_count   ,
    rohc_list_t                 *p_ext_hdr_list     ,
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       ,
    rohc_U8bit_t                *p_new_index_list   ,
    rohc_U32bit_t               esp_seq             ,
    rohc_ah_t                   **p_p_ah            ,
    rohc_gre_t                  **p_p_gre           ,
    rohc_r_sw_t                 *p_ext_hdr_ref_sw   ,
    rohc_decomp_ext_hdr_tt_t    *p_tt               ,
    rohc_U8bit_t  ref_index_list[ROHC_MAX_ENTRIES_IN_TT] ,
    rohc_U8bit_t                count               ,
    rohc_U8bit_t                l_insert            ,
    /* Coverity 54508 */
    rohc_U16bit_t               insert_bit_mask     , 
    /* Coverity 54508 */
    rohc_error_code_t           *p_ecode
 ));

static rohc_return_t rohcParseP1ExtHdrEncType3PSflagOff   ROHC_PROTO
((
    rohc_entity_t               *p_entity           ,
    rohc_dsc_rtp_t              *p_dsc              ,
    rohc_U8bit_t                *p_compressed_pkt   ,
    rohc_U32bit_t               *p_length           ,
    rohc_U8bit_t                *p_new_item_count   ,
    rohc_list_t                 *p_ext_hdr_list     ,
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       ,
    rohc_U8bit_t                *p_new_index_list   ,
    rohc_U32bit_t               esp_seq             ,
    rohc_ah_t                   **p_p_ah            ,
    rohc_gre_t                  **p_p_gre           ,
    rohc_r_sw_t                 *p_ext_hdr_ref_sw   ,
    rohc_decomp_ext_hdr_tt_t    *p_tt               ,
    rohc_U8bit_t    ref_index_list[ROHC_MAX_ENTRIES_IN_TT] ,
    rohc_U8bit_t                count               ,
    rohc_U8bit_t                l_insert            ,
    /* Coverity 54511 */
    rohc_U16bit_t               insert_bit_mask     , 
    /* Coverity 54511 */
    rohc_error_code_t           *p_ecode
 ));

static rohc_return_t rohcParseP2ExtHdrEncType3PSflagOn   ROHC_PROTO
((
    rohc_entity_t               *p_entity           ,
    rohc_dsc_udp_t              *p_dsc              ,
    rohc_U8bit_t                *p_compressed_pkt   ,
    rohc_U32bit_t               *p_length           ,
    rohc_U8bit_t                *p_new_item_count   ,
    rohc_list_t                 *p_ext_hdr_list     ,
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       ,
    rohc_U8bit_t                *p_new_index_list   ,
    rohc_U32bit_t               esp_seq             ,
    rohc_ah_t                   **p_p_ah            ,
    rohc_gre_t                  **p_p_gre           ,
    rohc_r_sw_t                 *p_ext_hdr_ref_sw   ,
    rohc_decomp_ext_hdr_tt_t    *p_tt               ,
    rohc_U8bit_t  ref_index_list[ROHC_MAX_ENTRIES_IN_TT] ,
    rohc_U8bit_t                count               ,
    rohc_U8bit_t                l_insert            ,
    /* Coverity 54511 */
    rohc_U16bit_t               insert_bit_mask     , 
    /* Coverity 54511 */
    rohc_error_code_t           *p_ecode
 ));

static rohc_return_t rohcParseP2ExtHdrEncType3PSflagOff   ROHC_PROTO
((
    rohc_entity_t               *p_entity           ,
    rohc_dsc_udp_t              *p_dsc              ,
    rohc_U8bit_t                *p_compressed_pkt   ,
    rohc_U32bit_t               *p_length           ,
    rohc_U8bit_t                *p_new_item_count   ,
    rohc_list_t                 *p_ext_hdr_list     ,
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       ,
    rohc_U8bit_t                *p_new_index_list   ,
    rohc_U32bit_t               esp_seq             ,
    rohc_ah_t                   **p_p_ah            ,
    rohc_gre_t                  **p_p_gre           ,
    rohc_r_sw_t                 *p_ext_hdr_ref_sw   ,
    rohc_decomp_ext_hdr_tt_t    *p_tt               ,
    rohc_U8bit_t    ref_index_list[ROHC_MAX_ENTRIES_IN_TT] ,
    rohc_U8bit_t                count               ,
    rohc_U8bit_t                l_insert            ,
    rohc_U16bit_t               insert_bit_mask     ,
    rohc_error_code_t           *p_ecode
 ));

static rohc_return_t rohcP1UpdateTempContxtSWInDPIB   ROHC_PROTO
((
    rohc_U8bit_t                *p_ext_hdr_sw       ,
    rohc_dsc_rtp_t              *p_dsc              ,
    rohc_U8bit_t                gen_id
 ));

static rohc_return_t rohcP2UpdateTempContxtSWInDPIB   ROHC_PROTO
((
    rohc_U8bit_t                *p_ext_hdr_sw       ,
    rohc_dsc_udp_t              *p_dsc              ,
    rohc_U8bit_t                gen_id
 ));
/* - SPR 17439 */
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/




/*****************************************************************************
**      FUNCTION:
**          parse_p1_ext_hdr_enc_type_0_pkt
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to parse an Encoding Type 0 packet
**      ARGUMENTS:
**          The compressor context, buffer etc. are taken as input
**      RETURN VALUE:
**          void
**      NOTES:
**          p_length is length of compressed pkt
*****************************************************************************/

rohc_return_t   parse_p1_ext_hdr_enc_type_0_pkt    ROHC_PARAMS
(
    (p_entity,p_dsc,p_compressed_pkt,p_length,p_new_item_count,p_ext_hdr_list,
    p_ext_hdr_tt,p_new_index_list,esp_seq,p_ext_hdr_sw,p_p_ah,p_p_gre,p_ecode),

    rohc_entity_t               *p_entity           _AND_
    rohc_dsc_rtp_t              *p_dsc              _AND_   
    rohc_U8bit_t                *p_compressed_pkt   _AND_
    rohc_U32bit_t                *p_length          _AND_
    rohc_U8bit_t                *p_new_item_count   _AND_
    rohc_list_t                 *p_ext_hdr_list     _AND_
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       _AND_
    rohc_U8bit_t                *p_new_index_list   _AND_
    rohc_U32bit_t               esp_seq             _AND_
    rohc_U8bit_t                *p_ext_hdr_sw       _AND_
    rohc_ah_t                   **p_p_ah            _AND_
    rohc_gre_t                  **p_p_gre           _AND_
    rohc_error_code_t           *p_ecode
)
{
    rohc_U8bit_t    gen_id, *p_item_start;
    rohc_U8bit_t    count = 0, /* Count is total CSRC items */ 
                    i = 0, index=0;
    rohc_bool_t     PS_flag;
    rohc_dpib_t     *p_dpib = ROHC_DSC_GET_DPIB(p_dsc);
    rohc_U8bit_t    index_list[ROHC_MAX_ENTRIES_IN_TT]; 
                    /*To store in SW*/
    rohc_decomp_ext_hdr_tt_t    *p_tt;
    
    ROHC_ENTER_FN("parse_p1_ext_hdr_enc_type_0_pkt");

    if(ROHC_MODE_R == p_dsc->d_mode)
    {
        p_tt = p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt;
    }
    else
    {
        p_tt = p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt;
    }

    PS_flag = (rohc_bool_t)ROHC_GET_BIT(*p_compressed_pkt, ROHC_PS_BIT);
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "PS_Flag [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    PS_flag);

    count = ROHC_GET_LOWER_NIBBLE(*p_compressed_pkt);
    if(ROHC_GET_BIT(*p_compressed_pkt, ROHC_GP_BIT))
    {
        /* implies gen_id exists */
        gen_id = *(++p_compressed_pkt);
        (*p_length)++;
        ROHC_LOG(LOG_INFO,
        "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
        "Gen id [%d] received in the packet\n", \
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
        gen_id);
    }
    else
    {
        gen_id = 0xFF;
    }
    if(count == 0)
    {
        (*p_length)++;
        return ROHC_SUCCESS;
    }
    p_compressed_pkt++;
    (*p_length)++;

    if(PS_flag)
    {
        p_item_start = p_compressed_pkt + count;
        *p_length = *p_length + count;

        /* Read XI List as 1 byte */
        for(i=0; i<count; i++)
        {
            index_list[i] = *(p_compressed_pkt + i);    
            ROHC_GET_NBITS(index_list[i], 1, 7, index);
        
            /*
            ** START_SPR_1306_FIX : Delayed packets are received from MAC
            ** and due to HFN mismatch at PDCP, ciphering module produces a 
            ** garble data which is not handled at ROHC and it crashes.
            */
            if (index >= ROHC_MAX_ENTRIES_IN_TT)
            {    
                ROHC_LOG(LOG_INFO,
                "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                "Index [%d] received in the packet is out of bound\n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,\
                ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                index);

                ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_0_pkt");
                return ROHC_FAILURE;
            }    
            /* END_SPR_1306_FIX */

            if(ROHC_GET_BIT(index_list[i], 8))
            {       
                /* Item in Item List
                 * Hence need to add in new item list and final list */

                /* Updating the new item list */
                if(ROHC_FAILURE == rohc_d_process_p1_new_item(p_entity, p_dsc,
                    (p_ext_hdr_tt + index), p_length, p_ext_hdr_list,
                    p_p_ah, p_p_gre, &p_item_start, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_0_pkt");
                    return ROHC_FAILURE;
                }

                /* Updating the new index list */
                *(p_new_index_list + (*p_new_item_count)) = index;

                /* Increment the new item counter */
                (*p_new_item_count)++;
            }
            else
            {
                /* In Encoding Type 0 this case might never occur */
                /* Item picked from translation table
                 * Hence need to add only in final list */

                /* Note that the item in TT is of type rohc_protocol_node_t */
                if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt + index),
                    p_ext_hdr_list, esp_seq, p_p_ah, p_p_gre, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_0_pkt");
                    return ROHC_FAILURE;
                }

                ROHC_LOG(LOG_INFO,
                "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                "Item[%d] to be picked from TT at index[%d]\n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
                ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
		((rohc_protocol_node_t *)((p_tt + index)->p_item))->protocol,
		index);
            }
            index_list[i] = index;
        }
    }
    else
    {   
        p_item_start = p_compressed_pkt + (count/2);
        *p_length = *p_length + (count/2);
        if(count % 2 != 0)
        {   /* Count is odd ,hence 1 more byte is added */
            p_item_start++;
            (*p_length)++;
        }

        /* Read XI list as 4 bits */
        while(i<count)
        {   
            /* Read the first 4 bits */
            index_list[i] = ROHC_GET_UPPER_NIBBLE(*p_compressed_pkt);
            ROHC_GET_NBITS(*p_compressed_pkt, 5, 3, index);
            if(ROHC_GET_BIT(*p_compressed_pkt, 8))
            {
                /* Item in Item List
                 * Hence need to add in new item list and final list */

                /* Updating the new item list */
                if(ROHC_FAILURE == rohc_d_process_p1_new_item(p_entity, p_dsc, 
                    (p_ext_hdr_tt + index), p_length, p_ext_hdr_list, p_p_ah, 
                    p_p_gre, &p_item_start, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_0_pkt");
                    return ROHC_FAILURE;
                }

                /* Updating the new index list */
                *(p_new_index_list + (*p_new_item_count)) = index;

                /* Increment the new item counter */
                (*p_new_item_count)++;
            }
            else
            {
                /* Item picked from Ref List
                 * Hence need to add only in final list */

                if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt + index),
                    p_ext_hdr_list, esp_seq, p_p_ah, p_p_gre, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_0_pkt");
                    return ROHC_FAILURE;
                }

                ROHC_LOG(LOG_INFO,
                "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                "Item[%d] to be picked from ref_list at index[%d]\n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
                ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                ((rohc_protocol_node_t *)((p_tt + index)->p_item))->protocol,
                index);
            }
            index_list[i++] = index;

            /* Now read the next 4 bits */
            if(i >= count) break;
            index_list[i] = ROHC_GET_LOWER_NIBBLE(*p_compressed_pkt);
            ROHC_GET_NBITS(*p_compressed_pkt, 1,3, index);
            if(ROHC_GET_BIT(*p_compressed_pkt, 4))
            {
                /* Item in Item List
                 * Hence need to add in new item list and final list */

                /* Updating the new item list */
                if(ROHC_FAILURE == rohc_d_process_p1_new_item(p_entity, p_dsc, 
                    (p_ext_hdr_tt + index), p_length, p_ext_hdr_list, p_p_ah,
                    p_p_gre, &p_item_start, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_0_pkt");
                    return ROHC_FAILURE;
                }

                /* Updating the new index list */
                *(p_new_index_list + (*p_new_item_count)) = index;

                /* Increment the new item counter */
                (*p_new_item_count)++;
            }
            else
            {
                /* Item picked from Ref List
                 * Hence need to add only in final list */

                if(ROHC_FAILURE == rohc_d_pick_item_from_tt(p_tt + index,
                    p_ext_hdr_list, esp_seq, p_p_ah, p_p_gre, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_0_pkt");
                    return ROHC_FAILURE;
                }

                ROHC_LOG(LOG_INFO,
                "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                "Item[%d] to be picked from TT at index[%d]\n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                ((rohc_protocol_node_t *)((p_tt + index)->p_item))->protocol, 
                index);
            }
            index_list[i++] = index;

            p_compressed_pkt++;
        }
    }
    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        ((rohc_r_sw_t *)(p_ext_hdr_sw))->rtp_sn_no =
                                    ROHC_DSC_GET_SN_NO(p_dpib);
        ((rohc_r_sw_t *)(p_ext_hdr_sw))->icount = count;
        if(count != 0)
        {
            ROHC_MEMCPY(((rohc_r_sw_t *)(p_ext_hdr_sw))->index_list,
                    index_list, count);
        }
    }
    else
    {
        ((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->gen_id = gen_id;
        ((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->icount = count;
        if(count != 0)
        {
            ROHC_MEMCPY(((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->index_list,
                    index_list, count);
        }
    }
        
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Total length of the compressed packet [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    *p_length);

    /*
    ** Incrementing Stats - ROHC_FUTURE_STATS
    ** pdcpStats.statsRohc.numberOfListEncNoGenSchmUsedProcessed++;
    */

    ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_0_pkt");
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**          parse_p2_ext_hdr_enc_type_0_pkt
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to parse an Encoding Type 0 packet
**      ARGUMENTS:
**          The compressor context, buffer etc. are taken as input
**      RETURN VALUE:
**          void
**      NOTES:
**          p_length is length of compressed pkt
*****************************************************************************/

rohc_return_t   parse_p2_ext_hdr_enc_type_0_pkt    ROHC_PARAMS
(
    (p_entity,p_dsc,p_compressed_pkt,p_length,p_new_item_count,p_ext_hdr_list,
p_ext_hdr_tt,p_new_index_list,esp_seq,p_ext_hdr_sw,p_p_ah,p_p_gre,p_ecode),
    rohc_entity_t               *p_entity           _AND_
    rohc_dsc_udp_t              *p_dsc              _AND_
    rohc_U8bit_t                *p_compressed_pkt   _AND_
    rohc_U32bit_t               *p_length           _AND_
    rohc_U8bit_t                *p_new_item_count   _AND_
    rohc_list_t                 *p_ext_hdr_list     _AND_
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       _AND_
    rohc_U8bit_t                *p_new_index_list   _AND_
    rohc_U32bit_t               esp_seq             _AND_
    rohc_U8bit_t                *p_ext_hdr_sw       _AND_
    rohc_ah_t                   **p_p_ah            _AND_
    rohc_gre_t                  **p_p_gre           _AND_
    rohc_error_code_t           *p_ecode
)
{
    rohc_U8bit_t    gen_id, *p_item_start;
    rohc_U8bit_t    count = 0, /* Count is total CSRC items */
                    i = 0, index=0;
    rohc_bool_t     PS_flag;
    rohc_dpib_t     *p_dpib = ROHC_DSC_GET_DPIB(p_dsc);
    rohc_U8bit_t    index_list[ROHC_MAX_ENTRIES_IN_TT];
                    /*To store in SW*/
    rohc_decomp_ext_hdr_tt_t    *p_tt;

    ROHC_ENTER_FN("parse_p2_ext_hdr_enc_type_0_pkt");

    if(ROHC_MODE_R == p_dsc->d_mode)
    {
        p_tt = p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt;
    }
    else
    {
        p_tt = p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt;
    }

    PS_flag = (rohc_bool_t)ROHC_GET_BIT(*p_compressed_pkt, ROHC_PS_BIT);
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "PS_Flag [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    PS_flag);

    count = ROHC_GET_LOWER_NIBBLE(*p_compressed_pkt);
    if(ROHC_GET_BIT(*p_compressed_pkt, ROHC_GP_BIT))
    {
        /* implies gen_id exists */
        gen_id = *(++p_compressed_pkt);
        (*p_length)++;
        ROHC_LOG(LOG_INFO,
        "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
        "Gen id [%d] received in the packet\n", \
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
        gen_id);
    }
    else
    {
        gen_id = 0xFF;
    }
    if(count == 0)
    {
        (*p_length)++;
        return ROHC_SUCCESS;
    }
    p_compressed_pkt++;
    (*p_length)++;

    if(PS_flag)
    {
        p_item_start = p_compressed_pkt + count;
        *p_length = *p_length + count;

        /* Read XI List as 1 byte */
        for(i=0; i<count; i++)
        {
            index_list[i] = *(p_compressed_pkt + i);
            ROHC_GET_NBITS(index_list[i], 1, 7, index);
        
            /*
            ** START_SPR_1306_FIX : Delayed packets are received from MAC
            ** and due to HFN mismatch at PDCP, ciphering module produces a 
            ** garble data which is not handled at ROHC and it crashes.
            */
            if (index >= ROHC_MAX_ENTRIES_IN_TT)
            {    
                ROHC_LOG(LOG_INFO,
                "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                "Index [%d] received in the packet is out of bound\n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,\
                ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                index);

                ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_0_pkt");
                return ROHC_FAILURE;
            }    
            /* END_SPR_1306_FIX */

            if(ROHC_GET_BIT(index_list[i], 8))
            {
                /* Item in Item List
                 * Hence need to add in new item list and final list */

                /* Updating the new item list */
                if(ROHC_FAILURE == rohc_d_process_p2_new_item(p_entity, p_dsc,
                    (p_ext_hdr_tt + index), p_length, p_ext_hdr_list,
                    p_p_ah, p_p_gre, &p_item_start, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_0_pkt");
                    return ROHC_FAILURE;
                }

                /* Updating the new index list */
                *(p_new_index_list + (*p_new_item_count)) = index;

                /* Increment the new item counter */
                (*p_new_item_count)++;
            }
            else
            {
                /* In Encoding Type 0 this case might never occur */
                /* Item picked from translation table
                 * Hence need to add only in final list */

                /* Note that the item in TT is of type rohc_protocol_node_t */
                if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt + index),
                    p_ext_hdr_list, esp_seq, p_p_ah, p_p_gre, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_0_pkt");
                    return ROHC_FAILURE;
                }

                ROHC_LOG(LOG_INFO,
                "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                "Item[%d] to be picked from TT at index[%d]\n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
                ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                ((rohc_protocol_node_t *)((p_tt + index)->p_item))->protocol,
                index);
            }
            index_list[i] = index;
        }
    }
    else
    {
        p_item_start = p_compressed_pkt + (count/2);
        *p_length = *p_length + (count/2);
        if(count % 2 != 0)
        {   /* Count is odd ,hence 1 more byte is added */
            p_item_start++;
            (*p_length)++;
        }

        /* Read XI list as 4 bits */
        while(i<count)
        {
            /* Read the first 4 bits */
            index_list[i] = ROHC_GET_UPPER_NIBBLE(*p_compressed_pkt);
            ROHC_GET_NBITS(*p_compressed_pkt, 5, 3, index);
            if(ROHC_GET_BIT(*p_compressed_pkt, 8))
            {
                /* Item in Item List
                 * Hence need to add in new item list and final list */

                /* Updating the new item list */
                if(ROHC_FAILURE == rohc_d_process_p2_new_item(p_entity, p_dsc,
                    (p_ext_hdr_tt + index), p_length, p_ext_hdr_list, p_p_ah,
                    p_p_gre, &p_item_start, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_0_pkt");
                    return ROHC_FAILURE;
                }

                /* Updating the new index list */
                *(p_new_index_list + (*p_new_item_count)) = index;

                /* Increment the new item counter */
                (*p_new_item_count)++;
            }
            else
            {
                /* Item picked from Ref List
                 * Hence need to add only in final list */

                if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt + index),
                    p_ext_hdr_list, esp_seq, p_p_ah, p_p_gre, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_0_pkt");
                    return ROHC_FAILURE;
                }

                ROHC_LOG(LOG_INFO,
                "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                "Item[%d] to be picked from ref_list at index[%d]\n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
                ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                ((rohc_protocol_node_t *)((p_tt + index)->p_item))->protocol,
                index);
            }
            index_list[i++] = index;

            /* Now read the next 4 bits */
            if(i >= count) break;
            index_list[i] = ROHC_GET_LOWER_NIBBLE(*p_compressed_pkt);
            ROHC_GET_NBITS(*p_compressed_pkt, 1,3, index);
            if(ROHC_GET_BIT(*p_compressed_pkt, 4))
            {
                /* Item in Item List
                 * Hence need to add in new item list and final list */

                /* Updating the new item list */
                if(ROHC_FAILURE == rohc_d_process_p2_new_item(p_entity, p_dsc,
                    (p_ext_hdr_tt + index), p_length, p_ext_hdr_list, p_p_ah,
                    p_p_gre, &p_item_start, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_0_pkt");
                    return ROHC_FAILURE;
                }

                /* Updating the new index list */
                *(p_new_index_list + (*p_new_item_count)) = index;

                /* Increment the new item counter */
                (*p_new_item_count)++;
            }
            else
            {
                /* Item picked from Ref List
                 * Hence need to add only in final list */

                if(ROHC_FAILURE == rohc_d_pick_item_from_tt(p_tt + index,
                    p_ext_hdr_list, esp_seq, p_p_ah, p_p_gre, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_0_pkt");
                    return ROHC_FAILURE;
                }

                ROHC_LOG(LOG_INFO,
                "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                "Item[%d] to be picked from TT at index[%d]\n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,\
                ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                ((rohc_protocol_node_t *)((p_tt + index)->p_item))->protocol,
                index);
            }
            index_list[i++] = index;

            p_compressed_pkt++;
        }
    }
    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        ((rohc_r_sw_t *)(p_ext_hdr_sw))->rtp_sn_no =
                                    ROHC_DSC_GET_SN_NO(p_dpib);
        ((rohc_r_sw_t *)(p_ext_hdr_sw))->icount = count;
        if(count != 0)
        {
            ROHC_MEMCPY(((rohc_r_sw_t *)(p_ext_hdr_sw))->index_list,
                    index_list, count);
        }
    }
    else
    {
        ((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->gen_id = gen_id;
        ((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->icount = count;
        if(count != 0)
        {
            ROHC_MEMCPY(((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->index_list,
                    index_list, count);
        }
    }

    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Total length of the compressed packet [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    *p_length);

    /*
    ** Incrementing Stats - ROHC_FUTURE_STATS
    ** pdcpStats.statsRohc.numberOfListEncNoGenSchmUsedProcessed++;
    */

    ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_0_pkt");
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**          parse_p1_ext_hdr_enc_type_1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to parse an Encoding Type 1 packet
**      ARGUMENTS:
**          The compressor context, buffer etc. are taken as input
**      RETURN VALUE:
**          void
**      NOTES:
**          p_length is length of compressed pkt
*****************************************************************************/

rohc_return_t parse_p1_ext_hdr_enc_type_1_pkt   ROHC_PARAMS
(
    (p_entity,p_dsc,p_compressed_pkt,p_length,p_new_item_count, p_ext_hdr_list,
    p_ext_hdr_tt, p_new_index_list, esp_seq,p_ext_hdr_sw, p_p_ah, p_p_gre, p_ecode),
    rohc_entity_t               *p_entity           _AND_
    rohc_dsc_rtp_t              *p_dsc              _AND_
    rohc_U8bit_t                *p_compressed_pkt   _AND_
    rohc_U32bit_t               *p_length           _AND_
    rohc_U8bit_t                *p_new_item_count   _AND_
    rohc_list_t                 *p_ext_hdr_list     _AND_
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       _AND_
    rohc_U8bit_t                *p_new_index_list   _AND_
    rohc_U32bit_t               esp_seq             _AND_
    rohc_U8bit_t                *p_ext_hdr_sw       _AND_
    rohc_ah_t                   **p_p_ah            _AND_
    rohc_gre_t                  **p_p_gre           _AND_
    rohc_error_code_t           *p_ecode
)
{
    rohc_U8bit_t    gen_id, ref_id, l_insert = 0, 
                    ref_index = 0, i = 0;
    rohc_U16bit_t   insert_bit_mask = 0;
    rohc_U8bit_t    count = 0; 
                    /* Count is total New CSRC items i.e total number 
                     * of 1's in insertion bit mask */
    rohc_bool_t     PS_flag;
    rohc_dpib_t     *p_dpib = ROHC_DSC_GET_DPIB(p_dsc);
    /* Klocwork Fix */
    rohc_U8bit_t    index_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] = {0}; 
                    /* To store in SW */
    rohc_decomp_ext_hdr_tt_t    *p_tt;
    rohc_r_sw_t     *p_ext_hdr_ref_sw;
    rohc_return_t   returnVal = ROHC_SUCCESS;

    ROHC_ENTER_FN("parse_p1_ext_hdr_enc_type_1_pkt");

    PS_flag = (rohc_bool_t)ROHC_GET_BIT(*p_compressed_pkt, ROHC_PS_BIT);
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Value of PS Flag [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    PS_flag);

    if(ROHC_GET_BIT(*p_compressed_pkt, ROHC_GP_BIT))
    {
        /* Gen ID exists */
        gen_id = *(p_compressed_pkt + 1);
        p_compressed_pkt++;
        (*p_length)++;
        ROHC_LOG(LOG_INFO,
        "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
        "Value of gen_id [%d]\n", \
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
        gen_id);
    }
    else
    {
        gen_id = 0xFF;
    }
    
    ref_id = *(p_compressed_pkt + 1);
    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        p_tt = p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt;
        /* Coverity 73152 Fix Start*/
        if(ref_id >= ROHC_MAX_ENTRIES_IN_TT)
        {
            return ROHC_FAILURE;
        }
        /* Coverity 73152 Fix End*/

        ref_index = p_dsc->map_r_ext_ref_id_n_idx[ref_id];
        p_ext_hdr_ref_sw = &(ROHC_DSC_GET_R_EXT_SW(p_dsc, ref_index));
    }
    else
    {   /* U/O Mode */
        p_tt = p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt;
        ref_index = p_dsc->map_uo_ext_ref_id_n_idx[ref_id];
        p_ext_hdr_ref_sw = (rohc_r_sw_t *)
                           (&(ROHC_DSC_GET_UO_EXT_SW(p_dsc, ref_index)));
    }
    p_dpib->ext_hdr_ref_index = ref_index;
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Value of ref_index [%d] inferred from ref_id [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    ref_index, ref_id);

    /* Read Insertion bit mask */
    if(ROHC_GET_BIT(*(p_compressed_pkt + 2), 1))
    {
        /* Insertion Bit mask is 16 bits */
        ROHC_GET_NBITS(*(p_compressed_pkt + 2), 1, 15, insert_bit_mask);
        l_insert = 15;
        p_compressed_pkt = p_compressed_pkt + 4;
        *p_length = *p_length + 4;
    }
    else
    {
        /* Insertion Bit mask is 8 bits */
        ROHC_GET_NBITS(*(p_compressed_pkt + 2), 1, 7, insert_bit_mask);
        l_insert = 7;
        p_compressed_pkt = p_compressed_pkt + 3;
        *p_length = *p_length + 3;
    }
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Value of insertion bit mask [%d] of length[%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    insert_bit_mask, l_insert);

    /* Find the number of 1 in insertion bit mask to get the
     * length of the XI List */
    for(i=1; i<=l_insert; i++)
    {
        if(ROHC_GET_BIT(insert_bit_mask, i))
            count++;
    }
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Number of 1s received in insertion bit mask [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    count);

    p_compressed_pkt++;
    i = 0;
    if(PS_flag)
    {

    /* Coverity 54507 */
        returnVal = rohcParseP1ExtHdrEncType1PSflagOn(p_entity,p_dsc,
                p_compressed_pkt, p_length,p_new_item_count, p_ext_hdr_list,
                p_ext_hdr_tt, p_new_index_list, esp_seq, p_p_ah, p_p_gre,
                p_ext_hdr_ref_sw, p_tt, count, l_insert, insert_bit_mask , p_ecode);
    /* Coverity 54507 */
        if (ROHC_FAILURE == returnVal)
        {
            return ROHC_FAILURE;
        }
    }
    else
    {
        /* Klocwork fix */
        returnVal = rohcParseP1ExtHdrEncType1PSflagOff(p_entity,p_dsc,
                p_compressed_pkt, p_length,p_new_item_count, p_ext_hdr_list,
                p_ext_hdr_tt, p_new_index_list, esp_seq, p_p_ah, p_p_gre,
                p_ext_hdr_ref_sw, p_tt, count, l_insert, insert_bit_mask, p_ecode);

        if (ROHC_FAILURE == returnVal)
        {
            return ROHC_FAILURE;
        }
    }

    /* Update the temp context of SW in DPIB */
    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        ((rohc_r_sw_t *)(p_ext_hdr_sw))->rtp_sn_no =
                                ROHC_DSC_GET_SN_NO(p_dpib);
        ((rohc_r_sw_t *)(p_ext_hdr_sw))->icount = i;
        
        ROHC_MEMCPY(((rohc_r_sw_t *)(p_ext_hdr_sw))->index_list,
                    index_list, i);
    }
    else
    {
        /* U/O Mode */
        ((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->gen_id = gen_id;
        ((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->icount = i;
        ROHC_MEMCPY(((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->index_list,
                    index_list, i);
    }

    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Total length of the compressed packet [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    *p_length);

    /*
    ** Incrementing Stats - ROHC_FUTURE_STATS
    ** pdcpStats.statsRohc.numberOfListEncNoInsOnlySchmUsedProcessed++;
    */

    ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_1_pkt");
    return ROHC_SUCCESS;
}



/*****************************************************************************
**      FUNCTION:
**          rohcParseP1ExtHdrEncType1PSflagOff
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to parse an Encoding Type 1 packet
**      ARGUMENTS:
**          The compressor context, buffer etc. are taken as input
**      RETURN VALUE:
**          void
**      NOTES:
**          p_length is length of compressed pkt
*****************************************************************************/
static rohc_return_t rohcParseP1ExtHdrEncType1PSflagOff   ROHC_PARAMS
(
    (p_entity,p_dsc,p_compressed_pkt,p_length,p_new_item_count, p_ext_hdr_list,
    p_ext_hdr_tt, p_new_index_list, esp_seq, p_p_ah, p_p_gre, 
    p_ext_hdr_ref_sw, p_tt, count, l_insert, insert_bit_mask, p_ecode),
    rohc_entity_t               *p_entity           _AND_
    rohc_dsc_rtp_t              *p_dsc              _AND_
    rohc_U8bit_t                *p_compressed_pkt   _AND_
    rohc_U32bit_t               *p_length           _AND_
    rohc_U8bit_t                *p_new_item_count   _AND_
    rohc_list_t                 *p_ext_hdr_list     _AND_
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       _AND_
    rohc_U8bit_t                *p_new_index_list   _AND_
    rohc_U32bit_t               esp_seq             _AND_
    rohc_ah_t                   **p_p_ah            _AND_
    rohc_gre_t                  **p_p_gre           _AND_
    rohc_r_sw_t                 *p_ext_hdr_ref_sw   _AND_
    rohc_decomp_ext_hdr_tt_t    *p_tt               _AND_
    rohc_U8bit_t                count               _AND_
    rohc_U8bit_t                l_insert            _AND_
    rohc_U16bit_t               insert_bit_mask     _AND_
    rohc_error_code_t           *p_ecode
)
{
    rohc_U8bit_t    *p_item_start = ROHC_NULL;
    rohc_U8bit_t    i = 0, j = 0;
    rohc_U8bit_t    index_list[ROHC_MAX_ENTRIES_IN_CSRC_TT]; 
                    /* To store in SW */
    rohc_bool_t     first_XI = ROHC_TRUE;
    rohc_U8bit_t    *p_start = p_compressed_pkt;
    rohc_U32bit_t     XI_place = 4; /* In case of 4 bit XI , this field indicates
                                   * from where to read XI List */

    /* XI list is of 4 bits */
    p_item_start = p_compressed_pkt + (count/2);
    *p_length = *p_length + (count/2);
    if((count % 2 != 0) && (count != 1))
    {
        p_item_start++;
        (*p_length)++;
    }

    first_XI = ROHC_TRUE; /* as first XI is present in 1st byte */

    for(j=l_insert; ((j>0) && (*p_new_item_count != count)); j--)
    {
        if(ROHC_GET_BIT(insert_bit_mask, j))
        {
            /* Insert bit is 1, hence index needs to be picked from
             * XI List which is 4 bit */

            /* The first item is present in the first byte */
            if(first_XI == ROHC_TRUE)
            {
                /* Indicates item needs to be picked from XI 1 */
                ROHC_GET_NBITS(*p_start, 1, 3, index_list[i]);
                if(ROHC_GET_BIT(*p_start, 4))
                {
                    /* X = 1, hence item present in item list */
                    /* Update the new item TT */
                    if(ROHC_FAILURE == rohc_d_process_p1_new_item(p_entity,
                                p_dsc, (p_ext_hdr_tt + index_list[i]), p_length, 
                                p_ext_hdr_list,p_p_ah, p_p_gre, &p_item_start, p_ecode))
                    {
                        ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_1_pkt");
                        return ROHC_FAILURE;
                    }

                    /* Add the index in the new index list */
                    *(p_new_index_list + (*p_new_item_count))=index_list[i];

                    /* Increment the new item count */
                    (*p_new_item_count)++;
                }
                else
                {
                    /* X = 0, item in TT */
                    if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt + 
                                    index_list[i]), p_ext_hdr_list, esp_seq, p_p_ah,
                                p_p_gre, p_ecode))
                    {
                        ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_1_pkt");
                        return ROHC_FAILURE;
                    }

                    ROHC_LOG(LOG_INFO,
                            "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                            "Item[%d] to be picked from TT at index[%d]\n", \
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId, 
                            ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                            ((rohc_protocol_node_t *)((p_tt + index_list[i])->
                                p_item))->protocol, index_list[i]);
                }
                first_XI = ROHC_FALSE;
            }
            ROHC_GET_NBITS(*p_compressed_pkt,
                    XI_place+1, 3, index_list[i]);
            if(ROHC_GET_BIT(*p_compressed_pkt, XI_place + 4))
            {
                /* X = 1, hence item present in item list */
                if(ROHC_FAILURE == rohc_d_process_p1_new_item(p_entity, p_dsc,
                            (p_ext_hdr_tt + index_list[i]), p_length, p_ext_hdr_list, 
                            p_p_ah, p_p_gre, &p_item_start, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_1_pkt");
                    return ROHC_FAILURE;
                }

                /* Add the index in the new index list */
                *(p_new_index_list + (*p_new_item_count)) = index_list[i];

                /* Increment the new item count */
                (*p_new_item_count)++;
            }
            else
            {
                /* X = 0, hence item present in TT */
                if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt +
                                index_list[i]), p_ext_hdr_list, esp_seq, p_p_ah,
                            p_p_gre, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_1_pkt");
                    return ROHC_FAILURE;
                }

                ROHC_LOG(LOG_INFO,
                        "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                        "Item[%d] to be picked from TT at index[%d]\n", \
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, \
                        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                        ((rohc_protocol_node_t *)((p_tt + index_list[i])->p_item))->
                        protocol, index_list[i]);

            }
            XI_place = ROHC_ABS(XI_place - 4);
            if(XI_place != 0)   p_compressed_pkt++;
        }
        else
        {
            /* Read the index from the Ref List in sliding window */

            index_list[i] = p_ext_hdr_ref_sw->index_list[i];

            /* Add the item in the final Item list */

            if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt +
                            index_list[i]),p_ext_hdr_list,esp_seq,p_p_ah,p_p_gre,p_ecode))
            {
                ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_1_pkt");
                return ROHC_FAILURE;
            }

            ROHC_LOG(LOG_INFO,
                    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                    "Item[%d] to be picked from ref_list at index[%d]\n", \
                    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                    ((rohc_protocol_node_t *)((p_tt + index_list[i])->p_item))->
                    protocol, index_list[i]);
        }
        i++;
    } /* end of for */
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**          rohcParseP1ExtHdrEncType1PSflagOn
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to parse an Encoding Type 1 packet
**      ARGUMENTS:
**          The compressor context, buffer etc. are taken as input
**      RETURN VALUE:
**          void
**      NOTES:
**          p_length is length of compressed pkt
*****************************************************************************/
static rohc_return_t rohcParseP1ExtHdrEncType1PSflagOn   ROHC_PARAMS
(
    (p_entity,p_dsc,p_compressed_pkt,p_length,p_new_item_count, p_ext_hdr_list,
    p_ext_hdr_tt, p_new_index_list, esp_seq, p_p_ah, p_p_gre,p_ext_hdr_ref_sw,
    p_tt, count, l_insert,insert_bit_mask , p_ecode),
    rohc_entity_t               *p_entity           _AND_
    rohc_dsc_rtp_t              *p_dsc              _AND_
    rohc_U8bit_t                *p_compressed_pkt   _AND_
    rohc_U32bit_t               *p_length           _AND_
    rohc_U8bit_t                *p_new_item_count   _AND_
    rohc_list_t                 *p_ext_hdr_list     _AND_
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       _AND_
    rohc_U8bit_t                *p_new_index_list   _AND_
    rohc_U32bit_t               esp_seq             _AND_
    rohc_ah_t                   **p_p_ah            _AND_
    rohc_gre_t                  **p_p_gre           _AND_
    rohc_r_sw_t                 *p_ext_hdr_ref_sw   _AND_
    rohc_decomp_ext_hdr_tt_t    *p_tt               _AND_
    rohc_U8bit_t                count               _AND_
    rohc_U8bit_t                l_insert            _AND_
    /* Coverity 54507 */
    rohc_U16bit_t               insert_bit_mask     _AND_ 
    /* Coverity 54507 */
    rohc_error_code_t           *p_ecode
)
{
    rohc_U8bit_t    *p_item_start = ROHC_NULL, 
                    ref_counter = 0;
    rohc_U8bit_t    i = 0, j = 0;
    rohc_U8bit_t    index_list[ROHC_MAX_ENTRIES_IN_CSRC_TT]; 
                    /* To store in SW */

    /* XI list is of 8 bits */

    p_item_start = p_compressed_pkt + count;
    *p_length = *p_length + count;

    for(j=l_insert; ((j>0) && (*p_new_item_count != count)); j--)
    {
        if(ROHC_GET_BIT(insert_bit_mask, j))
        {
            /* Insert bit is 1, hence index needs to be picked from
             * XI List which is 8 bit */

            ROHC_GET_NBITS(*(p_compressed_pkt + i), 1, 7, index_list[i]);
            if(ROHC_GET_BIT(*(p_compressed_pkt + i), 1))
            {
                /* X = 1, indicates item present in item list */

                /* New item, hence add in new item TT */
                if(ROHC_FAILURE == rohc_d_process_p1_new_item(p_entity,p_dsc,
                            (p_ext_hdr_tt + index_list[i]),p_length, p_ext_hdr_list,
                            p_p_ah, p_p_gre, &p_item_start, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_1_pkt");
                    return ROHC_FAILURE;
                }

                /* Add the index in the new index list */
                *(p_new_index_list + (*p_new_item_count)) = index_list[i];

                /* Increment the new item count */
                (*p_new_item_count)++;
            }
            else
            {
                /* X = 0, indicates item present in translation table
                 * hence only need to add in final ext hdr list */

                if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt + 
                                index_list[i]), p_ext_hdr_list, esp_seq, p_p_ah, 
                            p_p_gre, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_1_pkt");
                    return ROHC_FAILURE;
                }

                ROHC_LOG(LOG_INFO,
                        "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                        "Item[%d] to be picked from TT at index[%d]\n", \
                        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                        ((rohc_protocol_node_t *)((p_tt+index_list[i])->p_item))->
                        protocol, index_list[i]);
            }
        }
        else
        {
            /* Insert bit is 0 hence Read the index from the Ref List */

            index_list[i] = p_ext_hdr_ref_sw->index_list[ref_counter];
            ref_counter++;

            /* Add in the item in the final ext hdr list */
            if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt + 
                            index_list[i]),p_ext_hdr_list,esp_seq,p_p_ah,p_p_gre,p_ecode))
            {
                ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_1_pkt");
                return ROHC_FAILURE;
            }

            ROHC_LOG(LOG_INFO,
                    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                    "Item[%d] to be picked from ref_list at index[%d]\n", \
                    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                    ((rohc_protocol_node_t *)((p_tt + index_list[i])->p_item))->
                    protocol, index_list[i]);
        }
        i++;
    } /* end of for */
    return ROHC_SUCCESS;
}
/*****************************************************************************
**      FUNCTION:
**          parse_p2_ext_hdr_enc_type_1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to parse an Encoding Type 1 packet
**      ARGUMENTS:
**          The compressor context, buffer etc. are taken as input
**      RETURN VALUE:
**          void
**      NOTES:
**          p_length is length of compressed pkt
*****************************************************************************/

rohc_return_t parse_p2_ext_hdr_enc_type_1_pkt   ROHC_PARAMS
(
    (p_entity,p_dsc,p_compressed_pkt,p_length,p_new_item_count, p_ext_hdr_list,
    p_ext_hdr_tt, p_new_index_list, esp_seq,p_ext_hdr_sw, p_p_ah, p_p_gre, p_ecode),
    rohc_entity_t               *p_entity           _AND_
    rohc_dsc_udp_t              *p_dsc              _AND_
    rohc_U8bit_t                *p_compressed_pkt   _AND_
    rohc_U32bit_t               *p_length           _AND_
    rohc_U8bit_t                *p_new_item_count   _AND_
    rohc_list_t                 *p_ext_hdr_list     _AND_
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       _AND_
    rohc_U8bit_t                *p_new_index_list   _AND_
    rohc_U32bit_t               esp_seq             _AND_
    rohc_U8bit_t                *p_ext_hdr_sw       _AND_
    rohc_ah_t                   **p_p_ah            _AND_
    rohc_gre_t                  **p_p_gre           _AND_
    rohc_error_code_t           *p_ecode
)
{
    rohc_U8bit_t    gen_id, ref_id, l_insert, 
                    ref_index = 0;
    rohc_U16bit_t   insert_bit_mask = 0;
    rohc_U8bit_t    count = 0, 
                    /* Count is total New CSRC items i.e total number 
                     * of 1's in insertion bit mask */
                    i = 0;
    rohc_bool_t     PS_flag;
    rohc_dpib_t     *p_dpib = ROHC_DSC_GET_DPIB(p_dsc);
    /* Klocwork Fix */
    rohc_U8bit_t    index_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] = {0}; 
                    /* To store in SW */
    rohc_decomp_ext_hdr_tt_t    *p_tt;
    rohc_r_sw_t     *p_ext_hdr_ref_sw;
    rohc_return_t   returnVal = ROHC_SUCCESS;

    ROHC_ENTER_FN("parse_p2_ext_hdr_enc_type_1_pkt");

    PS_flag = (rohc_bool_t)ROHC_GET_BIT(*p_compressed_pkt, ROHC_PS_BIT);
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Value of PS Flag [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    PS_flag);

    if(ROHC_GET_BIT(*p_compressed_pkt, ROHC_GP_BIT))
    {
        /* Gen ID exists */
        gen_id = *(p_compressed_pkt + 1);
        p_compressed_pkt++;
        (*p_length)++;
        ROHC_LOG(LOG_INFO,
        "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
        "Value of gen_id [%d]\n", \
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
        gen_id);
    }
    else
    {
        gen_id = 0xFF;
    }
    
    ref_id = *(p_compressed_pkt + 1);
    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        p_tt = p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt;
        /* Coverity 72909 Fix Start*/
        if(ref_id >= ROHC_MAX_ENTRIES_IN_TT)
        {
            return ROHC_FAILURE;
        }
        /* Coverity 72909 Fix End*/
        ref_index = p_dsc->map_r_ext_ref_id_n_idx[ref_id];
        p_ext_hdr_ref_sw = &(ROHC_DSC_GET_R_EXT_SW(p_dsc, ref_index));
    }
    else
    {   /* U/O Mode */
        p_tt = p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt;
        ref_index = p_dsc->map_uo_ext_ref_id_n_idx[ref_id];
        p_ext_hdr_ref_sw = (rohc_r_sw_t *)
                           (&(ROHC_DSC_GET_UO_EXT_SW(p_dsc, ref_index)));
    }
    p_dpib->ext_hdr_ref_index = ref_index;
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Value of ref_index [%d] inferred from ref_id [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    ref_index, ref_id);

    /* Read Insertion bit mask */
    if(ROHC_GET_BIT(*(p_compressed_pkt + 2), 1))
    {
        /* Insertion Bit mask is 16 bits */
        ROHC_GET_NBITS(*(p_compressed_pkt + 2), 1, 15, insert_bit_mask);
        l_insert = 15;
        p_compressed_pkt = p_compressed_pkt + 4;
        *p_length = *p_length + 4;
    }
    else
    {
        /* Insertion Bit mask is 8 bits */
        ROHC_GET_NBITS(*(p_compressed_pkt + 2), 1, 7, insert_bit_mask);
        l_insert = 7;
        p_compressed_pkt = p_compressed_pkt + 3;
        *p_length = *p_length + 3;
    }
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Value of insertion bit mask [%d] of length[%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    insert_bit_mask, l_insert);

    /* Find the number of 1 in insertion bit mask to get the
     * length of the XI List */
    for(i=1; i<=l_insert; i++)
    {
        if(ROHC_GET_BIT(insert_bit_mask, i))
            count++;
    }
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Number of 1s received in insertion bit mask [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    count);

    p_compressed_pkt++;
    i = 0;
    if(PS_flag)
    {

    /* Coverity 54510 */
        returnVal = rohcParseP2ExtHdrEncType1PSflagOn(p_entity,p_dsc,
                p_compressed_pkt, p_length,p_new_item_count, p_ext_hdr_list,
                p_ext_hdr_tt, p_new_index_list, esp_seq, p_p_ah, p_p_gre,
                p_ext_hdr_ref_sw, p_tt, count,l_insert, insert_bit_mask , p_ecode);
    /* Coverity 54510 */
        if (ROHC_FAILURE == returnVal)
        {
            return ROHC_FAILURE;
        }
    }
    else
    {
        /* Klocwork fix */
        returnVal = rohcParseP2ExtHdrEncType1PSflagOff(p_entity,p_dsc,
                p_compressed_pkt, p_length,p_new_item_count, p_ext_hdr_list,
                p_ext_hdr_tt, p_new_index_list, esp_seq, p_p_ah, p_p_gre,
                p_ext_hdr_ref_sw, p_tt, count, l_insert, insert_bit_mask, p_ecode);

        if (ROHC_FAILURE == returnVal)
        {
            return ROHC_FAILURE;
        }
    } 

    /* Update the temp context of SW in DPIB */
    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        ((rohc_r_sw_t *)(p_ext_hdr_sw))->rtp_sn_no =
                                ROHC_DSC_GET_SN_NO(p_dpib);
        ((rohc_r_sw_t *)(p_ext_hdr_sw))->icount = i;
        
        ROHC_MEMCPY(((rohc_r_sw_t *)(p_ext_hdr_sw))->index_list,
                    index_list, i);
    }
    else
    {
        /* U/O Mode */
        ((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->gen_id = gen_id;
        ((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->icount = i;
        ROHC_MEMCPY(((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->index_list,
                    index_list, i);
    }

    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Total length of the compressed packet [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    *p_length);

    /*
    ** Incrementing Stats - ROHC_FUTURE_STATS
    ** pdcpStats.statsRohc.numberOfListEncNoInsOnlySchmUsedProcessed++;
    */

    ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_1_pkt");
    return ROHC_SUCCESS;
}


/*****************************************************************************
**      FUNCTION:
**          rohcParseP1ExtHdrEncType1PSflagOff
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to parse an Encoding Type 1 packet
**      ARGUMENTS:
**          The compressor context, buffer etc. are taken as input
**      RETURN VALUE:
**          void
**      NOTES:
**          p_length is length of compressed pkt
*****************************************************************************/
static rohc_return_t rohcParseP2ExtHdrEncType1PSflagOff   ROHC_PARAMS
(
    (p_entity,p_dsc,p_compressed_pkt,p_length,p_new_item_count, p_ext_hdr_list,
    p_ext_hdr_tt, p_new_index_list, esp_seq, p_p_ah, p_p_gre, p_ext_hdr_ref_sw,
    p_tt, count, l_insert, insert_bit_mask, p_ecode),
    rohc_entity_t               *p_entity           _AND_
    rohc_dsc_udp_t              *p_dsc              _AND_
    rohc_U8bit_t                *p_compressed_pkt   _AND_
    rohc_U32bit_t               *p_length           _AND_
    rohc_U8bit_t                *p_new_item_count   _AND_
    rohc_list_t                 *p_ext_hdr_list     _AND_
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       _AND_
    rohc_U8bit_t                *p_new_index_list   _AND_
    rohc_U32bit_t               esp_seq             _AND_
    rohc_ah_t                   **p_p_ah            _AND_
    rohc_gre_t                  **p_p_gre           _AND_
    rohc_r_sw_t                 *p_ext_hdr_ref_sw   _AND_
    rohc_decomp_ext_hdr_tt_t    *p_tt               _AND_
    rohc_U8bit_t                count               _AND_
    rohc_U8bit_t                l_insert            _AND_
    rohc_U16bit_t               insert_bit_mask     _AND_
    rohc_error_code_t           *p_ecode
)
{
    rohc_U8bit_t    *p_item_start = ROHC_NULL; 
    rohc_U8bit_t    i = 0, j = 0;
    rohc_U8bit_t    index_list[ROHC_MAX_ENTRIES_IN_CSRC_TT]; 
                    /* To store in SW */
    rohc_bool_t     first_XI = ROHC_TRUE;
    rohc_U8bit_t    *p_start = p_compressed_pkt;
    rohc_U8bit_t    XI_place = 4; /* In case of 4 bit XI , this field indicates
                                    from where to read XI List */

    /* XI list is of 4 bits */
    p_item_start = p_compressed_pkt + (count/2);
    *p_length = *p_length + (count/2);
    if((count % 2 != 0) && (count != 1))
    {
        p_item_start++;
        (*p_length)++;
    }

    first_XI = ROHC_TRUE; /* as first XI is present in 1st byte */

    for(j=l_insert; ((j>0) && (*p_new_item_count != count)); j--)
    {
        if(ROHC_GET_BIT(insert_bit_mask, j))
        {
            /* Insert bit is 1, hence index needs to be picked from
             * XI List which is 4 bit */

            /* The first item is present in the first byte */
            if(first_XI == ROHC_TRUE)
            {
                /* Indicates item needs to be picked from XI 1 */
                ROHC_GET_NBITS(*p_start, 1, 3, index_list[i]);
                if(ROHC_GET_BIT(*p_start, 4))
                {
                    /* X = 1, hence item present in item list */
                    /* Update the new item TT */
                    if(ROHC_FAILURE == rohc_d_process_p2_new_item(p_entity,
                                p_dsc, (p_ext_hdr_tt + index_list[i]), p_length, 
                                p_ext_hdr_list, p_p_ah, p_p_gre, &p_item_start, p_ecode))
                    {
                        ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_1_pkt");
                        return ROHC_FAILURE;
                    }

                    /* Add the index in the new index list */
                    *(p_new_index_list + (*p_new_item_count))=index_list[i];

                    /* Increment the new item count */
                    (*p_new_item_count)++;
                }
                else
                {
                    /* X = 0, item in TT */
                    if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt + 
                                    index_list[i]), p_ext_hdr_list, esp_seq, p_p_ah,
                                p_p_gre, p_ecode))
                    {
                        ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_1_pkt");
                        return ROHC_FAILURE;
                    }

                    ROHC_LOG(LOG_INFO,
                            "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                            "Item[%d] to be picked from TT at index[%d]\n", \
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId, \
                            ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                            ((rohc_protocol_node_t *)((p_tt + index_list[i])->
                                p_item))->protocol, index_list[i]);
                }
                first_XI = ROHC_FALSE;
            }
            ROHC_GET_NBITS(*p_compressed_pkt,
                    XI_place+1, 3, index_list[i]);
            if(ROHC_GET_BIT(*p_compressed_pkt, XI_place + 4))
            {
                /* X = 1, hence item present in item list */
                if(ROHC_FAILURE == rohc_d_process_p2_new_item(p_entity, 
                            p_dsc,(p_ext_hdr_tt + index_list[i]), p_length, 
                            p_ext_hdr_list, p_p_ah, p_p_gre, &p_item_start, 
                            p_ecode))
                {
                    ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_1_pkt");
                    return ROHC_FAILURE;
                }

                /* Add the index in the new index list */
                *(p_new_index_list + (*p_new_item_count)) = index_list[i];

                /* Increment the new item count */
                (*p_new_item_count)++;
            }
            else
            {
                /* X = 0, hence item present in TT */
                if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt +
                                index_list[i]), p_ext_hdr_list, esp_seq, p_p_ah,
                            p_p_gre, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_1_pkt");
                    return ROHC_FAILURE;
                }

                ROHC_LOG(LOG_INFO,
                        "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                        "Item[%d] to be picked from TT at index[%d]\n", \
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, \
                        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                        ((rohc_protocol_node_t *)((p_tt + index_list[i])->p_item))->
                        protocol, index_list[i]);

            }
            XI_place = ROHC_ABS(XI_place - 4);
            if(XI_place != 0)   p_compressed_pkt++;
        }
        else
        {
            /* Read the index from the Ref List in sliding window */

            index_list[i] = p_ext_hdr_ref_sw->index_list[i];

            /* Add the item in the final Item list */

            if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt +
                            index_list[i]),p_ext_hdr_list,esp_seq,p_p_ah,p_p_gre,p_ecode))
            {
                ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_1_pkt");
                return ROHC_FAILURE;
            }

            ROHC_LOG(LOG_INFO,
                    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                    "Item[%d] to be picked from ref_list at index[%d]\n", \
                    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                    ((rohc_protocol_node_t *)((p_tt + index_list[i])->p_item))->
                    protocol, index_list[i]);
        }
        i++;
    } /* end of for */
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**          rohcParseP2ExtHdrEncType1PSflagOn
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to parse an Encoding Type 1 packet
**      ARGUMENTS:
**          The compressor context, buffer etc. are taken as input
**      RETURN VALUE:
**          void
**      NOTES:
**          p_length is length of compressed pkt
*****************************************************************************/
static rohc_return_t rohcParseP2ExtHdrEncType1PSflagOn   ROHC_PARAMS
(
    (p_entity,p_dsc,p_compressed_pkt,p_length,p_new_item_count, p_ext_hdr_list,
    p_ext_hdr_tt, p_new_index_list, esp_seq, p_p_ah, p_p_gre,p_ext_hdr_ref_sw,
    p_tt, count, l_insert, insert_bit_mask , p_ecode),
    rohc_entity_t               *p_entity           _AND_
    /* + SPR 17439 */
    rohc_dsc_udp_t              *p_dsc              _AND_
    /* - SPR 17439 */
    rohc_U8bit_t                *p_compressed_pkt   _AND_
    rohc_U32bit_t               *p_length           _AND_
    rohc_U8bit_t                *p_new_item_count   _AND_
    rohc_list_t                 *p_ext_hdr_list     _AND_
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       _AND_
    rohc_U8bit_t                *p_new_index_list   _AND_
    rohc_U32bit_t               esp_seq             _AND_
    rohc_ah_t                   **p_p_ah            _AND_
    rohc_gre_t                  **p_p_gre           _AND_
    rohc_r_sw_t                 *p_ext_hdr_ref_sw   _AND_
    rohc_decomp_ext_hdr_tt_t   *p_tt                _AND_
    rohc_U8bit_t                count               _AND_
    rohc_U8bit_t                l_insert            _AND_
    /* Coverity 54510 */
    rohc_U16bit_t               insert_bit_mask     _AND_ 
    /* Coverity 54510 */
    rohc_error_code_t           *p_ecode
)
{
    rohc_U8bit_t    *p_item_start = ROHC_NULL, 
                    ref_counter = 0;
    rohc_U8bit_t    i = 0, j = 0;
    rohc_U8bit_t    index_list[ROHC_MAX_ENTRIES_IN_CSRC_TT]; 
                    /* To store in SW */

    /* XI list is of 8 bits */

    p_item_start = p_compressed_pkt + count;
    *p_length = *p_length + count;

    for(j=l_insert; ((j>0) && (*p_new_item_count != count)); j--)
    {
        if(ROHC_GET_BIT(insert_bit_mask, j))
        {
            /* Insert bit is 1, hence index needs to be picked from
             * XI List which is 8 bit */

            ROHC_GET_NBITS(*(p_compressed_pkt + i), 1, 7, index_list[i]);
            if(ROHC_GET_BIT(*(p_compressed_pkt + i), 1))
            {
                /* X = 1, indicates item present in item list */

                /* New item, hence add in new item TT */
                if(ROHC_FAILURE == rohc_d_process_p2_new_item(p_entity, p_dsc,
                            (p_ext_hdr_tt + index_list[i]),p_length, p_ext_hdr_list, 
                            p_p_ah, p_p_gre, &p_item_start, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_1_pkt");
                    return ROHC_FAILURE;
                }

                /* Add the index in the new index list */
                *(p_new_index_list + (*p_new_item_count)) = index_list[i];

                /* Increment the new item count */
                (*p_new_item_count)++;
            }
            else
            {
                /* X = 0, indicates item present in translation table
                 * hence only need to add in final ext hdr list */

                if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt + 
                                index_list[i]), p_ext_hdr_list, esp_seq, p_p_ah, 
                            p_p_gre, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_1_pkt");
                    return ROHC_FAILURE;
                }

                ROHC_LOG(LOG_INFO,
                        "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                        "Item[%d] to be picked from TT at index[%d]\n", \
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, \
                        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                        ((rohc_protocol_node_t *)((p_tt+index_list[i])->p_item))->
                        protocol, index_list[i]);
            }
        }
        else
        {
            /* Insert bit is 0 hence Read the index from the Ref List */

            index_list[i] = p_ext_hdr_ref_sw->index_list[ref_counter];
            ref_counter++;

            /* Add in the item in the final ext hdr list */
            if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt + 
                            index_list[i]),p_ext_hdr_list,esp_seq,p_p_ah,p_p_gre,p_ecode))
            {
                ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_1_pkt");
                return ROHC_FAILURE;
            }

            ROHC_LOG(LOG_INFO,
                    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                    "Item[%d] to be picked from ref_list at index[%d]\n", \
                    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                    ((rohc_protocol_node_t *)((p_tt + index_list[i])->p_item))->
                    protocol, index_list[i]);
        }
        i++;
    } /* end of for */
    return ROHC_SUCCESS;
}
/*****************************************************************************
**      FUNCTION:
**          parse_p1_ext_hdr_enc_type_2_pkt
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to parse an Encoding Type 2 packet
**      ARGUMENTS:
**          The compressor context, buffer etc. are taken as input
**      RETURN VALUE:
**          void
**      NOTES:
**          p_length is length of compressed pkt
*****************************************************************************/


rohc_return_t   parse_p1_ext_hdr_enc_type_2_pkt    ROHC_PARAMS
(
    (p_entity,p_dsc,p_compressed_pkt, p_length, p_ext_hdr_list, p_ext_hdr_tt, 
    p_new_index_list, esp_seq, p_ext_hdr_sw, p_p_ah, p_p_gre, p_ecode),
    rohc_entity_t               *p_entity           _AND_
    rohc_dsc_rtp_t              *p_dsc              _AND_
    rohc_U8bit_t                *p_compressed_pkt   _AND_
    rohc_U32bit_t               *p_length           _AND_
    rohc_list_t                 *p_ext_hdr_list     _AND_
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       _AND_
    rohc_U8bit_t                *p_new_index_list   _AND_
    rohc_U32bit_t               esp_seq             _AND_
    rohc_U8bit_t                *p_ext_hdr_sw       _AND_
    rohc_ah_t                   **p_p_ah            _AND_
    rohc_gre_t                  **p_p_gre           _AND_
    rohc_error_code_t           *p_ecode
)
{
    rohc_U8bit_t    gen_id, ref_id, i=0, j, l_remove, ref_index;
    rohc_U16bit_t   removal_bit_mask = 0;
    rohc_U8bit_t    final_count = 0; /* Indicates final items in the list */
    /* Klocwork Fix */
    rohc_U8bit_t    index_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] = {0};
    rohc_r_sw_t     *p_ext_hdr_ref_sw; 
    /* This SW is irrespective of mode as both have same structure */
    rohc_decomp_ext_hdr_tt_t   *p_tt;
    rohc_dpib_t     *p_dpib = ROHC_DSC_GET_DPIB(p_dsc);
    rohc_protocol_node_t    *p_ext_hdr_node;

    ROHC_ENTER_FN("parse_p1_ext_hdr_enc_type_2_pkt");

    if(ROHC_GET_BIT(*p_compressed_pkt, ROHC_GP_BIT))
    {
        gen_id = *(p_compressed_pkt + 1);
        p_compressed_pkt++;
        (*p_length)++;

        /* +- SPR 17777 */
/* SPR 20636 Changes Start */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                ROHC_GET_DSC_CID(p_dsc),(ADDR)p_new_index_list,sizeof(p_ext_hdr_tt),0, __func__,"");
/* SPR 20636 Changes End */
        /* +- SPR 17777 */
        ROHC_LOG(LOG_INFO,
        "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
        "Value of gen_id [%d]\n", \
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
        gen_id);
    }
    else
    {
        gen_id = 0xFF;
    }

    ref_id = *(p_compressed_pkt + 1);
    (*p_length)++;
    
    if(ROHC_GET_BIT(*(p_compressed_pkt + 2), 1))
    {
        /* Removal bit mask is 15 bits */
        ROHC_GET_NBITS(*(p_compressed_pkt+2), 1, 15, removal_bit_mask);
        l_remove = 15;
        *p_length = *p_length + 2;
    }
    else
    {
        /* removal bit_mask is 7 bits */
        ROHC_GET_NBITS(*(p_compressed_pkt + 2), 1, 7, removal_bit_mask);
        l_remove = 7;
        (*p_length)++;
    }
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Value of removal bit mask [%d] of length [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    removal_bit_mask, l_remove);

    /* Pick the ref list */
    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        p_tt = p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt;
        /* Coverity 73152 Fix Start*/
        if(ref_id >= ROHC_MAX_ENTRIES_IN_TT)
        {
            return ROHC_FAILURE;
        }
        /* Coverity 73152 Fix End*/


        ref_index = p_dsc->map_r_ext_ref_id_n_idx[ref_id];
        p_ext_hdr_ref_sw = &(ROHC_DSC_GET_R_EXT_SW(p_dsc, ref_index));
    }
    else
    {
        p_tt = p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt;
        ref_index = p_dsc->map_uo_ext_ref_id_n_idx[ref_id];
        p_ext_hdr_ref_sw = (rohc_r_sw_t *)
                           (&(ROHC_DSC_GET_UO_EXT_SW(p_dsc, ref_index)));
    }
    p_dpib->ext_hdr_ref_index = ref_index;

    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Value of ref_index [%d] inferred from ref_id [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    ref_index, ref_id);

    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Count of items present in SW [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    ROHC_GET_LOWER_NIBBLE(*p_compressed_pkt) );

    for(j=l_remove; j>0; j--)
    {
        if (!(ROHC_GET_BIT(removal_bit_mask, j)))
        {
            /* Remove bit 0 indicates pick the item from ref list */

            if(i > p_ext_hdr_ref_sw->icount)
            {
                /* Encountering 0 after the ref list is exhausted
                 * indicates failure in decompression */
                *p_ecode = ROHC_INVALID_BIT_IN_REMOVAL_BIT_MASK;
                return ROHC_FAILURE;
            }

            index_list[final_count] = p_ext_hdr_ref_sw->index_list[i];

            /* Add the item in the final CSRC list */
            p_ext_hdr_node = (rohc_protocol_node_t *)
                        (p_tt + index_list[final_count])->p_item;
            if(ROHC_FAILURE == rohc_d_add_n_update_list(p_ext_hdr_list, 
                p_ext_hdr_node, esp_seq, p_p_ah,p_p_gre, p_ecode))
            {
                ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_2_pkt");
                return ROHC_FAILURE;
            }
            ROHC_LOG(LOG_INFO,
            "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
            "Item[%d] to be picked from ref_list at index[%d]\n", \
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
            ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
            p_ext_hdr_node->protocol, index_list[final_count]);

            final_count++;
        }
        else
        {
            /* Remove bit 1 indicates item need not be picked from ref list */
        }
        i++;
    } /* end of for */

    /* Update the temp context of SW in DPIB */
    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        ((rohc_r_sw_t *)(p_ext_hdr_sw))->rtp_sn_no =
                                    ROHC_DSC_GET_SN_NO(p_dpib);
        ((rohc_r_sw_t *)(p_ext_hdr_sw))->icount = final_count;
        ROHC_MEMCPY(((rohc_r_sw_t *)(p_ext_hdr_sw))->index_list,
                    index_list, final_count);
    }
    else
    {
        /* U/O Mode */
        ((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->gen_id = gen_id;
        ((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->icount = final_count;
        ROHC_MEMCPY(((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->index_list,
                    index_list, final_count);
    }

    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Total length of the compressed packet [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    *p_length);

    /*
    ** Incrementing Stats - ROHC_FUTURE_STATS
    ** pdcpStats.statsRohc.numberOfListEncNoRemOnlySchmUsedProcessed++;
    */

    ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_2_pkt");
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**          parse_p2_ext_hdr_enc_type_2_pkt
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to parse an Encoding Type 2 packet
**      ARGUMENTS:
**          The compressor context, buffer etc. are taken as input
**      RETURN VALUE:
**          void
**      NOTES:
**          p_length is length of compressed pkt
*****************************************************************************/


rohc_return_t   parse_p2_ext_hdr_enc_type_2_pkt    ROHC_PARAMS
(
    (p_entity,p_dsc,p_compressed_pkt, p_length, p_ext_hdr_list, p_ext_hdr_tt, 
    p_new_index_list, esp_seq, p_ext_hdr_sw, p_p_ah, p_p_gre, p_ecode),
    rohc_entity_t               *p_entity           _AND_
    rohc_dsc_udp_t              *p_dsc              _AND_
    rohc_U8bit_t                *p_compressed_pkt   _AND_
    rohc_U32bit_t               *p_length           _AND_
    rohc_list_t                 *p_ext_hdr_list     _AND_
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       _AND_
    rohc_U8bit_t                *p_new_index_list   _AND_
    rohc_U32bit_t               esp_seq             _AND_
    rohc_U8bit_t                *p_ext_hdr_sw       _AND_
    rohc_ah_t                   **p_p_ah            _AND_
    rohc_gre_t                  **p_p_gre           _AND_
    rohc_error_code_t           *p_ecode
)
{
    rohc_U8bit_t    gen_id, ref_id, i=0, j, l_remove, ref_index;
    rohc_U16bit_t   removal_bit_mask = 0;
    rohc_U8bit_t    final_count = 0; /* Indicates final items in the list */
    /* Klocwork Fix */
    rohc_U8bit_t    index_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] = {0};
    rohc_r_sw_t     *p_ext_hdr_ref_sw; 
    /* This SW is irrespective of mode as both have same structure */
    rohc_decomp_ext_hdr_tt_t   *p_tt;
    rohc_dpib_t     *p_dpib = ROHC_DSC_GET_DPIB(p_dsc);
    rohc_protocol_node_t    *p_ext_hdr_node;

    ROHC_ENTER_FN("parse_p2_ext_hdr_enc_type_2_pkt");

    if(ROHC_GET_BIT(*p_compressed_pkt, ROHC_GP_BIT))
    {
        gen_id = *(p_compressed_pkt + 1);
        p_compressed_pkt++;
        (*p_length)++;

        ROHC_LOG(LOG_INFO,
        "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
        "Value of gen_id [%d]\n", \
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
        gen_id);
    }
    else
    {
        gen_id = 0xFF;
    }

    ref_id = *(p_compressed_pkt + 1);
    (*p_length)++;
    
    if(ROHC_GET_BIT(*(p_compressed_pkt + 2), 1))
    {
        /* Removal bit mask is 15 bits */
        ROHC_GET_NBITS(*(p_compressed_pkt+2), 1, 15, removal_bit_mask);
        l_remove = 15;
        *p_length = *p_length + 2;
    }
    else
    {
        /* removal bit_mask is 7 bits */
        ROHC_GET_NBITS(*(p_compressed_pkt + 2), 1, 7, removal_bit_mask);
        l_remove = 7;
        (*p_length)++;
    }
    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),(ADDR)p_new_index_list,sizeof(p_ext_hdr_tt),0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Value of removal bit mask [%d] of length [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    removal_bit_mask, l_remove);

    /* Pick the ref list */
    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        p_tt = p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt;
        /* Coverity 72909 Fix Start*/
        if(ref_id >= ROHC_MAX_ENTRIES_IN_TT)
        {
            return ROHC_FAILURE;
        }
        /* Coverity 72909 Fix End*/

        ref_index = p_dsc->map_r_ext_ref_id_n_idx[ref_id];
        p_ext_hdr_ref_sw = &(ROHC_DSC_GET_R_EXT_SW(p_dsc, ref_index));
    }
    else
    {
        p_tt = p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt;
        ref_index = p_dsc->map_uo_ext_ref_id_n_idx[ref_id];
        p_ext_hdr_ref_sw = (rohc_r_sw_t *)
                           (&(ROHC_DSC_GET_UO_EXT_SW(p_dsc, ref_index)));
    }
    p_dpib->ext_hdr_ref_index = ref_index;

    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Value of ref_index [%d] inferred from ref_id [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    ref_index, ref_id);

    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Count of items present in SW [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    ROHC_GET_LOWER_NIBBLE(*p_compressed_pkt) );

    for(j=l_remove; j>0; j--)
    {
        if (!(ROHC_GET_BIT(removal_bit_mask, j)))
        {
            /* Remove bit 0 indicates pick the item from ref list */

            if(i > p_ext_hdr_ref_sw->icount)
            {
                /* Encountering 0 after the ref list is exhausted
                 * indicates failure in decompression */
                *p_ecode = ROHC_INVALID_BIT_IN_REMOVAL_BIT_MASK;
                return ROHC_FAILURE;
            }

            index_list[final_count] = p_ext_hdr_ref_sw->index_list[i];

            /* Add the item in the final CSRC list */
            p_ext_hdr_node = (rohc_protocol_node_t *)
                        (p_tt + index_list[final_count])->p_item;
            if(ROHC_FAILURE == rohc_d_add_n_update_list(p_ext_hdr_list, 
                p_ext_hdr_node, esp_seq, p_p_ah,p_p_gre, p_ecode))
            {
                ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_2_pkt");
                return ROHC_FAILURE;
            }
            ROHC_LOG(LOG_INFO,
            "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
            "Item[%d] to be picked from ref_list at index[%d]\n", \
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
            ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
            p_ext_hdr_node->protocol, index_list[final_count]);

            final_count++;
        }
        else
        {
            /* Remove bit 1 indicates item need not be picked from ref list */
        }
        i++;
    } /* end of for */

    /* Update the temp context of SW in DPIB */
    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        ((rohc_r_sw_t *)(p_ext_hdr_sw))->rtp_sn_no =
                                    ROHC_DSC_GET_SN_NO(p_dpib);
        ((rohc_r_sw_t *)(p_ext_hdr_sw))->icount = final_count;
        ROHC_MEMCPY(((rohc_r_sw_t *)(p_ext_hdr_sw))->index_list,
                    index_list, final_count);
    }
    else
    {
        /* U/O Mode */
        ((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->gen_id = gen_id;
        ((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->icount = final_count;
        ROHC_MEMCPY(((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->index_list,
                    index_list, final_count);
    }

    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Total length of the compressed packet [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    *p_length);

    /*
    ** Incrementing Stats - ROHC_FUTURE_STATS
    ** pdcpStats.statsRohc.numberOfListEncNoRemOnlySchmUsedProcessed++;
    */

    ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_2_pkt");
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**          parse_p1_ext_hdr_enc_type_3_pkt
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to parse an Encoding Type 3 packet
**      ARGUMENTS:
**          The compressor context, buffer etc. are taken as input
**      RETURN VALUE:
**          void
**      NOTES:
**          p_length is length of compressed pkt
*****************************************************************************/

rohc_return_t parse_p1_ext_hdr_enc_type_3_pkt   ROHC_PARAMS
(
    (p_entity,p_dsc,p_compressed_pkt,p_length,p_new_item_count, p_ext_hdr_list,
    p_ext_hdr_tt, p_new_index_list, esp_seq, p_ext_hdr_sw, p_p_ah, p_p_gre,
    p_ecode),
    rohc_entity_t               *p_entity           _AND_
    rohc_dsc_rtp_t              *p_dsc              _AND_
    rohc_U8bit_t                *p_compressed_pkt   _AND_
    rohc_U32bit_t               *p_length           _AND_
    rohc_U8bit_t                *p_new_item_count   _AND_
    rohc_list_t                 *p_ext_hdr_list     _AND_
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       _AND_
    rohc_U8bit_t                *p_new_index_list   _AND_
    rohc_U32bit_t               esp_seq             _AND_
    rohc_U8bit_t                *p_ext_hdr_sw       _AND_
    rohc_ah_t                   **p_p_ah            _AND_
    rohc_gre_t                  **p_p_gre           _AND_
    rohc_error_code_t           *p_ecode
)
{
    rohc_decomp_ext_hdr_tt_t    *p_tt = ROHC_NULL;
    rohc_r_sw_t     *p_ext_hdr_ref_sw = ROHC_NULL;
    rohc_U8bit_t    gen_id = 0, ref_id = 0, ref_index =0, count = 0;
    rohc_bool_t     PS_flag;
    rohc_U16bit_t   removal_bit_mask = 0, insert_bit_mask = 0, 
                    l_insert = 0, l_remove = 0, i = 0, j=0, final_count = 0;
    rohc_dpib_t     *p_dpib = ROHC_DSC_GET_DPIB(p_dsc);
    rohc_U8bit_t    ref_index_list[ROHC_MAX_ENTRIES_IN_TT] = {0};
    rohc_return_t   returnVal = ROHC_SUCCESS;

    ROHC_ENTER_FN("parse_p1_ext_hdr_enc_type_3_pkt");

    PS_flag = (rohc_bool_t)ROHC_GET_BIT(*p_compressed_pkt, ROHC_PS_BIT);
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "PS_Flag [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    PS_flag);

    
    if(ROHC_GET_BIT(*p_compressed_pkt, ROHC_GP_BIT))
    {   /* GP flag is set */
        gen_id = *(p_compressed_pkt + 1);
        p_compressed_pkt++;
        (*p_length)++;
        ROHC_LOG(LOG_INFO,
        "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
        "Value of gen_id [%d] received \n", \
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
        gen_id);
    }
    else
    {
        gen_id = 0xFF;
    }
    p_compressed_pkt++;
    ref_id = *p_compressed_pkt;
    p_compressed_pkt++;
    *p_length = *p_length + 2;
    
    if(ROHC_GET_BIT(*p_compressed_pkt, 8))
    {   /* removal bit mask is 2 octets */
        ROHC_GET_NBITS(*p_compressed_pkt, 1, 15, removal_bit_mask);
        l_remove = 15;
        p_compressed_pkt = p_compressed_pkt + 2;
        *p_length = *p_length + 2;
    }   
    else
    {   /* removal bit mask is 1 octet */
        ROHC_GET_NBITS(*p_compressed_pkt, 1, 7, removal_bit_mask);
        l_remove = 7;
        p_compressed_pkt++;
        (*p_length)++;
    }
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Value of removal bit mask[%d] of length [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    removal_bit_mask, l_remove);

    if(ROHC_GET_BIT(*p_compressed_pkt, 8))
    {   /* insertion bit mask is 2 octets */
        ROHC_GET_NBITS(*p_compressed_pkt, 1, 15, insert_bit_mask);
        l_insert = 15;
        p_compressed_pkt = p_compressed_pkt + 2;
        *p_length = *p_length + 2;
    }
    else
    {   /* insertion bit mask is 1 octet */
        ROHC_GET_NBITS(*p_compressed_pkt, 1, 7, insert_bit_mask);
        l_insert = 7;
        p_compressed_pkt++;
        (*p_length)++;
    }
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Value of insertion bit mask[%d] of length [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    insert_bit_mask, l_insert);
    
    /* Pick the ref list */
    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        p_tt = p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt;
        /* Coverity 73152 Fix Start*/
        if(ref_id >= ROHC_MAX_ENTRIES_IN_TT)
        {
            return ROHC_FAILURE;
        }
        /* Coverity 73152 Fix End*/

        ref_index = p_dsc->map_r_ext_ref_id_n_idx[ref_id];
        p_ext_hdr_ref_sw = &(ROHC_DSC_GET_R_EXT_SW(p_dsc, ref_index));
    }
    else
    {
        p_tt = p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt;
        ref_index = p_dsc->map_uo_ext_ref_id_n_idx[ref_id];
        p_ext_hdr_ref_sw=(rohc_r_sw_t *)
                         (&(ROHC_DSC_GET_UO_EXT_SW(p_dsc,ref_index)));
    }
    p_dpib->ext_hdr_ref_index = ref_index;
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Value of ref_index[%d] inferred from ref_id [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    ref_index, ref_id);

    /* first apply removal bit mask by scannig from L->R*/
    for(j=l_remove; j > 0; j--)
    {
        if (!(ROHC_GET_BIT(removal_bit_mask, j)))
        {
            /* Pick the item from ref list */

            /* It might be the case that list is finished and 0's are 
             * just appended in the bit mask */

            if(i > p_ext_hdr_ref_sw->icount)
            {
                /* If now 0 is observed in the removal bit mask, then
                 * it is an error and header should not be communicated to
                 * upper layer */
                *p_ecode = ROHC_INVALID_BIT_IN_REMOVAL_BIT_MASK;
                return ROHC_FAILURE;
            }

            ref_index_list[final_count] = p_ext_hdr_ref_sw->index_list[i];
            ROHC_LOG(LOG_INFO,
            "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
            "Index [%d] picked from ref_list\n", \
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
            ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
            ref_index_list[final_count]);

            final_count++;
        }
        else
        {
            /* Remove bit 1 indicates item need not be picked from ref list */
        }
        i++;
    } /* end of for */

    for(i=1; i <= l_insert; i++)
    {
        if(ROHC_GET_BIT(insert_bit_mask, i))
            count++;
    }
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Number of 1s in the insertion bit mask[%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    count);

    /* Now apply insertion bit mask on the index list obtained above by
     * scanning from L->R */
    /*Coverity 73494 Changes Start*/
    /*i = 0;*/
    /*Coverity 73494 Changes End*/
    if(PS_flag)
    {
    /* Coverity 54508 */
        returnVal = rohcParseP1ExtHdrEncType3PSflagOn(p_entity,p_dsc,
                p_compressed_pkt, p_length,p_new_item_count, p_ext_hdr_list,
                p_ext_hdr_tt, p_new_index_list, esp_seq, p_p_ah, p_p_gre,
                p_ext_hdr_ref_sw, p_tt,ref_index_list, count, l_insert, insert_bit_mask,
                p_ecode);
    /* Coverity 54508 */

        if (ROHC_FAILURE == returnVal)
        {
            return ROHC_FAILURE;
        }
    }
    else
    {

    /* Coverity 54511 */
        returnVal = rohcParseP1ExtHdrEncType3PSflagOff(p_entity,p_dsc,
                p_compressed_pkt, p_length,p_new_item_count, p_ext_hdr_list,
                p_ext_hdr_tt, p_new_index_list, esp_seq, p_p_ah, p_p_gre,
                p_ext_hdr_ref_sw, p_tt,ref_index_list, count, l_insert, insert_bit_mask,
                p_ecode);
    /* Coverity 54511 */

        if (ROHC_FAILURE == returnVal)
        {
            return ROHC_FAILURE;
        }
    }

    /* Update the temp context of SW in DPIB */
    if(ROHC_FAILURE == rohcP1UpdateTempContxtSWInDPIB(p_ext_hdr_sw,p_dsc,gen_id))
    {
        return ROHC_FAILURE;
    }
            


    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Total length of the compressed packet [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    *p_length);

    /*
    ** Incrementing Stats - ROHC_FUTURE_STATS
    ** pdcpStats.statsRohc.numberOfListEncNoRemThenInsSchmUsedProcessed++;
    */

    ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_3_pkt");
    return ROHC_SUCCESS;
}


/*****************************************************************************
**      FUNCTION:
**          rohcParseP1ExtHdrEncType3PSflagOff
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to parse an Encoding Type 3 packet
**      ARGUMENTS:
**          The compressor context, buffer etc. are taken as input
**      RETURN VALUE:
**          void
**      NOTES:
**          p_length is length of compressed pkt
*****************************************************************************/
static rohc_return_t rohcParseP1ExtHdrEncType3PSflagOff   ROHC_PARAMS
(
    /* + SPR 17439 */
    (p_entity,p_dsc,p_compressed_pkt,p_length,p_new_item_count, p_ext_hdr_list,
    p_ext_hdr_tt, p_new_index_list, esp_seq, p_p_ah, p_p_gre,p_ext_hdr_ref_sw, p_tt,
    ref_index_list, count, l_insert, insert_bit_mask , p_ecode),
    rohc_entity_t               *p_entity           _AND_
    rohc_dsc_rtp_t              *p_dsc              _AND_
    rohc_U8bit_t                *p_compressed_pkt   _AND_
    rohc_U32bit_t               *p_length           _AND_
    rohc_U8bit_t                *p_new_item_count   _AND_
    rohc_list_t                 *p_ext_hdr_list     _AND_
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       _AND_
    rohc_U8bit_t                *p_new_index_list   _AND_
    rohc_U32bit_t               esp_seq             _AND_
    rohc_ah_t                   **p_p_ah            _AND_
    rohc_gre_t                  **p_p_gre           _AND_
    rohc_r_sw_t                 *p_ext_hdr_ref_sw   _AND_
    /* - SPR 17439 */
    rohc_decomp_ext_hdr_tt_t    *p_tt               _AND_
    rohc_U8bit_t    ref_index_list[ROHC_MAX_ENTRIES_IN_TT] _AND_
    rohc_U8bit_t                count               _AND_
    rohc_U8bit_t                l_insert            _AND_
    /* Coverity 54511 */
    rohc_U16bit_t               insert_bit_mask     _AND_ 
    /* Coverity 54511 */
    rohc_error_code_t           *p_ecode
)
{
    rohc_U8bit_t    *p_item_start = ROHC_NULL; 
    rohc_U8bit_t    i = 0, j = 0;
    rohc_U8bit_t    index_list[ROHC_MAX_ENTRIES_IN_CSRC_TT]; 
    /* To store in SW */
    /* SPR 3444 changes start */
    rohc_bool_t     first_XI = ROHC_TRUE;
    rohc_U8bit_t    *p_start = p_compressed_pkt;
    rohc_U8bit_t     XI_place = 4,ref_counter = 0;

    /* XI field is of 4 bits */
    p_item_start = p_compressed_pkt + (count/2);
    *p_length = *p_length + (count/2);

    first_XI = ROHC_TRUE; /* as first XI is present in 1st byte */

    for(j=l_insert; j>0; j--)
    {
        if(ROHC_GET_BIT(insert_bit_mask, j))
        {
            /* Insert bit is 1, hence index needs to be picked from
             * XI List which is 4 bit */

            /* The first item is present in the first byte */
            if(first_XI == ROHC_TRUE)
            {
                /* Indicates item needs to be picked from XI 1 */
                ROHC_GET_NBITS(*p_start, 1, 3, index_list[i]);
                if(ROHC_GET_BIT(*p_start, 4))
                {
                    /* X = 1, hence item present in item list */
                    /* Update the new item TT */
                    if(ROHC_FAILURE == rohc_d_process_p1_new_item(p_entity,
                                p_dsc, (p_ext_hdr_tt + index_list[i]), p_length, 
                                p_ext_hdr_list,p_p_ah, p_p_gre, &p_item_start, p_ecode))
                    {
                        ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_3_pkt");
                        return ROHC_FAILURE;
                    }

                    /* Add the index in the new index list */
                    *(p_new_index_list + (*p_new_item_count))=index_list[i];

                    /* Increment the new item count */
                    (*p_new_item_count)++;
                }
                else
                {
                    /* X = 0, item in TT */
                    if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt +
                                    index_list[i]), p_ext_hdr_list, esp_seq,
                                p_p_ah, p_p_gre, p_ecode))
                    {
                        ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_3_pkt");
                        return ROHC_FAILURE;
                    }

                    /* +- SPR 17777 */
                    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                            ROHC_GET_DSC_CID(p_dsc),sizeof(p_ext_hdr_ref_sw),0,0, __func__,"");
                    /* +- SPR 17777 */
                    ROHC_LOG(LOG_INFO,
                            "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                            "Item[%d] to be picked from TT at index[%d]\n", \
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId, \
                            ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                            ((rohc_protocol_node_t *)((p_tt + index_list[i])->
                                p_item))->protocol, index_list[i]);
                }
                first_XI = ROHC_FALSE;
            }
            ROHC_GET_NBITS(*p_compressed_pkt,
                    XI_place+1, 3, index_list[i]);
            if(ROHC_GET_BIT(*p_compressed_pkt, XI_place+4))
            {
                /* X = 1, hence item present in item list */
                if(ROHC_FAILURE == rohc_d_process_p1_new_item(p_entity, 
                            p_dsc,(p_ext_hdr_tt + index_list[i]),p_length,
                            p_ext_hdr_list,p_p_ah,p_p_gre, &p_item_start, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_3_pkt");
                    return ROHC_FAILURE;
                }
                /* Add the index in the new index list */
                *(p_new_index_list + (*p_new_item_count))=index_list[i];

                /* Increment the new item count */
                (*p_new_item_count)++;
            }
            else
            {
                /* X = 0, hence item present in TT */
                if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt +
                                index_list[i]), p_ext_hdr_list, esp_seq,
                            p_p_ah, p_p_gre, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_3_pkt");
                    return ROHC_FAILURE;
                }

                ROHC_LOG(LOG_INFO,
                        "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                        "Item[%d] to be picked from TT at index[%d]\n", \
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, \
                        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                        ((rohc_protocol_node_t *)((p_tt + index_list[i])->p_item))->
                        protocol, index_list[i]);
            }
            XI_place = ROHC_ABS(XI_place - 4);
            if(XI_place != 0)   p_compressed_pkt++;
        }
        else
        {   /* Insert bit is 0 */
            /* Read the index from the Ref List in sliding window */

            index_list[i] = ref_index_list[ref_counter];
            ref_counter++;

            /* Add the item in the final list */
            if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt + 
                            index_list[i]), p_ext_hdr_list, esp_seq,
                        p_p_ah, p_p_gre, p_ecode))
            {
                ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_3_pkt");
                return ROHC_FAILURE;
            }

            ROHC_LOG(LOG_INFO,
                    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                    "Item[%d] to be picked from ref_list at index[%d]\n", \
                    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                    ((rohc_protocol_node_t *)((p_tt + index_list[i])->p_item))->
                    protocol, index_list[i]);
        }
        i++;
    } /* end of for */
    return ROHC_SUCCESS;
}
/*****************************************************************************
**      FUNCTION:
**          rohcParseP1ExtHdrEncType3PSflagOn
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to parse an Encoding Type 3 packet
**      ARGUMENTS:
**          The compressor context, buffer etc. are taken as input
**      RETURN VALUE:
**          void
**      NOTES:
**          p_length is length of compressed pkt
*****************************************************************************/
static rohc_return_t rohcParseP1ExtHdrEncType3PSflagOn   ROHC_PARAMS
(
    /* + SPR 17439 */
    (p_entity,p_dsc,p_compressed_pkt,p_length,p_new_item_count, p_ext_hdr_list,
    p_ext_hdr_tt, p_new_index_list, esp_seq, p_p_ah, p_p_gre,p_ext_hdr_ref_sw, p_tt, ref_index_list,
    count, l_insert,insert_bit_mask , p_ecode),
    rohc_entity_t               *p_entity           _AND_
    rohc_dsc_rtp_t              *p_dsc              _AND_
    rohc_U8bit_t                *p_compressed_pkt   _AND_
    rohc_U32bit_t               *p_length           _AND_
    rohc_U8bit_t                *p_new_item_count   _AND_
    rohc_list_t                 *p_ext_hdr_list     _AND_
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       _AND_
    rohc_U8bit_t                *p_new_index_list   _AND_
    rohc_U32bit_t               esp_seq             _AND_
    rohc_ah_t                   **p_p_ah            _AND_
    rohc_gre_t                  **p_p_gre           _AND_
    rohc_r_sw_t                 *p_ext_hdr_ref_sw   _AND_
    /* - SPR 17439 */
    rohc_decomp_ext_hdr_tt_t    *p_tt               _AND_
    rohc_U8bit_t  ref_index_list[ROHC_MAX_ENTRIES_IN_TT] _AND_
    rohc_U8bit_t                count               _AND_
    rohc_U8bit_t                l_insert            _AND_
    /* Coverity 54508 */
    rohc_U16bit_t               insert_bit_mask     _AND_ 
    /* Coverity 54508 */
    rohc_error_code_t           *p_ecode
)
{
    rohc_U8bit_t    *p_item_start = ROHC_NULL; 
    rohc_U8bit_t    i = 0, j = 0;
    rohc_U8bit_t    index_list[ROHC_MAX_ENTRIES_IN_CSRC_TT]; 
                    /* To store in SW */

    /* XI list is of 8 bits */

    p_item_start = p_compressed_pkt + count;
    *p_length = *p_length + count;

    for(j=l_insert; ((j > 0) && (*p_new_item_count != count)); j--)
    {
        if(ROHC_GET_BIT(insert_bit_mask, j))
        {
            /* Insert bit is 1, hence index needs to be picked from
             * XI List which is 8 bit */

            ROHC_GET_NBITS(*(p_compressed_pkt + i), 1, 7, index_list[i]);
            if(ROHC_GET_BIT(*(p_compressed_pkt + i), 8))
            {
                /* X = 1, indicates item present in item list */

                /* New item, hence add in new item TT */
                if(ROHC_FAILURE == rohc_d_process_p1_new_item(p_entity, 
                            p_dsc,(p_ext_hdr_tt + index_list[i]),p_length, 
                            p_ext_hdr_list,p_p_ah,p_p_gre, &p_item_start, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_3_pkt");
                    return ROHC_FAILURE;
                }

                /* Add the index in the new index list */
                *(p_new_index_list + (*p_new_item_count)) = index_list[i];

                /* Increment the new item count */
                (*p_new_item_count)++;
            }
            else
            {
                /* X = 0, indicates item present in translation table
                 * hence only need to add in final ext hdr list */

                if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt +
                                index_list[i]), p_ext_hdr_list, esp_seq, p_p_ah,
                            p_p_gre, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_3_pkt");
                    return ROHC_FAILURE;
                }

                /* +- SPR 17777 */
                LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                        __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                        ROHC_GET_DSC_CID(p_dsc),sizeof(p_ext_hdr_ref_sw),0,0, __func__,"");
                /* +- SPR 17777 */
                ROHC_LOG(LOG_INFO,
                        "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                        "Item[%d] to be picked from TT at index[%d]\n", \
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, \
                        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                        ((rohc_protocol_node_t *)((p_tt + index_list[i])->p_item))->
                        protocol, index_list[i]);
            }
        }
        else
        {   /* Insert bit is 0 */
            /* Read the index from the Ref List in sliding window */

            index_list[i] = ref_index_list[i];

            /* Add in the item in the final ext hdr list */
            if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt +
                            index_list[i]), p_ext_hdr_list, esp_seq, p_p_ah, 
                        p_p_gre, p_ecode))
            {
                ROHC_EXIT_FN("parse_p1_ext_hdr_enc_type_3_pkt");
                return ROHC_FAILURE;
            }

            ROHC_LOG(LOG_INFO,
                    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                    "Item[%d] to be picked from SW at index[%d] in SW\n", \
                    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                    ((rohc_protocol_node_t *)((p_tt + index_list[i])->p_item))->
                    protocol, index_list[i]);
        }
        i++;
    } /* end of for */
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**          parse_p2_ext_hdr_enc_type_3_pkt
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to parse an Encoding Type 3 packet
**      ARGUMENTS:
**          The compressor context, buffer etc. are taken as input
**      RETURN VALUE:
**          void
**      NOTES:
**          p_length is length of compressed pkt
*****************************************************************************/

rohc_return_t parse_p2_ext_hdr_enc_type_3_pkt   ROHC_PARAMS
(
    (p_entity,p_dsc,p_compressed_pkt,p_length,p_new_item_count, p_ext_hdr_list,
    p_ext_hdr_tt, p_new_index_list, esp_seq, p_ext_hdr_sw, p_p_ah, p_p_gre,
    p_ecode),
    rohc_entity_t               *p_entity           _AND_
    rohc_dsc_udp_t              *p_dsc              _AND_
    rohc_U8bit_t                *p_compressed_pkt   _AND_
    rohc_U32bit_t               *p_length           _AND_
    rohc_U8bit_t                *p_new_item_count   _AND_
    rohc_list_t                 *p_ext_hdr_list     _AND_
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       _AND_
    rohc_U8bit_t                *p_new_index_list   _AND_
    rohc_U32bit_t               esp_seq             _AND_
    rohc_U8bit_t                *p_ext_hdr_sw       _AND_
    rohc_ah_t                   **p_p_ah            _AND_
    rohc_gre_t                  **p_p_gre           _AND_
    rohc_error_code_t           *p_ecode
)
{
    rohc_decomp_ext_hdr_tt_t    *p_tt = ROHC_NULL;
    rohc_r_sw_t     *p_ext_hdr_ref_sw = ROHC_NULL;
    rohc_U8bit_t    gen_id = 0, ref_id = 0, ref_index =0, count = 0;
    rohc_bool_t     PS_flag;
    rohc_U16bit_t   removal_bit_mask = 0, insert_bit_mask = 0, 
                    l_insert = 0, l_remove = 0, i = 0, j=0, final_count = 0;
    /* SPR 3444 changes start */
    rohc_U8bit_t ref_index_list[ROHC_MAX_ENTRIES_IN_TT] = {0};
                    /* SPR 3444 changes end */
    rohc_dpib_t *p_dpib = ROHC_DSC_GET_DPIB(p_dsc);
    rohc_return_t   returnVal = ROHC_SUCCESS;
    
    ROHC_ENTER_FN("parse_p2_ext_hdr_enc_type_3_pkt");

    PS_flag = (rohc_bool_t)ROHC_GET_BIT(*p_compressed_pkt, ROHC_PS_BIT);
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "PS_Flag [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    PS_flag);

    
    if(ROHC_GET_BIT(*p_compressed_pkt, ROHC_GP_BIT))
    {   /* GP flag is set */
        gen_id = *(p_compressed_pkt + 1);
        p_compressed_pkt++;
        (*p_length)++;
        ROHC_LOG(LOG_INFO,
        "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
        "Value of gen_id [%d] received \n", \
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
        gen_id);
    }
    else
    {
        gen_id = 0xFF;
    }
    p_compressed_pkt++;
    ref_id = *p_compressed_pkt;
    p_compressed_pkt++;
    *p_length = *p_length + 2;
    
    if(ROHC_GET_BIT(*p_compressed_pkt, 8))
    {   /* removal bit mask is 2 octets */
        ROHC_GET_NBITS(*p_compressed_pkt, 1, 15, removal_bit_mask);
        l_remove = 15;
        p_compressed_pkt = p_compressed_pkt + 2;
        *p_length = *p_length + 2;
    }   
    else
    {   /* removal bit mask is 1 octet */
        ROHC_GET_NBITS(*p_compressed_pkt, 1, 7, removal_bit_mask);
        l_remove = 7;
        p_compressed_pkt++;
        (*p_length)++;
    }
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Value of removal bit mask[%d] of length [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    removal_bit_mask, l_remove);

    if(ROHC_GET_BIT(*p_compressed_pkt, 8))
    {   /* insertion bit mask is 2 octets */
        ROHC_GET_NBITS(*p_compressed_pkt, 1, 15, insert_bit_mask);
        l_insert = 15;
        p_compressed_pkt = p_compressed_pkt + 2;
        *p_length = *p_length + 2;
    }
    else
    {   /* insertion bit mask is 1 octet */
        ROHC_GET_NBITS(*p_compressed_pkt, 1, 7, insert_bit_mask);
        l_insert = 7;
        p_compressed_pkt++;
        (*p_length)++;
    }
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Value of insertion bit mask[%d] of length [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    insert_bit_mask, l_insert);
    
    /* Pick the ref list */
    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        p_tt = p_dsc->rohc_d_r_ext_hdr_tt.d_r_ext_hdr_tt;
        /* Coverity 72909 Fix Start*/
        if(ref_id >= ROHC_MAX_ENTRIES_IN_TT)
        {
            return ROHC_FAILURE;
        }
        /* Coverity 72909 Fix End*/

        ref_index = p_dsc->map_r_ext_ref_id_n_idx[ref_id];
        p_ext_hdr_ref_sw = &(ROHC_DSC_GET_R_EXT_SW(p_dsc, ref_index));
    }
    else
    {
        p_tt = p_dsc->rohc_d_uo_ext_hdr_tt.d_uo_ext_hdr_tt;
        ref_index = p_dsc->map_uo_ext_ref_id_n_idx[ref_id];
        p_ext_hdr_ref_sw=(rohc_r_sw_t *)
                         (&(ROHC_DSC_GET_UO_EXT_SW(p_dsc,ref_index)));
    }
    p_dpib->ext_hdr_ref_index = ref_index;
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Value of ref_index[%d] inferred from ref_id [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    ref_index, ref_id);

    /* first apply removal bit mask by scannig from L->R*/
    for(j=l_remove; j > 0; j--)
    {
        if (!(ROHC_GET_BIT(removal_bit_mask, j)))
        {
            /* Pick the item from ref list */

            /* It might be the case that list is finished and 0's are 
             * just appended in the bit mask */

            if(i > p_ext_hdr_ref_sw->icount)
            {
                /* If now 0 is observed in the removal bit mask, then
                 * it is an error and header should not be communicated to
                 * upper layer */
                *p_ecode = ROHC_INVALID_BIT_IN_REMOVAL_BIT_MASK;
                return ROHC_FAILURE;
            }

            ref_index_list[final_count] = p_ext_hdr_ref_sw->index_list[i];
            ROHC_LOG(LOG_INFO,
            "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
            "Index [%d] picked from ref_list\n", \
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
            ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
            ref_index_list[final_count]);

            final_count++;
        }
        else
        {
            /* Remove bit 1 indicates item need not be picked from ref list */
        }
        i++;
    } /* end of for */

    for(i=1; i <= l_insert; i++)
    {
        if(ROHC_GET_BIT(insert_bit_mask, i))
            count++;
    }
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Number of 1s in the insertion bit mask[%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    count);

    /* Now apply insertion bit mask on the index list obtained above by
     * scanning from L->R */

    if(PS_flag)
    {
        returnVal = rohcParseP2ExtHdrEncType3PSflagOn(p_entity,p_dsc,
                p_compressed_pkt, p_length,p_new_item_count, p_ext_hdr_list,
                p_ext_hdr_tt, p_new_index_list, esp_seq, p_p_ah, p_p_gre,
                p_ext_hdr_ref_sw, p_tt,ref_index_list, count, l_insert,
                insert_bit_mask, p_ecode);

        if (ROHC_FAILURE == returnVal)
        {
            return ROHC_FAILURE;
        }
    }
    else
    {
        /* Klocwork fix */
        returnVal = rohcParseP2ExtHdrEncType3PSflagOff(p_entity,p_dsc,
                p_compressed_pkt, p_length,p_new_item_count, p_ext_hdr_list,
                p_ext_hdr_tt, p_new_index_list, esp_seq, p_p_ah, p_p_gre,
                p_ext_hdr_ref_sw, p_tt,ref_index_list, count, l_insert,insert_bit_mask, p_ecode);

        if (ROHC_FAILURE == returnVal)
        {
            return ROHC_FAILURE;
        }
    }
    /* Update the temp context of SW in DPIB */
    if(ROHC_FAILURE == rohcP2UpdateTempContxtSWInDPIB(p_ext_hdr_sw,p_dsc,gen_id))
    {
        return ROHC_FAILURE;
    }
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Total length of the compressed packet [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    *p_length);

    /*
    ** Incrementing Stats - ROHC_FUTURE_STATS
    ** pdcpStats.statsRohc.numberOfListEncNoRemThenInsSchmUsedProcessed++;
    */

    ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_3_pkt");
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
w
**          rohcParseP2ExtHdrEncType3PSflagOff
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to parse an Encoding Type 3 packet
**      ARGUMENTS:
**          The compressor context, buffer etc. are taken as input
**      RETURN VALUE:
**          void
**      NOTES:
**          p_length is length of compressed pkt
*****************************************************************************/
static rohc_return_t rohcParseP2ExtHdrEncType3PSflagOff   ROHC_PARAMS
(
    /* + SPR 17439 */
    (p_entity,p_dsc,p_compressed_pkt,p_length,p_new_item_count, p_ext_hdr_list,
    p_ext_hdr_tt, p_new_index_list, esp_seq, p_p_ah, p_p_gre,p_ext_hdr_ref_sw, p_tt, ref_index_list,
    count, l_insert, insert_bit_mask, p_ecode),
    rohc_entity_t               *p_entity           _AND_
    rohc_dsc_udp_t              *p_dsc              _AND_
    rohc_U8bit_t                *p_compressed_pkt   _AND_
    rohc_U32bit_t               *p_length           _AND_
    rohc_U8bit_t                *p_new_item_count   _AND_
    rohc_list_t                 *p_ext_hdr_list     _AND_
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       _AND_
    rohc_U8bit_t                *p_new_index_list   _AND_
    rohc_U32bit_t               esp_seq             _AND_
    rohc_ah_t                   **p_p_ah            _AND_
    rohc_gre_t                  **p_p_gre           _AND_
    rohc_r_sw_t                 *p_ext_hdr_ref_sw   _AND_
    /* - SPR 17439 */
    rohc_decomp_ext_hdr_tt_t    *p_tt               _AND_
    rohc_U8bit_t  ref_index_list[ROHC_MAX_ENTRIES_IN_TT] _AND_
    rohc_U8bit_t                count               _AND_
    rohc_U8bit_t                l_insert            _AND_
    rohc_U16bit_t               insert_bit_mask     _AND_ 
    rohc_error_code_t           *p_ecode
)
{
    rohc_U8bit_t    *p_item_start = ROHC_NULL, 
                    ref_counter = 0;
    rohc_U8bit_t    i = 0, j = 0;
    rohc_U8bit_t    index_list[ROHC_MAX_ENTRIES_IN_CSRC_TT]; 
    /* To store in SW */
    rohc_bool_t     first_XI = ROHC_TRUE;
    rohc_U8bit_t    *p_start = p_compressed_pkt;
    rohc_U32bit_t     XI_place = 4; /* In case of 4 bit XI , this field indicates
                                   * from where to read XI List */

    /* XI field is of 4 bits */
    p_item_start = p_compressed_pkt + (count/2);
    *p_length = *p_length + (count/2);

    first_XI = ROHC_TRUE; /* as first XI is present in 1st byte */

    for(j=l_insert; j>0; j--)
    {
        if(ROHC_GET_BIT(insert_bit_mask, j))
        {
            /* Insert bit is 1, hence index needs to be picked from
             * XI List which is 4 bit */

            /* The first item is present in the first byte */
            if(first_XI == ROHC_TRUE)
            {
                /* Indicates item needs to be picked from XI 1 */
                ROHC_GET_NBITS(*p_start, 1, 3, index_list[i]);
                if(ROHC_GET_BIT(*p_start, 4))
                {
                    /* X = 1, hence item present in item list */
                    /* Update the new item TT */
                    if(ROHC_FAILURE == rohc_d_process_p2_new_item(p_entity,
                                p_dsc, (p_ext_hdr_tt + index_list[i]), p_length, 
                                p_ext_hdr_list,p_p_ah, p_p_gre, &p_item_start, p_ecode))
                    {
                        ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_3_pkt");
                        return ROHC_FAILURE;
                    }

                    /* Add the index in the new index list */
                    *(p_new_index_list + (*p_new_item_count))=index_list[i];

                    /* Increment the new item count */
                    (*p_new_item_count)++;
                }
                else
                {
                    /* X = 0, item in TT */
                    if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt +
                                    index_list[i]), p_ext_hdr_list, esp_seq,
                                p_p_ah, p_p_gre, p_ecode))
                    {
                        ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_3_pkt");
                        return ROHC_FAILURE;
                    }

                    /* +- SPR 17777 */
                    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                            ROHC_GET_DSC_CID(p_dsc),sizeof(p_ext_hdr_ref_sw),0,0, __func__,"");
                    /* +- SPR 17777 */
                    ROHC_LOG(LOG_INFO,
                            "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                            "Item[%d] to be picked from TT at index[%d]\n", \
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId, \
                            ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                            ((rohc_protocol_node_t *)((p_tt + index_list[i])->
                                p_item))->protocol, index_list[i]);
                }
                first_XI = ROHC_FALSE;
            }
            ROHC_GET_NBITS(*p_compressed_pkt,
                    XI_place+1, 3, index_list[i]);
            if(ROHC_GET_BIT(*p_compressed_pkt, XI_place+4))
            {
                /* X = 1, hence item present in item list */
                if(ROHC_FAILURE == rohc_d_process_p2_new_item(p_entity, 
                            p_dsc, (p_ext_hdr_tt + index_list[i]), p_length, 
                            p_ext_hdr_list,p_p_ah,p_p_gre, &p_item_start, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_3_pkt");
                    return ROHC_FAILURE;
                }
                /* Add the index in the new index list */
                *(p_new_index_list + (*p_new_item_count))=index_list[i];

                /* Increment the new item count */
                (*p_new_item_count)++;
            }
            else
            {
                /* X = 0, hence item present in TT */
                if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt +
                                index_list[i]), p_ext_hdr_list, esp_seq,
                            p_p_ah, p_p_gre, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_3_pkt");
                    return ROHC_FAILURE;
                }

                ROHC_LOG(LOG_INFO,
                        "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                        "Item[%d] to be picked from TT at index[%d]\n", \
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, \
                        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                        ((rohc_protocol_node_t *)((p_tt + index_list[i])->p_item))->
                        protocol, index_list[i]);
            }
            XI_place = ROHC_ABS(XI_place - 4);
            if(XI_place != 0)   p_compressed_pkt++;
        }
        else
        {   /* Insert bit is 0 */
            /* Read the index from the Ref List in sliding window */

            index_list[i] = ref_index_list[ref_counter];
            ref_counter++;

            /* Add the item in the final list */
            if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt + 
                            index_list[i]), p_ext_hdr_list, esp_seq,
                        p_p_ah, p_p_gre, p_ecode))
            {
                ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_3_pkt");
                return ROHC_FAILURE;
            }

            ROHC_LOG(LOG_INFO,
                    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                    "Item[%d] to be picked from ref_list at index[%d]\n", \
                    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                    ((rohc_protocol_node_t *)((p_tt + index_list[i])->p_item))->
                    protocol, index_list[i]);
        }
        i++;
    } /* end of for */
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**          rohcParseP2ExtHdrEncType3PSflagOn
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to parse an Encoding Type 3 packet
**      ARGUMENTS:
**          The compressor context, buffer etc. are taken as input
**      RETURN VALUE:
**          void
**      NOTES:
**          p_length is length of compressed pkt
*****************************************************************************/
static rohc_return_t rohcParseP2ExtHdrEncType3PSflagOn   ROHC_PARAMS
(
    /* + SPR 17439 */
    (p_entity,p_dsc,p_compressed_pkt,p_length,p_new_item_count, p_ext_hdr_list,
    p_ext_hdr_tt, p_new_index_list, esp_seq, p_p_ah, p_p_gre,p_ext_hdr_ref_sw, p_tt, ref_index_list,
    count, l_insert, insert_bit_mask , p_ecode),
    rohc_entity_t               *p_entity           _AND_
    rohc_dsc_udp_t              *p_dsc              _AND_
    rohc_U8bit_t                *p_compressed_pkt   _AND_
    rohc_U32bit_t               *p_length           _AND_
    rohc_U8bit_t                *p_new_item_count   _AND_
    rohc_list_t                 *p_ext_hdr_list     _AND_
    rohc_decomp_ext_hdr_tt_t    *p_ext_hdr_tt       _AND_
    rohc_U8bit_t                *p_new_index_list   _AND_
    rohc_U32bit_t               esp_seq             _AND_
    rohc_ah_t                   **p_p_ah            _AND_
    rohc_gre_t                  **p_p_gre           _AND_
    rohc_r_sw_t                 *p_ext_hdr_ref_sw   _AND_
    /* - SPR 17439 */
    rohc_decomp_ext_hdr_tt_t    *p_tt               _AND_
    rohc_U8bit_t  ref_index_list[ROHC_MAX_ENTRIES_IN_TT] _AND_
    rohc_U8bit_t                count               _AND_
    rohc_U8bit_t                l_insert            _AND_
    /* Coverity 54511 */
    rohc_U16bit_t               insert_bit_mask     _AND_ 
    /* Coverity 54511 */
    rohc_error_code_t           *p_ecode
)
{
    rohc_U8bit_t    *p_item_start = ROHC_NULL; 
    rohc_U8bit_t    i = 0, j = 0;
    rohc_U8bit_t    index_list[ROHC_MAX_ENTRIES_IN_CSRC_TT]; 
    /* To store in SW */

    /* XI list is of 8 bits */

    p_item_start = p_compressed_pkt + count;
    *p_length = *p_length + count;

    for(j=l_insert; ((j > 0) && (*p_new_item_count != count)); j--)
    {
        if(ROHC_GET_BIT(insert_bit_mask, j))
        {
            /* Insert bit is 1, hence index needs to be picked from
             * XI List which is 8 bit */

            ROHC_GET_NBITS(*(p_compressed_pkt + i), 1, 7, index_list[i]);
            if(ROHC_GET_BIT(*(p_compressed_pkt + i), 8))
            {
                /* X = 1, indicates item present in item list */

                /* New item, hence add in new item TT */
                if(ROHC_FAILURE == rohc_d_process_p2_new_item(p_entity, 
                            p_dsc,(p_ext_hdr_tt + index_list[i]), p_length, 
                            p_ext_hdr_list, p_p_ah, p_p_gre, &p_item_start, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_3_pkt");
                    return ROHC_FAILURE;
                }

                /* Add the index in the new index list */
                *(p_new_index_list + (*p_new_item_count)) = index_list[i];

                /* Increment the new item count */
                (*p_new_item_count)++;
            }
            else
            {
                /* X = 0, indicates item present in translation table
                 * hence only need to add in final ext hdr list */

                if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt +
                                index_list[i]), p_ext_hdr_list, esp_seq, p_p_ah,
                            p_p_gre, p_ecode))
                {
                    ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_3_pkt");
                    return ROHC_FAILURE;
                }
                /* +- SPR 17777 */
                LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
                        __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                        ROHC_GET_DSC_CID(p_dsc),sizeof(p_ext_hdr_ref_sw),0,0, __func__,"");
                /* +- SPR 17777 */

                ROHC_LOG(LOG_INFO,
                        "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                        "Item[%d] to be picked from TT at index[%d]\n", \
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, \
                        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                        ((rohc_protocol_node_t *)((p_tt + index_list[i])->p_item))->
                        protocol, index_list[i]);
            }
        }
        else
        {   /* Insert bit is 0 */
            /* Read the index from the Ref List in sliding window */

            index_list[i] = ref_index_list[i];

            /* Add in the item in the final ext hdr list */
            if(ROHC_FAILURE == rohc_d_pick_item_from_tt((p_tt +
                            index_list[i]), p_ext_hdr_list, esp_seq, p_p_ah, 
                        p_p_gre, p_ecode))
            {
                ROHC_EXIT_FN("parse_p2_ext_hdr_enc_type_3_pkt");
                return ROHC_FAILURE;
            }

            ROHC_LOG(LOG_INFO,
                    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                    "Item[%d] to be picked from SW at index[%d] in SW\n", \
                    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                    ((rohc_protocol_node_t *)((p_tt + index_list[i])->p_item))->
                    protocol, index_list[i]);
        }
        i++;
    } /* end of for */
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**          rohcP1UpdateTempContxtSWInDPIB
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to update Temp context of SW in dpib
**      ARGUMENTS:
**          The decompressor context, buffer etc. are taken as input
**      RETURN VALUE:
**          void
*****************************************************************************/

static rohc_return_t rohcP1UpdateTempContxtSWInDPIB   ROHC_PARAMS
(
   (p_ext_hdr_sw,p_dsc,gen_id),
    rohc_U8bit_t                *p_ext_hdr_sw       _AND_
    rohc_dsc_rtp_t              *p_dsc              _AND_
    rohc_U8bit_t                gen_id)
{
    rohc_U16bit_t i = 0;
    /* Coverity 11399 starts*/
    rohc_U8bit_t    index_list[ROHC_MAX_ENTRIES_IN_TT] = {0}; 
    /* Coverity 11399 starts*/
    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        ((rohc_r_sw_t *)(p_ext_hdr_sw))->rtp_sn_no =
            ROHC_DSC_GET_SN_NO(p_dsc->dpib);
        ((rohc_r_sw_t *)(p_ext_hdr_sw))->icount = i+1;
        /* SPR 3532 Klocwork fix */

        ROHC_MEMCPY(((rohc_r_sw_t *)(p_ext_hdr_sw))->index_list,
                index_list, i+1);
    }
    else
    {
        /* U/O Mode */
        ((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->gen_id = gen_id;
        ((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->icount = i+1;
        /* SPR 3532 Klocwork Fix */

        ROHC_MEMCPY(((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->index_list,
                index_list, i+1);
    }
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**          rohcP2UpdateTempContxtSWInDPIB
******************************************************************************
**
**      DESCRIPTION:
**          This function is used to update Temp context of SW in dpib
**      ARGUMENTS:
**          The decompressor context, buffer etc. are taken as input
**      RETURN VALUE:
**          void
*****************************************************************************/

static rohc_return_t rohcP2UpdateTempContxtSWInDPIB   ROHC_PARAMS
(
   (p_ext_hdr_sw,p_dsc,gen_id),
    rohc_U8bit_t                *p_ext_hdr_sw       _AND_
    rohc_dsc_udp_t              *p_dsc              _AND_
    rohc_U8bit_t                gen_id)
{
    rohc_U16bit_t i = 0;
    /* Coverity 11400 starts*/
    rohc_U8bit_t    index_list[ROHC_MAX_ENTRIES_IN_TT] = {0}; 
    /* Coverity 11400 starts*/
    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        ((rohc_r_sw_t *)(p_ext_hdr_sw))->rtp_sn_no =
            ROHC_DSC_GET_SN_NO(p_dsc->dpib);
        ((rohc_r_sw_t *)(p_ext_hdr_sw))->icount = i+1;
        /* SPR 3532 Klocwork fix */
        ROHC_MEMCPY(((rohc_r_sw_t *)(p_ext_hdr_sw))->index_list,
                index_list, i+1);
    }
    else
    {
        /* U/O Mode */
        ((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->gen_id = gen_id;
        ((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->icount = i+1;
        /* SPR 3532 Klocwork Fix */
        ROHC_MEMCPY(((rohc_d_uo_sw_t *)(p_ext_hdr_sw))->index_list,
                index_list, i+1);
    }
    return ROHC_SUCCESS;
}
