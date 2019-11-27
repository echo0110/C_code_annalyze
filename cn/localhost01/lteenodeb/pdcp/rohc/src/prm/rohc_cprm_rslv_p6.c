/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 *******************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "rohc_cprm_proto.h"
#include "rohc_cprm_mcr.h"
#include "rohc_cprm_def_p6.h"
#include "rohc_ed_def.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/
 STATIC rohc_U32bit_t rohc_cprm_p6_delta_inner_ip4 ROHC_PROTO
     ((
      rohc_csc_tcp_t          *p_csc));

 STATIC rohc_U32bit_t rohc_cprm_p6_delta_tcp_flags ROHC_PROTO
     ((
      rohc_csc_tcp_t          *p_csc));

 STATIC rohc_void_t rohc_sel_pkt_optlist_rsf_ttl_ecn_change(
        rohc_entity_t           *p_entity,
        rohc_csc_tcp_t          *p_csc, 
        rohc_U32bit_t           sn,
        rohc_U32bit_t           ack,
        rohc_encoded_value_t    *p_en_sn,
        rohc_encoded_value_t    *p_en_ack,
        rohc_U8bit_t            isSNChanged,
        rohc_U8bit_t            isACKChanged,
        rohc_U8bit_t            *p_pkt_typ);

 STATIC rohc_void_t rohc_sel_pkt_tcp_window_change(
        rohc_entity_t           *p_entity,
        rohc_csc_tcp_t          *p_csc, 
        rohc_U32bit_t           sn,
        rohc_U32bit_t           ack,
        rohc_encoded_value_t    *p_en_sn,
        rohc_encoded_value_t    *p_en_ack,
        rohc_U8bit_t            isSNChanged,
        rohc_U8bit_t            isACKChanged,
        rohc_U8bit_t            *p_pkt_typ);
        
 STATIC rohc_void_t rohc_sel_pkt_tcp_sn_and_ack_change(
        rohc_entity_t           *p_entity,
        rohc_csc_tcp_t          *p_csc, 
        rohc_U32bit_t           sn,
        rohc_U32bit_t           ack,
        rohc_encoded_value_t    *p_en_sn,
        rohc_encoded_value_t    *p_en_ack,
        rohc_U8bit_t            isSNChanged,
        rohc_U8bit_t            isACKChanged,
        rohc_U8bit_t            *p_pkt_typ,
        rohc_U8bit_t            isSNFieldScaling);

 STATIC rohc_void_t rohc_sel_pkt_tcp_sn_change(
        rohc_entity_t           *p_entity,
        rohc_csc_tcp_t          *p_csc, 
        rohc_U32bit_t           sn,
        rohc_U32bit_t           ack,
        rohc_encoded_value_t    *p_en_sn,
        rohc_encoded_value_t    *p_en_ack,
        rohc_U8bit_t            isSNChanged,
        rohc_U8bit_t            isACKChanged,
        rohc_U8bit_t            *p_pkt_typ,
        rohc_U8bit_t            isSNFieldScaling);

 STATIC rohc_void_t rohc_sel_pkt_tcp_ack_change(
        rohc_entity_t           *p_entity,
        rohc_csc_tcp_t          *p_csc, 
        rohc_U32bit_t           sn,
        rohc_U32bit_t           ack,
        rohc_encoded_value_t    *p_en_sn,
        rohc_encoded_value_t    *p_en_ack,
        rohc_U8bit_t            isSNChanged,
        rohc_U8bit_t            isACKChanged,
        rohc_U8bit_t            *p_pkt_typ,
        rohc_U8bit_t            isACKFieldScaling,
        rohc_encoded_value_t    *p_en_scaled_ack,
        rohc_U8bit_t            *p_num_of_co_common_pkts_sent);

STATIC rohc_U32bit_t rohc_check_gen_opt_len(
     rohc_csc_tcp_t    *p_csc);
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
**      FUNCTION:
**              rohc_cprm_p6_compute_delta
******************************************************************************
**
**      DESCRIPTION:
**              Computes the delta bit field for profile p1
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**              en_sn   [IN]      Encoded value of sn
**              en_ts   [IN]      Encoded value of ack
**              en_ip_id[IN]      Encoded value of ip_id
**      RETURN VALUE:
**              Delta bit map field
**      NOTES:
**               
*****************************************************************************/
        /* +- SPR 17777 */
