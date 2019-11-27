/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_dprm_update_p23.c,v 1.1.1.1.16.1 2010/11/03 07:07:56 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains function declaration for DPIB to DSC
 *                     updation for profile p23.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_dprm_update_p23.c,v $
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

/******************************************************************************
  Private Definitions
 *****************************************************************************/
#define ROHC_ENC_P23_INNERIPFLAG_EXT3FLAGP1_MASK 0x02
#define ROHC_ENC_P23_OUTERIPFLAG_EXT3FLAGP1_MASK 0x01
#define ROHC_ENC_P23_MODE_EXT3FLAGP1_MASK 0x18

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



/***************************************************************************
*  Function Name    : rohc_dprm_p2_base_update_pkt
*
*  Description      :
*                This function provides the funtionality to update the 
*                sn/ip_id/ip_id2/ in the DSC for received ROHC packet 
*
*  Parameters       :
*         p_entity : Pointer to rohc entity block
*         p_dsc    : Pointer to rohc decompressor stream context block
*
*  Return Value     :void
*
* CAUTION/NOTE:
****************************************************************************/

rohc_void_t rohc_dprm_p2_base_update_pkt  
                    ROHC_PARAMS((p_entity, p_dsc, p_dprm_update_blk),
                    rohc_entity_t *p_entity _AND_ rohc_dsc_udp_t *p_dsc _AND_
                    rohc_dprm_update_blk_t *p_dprm_update_blk)
{

    ROHC_ENTER_FN("rohc_dprm_p2_base_update_pkt");
    if(p_dsc->udp_cksum_present==ROHC_TRUE)
    {
        (p_dprm_update_blk)->udp->cksum =
                ROHC_HTONS((rohc_U16bit_t)p_dsc->dpib->field_val
                [ROHC_UDP_CKSUM_FID - ROHC_VAR_FID_BASE]);
    }
    if(p_dsc->bit_mask & ROHC_UPDATE_DSC_FOR_ED)
    {
        p_dsc->sn_prev_prev = p_dsc->sn_prev ;
        p_dsc->sn_prev =p_dsc->dpib->decoded_sn ;   
    }
    /* note marker bit need not to be updated in the context*/
    /* if value inner ip id is non ROHC_NULL) update the ctx*/
    if(p_dprm_update_blk->inner_ip4 != ROHC_NULL && (!p_dsc->rnd))
    {
        p_dprm_update_blk->inner_ip4->id=
                            ROHC_HTONS(p_dsc->dpib->decoded_ipid);
        if(p_dsc->nbo == ROHC_FALSE)                             
        {                                                        
            p_dprm_update_blk->inner_ip4->id = 
                        p_dprm_update_blk->inner_ip4->id >> 8 |
                            p_dprm_update_blk->inner_ip4->id << 8 ;
        }
        if(p_dsc->bit_mask & ROHC_UPDATE_DSC_FOR_ED)
        {
            p_dsc->pre_inner_ip_id = p_dsc->dpib->decoded_ipid;
        }
        if(p_dprm_update_blk->outer_ip4 != ROHC_NULL && (!p_dsc->rnd2))
        {
            p_dprm_update_blk->outer_ip4->id =
                        ROHC_HTONS( p_dsc->dpib->decoded_ipid);
            
            if(p_dsc->nbo2 == ROHC_FALSE)                             
            {
                p_dprm_update_blk->outer_ip4->id = 
                        p_dprm_update_blk->outer_ip4->id >> 8 |
                            p_dprm_update_blk->outer_ip4->id << 8 ;
            }   
            if(p_dsc->bit_mask & ROHC_UPDATE_DSC_FOR_ED)
            {
                p_dsc->pre_outer_ip_id = p_dsc->dpib->decoded_ipid;
            }
        }   
    }
    else if(p_dprm_update_blk->outer_ip4 != ROHC_NULL && (!p_dsc->rnd2)) 
    {
        p_dprm_update_blk->outer_ip4->id =
                ROHC_HTONS( p_dsc->dpib->decoded_ipid);
        p_dprm_update_blk->outer_ip4->id = 
                        p_dprm_update_blk->outer_ip4->id >> 8 |
                            p_dprm_update_blk->outer_ip4->id << 8 ;

        if(p_dsc->bit_mask & ROHC_UPDATE_DSC_FOR_ED)
        {
            p_dsc->pre_outer_ip_id = p_dsc->dpib->decoded_ipid;
        }   
    }
    if(p_dprm_update_blk->inner_ip4 != ROHC_NULL && (p_dsc->rnd))
    {   
         (p_dprm_update_blk->inner_ip4)->id = 
            ROHC_HTONS((rohc_U16bit_t)
                p_dsc->dpib->field_val[ROHC_INNERIP_ID_FID -ROHC_VAR_FID_BASE]);         
    }
    if(p_dprm_update_blk->outer_ip4 != ROHC_NULL && (p_dsc->rnd2))
    {
        (p_dprm_update_blk->outer_ip4)->id = ROHC_HTONS((rohc_U16bit_t)
                p_dsc->dpib->field_val[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE]);  
    }   

     if(p_dprm_update_blk->inner_ip4 != ROHC_NULL)
    {
        p_dprm_update_blk->inner_ip4->cksum = 0;
        p_dprm_update_blk->inner_ip4->cksum =
                (rohc_cal_ip_checksum(
                        (rohc_U8bit_t *)p_dprm_update_blk->inner_ip4,
                                            20));

    }
    if(p_dprm_update_blk->outer_ip4 != ROHC_NULL)
    {
        p_dprm_update_blk->outer_ip4->cksum = 0;
        p_dprm_update_blk->outer_ip4->cksum =
                (rohc_cal_ip_checksum(
                        (rohc_U8bit_t *)p_dprm_update_blk->outer_ip4,
                                            20));

    }

    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_dprm_p2_base_update_pkt");
}

