/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_dprm_update_p1.c,v 1.3.36.3 2010/12/02 08:59:31 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains function declaration for DPIB to DSC
 *                     updation for profile p1.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_dprm_update_p1.c,v $
 * Revision 1.3.36.3  2010/12/02 08:59:31  gur22059
 * Conflicting profile 1 functions were renamed acc to new Naming Convention
 *
 * Revision 1.3.36.2  2010/11/24 03:45:41  gur22059
 * Removal of conditional checks w.r.t profile1
 *
 * Revision 1.3.36.1  2010/11/09 05:26:20  gur22059
 * Segregation of code w.r.t profile 1
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




/***************************************************************************
*  Function Name    :  rohc_dprm_p1_type_base_update_pkt                               
*
*  Description      :
*                This function provides the funtionality to update the 
*                sn/ip_id/ts in the DSC for received ROHC packet .
*
*  Parameters       :
*         p_entity : Pointer to rohc entity block
*         p_dsc    : Pointer to rohc decompressor stream context block
*
*  Return Value     :void
*
* CAUTION/NOTE:
****************************************************************************/
rohc_void_t rohc_dprm_p1_type_base_update_pkt  ROHC_PARAMS((p_entity, p_dsc,
                                                       p_dprm_update_blk),

                             rohc_entity_t          *p_entity   _AND_ 
                             rohc_dsc_rtp_t         *p_dsc      _AND_ 
                             rohc_dprm_update_blk_t *p_dprm_update_blk)
{
    ROHC_ENTER_FN("rohc_dprm_p1_type_base_update_pkt");
    ((rohc_rtp_t *)p_dprm_update_blk->rtp)->sn=
                            ROHC_HTONS((rohc_U16bit_t)p_dsc->dpib->decoded_sn);
    ((rohc_rtp_t *)p_dprm_update_blk->rtp)->ts = 
                                ROHC_HTONL(p_dsc->dpib->decoded_ts);
    /* Start SPR 6116 Fix */
    if (p_dsc->dpib->field_val[ROHC_RTP_M_FID-ROHC_VAR_FID_BASE])
    {
        ((rohc_rtp_t *) p_dprm_update_blk->rtp)->m_pt |=  (ROHC_M_MASK) &
            ((p_dsc->dpib->field_val[ROHC_RTP_M_FID-ROHC_VAR_FID_BASE])<<7);
    }
    /* End SPR 6116 Fix */
    /* Start SPR 6196 Fix */
    else
    {
        /* Reset M bit for base header for non 0 M FID*/
        ((rohc_rtp_t *) p_dprm_update_blk->rtp)->m_pt &= (~ROHC_M_MASK); 
    }
    /* End SPR 6196 Fix */

    if(p_dsc->bit_mask & ROHC_UPDATE_DSC_FOR_ED)
    {
        p_dsc->sn_prev_prev = p_dsc->sn_prev ;
        p_dsc->pre_ts = p_dsc->dpib->decoded_ts;
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
    }
    else if(p_dprm_update_blk->outer_ip4 != ROHC_NULL && (!p_dsc->rnd2))
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
    if(p_dprm_update_blk->inner_ip4 != ROHC_NULL && (p_dsc->rnd))
    {   
         (p_dprm_update_blk->inner_ip4)->id = 
            ROHC_HTONS((rohc_U16bit_t)
                p_dsc->dpib->field_val[ROHC_INNERIP_ID_FID -ROHC_VAR_FID_BASE]);

        if(p_dsc->bit_mask & ROHC_UPDATE_DSC_FOR_ED)
        {
            p_dsc->pre_inner_ip_id = p_dsc->dpib->field_val[ROHC_INNERIP_ID_FID-
                                ROHC_VAR_FID_BASE];
        }   
    }
    if(p_dprm_update_blk->outer_ip4 != ROHC_NULL && (p_dsc->rnd2))
    {
        (p_dprm_update_blk->outer_ip4)->id = ROHC_HTONS((rohc_U16bit_t)
                p_dsc->dpib->field_val[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE]);  
        if(p_dsc->bit_mask & ROHC_UPDATE_DSC_FOR_ED)
        {
            p_dsc->pre_outer_ip_id = p_dsc->dpib->field_val[ROHC_OUTERIP_ID_FID-
                                ROHC_VAR_FID_BASE]; 
        }   
    }   
    if(p_dsc->udp_cksum_present==ROHC_TRUE)
    {
        (p_dprm_update_blk)->udp->cksum =
           ROHC_HTONS((rohc_U16bit_t)p_dsc->dpib->field_val[ROHC_UDP_CKSUM_FID -
                                        ROHC_VAR_FID_BASE]);
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

    ROHC_EXIT_FN("rohc_dprm_p1_type_base_update_pkt");
}

/***************************************************************************
*  Function Name    : rohc_dprm_p1_type_base_ext3_update_pkt
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
rohc_void_t rohc_dprm_p1_type_base_ext3_update_pkt  ROHC_PARAMS((p_entity, p_dsc,
                                                            p_dprm_update_blk),
                             rohc_entity_t  *p_entity   _AND_ 
                             rohc_dsc_rtp_t *p_dsc      _AND_ 
                             rohc_dprm_update_blk_t *p_dprm_update_blk)

{
    ROHC_ENTER_FN("rohc_dprm_p1_type_base_ext3_update_pkt");
    /* At least one inner header is always present for profile p1 */
    /* That coudl be ip4 or ip6 */
    if(p_dsc->dpib->field_val[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE] &
        ROHC_ENC_INNERIPFLAG_EXT3FLAGP1_MASK)
    {   
        /* inner ip present 4/6 */
        /* ip6 */
      if(p_dprm_update_blk->inner_ip6 != ROHC_NULL)
      {
        if(p_dsc->dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] & 
                ROHC_ENC_TOS_INNERIPFLAGP1_MASK )
        {
            p_dprm_update_blk->inner_ip6->ip6_ctlun.ip6_un1.un1_flow |= 
          (p_dsc->dpib->field_val[ROHC_INNERIP6_TCLASS_FID-ROHC_VAR_FID_BASE] & 
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
            p_dsc->dpib->field_val[ROHC_INNERIP6_NHEADER_FID-ROHC_VAR_FID_BASE];
        }
      }
      else
      { 
        /* ip4 */   
        if(p_dsc->dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
            ROHC_ENC_TOS_INNERIPFLAGP1_MASK)
        {
            p_dprm_update_blk->inner_ip4->tos |= 
            (p_dsc->dpib->field_val[ROHC_INNERIP4_TOS_FID-ROHC_VAR_FID_BASE]);
                /*& 
                    ROHC_IP4_TOS_MASK);*/
        } 
        
        if(p_dsc->dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
            ROHC_ENC_TTL_INNERIPFLAGP1_MASK )
        {
            p_dprm_update_blk->inner_ip4->ttl = 
                p_dsc->dpib->field_val[ROHC_INNERIP4_TTL_FID-ROHC_VAR_FID_BASE];
        }
        if(p_dsc->dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
            ROHC_ENC_PROTO_INNERIPFLAGP1_MASK)
        {
            p_dprm_update_blk->inner_ip4->proto = 
             p_dsc->dpib->field_val[ROHC_INNERIP4_PROTO_FID-ROHC_VAR_FID_BASE];
        }

        
        
        p_dprm_update_blk->inner_ip4->off |= 
                    ((p_dsc->dpib->field_val[ROHC_INNERIP_DF_FID-
                     ROHC_VAR_FID_BASE])<<13) & ROHC_IP4_DF;
        p_dsc->rnd =
            (p_dsc->dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                    ROHC_ENC_RND_INNERIPFLAGP1_MASK)>>1 ;
                    
        p_dsc->nbo =
            (p_dsc->dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                    ROHC_ENC_NBO_INNERIPFLAGP1_MASK)>>2 ;                   
        
      } /* inner ip id is updated with the decoded value */
    }   
    /* Either of outer ip4 or ip4 would be present OR  */
    /* none will be present */
    /* outer ip present */
    if(p_dsc->dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
        ROHC_ENC_IP2_INNERIPFLAGP1_MASK )
    {   
        /* ip-6*/
      if(p_dprm_update_blk->outer_ip6 != ROHC_NULL)
      {
        if(p_dsc->dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_TOS_OUTERIPFLAGP1_MASK)
        {
            p_dprm_update_blk->outer_ip6->ip6_ctlun.ip6_un1.un1_flow |= 
          (p_dsc->dpib->field_val[ROHC_OUTERIP6_TCLASS_FID-ROHC_VAR_FID_BASE] & 
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
                p_dsc->dpib->field_val[ROHC_OUTERIP4_TTL_FID-ROHC_VAR_FID_BASE];
        }
        if(p_dsc->dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_PROTO_OUTERIPFLAGP1_MASK)
        {
            p_dprm_update_blk->outer_ip4->proto = 
              p_dsc->dpib->field_val[ROHC_OUTERIP4_PROTO_FID-ROHC_VAR_FID_BASE];
        }
        if(p_dsc->dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_I_OUTERIPFLAGP1_MASK)
        {
            p_dprm_update_blk->outer_ip4->id = 
                p_dsc->dpib->field_val[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE];
        }
        p_dprm_update_blk->outer_ip4->off |= 
                    (((p_dsc->dpib->field_val[ROHC_OUTERIP_DF_FID-
                       ROHC_VAR_FID_BASE])<<13) &
                                         ROHC_IP4_DF);
        
        p_dsc->rnd2 = (p_dsc->dpib->field_val[ROHC_OUTERIP_FLAGS_FID-
                      ROHC_VAR_FID_BASE] & ROHC_ENC_RND_OUTERIPFLAGP1_MASK)>>1;
               
        /* SPR 4808 Fix Start */     
        p_dsc->nbo2 = (p_dsc->dpib->field_val[ROHC_OUTERIP_FLAGS_FID-
                       ROHC_VAR_FID_BASE] &
        //            ROHC_ENC_NBO_INNERIPFLAGP1_MASK)>>4 /* >>5 (Before) */ ;
                    /* Coverity Fix 10129*/
                    ROHC_ENC_NBO_INNERIPFLAGP1_MASK)>>2 ;
                    
        /* SPR 4808 Fix End */     
        p_dprm_update_blk->outer_ip4->id =
                 p_dsc->dpib->field_val[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE];
      }
    }
    /* update for RTP  */
    if(p_dsc->dpib->field_val[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE] &
                    ROHC_ENC_RTPFLAG_EXT3FLAGP1_MASK )
    {
            
            /*p_dsc->d_mode  = 
                p_dsc->dpib->field_val[ROHC_MODE_FID-ROHC_VAR_FID_BASE];
            I don't know what is the use of mode received in the 
            compressed packet   
            */  
            if(p_dsc->dpib->field_val[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE]
                & ROHC_ENC_R_PT_RTPFLAGP1_MASK)
            {
                p_dprm_update_blk->rtp->v_p_x_cc |= (ROHC_PAD_MASK & 
                    p_dsc->dpib->field_val[ROHC_RTP_R_P_FID-ROHC_VAR_FID_BASE]);
                p_dprm_update_blk->rtp->m_pt |= (ROHC_PT_MASK &
                    p_dsc->dpib->field_val[ROHC_RTP_PT_FID-ROHC_VAR_FID_BASE]);   
            }   
            if(p_dsc->dpib->field_val[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE]
                & ROHC_ENC_TSS_RTPFLAGP1_MASK)
            {
               p_dsc->tss = 
               p_dsc->dpib->field_val[ROHC_RTP_TS_STRIDE_FID-ROHC_VAR_FID_BASE];
            }
            if(p_dsc->dpib->field_val[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE]
                & ROHC_ENC_TIS_RTPFLAGP1_MASK)
            {
               p_dsc->tis = 
             p_dsc->dpib->field_val[ROHC_RTP_TIME_STRIDE_FID-ROHC_VAR_FID_BASE];
            }          
        
            p_dprm_update_blk->rtp->v_p_x_cc |= ( ROHC_X_MASK &
                    p_dsc->dpib->field_val[ROHC_RTP_X_FID-ROHC_VAR_FID_BASE]);
            
            /* Start SPR 6116 Fix
             * It is required to update the M bit whenever there is
             * change in value as it is semistatic value
             */
            if (p_dsc->dpib->field_val[ROHC_RTP_M_FID-ROHC_VAR_FID_BASE])
            {
                p_dprm_update_blk->rtp->m_pt |=  (ROHC_M_MASK) &
                    ((p_dsc->dpib->field_val[ROHC_RTP_M_FID-ROHC_VAR_FID_BASE])<<7);
            }
            else
            {
                p_dprm_update_blk->rtp->m_pt  &= (~ROHC_M_MASK);
            }
            /* End SPR 6116 Fix */
    }               
/*SPR #1436 Fix */
    rohc_dprm_p1_type_base_update_pkt(p_entity, p_dsc,p_dprm_update_blk);
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
            ROHC_GET_DSC_CID(p_dsc),0,0,0, __func__,"");
    /* +- SPR 17777 */
    ROHC_EXIT_FN("rohc_dprm_p1_type_base_ext3_update_pkt");
}

