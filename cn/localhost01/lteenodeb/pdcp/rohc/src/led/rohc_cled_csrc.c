/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_cled_csrc.c,v 1.4.34.2 2010/11/09 05:16:29 gur22059 Exp $
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
 * $Log: rohc_cled_csrc.c,v $
 * Revision 1.4.34.2  2010/11/09 05:16:29  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.4.34.1  2010/10/13 03:48:23  gur22059
 * Comment Incorporation after reduction in context block
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
#include "rohc_def.h"
#include "rohc_typ.h"
#include "rohc_com_def.h"
#include "rohc_g_typ.h"
#include "rohc_gcom_mcr.h"
#include "rohc_stats_def.h"
#include "rohc_protocol_header.h"
#include "rohc_c_typ.h"
#include "rohc_ccom_mcr.h"
#include "rohc_trace_mcr.h"
#include "rohc_led_proto.h"
#include "rohc_led_mcr.h"
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
/* +- SPR 17777 */
extern tickType_t pdcpCurrentTime_g;
/* +- SPR 17777 */
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/




/*****************************************************************************
**    	FUNCTION:
**        	rohc_cled_p1_r_csrc_list_comp
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to  compress CSRC LIST in R Mode
**   	ARGUMENTS: 
**         	The compressor context, rohc entity context are taken as input
**			and length, compressed list are output parameters.
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
**		LSB of RTP SN No is used as ref_id
*****************************************************************************/

