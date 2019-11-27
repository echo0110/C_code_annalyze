/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_dled_parse_csrc_pkt.c,v 1.4.34.2 2010/11/09 05:19:14 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains functions for decompression side 
 *                     list compressor.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_dled_parse_csrc_pkt.c,v $
 * Revision 1.4.34.2  2010/11/09 05:19:14  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.4.34.1  2010/10/01 14:09:22  gur22059
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
#include "rohc_led_def.h"
#include "rohc_led_proto.h"
#include "rohc_opt_proto.h"

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

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/





/*****************************************************************
**
**    FUNCTION NAME
**        parse_p1_csrc_enc_type_0_pkt
**
*******************************************************************
**
**    DESCRIPTION
**			This function is used to parse Encoding type 0 CSRC List
**
**    INPUTS
**         p_entity - Pointer to Rohc Entity
**		   p_dsc - The de-compressor context 
**		   p_compressed_pkt - Pointer compressed packet 
**		   p_length - Length of Packet
**    RETURNS
**        Length of the compressed packet
**
******************************************************************/

/* +- SPR 17777 */
rohc_return_t	parse_p1_csrc_enc_type_0_pkt	ROHC_PARAMS
(
	(p_entity, p_dsc, p_compressed_pkt, p_length),
	rohc_entity_t		*p_entity			_AND_
	rohc_dsc_rtp_t		*p_dsc				_AND_	
	rohc_U8bit_t		*p_compressed_pkt	_AND_
	rohc_U32bit_t		*p_length
    /* +- SPR 17777 */
)
{
	rohc_U8bit_t	gen_id;
	rohc_U8bit_t	count = 0, /* Count is total CSRC items */ 
					i = 0, index;
	rohc_U8bit_t	*p_item_start;
	rohc_bool_t		PS_flag;
	rohc_dpib_t	*p_dpib = ROHC_DSC_GET_DPIB(p_dsc);
    /* Klocwork Fix */
	rohc_U8bit_t	index_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] = {0}; /*To store in SW*/
	rohc_decomp_csrc_tt_t	*p_csrc_tt;
	
	ROHC_ENTER_FN("parse_p1_csrc_enc_type_0_pkt");

	if(ROHC_MODE_R == p_dsc->d_mode)
	{
		p_csrc_tt = p_dsc->rohc_d_r_csrc_tt.d_r_csrc_tt;
	}
	else
	{
		p_csrc_tt = p_dsc->rohc_d_uo_csrc_tt.d_uo_csrc_tt;
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
		gen_id = *(p_compressed_pkt + 1);
		p_compressed_pkt++;
		(*p_length)++;
        ROHC_LOG(LOG_INFO,"DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
        " gen_id[%d] present in the packet\n", 
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
        gen_id);

	}
	else
	{
		/* Set gen_id to an invalid entry */
		gen_id = 0xFF;
	}
	p_compressed_pkt++;
	(*p_length)++;

	if(PS_flag)
	{
		p_item_start = p_compressed_pkt + count;
		*p_length = *p_length + count;

		/* Read XI List as 1 byte */
		/* SPR 3532 Klocwork Fix */
		if(count >= ROHC_MAX_ENTRIES_IN_CSRC_TT)
		{
		  ROHC_LOG(LOG_INFO,"Invalid value of count");
		  ROHC_EXIT_FN("parse_p1_csrc_enc_type_0_pkt");
		  return ROHC_FAILURE;
		}
		for(i=1; (i<=count); i++)
		{
			index_list[i] = *(p_compressed_pkt + i);	
			ROHC_GET_NBITS(index_list[i], 1, 7, index);
			if(ROHC_GET_BIT(index_list[i], 1))
			{		
				/* Item in Item List
				 * Hence need to add in new item list and final list */

				rohc_d_process_p1_csrc_new_item(p_entity, p_dsc, index, i, 
					p_item_start);

				p_item_start += 4;
				*p_length = *p_length + 4;
			}
			else
			{
				/* Item picked from Ref List
				 * Hence need to add only in final list */

				p_dpib->final_csrc_list[i] = 
					(p_csrc_tt + index)->item;

				ROHC_LOG(LOG_INFO,
                "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
				"Item[%d] picked from TT at index [%d]\n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,\
                ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                p_dpib->final_csrc_list[i], index);

			}
			index_list[i] = index;
		}
	}
	else
	{	
		p_item_start = p_compressed_pkt + (count/2);
		*p_length = *p_length + (count/2);

		if(count % 2 != 0)
		{	/* Count is odd hence padding also present */
			 p_item_start++;
            (*p_length)++;
		}

		/* Read XI list as 4 bits */
		while(i<count)
		{	
			/* Read the first 4 bits */
			index_list[i] = ROHC_GET_UPPER_NIBBLE(*p_compressed_pkt);
			ROHC_GET_NBITS(*p_compressed_pkt, 5,3, index);
			if(ROHC_GET_BIT(*p_compressed_pkt,8))
			{
                /* Item in Item List
                 * Hence need to add in new item list and final list */

				rohc_d_process_p1_csrc_new_item(p_entity, p_dsc, index, i, 
					p_item_start);

                p_item_start += 4;
				*p_length = *p_length + 4;
			}
			else
			{
				/* Item picked from Ref List
				 * Hence need to add only in final list */

				p_dpib->final_csrc_list[i] = 
					(p_csrc_tt + index)->item;
                ROHC_LOG(LOG_INFO,
                "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                "Item[%d] picked from TT at index [%d]\n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
                ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                p_dpib->final_csrc_list[i], index);

			}
			index_list[i++] = index;
			if(i > count-1) break;

			/* Now read the next 4 bits */
			index_list[i] = ROHC_GET_LOWER_NIBBLE(*p_compressed_pkt);
			ROHC_GET_NBITS(*p_compressed_pkt, 1,3, index);
			if(ROHC_GET_BIT(*p_compressed_pkt,4))
			{
                /* Item in Item List
                 * Hence need to add in new item list and final list */

				rohc_d_process_p1_csrc_new_item(p_entity, p_dsc, index, i, 
					p_item_start);

                p_item_start += 4;
				*p_length = *p_length + 4;
			}
			else
			{
                /* Item picked from Ref List
                 * Hence need to add only in final list */

                p_dpib->final_csrc_list[i] =
					(p_csrc_tt + index)->item;

                ROHC_LOG(LOG_INFO,
                "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                "Item[%d] picked from TT at index [%d]\n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,\
                ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                p_dpib->final_csrc_list[i], index);

			}
			index_list[i++] = index;

			p_compressed_pkt++;
		}
	}
	if(p_dsc->d_mode == ROHC_MODE_R)
    {
        p_dpib->csrc_r_sw.rtp_sn_no = ROHC_DSC_GET_SN_NO(p_dpib);
		p_dpib->csrc_r_sw.icount = count;
		ROHC_MEMCPY(p_dpib->csrc_r_sw.index_list, index_list, count);
    }
    else
    {
		p_dpib->csrc_uo_sw.gen_id = gen_id;
		p_dpib->csrc_uo_sw.icount = count;
		ROHC_MEMCPY(p_dpib->csrc_uo_sw.index_list, index_list, count);
    }

		
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
	"Length of the compressed list [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    *p_length);

    /*
    ** Incrementing Stats - ROHC_FUTURE_STATS
    ** pdcpStats.statsRohc.numberOfListEncNoGenSchmUsedProcessed++;
    */

	ROHC_EXIT_FN("parse_p1_csrc_enc_type_0_pkt");
	return ROHC_SUCCESS;
}

