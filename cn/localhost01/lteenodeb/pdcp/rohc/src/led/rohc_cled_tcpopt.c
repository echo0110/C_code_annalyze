/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "rohc_def.h"
#include "rohc_led_def.h"
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
#include "rohc_ed_def.h"

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

/*****************************************************************************
**    	FUNCTION:
**        	rohc_list_encoding_p6
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to compress TCP OPTIONS in U/O Mode
**   	ARGUMENTS: 
**         	The compressor context, rohc entity context are taken as input
**			and length, compressed list are output parameters.
**    	RETURN VALUE:
**        	rohc_return_t
*****************************************************************************/

rohc_return_t	 rohc_list_encoding_p6 ROHC_PARAMS
	(
		(p_rohc_entity, p_csc, p_length, p_compressed_list, p_ecode),
			rohc_entity_t	*p_rohc_entity 		_AND_
			rohc_csc_tcp_t	*p_csc 			_AND_
			rohc_U32bit_t	*p_length 		_AND_
			rohc_U8bit_t    *p_compressed_list 	_AND_
			rohc_error_code_t	*p_ecode
	)
{
    rohc_U8bit_t        tot_len = 0; /* Total length of option list in bytes in
                                        uncomp header */
    rohc_U8bit_t        count = 0;
    rohc_U8bit_t        curr_index = 0;
    rohc_U8bit_t        numXI_items = 0;
    rohc_U8bit_t        i = 0;
    /*+ Coverity 83199 Changes */
    rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_TCP_OPT_IDX] = {0};
    /*- Coverity 83199 Changes */
    /* SPR 19690 fix start */
    rohc_U8bit_t        item_list[MAX_TCP_OPT_SIZE];
    rohc_U8bit_t        *p_item_list = NULL;
    /* SPR 19690 fix end */
    rohc_U8bit_t        len = 0;
    rohc_U8bit_t        item_list_len = 0;
    rohc_U8bit_t        *tcp_opt_list = NULL;
    rohc_c_tcp_opt_t    *p_tcp_opt = NULL;
    rohc_U8bit_t        index = 0;
    rohc_U8bit_t        ts_opt_present = 0;
    
    rohc_U8bit_t temp_XI_index[ROHC_MAX_ENTRIES_TCP_OPT_IDX] = {0};
    rohc_bool_t         PS_flag = ROHC_FALSE;
    rohc_return_t       ret = ROHC_SUCCESS;
    rohc_cpib_t         *p_cpib = ROHC_CSC_GET_CPIB(p_csc);

    tot_len = ((ROHC_GET_UPPER_NIBBLE(((rohc_tcp_t*)p_csc->p_cpib->tcp)->
                    offset)) * 4) - 20;
    /* copy the tcp option kinds in compressor context in same order as they 
     * appear in uncompressed header */
    count = tot_len;
    /* check if there are any existing generic options*/
    if(p_csc->rohc_c_tcp_opt.rohc_c_generic_opt.next_free_index)
    {
        /* free the generic  options in the context*/
        for(index = 0; index < p_csc->rohc_c_tcp_opt.rohc_c_generic_opt.
                next_free_index; index++)
        {
            ROHC_FREE(p_csc->rohc_c_tcp_opt.rohc_c_generic_opt.
                    rohc_generic_opt_elem[index].opt_val);
            p_csc->rohc_c_tcp_opt.rohc_c_generic_opt.rohc_generic_opt_elem
                [index].opt_val = ROHC_NULL;
        }
        p_csc->rohc_c_tcp_opt.rohc_c_generic_opt.next_free_index = 0;
    }
    p_tcp_opt = &(p_csc->rohc_c_tcp_opt);
    for(i = 0; i < ROHC_MAX_ENTRIES_TCP_OPT_IDX; i++)
    {
         ROHC_C_INIT_OPT_KIND(p_tcp_opt->tcp_opt_kind[i], 0xff);
    }

    tcp_opt_list = (p_cpib->tcp) + sizeof(rohc_tcp_t);
    i = 0;
    while(count)
    {
        switch(*tcp_opt_list)
        {
            case TCP_NOP_OPT :
                p_tcp_opt->tcp_opt_kind[i] = TCP_NOP_OPT;
                i++;
                tcp_opt_list++;
                count--;
                numXI_items++;
                break;

            case TCP_MSS_OPT :
                p_tcp_opt->tcp_opt_kind[i] = TCP_MSS_OPT;
                i++;
                count = count - TCP_MSS_OPT_LEN;
                p_tcp_opt->mss = ((rohc_tcp_mss_opt_t*)tcp_opt_list)->val;
                tcp_opt_list+=TCP_MSS_OPT_LEN;
                numXI_items++;
                break;

            case TCP_WS_OPT :
                p_tcp_opt->tcp_opt_kind[i] = TCP_WS_OPT;
                i++;
                p_tcp_opt->ws = ((rohc_tcp_ws_opt_t*)tcp_opt_list)->val;
                tcp_opt_list+=TCP_WS_OPT_LEN;
                count = count - TCP_WS_OPT_LEN;
                numXI_items++;
                break;

            case TCP_SACK_PERMTD_OPT :
                p_tcp_opt->tcp_opt_kind[i] = TCP_SACK_PERMTD_OPT;
                i++;
                tcp_opt_list+=TCP_SACK_PERMTD_OPT_LEN;
                count = count - TCP_SACK_PERMTD_OPT_LEN;
                numXI_items++;
                break;

            case TCP_SACK_OPT :
                p_tcp_opt->tcp_opt_kind[i] = TCP_SACK_OPT;
                i++;
                len = ((rohc_tcp_sack_opt_t*)tcp_opt_list)->length;
                p_tcp_opt->tcp_opt_len[SACK_LEN_ID] = len;
                ROHC_MEMCPY(p_tcp_opt->sack_ptr, tcp_opt_list+2, len-2);
                tcp_opt_list+=len;
                count = count - len;
                numXI_items++;
                break;

            case TCP_TS_OPT :
                p_tcp_opt->tcp_opt_kind[i] = TCP_TS_OPT;
                i++;
                ROHC_MEMCPY(p_tcp_opt->ts, tcp_opt_list+2, 8);
                tcp_opt_list+=TCP_TS_OPT_LEN;
                count = count - TCP_TS_OPT_LEN;
                numXI_items++;
                /* update value in sliding window as this option is passed as
                 * lsb encoded value in irregular chain for compressed pkts*/
                ts_opt_present = ROHC_TRUE;
                p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].
                    ts_opt_recvd = ROHC_TRUE;
                rohc_ed_p6_update_gsw_ts_val(p_rohc_entity,p_csc,
                        ROHC_NTOHL(p_tcp_opt->ts[0]));
                rohc_ed_p6_update_gsw_ts_echo(p_rohc_entity,p_csc,
                        ROHC_NTOHL(p_tcp_opt->ts[1]));
                break;

            case TCP_ALT_CKSUM_REQ_OPT :
                p_tcp_opt->tcp_opt_kind[i] = TCP_ALT_CKSUM_REQ_OPT;
                i++;
                p_tcp_opt->alt_cksum_req = ((rohc_tcp_alt_cksum_req_opt_t*)
                        tcp_opt_list)->val;
                tcp_opt_list+=TCP_ALT_CKSUM_REQ_OPT_LEN;
                count = count - TCP_ALT_CKSUM_REQ_OPT_LEN;
                numXI_items++;
                break;

            case TCP_EOL_OPT :
                /* means options are over, rest are padding octets*/
                p_tcp_opt->tcp_opt_kind[i] = TCP_EOL_OPT;
                p_tcp_opt->tcp_opt_len[EOL_LEN_ID] = count; 
                /* count tell the remaining
                 * bytes of padding including EOL type byte*/
                count = 0;
                numXI_items++;
                break;

            default :
                /* generic option */
                p_tcp_opt->tcp_opt_kind[i] = *tcp_opt_list;
                i++;
                len = *(tcp_opt_list + 1);
                if(ROHC_FAILURE == rohc_c_add_generic_option(tcp_opt_list,
                            &(p_tcp_opt->rohc_c_generic_opt),p_ecode))
                {
                    return ROHC_FAILURE;
                }
                tcp_opt_list+=len;
                count = count - len;
                numXI_items++;
                break;
        }
    }
    count = 0;
    if(!ts_opt_present)
    {
         p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].
             ts_opt_recvd = ROHC_FALSE;
    }
    p_item_list = item_list;
    rohc_U8bit_t gen_opt_idx = 7;
    /* Prepare XI list in the order stored in optArray */
    for(i=0; i< numXI_items; i++)
    { 
        /* SPR 19690 fix start */
        curr_index = getIndexforOptionKindAndUpdateItemList(p_tcp_opt->
                tcp_opt_kind[i],p_item_list,p_tcp_opt,p_csc,&len,&gen_opt_idx);
        /* SPR 19690 fix end */
        if(curr_index > 7)
        {
            PS_flag = ROHC_TRUE;
        }
        p_item_list = p_item_list + len;
        item_list_len += len;
        temp_XI_index[i] = curr_index;
        count++;
    }
    /* update XI List */
     for(i = 0; i < numXI_items; i++)
     {
         rohc_p6_update_XI_list(&XI_list[i], temp_XI_index[i], 1 ,
                 PS_flag);
     }
    *p_length = rohc_c_list_enc_tcp_opt_pkt (PS_flag,
            (rohc_U8bit_t)count, XI_list, item_list,
            (rohc_U8bit_t)item_list_len, p_compressed_list);
    ROHC_EXIT_FN("rohc_list_encoding_p6");
    return ret;
}

