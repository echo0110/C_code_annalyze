/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_dprm_update_p6.c,v 1.1.1.1.16.1 2013/10/06 12:13:00 gur32542 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains function declaration for DPIB to DSC
 *                     updation for profile p6.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_dprm_update_p6.c,v $
 * Revision 1.1.1.1.16.1  2010/11/03 07:07:56  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.3.28.6  2010/10/13 04:00:06  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.3.28.5  2010/10/01 14:08:37  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.3.28.4  2010/07/08 09:30:19  gur21145
 * code review comments for prof0,2 enhancement activity
 *
 * Revision 1.3.28.3  2010/07/07 11:56:43  gur21145
 * PROFILE2_comments_and PROFILE0_comments_inc
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
#include "rohc_dprm_proto.h"
#include "rohc_pbm_ext_if.h"
#include "rohc_ed_def.h"
#include "rohc_d_pbm.h"
#include "rohc_ed_proto.h"
/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static rohc_void_t rohcDprmParseSackBlock( 
        rohc_U32bit_t          *baseNum,  
        rohc_dsc_tcp_t         *p_dsc   , 
        rohc_dprm_update_blk_t *p_dprm_update_blk ,
        rohc_U8bit_t *p_size ,
        rohc_U8bit_t *p_opt_list_index );

STATIC rohc_void_t rohc_p6_update_dsc_ip (
        rohc_dsc_tcp_t         *p_dsc ,
        rohc_dprm_update_blk_t *p_dprm_update_blk);

STATIC rohc_void_t rohc_p6_update_dsc_tcp (
        rohc_dsc_tcp_t         *p_dsc  ,
        rohc_dprm_update_blk_t *p_dprm_update_blk);

STATIC rohc_void_t rohc_p6_update_dsc_tcp_dyn (
        rohc_dsc_tcp_t         *p_dsc    ,
        rohc_dprm_update_blk_t *p_dprm_update_blk);

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



/***************************************************************************
*  Function Name    : rohcDprmP6BaseUpdatePkt
*
*  Description      :
*                This function provides the funtionality to update the 
*                ts/msn/ip_id/ip_id2/ in the DSC for received ROHC packet 
*
*  Parameters       :
*         p_entity : Pointer to rohc entity block
*         p_dsc    : Pointer to rohc decompressor stream context block
*
*  Return Value     :void
*
* CAUTION/NOTE:
****************************************************************************/