/*****************************************************************
**
**    FUNCTION NAME
**        parse_p1_csrc_enc_type_1_pkt
**
*******************************************************************
**
**    DESCRIPTION
**			This function is used to decode Encoding 
**			type 1 CSRC List in R-Mode
**
**    INPUTS
**         p_entity - Pointer to Rohc Entity
**		   p_dsc - The de-compressor context 
**		   p_compressed_pkt - Pointer compressed packet 
**		   p_length - Length of Packet
**
**    RETURNS
**        Length of the compressed packet
**
******************************************************************/

        /* +- SPR 17777 */
rohc_return_t parse_p1_csrc_enc_type_1_pkt   ROHC_PARAMS
(
    (p_entity, p_dsc, p_compressed_pkt, p_length),
	rohc_entity_t	*p_entity	        _AND_
    rohc_dsc_rtp_t  *p_dsc              _AND_
    rohc_U8bit_t    *p_compressed_pkt   _AND_
    rohc_U32bit_t   *p_length	        
        /* +- SPR 17777 */
)
{
    rohc_U8bit_t    gen_id, ref_id, l_insert, ref_index = 0, ref_counter=0;
	rohc_U16bit_t	insert_bit_mask = 0;
    rohc_U8bit_t    count = 0, /* Count is total New CSRC items */
                    i = 0, j =0,
                    XI_place = 4; /* In case of 4 bit XI , this field indicates
                                   * from where to read XI List */
    rohc_U8bit_t   *p_item_start;
    rohc_bool_t     PS_flag;
    rohc_dpib_t     *p_dpib = ROHC_DSC_GET_DPIB(p_dsc);
    /* Klocwork Fix */
    rohc_U8bit_t    index_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] = {0}; /* To store in SW */
    rohc_U8bit_t    *p_start = p_compressed_pkt;
    rohc_r_sw_t     *p_r_csrc_sw;
    rohc_decomp_csrc_tt_t   *p_csrc_tt;
    rohc_bool_t     first_XI = ROHC_TRUE;

    ROHC_ENTER_FN("parse_p1_csrc_enc_type_1_pkt");

    PS_flag = (rohc_bool_t)ROHC_GET_BIT(*p_compressed_pkt, ROHC_PS_BIT);

	ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
	"PS_Flag [%d]\n", \
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
		" Gen ID [%d] received in the packet\n", \
        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
		gen_id);
    }
	else
	{
		gen_id = 0xFF;
	}

    ref_id = *(p_compressed_pkt + 1);

	/* Get the ref_list from the sliding window, by first getting the 
	* ref_index at which the entry is stored. This ref_index can be obtained
	* by accessing the mapping between ref_id and ref_index 
	* This ref_id is the LSB of gen_id/sn_no  */

    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        p_csrc_tt = p_dsc->rohc_d_r_csrc_tt.d_r_csrc_tt;
		ref_index = p_dsc->map_r_csrc_ref_id_n_idx[ref_id];
		p_r_csrc_sw = &(ROHC_DSC_GET_R_CSRC_SW(p_dsc, ref_index));
    }
    else
    {   /* U/O Mode */
        p_csrc_tt = p_dsc->rohc_d_uo_csrc_tt.d_uo_csrc_tt;
		ref_index = p_dsc->map_uo_csrc_ref_id_n_idx[ref_id];
		p_r_csrc_sw =  (rohc_r_sw_t *)
                        &(ROHC_DSC_GET_UO_CSRC_SW(p_dsc, ref_index));
    }
	p_dpib->csrc_ref_index = ref_index;

	ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
	"Ref_id [%d] received in the packet maps to ref_index[%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
	ref_id, ref_index);

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
        l_insert=7;
        p_compressed_pkt = p_compressed_pkt + 3;
		*p_length = *p_length + 3;
    }
	ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
	" Insertion Bit mask [%d] of length [%d] received in the packet \n", \
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
	"Number of 1s in the insertion bit mask [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
	count);

    i = 0;
    if(PS_flag)
    {
        /* XI list is of 8 bits */

        p_item_start = p_compressed_pkt + count;
		*p_length = *p_length + count;

        for(j=l_insert;((j>0) && (p_dpib->csrc_new_item_count != count)); j--)
        {
            if(ROHC_GET_BIT(insert_bit_mask, j))
            {
                /* Insert bit is 1, hence index needs to be picked from
                 * XI List which is 8 bit */

                ROHC_GET_NBITS(*(p_compressed_pkt + i), 1, 7, index_list[i]);
                if(ROHC_GET_BIT(*(p_compressed_pkt + i), 1))
                {
                    /* X = 1, indicates item present in item list */

                    /* New item, hence add in TT */
					rohc_d_process_p1_csrc_new_item(p_entity, p_dsc, 
                            index_list[i], i, p_item_start);

                    p_item_start += 4;
					*p_length = *p_length + 4;
                }
                else
                {
                    /* X = 0, indicates item present in translation table */

                    p_dpib->final_csrc_list[i] =
                        (p_csrc_tt + index_list[i])->item;

                	ROHC_LOG(LOG_INFO,
                	"DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                	"Item[%d] picked from TT at index [%d]\n", \
                	p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId, \
                	ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                	p_dpib->final_csrc_list[i], index_list[i]);
                }
            }
            else
            {
                /* Read the index from the Ref List in sliding window */

                index_list[i] = p_r_csrc_sw->index_list[ref_counter];
				ref_counter++;

                /* Add the item in the final CSRC list */
                p_dpib->final_csrc_list[i] =
                    (p_csrc_tt + index_list[i])->item;

                ROHC_LOG(LOG_INFO,
                "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                "Item[%d] picked from ref_list at index [%d]\n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,\
                ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                p_dpib->final_csrc_list[i], index_list[i]);

            }
			i++;
        } /* end of for */
    }
    else
    {
        /* XI list is of 4 bits */
        p_item_start = p_compressed_pkt + (count/2);
		*p_length = *p_length + (count/2);

        first_XI = ROHC_TRUE; /* as first XI is present in 1st byte */

        for(j=l_insert; ((j>0) && (p_dpib->csrc_new_item_count != count)); j--)
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
						rohc_d_process_p1_csrc_new_item(p_entity, p_dsc, 
							index_list[i], i, p_item_start);

                        p_item_start += 4;
						*p_length = *p_length + 4;
                    }
                    else
                    {
                        /* X = 0, item in TT */
                        p_dpib->final_csrc_list[i] =
                            (p_csrc_tt + index_list[i])->item;
                    	ROHC_LOG(LOG_INFO,
                    	"DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                    	"Item[%d] picked from TT at index [%d]\n", \
                    	p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, \
                    	ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                    	p_dpib->final_csrc_list[i], index_list[i]);

                    }
                    first_XI = ROHC_FALSE;
                }
                ROHC_GET_NBITS(*p_compressed_pkt,
                                        XI_place+1, 3, index_list[i]);
                if(ROHC_GET_BIT(*p_compressed_pkt, XI_place+4))
                {
                    /* X = 1, hence item present in item list */

					rohc_d_process_p1_csrc_new_item(p_entity, p_dsc, 
                        index_list[i], i, p_item_start);

                    p_item_start += 4;
					*p_length = *p_length + 4;
                }
                else
                {
                    /* X = 0, hence item present in TT */
                     p_dpib->final_csrc_list[i] =
                            (p_csrc_tt + index_list[i])->item;

                    ROHC_LOG(LOG_INFO,
                    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                    "Item[%d] picked from TT at index [%d]\n", \
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId, \
                    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                    p_dpib->final_csrc_list[i], index_list[i]);

                }
                XI_place = ROHC_ABS(XI_place - 4);
				if(XI_place != 0) p_compressed_pkt++;
            }
            else
            {
                /* Read the index from the Ref List in sliding window */

                index_list[i] = p_r_csrc_sw->index_list[i];

                /* Add the item in the final CSRC list */
                p_dpib->final_csrc_list[i] =
                    (p_csrc_tt + index_list[i])->item;

                ROHC_LOG(LOG_INFO,
                "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                "Item[%d] picked from ref_list at index [%d]\n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,\
                ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                p_dpib->final_csrc_list[i], index_list[i]);

            }
			i++;
        } /* end of for */
    }

    /* Update the temp context of SW in DPIB */
    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        p_dpib->csrc_r_sw.rtp_sn_no = ROHC_DSC_GET_SN_NO(p_dpib);
        p_dpib->csrc_r_sw.icount = i;
        ROHC_MEMCPY(p_dpib->csrc_r_sw.index_list, index_list, i);
    }
    else
    {
        /* U/O Mode */
        p_dpib->csrc_uo_sw.gen_id = gen_id;
        p_dpib->csrc_uo_sw.icount = i;
        ROHC_MEMCPY(p_dpib->csrc_uo_sw.index_list, index_list, i);
    }

	ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
	" Length of the Compressed packet [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
	*p_length);

    /*
    ** Incrementing Stats - ROHC_FUTURE_STATS
    ** pdcpStats.statsRohc.numberOfListEncNoInsOnlySchmUsedProcessed++;
    */

    ROHC_EXIT_FN("parse_p1_csrc_enc_type_1_pkt");
	return ROHC_SUCCESS;
}