/*****************************************************************************
**    	FUNCTION:
**         rohc_c_add_generic_option
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to parse and store tcp generic options
**   	ARGUMENTS: 
**         	The compressor context is taken as input
**			and options updated in compressor context.
**    	RETURN VALUE:
**        	rohc_return_t
*****************************************************************************/
rohc_return_t rohc_c_add_generic_option ROHC_PARAMS
	(
		(tcp_opt_list, p_generic_opt, p_ecode),
            rohc_U8bit_t    *tcp_opt_list   _AND_
			rohc_c_generic_opt_t *p_generic_opt     _AND_
            rohc_error_code_t   *p_ecode
	)
{
    rohc_c_generic_opt_elem_t    *p_generic_opt_elem;
    rohc_U8bit_t                 len = 0;
    p_generic_opt_elem = &(ROHC_CSC_GET_GENERIC_ELEM_INDEX(p_generic_opt));
    p_generic_opt_elem->kind = *tcp_opt_list;
    p_generic_opt_elem->length = *(tcp_opt_list+1);
    p_generic_opt_elem->option_static = 0;
    len = p_generic_opt_elem->length;
    p_generic_opt_elem->opt_val = 
        (rohc_U8bit_t *)ROHC_MALLOC(sizeof(rohc_U8bit_t)* (len-2));

    if(ROHC_NULL == p_generic_opt_elem->opt_val)
    {
        *p_ecode = ROHC_MEMORY_FAILURE;
        ROHC_EXIT_FN("rohc_c_add_generic_option");
        return ROHC_FAILURE;
    }
    ROHC_MEMSET(p_generic_opt_elem->opt_val, 0,
            sizeof(rohc_U8bit_t)*(len-2));
    ROHC_MEMCPY(p_generic_opt_elem->opt_val, tcp_opt_list+2, len-2);
    ROHC_CSC_SET_GENERIC_ELEM_INDEX(p_generic_opt);
    return ROHC_SUCCESS;
}