rohc_return_t	rohc_cled_p1_r_csrc_list_comp ROHC_PARAMS
	(
		(p_entity, p_csc, p_length, p_compressed_list, p_ecode),
			rohc_entity_t		*p_entity 		_AND_
			rohc_csc_rtp_t		*p_csc 			_AND_
			rohc_U32bit_t		*p_length 		_AND_
			rohc_U8bit_t      	*p_compressed_list 	_AND_
			rohc_error_code_t	*p_ecode
	)
{
	rohc_return_t		ret = ROHC_SUCCESS;
	rohc_U32bit_t		count = 0, i = 0,  /* counters */
						item_count = 0, /* Count of index list constructed
						after removal bit mask */
						final_count = 0, /* Count of final index list */
						index_count = 0, /* Count of XI List */
						item_list_idx = 0; /* Count of item list */

	rohc_U32bit_t		index_found = 0xFF; /* FF is the invalid entry */
    /* Coverity Fix for 54681*/
	rohc_U8bit_t		index_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] = {0};
						/* index list constructed after removal bit mask */ 
    /* Coverity Fix for 54681*/
	rohc_U8bit_t		final_idx_list[ROHC_MAX_ENTRIES_IN_CSRC_TT];
						/* Final index list to be stored in SW */

	rohc_U32bit_t		item_list[ROHC_MAX_ENTRIES_IN_CSRC_TT];
	rohc_U8bit_t		curr_index, /* Index of TT */
						ref_id,
						item_found_index = 0;
	rohc_r_sw_t			sw_ref_entry; /* SW reference entry */
	r_csrc_tt_t			tt_entry; /* Entry in TT */
	rohc_U16bit_t		removal_bit_mask = 0xFFFF, insertion_bit_mask = 0;
    /*+ Coverity 83205 Changes */
	rohc_U8bit_t		XI_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] = {0};
    /*- Coverity 83205 Changes */
        /* SPR 4808 Fix Start */
	rohc_U32bit_t		*p_csrc_node = NULL;
        /* SPR 4808 Fix End */
	rohc_U32bit_t		*p_last_csrc_node = NULL;
	rohc_U32bit_t		tot_count = 0; /* Total no. of item in curr list */
	rohc_bool_t			PS_flag = ROHC_FALSE, item_present_in_tt = ROHC_FALSE,
						item_remove_flag = ROHC_FALSE;
	rohc_cpib_t			*p_cpib = ROHC_CSC_GET_CPIB(p_csc);

	ROHC_ENTER_FN("rohc_cled_p1_r_csrc_list_comp");

	p_csrc_node = (rohc_U32bit_t *)((p_cpib->rtp) + sizeof(rohc_rtp_t));

	tot_count = (((rohc_rtp_t *)(p_cpib->rtp))->v_p_x_cc & ROHC_CC_MASK);
    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,
	"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
	"R - Total Count of CSRC items [%d]\n", \
	p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
    ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc), tot_count);

	if(ROHC_CSC_GET_R_CSRC_TT_NIDX(p_csc) > 8)
	{
		/* This implies that the index list will be stored in 1 byte field */
		PS_flag = ROHC_TRUE;
		ROHC_LOG(LOG_INFO,
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
		" Setting PS_flag as TRUE\n", \
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));
	}

	/* Checking the case when no ACK is received for the list till now */
	if (ROHC_FALSE == ROHC_CSC_GET_R_CSRC_SW_ACKED(p_csc))
	{
		ROHC_LOG(LOG_INFO,
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]: " \
		"Handling the case when no ACK is recieved till now\n", \
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

		/* This implies that Generic Encoding scheme needs to be applied */
		for(i=0; i<tot_count; i++) /* curr_list */
		{
		/* SPR 3444 changes start */
                    if(i < ROHC_MAX_ENTRIES_IN_CSRC_TT)
                     {
		/* SPR 3444 changes end */
			curr_index = 
				rohc_c_add_n_chk_p1_r_csrc_tt(p_csc, *p_csrc_node, 
				                                &item_present_in_tt);
			index_list[count] = curr_index;
			rohc_update_XI_list(&XI_list[count], curr_index, 1);
			item_list[item_list_idx++] = *p_csrc_node;
			count++;
			p_csrc_node++;
		      }
		} /* end of while */

		/* Construct an Encoding Type 0 packet*/
		ROHC_LOG(LOG_INFO,
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
		"Constructing Encoding Type 0 packet\n",\
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

        /*
        ** Incrementing Stats - ROHC_FUTURE_STATS
        ** pdcpStats.statsRohc.numberOfListEncNoGenSchmUsedCreated++;
        */

		*p_length = rohc_c_enc_type_0_csrc_pkt (0, ROHC_FALSE, PS_flag, 
                        (rohc_U8bit_t)count, XI_list, item_list, 
                        (rohc_U8bit_t)(item_list_idx), p_compressed_list);

		/* Add entry in Sliding Window */		
		rohc_c_add_p1_r_mode_csrc_sw (p_csc, (rohc_U8bit_t)count, index_list);
	}
	else
	{
		/* 
		* Reference list exists . Hence we need to compare the current
		* CSRC list with the reference list and construct the
		* bit mask accordingly 
		*/
		sw_ref_entry = ROHC_CSC_GET_R_CSRC_SW_REF(p_csc);
		ref_id = ROHC_GET_LOWER_NIBBLE(sw_ref_entry.rtp_sn_no);

		ROHC_LOG(LOG_INFO,
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
		"RTP Sequence number of the ref list [%d]\n", \
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc), 
        sw_ref_entry.rtp_sn_no);
		
        ROHC_LOG(LOG_INFO,
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
		"Number of items in ref list [%d]\n", \
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc), 
        sw_ref_entry.icount);

		/* First we will construct removal bit mask */

		for(count = 0; count < sw_ref_entry.icount; count++) /*ref_list*/
		{
			while(i<tot_count) /* curr_list */
			{
				tt_entry = ROHC_CSC_GET_R_CSRC_TT(p_csc, 
                                sw_ref_entry.index_list[count]);
                                /* SPR 4808 Fix Start */
                                if(NULL != p_csrc_node)
                                {
				    if(tt_entry.item == *p_csrc_node)				
				    {
					/* Item found in ref_list */
					index_found = sw_ref_entry.index_list[count];
					p_csrc_node++;
					i++;
					p_last_csrc_node = p_csrc_node;
					item_found_index = i;
					ROHC_LOG(LOG_INFO,
					"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
					"Item[%d] found in ref list at index [%d]\n", \
					p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
					ROHC_CSC_GET_MODE(p_csc), tt_entry.item, index_found);
					break;
				    }
                                }
                                else
                                {
                                    ROHC_LOG(LOG_INFO,"Unable to Retrieve Data from RTP Packet\n");
                                    return ROHC_FAILURE;
                                }
                                /* SPR 4808 Fix End */
				p_csrc_node++;
				i++;
			} /* end of while */
			if(index_found == 0xFF)
			{
				/* This implies that the reference item is not 
				 * there in the current list. Hence remove bit should be 1 */
				/* Again move to the last found item 
				p_csrc_node = (rohc_U32bit_t *)((p_cpib->rtp) + 
									sizeof(rohc_rtp_t)); */
				p_csrc_node = p_last_csrc_node;
				i = item_found_index;
				item_remove_flag = ROHC_TRUE;
			}
			else
			{
				/* Set the remove bit as 0 */
				if (count < 8)
					removal_bit_mask = ROHC_UNSET_BIT(removal_bit_mask, 
							(7 - count));
				else
					removal_bit_mask = ROHC_UNSET_BIT(removal_bit_mask, 
							(24 -count));
				/* SPR 3444 changes start */
				if(item_count < ROHC_MAX_ENTRIES_IN_CSRC_TT)
				{
				/* SPR 3444 changes end */
					index_list[item_count++] = index_found;
					index_found = 0xff;
				}
			}
		} /* end of for */

		ROHC_LOG(LOG_INFO,
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
		"Removal bit mask constructed[%d]\n", \
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc), removal_bit_mask);

		if(item_count < tot_count)
		{
			/* Construct insertion bit mask using the index list
			* constructed above as reference list */
			ROHC_LOG(LOG_INFO,
			"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
			" Constructing Insertion bit mask\n", \
			p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
            ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));
			
			p_csrc_node=(rohc_U32bit_t *)((p_cpib->rtp) + sizeof(rohc_rtp_t));
			count = 0;
			for(i=0; i<tot_count; i++) /*curr_list*/
			{
				while(count < item_count)  /*ref_list*/
				{
					tt_entry = ROHC_CSC_GET_R_CSRC_TT(p_csc,
								index_list[count]);
					if(tt_entry.item == *p_csrc_node)
					{
						/* Item found in ref_list */
						index_found = index_list[count];
					   ROHC_LOG(LOG_INFO,
						"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
						"Item[%d] found in ref list at index [%d]\n",  \
						p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),
						ROHC_CSC_GET_MODE(p_csc), tt_entry.item, index_found);
						final_idx_list[final_count++] = index_found;

						rohc_c_add_p1_sn_r_mode_csrc_tt(p_csc, 
                                (rohc_U8bit_t)index_found, &tt_entry); 
						count++;
						break;
					}
					else
					{
						index_found = 0xFF;
					}
					count++;
				} /* end of while */
				if(index_found == 0xFF)
				{
					/* This implies it is a new item */
					index_found = rohc_c_add_n_chk_p1_r_csrc_tt(p_csc,
                                        *p_csrc_node, &item_present_in_tt);
					ROHC_LOG(LOG_INFO,
					"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
					"Item[%d] not found in ref list," 
                    "hence inserted in TT at index [%d]\n", \
					p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
					ROHC_CSC_GET_MODE(p_csc), *p_csrc_node, index_found);

					/* SPR 3444 changes start */
                                        if(final_count < ROHC_MAX_ENTRIES_IN_CSRC_TT)
                                        {
					    final_idx_list[final_count++] = index_found;
                                        }
					/* SPR 3444 changes end */
					if(item_present_in_tt && (index_found < ROHC_MAX_ENTRIES_IN_CSRC_TT))
					{
						tt_entry = ROHC_CSC_GET_R_CSRC_TT(p_csc, index_found);
						if(tt_entry.known == ROHC_FALSE)
						{
							/* Item not acked */
							rohc_update_XI_list(&XI_list[index_count],
								(rohc_U8bit_t)index_found, 1);
							item_list[item_list_idx++] = *p_csrc_node; 
						}
						else
						{
							rohc_update_XI_list(&XI_list[index_count],
								(rohc_U8bit_t)index_found,0);
						}
					}
					else
					{
						rohc_update_XI_list(&XI_list[index_count],
                                (rohc_U8bit_t)index_found, 1);
						item_list[item_list_idx++] = *p_csrc_node; 
					}
					index_count++;
					/* Item at ith entry is inserted */
					if(i<8)
						insertion_bit_mask = ROHC_SET_BIT(insertion_bit_mask, 
                                                                    (7 - i));
					else
						insertion_bit_mask = ROHC_SET_BIT(insertion_bit_mask, 
                                                                    (24 - i));
					ROHC_LOG(LOG_INFO, 
					"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
					"Insertion bit mask constructed till now[%d]\n", \
					p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
					ROHC_CSC_GET_MODE(p_csc), insertion_bit_mask);

					index_found = 0xFF;
					count = 0;
				}
				else
				{
					index_found = 0xFF;
				}
				p_csrc_node++;
			} /* end of for */


			if((insertion_bit_mask != 0) & (item_remove_flag == ROHC_TRUE))
			{
				ROHC_LOG(LOG_INFO,
				"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
				"Constructing Encoding Type 3 packet\n", \
				p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
                ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

                /*
                ** Incrementing Stats - ROHC_FUTURE_STATS
                ** pdcpStats.statsRohc.numberOfListEncNoRemThenInsSchmUsedCreated++;
                */

				*p_length = rohc_c_enc_type_3_csrc_pkt(0, ref_id, ROHC_FALSE, 
                                PS_flag, (rohc_U8bit_t)index_count, 
                                removal_bit_mask, insertion_bit_mask, XI_list, 
					            item_list, item_list_idx, p_compressed_list);

				/* Add the entry in the sliding window */
				rohc_c_add_p1_r_mode_csrc_sw (p_csc, (rohc_U8bit_t)final_count,
                                              final_idx_list);
			}
			else if(insertion_bit_mask != 0)
			{
				ROHC_LOG(LOG_INFO,
				"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
				"Constructing Encoding Type 1 packet\n", \
				p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
                ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

                /*
                ** Incrementing Stats - ROHC_FUTURE_STATS
                ** pdcpStats.statsRohc.numberOfListEncNoInsOnlySchmUsedCreated++;
                */

				*p_length = rohc_c_enc_type_1_csrc_pkt(0, ref_id, ROHC_FALSE, 
                                PS_flag, (rohc_U8bit_t)index_count, 
                                insertion_bit_mask, XI_list, item_list, 
					            item_list_idx, p_compressed_list);

				/* Add the entry in the sliding window */
				rohc_c_add_p1_r_mode_csrc_sw (p_csc, (rohc_U8bit_t)final_count,
                                                final_idx_list);
			}
		}
		else if ((item_count != tot_count) || (item_remove_flag == ROHC_TRUE))
		{
			ROHC_LOG(LOG_INFO,
			"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
			"Constructing Encoding Type 2 packet\n", \
			p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
            ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

            /*
            ** Incrementing Stats - ROHC_FUTURE_STATS
            ** pdcpStats.statsRohc.numberOfListEncNoRemOnlySchmUsedCreated++;
            */

			/* Construct the Encoding Type 2 packet */
			*p_length = rohc_c_enc_type_2_pkt (0, ref_id,  0, 
                            (rohc_U8bit_t)item_count, removal_bit_mask, 
                            p_compressed_list);

			/* Add the entry in the sliding window */
			rohc_c_add_p1_r_mode_csrc_sw (p_csc, (rohc_U8bit_t)item_count, 
                                            index_list);

		}
		else
		{
			ROHC_LOG(LOG_INFO,
			"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
			"Constructing Encoding Type 1 packet as ist is unchanged\n", \
			p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
            ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

            /*
            ** Incrementing Stats - ROHC_FUTURE_STATS
            ** pdcpStats.statsRohc.numberOfListEncNoInsOnlySchmUsedCreated++;
            */

			/* Implies List is unchanged */
			*p_length = rohc_c_enc_type_1_csrc_pkt (0, ref_id, ROHC_FALSE,
                            ROHC_FALSE, (rohc_U8bit_t)item_count, 0, 0, 0, 0, 
                            p_compressed_list);
		} /* endif */
	} /* endif */

	ROHC_EXIT_FN("rohc_cled_p1_r_csrc_list_comp");
	return ret;
}

/*****************************************************************************
**    	FUNCTION:
**        	rohc_cled_p1_uo_csrc_list_comp
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to compress CSRC LIST in U/O Mode
**   	ARGUMENTS: 
**         	The compressor context, rohc entity context are taken as input
**			and length, compressed list are output parameters.
**    	RETURN VALUE:
**        	rohc_return_t
**		NOTES:
**		LSB of gen_id is used as ref_id in U/O Mode
*****************************************************************************/

rohc_return_t	rohc_cled_p1_uo_csrc_list_comp ROHC_PARAMS
	(
		(p_entity, p_csc, p_length, p_compressed_list, p_ecode),
			rohc_entity_t	*p_entity 		_AND_
			rohc_csc_rtp_t	*p_csc 			_AND_
			rohc_U32bit_t	*p_length 		_AND_
			rohc_U8bit_t    *p_compressed_list 	_AND_
			rohc_error_code_t	*p_ecode
	)
{
    rohc_return_t       ret = ROHC_SUCCESS;
    rohc_U32bit_t       count = 0, i = 0,  /* Counters */
						item_count = 0, /* Count of index list constructed
						after removal bit mask */
						final_count = 0, /* Count of final index list */
                        index_count = 0, /* Count of XI List */
						index_found = 0xFF, /* FF is for invalid entry */
						item_list_idx = 0; /* Count of item list */

    /* Coverity Fix for 54681*/
	rohc_U8bit_t		index_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] = {0};
						/* index list constructed after removal bit mask */ 
    /* Coverity Fix for 54681*/
    rohc_U8bit_t        final_idx_list[ROHC_MAX_ENTRIES_IN_CSRC_TT];
    rohc_U32bit_t       item_list[ROHC_MAX_ENTRIES_IN_CSRC_TT];
    rohc_U8bit_t       	curr_index, item_found_index = 0;
    rohc_uo_sw_t        sw_ref_entry;
    /* SPR 3444 changes start */
    uo_csrc_tt_t      	tt_entry = {0};
    /* SPR 3444 changes end */
    rohc_U16bit_t       removal_bit_mask = 0xFFFF, insertion_bit_mask = 0;
    /*+ Coverity 83197 Changes */
    rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_IN_CSRC_TT] = {0};
    /*- Coverity 83197 Changes */
    /* SPR 4808 Fix Start */
    rohc_U32bit_t       *p_csrc_node = NULL;
    /* SPR 4808 Fix End */
    rohc_U32bit_t       *p_last_csrc_node = NULL;
    rohc_U8bit_t        gen_id = 0, 
						ref_id;
    rohc_U8bit_t       	tot_count = 0; /* Total items in curr_list */
	rohc_bool_t			GP_flag = ROHC_TRUE, 
						PS_flag = ROHC_FALSE, 
						item_present_in_tt = ROHC_FALSE,
						item_remove_flag = ROHC_FALSE;
	rohc_cpib_t			*p_cpib = ROHC_CSC_GET_CPIB(p_csc);

	ROHC_ENTER_FN("rohc_cled_p1_uo_csrc_list_comp");

	p_csrc_node = (rohc_U32bit_t *)((p_cpib->rtp) + sizeof(rohc_rtp_t));

	tot_count = (((rohc_rtp_t *)(p_cpib->rtp))->v_p_x_cc & ROHC_CC_MASK);
    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_CSC_CID(p_csc),(ADDR)p_ecode,0,0, __func__,"");