/*****************************************************************
**
**    FUNCTION NAME
**        parse_p1_csrc_enc_type_2_pkt
**
*******************************************************************
**
**    DESCRIPTION
**			This function is used to decode Encoding 
**			type 2 CSRC List in R-Mode
**
**    INPUTS
**         p_entity - Pointer to Rohc Entity
**		   p_dsc - The de-compressor context 
**		   p_compressed_pkt - Pointer compressed packet 
**		   p_length - Length of Packet
**		   p_ecode - Pointer to Error Code 
**
**    RETURNS
**        Length of the compressed packet
**
******************************************************************/

        /* +- SPR 17777 */
rohc_return_t	parse_p1_csrc_enc_type_2_pkt	ROHC_PARAMS
(
	(p_dsc, p_compressed_pkt, p_length,p_ecode),
	rohc_dsc_rtp_t	    *p_dsc	        _AND_
	rohc_U8bit_t	    *p_compressed_pkt   _AND_
	rohc_U32bit_t	    *p_length	        _AND_
	rohc_error_code_t   *p_ecode
)
{
	rohc_U8bit_t	gen_id = 0, ref_id = 0, i = 0, j = 0, l_remove, ref_index=0;
	rohc_U16bit_t	removal_bit_mask = 0;
	rohc_U8bit_t	count = 0, /* Number of items in ref list to be picked */
					final_count = 0; /* Indicates final items in the list */
    /* Klocwork Fix */
	rohc_U8bit_t	        index_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] = {0};
	rohc_r_sw_t             *p_r_csrc_sw;
	rohc_decomp_csrc_tt_t   *p_csrc_tt;
	rohc_dpib_t		*p_dpib = ROHC_DSC_GET_DPIB(p_dsc);
	rohc_U8bit_t	        *p_start = p_compressed_pkt;

	ROHC_ENTER_FN("parse_p1_csrc_enc_type_2_pkt");

	/* Set the new item count in DPIB as 0 as no new items can be added
     * in this encoding type */
	p_dpib->csrc_new_item_count = 0;

	if(ROHC_GET_BIT(*p_compressed_pkt, ROHC_GP_BIT))
	{
		gen_id = *(p_compressed_pkt + 1);
		p_compressed_pkt++;
		(*p_length)++;
		ROHC_LOG(LOG_INFO,
    	"DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
		" Gen ID [%d] received in the packet \n", \
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
	" Removal Bit mask [%d] of length [%d] received in the packet \n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
	removal_bit_mask, l_remove);

	/* Pick the ref list */
	if(p_dsc->d_mode == ROHC_MODE_R)
	{
		p_csrc_tt = p_dsc->rohc_d_r_csrc_tt.d_r_csrc_tt;
		ref_index = p_dsc->map_r_csrc_ref_id_n_idx[ref_id];
		p_r_csrc_sw = &(ROHC_DSC_GET_R_CSRC_SW(p_dsc, ref_index));
	}
	else
	{
		p_csrc_tt = p_dsc->rohc_d_uo_csrc_tt.d_uo_csrc_tt;
		ref_index = p_dsc->map_uo_csrc_ref_id_n_idx[ref_id];
		p_r_csrc_sw=(rohc_r_sw_t *)&(ROHC_DSC_GET_UO_CSRC_SW(p_dsc, ref_index));
    }
	p_dpib->csrc_ref_index = ref_index;
	ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
	"Ref_id [%d] received in the packet maps to ref_index[%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
	ref_id, ref_index);

    count = ROHC_GET_LOWER_NIBBLE(*p_start);
    ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
    "Count of items present in SW [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
    count);

	for(j=l_remove; j>0; j--)
	{
		if(ROHC_FALSE == ROHC_GET_BIT(removal_bit_mask, j))
		{
			if(i > count)
			{
				/* Encountering 0 after the ref list is exhausted
                 * indicates failure in decompression */
                *p_ecode = ROHC_INVALID_BIT_IN_REMOVAL_BIT_MASK;
                return ROHC_FAILURE;
			}

			/* Pick the item from ref list */
			index_list[final_count] = p_r_csrc_sw->index_list[i];

            /* Add the item in the final CSRC list */
            p_dpib->final_csrc_list[final_count] =
                (p_csrc_tt + index_list[final_count])->item;

            ROHC_LOG(LOG_INFO,
            "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
            "Item[%d] picked from ref_list at index [%d]\n", \
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
            ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
            p_dpib->final_csrc_list[final_count], index_list[final_count]);


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
        p_dpib->csrc_r_sw.rtp_sn_no = ROHC_DSC_GET_SN_NO(p_dpib);
        p_dpib->csrc_r_sw.icount = final_count;
        ROHC_MEMCPY(p_dpib->csrc_r_sw.index_list, index_list, final_count);
    }
    else
    {
        /* U/O Mode */
        p_dpib->csrc_uo_sw.gen_id = gen_id;
        p_dpib->csrc_uo_sw.icount = final_count;
        ROHC_MEMCPY(p_dpib->csrc_uo_sw.index_list, index_list, final_count);
    }

	ROHC_LOG(LOG_INFO,
    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
	" Length of the Compressed packet [%d]\n", \
    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
    ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
	*p_length);

    /*
    ** Incrementing Stats - ROHC_FUTURE_STATS
    ** pdcpStats.statsRohc.numberOfListEncNoRemOnlySchmUsedProcessed++;
    */

	ROHC_EXIT_FN("parse_p1_csrc_enc_type_2_pkt");
	return ROHC_SUCCESS;
}