/*****************************************************************************
**    	FUNCTION:
**        	getIndexforOptionKindAndUpdateItemList
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to update item in table based compression
**          scheme.
**   	ARGUMENTS: 
**         	The compressor context, rohc entity context are taken as input
**			and length, compressed list are output parameters.
**    	RETURN VALUE:
**        	rohc_return_t
*****************************************************************************/
rohc_return_t getIndexforOptionKindAndUpdateItemList ROHC_PARAMS
	(
		(kind, p_item_list, p_tcp_opt, p_csc, p_len, gen_opt_idx),
			rohc_U8bit_t kind 		        _AND_
			rohc_U8bit_t *p_item_list 	    _AND_
			rohc_c_tcp_opt_t *p_tcp_opt     _AND_
            rohc_csc_tcp_t  *p_csc          _AND_
			rohc_U8bit_t *p_len             _AND_
            rohc_U8bit_t *gen_opt_idx
	)
{
    rohc_U8bit_t ret_index;
    rohc_U8bit_t len;
    rohc_U8bit_t i, l_index;
    
    rohc_c_generic_opt_elem_t    generic_opt_elem;
    *p_len = 0;
    switch(kind)
    {
        case TCP_NOP_OPT :
          
                ret_index = INDEX_NOP;
          
            break;

        case TCP_EOL_OPT :
            ret_index = INDEX_EOL;
            *p_item_list = (p_tcp_opt->tcp_opt_len[EOL_LEN_ID]-1)*8;/*in bits*/
            *p_len = 1;
            break;

        case TCP_MSS_OPT :
            ret_index = INDEX_MSS;
            ROHC_MEMCPY(p_item_list,&(p_tcp_opt->mss),2);
            *p_len = 2;
            break;

        case TCP_WS_OPT :
            ret_index = INDEX_WINDOW_SCAL;
            *p_item_list = p_tcp_opt->ws;
            *p_len = 1;
            break;

        case TCP_TS_OPT :
            ret_index = INDEX_TIMESTAMP;
            ROHC_MEMCPY(p_item_list,p_tcp_opt->ts,8);
            *p_len = 8;
            break;

        case TCP_SACK_PERMTD_OPT :
            ret_index = INDEX_SACK_PERM;
            break;

        case TCP_SACK_OPT :
            ret_index = INDEX_SACK;
            len = p_tcp_opt->tcp_opt_len[SACK_LEN_ID];
            if(10 == len)
            {
                /* 1 sack block */
                sack1_item_enc(p_tcp_opt->sack_ptr,
                      p_csc->field_value[ROHC_TCP_ACKNO_FID-ROHC_VAR_FID_BASE],
                      p_item_list,p_len);
            }
            if(18 == len)
            {
                /* 2 sack blocks */
                sack2_item_enc(p_tcp_opt->sack_ptr,
                      p_csc->field_value[ROHC_TCP_ACKNO_FID-ROHC_VAR_FID_BASE],
                      p_item_list,p_len);
            }
            if(26 == len)
            {
                /* 3 sack blocks */
                sack3_item_enc(p_tcp_opt->sack_ptr,
                      p_csc->field_value[ROHC_TCP_ACKNO_FID-ROHC_VAR_FID_BASE],
                      p_item_list,p_len);
            }
            if(34 == len)
            {
                /* 4 sack blocks */
                sack4_item_enc(p_tcp_opt->sack_ptr,
                      p_csc->field_value[ROHC_TCP_ACKNO_FID-ROHC_VAR_FID_BASE],
                      p_item_list,p_len);
            }

            break;

        case TCP_ALT_CKSUM_REQ_OPT :
            ret_index = *gen_opt_idx;
            (*gen_opt_idx)++;
            *p_item_list++ = TCP_ALT_CKSUM_REQ_OPT;
            *p_item_list++ = TCP_ALT_CKSUM_REQ_OPT_LEN;
            *p_item_list = p_tcp_opt->alt_cksum_req;
            *p_len = 3;
            break;
        default :
            /* Generic options */
            ret_index = *gen_opt_idx;
            l_index = p_tcp_opt->rohc_c_generic_opt.next_free_index;

            for(i = 0; i<l_index; i++)
            {
                if(kind == (p_tcp_opt)->rohc_c_generic_opt.
                        rohc_generic_opt_elem[i].kind)
                {
                    break;
                }
            }
            /* list encoding of generic options */
            generic_opt_elem =(p_tcp_opt)->rohc_c_generic_opt.
                rohc_generic_opt_elem[i];
            *p_item_list++ = generic_opt_elem.kind;
            *p_item_list = generic_opt_elem.option_static << 7 |
                generic_opt_elem.length;
            p_item_list++;
            ROHC_MEMCPY(p_item_list,generic_opt_elem.opt_val,
                    generic_opt_elem.length-2);
            *p_len = generic_opt_elem.length;
            (*gen_opt_idx)++;
            break;
    }
    return ret_index;
}