/* SPR 20636 Changes End */
    /* +- SPR 17777 */
	ROHC_LOG(LOG_INFO,
	"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
	"Total Count of CSRC items [%d]\n", \
	p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
    ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc), tot_count);

	/* Here, its assumed that tot_count can never be Zero. The calling layer
	 * should ensure this */

	if(ROHC_CSC_GET_UO_CSRC_TT_NIDX(p_csc) > 8)
	{
		/* This implies that the index list will be stored in 1 byte field */
		PS_flag = ROHC_TRUE;
		ROHC_LOG(LOG_INFO,
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
		" Setting PS_flag as TRUE\n", \
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));
	}

	if (ROHC_FALSE == ROHC_CSC_GET_UO_CSRC_SW_ACKED(p_csc))
    {
		ROHC_LOG(LOG_INFO,
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
		"Handling the case when no ACK is recieved till now\n", \
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

        /* This implies that Generic Encoding scheme needs to be applied 
		 * as REF ID is not present */

		for(i=0; i<tot_count; i++) /* curr_list */
        {
            curr_index =
            rohc_c_add_n_chk_p1_uo_csrc_tt(p_csc,*p_csrc_node,
                                           &item_present_in_tt);
            index_list[count] = curr_index;
            rohc_update_XI_list(&XI_list[count], curr_index, 1);
            item_list[item_list_idx++] = *p_csrc_node;
			count++;
            p_csrc_node++;
        } /* end of for */

		if(p_csc->dont_update_csc_flag)
		{
			gen_id = 0;
			GP_flag = ROHC_FALSE;
		}
		else
		{
			if(ROHC_CSC_GET_UO_CSRC_SW_NIDX(p_csc) != 0)
			{	
				/* Hence items are present in SW
			 	 * Compare this compressed list with the previous list 
			 	 * to see whether they are identical or not */
				gen_id = rohc_led_p1_generate_csrc_gen_id(p_entity, p_csc,
						(rohc_U8bit_t)count, index_list, &GP_flag);
			}
			else
			{
			    gen_id = 1;
        		ROHC_LOG(LOG_INFO,
        		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
        		"Generating new gen_id [%d] for the list\n", \
        		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
                ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc), gen_id);

        		/* Add the entry in the sliding window */
        		rohc_c_add_p1_uo_mode_csrc_sw(p_csc, gen_id, 
                                             (rohc_U8bit_t)count,index_list);
        		GP_flag = ROHC_TRUE;
			}
		}

        /* Construct an Encoding Type 0 packet*/
		ROHC_LOG(LOG_INFO,
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
		"Constructing Encoding Type 0 packet\n", \
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

        /*
        ** Incrementing Stats - ROHC_FUTURE_STATS
        ** pdcpStats.statsRohc.numberOfListEncNoGenSchmUsedCreated++;
        */

        *p_length = rohc_c_enc_type_0_csrc_pkt (gen_id, GP_flag, PS_flag, 
                        (rohc_U8bit_t)count, XI_list, item_list,
                        (rohc_U8bit_t) item_list_idx, p_compressed_list);
    }
    else
    {
        /*
        * Entries are there in the TT. Hence we need to compare the current
        * Extension header list with the reference list and construct the
        * bit mask accordingly
        */
    	sw_ref_entry = ROHC_CSC_GET_UO_CSRC_SW_REF(p_csc);
		ref_id = sw_ref_entry.gen_id;

		ROHC_LOG(LOG_INFO,
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
		"Gen ID of the ref list [%d]\n", \
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc), sw_ref_entry.gen_id);

		ROHC_LOG(LOG_INFO,
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
		"Number of items in ref list [%d]\n",  \
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc), sw_ref_entry.icount);


        /* First we will construct removal bit mask */

        for(count = 0; count < sw_ref_entry.icount; count++) /*ref_list*/
        {
			while(i<tot_count) /*curr_list*/
            {
                tt_entry = ROHC_CSC_GET_UO_CSRC_TT(p_csc,
                                sw_ref_entry.index_list[count]);
                /* SPR 4808 Fix Start */
                if(NULL != p_csrc_node)
                {
                    if(tt_entry.item == *p_csrc_node)
                    {
                        /* Item found in ref_list */
                        index_found = sw_ref_entry.index_list[count];
                        p_csrc_node++;
					i++;
					p_last_csrc_node = p_csrc_node;
					item_found_index = i;
					ROHC_LOG(LOG_INFO,
					"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
					"Item found in ref_list at index [%d]\n", \
					p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
					ROHC_CSC_GET_MODE(p_csc), index_found);

                        break;
                    
                    }
                }
                else
                {
                   ROHC_LOG(LOG_INFO," Unable to Retrieve Data From RTP Packet\n");
                   return ROHC_FAILURE;
 
                }
                /* SPR 4808 Fix End */
                p_csrc_node++;
				i++;
            } /* end of while */
            if(index_found == 0xFF)
            {
                /* This implies that the reference item is not
                * there in the current list */
				item_remove_flag = ROHC_TRUE;
				p_csrc_node = p_last_csrc_node;
				i = item_found_index;
            }
            else
	    {
		    if(count < 8)
			    removal_bit_mask = ROHC_UNSET_BIT(removal_bit_mask, 
					    (7 - count));
		    else
			    removal_bit_mask = ROHC_UNSET_BIT(removal_bit_mask, 
					    (24 - count));
		    /* SPR 3444 changes start */
		    if(item_count < ROHC_MAX_ENTRIES_IN_CSRC_TT)
		    {
			    index_list[item_count++] = index_found;
		    }
		    /* SPR 3444 changes end */
		    index_found = 0xff;
	    }
        } /* end of for */
		ROHC_LOG(LOG_INFO,
		"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
		"Removal bit mask = [%d]\n", \
		p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
        ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc), removal_bit_mask);

        if(item_count < tot_count)
        {
            /* Construct insertion bit mask using the index list
            * constructed above as reference list */
			ROHC_LOG(LOG_INFO,
			"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
			"Constructing Insertion bit mask now \n", \
			p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
            ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

            p_csrc_node=(rohc_U32bit_t *)((p_cpib->rtp) + sizeof(rohc_rtp_t));
            count = 0;
			for(i=0; i<tot_count; i++) /*curr_list*/
            {
                while(count < item_count)  /*ref_list*/
                {
                    tt_entry = ROHC_CSC_GET_UO_CSRC_TT(p_csc,
                                index_list[count]);
                    if(tt_entry.item == *p_csrc_node)
                    {
                        /* Item found in ref_list */
                        index_found = index_list[count];
					   	ROHC_LOG(LOG_INFO,
						"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
						"Item found in ref list at index [%d]\n",  \
						p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc),
						ROHC_CSC_GET_MODE(p_csc), index_found);

			/* SPR 3444 changes start */
                        if(final_count < ROHC_MAX_ENTRIES_IN_CSRC_TT)
                        {
                            final_idx_list[final_count++] = index_found;
                        }
			/* SPR 3444 changes end */

						/* Increment the counter */
						rohc_c_incr_counter_uo_csrc_tt(p_entity, &tt_entry);
                        count++;
                        break;
                    }
                    else
                        index_found = 0xFF;

                    count++;
                } /* end of while */
                if(index_found == 0xFF)
                {
                    /* This implies it is a new item */
                    index_found = rohc_c_add_n_chk_p1_uo_csrc_tt(p_csc,
							*p_csrc_node, &item_present_in_tt);
					ROHC_LOG(LOG_INFO,
					"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
					"Item not found in list hence inserted at index[%d]\n", \
					p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
					ROHC_CSC_GET_MODE(p_csc), index_found);

		/* SPR 3444 changes start */
                    if(final_count < ROHC_MAX_ENTRIES_IN_CSRC_TT)
                    {
                        final_idx_list[final_count++] = index_found;
                    }
					if(item_present_in_tt && (index_found < ROHC_MAX_ENTRIES_IN_CSRC_TT))
		/* SPR 3444 changes end */
					{
                    	tt_entry = ROHC_CSC_GET_UO_CSRC_TT(p_csc, index_found);
						if(tt_entry.known == ROHC_FALSE)	
						{
							/* Item not yet ACKED */
                    		rohc_update_XI_list(&XI_list[index_count], 
								(rohc_U8bit_t)index_found, 1);
                    		item_list[item_list_idx++] = tt_entry.item;
						}
						else
						{
                    		rohc_update_XI_list(&XI_list[index_count], 
								(rohc_U8bit_t)index_found, 0);
						}
					}
					else
					{
                    	rohc_update_XI_list(&XI_list[index_count], 
                            (rohc_U8bit_t)index_found, 1);
                    	item_list[item_list_idx++] = tt_entry.item;
					}
					index_count++;
					if(i<8)
                    	insertion_bit_mask = ROHC_SET_BIT(insertion_bit_mask, 
                                                                    (7 - i));
					else
                    	insertion_bit_mask = ROHC_SET_BIT(insertion_bit_mask, 
                                                                    (24 - i));
					ROHC_LOG(LOG_INFO, 
					"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
					"Insertion bit mask constructed till now [%d]\n",  \
					p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId, ROHC_CSC_GET_CID(p_csc), \
					ROHC_CSC_GET_MODE(p_csc), insertion_bit_mask);

                    index_found = 0xFF;
                    count = 0;
                }
                else
                {
                    index_found = 0xFF;
                }
                p_csrc_node++;
            } /* end of for */

			/*Compare this compressed list with the previous list to see whether
			 * they are identical or not */
			if(p_csc->dont_update_csc_flag)
			{
				gen_id = 0;
				GP_flag = ROHC_FALSE;
			}
			else
			{
				gen_id = rohc_led_p1_generate_csrc_gen_id(p_entity, p_csc,
						(rohc_U8bit_t)final_count, final_idx_list, &GP_flag);
			}

        	if((insertion_bit_mask != 0) & (item_remove_flag == ROHC_TRUE))
        	{
				ROHC_LOG(LOG_INFO,
				"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
				"Constructing Encoding Type 3 packet \n", \
				p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
                ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

                /*
                ** Incrementing Stats - ROHC_FUTURE_STATS
                ** pdcpStats.statsRohc.numberOfListEncNoRemThenInsSchmUsedCreated++;
                */

            	*p_length = rohc_c_enc_type_3_csrc_pkt(gen_id, ref_id, GP_flag, 
                                PS_flag, (rohc_U8bit_t)index_count, 
                                removal_bit_mask, insertion_bit_mask, XI_list, 
                                item_list, item_list_idx, p_compressed_list);

				rohc_c_add_p1_uo_mode_csrc_sw(p_csc, gen_id, 
                    (rohc_U8bit_t)final_count, final_idx_list); 
        	}
        	else if(insertion_bit_mask != 0)
        	{
				ROHC_LOG(LOG_INFO,
				"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
				"Constructing Encoding Type 1 packet \n", \
				p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
                ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

                /*
                ** Incrementing Stats - ROHC_FUTURE_STATS
                ** pdcpStats.statsRohc.numberOfListEncNoInsOnlySchmUsedCreated++;
                */

            	*p_length = rohc_c_enc_type_1_csrc_pkt(gen_id, ref_id, GP_flag, 
                                PS_flag, (rohc_U8bit_t)index_count, 
                                insertion_bit_mask, XI_list, item_list, 
                                item_list_idx, p_compressed_list);
				rohc_c_add_p1_uo_mode_csrc_sw(p_csc, gen_id, 
                    (rohc_U8bit_t)final_count, final_idx_list); 
        	}

        } /* end of insertion bit mask */
		else if ((item_count != tot_count) || (item_remove_flag == ROHC_TRUE))
        {
			/*Compare this compressed list with the previous list to see whether
			 * they are identical or not */
			if(p_csc->dont_update_csc_flag)
			{
				gen_id = 0;
				GP_flag = ROHC_FALSE;
			}
			else
			{
				gen_id = rohc_led_p1_generate_csrc_gen_id(p_entity, p_csc,
						(rohc_U8bit_t)item_count, index_list, &GP_flag);
			}

            /* Construct the Encoding Type 2 packet */
			ROHC_LOG(LOG_INFO,
			"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
			"Constructing Encoding Type 2 packet \n", \
			p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
            ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

            /*
            ** Incrementing Stats - ROHC_FUTURE_STATS
            ** pdcpStats.statsRohc.numberOfListEncNoRemOnlySchmUsedCreated++;
            */

            *p_length = rohc_c_enc_type_2_pkt (gen_id, ref_id, GP_flag,
                            (rohc_U8bit_t)item_count,removal_bit_mask, 
                            p_compressed_list);
			rohc_c_add_p1_uo_mode_csrc_sw(p_csc, gen_id, 
                (rohc_U8bit_t)final_count, final_idx_list); 
        }
        else
        {
            /* Implies List is unchanged
			 * hence gen_id will not be sent in this case as list 
			 * is same as ref list */
			ROHC_LOG(LOG_INFO,
			"CLED:EID[%d:%d]:CID[%d]:Mode[%d]:" \
			"Constructing Encoding Type 1 packet as list is unchanged\n", \
			p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId, 
            ROHC_CSC_GET_CID(p_csc), ROHC_CSC_GET_MODE(p_csc));

	        /*
            ** Incrementing Stats - ROHC_FUTURE_STATS
            ** pdcpStats.statsRohc.numberOfListEncNoInsOnlySchmUsedCreated++;
            */

            *p_length = rohc_c_enc_type_1_csrc_pkt (0, ref_id, ROHC_FALSE, 
                            PS_flag, (rohc_U8bit_t)item_count, 0, 0, ROHC_NULL, 
                            0, p_compressed_list);
        } /* endif */

    } /* endif */

	ROHC_EXIT_FN("rohc_cled_p1_uo_csrc_list_comp");

    return ret;
}