/***************************************************************************
*  Function Name    : rohc_dprm_p2_ir_irdyn_update_pkt
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
rohc_void_t rohc_dprm_p2_ir_irdyn_update_pkt 
        ROHC_PARAMS((p_entity, p_dsc, p_dprm_update_blk),\
        rohc_entity_t          *p_entity _AND_ 
        rohc_dsc_udp_t         *p_dsc    _AND_
        rohc_dprm_update_blk_t *p_dprm_update_blk)
{
    rohc_U16bit_t u16_tmp;
    ROHC_ENTER_FN("rohc_dprm_p2_ir_irdyn_update_pkt");
    
    p_dsc->profile = p_dsc->dpib->profile;
    /* IP4 */
    if(p_dsc->dpib->inner_ip6_static != ROHC_NULL)
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
    if(p_dsc->dpib->inner_ip6_dyn != ROHC_NULL)
    {
        p_dprm_update_blk->inner_ip6->ip6_ctlun.ip6_un1.un1_flow |= 
                    ROHC_HTONL((p_dsc->dpib->inner_ip6_dyn->traffic_class<<20));
                     
        p_dprm_update_blk->inner_ip6->ip6_ctlun.ip6_un1.un1_hlim = 
                            (p_dsc->dpib->inner_ip6_dyn->hop_limit);     

    }
    if(p_dsc->dpib->outer_ip6_static != ROHC_NULL)
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
    if(p_dsc->dpib->outer_ip6_dyn != ROHC_NULL)
    {
        p_dprm_update_blk->outer_ip6->ip6_ctlun.ip6_un1.un1_flow |=
            /* SPR 4808 Fix Start */ 
            ROHC_HTONL(p_dsc->dpib->outer_ip6_dyn->traffic_class<<20); 
            /* SPR 4808 Fix End */ 
        p_dprm_update_blk->outer_ip6->ip6_ctlun.ip6_un1.un1_hlim =
                        (p_dsc->dpib->outer_ip6_dyn->hop_limit);     
    }
    /* IP4*/
    if(p_dsc->dpib->inner_ip4_static != ROHC_NULL)
    {
        p_dprm_update_blk->inner_ip4->vhl = (ROHC_IP4_STATIC_VER_MASK & 
                                p_dsc->dpib->inner_ip4_static->ver) | 
                                                    ROHC_IHL_VAL;
        p_dprm_update_blk->inner_ip4->proto  = p_dsc->dpib->inner_ip4_static->proto;
        
        ROHC_ASSIGN_4_OCTS((p_dsc->dpib->inner_ip4_static->src),
                    ((rohc_U8bit_t*)(&p_dprm_update_blk->inner_ip4->src)) );
        
        ROHC_ASSIGN_4_OCTS((p_dsc->dpib->inner_ip4_static->dst),
                    ((rohc_U8bit_t*)(&p_dprm_update_blk->inner_ip4->dst)) );
        
    }
    if(p_dsc->dpib->inner_ip4_dyn != ROHC_NULL)
    {
        p_dprm_update_blk->inner_ip4->tos = p_dsc->dpib->inner_ip4_dyn->tos; 
        p_dprm_update_blk->inner_ip4->ttl =p_dsc->dpib->inner_ip4_dyn->ttl; 
          
        p_dprm_update_blk->inner_ip4->off = 
             ROHC_HTONS((rohc_U16bit_t)((p_dsc->dpib->inner_ip4_dyn->flags &
                         ROHC_IP4_DYN_DF_MASK)<<7));
        
        p_dsc->rnd =(p_dsc->dpib->inner_ip4_dyn->flags & 
                     ROHC_IP4_DYN_RND_MASK)>>6; /* 6th bit is on */
                                                                                                    
        p_dsc->nbo = (p_dsc->dpib->inner_ip4_dyn->flags & 
                      ROHC_IP4_DYN_NBO_MASK)>>5; /* 5th bit is on*/
        
        p_dprm_update_blk->inner_ip4->id = 
                ROHC_GET_2BYTES((p_dsc->dpib->inner_ip4_dyn->id));
        p_dsc->pre_inner_ip_id = p_dsc->inner_ip4->id;
        
        }
    if(p_dsc->dpib->outer_ip4_static != ROHC_NULL)
    {
        p_dprm_update_blk->outer_ip4->vhl = (ROHC_IP4_STATIC_VER_MASK & 
                                p_dsc->dpib->outer_ip4_static->ver) |
                                ROHC_IHL_VAL;
        p_dprm_update_blk->outer_ip4->proto = 
                                    p_dsc->dpib->outer_ip4_static->proto;
        ROHC_ASSIGN_4_OCTS((p_dsc->dpib->outer_ip4_static->src),
                    ((rohc_U8bit_t*)(&p_dprm_update_blk->outer_ip4->src)) );
        
        ROHC_ASSIGN_4_OCTS((p_dsc->dpib->outer_ip4_static->dst),
                    ((rohc_U8bit_t*)(&p_dprm_update_blk->outer_ip4->dst)) );
    }   
    if(p_dsc->dpib->outer_ip4_dyn != ROHC_NULL)
    {
        p_dprm_update_blk->outer_ip4->tos = p_dsc->dpib->outer_ip4_dyn->tos; 
        p_dprm_update_blk->outer_ip4->ttl =p_dsc->dpib->outer_ip4_dyn->ttl; 
        
        p_dprm_update_blk->outer_ip4->off = 
               ROHC_HTONS((rohc_U16bit_t)((p_dsc->dpib->outer_ip4_dyn->flags &
                                 ROHC_IP4_DYN_DF_MASK)<<7));
        p_dsc->rnd2 =   (p_dsc->dpib->outer_ip4_dyn->flags & 
                        ROHC_IP4_DYN_RND_MASK)>>6; /* 6th bit is on */
                                                                                                
        p_dsc->nbo2 = (p_dsc->dpib->outer_ip4_dyn->flags &
                         ROHC_IP4_DYN_NBO_MASK)>>5; /* 5th bit is on*/
        
        ROHC_ASSIGN_2_OCTS((p_dsc->dpib->outer_ip4_dyn->id),
                        ((rohc_U8bit_t *)&(p_dprm_update_blk->outer_ip4->id)));
                        
        p_dprm_update_blk->outer_ip4->id = 
                ROHC_GET_2BYTES((p_dsc->dpib->outer_ip4_dyn->id));  
        p_dsc->pre_outer_ip_id = p_dsc->outer_ip4->id;
    }
       
    /* udp */
    if(p_dsc->dpib->udp_static != ROHC_NULL)
    {
        ROHC_ASSIGN_2_OCTS((p_dsc->dpib->udp_static->src),
                        ((rohc_U8bit_t *)&p_dprm_update_blk->udp->sport));
        ROHC_ASSIGN_2_OCTS((p_dsc->dpib->udp_static->dst),
                        ((rohc_U8bit_t *)&p_dprm_update_blk->udp->dport));
            
    }
    if(p_dsc->dpib->udp_dyn  != ROHC_NULL)
    {
        ROHC_ASSIGN_2_OCTS((p_dsc->dpib->udp_dyn->cksum),
                        ((rohc_U8bit_t *)&p_dprm_update_blk->udp->cksum));
        p_dsc->sn_prev_prev = p_dsc->sn_prev;
        /*referring to section  5.11.1 of ROHC-3095*/
        p_dsc->sn_prev = 
            ROHC_GET_2BYTES(((rohc_U8bit_t *)p_dsc->dpib->udp_dyn+2));
        if(p_dsc->udp->cksum != 0)
        {
            p_dsc->udp_cksum_present = ROHC_TRUE;   
        }
    }
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
       
    ROHC_EXIT_FN("rohc_dprm_p2_ir_irdyn_update_pkt");
}