/*****************************************************************************
**    	FUNCTION:
**        	sack1_item_enc
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to encode SACK option
**   	ARGUMENTS: 
**         	The compressor context are taken as input
**			and length, compressed list are output parameters.
**    	RETURN VALUE:
**        	rohc_return_t
*****************************************************************************/
rohc_void_t sack1_item_enc ROHC_PARAMS
	(
		(p_sack_field, ack_no, p_item_list, p_len),
			rohc_U32bit_t *p_sack_field     _AND_
			rohc_U32bit_t  ack_no           _AND_
			rohc_U8bit_t *p_item_list 	    _AND_
			rohc_U8bit_t *p_len
	)
{
    /* discriminator */
    *p_item_list = ROHC_SACK_1_DISC;
    (*p_len)++;
    /* each block contains two 32bit pointers */
    /* call encoding of block1 start field */
    sack_block_enc( *p_sack_field,ack_no,p_item_list+*p_len,p_len);
    /* call encoding of block1 end field */
    sack_block_enc(*(p_sack_field+1) ,ROHC_NTOHL(*p_sack_field),p_item_list+*p_len,p_len);
}

/*****************************************************************************
**    	FUNCTION:
**        	sack2_item_enc
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to encode SACK option
**   	ARGUMENTS: 
**         	The compressor context are taken as input
**			and length, compressed list are output parameters.
**    	RETURN VALUE:
**        	rohc_return_t
*****************************************************************************/
rohc_void_t sack2_item_enc ROHC_PARAMS
	(
		(p_sack_field, ack_no, p_item_list, p_len),
			rohc_U32bit_t *p_sack_field     _AND_
			rohc_U32bit_t  ack_no           _AND_
			rohc_U8bit_t *p_item_list 	    _AND_
			rohc_U8bit_t *p_len
	)
{
    /* discriminator */
    *p_item_list = ROHC_SACK_2_DISC;
    (*p_len)++;
    /* each block contains two 32bit pointers */
    /* call encoding of block1 start field */
    sack_block_enc( *p_sack_field,ack_no,p_item_list+*p_len,p_len);
    /* call encoding of block1 end field */
    sack_block_enc(*(p_sack_field+1) ,ROHC_NTOHL(*p_sack_field),p_item_list+*p_len,p_len);
    /* call encoding of block2 start field */
    sack_block_enc(*(p_sack_field+2) ,ROHC_NTOHL(*(p_sack_field+1)),
            p_item_list+*p_len,p_len);
    /* call encoding of block2 end field */
    sack_block_enc(*(p_sack_field+3) ,ROHC_NTOHL(*(p_sack_field+2)),p_item_list+*p_len
            ,p_len);
}