/*****************************************************************
**
**    FUNCTION NAME
**        parse_p1_csrc_enc_type_3_pkt
**
*******************************************************************
**
**    DESCRIPTION
**			This function is used to decode Encoding 
**			type 3 CSRC List in R-Mode
**
**    INPUTS
**         p_entity - Pointer to Rohc Entity
**		   p_dsc - The de-compressor context 
**		   p_compressed_pkt - Pointer compressed packet 
**		   p_length - Length of Packet
**		   p_ecode - Pointer to Error Code 
**
**    RETURNS
**        Length of the compressed packet
**
******************************************************************/

rohc_return_t parse_p1_csrc_enc_type_3_pkt   ROHC_PARAMS
(
    (p_entity, p_dsc, p_compressed_pkt, p_length,p_ecode),
    rohc_entity_t	*p_entity	    _AND_
    rohc_dsc_rtp_t      *p_dsc  	    _AND_
    rohc_U8bit_t    	*p_compressed_pkt   _AND_
    rohc_U32bit_t    	*p_length	    _AND_
    rohc_error_code_t	*p_ecode
)
{

    rohc_U8bit_t	gen_id = 0, ref_id = 0, i = 0, j = 0, l_remove = 0, 
    ref_index = 0, l_insert = 0;
    rohc_U16bit_t	removal_bit_mask = 0, insert_bit_mask = 0;
    rohc_U8bit_t	count = 0, 
                    /* count indicates the number of 1's in insert bit mask */
                    final_count = 0, /* Indicates final items in the list */
                    ref_count = 0;
    /* Klocwork Fix */
    rohc_U8bit_t	index_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] = {0}; 
    rohc_U8bit_t	*p_start = ROHC_NULL,
                    XI_place = 4;
    rohc_r_sw_t     *p_csrc_sw;
    rohc_decomp_csrc_tt_t   *p_csrc_tt;
    rohc_dpib_t		*p_dpib = ROHC_DSC_GET_DPIB(p_dsc);
    rohc_U8bit_t	*p_item_start = ROHC_NULL;
    /*SPR 3444 changes start*/
    rohc_U32bit_t	ref_index_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] = {0};
    /*SPR 3444 changes end */
    rohc_bool_t		PS_flag, first_XI = ROHC_FALSE;

    ROHC_ENTER_FN("parse_p1_csrc_enc_type_3_pkt");

    p_start = p_compressed_pkt;
    PS_flag = (rohc_bool_t)ROHC_GET_BIT(*p_compressed_pkt, ROHC_PS_BIT);

    ROHC_LOG(LOG_INFO,
            "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
            "PS_Flag [%d]\n", \
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
            ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
            PS_flag);

    p_compressed_pkt++;

    if(ROHC_GET_BIT(*p_start, ROHC_GP_BIT))
    {
        gen_id = *p_compressed_pkt;
        p_compressed_pkt++;
        (*p_length)++;
        ROHC_LOG(LOG_INFO,
                "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                "Gen ID [%d] received in the packet\n", \
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
                ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                gen_id);
    }
    else
    {
        gen_id = 0xFF; /* This is the invalid value */
    }

    ref_id = *p_compressed_pkt;
    p_compressed_pkt++;
    *p_length = *p_length + 2;

    if(ROHC_GET_BIT(*p_compressed_pkt, 8))
    {
        /* Removal bit mask is 15 bits */
        ROHC_GET_NBITS(*p_compressed_pkt, 1, 15, removal_bit_mask);
        l_remove = 15;
        *p_length = *p_length + 2;
        p_compressed_pkt = p_compressed_pkt + 2;
    }
    else
    {
        /* removal bit_mask is 7 bits */
        ROHC_GET_NBITS(*p_compressed_pkt, 1, 7, removal_bit_mask);
        l_remove = 7;
        (*p_length)++;
        p_compressed_pkt++;
    }
    ROHC_LOG(LOG_INFO,
            "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
            " Removal Bit mask [%d] of length [%d] received in the packet \n", \
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
            ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
            removal_bit_mask, l_remove);

    if(ROHC_GET_BIT(*p_compressed_pkt, 8))
    {
        /* Insertion bit mask is 15 bits */
        ROHC_GET_NBITS(*p_compressed_pkt, 1, 15, insert_bit_mask);
        l_insert = 15;
        *p_length = *p_length + 2;
        p_compressed_pkt = p_compressed_pkt + 2;
    }
    else
    {
        /* removal bit_mask is 7 bits */
        ROHC_GET_NBITS(*p_compressed_pkt, 1, 7, insert_bit_mask);
        l_insert = 7;
        (*p_length)++;
        p_compressed_pkt++;
    }
    ROHC_LOG(LOG_INFO,
            "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
            " Insertion Bit mask [%d] of length [%d] received in the packet \n", \
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
            ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
            insert_bit_mask, l_insert);

    /* Pick the ref list */
    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        p_csrc_tt = p_dsc->rohc_d_r_csrc_tt.d_r_csrc_tt;
        ref_index = p_dsc->map_r_csrc_ref_id_n_idx[ref_id];
        p_csrc_sw = &(ROHC_DSC_GET_R_CSRC_SW(p_dsc, ref_index));
    }
    else
    {
        p_csrc_tt = p_dsc->rohc_d_uo_csrc_tt.d_uo_csrc_tt;
        ref_index = p_dsc->map_uo_csrc_ref_id_n_idx[ref_id];
        p_csrc_sw = (rohc_r_sw_t *)&(ROHC_DSC_GET_UO_CSRC_SW(p_dsc, ref_index));
    }
    p_dpib->csrc_ref_index = ref_index;

    ROHC_LOG(LOG_INFO,
            "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
            "Ref_id [%d] received in the packet maps to ref_index[%d]\n", \
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
            ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
            ref_id, ref_index);

    for(j=l_remove; j>0; j--)
    {
        if(ROHC_FALSE == ROHC_GET_BIT(removal_bit_mask, j))
        {
            if(i > p_csrc_sw->icount)
            {
                /* If now 0 is observed in the removal bit mask, then
                 * it is an error and header should not be communicated to
                 * upper layer */
                *p_ecode = ROHC_INVALID_BIT_IN_REMOVAL_BIT_MASK;
                return ROHC_FAILURE;
            }

            /* Pick the item from ref list */
            ref_index_list[final_count] = p_csrc_sw->index_list[i];
            ROHC_LOG(LOG_INFO,
                    "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                    "Item at index [%d] picked from ref_list\n", \
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

    i = 0;
    /* Now apply insertion bit mask */
    for(i=1; i<=l_insert; i++)
    {
        if(ROHC_GET_BIT(insert_bit_mask, i))
            count++; 
    }
    ROHC_LOG(LOG_INFO,
            "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
            "Number of 1s in insertion bit mask [%d]\n", \
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
            ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
            count);

    i=0;	

    if(PS_flag)
    {
        /* XI list is of 8 bits */

        p_item_start = p_compressed_pkt + count;
        *p_length = *p_length + count;

        for(j=l_insert; ((j>0) && (p_dpib->csrc_new_item_count != count)); j--)
        {
            if(ROHC_GET_BIT(insert_bit_mask, j))
            {
                /* Insert bit is 1, hence index needs to be picked from
                 * XI List which is 8 bit */

                ROHC_GET_NBITS(*(p_compressed_pkt + i), 1, 7, index_list[i]);
                if(ROHC_GET_BIT(*(p_compressed_pkt + i), 8))
                {
                    /* X = 1, indicates item present in item list */

                    /* New item, hence add in TT */

                    rohc_d_process_p1_csrc_new_item(p_entity, p_dsc, index_list[i],
                            i, p_item_start);

                    p_item_start += 4;
                    *p_length = *p_length + 4;
                }
                else
                {
                    /* X = 0, indicates item present in translation table */

                    p_dpib->final_csrc_list[i] =
                        (p_csrc_tt + index_list[i])->item;

                    ROHC_LOG(LOG_INFO,
                            "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                            "Item[%d] picked from TT at index [%d]\n", \
                            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
                            ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                            p_dpib->final_csrc_list[i], index_list[i]);

                }
            }
            else
            {
                /* Read the index from the Ref List in sliding window */

                /* It might be the case that the list is over and the bit mask
                 * is just padded with 0's now */
                if(i > count + final_count)
                    break;

                index_list[i] = ref_index_list[ref_count];

                /* Add the item in the final CSRC list */
                p_dpib->final_csrc_list[i] =
                    (p_csrc_tt + index_list[i])->item;
                ROHC_LOG(LOG_INFO,
                        "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                        "Item[%d] picked from ref_list at index [%d]\n", \
                        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
                        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                        p_dpib->final_csrc_list[i], index_list[i]);

                ref_count++;
            }
            i++;
        } /* end of for */
    }
    else
    {
        /* XI list is of 4 bits */
        p_item_start = p_compressed_pkt + (count/2);
        *p_length = *p_length + (count/2);

        first_XI = ROHC_TRUE; /* as first XI is present in 1st byte */

        for(j=l_insert; ((j>0) && (p_dpib->csrc_new_item_count != count)); j--)
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

                        rohc_d_process_p1_csrc_new_item(p_entity, p_dsc, 
                                index_list[i], i, p_item_start);

                        p_item_start += 4;
                        *p_length = *p_length + 4;
                    }
                    else
                    {
                        /* X = 0, item in TT */
                        p_dpib->final_csrc_list[i] =
                            (p_csrc_tt + index_list[i])->item;
                        ROHC_LOG(LOG_INFO,
                                "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                                "Item[%d] picked from TT at index [%d]\n", \
                                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
                                ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                                p_dpib->final_csrc_list[i], index_list[i]);

                    }
                    first_XI = ROHC_FALSE;
                }
                else
                {
                    ROHC_GET_NBITS(*p_compressed_pkt,
                            XI_place+1, 3, index_list[i]);
                    if(ROHC_GET_BIT(*p_compressed_pkt, XI_place + 4))
                    {
                        /* X = 1, hence item present in item list */

                        rohc_d_process_p1_csrc_new_item(p_entity, p_dsc, 
                                index_list[i], i, p_item_start);

                        p_item_start += 4;
                        *p_length = *p_length + 4;
                    }
                    else
                    {
                        /* X = 0, hence item present in TT */
                        p_dpib->final_csrc_list[i] =
                            (p_csrc_tt + index_list[i])->item;
                        ROHC_LOG(LOG_INFO,
                                "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                                "Item[%d] picked from TT at index [%d]\n", \
                                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
                                ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                                p_dpib->final_csrc_list[i], index_list[i]);

                    }
                    XI_place = ROHC_ABS(XI_place - 4);
                    if(XI_place != 0)	p_compressed_pkt++;
                }
            } /* end of insert bit 1 handling */
            else
            {
                /* Read the index from the Ref List in sliding window */

                index_list[i] = ref_index_list[ref_count];
                ref_count++;

                /* Add the item in the final CSRC list */
                p_dpib->final_csrc_list[i] =
                    (p_csrc_tt + index_list[i])->item;

                ROHC_LOG(LOG_INFO,
                        "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
                        "Item[%d] picked from ref_list at index [%d]\n", \
                        p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
                        ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
                        p_dpib->final_csrc_list[i], index_list[i]);
            }
            i++;
        } /* end of for */
    }

    /* Update the temp context of SW in DPIB */
    if(p_dsc->d_mode == ROHC_MODE_R)
    {
        p_dpib->csrc_r_sw.rtp_sn_no = ROHC_DSC_GET_SN_NO(p_dpib);
        p_dpib->csrc_r_sw.icount = i;
        ROHC_MEMCPY(p_dpib->csrc_r_sw.index_list, index_list, i);
    }
    else
    {
        /* U/O Mode */
        p_dpib->csrc_uo_sw.gen_id = gen_id;
        p_dpib->csrc_uo_sw.icount = i;
        ROHC_MEMCPY(p_dpib->csrc_uo_sw.index_list, index_list, i);
    }

    ROHC_LOG(LOG_INFO,
            "DLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
            " Length of the Compressed packet [%d]\n", \
            p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, \
            ROHC_GET_DSC_CID(p_dsc), ROHC_DSC_GET_MODE(p_dsc), \
            *p_length);

    /*
     ** Incrementing Stats - ROHC_FUTURE_STATS
     ** pdcpStats.statsRohc.numberOfListEncNoRemThenInsSchmUsedProcessed++;
     */

    ROHC_EXIT_FN("parse_p1_csrc_enc_type_3_pkt");
    return ROHC_SUCCESS;
}