rohc_void_t rohcDprmP6BaseUpdatePkt(  
                    rohc_entity_t *p_entity,
                    /* + SPR 17439 */
                    rohc_dsc_tcp_t *p_dsc,
                    /* - SPR 17439 */
                    rohc_dprm_update_blk_t *p_dprm_update_blk)
{

    ROHC_ENTER_FN("rohcDprmP6BaseUpdatePkt");
    if(ROHC_TRUE == ((rohc_dsc_tcp_t *)p_dsc)->tcp_cksum_present)
    {
        (p_dprm_update_blk)->tcp->checksum =
                ROHC_HTONS((rohc_U16bit_t)((rohc_dsc_tcp_t *)p_dsc)->dpib->field_val
                [ROHC_TCP_CKSUM_FID - ROHC_VAR_FID_BASE]);
    }
    
    if(((rohc_dsc_tcp_t *)p_dsc)->bit_mask & ROHC_UPDATE_DSC_FOR_ED)
    {
        if ( ((rohc_dsc_tcp_t *)p_dsc)->dpib->decoded_sn )
        {
            ((rohc_dsc_tcp_t *)p_dsc)->sn_prev =((rohc_dsc_tcp_t *)p_dsc)->dpib->
                decoded_sn ;
        }   
        ((rohc_dsc_tcp_t *)p_dsc)->msn_prev =((rohc_dsc_tcp_t *)p_dsc)->dpib->
            decoded_msn ;   
        if ( ((rohc_dsc_tcp_t *)p_dsc)->dpib->decoded_ack_num )
        {    
            ((rohc_dsc_tcp_t *)p_dsc)->ack_recvd_prev =((rohc_dsc_tcp_t *)p_dsc)->
                dpib->decoded_ack_num;
        }   

        if ( ((rohc_dsc_tcp_t *)p_dsc)->dpib->decoded_ttl )
        {
            ((rohc_dsc_tcp_t *)p_dsc)->ttl_prev =
                ((rohc_dsc_tcp_t *)p_dsc)->dpib->decoded_ttl;
        }
        else 
        {
            if( ((rohc_dsc_tcp_t *)p_dsc)->dpib->field_val
                    [ROHC_INNERIP4_TTL_HPL_PRESENT_FID- ROHC_TCP_CTRL_FID_BASE])
            {
                ((rohc_dsc_tcp_t *)p_dsc)->ttl_prev =
                    ((rohc_dsc_tcp_t *)p_dsc)->dpib->field_val
                    [ROHC_INNERIP4_TTL_FID-ROHC_VAR_FID_BASE];
            }
        }

        if ( ((rohc_dsc_tcp_t *)p_dsc)->dpib->decoded_ts )
        {
            ((rohc_dsc_tcp_t *)p_dsc)->pre_ts =((rohc_dsc_tcp_t *)p_dsc)->dpib->
                decoded_ts ;  
        }
        if ( ((rohc_dsc_tcp_t *)p_dsc)->dpib->decodedTsEchoVal )
        {
            ((rohc_dsc_tcp_t *)p_dsc)->tsEchoVal =((rohc_dsc_tcp_t *)p_dsc)->dpib->
                decodedTsEchoVal ;  
        }
        if ( (((rohc_dsc_tcp_t *)p_dsc)->dpib)->
                field_val[ROHC_TCP_ACK_STRIDE_FID - ROHC_VAR_FID_BASE] )
        {
           ( (rohc_dsc_tcp_t *)p_dsc)->ackstride = 
                (((rohc_dsc_tcp_t *)p_dsc)->dpib->
                field_val[ROHC_TCP_ACK_STRIDE_FID - ROHC_VAR_FID_BASE]);
        }
        if ( (ROHC_CO_P6_RND8_PKTYPE == (((rohc_dsc_tcp_t *)p_dsc)->dpib)->rcv_pkt_typ) || 
                (ROHC_CO_P6_SEQ8_PKTYPE == (((rohc_dsc_tcp_t *)p_dsc)->dpib)->rcv_pkt_typ) ||
                (ROHC_CO_P6_COMMON_PKTYPE == (((rohc_dsc_tcp_t *)p_dsc)->dpib)->rcv_pkt_typ) )
        {
            ((rohc_dsc_tcp_t *)p_dsc)->ecn_used = ((((rohc_dsc_tcp_t *)p_dsc)->dpib)->
                field_val[ROHC_TCP_ECN_USED_FID - ROHC_VAR_FID_BASE]);
        }

	if ((ROHC_CO_P6_SEQ7_PKTYPE == (((rohc_dsc_tcp_t *)p_dsc)->dpib)->rcv_pkt_typ))
	{
        	((rohc_dsc_tcp_t *)p_dsc)->win_prev =((rohc_dsc_tcp_t *)p_dsc)->dpib->
            		decoded_win ;   
	}
	
	((rohc_dsc_tcp_t *)p_dsc)->pre_inner_ip_id = (((rohc_dsc_tcp_t *)p_dsc)->dpib)->decoded_ipid;

	/* Called Make to Update DSC Context Inner Ip / TCP / TCP option */    
	rohcDecompMakeInnerIp4BlockP6( (rohc_dsc_tcp_t *)p_dsc , p_dprm_update_blk);
	rohcDecompMakeTcpBlockP6( (rohc_dsc_tcp_t *)p_dsc , p_dprm_update_blk);
        rohcDecompMakeTcpIrregChain( (rohc_dsc_tcp_t *)p_dsc , p_dprm_update_blk ); 
    }

    if(ROHC_NULL != p_dprm_update_blk->inner_ip4)
    {
        p_dprm_update_blk->inner_ip4->cksum = 0;
        p_dprm_update_blk->inner_ip4->cksum =
                (rohc_cal_ip_checksum(
                        (rohc_U8bit_t *)p_dprm_update_blk->inner_ip4,
                                            20));

    }
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    /* note marker bit need not to be updated in the context*/
    /* if value inner ip id is non ROHC_NULL update the ctx*/
    
    ROHC_EXIT_FN("rohcDprmP6BaseUpdatePkt");
}

/***************************************************************************
*  Function Name    : rohcDprmP6CRBaseUpdatePkt
*
*  Description      :
*                This function provides the funtionality to update the
*                ts/msn/ip_id/ip_id2/ in the DSC for received ROHC packet
*
*  Parameters       :
*         p_entity : Pointer to rohc entity block
*         p_dsc    : Pointer to rohc decompressor stream context block
*
*  Return Value     :void
*
* CAUTION/NOTE:
****************************************************************************/

rohc_void_t rohcDprmP6CRBaseUpdatePkt(
                    rohc_entity_t *p_entity,
                    /* + SPR 17439 */
                    rohc_dsc_tcp_t *p_dsc,
                    /* - SPR 17439 */
                    rohc_dprm_update_blk_t *p_dprm_update_blk)
{
    ROHC_ENTER_FN("rohcDprmP6CRBaseUpdatePkt");
    /* update MSN */
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),sizeof(p_dprm_update_blk),0,0, __func__,"");
    /* +- SPR 17777 */
    ((rohc_dsc_tcp_t *)p_dsc)->msn_prev =ROHC_GET_2BYTES(((rohc_dsc_tcp_t *)
               p_dsc)->dpib->tcp_replicate_partial->msn);

    /* update Ack Stride */
    if (ROHC_REP_ACK_STRD_FLAG_MASK & ((((rohc_dsc_tcp_t *)p_dsc)->dpib)->
                tcp_replicate_partial->winp_listp_srcprt_dstprt_asf))
    {
        ( (rohc_dsc_tcp_t *)p_dsc)->ackstride =
            ROHC_GET_2BYTES(((rohc_dsc_tcp_t *)p_dsc)->dpib->
                    tcp_replicate_ackstride->ackstrd);
    }

    ROHC_EXIT_FN("rohcDprmP6CRBaseUpdatePkt");
}

