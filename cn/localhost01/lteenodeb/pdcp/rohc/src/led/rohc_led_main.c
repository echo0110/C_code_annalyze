/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_led_main.c,v 1.3.36.4 2010/11/09 05:20:30 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file is main file for list compression.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_led_main.c,v $
 * Revision 1.3.36.4  2010/11/09 05:20:30  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.3.36.3  2010/10/13 03:52:07  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.3.36.2  2010/10/01 14:09:42  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.3.36.1  2010/09/23 05:11:50  gur22059
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
#include "rohc_typ.h"
#include "rohc_def.h"
#include "rohc_ccom_mcr.h"
#include "rohc_g_typ.h"
#include "rohc_led_proto.h"
#include "rohc_led_def.h"
#include "rohc_led_fn_tbl.h"
#include "rohc_trace_mcr.h"
/* + SPR 17439 */
#include "rohc_led_ext_if.h"
/* - SPR 17439 */
/******************************************************************************
  Private Definitions
 *****************************************************************************/

/* Start of change for VARIBLE Declaration */

c_p1_func_table_t   rohc_c_p1_func_table[CSRC_FID_R_MODE + 1];
c_p2_func_table_t   rohc_c_p2_func_table[OUTER_IP_EXT_LIST_FID_R_MODE + 1];
d_p1_func_table_t   rohc_d_p1_func_table[CSRC_FID_R_MODE + 1];
d_p2_func_table_t   rohc_d_p2_func_table[OUTER_IP_EXT_LIST_FID_R_MODE + 1];

/* End of change for VARIBLE Declaration */

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
**        rohc_list_encoding_p1
**
*******************************************************************
**
**    DESCRIPTION
**         This function is the entry function for list encoding and
**         should be called irrespective of whether list is present
**         or not.
**
**    INPUTS
**		   p_entity - Pointer to Rohc Entity 
**		   field_id - Field Identifier of field which need to be compressed
**		   p_csc - Pointer to Compressor Context
**		   p_length - Length 
**		   p_compressed_list - Pointer to Compressed List 
**		   p_ecode - Pointer to Error Code
**
**    RETURNS
**        rohc_return_t
**
******************************************************************/
rohc_return_t   rohc_list_encoding_p1 ROHC_PARAMS
    (
        (p_entity, field_id, p_csc, p_length, p_compressed_list, p_ecode),
            rohc_entity_t       *p_entity           _AND_
            rohc_U8bit_t        field_id            _AND_
            rohc_csc_rtp_t      *p_csc              _AND_
            rohc_U32bit_t       *p_length           _AND_
            rohc_U8bit_t        *p_compressed_list  _AND_
            rohc_error_code_t   *p_ecode
    )
{

    rohc_return_t   ret = ROHC_SUCCESS;
    c_p1_func_table_t  function_handler;

    ROHC_ENTER_FN("rohc_list_encoding_p1");

    function_handler = rohc_c_p1_func_table[field_id - 
                        ROHC_INNERIP_EXTHEADERS_LIST_FID + p_csc->c_mode - 1];

    if (function_handler ==  ROHC_NULL)
    {
        ret = ROHC_FAILURE;
    }
    else
    {
        ret = function_handler(p_entity, p_csc, p_length, 
                               p_compressed_list, p_ecode);
    }

    ROHC_EXIT_FN("rohc_list_encoding_p1");

    return ret;
}
/*****************************************************************
**
**    FUNCTION NAME
**        rohc_list_encoding_p2
**
*******************************************************************
**
**    DESCRIPTION
**         This function is the entry function for list encoding and
**         should be called irrespective of whether list is present
**         or not.
**
**    INPUTS
**		   p_entity - Pointer to Rohc Entity 
**		   field_id - Field Identifier
**		   p_csc - Pointer to Compressor Context
**		   p_length - Length 
**		   p_compressed_list - Pointer to Compressed List 
**		   p_ecode - Pointer to Error Code
**
**    RETURNS
**        rohc_return_t
**
******************************************************************/
rohc_return_t   rohc_list_encoding_p2 ROHC_PARAMS
    (
        (p_entity, field_id, p_csc, p_length, p_compressed_list, p_ecode),
            rohc_entity_t       *p_entity           _AND_
            rohc_U8bit_t        field_id            _AND_
            rohc_csc_udp_t      *p_csc              _AND_
            rohc_U32bit_t       *p_length           _AND_
            rohc_U8bit_t        *p_compressed_list  _AND_
            rohc_error_code_t   *p_ecode
    )
{

    rohc_return_t   ret = ROHC_SUCCESS;
    c_p2_func_table_t  function_handler;

    ROHC_ENTER_FN("rohc_list_encoding_p2");

    function_handler = rohc_c_p2_func_table[field_id - 
                        ROHC_INNERIP_EXTHEADERS_LIST_FID + p_csc->c_mode - 1];

    if (function_handler ==  ROHC_NULL)
    {
        ret = ROHC_FAILURE;
    }
    else
    {
        ret = function_handler(p_entity, p_csc, p_length, 
                                    p_compressed_list, p_ecode);
    }

    ROHC_EXIT_FN("rohc_list_encoding_p2");

    return ret;
}
/*****************************************************************
**
**    FUNCTION NAME
**        rohc_list_decoding_p1
**
*******************************************************************
**
**    DESCRIPTION
**         This function is the entry function for list decoding
**
**    INPUTS
**		   p_entity - Pointer to Rohc Entity 
**		   field_id - Field Identifier
**		   p_csc - Pointer to Compressor Context
**		   p_length - Length 
**		   p_compressed_list - Pointer to Compressed List 
**		   p_ecode - Pointer to Error Code
**
**    RETURNS
**        rohc_return_t
**
******************************************************************/

