/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  File Description : This file contains the function definition for updating
 *                     the context (CSC) from DPIB for profile p6(TCP/IP).
 *
 ******************************************************************************
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "rohc_cprm_proto.h"

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
**      FUNCTION:
**              rohc_cprm_p1_update_csc_tcp
******************************************************************************
**
**      DESCRIPTION:
**              This funtion updates from CPIB to csc for tcp 
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**              delta_field[IN]   Computed Delta field 
**              en_sn[IN]         Encoded value of sn
**              st_update_flag[IN]   Specifies whether to update the static 
**                                context or not.
**              
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/
    /* +- SPR 17777 */
rohc_void_t rohc_cprm_p6_update_csc_tcp ROHC_PARAMS(
            ( p_csc, en_sn, en_ack, en_msn),
            rohc_csc_tcp_t       *p_csc    _AND_
            rohc_encoded_value_t *en_sn    _AND_
            rohc_encoded_value_t *en_ack   _AND_ 
            rohc_encoded_value_t *en_msn   
            )
{
 
    ROHC_ENTER_FN("rohc_cprm_p6_update_csc_tcp");
    /* TCP static param */
    p_csc->field_value[ROHC_TCP_SRC_PORT_FID-ROHC_VAR_FID_BASE] = 
            ROHC_NTOHS((((rohc_tcp_t*)p_csc->p_cpib->tcp)->sport));
    p_csc->field_value[ROHC_TCP_DST_PORT_FID-ROHC_VAR_FID_BASE] = 
            ROHC_NTOHS((((rohc_tcp_t*)p_csc->p_cpib->tcp)->dport));

    /* TCP Dyn param bitmask based updation */
    p_csc->field_value[ROHC_SN_FID-ROHC_VAR_FID_BASE] = en_sn->value;    
    p_csc->field_len[ROHC_SN_FID-ROHC_VAR_FID_BASE] = en_sn->bits;
    p_csc->field_value[ROHC_TCP_ACKNO_FID-ROHC_VAR_FID_BASE] = en_ack->value;   
    p_csc->field_len[ROHC_TCP_ACKNO_FID-ROHC_VAR_FID_BASE] = en_ack->bits;
    p_csc->field_value[ROHC_TCP_RES_FLAG_FID-ROHC_VAR_FID_BASE] = 
        ((rohc_tcp_t*)p_csc->p_cpib->tcp)->offset & ROHC_TCP_RES_MASK;    
    p_csc->field_value[ROHC_TCP_MSN_FID-ROHC_VAR_FID_BASE] = en_msn->value;   
    p_csc->field_len[ROHC_TCP_MSN_FID-ROHC_VAR_FID_BASE] = en_msn->bits;
    
    
    if((3 == p_csc->no_of_pkt_recvd) &&
            (p_csc->field_value[ROHC_TCP_ACK_STRIDE_FID-ROHC_VAR_FID_BASE]==
             p_csc->ack_stride))
    {
        /* ack stride is constant so now we can communicate ack stride to 
         * decompressor from 3rd IR pkt onwards*/
        p_csc->field_value[ROHC_TCP_ACK_STRIDE_FLAG_FID-ROHC_VAR_FID_BASE] = 1;
    }

    else if (p_csc->no_of_pkt_recvd > 3)
    {
        if(p_csc->c_state != ROHC_C_ST_IR)
        {
            /* case of IR-DYN pkt */
            if(p_csc->ack_stride_change)
            {
                /* send ack_stride */
                p_csc->field_value[ROHC_TCP_ACK_STRIDE_FLAG_FID-ROHC_VAR_FID_BASE] = 1;
            }
        }
        else
        {
            if(p_csc->field_value[ROHC_TCP_ACK_STRIDE_FID-ROHC_VAR_FID_BASE] ==
                    p_csc->ack_stride)
            {
                p_csc->field_value[ROHC_TCP_ACK_STRIDE_FLAG_FID-ROHC_VAR_FID_BASE] = 0;
            }
            else
            {
                p_csc->field_value[ROHC_TCP_ACK_STRIDE_FLAG_FID-ROHC_VAR_FID_BASE] = 1;
                p_csc->field_value[ROHC_TCP_ACK_STRIDE_FID-ROHC_VAR_FID_BASE] = p_csc->ack_stride;
            }
        }
    }
    else
    {
        /* not to send ack stride in initial 2 IR pkts */
        p_csc->field_value[ROHC_TCP_ACK_STRIDE_FID-ROHC_VAR_FID_BASE] = p_csc->ack_stride;
        p_csc->field_value[ROHC_TCP_ACK_STRIDE_FLAG_FID-ROHC_VAR_FID_BASE] = 0;
    }
    
    if(p_csc->field_value[ROHC_TCP_ACKNO_FID-ROHC_VAR_FID_BASE])
    {
        p_csc->field_value[ROHC_TCP_ACK_ZERO_FID-ROHC_VAR_FID_BASE] = 0;
    }
    else
    {
        p_csc->field_value[ROHC_TCP_ACK_ZERO_FID-ROHC_VAR_FID_BASE] = 1;
    }
    p_csc->field_value[ROHC_TCP_WINDOW_FID-ROHC_VAR_FID_BASE] = 
              ROHC_NTOHS((((rohc_tcp_t*)p_csc->p_cpib->tcp)->window));
    p_csc->field_value[ROHC_TCP_CKSUM_FID-ROHC_VAR_FID_BASE] = 
              ROHC_NTOHS((((rohc_tcp_t*)p_csc->p_cpib->tcp)->checksum));
    p_csc->field_value[ROHC_TCP_URG_PTR_FID-ROHC_VAR_FID_BASE] = 
              ROHC_NTOHS((((rohc_tcp_t*)p_csc->p_cpib->tcp)->urg_ptr));
    
    if(p_csc->field_value[ROHC_TCP_URG_PTR_FID-ROHC_VAR_FID_BASE])
    {
        p_csc->field_value[ROHC_TCP_URP_ZERO_FID-ROHC_VAR_FID_BASE] = 0;
    }
    else
    {
        p_csc->field_value[ROHC_TCP_URP_ZERO_FID-ROHC_VAR_FID_BASE] = 1;
    }
    
    p_csc->field_value[ROHC_TCP_URG_FLAG_FID-ROHC_VAR_FID_BASE] = 
         (((rohc_tcp_t*)p_csc->p_cpib->tcp)->flags & ROHC_TCP_URG_MASK )>>5;
    p_csc->field_value[ROHC_TCP_ACK_FLAG_FID-ROHC_VAR_FID_BASE] = 
         (((rohc_tcp_t*)p_csc->p_cpib->tcp)->flags & ROHC_TCP_ACK_MASK )>>4;
    p_csc->field_value[ROHC_TCP_PSH_FLAG_FID-ROHC_VAR_FID_BASE] = 
         (((rohc_tcp_t*)p_csc->p_cpib->tcp)->flags & ROHC_TCP_PSH_MASK )>>3;
    p_csc->field_value[ROHC_TCP_RST_FLAG_FID-ROHC_VAR_FID_BASE] = 
         (((rohc_tcp_t*)p_csc->p_cpib->tcp)->flags & ROHC_TCP_RST_MASK )>>2;
    p_csc->field_value[ROHC_TCP_SYN_FLAG_FID-ROHC_VAR_FID_BASE] = 
         (((rohc_tcp_t*)p_csc->p_cpib->tcp)->flags & ROHC_TCP_SYN_MASK )>>1;
    p_csc->field_value[ROHC_TCP_FIN_FLAG_FID-ROHC_VAR_FID_BASE] = 
         (((rohc_tcp_t*)p_csc->p_cpib->tcp)->flags & ROHC_TCP_FIN_MASK );
    if(p_csc->no_of_pkt_recvd >1)
    {
        /* check if ip-ecn or tcp ecn flags are changed */
        if(ROHC_CPRM_IS_CH_INNER_IP_ECN_FLAG(p_csc) || 
                ROHC_CPRM_IS_CH_TCP_ECN_FLAG(p_csc))
        {
            p_csc->ecn_used = 1;
        /* update inner ip ecn flags and tcp ecn flags in context */
        p_csc->field_value[ROHC_INNERIP4_ECN_FID-ROHC_VAR_FID_BASE] = 
            (((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->tos) & ROHC_PROF6_IP4_ECN_FLAG_MASK;
        p_csc->field_value[ROHC_TCP_ECN_FLAG_FID-ROHC_VAR_FID_BASE] = 
            (((rohc_tcp_t*)p_csc->p_cpib->tcp)->flags & ROHC_TCP_ECN_MASK )>>6;
        }
        else
        {
            p_csc->ecn_used = 0;
        }
    }
    else
    {
        /* update ecn behavior according to TCP ECN flags or according to IP
         * ECN Flags received in first pkt*/
        if((ROHC_CSC_TCP_ECN_FLAGS(p_csc->p_cpib)) ||
                (ROHC_CSC_INNER_IP_ECN_FLAGS(p_csc->p_cpib)))
        {
            p_csc->ecn_used = 1;
        }
        else
        {
            p_csc->ecn_used = 0;
        }
        /* update inner ip ecn flags and tcp ecn flags in context */
        p_csc->field_value[ROHC_INNERIP4_ECN_FID-ROHC_VAR_FID_BASE] = 
            (((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->tos) & ROHC_PROF6_IP4_ECN_FLAG_MASK;
        p_csc->field_value[ROHC_TCP_ECN_FLAG_FID-ROHC_VAR_FID_BASE] = 
            (((rohc_tcp_t*)p_csc->p_cpib->tcp)->flags & ROHC_TCP_ECN_MASK )>>6;
    }
    p_csc->field_value[ROHC_TCP_ECN_USED_FID-ROHC_VAR_FID_BASE] = 
        p_csc->ecn_used;
    /* store scaled value, stride  and residue of SN and ACK in context*/
    p_csc->field_value[ROHC_TCP_SN_STRIDE_FID-ROHC_VAR_FID_BASE] = 
        p_csc->sn_stride;
    p_csc->sn_residue = p_csc->temp_sn_residue;
    if(p_csc->sn_stride)
    {
        p_csc->field_value[ROHC_TCP_SCALED_SN_FID-ROHC_VAR_FID_BASE] = 
            p_csc->field_value[ROHC_SN_FID-ROHC_VAR_FID_BASE] / p_csc->sn_stride;
    }

    p_csc->ack_residue = p_csc->temp_ack_residue;
    if(p_csc->ack_stride)
    {
        p_csc->field_value[ROHC_TCP_SCALED_ACKNO_FID-ROHC_VAR_FID_BASE] = 
            p_csc->field_value[ROHC_TCP_ACKNO_FID-ROHC_VAR_FID_BASE] / p_csc->ack_stride;
    }
    ROHC_EXIT_FN("rohc_cprm_p6_update_csc_tcp");
}  


/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_p6_update_csc_inner_ip4
******************************************************************************
**
**      DESCRIPTION:
**              This funtion updates from CPIB to csc for inner_ip4 
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**              delta_field[IN]   Computed Delta field    
**              st_update_flag[IN]Specifies whether to update the static 
**                                context or not.
**
**    Static part:
**
**      Version, Protocol, Source Address, Destination Address.
**
**   Dynamic part:
**
**      Type of Service, Time to Live, Identification, DF, RND, NBO,
**      extension header list.
**
**   
**      RETURN VALUE:
**              rohc_void_t
**      NOTES:
**               
*****************************************************************************/
    /* +- SPR 17777 */
rohc_void_t rohc_cprm_p6_update_csc_inner_ip4 ROHC_PARAMS(
            (p_csc, en_ip_id),
            rohc_csc_tcp_t        *p_csc    _AND_
            rohc_encoded_value_t  *en_ip_id 
            )
{
    ROHC_ENTER_FN("rohc_cprm_p6_update_csc_inner_ip4");
    /* Inner ipv4 static param */
    p_csc->field_value[ROHC_INNERIP4_SRC_FID-ROHC_VAR_FID_BASE] = 
                ROHC_NTOHL(((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->src);
                                    
    p_csc->field_value[ROHC_INNERIP4_DST_FID-ROHC_VAR_FID_BASE] = 
                ROHC_NTOHL(((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->dst); 
    
    /* Inner ipv4 dynamic param
     proto field is to be verified */
    p_csc->field_value[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE] = 
                            en_ip_id->value;
    p_csc->field_len[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE] = en_ip_id->bits;
    
    p_csc->field_value[ROHC_INNERIP4_DSCP_FID-ROHC_VAR_FID_BASE] = 
                            ((((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->tos) & ROHC_PROF6_IP4_DSCP_MASK)>>2;
    
    p_csc->field_value[ROHC_INNERIP4_TTL_FID-ROHC_VAR_FID_BASE] = 
                            ((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->ttl;
    
    p_csc->old_inner_ip4_ttl = 
                            ((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->ttl;
    
    p_csc->field_value[ROHC_INNERIP4_PROTO_FID-ROHC_VAR_FID_BASE] = 
                            ((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->proto;
    p_csc->field_value[ROHC_INNERIP_DF_FID-ROHC_VAR_FID_BASE ] = 
                (ROHC_NTOHS(((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->off) &
                 ROHC_IP4_DF)>>14;
    p_csc->field_value[ROHC_INNERIP_BEH_FID-ROHC_VAR_FID_BASE] =
                  p_csc->inner_ipid_beh;
                            
                
    ROHC_EXIT_FN("rohc_cprm_p6_update_csc_inner_ip4");
}
    