/*****************************************************************************
**    	FUNCTION:
**        	sack3_item_enc
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to encode SACK option
**   	ARGUMENTS: 
**         	The compressor context are taken as input
**			and length, compressed list are output parameters.
**    	RETURN VALUE:
**        	rohc_return_t
*****************************************************************************/
rohc_void_t sack3_item_enc ROHC_PARAMS
	(
		(p_sack_field, ack_no, p_item_list, p_len),
			rohc_U32bit_t *p_sack_field     _AND_
			rohc_U32bit_t  ack_no           _AND_
			rohc_U8bit_t *p_item_list 	    _AND_
			rohc_U8bit_t *p_len
	)
{
    /* discriminator */
    *p_item_list = ROHC_SACK_3_DISC;
    (*p_len)++;
    /* each block contains two 32bit pointers */
    /* call encoding of block1 start field */
    sack_block_enc( *p_sack_field,ack_no,p_item_list+*p_len,p_len);
    /* call encoding of block1 end field */
    sack_block_enc(*(p_sack_field+1) ,ROHC_NTOHL(*p_sack_field),p_item_list+*p_len,p_len);
    /* call encoding of block2 start field */
    sack_block_enc(*(p_sack_field+2) ,ROHC_NTOHL(*(p_sack_field+1)),p_item_list+*p_len,
            p_len);
    /* call encoding of block2 end field */
    sack_block_enc(*(p_sack_field+3) ,ROHC_NTOHL(*(p_sack_field+2)),p_item_list+*p_len,
            p_len);
    /* call encoding of block3 start field */
    sack_block_enc(*(p_sack_field+4) ,ROHC_NTOHL(*(p_sack_field+3)),p_item_list+*p_len,
            p_len);
    /* call encoding of block3 end field */
    sack_block_enc(*(p_sack_field+5) ,ROHC_NTOHL(*(p_sack_field+4)),p_item_list+*p_len,
            p_len);
}