rohc_return_t   rohc_list_decoding_p1  ROHC_PARAMS
(
    (p_entity, p_dsc, field_id, p_length, p_compressed_pkt, p_ecode),
    rohc_entity_t       *p_entity           _AND_
    rohc_dsc_rtp_t      *p_dsc              _AND_
    rohc_U8bit_t        field_id            _AND_
    rohc_U32bit_t       *p_length           _AND_
    rohc_U8bit_t        *p_compressed_pkt   _AND_
    rohc_error_code_t   *p_ecode
)
{
    rohc_return_t   ret = ROHC_SUCCESS;
    d_p1_func_table_t  function_handler;
    
    ROHC_ENTER_FN("rohc_list_decoding_p1");

    *p_length = 0;

    function_handler = rohc_d_p1_func_table[field_id - 
                        ROHC_INNERIP_EXTHEADERS_LIST_FID + p_dsc->d_mode - 1];

    if (function_handler ==  ROHC_NULL)
    {
        ret = ROHC_FAILURE;
    }
    else
    {
        ret = function_handler(p_entity, p_dsc, p_length, 
                                p_compressed_pkt, p_ecode);
    }

    ROHC_EXIT_FN("rohc_list_decoding_p1");
    return ret;
}

rohc_return_t   rohc_list_decoding_p2  ROHC_PARAMS
(
    (p_entity, p_dsc, field_id, p_length, p_compressed_pkt, p_ecode),
    rohc_entity_t       *p_entity         _AND_
    rohc_dsc_udp_t      *p_dsc            _AND_
    rohc_U8bit_t        field_id          _AND_
    rohc_U32bit_t       *p_length         _AND_
    rohc_U8bit_t        *p_compressed_pkt _AND_
    rohc_error_code_t   *p_ecode
)
{
    rohc_return_t   ret = ROHC_SUCCESS;
    d_p2_func_table_t  function_handler;
    
    ROHC_ENTER_FN("rohc_list_decoding_p2");

    *p_length = 0;

    function_handler = rohc_d_p2_func_table[field_id - 
                      ROHC_INNERIP_EXTHEADERS_LIST_FID + p_dsc->d_mode - 1];

    if (function_handler ==  ROHC_NULL)
    {
        ret = ROHC_FAILURE;
    }
    else
    {
        ret = function_handler(p_entity, p_dsc, p_length, 
                                p_compressed_pkt, p_ecode);
    }

    ROHC_EXIT_FN("rohc_list_decoding_p2");
    return ret;
}