/***************************************************************************
*  Function Name    : rohc_dprm_p2_base_ext3_update_pkt 
*
*  Description      :
*                This function provides the funtionality to update the DSC for
*                for received EXT-3.
*
*  Parameters       :
*         p_entity : Pointer to rohc entity block
*         p_dsc    : Pointer to rohc decompressor stream context block
*
*  Return Value     :void
*
* CAUTION/NOTE:
****************************************************************************/



rohc_void_t rohc_dprm_p2_base_ext3_update_pkt  
                ROHC_PARAMS((p_entity, p_dsc, p_dprm_update_blk),\
                             rohc_entity_t *p_entity _AND_ 
                             rohc_dsc_udp_t *p_dsc _AND_
                             rohc_dprm_update_blk_t *p_dprm_update_blk)
{
    ROHC_ENTER_FN("rohc_dprm_p2_base_ext3_update_pkt");
    /* At least one inner header is always present for profile p1 */
    /* That coudl be ip4 or ip6 */
    if(p_dsc->dpib->field_val[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE] &
        ROHC_ENC_P23_INNERIPFLAG_EXT3FLAGP1_MASK)
    {   
        /* inner ip present 4/6 */
        /* ip6 */
      if(p_dprm_update_blk->inner_ip6 != ROHC_NULL)
      {
        if(p_dsc->dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] & 
                ROHC_ENC_TOS_INNERIPFLAGP1_MASK )
        {
            p_dprm_update_blk->inner_ip6->ip6_ctlun.ip6_un1.un1_flow |= 
                (p_dsc->dpib->field_val[ROHC_INNERIP6_TCLASS_FID-ROHC_VAR_FID_BASE] & \
                    ROHC_IP6_TRAFFICCLASS_MASK);
        } 
        if(p_dsc->dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_TTL_INNERIPFLAGP1_MASK)
        {
            p_dprm_update_blk->inner_ip6->ip6_ctlun.ip6_un1.un1_hlim = 
                p_dsc->dpib->field_val[ROHC_INNERIP6_HLIM_FID-ROHC_VAR_FID_BASE];
        }
        if(p_dsc->dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_PROTO_INNERIPFLAGP1_MASK)
        {
            p_dprm_update_blk->inner_ip6->ip6_ctlun.ip6_un1.un1_nxt = 
                p_dsc->dpib->field_val[ROHC_INNERIP6_NHEADER_FID-ROHC_VAR_FID_BASE]   ;
        }
      }
      else
      { 
        /* ip4 */   
        if(p_dsc->dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
            ROHC_ENC_TOS_INNERIPFLAGP1_MASK)
        {
            p_dprm_update_blk->inner_ip4->tos |= 
                (p_dsc->dpib->field_val[ROHC_INNERIP4_TOS_FID-ROHC_VAR_FID_BASE] );
                /*& \
                    ROHC_IP4_TOS_MASK);*/
        } 
        
        if(p_dsc->dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
            ROHC_ENC_TTL_INNERIPFLAGP1_MASK )
        {
            p_dprm_update_blk->inner_ip4->ttl = 
                p_dsc->dpib->field_val[ROHC_INNERIP4_TTL_FID-ROHC_VAR_FID_BASE]   ;
        }
        if(p_dsc->dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
            ROHC_ENC_PROTO_INNERIPFLAGP1_MASK)
        {
            p_dprm_update_blk->inner_ip4->proto = 
                p_dsc->dpib->field_val[ROHC_INNERIP4_PROTO_FID-ROHC_VAR_FID_BASE];
        }
        p_dprm_update_blk->inner_ip4->off |= 
                    (((p_dsc->dpib->field_val[ROHC_INNERIP_DF_FID-ROHC_VAR_FID_BASE]<<13)) &\
                                         ROHC_IP4_DF);
                
        p_dsc->rnd = p_dsc->dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                    ROHC_ENC_RND_INNERIPFLAGP1_MASK ;
                    
        p_dsc->nbo = p_dsc->dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                    ROHC_ENC_NBO_INNERIPFLAGP1_MASK ;                   
        
      } /* inner ip id is updated with the decoded value */
    }   
    /* Either of outer ip4 or ip4 would be present OR  */
    /* none will be present */
    /* outer ip present */
    if(p_dsc->dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
        ROHC_ENC_P23_OUTERIPFLAG_EXT3FLAGP1_MASK )
    {   
        /* ip-6*/
      if(p_dprm_update_blk->outer_ip6 != ROHC_NULL)
      {
        if(p_dsc->dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_TOS_OUTERIPFLAGP1_MASK)
        {
            p_dprm_update_blk->outer_ip6->ip6_ctlun.ip6_un1.un1_flow |= 
                (p_dsc->dpib->field_val[ROHC_OUTERIP6_TCLASS_FID-ROHC_VAR_FID_BASE] & \
                    ROHC_IP6_TRAFFICCLASS_MASK);
        } 
        if(p_dsc->dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_TTL_OUTERIPFLAGP1_MASK)
        {
            p_dprm_update_blk->outer_ip6->ip6_ctlun.ip6_un1.un1_hlim = 
                p_dsc->dpib->field_val[ROHC_OUTERIP6_HLIM_FID-ROHC_VAR_FID_BASE];
        }
        if(p_dsc->dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_PROTO_OUTERIPFLAGP1_MASK)
        {
            p_dprm_update_blk->outer_ip6->ip6_ctlun.ip6_un1.un1_nxt = 
                p_dsc->dpib->field_val[ROHC_OUTERIP6_NHEADER_FID-ROHC_VAR_FID_BASE];
        }
      }
      else if(p_dprm_update_blk->outer_ip4 != ROHC_NULL)
      {
        /*ip-4 */
        if(p_dsc->dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_TOS_OUTERIPFLAGP1_MASK)
        {
            p_dprm_update_blk->outer_ip4->tos |= 
                (p_dsc->dpib->field_val[ROHC_OUTERIP4_TOS_FID-ROHC_VAR_FID_BASE]);
        } 
        
        if(p_dsc->dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_TTL_OUTERIPFLAGP1_MASK)
        {
            p_dprm_update_blk->outer_ip4->ttl = 
                p_dsc->dpib->field_val[ROHC_OUTERIP4_TTL_FID-ROHC_VAR_FID_BASE]   ;
        }
        if(p_dsc->dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_PROTO_OUTERIPFLAGP1_MASK)
        {
            p_dprm_update_blk->outer_ip4->proto = 
                p_dsc->dpib->field_val[ROHC_OUTERIP4_PROTO_FID-ROHC_VAR_FID_BASE];
        }
        p_dprm_update_blk->outer_ip4->off |= 
                    (((p_dsc->dpib->field_val[ROHC_OUTERIP_DF_FID-ROHC_VAR_FID_BASE])<<13) &\
                                         ROHC_IP4_DF);
        
        p_dsc->rnd = p_dsc->dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                    ROHC_ENC_RND_OUTERIPFLAGP1_MASK ;
                    
        p_dsc->nbo = p_dsc->dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                    ROHC_ENC_NBO_INNERIPFLAGP1_MASK ;
                    
        p_dprm_update_blk->outer_ip4->id =p_dsc->dpib->field_val[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE];
      }
    }
/* SRPR #1436 fix */      

    rohc_dprm_p2_base_update_pkt(p_entity, p_dsc,p_dprm_update_blk);
 
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_dprm_p2_base_ext3_update_pkt");
}