rohc_U32bit_t rohc_cprm_p6_compute_delta(
    rohc_csc_tcp_t          *p_csc)
{
    rohc_U32bit_t ret_value = 0;
    rohc_U32bit_t return_bitmap = 0;
    rohc_U8bit_t  list_present = 0;
    ROHC_ENTER_FN("rohc_cprm_p6_compute_delta");
    p_csc->ctrl_field_val_p6[ROHC_TCP_RSF_FLAGS_FID
        -ROHC_PROF6_VAR_CTRL_FID_BASE]=0;
    p_csc->ctrl_field_val_p6[ROHC_INNERIP4_TTL_HPL_PRESENT_FID
        - ROHC_PROF6_VAR_CTRL_FID_BASE] = 0;
    p_csc->ctrl_field_val_p6[ROHC_TCP_URG_PTR_PRESENT_FID
        -ROHC_PROF6_VAR_CTRL_FID_BASE] = 0;
    p_csc->ctrl_field_val_p6[ROHC_TCP_LIST_PRESENT_FID-
        ROHC_PROF6_VAR_CTRL_FID_BASE] = 0;
    p_csc->ctrl_field_val_p6[ROHC_INNERIP4_DSCP_PRESENT_FID
        -ROHC_PROF6_VAR_CTRL_FID_BASE] = 0;
    p_csc->ctrl_field_val_p6[ROHC_INNERIP4_TTL_HPL_PRESENT_FID-
        ROHC_PROF6_VAR_CTRL_FID_BASE] = 0;
    p_csc->ctrl_field_val_p6[ROHC_TCP_WINDOW_IND_FID-ROHC_PROF6_VAR_CTRL_FID_BASE] = 0;
    p_csc->sn_residue_change = 0;
    p_csc->sn_stride_change = 0;
    p_csc->ack_residue_change = 0;
    p_csc->ack_stride_change = 0;
    rohc_parse_tcp_options(p_csc, &list_present);

    return_bitmap = rohc_cprm_p6_delta_tcp_flags(p_csc);
    ret_value |= return_bitmap;
    return_bitmap = 0;

    if(ROHC_CPRM_IS_CH_URG_PTR(p_csc) || ROHC_CPRM_IS_CH_URG_FLAG(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_UPTR;
        p_csc->field_value[ROHC_TCP_URG_FLAG_FID-ROHC_VAR_FID_BASE] =  
            (((rohc_tcp_t*)p_csc->p_cpib->tcp)->flags & ROHC_TCP_URG_MASK )>>5;
        if(ROHC_CPRM_IS_CH_URG_PTR(p_csc))
        {
            p_csc->field_value[ROHC_TCP_URG_PTR_FID-ROHC_VAR_FID_BASE] = 
                ROHC_NTOHS(((rohc_tcp_t*)p_csc->p_cpib->tcp)->urg_ptr) ;
            p_csc->ctrl_field_val_p6[ROHC_TCP_URG_PTR_PRESENT_FID-
                ROHC_PROF6_VAR_CTRL_FID_BASE] = 1;
        }
    }

    if(ROHC_CPRM_IS_CH_TCP_OPT_LIST(p_csc) || list_present)
    {
        ret_value |= ROHC_CPRM_CH_TCP_OPT_LIST;
        /*  p_csc->field_value[ROHC_TCP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
         *  ROHC_ENC_TCP_OPT_LIST_P6_MASK;*/
        p_csc->ctrl_field_val_p6[ROHC_TCP_LIST_PRESENT_FID-ROHC_PROF6_VAR_CTRL_FID_BASE]
            = 1;
        /* update the tcp_opt_kind array in context to store new order of
         * tcp options*/
        ROHC_MEMCPY((p_csc->rohc_c_tcp_opt).tcp_opt_kind, 
                p_csc->p_cpib->temp_tcp_opt_kind, ROHC_MAX_ENTRIES_TCP_OPT_IDX+1);
    }
    if(!p_csc->ctrl_field_val_p6[ROHC_TCP_LIST_PRESENT_FID-ROHC_PROF6_VAR_CTRL_FID_BASE])
    {
        return_bitmap = rohc_check_gen_opt_len(p_csc);
        ret_value |= return_bitmap;
        return_bitmap = 0;
    }

    if(ROHC_CPRM_IS_CH_SN_STRIDE(p_csc))
    {
        p_csc->sn_stride_change = 1;
        p_csc->field_value[ROHC_TCP_SN_STRIDE_FID-ROHC_VAR_FID_BASE] =
            p_csc->sn_stride;
        /* scaled values need to be updated */
        if(p_csc->sn_stride)
        {
            p_csc->field_value[ROHC_TCP_SCALED_SN_FID-ROHC_VAR_FID_BASE] = 
                p_csc->field_value[ROHC_SN_FID-ROHC_VAR_FID_BASE] / (p_csc->sn_stride);
        }
    }

    if(ROHC_CPRM_IS_CH_SN_RESIDUE(p_csc))
    {
        p_csc->sn_residue_change = 1;
        p_csc->sn_residue = p_csc->temp_sn_residue;
    }

    if(ROHC_CPRM_IS_CH_ACK_RESIDUE(p_csc))
    {
        p_csc->ack_residue_change = 1;
        p_csc->ack_residue = p_csc->temp_ack_residue;
    }

    if(ROHC_CPRM_IS_CH_TCP_WINDOW(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_TCP_WINDOW;
        p_csc->field_value[ROHC_TCP_WINDOW_FID-ROHC_VAR_FID_BASE] = 
            ROHC_NTOHS((((rohc_tcp_t*)p_csc->p_cpib->tcp)->window));
        p_csc->ctrl_field_val_p6[ROHC_TCP_WINDOW_IND_FID-ROHC_PROF6_VAR_CTRL_FID_BASE] = 1;
    }

    if(ROHC_CPRM_IS_CH_TCP_SN(p_csc) )
    {
        ret_value |= ROHC_CPRM_CH_TCP_SN;
        p_csc->old_tcp_sn = p_csc->field_value[ROHC_SN_FID-ROHC_VAR_FID_BASE];
        p_csc->field_value[ROHC_SN_FID-ROHC_VAR_FID_BASE] =  
            p_csc->p_cpib->temp_sn ;
        /* update scaled value stored in context*/
        if(p_csc->sn_stride)
        {
            p_csc->field_value[ROHC_TCP_SCALED_SN_FID-ROHC_VAR_FID_BASE] =
                (p_csc->field_value[ROHC_SN_FID-ROHC_VAR_FID_BASE]) / (p_csc->sn_stride);
        }
    }
    /* msn is incremented  at every packet*/
    p_csc->field_value[ROHC_TCP_MSN_FID-ROHC_VAR_FID_BASE] = 
        p_csc->tcp_msn;

    if(ROHC_CPRM_IS_CH_TCP_ACK_NO(p_csc) )
    {
        ret_value |= ROHC_CPRM_CH_TCP_ACK_NO;
        p_csc->field_value[ROHC_TCP_ACKNO_FID-ROHC_VAR_FID_BASE] =  
            p_csc->p_cpib->temp_ack ;
        if(p_csc->ack_stride)
        {
            p_csc->field_value[ROHC_TCP_SCALED_ACKNO_FID-ROHC_VAR_FID_BASE] =
                (p_csc->field_value[ROHC_TCP_ACKNO_FID-ROHC_VAR_FID_BASE]) /
                (p_csc->ack_stride);
        }
    }

    if(ROHC_CPRM_IS_CH_ACK_STRIDE(p_csc))
    {
        p_csc->ack_stride_change = 1;
        p_csc->field_value[ROHC_TCP_ACK_STRIDE_FLAG_FID-ROHC_VAR_FID_BASE] = 1;
        p_csc->field_value[ROHC_TCP_ACK_STRIDE_FID-ROHC_VAR_FID_BASE] =
            p_csc->ack_stride;
        /* scaled values need to be updated */
        if(p_csc->ack_stride)
        {
            p_csc->field_value[ROHC_TCP_ACK_STRIDE_FLAG_FID-ROHC_VAR_FID_BASE] = 1;
            p_csc->field_value[ROHC_TCP_SCALED_ACKNO_FID-ROHC_VAR_FID_BASE] = 
                p_csc->field_value[ROHC_TCP_ACKNO_FID-ROHC_VAR_FID_BASE] / 
                (p_csc->ack_stride);
        }
        else
        {
            p_csc->field_value[ROHC_TCP_ACK_STRIDE_FLAG_FID-ROHC_VAR_FID_BASE]
                = 0;
        }
    }
    else
    {
        p_csc->field_value[ROHC_TCP_ACK_STRIDE_FLAG_FID-ROHC_VAR_FID_BASE] = 0;
    }


    /* IP - inner */    
    if(ROHC_CPRM_IS_CH_I(p_csc))
    {
        /* IPv4 and IP-ID */
        /* In SEQ packet format, we have to check here for IP-ID
         * behavior*/
        return_bitmap = rohc_cprm_p6_delta_inner_ip4(p_csc);
    }
    else
    {
        /* Handling of inner ipv6 */
    }

    ret_value |= return_bitmap; 

    if( p_csc->p_cpib->tcp_cksum_present ) 
    {
        p_csc->field_value[ROHC_TCP_CKSUM_FID-ROHC_VAR_FID_BASE] = 
            ROHC_NTOHS(((rohc_tcp_t*)(p_csc->p_cpib->tcp))->checksum);
    }
    /* update PSH flag */
    p_csc->field_value[ROHC_TCP_PSH_FLAG_FID-ROHC_VAR_FID_BASE] =
        (((rohc_tcp_t*)p_csc->p_cpib->tcp)->flags & ROHC_TCP_PSH_MASK )>>3;

    ROHC_EXIT_FN("rohc_cprm_p6_compute_delta");
    return ret_value;
}

/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_p6_delta_tcp_flags
******************************************************************************
**
**      DESCRIPTION:
**              Computes the delta bit field for tcp hdr flags
**      ARGUMENTS:
**              p_csc   [IN]      Pointer to the CSC block
**      RETURN VALUE:
**              Delta bit map field
**               
*****************************************************************************/
STATIC rohc_U32bit_t rohc_cprm_p6_delta_tcp_flags ROHC_PARAMS
    ((p_csc) ,
     rohc_csc_tcp_t    *p_csc)
{
    rohc_U32bit_t ret_value = 0;
    if(ROHC_CPRM_IS_CH_T_ACK_FLAG(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_ACK_FLAG;
        p_csc->field_value[ROHC_TCP_ACK_FLAG_FID-ROHC_VAR_FID_BASE] =  
            (((rohc_tcp_t*)p_csc->p_cpib->tcp)->flags & ROHC_TCP_ACK_MASK )>>4;
    }
    if(ROHC_CPRM_IS_CH_RST_FLAG(p_csc) )
    {
        p_csc->field_value[ROHC_TCP_RST_FLAG_FID-ROHC_VAR_FID_BASE] =  
            (((rohc_tcp_t*)p_csc->p_cpib->tcp)->flags & ROHC_TCP_RST_MASK )>>2;
        p_csc->ctrl_field_val_p6[ROHC_TCP_RSF_FLAGS_FID-ROHC_PROF6_VAR_CTRL_FID_BASE] |=
            ROHC_ENC_RST_FLAG_TCP_FLAGP6_MASK ;
    }
    if(ROHC_CPRM_IS_CH_SYN_FLAG(p_csc) )
    {

        p_csc->field_value[ROHC_TCP_SYN_FLAG_FID-ROHC_VAR_FID_BASE] =  
            (((rohc_tcp_t*)p_csc->p_cpib->tcp)->flags & ROHC_TCP_SYN_MASK )>>1;
        p_csc->ctrl_field_val_p6[ROHC_TCP_RSF_FLAGS_FID-ROHC_PROF6_VAR_CTRL_FID_BASE] |=
            ROHC_ENC_SYN_FLAG_TCP_FLAGP6_MASK ;
    }
    if(ROHC_CPRM_IS_CH_FIN_FLAG(p_csc) )
    {
        p_csc->field_value[ROHC_TCP_FIN_FLAG_FID-ROHC_VAR_FID_BASE] =  
            (((rohc_tcp_t*)p_csc->p_cpib->tcp)->flags) & (ROHC_TCP_FIN_MASK );
        p_csc->ctrl_field_val_p6[ROHC_TCP_RSF_FLAGS_FID-ROHC_PROF6_VAR_CTRL_FID_BASE] |=
            ROHC_ENC_FIN_FLAG_TCP_FLAGP6_MASK ;
    }
    if(p_csc->ctrl_field_val_p6[ROHC_TCP_RSF_FLAGS_FID-ROHC_PROF6_VAR_CTRL_FID_BASE])
    {

        ret_value |= ROHC_CPRM_CH_RSF_FLAG;  /* correspond to change in any of 
                                              * RST/SYN/FIN flag*/
    }
    return ret_value;
}

/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_p6_delta_inner_ip4
******************************************************************************
**
**      DESCRIPTION:
**              Computes the delta bit field for inner ipv4 hdr for profile p6
**      ARGUMENTS:
**              p_csc   [IN]      Pointer to the CSC block
**      RETURN VALUE:
**              Delta bit map field
**               
*****************************************************************************/
STATIC rohc_U32bit_t rohc_cprm_p6_delta_inner_ip4 ROHC_PARAMS
    ((p_csc) ,
     rohc_csc_tcp_t    *p_csc)
{
    rohc_U32bit_t ret_value = 0;
    if(ROHC_CPRM_IS_CH_DSCP(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_IP_DSCP;
        p_csc->ctrl_field_val_p6[ROHC_INNERIP4_DSCP_PRESENT_FID-ROHC_PROF6_VAR_CTRL_FID_BASE] = 1;
        p_csc->field_value[ROHC_INNERIP4_DSCP_FID-ROHC_VAR_FID_BASE] = 
            ((((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->tos) & ROHC_PROF6_IP4_DSCP_MASK)>>2;

    }
    if(ROHC_CPRM_IS_CH_TTL(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_IP_TTL_HL;
        p_csc->ctrl_field_val_p6[ROHC_INNERIP4_TTL_HPL_PRESENT_FID-ROHC_PROF6_VAR_CTRL_FID_BASE] = 1;
        p_csc->old_inner_ip4_ttl = p_csc->field_value[ROHC_INNERIP4_TTL_FID -
            ROHC_VAR_FID_BASE];
        p_csc->field_value[ROHC_INNERIP4_TTL_FID-ROHC_VAR_FID_BASE] = 
            ((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->ttl;
    }


    if(ROHC_CPRM_IS_CH_DF(p_csc))
    {   
        ret_value |= ROHC_CPRM_CH_IP_DF;
        p_csc->field_value[ROHC_INNERIP_DF_FID-ROHC_VAR_FID_BASE ] = 
            (ROHC_NTOHS(((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->off) & 
             ROHC_IP4_DF)>>14;

    }

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

    if(ROHC_CPRM_IS_CH_ECN_BEH(p_csc))
    {   
        ret_value |= ROHC_CPRM_CH_ECN_USED;
        p_csc->field_value[ROHC_TCP_ECN_USED_FID-ROHC_VAR_FID_BASE ] = 
            p_csc->ecn_used;
    }
    if(ROHC_CPRM_IS_CH_INNER_IPID_BEH(p_csc))
    {   
        ret_value |= ROHC_CPRM_CH_INNER_IPID_BEH;
        p_csc->field_value[ROHC_INNERIP_BEH_FID-ROHC_VAR_FID_BASE ] = 
            p_csc->inner_ipid_beh;
    }
    return ret_value;
}

/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_p6_pkt_det
******************************************************************************
**
**      DESCRIPTION:
**              This function determines the packet type for profile 1,
**              when compressor is operated in U/O mode
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**              delta_bit_map[IN] Delta bit map for changing fields
**              en_sn   [IN]      Encoded value of sn
**              en_ts   [IN]      Encoded value of ts
**              en_ip_id[IN]      Encoded value of ip_id
**              no_crc_bit[IN}    Number of crc bits    
**      RETURN VALUE:
**              Rohc packet type
**      NOTES:
**               
*****************************************************************************/

rohc_U8bit_t rohc_cprm_p6_pkt_det ROHC_PARAMS(
            (p_entity, p_csc, delta_bit_map, p_en_sn, p_en_scaled_sn,
                 p_en_ack, p_en_scaled_ack, p_en_ip_id, p_pkt_typ),
                    rohc_entity_t           *p_entity        _AND_
                    rohc_csc_tcp_t          *p_csc           _AND_
                    rohc_U32bit_t           delta_bit_map    _AND_
                    rohc_encoded_value_t    *p_en_sn         _AND_
                    rohc_encoded_value_t    *p_en_scaled_sn  _AND_
                    rohc_encoded_value_t    *p_en_ack        _AND_
                    rohc_encoded_value_t    *p_en_scaled_ack _AND_
                    rohc_encoded_value_t    *p_en_ip_id      _AND_
                    rohc_U8bit_t            *p_pkt_typ)
{ 
    ROHC_ENTER_FN("rohc_cprm_p6_pkt_det");
        /* +- SPR 17777 */
  LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
          __LINE__,p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
          ROHC_GET_CSC_CID(p_csc),sizeof(p_en_ip_id),0,0, __func__,"");

    rohc_U8bit_t isSNFieldScaling = 0;
    rohc_U8bit_t isACKFieldScaling = 0;
    rohc_U8bit_t isSNChanged = 0;
    rohc_U8bit_t isACKChanged = 0;
    rohc_U8bit_t index = 0;
    rohc_U8bit_t ts_opt_present = 0;
    rohc_U8bit_t ts_opt_list = 0;
    rohc_U32bit_t   ts_val=0;
    rohc_U32bit_t   ts_echo=0;
    rohc_encoded_value_t p_en_val;
    ROHC_MEMSET((rohc_void_tp)&p_en_val,0,
        sizeof(rohc_encoded_value_t));
    rohc_U32bit_t sn = p_csc->field_value[ROHC_SN_FID-ROHC_VAR_FID_BASE];
    rohc_U32bit_t ack = p_csc->field_value[ROHC_TCP_ACKNO_FID-ROHC_VAR_FID_BASE];

    /* Call encoding functiion if value of concerned field is changed*/
    if(delta_bit_map & ROHC_CPRM_CH_TCP_SN)
    {
        isSNChanged = 1;
        /* update window of scaled SN also */
        /* rohc_ed_p6_update_gsw_sn_scaled(p_entity, p_csc, 
           sn, p_en_scaled_sn);*/
        /* determine if SN can be sent scaled or not */
        isSNFieldScaling = field_scaling_sn(sn,
                p_csc->field_value[ROHC_TCP_SCALED_SN_FID-ROHC_VAR_FID_BASE],
                p_csc->field_value[ROHC_TCP_SN_STRIDE_FID-ROHC_VAR_FID_BASE],
                p_csc->sn_residue,
                /* +- SPR 17777 */
                &(p_csc->sn_scaling_factor_estab_counter),
                p_csc->sn_stride_change,
                p_csc->sn_residue_change,
                p_csc->old_tcp_sn,
                p_en_scaled_sn);

    }

    if(delta_bit_map & ROHC_CPRM_CH_TCP_ACK_NO)
    {
        isACKChanged = 1;
        /* update window of scaled ACK also */
        /* rohc_ed_p6_update_gsw_ack_scaled(p_entity, p_csc, 
           ack, p_en_scaled_ack);*/
        /* determine if ACK NO can be sent scaled or not */
        isACKFieldScaling = field_scaling_ack(ack,
                p_csc->field_value[ROHC_TCP_SCALED_ACKNO_FID-ROHC_VAR_FID_BASE],
                p_csc->field_value[ROHC_TCP_ACK_STRIDE_FID-ROHC_VAR_FID_BASE],
                p_csc->ack_residue,
                /* +- SPR 17777 */
                &(p_csc->ack_scaling_factor_estab_counter),
                p_csc->ack_stride_change,
                p_csc->ack_residue_change);

    }
    if(!isACKFieldScaling)
    {
        /* Start SPR 8415 FIX */
           p_csc->num_of_co_common_pkts_sent = 0;
        /* End SPR 8415 FIX */
    }
    /* check if ts option if present  can be sent in irregular chain, if not, 
     * it must be sent in list and pkt will be either co-common or rnd8 */
    if(!(p_csc->ctrl_field_val_p6[ROHC_TCP_LIST_PRESENT_FID -
            ROHC_PROF6_VAR_CTRL_FID_BASE]))
    {
        while(p_csc->rohc_c_tcp_opt.tcp_opt_kind[index]!= 0xff &&
                index < ROHC_MAX_ENTRIES_TCP_OPT_IDX)
        {
            if(TCP_TS_OPT == p_csc->rohc_c_tcp_opt.tcp_opt_kind[index])
            {
                ts_opt_present = 1;
                break;
            }
            index++;
        }
        if(ts_opt_present)
        {
            ts_val =  ROHC_NTOHL(p_csc->p_cpib->rohc_c_temp_tcp_opt.ts[0]);
            ts_echo = ROHC_NTOHL(p_csc->p_cpib->rohc_c_temp_tcp_opt.ts[1]);
            rohc_encode_p6_ts_opt(p_entity, ts_val, &p_en_val,
                    p_csc->rohc_gsw.min_ts_val,p_csc->rohc_gsw.max_ts_val);
            if(0 == p_en_val.bits)
            {
                /* ts to be sent in list */
                ts_opt_list = 1;
            }
            ROHC_MEMSET((rohc_void_tp)&p_en_val,0,
                    sizeof(rohc_encoded_value_t));
            rohc_encode_p6_ts_opt(p_entity,ts_echo, &p_en_val,
                    p_csc->rohc_gsw.min_ts_echo,p_csc->rohc_gsw.max_ts_echo);
            if(0 == p_en_val.bits)
            {
                /* ts to be sent in list */
                ts_opt_list = 1;
            }

            /* SPR 9591 fix start */
            ROHC_LOG(LOG_INFO,"ED:EID[%u:%u]:CID[%u]:"\
                              "GSW_START_ID[%u]:"\
                              "GSW_CUR_ID[%u]" \
                              "TS ENCODING CALLED\n", \
                              p_entity->rohc_entity_id.ueIndex,
                              p_entity->rohc_entity_id.lcId,
                              ROHC_GET_CSC_CID(p_csc),
                              p_csc->rohc_gsw.start_index,
                              ROHC_ED_GET_CUR_INDEX(p_csc));
            /* SPR 9591 fix end */
            
        }
    }
    if(ts_opt_list)
    {
        p_csc->ctrl_field_val_p6[ROHC_TCP_LIST_PRESENT_FID - 
            ROHC_PROF6_VAR_CTRL_FID_BASE] = 1;
    }

    /* Packet determination starts here */
    /* first determine common compressed packet */
    if(delta_bit_map & (ROHC_CPRM_CH_ACK_FLAG | ROHC_CPRM_CH_UPTR |
                ROHC_CPRM_CH_IP_DSCP |ROHC_CPRM_CH_IP_DF | 
                ROHC_CPRM_CH_INNER_IPID_BEH))
    {
        *p_pkt_typ = ROHC_CO_P6_COMMON_PKTYPE;
        encode_sn_ack_co_common_pkt(p_entity, p_csc, sn, ack, p_en_sn,
                p_en_ack,isSNChanged,isACKChanged);
    }

    else if((delta_bit_map & (ROHC_CPRM_CH_TCP_OPT_LIST | ROHC_CPRM_CH_RSF_FLAG |
                ROHC_CPRM_CH_IP_TTL_HL|ROHC_CPRM_CH_ECN_USED)) || ts_opt_list)
    {
        rohc_sel_pkt_optlist_rsf_ttl_ecn_change(p_entity,p_csc,
                sn,ack,p_en_sn,p_en_ack,isSNChanged,isACKChanged,p_pkt_typ);
    }

    else if(delta_bit_map & ROHC_CPRM_CH_TCP_WINDOW)
    {
        rohc_sel_pkt_tcp_window_change(p_entity,p_csc,
                sn,ack,p_en_sn,p_en_ack,isSNChanged,isACKChanged,p_pkt_typ);
    }

    else if((delta_bit_map & ROHC_CPRM_CH_TCP_SN) && (
                delta_bit_map & ROHC_CPRM_CH_TCP_ACK_NO))
    {
        rohc_sel_pkt_tcp_sn_and_ack_change(p_entity,p_csc, sn,ack,p_en_sn,
                p_en_ack,isSNChanged,isACKChanged,p_pkt_typ,isSNFieldScaling);
    }

    else if(delta_bit_map & ROHC_CPRM_CH_TCP_SN)
    {
        rohc_sel_pkt_tcp_sn_change(p_entity,p_csc, sn,ack,p_en_sn,
                p_en_ack,isSNChanged,isACKChanged,p_pkt_typ,isSNFieldScaling);
    }

    else if(delta_bit_map & ROHC_CPRM_CH_TCP_ACK_NO)
    {
        /* Start SPR 8415 FIX */
        rohc_sel_pkt_tcp_ack_change(p_entity,p_csc,sn,ack,p_en_sn,
                p_en_ack,isSNChanged,isACKChanged,p_pkt_typ,isACKFieldScaling,
                p_en_scaled_ack,&(p_csc->num_of_co_common_pkts_sent));
        /* End SPR 8415 FIX */
    }
    else if(0 == delta_bit_map)
    {
        /* special case when none of the fields are changing so send pkt with minimal length*/
        *p_pkt_typ = ROHC_CO_P6_RND1_PKTYPE;
    }
    else
    {
        *p_pkt_typ = ROHC_UNKNOWN_PKT;
    }
    p_csc->old_inner_ip4_ttl = p_csc->field_value[ROHC_INNERIP4_TTL_FID -
        ROHC_VAR_FID_BASE];
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION: encode_sn_ack_co_common_pkt
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function encodes sn and ack for co-common pkt
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**              delta_bit_map[IN] Delta bit map for changing fields
**              en_sn   [IN]      Encoded value of sn
**              en_ack   [IN]      Encoded value of ack
**      RETURN VALUE:
**              Rohc packet type
**      NOTES:
**               
*****************************************************************************/
rohc_void_t encode_sn_ack_co_common_pkt(
        rohc_entity_t           *p_entity,
        rohc_csc_tcp_t          *p_csc, 
        rohc_U32bit_t           sn,
        rohc_U32bit_t           ack,
        rohc_encoded_value_t    *p_en_sn,
        rohc_encoded_value_t    *p_en_ack,
        rohc_U8bit_t            isSNChanged,
        rohc_U8bit_t            isACKChanged
        )
{
    if(isSNChanged)
    {
        if(ROHC_FAILURE == rohc_encode_p6_sn(p_entity, p_csc,
                    sn ,p_en_sn,8,63))
        {
            /* call encoding with 16 bits */

            if(ROHC_FAILURE == rohc_encode_p6_sn(p_entity,  p_csc,
                        sn ,p_en_sn, 16, 16383))
            {
                /* send SN in 32 bits */
                p_en_sn->bits = 32;
            }
        }
    }
    if(isACKChanged)
    {
        if(ROHC_FAILURE == rohc_encode_p6_ack(p_entity,  p_csc,
                    ack, p_en_ack, 8, 63))
        {
            /* call encoding with 16 bits */

            if(ROHC_FAILURE == rohc_encode_p6_ack(p_entity,  p_csc,
                        ack,p_en_ack, 16, 16383))
            {
                /* send ACK in 32 bits */
                p_en_ack->bits = 32;
            }

        }
    }
}

/*****************************************************************************
**      FUNCTION:
**              rohc_parse_tcp_options
******************************************************************************
**
**      DESCRIPTION:
**              This function used to parse tcp options and get order of tcp options
**              in uncompressed packet
**      ARGUMENTS:
**              p_csc   [IN]      Pointer to the CSC block
**      RETURN VALUE:
**              stores order of tcp options in p_cpib->temp_tcp_opt_kind array
**      NOTES:
**               
*****************************************************************************/
rohc_void_t rohc_parse_tcp_options ROHC_PARAMS 
    ((p_csc, p_list_present) ,
    rohc_csc_tcp_t          *p_csc  _AND_ 
    rohc_U8bit_t            *p_list_present )
{

    rohc_U8bit_t        index = 0;
    rohc_U8bit_t        *tcp_opt_list = NULL;
    rohc_U8bit_t        len = 0;
    rohc_U8bit_t        tot_len = 0;
    rohc_cpib_t         *p_cpib = ROHC_CSC_GET_CPIB(p_csc);
    rohc_U8bit_t        tcp_opt_present = 0;
    tot_len = ((ROHC_GET_UPPER_NIBBLE(((rohc_tcp_t*)p_csc->p_cpib->tcp)->offset)) 
            * 4) - 20;
    if(tot_len)
    {
        /* store order of tcp options in temp array to check if order of tcp options 
         * is changed or not*/
        for(index = 0; index < ROHC_MAX_ENTRIES_TCP_OPT_IDX; index++)
        {
            ROHC_C_INIT_OPT_KIND(p_csc->p_cpib->temp_tcp_opt_kind[index], 0xff);
        }
        tcp_opt_list = (p_cpib->tcp) + sizeof(rohc_tcp_t);
        index = 0;
        while(tot_len)
        {
            switch(*tcp_opt_list)
            {
                case TCP_NOP_OPT :
                p_cpib->temp_tcp_opt_kind[index] = TCP_NOP_OPT;
                index++;
                tcp_opt_list++;
                tot_len--;
                break;

                case TCP_MSS_OPT :
                p_cpib->temp_tcp_opt_kind[index] = TCP_MSS_OPT;
                p_cpib->rohc_c_temp_tcp_opt.mss = ((rohc_tcp_mss_opt_t*)
                        tcp_opt_list)->val;
                index++;
                tot_len = tot_len - TCP_MSS_OPT_LEN;
                tcp_opt_list+=TCP_MSS_OPT_LEN;
                if( p_cpib->rohc_c_temp_tcp_opt.mss != p_csc->rohc_c_tcp_opt.mss)
                {
                    *p_list_present = 1;
                }
                break;

                case TCP_WS_OPT :
                p_cpib->temp_tcp_opt_kind[index] = TCP_WS_OPT;
                p_cpib->rohc_c_temp_tcp_opt.ws = ((rohc_tcp_ws_opt_t*)
                        tcp_opt_list)->val;
                index++;
                tcp_opt_list+=TCP_WS_OPT_LEN;
                tot_len = tot_len - TCP_WS_OPT_LEN;
                if( p_cpib->rohc_c_temp_tcp_opt.ws != p_csc->rohc_c_tcp_opt.ws)
                {
                    *p_list_present = 1;
                }
                break;

                case TCP_SACK_PERMTD_OPT :
                p_cpib->temp_tcp_opt_kind[index] = TCP_SACK_PERMTD_OPT;
                index++;
                tcp_opt_list+=TCP_SACK_PERMTD_OPT_LEN;
                tot_len = tot_len - TCP_SACK_PERMTD_OPT_LEN;
                break;

                case TCP_SACK_OPT :
                p_cpib->temp_tcp_opt_kind[index] = TCP_SACK_OPT;
                index++;
                len = ((rohc_tcp_sack_opt_t*)tcp_opt_list)->length;
                p_cpib->rohc_c_temp_tcp_opt.sack_len = len;
                ROHC_MEMCPY(p_cpib->rohc_c_temp_tcp_opt.sack_ptr, tcp_opt_list+2, len-2);
                tcp_opt_list+=len;
                tot_len = tot_len - len;
                break;

                case TCP_TS_OPT :
                p_cpib->temp_tcp_opt_kind[index] = TCP_TS_OPT;
                index++;
                ROHC_MEMCPY(p_cpib->rohc_c_temp_tcp_opt.ts, tcp_opt_list+2, 8);
                tcp_opt_list+=TCP_TS_OPT_LEN;
                tot_len = tot_len - TCP_TS_OPT_LEN;
                tcp_opt_present = ROHC_TRUE;
                break;

                case TCP_EOL_OPT :
                /* means options are over, rest are padding octets*/
                p_cpib->temp_tcp_opt_kind[index] = TCP_EOL_OPT;
                p_cpib->rohc_c_temp_tcp_opt.eol_len = tot_len;
                tot_len = 0;
                break;

                case TCP_ALT_CKSUM_REQ_OPT :
                p_cpib->temp_tcp_opt_kind[index] = TCP_ALT_CKSUM_REQ_OPT;
                index++;
                p_cpib->rohc_c_temp_tcp_opt.alt_cksum_req = ((
                            rohc_tcp_alt_cksum_req_opt_t*)tcp_opt_list)->val;
                tcp_opt_list+=TCP_ALT_CKSUM_REQ_OPT_LEN;
                tot_len = tot_len - TCP_ALT_CKSUM_REQ_OPT_LEN;
                if( p_cpib->rohc_c_temp_tcp_opt.alt_cksum_req != p_csc->rohc_c_tcp_opt.alt_cksum_req)
                {
                    *p_list_present = 1;
                }
                break;

                default :
                /* generic options */
                p_cpib->temp_tcp_opt_kind[index] = *tcp_opt_list;
                index++;
                len = *(tcp_opt_list + 1);
                rohc_c_parse_generic_option(tcp_opt_list,&(p_cpib->rohc_c_temp_tcp_opt.
                            rohc_c_generic_opt));
                tcp_opt_list+=len;
                tot_len = tot_len - len;
                break;
            }
        }
        if(tcp_opt_present)
        {
            /* set presence of TS opt true for current pkt */
            p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].
                ts_opt_recvd = ROHC_TRUE;
        }
        else
        {
            /* set presence of TS opt false for current pkt */
            p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].
                ts_opt_recvd = ROHC_FALSE;
        }
    }
}

/*****************************************************************************
**      FUNCTION:
**              rohc_c_parse_generic_option
******************************************************************************
**
**      DESCRIPTION:
**              This function used to parse tcp options and get order of tcp options
**              in uncompressed packet
**      ARGUMENTS:
**              p_csc   [IN]      Pointer to the CSC block
**      RETURN VALUE:
**              
**      NOTES:
**               
*****************************************************************************/
rohc_void_t rohc_c_parse_generic_option ROHC_PARAMS 
    ((tcp_opt_list, p_generic_opt) ,
    rohc_U8bit_t    *tcp_opt_list   _AND_
    rohc_c_generic_opt_t *p_generic_opt )
{
    rohc_c_generic_opt_elem_t    *p_generic_opt_elem;
    
    p_generic_opt_elem = &(p_generic_opt->rohc_generic_opt_elem[p_generic_opt
            ->next_free_index]);
    p_generic_opt_elem->kind = *tcp_opt_list;
    p_generic_opt_elem->length = *(tcp_opt_list+1);
    p_generic_opt_elem->option_static = 0;
    p_generic_opt_elem->opt_val = tcp_opt_list+2;
  
    p_generic_opt->next_free_index = p_generic_opt->next_free_index + 1;
}

/*****************************************************************************
**      FUNCTION:
**             field_scaling_sn
******************************************************************************
**
**      DESCRIPTION:
**              This function determines whether the field value
                can be send scaled or not
**      ARGUMENTS:
**              unscaled_val[IN]  Unscaled value of the field
**              stride   [IN]     scaling factor
**              residue[IN]       offset i.e. unscaled val %stride
**              delta_field[IN]   determines whether field is changed or not
**      RETURN VALUE:
**              SUCCESS if scaling can be used else FAILURE
**      NOTES:
**               
*****************************************************************************/
                /* +- SPR 17777 */
rohc_return_t field_scaling_sn ROHC_PARAMS(
            (unscaled_val, scaled_val, stride, residue,
             p_sn_scaling_factor_estab_counter,sn_stride_change,sn_residue_change,
             old_tcp_sn,p_en_scaled_sn),
                    rohc_U32bit_t           unscaled_val     _AND_
                    rohc_U32bit_t           scaled_val       _AND_
                    rohc_U16bit_t           stride           _AND_
                    rohc_U32bit_t           residue          _AND_
                    rohc_U8bit_t            *p_sn_scaling_factor_estab_counter _AND_
                    rohc_U8bit_t            sn_stride_change      _AND_ 
                    rohc_U8bit_t            sn_residue_change   _AND_
                    rohc_U32bit_t           old_tcp_sn          _AND_
                    rohc_encoded_value_t    *p_en_scaled_sn
                    ) 
{
    rohc_return_t ret_val = ROHC_SUCCESS;
    if((stride!= 0) && (residue == unscaled_val % stride) &&
            (unscaled_val == scaled_val * stride + residue)&&
             !(sn_residue_change) && !(sn_stride_change))

    {
        (*p_sn_scaling_factor_estab_counter)++;
        if(*p_sn_scaling_factor_estab_counter > 2)
        {
            /* check if scaled values lies withing interpretation interval*/
            if(ROHC_FAILURE == rohc_encode_p6_scaled_sn( 
                        scaled_val ,p_en_scaled_sn, 
                        4,7,old_tcp_sn,stride ))
            {
                ret_val = ROHC_FAILURE;
            }
            else
            {
            ret_val = ROHC_SUCCESS;
            }
        }
        else
        {
            ret_val = ROHC_FAILURE;
        }
        
    }
    else
    {
        *p_sn_scaling_factor_estab_counter = 0;
        ret_val = ROHC_FAILURE;
    }
    return ret_val;
}


/*****************************************************************************
**      FUNCTION:
**             field_scaling_ack
******************************************************************************
**
**      DESCRIPTION:
**              This function determines whether the field value
                can be send scaled or not
**      ARGUMENTS:
**              unscaled_val[IN]  Unscaled value of the field
**              stride   [IN]     scaling factor
**              residue[IN]       offset i.e. unscaled val %stride
**      RETURN VALUE:
**              SUCCESS if scaling can be used else FAILURE
**      NOTES:
**               
*****************************************************************************/
                /* +- SPR 17777 */
rohc_return_t field_scaling_ack ROHC_PARAMS(
            (unscaled_val, scaled_val, stride, residue,
             p_ack_scaling_factor_estab_counter,ack_stride_change,ack_residue_change),
                    rohc_U32bit_t           unscaled_val     _AND_
                    rohc_U32bit_t           scaled_val       _AND_
                    rohc_U16bit_t           stride           _AND_
                    rohc_U32bit_t           residue          _AND_
                    rohc_U8bit_t            *p_ack_scaling_factor_estab_counter _AND_
                    rohc_U8bit_t            ack_stride_change      _AND_
                    rohc_U8bit_t            ack_residue_change                         
                    )
{
    rohc_return_t ret_val = ROHC_SUCCESS;
    if((stride!= 0) && (residue == unscaled_val % stride) &&
            (unscaled_val == scaled_val * stride + residue)&&
            !(ack_stride_change) && !(ack_residue_change))

    {
       (*p_ack_scaling_factor_estab_counter)++;
        if(*p_ack_scaling_factor_estab_counter > 2)
        {
            ret_val = ROHC_SUCCESS;
       
        }
        else
        {
    
        ret_val = ROHC_FAILURE;
    }
    }
    else
    {
    *p_ack_scaling_factor_estab_counter = 0;
        ret_val = ROHC_FAILURE;
    }
    return ret_val;
}
            
/*****************************************************************************
**      FUNCTION: rohc_sel_pkt_optlist_rsf_ttl_ecn_change
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function selects pkt when optlist,rsf,ttl or ecn changes
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**              delta_bit_map[IN] Delta bit map for changing fields
**              en_sn   [IN]      Encoded value of sn
**              en_ack   [IN]      Encoded value of ack
**      RETURN VALUE:
**              Rohc packet type
**      NOTES:
**               
*****************************************************************************/
STATIC rohc_void_t rohc_sel_pkt_optlist_rsf_ttl_ecn_change(
        rohc_entity_t           *p_entity,
        rohc_csc_tcp_t          *p_csc, 
        rohc_U32bit_t           sn,
        rohc_U32bit_t           ack,
        rohc_encoded_value_t    *p_en_sn,
        rohc_encoded_value_t    *p_en_ack,
        rohc_U8bit_t            isSNChanged,
        rohc_U8bit_t            isACKChanged,
        rohc_U8bit_t            *p_pkt_typ
        )
{
    /* possible packet types are rnd_8 and common comp. */
    if(((!isSNChanged) && (!isACKChanged))  
            ||(p_csc->delta_field & ROHC_CPRM_CH_TCP_WINDOW))
    {
        *p_pkt_typ = ROHC_CO_P6_COMMON_PKTYPE;
        encode_sn_ack_co_common_pkt(p_entity,p_csc,sn,
                ack,p_en_sn,p_en_ack,isSNChanged,isACKChanged);
    }
    else
    {
        /* encoding for rnd8 pkt type */
        if(isSNChanged)
        {
            if(ROHC_FAILURE == rohc_encode_p6_sn(p_entity, p_csc,
                        sn ,p_en_sn,16,65535))
            {
                *p_pkt_typ = ROHC_CO_P6_COMMON_PKTYPE;
                encode_sn_ack_co_common_pkt(p_entity,p_csc,sn,
                        ack,p_en_sn,p_en_ack,isSNChanged,isACKChanged);

            }
            else
            { 
                *p_pkt_typ = ROHC_CO_P6_RND8_PKTYPE;
            }
        }
        if(isACKChanged && (ROHC_CO_P6_COMMON_PKTYPE != *p_pkt_typ))
        {
            if(ROHC_FAILURE == rohc_encode_p6_ack(p_entity, p_csc,
                        ack ,p_en_ack,16,16383))
            {
                *p_pkt_typ = ROHC_CO_P6_COMMON_PKTYPE;
                encode_sn_ack_co_common_pkt(p_entity,p_csc,sn,
                        ack,p_en_sn,p_en_ack,isSNChanged,isACKChanged);

            }
            else
            { 
                *p_pkt_typ = ROHC_CO_P6_RND8_PKTYPE;
            }
        }
        if(ROHC_CO_P6_RND8_PKTYPE == *p_pkt_typ)
        {
            if(ROHC_FAILURE == rohc_encode_p6_ttl(p_csc->field_value
                        [ROHC_INNERIP4_TTL_FID -ROHC_VAR_FID_BASE],p_csc->old_inner_ip4_ttl,3,3))
            {
                *p_pkt_typ = ROHC_CO_P6_COMMON_PKTYPE;
            }
        }

    }
}

/*****************************************************************************
**      FUNCTION: rohc_sel_pkt_tcp_window_change
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function selects pkt when tcp window changes
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**              delta_bit_map[IN] Delta bit map for changing fields
**              en_sn   [IN]      Encoded value of sn
**              en_ack   [IN]      Encoded value of ack
**      RETURN VALUE:
**              Rohc packet type
**      NOTES:
**               
*****************************************************************************/
STATIC rohc_void_t rohc_sel_pkt_tcp_window_change(
        rohc_entity_t           *p_entity,
        rohc_csc_tcp_t          *p_csc, 
        rohc_U32bit_t           sn,
        rohc_U32bit_t           ack,
        rohc_encoded_value_t    *p_en_sn,
        rohc_encoded_value_t    *p_en_ack,
        rohc_U8bit_t            isSNChanged,
        rohc_U8bit_t            isACKChanged,
        rohc_U8bit_t            *p_pkt_typ
        )
{
    /* possible packet types are rnd_7 and common comp. */
    if(isSNChanged ) 
    {
        *p_pkt_typ = ROHC_CO_P6_COMMON_PKTYPE;
        encode_sn_ack_co_common_pkt(p_entity,p_csc,sn,ack,p_en_sn,
                p_en_ack,isSNChanged,isACKChanged);
    }
    else
    { 
        if(ROHC_FAILURE == rohc_encode_p6_ack(p_entity, p_csc, ack ,p_en_ack,
                    18, 65535 ))
        {
            *p_pkt_typ = ROHC_CO_P6_COMMON_PKTYPE;
            encode_sn_ack_co_common_pkt(p_entity,p_csc,sn,ack,p_en_sn,
                    p_en_ack,isSNChanged,isACKChanged);
        }
        else
        {
            *p_pkt_typ = ROHC_CO_P6_RND7_PKTYPE;
        }
    }
}

/*****************************************************************************
**      FUNCTION: rohc_sel_pkt_tcp_sn_and_ack_change
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function selects pkt when tcp sn and ack changes
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**              delta_bit_map[IN] Delta bit map for changing fields
**              en_sn   [IN]      Encoded value of sn
**              en_ack   [IN]      Encoded value of ack
**      RETURN VALUE:
**              Rohc packet type
**      NOTES:
**               
*****************************************************************************/
STATIC rohc_void_t rohc_sel_pkt_tcp_sn_and_ack_change(
        rohc_entity_t           *p_entity,
        rohc_csc_tcp_t          *p_csc, 
        rohc_U32bit_t           sn,
        rohc_U32bit_t           ack,
        rohc_encoded_value_t    *p_en_sn,
        rohc_encoded_value_t    *p_en_ack,
        rohc_U8bit_t            isSNChanged,
        rohc_U8bit_t            isACKChanged,
        rohc_U8bit_t            *p_pkt_typ,
        rohc_U8bit_t            isSNFieldScaling
        )
{
    /* both sn and ack no are changed- possible pkt types are rnd_5 
     * rnd_6 , rnd_8 and common compressed*/
    if(isSNFieldScaling && (ROHC_SUCCESS == 
                rohc_encode_p6_ack(p_entity, p_csc, ack ,
                    p_en_ack, 16, 16383)))
    {
        *p_pkt_typ = ROHC_CO_P6_RND6_PKTYPE;
    }
    else if(!isSNFieldScaling && (ROHC_SUCCESS == 
                rohc_encode_p6_sn(p_entity, p_csc, sn,
                    p_en_sn, 14, 8191)) && 
            (ROHC_SUCCESS == rohc_encode_p6_ack(p_entity, p_csc, ack,
                                                p_en_ack, 15, 8191)))
    {
        *p_pkt_typ = ROHC_CO_P6_RND5_PKTYPE;
    }
    else if(!isSNFieldScaling && (ROHC_SUCCESS == 
                rohc_encode_p6_sn(p_entity, p_csc, sn,
                    p_en_sn, 16, 65535)) && 
            (ROHC_SUCCESS == rohc_encode_p6_ack(p_entity, p_csc, ack,
                                                p_en_ack, 16, 16383))&&
            (ROHC_SUCCESS == rohc_encode_p6_ttl(p_csc->field_value[
                                                ROHC_INNERIP4_TTL_FID -ROHC_VAR_FID_BASE],
                                                p_csc->old_inner_ip4_ttl, 3,3)))
    {
        *p_pkt_typ = ROHC_CO_P6_RND8_PKTYPE;
    }
    else 
    {
        *p_pkt_typ = ROHC_CO_P6_COMMON_PKTYPE;
        encode_sn_ack_co_common_pkt(p_entity,p_csc,sn,ack,p_en_sn,
                p_en_ack,isSNChanged,isACKChanged);
    }
}

/*****************************************************************************
**      FUNCTION: rohc_sel_pkt_tcp_sn_change
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function selects pkt when tcp sn and ack changes
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**              delta_bit_map[IN] Delta bit map for changing fields
**              en_sn   [IN]      Encoded value of sn
**              en_ack   [IN]      Encoded value of ack
**      RETURN VALUE:
**              Rohc packet type
**      NOTES:
**               
*****************************************************************************/
STATIC rohc_void_t rohc_sel_pkt_tcp_sn_change(
        rohc_entity_t           *p_entity,
        rohc_csc_tcp_t          *p_csc, 
        rohc_U32bit_t           sn,
        rohc_U32bit_t           ack,
        rohc_encoded_value_t    *p_en_sn,
        rohc_encoded_value_t    *p_en_ack,
        rohc_U8bit_t            isSNChanged,
        rohc_U8bit_t            isACKChanged,
        rohc_U8bit_t            *p_pkt_typ,
        rohc_U8bit_t            isSNFieldScaling
        )
{
    /* only SN is changing, possible pkt types are rnd_1  rnd_2 and common
     * comp*/
    if(isSNFieldScaling )
    {
        *p_pkt_typ = ROHC_CO_P6_RND2_PKTYPE;
    }
    else
    {
        if(ROHC_SUCCESS == rohc_encode_p6_sn(p_entity, p_csc, sn,
                    p_en_sn, 18, 65535))
        {
            *p_pkt_typ = ROHC_CO_P6_RND1_PKTYPE;
        }
        else
        {
            *p_pkt_typ = ROHC_CO_P6_COMMON_PKTYPE;
            encode_sn_ack_co_common_pkt(p_entity,p_csc,sn,ack,p_en_sn,
                    p_en_ack,isSNChanged,isACKChanged);
        }
    }
}

/*****************************************************************************
**      FUNCTION: rohc_sel_pkt_tcp_ack_change
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function selects pkt when tcp sn and ack changes
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**              delta_bit_map[IN] Delta bit map for changing fields
**              en_sn   [IN]      Encoded value of sn
**              en_ack   [IN]      Encoded value of ack
**      RETURN VALUE:
**              Rohc packet type
**      NOTES:
**               
*****************************************************************************/
STATIC rohc_void_t rohc_sel_pkt_tcp_ack_change(
        rohc_entity_t           *p_entity,
        rohc_csc_tcp_t          *p_csc, 
        rohc_U32bit_t           sn,
        rohc_U32bit_t           ack,
        rohc_encoded_value_t    *p_en_sn,
        rohc_encoded_value_t    *p_en_ack,
        rohc_U8bit_t            isSNChanged,
        rohc_U8bit_t            isACKChanged,
        rohc_U8bit_t            *p_pkt_typ,
        rohc_U8bit_t            isACKFieldScaling,
        rohc_encoded_value_t    *p_en_scaled_ack,
        rohc_U8bit_t            *p_num_of_co_common_pkts_sent
        )
{
    if(isACKFieldScaling)
    {
        /* means scaling can be done so communicate ack stride to decompressor
         * by sending common compressed pkt */
        if(*p_num_of_co_common_pkts_sent < 2)
        {
            *p_pkt_typ = ROHC_CO_P6_COMMON_PKTYPE;
            encode_sn_ack_co_common_pkt(p_entity,p_csc,sn,ack,p_en_sn,
                    p_en_ack,isSNChanged,isACKChanged);
            p_csc->field_value[ROHC_TCP_ACK_STRIDE_FLAG_FID-ROHC_VAR_FID_BASE]
                = 1;
            (*p_num_of_co_common_pkts_sent)++;
        }
        else
        {
            /* now can send scaled ack, so check if scaled value lies
             * within interpretation interval */
            if(ROHC_SUCCESS == rohc_encode_p6_scaled_ack(p_entity,
                        p_csc, p_csc->field_value[ROHC_TCP_SCALED_ACKNO_FID-
                        ROHC_VAR_FID_BASE] ,p_en_scaled_ack, 
                        4,3 ))
            {
                *p_pkt_typ = ROHC_CO_P6_RND4_PKTYPE;
            }
            else
            {
                if((ROHC_SUCCESS == rohc_encode_p6_ack(p_entity, p_csc, ack,
                                p_en_ack, 15, 8191)))
                {
                    *p_pkt_typ = ROHC_CO_P6_RND3_PKTYPE;
                }
                else
                {
                    *p_pkt_typ = ROHC_CO_P6_COMMON_PKTYPE;
                    encode_sn_ack_co_common_pkt(p_entity,p_csc,sn,ack,p_en_sn,
                            p_en_ack,isSNChanged,isACKChanged);
                }
            }
        }
    }
    else
    {
        *p_num_of_co_common_pkts_sent = 0;
        /* send unscaled ACK */
        if((ROHC_SUCCESS == rohc_encode_p6_ack(p_entity, p_csc, ack,
                        p_en_ack, 15, 8191)))
        {
            *p_pkt_typ = ROHC_CO_P6_RND3_PKTYPE;
        }
        else
        {
            *p_pkt_typ = ROHC_CO_P6_COMMON_PKTYPE;
            encode_sn_ack_co_common_pkt(p_entity,p_csc,sn,ack,p_en_sn,
                    p_en_ack,isSNChanged,isACKChanged);
        }
    }
}

/*****************************************************************************
**      FUNCTION:
**              rohc_check_gen_opt_len
******************************************************************************
**
**      DESCRIPTION:
**              Checks if length of any generic option is changed
**      ARGUMENTS:
**              p_csc   [IN]      Pointer to the CSC block
**      RETURN VALUE:
**              Delta bit map field
**               
*****************************************************************************/
STATIC rohc_U32bit_t rohc_check_gen_opt_len 
     (rohc_csc_tcp_t    *p_csc)
{
    rohc_U32bit_t ret_value = 0;
    rohc_U8bit_t    i, l_index = 0;
    rohc_c_generic_opt_elem_t temp_gen_option;
    rohc_c_generic_opt_elem_t gen_option_context;
    /* structure of list is same but if length of any generic option is changed, it is
     * to be sent as part of list */
    if(p_csc->p_cpib->rohc_c_temp_tcp_opt.rohc_c_generic_opt.next_free_index != 0)
    {
        l_index = p_csc->rohc_c_tcp_opt.rohc_c_generic_opt.
            next_free_index;
        for(i = 0; ((i<l_index) && (i < ROHC_MAX_TCP_GENERIC_OPT)); i++)/*GCC_4.9_FIX*/
        {
            temp_gen_option = p_csc->p_cpib->rohc_c_temp_tcp_opt.
                rohc_c_generic_opt.rohc_generic_opt_elem[i];
            gen_option_context = p_csc->rohc_c_tcp_opt.
                rohc_c_generic_opt.rohc_generic_opt_elem[i];
            if(temp_gen_option.length != gen_option_context.
                    length)
            {
                p_csc->ctrl_field_val_p6[ROHC_TCP_LIST_PRESENT_FID-
                    ROHC_PROF6_VAR_CTRL_FID_BASE] = 1;
                ret_value |= ROHC_CPRM_CH_TCP_OPT_LIST;
                break;

            }
        }
    }
    return ret_value;
}