/*****************************************************************************
**    	FUNCTION:
**        	sack4_item_enc
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to encode SACK option
**   	ARGUMENTS: 
**         	The compressor context are taken as input
**			and length, compressed list are output parameters.
**    	RETURN VALUE:
**        	rohc_return_t
*****************************************************************************/
rohc_void_t sack4_item_enc ROHC_PARAMS
	(
		(p_sack_field, ack_no, p_item_list, p_len),
			rohc_U32bit_t *p_sack_field     _AND_
			rohc_U32bit_t  ack_no           _AND_
			rohc_U8bit_t *p_item_list 	    _AND_
			rohc_U8bit_t *p_len
	)
{
    /* discriminator */
    *p_item_list = ROHC_SACK_4_DISC;
    (*p_len)++;
    /* each block contains two 32bit pointers */
    /* call encoding of block1 start field */
    sack_block_enc( *p_sack_field,ack_no,p_item_list+*p_len,p_len);
    /* call encoding of block1 end field */
    sack_block_enc(*(p_sack_field+1) ,ROHC_NTOHL(*p_sack_field),p_item_list+*p_len,p_len);
    /* call encoding of block2 start field */
    sack_block_enc(*(p_sack_field+2) ,ROHC_NTOHL(*(p_sack_field+1)),p_item_list+*p_len,
            p_len);
    /* call encoding of block2 end field */
    sack_block_enc(*(p_sack_field+3) ,ROHC_NTOHL(*(p_sack_field+2)),p_item_list+*p_len,
            p_len);
    /* call encoding of block3 start field */
    sack_block_enc(*(p_sack_field+4) ,ROHC_NTOHL(*(p_sack_field+3)),p_item_list+*p_len,
            p_len);
    /* call encoding of block3 end field */
    sack_block_enc(*(p_sack_field+5) ,ROHC_NTOHL(*(p_sack_field+4)),p_item_list+*p_len,
            p_len);
    /* call encoding of block4 start field */
    sack_block_enc(*(p_sack_field+6) ,ROHC_NTOHL(*(p_sack_field+5)),p_item_list+*p_len,
            p_len);
    /* call encoding of block4 end field */
    sack_block_enc(*(p_sack_field+7) ,ROHC_NTOHL(*(p_sack_field+6)),p_item_list+*p_len,
            p_len);
}

/*****************************************************************************
**    	FUNCTION:
**        	sack_block_enc
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to encode SACK block
**   	ARGUMENTS: 
**         	The compressor context are taken as input
**			and length, compressed list are output parameters.
**    	RETURN VALUE:
**        	void
*****************************************************************************/
rohc_void_t sack_block_enc ROHC_PARAMS
	(
		(sack_field, ref_val, p_item_list, p_len),
			rohc_U32bit_t sack_field        _AND_
			rohc_U32bit_t ref_val           _AND_
			rohc_U8bit_t *p_item_list 	    _AND_
			rohc_U8bit_t *p_len
	)
{
    rohc_U8bit_t no_of_bits;
    rohc_U16bit_t temp_16bit_val = 0;
    rohc_U8bit_t temp_8bit_val = 0;
    /* convert values of sack in netowrk-to-host order before processing*/
    sack_field = ROHC_NTOHL(sack_field);
    no_of_bits = sack_pure_lsb(sack_field, ref_val);
    if(15 == no_of_bits)
    {
        temp_16bit_val = (rohc_U16bit_t)sack_field;
        temp_16bit_val &= 0x7fff;
        ROHC_SET_2_OCTS(p_item_list,temp_16bit_val);
        *p_len+=2;
    }
    else if(22 == no_of_bits)
    {
        temp_8bit_val = 0x80;
        temp_8bit_val |= (sack_field >> 16)& 0x3f;
        *p_item_list++ = temp_8bit_val;
        /* now set the remaining 2 octets */
        temp_16bit_val = (rohc_U16bit_t)sack_field;
        ROHC_SET_2_OCTS(p_item_list,temp_16bit_val);
        *p_len+=3;
    }
    else if(29 == no_of_bits)
    {
        temp_8bit_val = 0xc0;
        temp_8bit_val |= (sack_field >> 24)& 0x1f;
        *p_item_list++ = temp_8bit_val;
        temp_8bit_val = (sack_field >> 16)& 0xff;
        *p_item_list++ = temp_8bit_val;
        /* now set the remaining 2 octets */
        temp_16bit_val = (rohc_U16bit_t)sack_field;
        ROHC_SET_2_OCTS(p_item_list,temp_16bit_val);
        *p_len+=4;
    }
    else
    {/* case of full offset */
        *p_item_list++ = 255; /* discriminator 11111111 */
        ROHC_SET_4_OCTS(p_item_list,sack_field);
        *p_len+=5;
    }
}