/******************************************************************
**      FUNCTION:
**              rohcParseCompressedHeaderChain_p6
*******************************************************************
**
**      DESCRIPTION:
**              Performs parsing of compressed header chin
**              on Profile 6 structures.
**      ARGUMENTS:
**              p_dsc - Pointer to Decompressor context 
**              field_id - Field Identifier   
**              p_p_pkt - Pointer to pointer of Rohc Packet    
**      RETURN VALUE:
**              ROHC_SUCCESS/ROHC_FAILURE
**      NOTES:
**
********************************************************************/
rohc_void_t rohcParseCompressedHeaderChain_p6(
    rohc_dsc_tcp_t      *p_dsc ,
    rohc_U8bit_t        field_id ,
    rohc_U8bit_t        **p_p_pkt )
{
    rohc_U8bit_t    protocol = 0 ;
    rohc_U8bit_t    innerip = 0;

    ROHC_ENTER_FN("rohcParseCompressedHeaderChain_p6");

    if((ROHC_OUTERIP_EXTHEADERS_LIST_FID == field_id)
            && p_dsc->dpib->outer_ip4_static)
    {
        protocol =  p_dsc->dpib->outer_ip4_static->proto;
        innerip = 0;
    }
    else if((ROHC_INNERIP_EXTHEADERS_LIST_FID == field_id) 
            && p_dsc->dpib->inner_ip4_static)
    {
        protocol =  p_dsc->dpib->inner_ip4_static->proto;  
        innerip = 1;
    }

    if ((ROHC_IRDYN_PKTTYPE == (p_dsc->dpib->rcv_pkt_typ)))
    {
	    protocol =  p_dsc->profile;
        innerip = 1;
    }

    //IPv4 0x04
    if( ROHC_IPPROTO_TCP == protocol)
    {
        if(0 == innerip)
        {
            p_dsc->dpib->outer_ip4_P6_dyn = (rohc_ip4_prof6_dyn_t*) *p_p_pkt;
        }
        else
        {
            p_dsc->dpib->inner_ip4_P6_dyn = (rohc_ip4_prof6_dyn_t*) *p_p_pkt;
        }
            
        *p_p_pkt+=sizeof(rohc_ip4_prof6_dyn_t);
        
        if(0 == innerip)
        {
            if(IP_ID_BEHAVIOR_ZERO != (p_dsc->dpib->outer_ip4_P6_dyn->res_df_ipid_beh
                    & ROHC_PROF6_IP4_IPID_BEH_MASK))
            {
                p_dsc->dpib->outer_ip4_P6_dyn_id = (rohc_ip4_prof6_dyn_id_t*)
                    *p_p_pkt;
                *p_p_pkt+=sizeof(rohc_ip4_prof6_dyn_id_t);
            }
        }
        else
        {
            if(IP_ID_BEHAVIOR_ZERO != (p_dsc->dpib->inner_ip4_P6_dyn->res_df_ipid_beh
                    & ROHC_PROF6_IP4_IPID_BEH_MASK))
            {
                p_dsc->dpib->inner_ip4_P6_dyn_id = (rohc_ip4_prof6_dyn_id_t*)
                    *p_p_pkt;
                *p_p_pkt+=sizeof(rohc_ip4_prof6_dyn_id_t);
            }
        }
    }

    ROHC_EXIT_FN("rohcParseCompressedHeaderChain_p6");
}

rohc_void_t     c_func_hand_init_p1  ROHC_NO_PARAMS ()
{

    rohc_c_p1_func_table[INNER_IP_EXT_LIST_FID_U_MODE] =
                                    rohc_cled_p1_uo_inneripext_list_comp;
    rohc_c_p1_func_table[INNER_IP_EXT_LIST_FID_O_MODE] =
                                    rohc_cled_p1_uo_inneripext_list_comp;
    rohc_c_p1_func_table[INNER_IP_EXT_LIST_FID_R_MODE] = 
                                    rohc_cled_p1_r_inneripext_list_comp;

    rohc_c_p1_func_table[OUTER_IP_EXT_LIST_FID_U_MODE] =
                                    rohc_cled_p1_uo_outeripext_list_comp;
    rohc_c_p1_func_table[OUTER_IP_EXT_LIST_FID_O_MODE] =
                                    rohc_cled_p1_uo_outeripext_list_comp;
    rohc_c_p1_func_table[OUTER_IP_EXT_LIST_FID_R_MODE] = 
                                    rohc_cled_p1_r_outeripext_list_comp;

    rohc_c_p1_func_table[CSRC_FID_U_MODE]  =   rohc_cled_p1_uo_csrc_list_comp;
    rohc_c_p1_func_table[CSRC_FID_O_MODE]  =   rohc_cled_p1_uo_csrc_list_comp;
    rohc_c_p1_func_table[CSRC_FID_R_MODE]  =   rohc_cled_p1_r_csrc_list_comp;
}

rohc_void_t     c_func_hand_init_p2  ROHC_NO_PARAMS ()
{

    rohc_c_p2_func_table[INNER_IP_EXT_LIST_FID_U_MODE] =
                                    rohc_cled_p2_uo_inneripext_list_comp;
    rohc_c_p2_func_table[INNER_IP_EXT_LIST_FID_O_MODE] =
                                    rohc_cled_p2_uo_inneripext_list_comp;
    rohc_c_p2_func_table[INNER_IP_EXT_LIST_FID_R_MODE] = 
                                    rohc_cled_p2_r_inneripext_list_comp;

    rohc_c_p2_func_table[OUTER_IP_EXT_LIST_FID_U_MODE] =
                                    rohc_cled_p2_uo_outeripext_list_comp;
    rohc_c_p2_func_table[OUTER_IP_EXT_LIST_FID_O_MODE] =
                                    rohc_cled_p2_uo_outeripext_list_comp;
    rohc_c_p2_func_table[OUTER_IP_EXT_LIST_FID_R_MODE] = 
                                    rohc_cled_p2_r_outeripext_list_comp;

}