/***************************************************************************
*  Function Name    : rohc_dprm_p1_type_ir_irdyn_update_pkt
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
rohc_void_t rohc_dprm_p1_type_ir_irdyn_update_pkt  ROHC_PARAMS((p_entity, p_dsc,
                                                          p_dprm_update_blk),
                             rohc_entity_t  *p_entity _AND_ 
                             rohc_dsc_rtp_t *p_dsc    _AND_ 
                             rohc_dprm_update_blk_t  *p_dprm_update_blk)
{
    rohc_U16bit_t u16_tmp;
    rohc_U8bit_t sdlv_len = 0;
    ROHC_ENTER_FN("rohc_dprm_p1_type_ir_irdyn_update_pkt");
    p_dsc->profile = p_dsc->dpib->profile;
            
    /* IP4 */
    if(p_dsc->dpib->inner_ip6_static != ROHC_NULL)
    {
        /* for version, flowlabel */    
    u16_tmp = ROHC_GET_2BYTES(p_dsc->dpib->inner_ip6_static->flowlabel);
        p_dprm_update_blk->inner_ip6->ip6_ctlun.ip6_un1.un1_flow |=
                ROHC_HTONL( 0x60000000 |
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
                     
        p_dsc->inner_ip6->ip6_ctlun.ip6_un1.un1_hlim = 
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
            ROHC_HTONL((p_dsc->dpib->outer_ip6_dyn->traffic_class <<20));
    /* SPR 4808 Fix End */
        p_dsc->outer_ip6->ip6_ctlun.ip6_un1.un1_hlim = 
                    (p_dsc->dpib->outer_ip6_dyn->hop_limit);     
    }
    /* IP4*/
    if(p_dsc->dpib->inner_ip4_static != ROHC_NULL)
    {
        p_dsc->inner_ip4->vhl = (ROHC_IP4_STATIC_VER_MASK & 
                                p_dsc->dpib->inner_ip4_static->ver)
                                | ROHC_IHL_VAL;
        p_dsc->inner_ip4->proto  = p_dsc->dpib->inner_ip4_static->proto;
        
        ROHC_ASSIGN_4_OCTS((p_dsc->dpib->inner_ip4_static->src),
                    ((rohc_U8bit_t*)(&p_dsc->inner_ip4->src)) );
        
        ROHC_ASSIGN_4_OCTS((p_dsc->dpib->inner_ip4_static->dst),
                    ((rohc_U8bit_t*)(&p_dsc->inner_ip4->dst)) );
        
    }
    if(p_dsc->dpib->inner_ip4_dyn != ROHC_NULL)
    {
        p_dsc->inner_ip4->tos = p_dsc->dpib->inner_ip4_dyn->tos; 
        p_dsc->inner_ip4->ttl =p_dsc->dpib->inner_ip4_dyn->ttl; 
          
        p_dsc->inner_ip4->off = 
             ROHC_HTONS((rohc_U16bit_t)((p_dsc->dpib->inner_ip4_dyn->flags & 
                          ROHC_IP4_DYN_DF_MASK)<<7));
        
        p_dsc->rnd =
            (p_dsc->dpib->inner_ip4_dyn->flags & ROHC_IP4_DYN_RND_MASK)>>6; 
        /* 6th bit is on */
        p_dsc->nbo =
        (p_dsc->dpib->inner_ip4_dyn->flags & ROHC_IP4_DYN_NBO_MASK)>>5; 
        /* 5th bit is on*/
        
        p_dsc->inner_ip4->id = 
                ROHC_GET_2BYTES((p_dsc->dpib->inner_ip4_dyn->id));

        p_dsc->pre_inner_ip_id = p_dsc->inner_ip4->id;
    }
    if(p_dsc->dpib->outer_ip4_static != ROHC_NULL)
    {
        p_dsc->outer_ip4->vhl = (ROHC_IP4_STATIC_VER_MASK & 
                                p_dsc->dpib->outer_ip4_static->ver)
                                            | ROHC_IHL_VAL;
        p_dsc->outer_ip4->proto  = p_dsc->dpib->outer_ip4_static->proto;
        ROHC_ASSIGN_4_OCTS((p_dsc->dpib->outer_ip4_static->src),
                    ((rohc_U8bit_t*)(&p_dsc->outer_ip4->src)) );
        
        ROHC_ASSIGN_4_OCTS((p_dsc->dpib->outer_ip4_static->dst),
                    ((rohc_U8bit_t*)(&p_dsc->outer_ip4->dst)) );
    }   
    if(p_dsc->dpib->outer_ip4_dyn != ROHC_NULL)
    {
        p_dsc->outer_ip4->tos = p_dsc->dpib->outer_ip4_dyn->tos; 
        p_dsc->outer_ip4->ttl =p_dsc->dpib->outer_ip4_dyn->ttl; 
        
        p_dsc->outer_ip4->off = ROHC_HTONS((rohc_U16bit_t)
                    ((p_dsc->dpib->outer_ip4_dyn->flags & 
                                ROHC_IP4_DYN_DF_MASK) <<7));
        p_dsc->rnd2 =   (p_dsc->dpib->outer_ip4_dyn->flags & 
                        ROHC_IP4_DYN_RND_MASK)>>6; /* 6th bit is on */
                                                                                                
        p_dsc->nbo2 = (p_dsc->dpib->outer_ip4_dyn->flags &
                         ROHC_IP4_DYN_NBO_MASK)>>5; /* 5th bit is on*/
        
        p_dsc->outer_ip4->id = 
                ROHC_GET_2BYTES((p_dsc->dpib->outer_ip4_dyn->id));
        p_dsc->pre_outer_ip_id = p_dsc->outer_ip4->id;
            
    }
    /*RTP*/
        
    if(p_dsc->dpib->rtp_static != ROHC_NULL)
    {
        ROHC_ASSIGN_4_OCTS((p_dsc->dpib->rtp_static->ssrc),
                    ((rohc_U8bit_t *)&(p_dsc->rtp->ssrc)));
        
    }
    if(p_dsc->dpib->rtp_head_dyn != ROHC_NULL)
    {
        /* RX is not RTP X bit */
        /* Fix for RX Bit */
        p_dsc->rtp->v_p_x_cc &=  ROHC_DYN_RX_MASK;
        p_dsc->rtp->v_p_x_cc |= (p_dsc->dpib->rtp_head_dyn->ver_p_rx_cc &
                    (~ ROHC_DYN_RX_MASK));
        p_dsc->rtp->m_pt = p_dsc->dpib->rtp_head_dyn->m_pt;
        
        p_dsc->rtp->sn = ROHC_GET_2BYTES(p_dsc->dpib->rtp_head_dyn->sn);
        
        p_dsc->rtp->ts = ROHC_GET_4BYTES(p_dsc->dpib->rtp_head_dyn->ts);
        
        
        /* updating cadidate set */
        p_dsc->pre_ts=p_dsc->rtp->ts;
        p_dsc->sn_prev_prev = p_dsc->sn_prev;
        p_dsc->sn_prev = p_dsc->rtp->sn ;
    }
    
    if(p_dsc->dpib->rtp_tail_dyn != ROHC_NULL)
    {
        /* Fix for RX bit Mask Start */
        p_dsc->rtp->v_p_x_cc &=  (~(ROHC_DYN_RX_MASK));
        /* Fix for RX bit Mask End */
        p_dsc->rtp->v_p_x_cc |= (p_dsc->dpib->rtp_tail_dyn->x_mode_tis_tss
                                 &(ROHC_DYN_X_MASK));
        if(p_dsc->dpib->rtp_tail_dyn->x_mode_tis_tss  & ROHC_DYN_TSS_MASK)
        {
            
               p_dsc->ts_stride = rohc_d_sdvalue_decode(
                        (((rohc_U8bit_t *)p_dsc->dpib->rtp_tail_dyn)+1),
                        &sdlv_len); 
		p_dsc->tss = ROHC_TRUE;
        }
        if(p_dsc->dpib->rtp_tail_dyn->x_mode_tis_tss  & ROHC_DYN_TIS_MASK)
        {
            p_dsc->time_stride = rohc_d_sdvalue_decode(
                        (((rohc_U8bit_t *)p_dsc->dpib->rtp_tail_dyn)+ 1 + 
                         sdlv_len), &sdlv_len); 
		p_dsc->tis = ROHC_TRUE;
        }                                   
    }   
    
    /* udp */
    if(p_dsc->dpib->udp_static != ROHC_NULL)
    {
        ROHC_ASSIGN_2_OCTS((p_dsc->dpib->udp_static->src),
                        ((rohc_U8bit_t *)&p_dsc->udp->sport));
        ROHC_ASSIGN_2_OCTS((p_dsc->dpib->udp_static->dst),
                        ((rohc_U8bit_t *)&p_dsc->udp->dport));
        
    }
    if(p_dsc->dpib->udp_dyn  != ROHC_NULL)
    {
        ROHC_ASSIGN_2_OCTS((p_dsc->dpib->udp_dyn->cksum),
                        ((rohc_U8bit_t *)&p_dsc->udp->cksum));
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
    ROHC_EXIT_FN("rohc_dprm_p1_type_ir_irdyn_update_pkt");
}