/***************************************************************************
*  Function Name    : rohc_void_t  rohcDprmP6IrIrdynTcpOption
*
*  Description      :
*                This function provides the funtionality to update the 
*                dprm update blk with the TCP Options  
*
*  Parameters       :
*         p_entity : Pointer to rohc entity block
*         p_dsc    : Pointer to rohc decompressor stream context block
*
*  Return Value     :void
*
* CAUTION/NOTE:
****************************************************************************/
rohc_void_t rohcDprmP6IrIrdynTcpOption(
        rohc_dprm_update_blk_t *p_dprm_update_blk,
        rohc_dsc_tcp_t         *p_dsc,
        rohc_U8bit_t XI,
        rohc_U8bit_t *p_size,
        rohc_U8bit_t *p_opt_list_index)
{
   rohc_U8bit_t         padding_size; 
   rohc_U32bit_t        ackNum  = 0 ;  
   rohc_U8bit_t         genLength; 
          
   ROHC_ENTER_FN("rohcDprmP6IrIrdynTcpOption");
   
   switch(XI)
   {
       case INDEX_NOP:
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 1;
            break;
       case INDEX_EOL:
                padding_size = *(p_dsc->dpib->tcp_dyn_options_items->xi_items
                + *p_opt_list_index)/8;
                /*Option header field always ends at a 32 bit word boundary,
                it option header is such that it does not lies within the word 
                boundary ,it is appended with padding bits*/
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 0;
                
                ROHC_MEMSET((((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)),
                            0 ,padding_size);
                (*p_size) += padding_size;
            break;
       case INDEX_MSS:
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 2;   
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 4;
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 
                            *(p_dsc->dpib->tcp_dyn_options_items->
                             xi_items + (*p_opt_list_index)++);
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 
                            *(p_dsc->dpib->tcp_dyn_options_items->
                             xi_items + (*p_opt_list_index)++);
            break;
       case INDEX_WINDOW_SCAL:
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 3;
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 3;
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 
                            *(p_dsc->dpib->tcp_dyn_options_items->
                             xi_items + (*p_opt_list_index)++);
            break;
       
        case INDEX_TIMESTAMP:
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 8;
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 10;
                ROHC_MEMCPY((((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + *p_size), 
                        (p_dsc->dpib->tcp_dyn_options_items->xi_items 
                        + *p_opt_list_index), 8 );
                ((rohc_dsc_tcp_t *)p_dsc)->pre_ts = ROHC_GET_4BYTES
                    (((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + *p_size);
                *p_size +=4;
                ((rohc_dsc_tcp_t *)p_dsc)->tsEchoVal = ROHC_GET_4BYTES
                    (((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + *p_size);
                *p_size +=4;
                *p_opt_list_index +=8;
           break;
       case INDEX_SACK_PERM:
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 4;
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 2;
           break;
       case INDEX_SACK:
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 5;
                ackNum =  ROHC_HTONL(p_dprm_update_blk->tcp->ack); 
                switch (*(p_dsc->dpib->tcp_dyn_options_items->xi_items 
                        + (*p_opt_list_index)++))  
                {
                    case 0x04: // ROHC_SACK_4_LIST_ITEM:
                            *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 0x22;
                            /* Block Start And Block End*/    
                            rohcDprmParseSackBlock (&ackNum, 
                                    p_dsc , p_dprm_update_blk ,p_size ,p_opt_list_index);
                            rohcDprmParseSackBlock (&ackNum, 
                                    p_dsc , p_dprm_update_blk ,p_size ,p_opt_list_index);
                            rohcDprmParseSackBlock (&ackNum, 
                                    p_dsc , p_dprm_update_blk ,p_size ,p_opt_list_index);
                            rohcDprmParseSackBlock (&ackNum, 
                                    p_dsc , p_dprm_update_blk ,p_size ,p_opt_list_index);
                            rohcDprmParseSackBlock (&ackNum, 
                                    p_dsc , p_dprm_update_blk ,p_size ,p_opt_list_index);
                            rohcDprmParseSackBlock (&ackNum, 
                                    p_dsc , p_dprm_update_blk ,p_size ,p_opt_list_index);
                            rohcDprmParseSackBlock (&ackNum, 
                                    p_dsc , p_dprm_update_blk ,p_size ,p_opt_list_index);
                            rohcDprmParseSackBlock (&ackNum, 
                                    p_dsc , p_dprm_update_blk ,p_size ,p_opt_list_index);
                            
                            break;                                        
                    case 0x03: //ROHC_SACK_3_LIST_ITEM:
                            /* Block Start And Block End*/    
                            *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 0x1A;
                            rohcDprmParseSackBlock (&ackNum, 
                                    p_dsc , p_dprm_update_blk ,p_size ,p_opt_list_index);
                            rohcDprmParseSackBlock (&ackNum, 
                                    p_dsc , p_dprm_update_blk ,p_size ,p_opt_list_index);
                            rohcDprmParseSackBlock (&ackNum, 
                                    p_dsc , p_dprm_update_blk ,p_size ,p_opt_list_index);
                            rohcDprmParseSackBlock (&ackNum, 
                                    p_dsc , p_dprm_update_blk ,p_size ,p_opt_list_index);
                            rohcDprmParseSackBlock (&ackNum, 
                                    p_dsc , p_dprm_update_blk ,p_size ,p_opt_list_index);
                            rohcDprmParseSackBlock (&ackNum, 
                                    p_dsc , p_dprm_update_blk ,p_size ,p_opt_list_index);
                    break;   
                    case 0x02: //ROHC_SACK_3_LIST_ITEM:
                            *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 0x12;
                            /* Block Start And Block End*/    
                            rohcDprmParseSackBlock (&ackNum, 
                                    p_dsc , p_dprm_update_blk ,p_size ,p_opt_list_index);
                            rohcDprmParseSackBlock (&ackNum, 
                                    p_dsc , p_dprm_update_blk ,p_size ,p_opt_list_index);
                            rohcDprmParseSackBlock (&ackNum, 
                                    p_dsc , p_dprm_update_blk ,p_size ,p_opt_list_index);
                            rohcDprmParseSackBlock (&ackNum, 
                                    p_dsc , p_dprm_update_blk ,p_size ,p_opt_list_index);
                            break;
                    case 0x01: //ROHC_SACK_1_LIST_ITEM:    
                            /* Block1 Start And Block1 End*/    
                            *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 0x0A;
                            rohcDprmParseSackBlock (&ackNum, 
                                    p_dsc , p_dprm_update_blk ,p_size ,p_opt_list_index);
                            rohcDprmParseSackBlock (&ackNum, 
                                    p_dsc , p_dprm_update_blk ,p_size ,p_opt_list_index);
                break;  
                }     
                break;
            default:
                /* Generic Options Handling*/ 
                if ((p_dsc)->bit_mask & ROHC_UPDATE_DSC_FOR_ED )
                {
                    p_dsc->tcpOptGenStaticFlag =   ((0x80 &
                    *(p_dsc->dpib->tcp_dyn_options_items->
                     xi_items + (*(p_opt_list_index) + 1 ))) >> 7) <<
                   (*(p_dsc->dpib->tcp_dyn_options_items->xi_items + (*p_opt_list_index )));
                   if(XI < ROHC_MAX_ENTRIES_TCP_OPT_IDX)
                    {
                    p_dsc->optionTypeMap[XI - INDEX_SACK]=
                     *(p_dsc->dpib->tcp_dyn_options_items->xi_items + (*p_opt_list_index )) ;
                    }

                }
                
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 
                        *(p_dsc->dpib->tcp_dyn_options_items->
                             xi_items + (*p_opt_list_index)++);
                genLength = (0x7F & *(p_dsc->dpib->tcp_dyn_options_items->
                             xi_items + (*p_opt_list_index)++));
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = genLength;
                ROHC_MEMCPY((((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) , 
                            (p_dsc->dpib->tcp_dyn_options_items->
                             xi_items + (*p_opt_list_index)) , genLength);
                (*p_opt_list_index) +=  (genLength -2) ;  
                (*p_size) +=  (genLength -2);  
    }   
    ROHC_EXIT_FN("rohcDprmP6IrIrdynTcpOption");

}

/***************************************************************************
*  Function Name    : rohcDprmParseSackBlock 
*
*  Description      : This function parses the Sack Block received  
*                     in IR buffer.
*  Parameters       :
*         p_entity : Pointer to rohc entity block
*         p_dsc    : Pointer to rohc decompressor stream context block
*
*  Return Value     :void
*
* CAUTION/NOTE:
****************************************************************************/
static rohc_void_t rohcDprmParseSackBlock( 
        rohc_U32bit_t          *baseNum,  
        rohc_dsc_tcp_t         *p_dsc   , 
        rohc_dprm_update_blk_t *p_dprm_update_blk ,
        rohc_U8bit_t *p_size ,
        rohc_U8bit_t *p_opt_list_index )
{
    rohc_encoded_value_t    en_tmp;
    rohc_error_code_t ecode ;
    /*this field will get Initilaised when Switch  Block will get Executed*/ 
    if ( !(( *(p_dsc->dpib->tcp_dyn_options_items->xi_items
                        + (*p_opt_list_index))) & 0x80 ))
    {
        /*
         *  COMPRESSED lsb_15 {
         *      discriminator =:= '0' [ 1 ];
         *      sack_offset =:= lsb(15, -1) [ 15 ];
         *  }
         */
        en_tmp.value = ( (0x00007FFF) & (ROHC_GET_2BYTES(
                        (p_dsc->dpib->tcp_dyn_options_items->xi_items 
                         + *p_opt_list_index))) );
        (*p_opt_list_index) += 2;
        en_tmp.bits = 15;
        rohcDecodeP6SackBlock(p_dsc, &en_tmp, baseNum  ,&ecode , baseNum);

        ROHC_SET_4_OCTS((((((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + *p_size)))  
                , *baseNum);
        (*p_size) += 4;
    }    
    else
    {    
        if ( 0x80  ==(( *(p_dsc->dpib->tcp_dyn_options_items->xi_items
                            + (*p_opt_list_index))) & 0xC0 ))
        {  
            /*
             *  COMPRESSED lsb_22 {
             *      discriminator =:= '10' [ 2 ];
             *      sack_offset =:= lsb(22, -1) [ 22 ];
             *  }
             */

            en_tmp.value = ( (0x003FFFFF) & (ROHC_GET_3BYTES(
                            (p_dsc->dpib->tcp_dyn_options_items->xi_items 
                             + *p_opt_list_index))) );
            (*p_opt_list_index) += 3;
            en_tmp.bits = 22;
            rohcDecodeP6SackBlock(p_dsc, &en_tmp, baseNum  ,&ecode , baseNum);
            ROHC_SET_4_OCTS((((((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + *p_size)))  
                    ,*baseNum); 
            (*p_size) += 4;
        } 
        else
        {
            if ( 0xC0  ==(( *(p_dsc->dpib->tcp_dyn_options_items->xi_items
                                + (*p_opt_list_index))) & 0xC0 ))
            {  
                /*
                 *  COMPRESSED lsb_32 {
                 *      discriminator =:= '11' [ 2 ];
                 *      sack_offset =:= lsb(30, -1) [ 30 ];
                 *  }
                 */
                en_tmp.value = ( (0x3FFFFFFF) & (ROHC_GET_4BYTES(
                                (p_dsc->dpib->tcp_dyn_options_items->xi_items 
                                 + *p_opt_list_index))) );
                (*p_opt_list_index) += 4;
                en_tmp.bits = 30;
                rohcDecodeP6SackBlock(p_dsc, &en_tmp, baseNum  ,&ecode , baseNum);
                ROHC_SET_4_OCTS((((((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + *p_size)))  
                        , *baseNum); 
                (*p_size) += 4;
            }

        }
    }
}


/***************************************************************************
*  Function Name    : rohc_dprm_p6_ir_irdyn_update_pkt
*
*  Description      :
*                This function provides the funtionality to update the DSC for
*                for received for IR/IR-DYN.
*
*  Parameters       :
*         p_entity : Pointer to rohc entity block
*         p_dsc    : Pointer to rohc decompressor stream context block
*
*  Return Value     :void
*
* CAUTION/NOTE:
****************************************************************************/
rohc_void_t rohc_dprm_p6_ir_irdyn_update_pkt 
        ROHC_PARAMS((p_entity, p_dsc, p_dprm_update_blk),\
        rohc_entity_t          *p_entity _AND_ 
        rohc_dsc_tcp_t         *p_dsc    _AND_
        rohc_dprm_update_blk_t *p_dprm_update_blk)
{
   
    
    ROHC_ENTER_FN("rohc_dprm_p6_ir_irdyn_update_pkt");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    
    p_dsc->profile = p_dsc->dpib->profile;
    rohc_p6_update_dsc_ip(p_dsc,p_dprm_update_blk);
    rohc_p6_update_dsc_tcp(p_dsc,p_dprm_update_blk);
   
    ROHC_EXIT_FN("rohc_dprm_p6_ir_irdyn_update_pkt");
}

/***************************************************************************
*  Function Name    : rohc_p6_update_dsc_ip
*
*  Description      :
*                This function provides the funtionality to update the DSC for
*                 ip 
*
*  Parameters       :
*         p_dsc    : Pointer to rohc decompressor stream context block
*         p_dprm_update_blk: Pointer to rohc decompressor update block
*
*  Return Value     :void
*
* CAUTION/NOTE:
****************************************************************************/
STATIC rohc_void_t rohc_p6_update_dsc_ip (
        rohc_dsc_tcp_t         *p_dsc   ,
        rohc_dprm_update_blk_t *p_dprm_update_blk)
{
    rohc_U16bit_t u16_tmp;
    /* IP6 */
    if(ROHC_NULL != p_dsc->dpib->inner_ip6_static)
    {
        /* for version, flowlabel */    
        u16_tmp = ROHC_GET_2BYTES(p_dsc->dpib->inner_ip6_static->flowlabel);
        p_dprm_update_blk->inner_ip6->ip6_ctlun.ip6_un1.un1_flow |= 
            ROHC_HTONL(0x60000000 |
                    ((p_dsc->dpib->inner_ip6_static->v_flowlabel&
                      ROHC_IP6_STATIC_FLOWLABEL_MASK) << 16) | u16_tmp);

        p_dprm_update_blk->inner_ip6->ip6_ctlun.ip6_un1.un1_nxt = 
            (p_dsc->dpib->inner_ip6_static->next_hdr);

        ROHC_MEMCPY(p_dprm_update_blk->inner_ip6->src,
                p_dsc->dpib->inner_ip6_static->src,16);
        ROHC_MEMCPY(p_dprm_update_blk->inner_ip6->dst,
                p_dsc->dpib->inner_ip6_static->dst,16);
    }
    if(ROHC_NULL != p_dsc->dpib->inner_ip6_dyn)
    {
        p_dprm_update_blk->inner_ip6->ip6_ctlun.ip6_un1.un1_flow |= 
            ROHC_HTONL((p_dsc->dpib->inner_ip6_dyn->traffic_class<<20));

        p_dprm_update_blk->inner_ip6->ip6_ctlun.ip6_un1.un1_hlim = 
            (p_dsc->dpib->inner_ip6_dyn->hop_limit);     

    }
    if(ROHC_NULL != p_dsc->dpib->outer_ip6_static)
    {
        /* for version, flowlabel */    
        u16_tmp = ROHC_GET_2BYTES(p_dsc->dpib->outer_ip6_static->flowlabel);

        p_dprm_update_blk->outer_ip6->ip6_ctlun.ip6_un1.un1_flow |= 
            ROHC_HTONL(0x60000000 |
                    ((p_dsc->dpib->outer_ip6_static->v_flowlabel
                      & ROHC_IP6_STATIC_FLOWLABEL_MASK) << 16)| u16_tmp) ; 

        p_dprm_update_blk->outer_ip6->ip6_ctlun.ip6_un1.un1_nxt = 
            (p_dsc->dpib->outer_ip6_static->next_hdr);

        ROHC_MEMCPY(p_dprm_update_blk->outer_ip6->src,
                p_dsc->dpib->outer_ip6_static->src,16);
        ROHC_MEMCPY(p_dprm_update_blk->outer_ip6->dst,
                p_dsc->dpib->outer_ip6_static->dst,16);
    }
    if(ROHC_NULL != p_dsc->dpib->outer_ip6_dyn)
    {
        p_dprm_update_blk->outer_ip6->ip6_ctlun.ip6_un1.un1_flow |=
            /* SPR 4808 Fix Start */ 
            ROHC_HTONL(p_dsc->dpib->outer_ip6_dyn->traffic_class<<20); 
        /* SPR 4808 Fix End */ 
        p_dprm_update_blk->outer_ip6->ip6_ctlun.ip6_un1.un1_hlim =
            (p_dsc->dpib->outer_ip6_dyn->hop_limit);     
    }
    /* IP4*/
    if(ROHC_NULL != p_dsc->dpib->inner_ip4_static)
    {
        p_dprm_update_blk->inner_ip4->vhl = (IPV4<<4)|ROHC_IHL_VAL; 
        p_dprm_update_blk->inner_ip4->proto  = p_dsc->dpib->inner_ip4_static->proto;

        ROHC_ASSIGN_4_OCTS((p_dsc->dpib->inner_ip4_static->src) ,
                ((rohc_U8bit_t*)(&p_dprm_update_blk->inner_ip4->src)));

        ROHC_ASSIGN_4_OCTS( (p_dsc->dpib->inner_ip4_static->dst) , 
                ((rohc_U8bit_t*)(&p_dprm_update_blk->inner_ip4->dst)));

    }
    if(ROHC_NULL != p_dsc->dpib->inner_ip4_P6_dyn)
    {
        p_dprm_update_blk->inner_ip4->tos  = p_dsc->dpib->inner_ip4_P6_dyn->
            dscp_ecn;
        p_dsc->ip_inner_ecn =   (p_dsc->dpib->inner_ip4_P6_dyn-> dscp_ecn
                & ROHC_PROF6_IP4_ECN_FLAG_MASK);
        p_dprm_update_blk->inner_ip4->ttl = p_dsc->dpib->inner_ip4_P6_dyn->ttl_hopl; 
        p_dsc->ttl_prev = p_dsc->dpib->inner_ip4_P6_dyn->ttl_hopl;
        p_dprm_update_blk->inner_ip4->off = 
            ROHC_HTONS((rohc_U16bit_t) ((p_dsc->dpib->inner_ip4_P6_dyn->
                            res_df_ipid_beh & ROHC_PROF6_IP4_DYN_DF_MASK )));
        p_dprm_update_blk->inner_ip4->off=  p_dprm_update_blk->inner_ip4->off >>4;
        p_dsc->ipid_beh_inner = (p_dsc->dpib->inner_ip4_P6_dyn->res_df_ipid_beh
                & ROHC_PROF6_IP4_IPID_BEH_MASK);
        if(IP_ID_BEHAVIOR_ZERO ==  p_dsc->ipid_beh_inner)
        {	
            p_dprm_update_blk->inner_ip4->id = 0;	
        }
    }
    if(ROHC_NULL != p_dsc->dpib->inner_ip4_P6_dyn_id)
    {
        ROHC_ASSIGN_2_OCTS(p_dsc->dpib->inner_ip4_P6_dyn_id->id, 
                ((rohc_U8bit_t*)&p_dprm_update_blk->inner_ip4->id));
        p_dsc->pre_inner_ip_id = 
            ROHC_GET_2BYTES((p_dsc->dpib->inner_ip4_P6_dyn_id->id));
    }
    if(ROHC_NULL != p_dsc->dpib->outer_ip4_static)
    {
        p_dprm_update_blk->outer_ip4->vhl = (IPV4<<4)|ROHC_IHL_VAL; 
        p_dprm_update_blk->outer_ip4->proto = 
            p_dsc->dpib->outer_ip4_static->proto;
        ROHC_ASSIGN_4_OCTS((p_dsc->dpib->outer_ip4_static->src),
                ((rohc_U8bit_t*)(&p_dprm_update_blk->outer_ip4->src)));

        ROHC_ASSIGN_4_OCTS((p_dsc->dpib->outer_ip4_static->dst),
                ((rohc_U8bit_t*)(&p_dprm_update_blk->outer_ip4->dst)));
    }   
    if(ROHC_NULL != p_dsc->dpib->outer_ip4_P6_dyn)
    {
        p_dprm_update_blk->outer_ip4->tos  = p_dsc->dpib->outer_ip4_P6_dyn->
            dscp_ecn;
        p_dprm_update_blk->outer_ip4->ttl = p_dsc->dpib->outer_ip4_P6_dyn->
            ttl_hopl; 
        p_dsc->ttl_prev = p_dsc->dpib->outer_ip4_P6_dyn->ttl_hopl;
        p_dprm_update_blk->outer_ip4->off = 
            ROHC_HTONS((rohc_U16bit_t) ((p_dsc->dpib->outer_ip4_P6_dyn->
                            res_df_ipid_beh & ROHC_PROF6_IP4_DYN_DF_MASK)));
        p_dprm_update_blk->outer_ip4->off = p_dprm_update_blk->outer_ip4->off >>4;
        p_dsc->ipid_beh_outer = (p_dsc->dpib->outer_ip4_P6_dyn->res_df_ipid_beh
                & ROHC_PROF6_IP4_IPID_BEH_MASK);
    }
    if(ROHC_NULL != p_dsc->dpib->outer_ip4_P6_dyn_id)
    {
        ROHC_ASSIGN_2_OCTS((p_dsc->dpib->outer_ip4_P6_dyn_id->id), 
                ((rohc_U8bit_t*)&p_dprm_update_blk->outer_ip4->id));
        p_dsc->pre_outer_ip_id = p_dsc->outer_ip4->id = 
            ROHC_GET_2BYTES((p_dsc->dpib->outer_ip4_P6_dyn_id->id));
    }
}

/***************************************************************************
*  Function Name    : rohc_p6_update_dsc_tcp
*
*  Description      :
*                This function provides the funtionality to update the DSC for
*                 ip 
*
*  Parameters       :
*         p_dsc    : Pointer to rohc decompressor stream context block
*         p_dprm_update_blk: Pointer to rohc decompressor update block
*
*  Return Value     :void
*
* CAUTION/NOTE:
****************************************************************************/
STATIC rohc_void_t rohc_p6_update_dsc_tcp (
        rohc_dsc_tcp_t         *p_dsc   ,
        rohc_dprm_update_blk_t *p_dprm_update_blk)
{
    rohc_U8bit_t ps_mask = 0;
    rohc_U8bit_t m_count = 0;
    rohc_U8bit_t XI = 0x00;
    rohc_U8bit_t size_opt = sizeof(rohc_tcp_t);
    rohc_U8bit_t opt_item_list_index = 0;
    rohc_U16bit_t i=0;
    if( ROHC_NULL != p_dsc->dpib->tcp_dyn_options)
    {
        ps_mask = p_dsc->dpib->tcp_dyn_options->res_ps_m & ROHC_DYN_PS_MASK;
        m_count = p_dsc->dpib->tcp_dyn_options->res_ps_m & ROHC_DYN_TCP_M_MASK;
    }

    if(ROHC_NULL != p_dsc->dpib->tcp_static)
    {
        ROHC_ASSIGN_2_OCTS((p_dsc->dpib->tcp_static->src),
                ((rohc_U8bit_t *)&p_dprm_update_blk->tcp->sport));
        ROHC_ASSIGN_2_OCTS((p_dsc->dpib->tcp_static->dst),
                ((rohc_U8bit_t *)&p_dprm_update_blk->tcp->dport));

    }

    if(ROHC_NULL != p_dsc->dpib->tcp_dyn)
    {
        rohc_p6_update_dsc_tcp_dyn(p_dsc, p_dprm_update_blk);
    }

    if(ROHC_NULL != p_dsc->dpib->tcp_dyn_acknum)
    {
        ROHC_ASSIGN_4_OCTS((p_dsc->dpib->tcp_dyn_acknum->acknum),
                ((rohc_U8bit_t *)&p_dprm_update_blk->tcp->ack));
    }
    else
    {
        p_dprm_update_blk->tcp->ack = 0;
    }

    if(ROHC_NULL != p_dsc->dpib->tcp_dyn_windncksum)
    {
        ROHC_ASSIGN_2_OCTS((p_dsc->dpib->tcp_dyn_windncksum->window),
                ((rohc_U8bit_t *)&p_dprm_update_blk->tcp->window));
        p_dsc->win_prev = ROHC_GET_2BYTES
            (p_dsc->dpib->tcp_dyn_windncksum->window);
        ROHC_ASSIGN_2_OCTS((p_dsc->dpib->tcp_dyn_windncksum->checksum),
                ((rohc_U8bit_t *)&p_dprm_update_blk->tcp->checksum));
    }

    if(ROHC_NULL != p_dsc->dpib->tcp_dyn_urgptr)
    {
        ROHC_ASSIGN_2_OCTS((p_dsc->dpib->tcp_dyn_urgptr->urg_ptr),
                ((rohc_U8bit_t *)&p_dprm_update_blk->tcp->urg_ptr)); 
    }
    else
    {
        p_dprm_update_blk->tcp->urg_ptr = 0;
    }

    if(ROHC_NULL != p_dsc->dpib->tcp_dyn_ackstride)
    {
        p_dsc->ackstride = ROHC_GET_2BYTES((p_dsc->dpib->tcp_dyn_ackstride->ack_stride));
    }

    if(0 < m_count)
    {
        XI = 0x00;
        if(!ps_mask)//4bit XI
        {
            if(m_count % 2)
            {
                m_count = (m_count/2) + 1;
            }
            else
            {
                m_count /= 2;
            }
        }

        for(i=0;i<m_count;i++)
        {
            if(!ps_mask)//4bit XI
            {
                XI = ( (p_dsc->dpib->tcp_dyn_options_XI->xi_m[i] >> 4) & 0x07);
                rohcDprmP6IrIrdynTcpOption(p_dprm_update_blk,
                        p_dsc,XI,&size_opt,&opt_item_list_index);

                XI = (p_dsc->dpib->tcp_dyn_options_XI->xi_m[i] & 0x07);
                rohcDprmP6IrIrdynTcpOption(p_dprm_update_blk,
                        p_dsc,XI,&size_opt,&opt_item_list_index);
            }
            else//1byte XI
            {
                XI = ( p_dsc->dpib->tcp_dyn_options_XI->xi_m[i] & 0x0F);
                rohcDprmP6IrIrdynTcpOption(p_dprm_update_blk,
                        p_dsc,XI,&size_opt,&opt_item_list_index);
            }
        }
    }

    if(ROHC_NULL != p_dsc->dpib->tcp_dyn_options) 
    {   /* 4 bit shift as (offset = (4 bit offset + 3 bit Reserved + 1 bit NS)) */
        p_dprm_update_blk->tcp->offset = ((size_opt)/4)<<4;
    }
}

/***************************************************************************
*  Function Name    : rohc_p6_update_dsc_tcp_dyn
*
*  Description      :
*                This function provides the funtionality to update the DSC for
*                 tcp dynamic inf.
*
*  Parameters       :
*         p_dsc    : Pointer to rohc decompressor stream context block
*         p_dprm_update_blk: Pointer to rohc decompressor update block
*
*  Return Value     :void
*
* CAUTION/NOTE:
****************************************************************************/
STATIC rohc_void_t rohc_p6_update_dsc_tcp_dyn (
        rohc_dsc_tcp_t         *p_dsc    ,
        rohc_dprm_update_blk_t *p_dprm_update_blk)
{

    ROHC_ASSIGN_4_OCTS((p_dsc->dpib->tcp_dyn->seq_num),
            ((rohc_U8bit_t *)&p_dprm_update_blk->tcp->seq));

    p_dsc->sn_prev = ROHC_GET_4BYTES(p_dsc->dpib->tcp_dyn->seq_num);

    if ( NULL != p_dsc->dpib->tcp_dyn_acknum)
    {
        p_dsc->ack_recvd_prev = ROHC_GET_4BYTES
            (p_dsc->dpib->tcp_dyn_acknum->acknum);
    }

    if(p_dsc->dpib->tcp_dyn->ecnu_acksf_ackz_urpz_resr 
            & ROHC_DYN_ECN_USED_MASK)
    {
        p_dsc->ecn_used =   (p_dsc->dpib->tcp_dyn->ecnu_acksf_ackz_urpz_resr
                & ROHC_DYN_ECN_USED_MASK);
    }
    else
    {
        p_dsc->ecn_used = 0;
    }

    if(p_dsc->dpib->tcp_dyn->ecnu_acksf_ackz_urpz_resr
            & ROHC_DYN_ACK_STRIDE_MASK)
    {
        p_dsc->ack_stride_flag =   (p_dsc->dpib->tcp_dyn->
                ecnu_acksf_ackz_urpz_resr & 
                ROHC_DYN_ACK_STRIDE_MASK);
    }
    else
    {
        p_dsc->ack_stride_flag = 0;
        //p_dsc->ackstride = 0;
    }

    if(p_dsc->dpib->tcp_dyn->ecnu_acksf_ackz_urpz_resr 
            & ROHC_DYN_TCP_RES_MASK)
    {
        p_dsc->tcp_res_flag =   (p_dsc->dpib->tcp_dyn->
                ecnu_acksf_ackz_urpz_resr & 
                ROHC_DYN_TCP_RES_MASK);
    }
    else
    {
        p_dsc->tcp_res_flag = 0;
    }

    p_dprm_update_blk->tcp->flags = p_dsc->dpib->tcp_dyn->
        ecnf_urgf_ackf_pshf_rsf;

    p_dsc->msn_prev = ROHC_GET_2BYTES
        (p_dsc->dpib->tcp_dyn->msn);
    p_dsc->dpib->decoded_msn = ROHC_GET_2BYTES
        (p_dsc->dpib->tcp_dyn->msn);
}