/*****************************************************************************
**    	FUNCTION:
**        	sack_pure_lsb
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used find no. of lsbs to send 
**   	ARGUMENTS: 
**         	The compressor context are taken as input
**			and no of bits is output parameter.
**    	RETURN VALUE:
**        	rohc_return_t
*****************************************************************************/
rohc_return_t sack_pure_lsb ROHC_PARAMS
	(
		(sack_field, ref_val),
			rohc_U32bit_t sack_field        _AND_
			rohc_U32bit_t ref_val           
	)
{
    rohc_U8bit_t no_of_bits;
    rohc_U32bit_t offset;
    offset = sack_field - ref_val;
    if(offset <= 0x7fff)
    {
        no_of_bits = 15;
    }
    else if(offset <= 0x3fffff)
    {
        no_of_bits = 22;
    }
    else if(offset <= 0x1fffffff)
    {
        no_of_bits = 29;
    }
    else
    {
        no_of_bits = 32;
    }
    return no_of_bits;
}

/*****************************************************************************
**    	FUNCTION:
**        	rohc_c_list_enc_tcp_opt_pkt
******************************************************************************
**
**    	DESCRIPTION:
**         	This function is used to encode tcp options in IR/IRDYN pkt.
**   	ARGUMENTS: 
**         	The compressor context are taken as input
**			and length, compressed list are output parameters.
**    	RETURN VALUE:
**        	rohc_return_t
*****************************************************************************/

rohc_U32bit_t  rohc_c_list_enc_tcp_opt_pkt ROHC_PARAMS
(
    (PS_flag, count, XI_list, item_list, size,
     p_compressed_list),
     rohc_U8bit_t        PS_flag _AND_
     rohc_U8bit_t        count _AND_
     rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_TCP_OPT_IDX] _AND_
     /* SPR 19690 fix start */
     rohc_U8bit_t       item_list[MAX_TCP_OPT_SIZE] _AND_
     /* SPR 19690 fix end */
     rohc_U8bit_t        size _AND_
     rohc_U8bit_t        *p_compressed_list
 )
{
    rohc_U32bit_t       i;
    rohc_U32bit_t       tot_size = 0;

    ROHC_ENTER_FN("rohc_c_list_enc_tcp_opt_pkt");
    *p_compressed_list = ROHC_SET_LOWER_NIBBLE(*p_compressed_list, count);
    if(PS_flag)
    {
        *p_compressed_list = (rohc_U8bit_t)ROHC_SET_BIT(*p_compressed_list, 
                ROHC_PS_BIT);
    }
    p_compressed_list = p_compressed_list + 1;
    tot_size++;
    if(PS_flag)
    {
        for(i=0; i<count; i++)
        {
            *(p_compressed_list + i) = XI_list[i];
        }
        p_compressed_list = p_compressed_list + count;
        tot_size = tot_size + count;
    }
    else /* PS = 0 */
    {
        if(count == 1)
        {
            *p_compressed_list = ROHC_SET_UPPER_NIBBLE(*p_compressed_list, 
                    XI_list[0]);
            p_compressed_list = p_compressed_list + 1;
            tot_size++;
        }
        else
        {
            for(i=1; i<count; i=i + 2)
            {
                if(i < ROHC_MAX_ENTRIES_TCP_OPT_IDX)
                {
                    *p_compressed_list = ROHC_SET_LOWER_NIBBLE(
                            *p_compressed_list, XI_list[i]);
                    *p_compressed_list = ROHC_SET_UPPER_NIBBLE(
                            *p_compressed_list, XI_list[i-1]);
                    p_compressed_list = p_compressed_list + 1;
                    tot_size++;
                }
            }
            /* Coverity Fix 54587 */
            if(((count % 2) != 0) && ( count < ROHC_MAX_ENTRIES_TCP_OPT_IDX ))/* need to set padding */
            {
            /* Coverity Fix 54587 */
                *p_compressed_list = ROHC_SET_UPPER_NIBBLE(*p_compressed_list,
                        XI_list[count-1]);
                p_compressed_list = p_compressed_list + 1;
                tot_size++;
            }
        }
    }
    ROHC_MEMCPY(p_compressed_list, item_list, size);
    tot_size = tot_size +  size;

    ROHC_EXIT_FN("rohc_c_list_enc_tcp_opt_pkt");
    return tot_size;
}