rohc_void_t     d_func_hand_init_p1  ROHC_NO_PARAMS ()
{

    rohc_d_p1_func_table[INNER_IP_EXT_LIST_FID_U_MODE] = 
                                    rohc_dled_p1_uo_inneripext_list_dec;
    rohc_d_p1_func_table[INNER_IP_EXT_LIST_FID_O_MODE] = 
                                    rohc_dled_p1_uo_inneripext_list_dec;
    rohc_d_p1_func_table[INNER_IP_EXT_LIST_FID_R_MODE] = 
                                    rohc_dled_p1_r_inneripext_list_dec;

    rohc_d_p1_func_table[OUTER_IP_EXT_LIST_FID_U_MODE] = 
                                    rohc_dled_p1_uo_outeripext_list_dec;
    rohc_d_p1_func_table[OUTER_IP_EXT_LIST_FID_O_MODE] = 
                                    rohc_dled_p1_uo_outeripext_list_dec;
    rohc_d_p1_func_table[OUTER_IP_EXT_LIST_FID_R_MODE] = 
                                    rohc_dled_p1_r_outeripext_list_dec;

    rohc_d_p1_func_table[CSRC_FID_U_MODE]  =   rohc_dled_p1_csrc_list_dec;
    rohc_d_p1_func_table[CSRC_FID_O_MODE]  =   rohc_dled_p1_csrc_list_dec;
    rohc_d_p1_func_table[CSRC_FID_R_MODE]  =   rohc_dled_p1_csrc_list_dec;
}

rohc_void_t     d_func_hand_init_p2  ROHC_NO_PARAMS ()
{

    rohc_d_p2_func_table[INNER_IP_EXT_LIST_FID_U_MODE] = 
                                    rohc_dled_p2_uo_inneripext_list_dec;
    rohc_d_p2_func_table[INNER_IP_EXT_LIST_FID_O_MODE] = 
                                    rohc_dled_p2_uo_inneripext_list_dec;
    rohc_d_p2_func_table[INNER_IP_EXT_LIST_FID_R_MODE] = 
                                    rohc_dled_p2_r_inneripext_list_dec;

    rohc_d_p2_func_table[OUTER_IP_EXT_LIST_FID_U_MODE] = 
                                    rohc_dled_p2_uo_outeripext_list_dec;
    rohc_d_p2_func_table[OUTER_IP_EXT_LIST_FID_O_MODE] = 
                                    rohc_dled_p2_uo_outeripext_list_dec;
    rohc_d_p2_func_table[OUTER_IP_EXT_LIST_FID_R_MODE] = 
                                    rohc_dled_p2_r_outeripext_list_dec;
}

/******************************************************************
**      FUNCTION:
**              rohc_led_p1_init
*******************************************************************
**
**      DESCRIPTION:
**              Initializes the LED data structures.
**      ARGUMENTS:
**              None
**      RETURN VALUE:
**              ROHC_SUCCESS/ROHC_FAILURE
**      NOTES:
**
********************************************************************/

rohc_return_t   rohc_led_p1_init ROHC_NO_PARAMS ()
{
    rohc_U32bit_t   count = 0;

    ROHC_ENTER_FN("rohc_led_p1_init");

    for(count = 0; count <= CSRC_FID_R_MODE; count++)
    {
        rohc_c_p1_func_table[count] = ROHC_NULL;
        rohc_d_p1_func_table[count] = ROHC_NULL;
    }
    c_func_hand_init_p1();
    d_func_hand_init_p1();

    ROHC_EXIT_FN("rohc_led_p1_init");
    return ROHC_SUCCESS;
}
/******************************************************************
**      FUNCTION:
**              rohc_led_p2_init
*******************************************************************
**
**      DESCRIPTION:
**              Initializes the LED data structures.
**      ARGUMENTS:
**              None
**      RETURN VALUE:
**              ROHC_SUCCESS/ROHC_FAILURE
**      NOTES:
**
********************************************************************/

rohc_return_t   rohc_led_p2_init ROHC_NO_PARAMS ()
{
    rohc_U32bit_t   count = 0;

    ROHC_ENTER_FN("rohc_led_p2_init");

    for(count = 0; count <= OUTER_IP_EXT_LIST_FID_R_MODE; count++)
    {
        rohc_c_p2_func_table[count] = ROHC_NULL;
        rohc_d_p2_func_table[count] = ROHC_NULL;
    }
    c_func_hand_init_p2();
    d_func_hand_init_p2();

    ROHC_EXIT_FN("rohc_led_p2_init");
    return ROHC_SUCCESS;
}
