/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_d_parse_rohc.c,v 1.6.6.7 2010/11/26 05:36:23 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : DeCompressor Packet Parser module Implementation
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_d_parse_rohc.c,v $
 * Revision 1.6.6.7  2010/11/26 05:36:23  gur22059
 * Use of register variables for profile 1
 *
 * Revision 1.6.6.6  2010/11/24 03:43:30  gur22059
 * Removal of conditional checks w.r.t profile1
 *
 * Revision 1.6.6.5  2010/11/09 05:23:09  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.6.6.4  2010/10/13 04:18:13  gur22059
 * Remove Warnings
 *
 * Revision 1.6.6.3  2010/10/13 03:55:44  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.6.6.2  2010/10/01 14:07:21  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.6.6.1  2010/07/07 11:36:07  gur21145
 * PROFILE2_comments_and PROFILE0_comments_inc
 *
 * Revision 1.6  2010/06/23 07:06:45  gur21145
 * .
 *
 * Revision 1.5  2009/05/28 13:50:29  gur19140
 * *** empty log message ***
 *
 * Revision 1.4  2009/05/28 03:43:19  gur19836
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
#include "rohc_d_parse_rohc.h"
#include "rohc_ed_ext_if.h"
#include "rohc_crc_ext_if.h"
#include "rohc_dcpm_ext_if.h"
#include "rohc_stats_def.h"
#include "rohc_opt_proto.h"
#ifdef PERF_STATS 
 /* + PERF_CA */
#include "ltePdcpContext.h"
 /* - PERF_CA */
#endif

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static void rohcCalCommCompPktVarlenFieldSize( 
            rohc_U8bit_t   field_id,
            rohc_U8bit_t * no_of_bits , 
            rohc_dsc_tcp_t* p_dsc);

static void rohcParseSackIrregularBlock(
            rohc_dsc_tcp_t           *p_dsc ,
            rohc_U8bit_t            **p_rohc_pkt  ,
            rohc_U8bit_t             field_id);
static void rohcParseIrregularChain( 
                rohc_dsc_tcp_t          *p_dsc,
                rohc_U8bit_t            **p_rohc_pkt ,
                rohc_U8bit_t            XI,
                rohc_U8bit_t            *pLen);
static void rohcParseSackIregularChain(
                rohc_dsc_tcp_t          *p_dsc,
                rohc_U8bit_t            **p_rohc_pkt);
rohc_void_t rohcParseCompressedHeaderChain_p6 (
                rohc_dsc_tcp_t      *p_dsc ,
                rohc_U8bit_t        field_id ,
                rohc_U8bit_t        **p_p_pkt );

static void rohcGetGenericOptionLen ( 
            rohc_dsc_tcp_t          *p_dsc ,
            rohc_U8bit_t            *pRohcGenricOptionListVar );

STATIC rohc_U8bit_t  rohcP6GetIrregChainType(
    rohc_dsc_tcp_t          * p_dsc ,
    rohc_U8bit_t            *p_next_header);

STATIC rohc_void_t  rohcP6DecodeIPIrregChain(
    rohc_dsc_tcp_t          * p_dsc ,
    rohc_U8bit_t            irregular_chain_type,
    register rohc_U8bit_t   pkt_type ,
    rohc_U8bit_t            **p_rohc_pkt);

STATIC rohc_void_t  rohcP6DecodeTCPIrregChain(
    rohc_dsc_tcp_t          * p_dsc  ,
    register rohc_U8bit_t   pkt_type ,
    rohc_U8bit_t            ** p_rohc_pkt );

STATIC rohc_void_t  rohcP6DecodeTCPOptionsIrregChain(
    rohc_dsc_tcp_t          * p_dsc ,
    rohc_U8bit_t            ** p_rohc_pkt );

STATIC rohc_void_t rohc_parse_p6_tcp_replicate_partial_chain(
            rohc_dsc_tcp_t  *p_dsc ,
            rohc_U8bit_t    *p_pkt ,
            rohc_U8bit_t    *p_len );

STATIC rohc_return_t
rohc_get_pkt_type(
            struct rohc_entity_t * p_entity,
            rohc_U16bit_t cid,
            rohc_dsc_common_t    **  p_p_dsc,
            rohc_dpib_t          *p_dpib,
            rohc_U8bit_t         *pkt_type,
            rohc_U8bit_t         * p_pkt,
            rohc_U8bit_t         cid_len,
            rohc_error_code_t    *p_ecode);
/* + SPR 17439 */
rohc_U8bit_t 
rohc_inter_r0_ptype_func
                (rohc_U8bit_t    *p_pkt ,
                rohc_U8bit_t    cid_len);
rohc_U8bit_t 
rohc_inter_r0crc_ptype_func
                (rohc_U8bit_t    *p_pkt ,
                rohc_U8bit_t    cid_len);
rohc_U8bit_t 
rohc_inter_uo0_ptype_func
                (rohc_U8bit_t    *p_pkt ,
                rohc_U8bit_t    cid_len);
rohc_U8bit_t 
rohc_inter_r1_ptype_func
                (rohc_U8bit_t    *p_pkt, 
                rohc_U8bit_t    cid_len);
rohc_U8bit_t 
rohc_inter_r1idts_ptype_func
                (rohc_U8bit_t    *p_pkt,
                rohc_U8bit_t    cid_len);
rohc_U8bit_t 
rohc_inter_uo1_ptype_func
                (rohc_U8bit_t    *p_pkt ,
                rohc_U8bit_t    cid_len);
rohc_U8bit_t 
rohc_inter_uo1idts_ptype_func
                (rohc_U8bit_t    *p_pkt ,
                rohc_U8bit_t    cid_len);
rohc_U8bit_t 
rohc_inter_uor2_ptype_func
                (rohc_U8bit_t    *p_pkt ,
                rohc_U8bit_t    cid_len);
rohc_U8bit_t
rohc_inter_uor2idts_ptype_func
                (rohc_U8bit_t    *p_pkt ,
                rohc_U8bit_t    cid_len);
STATIC rohc_U32bit_t 
rohc_parse_p1_static_chain
            (rohc_U8bit_t    *p_pkt ,
            rohc_dsc_rtp_t  *p_dsc);
STATIC rohc_U32bit_t 
rohc_parse_p2_static_chain
            (rohc_U8bit_t    *p_pkt ,
            rohc_dsc_udp_t  *p_dsc);
STATIC rohc_return_t 
rohc_parse_p1_dynamic_chain
            (rohc_U8bit_t        *p_pkt     ,
            rohc_dsc_rtp_t      *p_dsc      ,
            rohc_entity_t       * p_entity  ,
            rohc_U32bit_t       *p_len      ,
            rohc_error_code_t   *p_ecode);
STATIC rohc_return_t 
rohc_parse_p2_dynamic_chain
            (rohc_U8bit_t    *p_pkt ,
            rohc_dsc_udp_t  *p_dsc ,
            rohc_entity_t    * p_entity ,
            rohc_U32bit_t *p_len,
            rohc_error_code_t   *p_ecode);
STATIC rohc_return_t 
parse_ir_irdyn_pkts
        (rohc_entity_t       *p_entity ,
        rohc_U8bit_t        pkt_type ,
        rohc_U8bit_t        *p_pkt ,
        rohc_dsc_common_t   *p_dsc ,
        rohc_error_code_t   *p_ecode);
static rohc_return_t 
parse_ir_cr_pkts
        (rohc_entity_t       *p_entity ,
        rohc_U8bit_t        pkt_type,
        rohc_U8bit_t        *p_pkt ,
        rohc_dsc_common_t   *p_dsc ,
        rohc_error_code_t   *p_ecode);
static rohc_return_t 
parse_ir_cr_pkts
        (rohc_entity_t       *p_entity ,
        rohc_U8bit_t        pkt_type ,
        rohc_U8bit_t        *p_pkt ,
        rohc_dsc_common_t   *p_dsc ,
        rohc_error_code_t   *p_ecode);
/* - SPR 17439 */
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* +- SPR 17777 */
extern tickType_t pdcpCurrentTime_g; 
extern  rohc_U32bit_t rohc_global_pkt_types_to_stats_pr[ROHC_MAX_PKT_TYPES];
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
static rohc_U8bit_t 
    rohc_inter_ptypes[7][ROHC_MAX_MODE_TYPES][ROHC_MAX_RND_TYPES]
        [ROHC_MAX_RND_TYPES];

static rohc_U8bit_t 
    rohc_inter_ptypes_p23[7][ROHC_MAX_MODE_TYPES];

static rohc_funcptr_ptypes_t    rohc_funcptr_ptypes[ROHC_INTER_MAX_PTYPES];

rohc_void_t (*rohc_create_dsc_sub_blocks_fpt [ROHC_ALL_PROFILE - ROHC_UNCOMP])
                                          (
                                           /* + SPR 17439 */
                                          rohc_dsc_common_t    *p_p_dsc
                                          /* - SPR 17439 */
                                          ) = 
{NULL, rohc_db_p1_create_dsc_sub_blocks, rohc_db_p2_create_dsc_sub_blocks, NULL, 
    NULL,NULL,rohc_db_p6_create_dsc_sub_blocks}; 

rohc_return_t (*rohc_d_decode_value_fpt [ROHC_ALL_PROFILE - ROHC_UNCOMP])
                               ROHC_PROTO((
                                          /* + SPR 17439 */
                                          rohc_dsc_common_t       *p_p_dsc,
                                          /* - SPR 17439 */
                                          rohc_U8bit_t      * p_pkt,
                                          rohc_U8bit_t      pkt_type,
                                          rohc_U32bit_t     *p_len,
                                          rohc_entity_t     * p_entity,
                                          rohc_error_code_t *p_ecode
                                          )) = 
{NULL, rohc_d_decode_p1_value, rohc_d_decode_p2_value, NULL,NULL,NULL,
    rohc_d_DecodeP6Value}; 


/****************************************************************************
 * Name     :   rohcGetPktTypeProfile6
 * Desc     :   it determines the packet type of recieved packet for profile 6
 * Params   :   Pointer to ROHC packet
            :   Pointer to DSC
 * Returns  : Type of the packet
 **************************************************************************/
static rohc_U8bit_t 
rohcGetPktTypeProfile6(
        rohc_U8bit_t           *p_pkt,
        rohc_dsc_common_t      *p_dsc )
{
    ROHC_ENTER_FN("rohcGetPktTypeProfile6");

    if (ROHC_D_ST_FC == p_dsc->d_state)
    {
        /* First four bits of packet determine the discriminator of 
         * packet which define its packet type */
        switch(*p_pkt&0xf0) 
        {

            /* Discriminator = 0xA0 for SEQ1 and RND6 type packet */
            case ROHC_P6_RND6_SEQ1_MASK:                            
                /* ipid_beh_inner of common compressed packet determines type 
                 * of behavior of received packet */
                if ((IP_ID_BEHAVIOR_RANDOM == p_dsc->ipid_beh_inner) ||
                        (IP_ID_BEHAVIOR_ZERO == p_dsc->ipid_beh_inner))
                {
                    /* RND 6 Compressed packet */
                    return ROHC_CO_P6_RND6_PKTYPE;
                }
                else
                {
                    /* SEQ 1 Compressed packet */
                    return ROHC_CO_P6_SEQ1_PKTYPE;
                }

                /* Discriminator = 0xB0 for SEQ8 and first 4 nibbles are 0x1011 
                 * for RND1, RND7 and RND8 type packet */
            case ROHC_P6_RND_1_7_8_SEQ8_MASK: 
                /* ipid_beh_inner of common compressed packet determines type 
                 * of behavior of received packet */
                if ((IP_ID_BEHAVIOR_RANDOM == p_dsc->ipid_beh_inner) ||
                        (IP_ID_BEHAVIOR_ZERO == p_dsc->ipid_beh_inner))
                {
                    /* Discriminator = 101111 for RND7 packet type */
                    if ( 0xBC == (*p_pkt & 0xFC))       
                    {
                        return ROHC_CO_P6_RND7_PKTYPE;
                    }
                    else
                    {
                        if ( 0xB8 == (*p_pkt & 0xF8))   
                        {
                            /* Discriminator = 101110 for RND1 packet type */
                            return ROHC_CO_P6_RND1_PKTYPE;
                        }
                        else
                        {
                            /* Discriminator = 10110 for RND8 packet type */
                            return ROHC_CO_P6_RND8_PKTYPE; 
                        }
                    }
                }
                else
                {
                    /* Discriminator = 0xB0 for SEQ8 packet type */
                    return ROHC_CO_P6_SEQ8_PKTYPE; 
                }

                /* Discriminator = 0xC0 for RND2 and SEQ7 packet type */
            case ROHC_P6_RND2_SEQ7_MASK: 
                /* ipid_beh_inner of common compressed packet determines
                 * type of behavior of received packet */
                if ((IP_ID_BEHAVIOR_RANDOM == p_dsc->ipid_beh_inner) ||
                        (IP_ID_BEHAVIOR_ZERO == p_dsc->ipid_beh_inner))
                {
                    /* RND 2 Compressed packet */
                    return ROHC_CO_P6_RND2_PKTYPE;
                }
                else
                {
                    /* SEQ 7 Compressed packet */
                    return ROHC_CO_P6_SEQ7_PKTYPE;  
                }

                /* Discriminator = 0xD0 for RND4 and first 4 nibbles are
                 * 1101 for SEQ2 packet type */
            case ROHC_P6_RND4_SEQ_2_6_MASK: 
                if ((IP_ID_BEHAVIOR_RANDOM == p_dsc->ipid_beh_inner) ||
                        (IP_ID_BEHAVIOR_ZERO == p_dsc->ipid_beh_inner))
                {
                    /* RND 4 Compressed packet */
                    return ROHC_CO_P6_RND4_PKTYPE;
                }
                else 
                {
                    if ( 0xD8 == (*p_pkt & 0xF8))    
                    {
                        /* Discriminator = 11011 for SEQ6 packet type */
                        return ROHC_CO_P6_SEQ6_PKTYPE;
                    }
                    else
                    {
                        /* Discriminator = 11010 for SEQ2 packet type */
                        return ROHC_CO_P6_SEQ2_PKTYPE;     
                    }
                }

                /* Discriminator = 0x80 for SEQ5 and first 3 nibbles are 
                 * 100 for RND5 and SEQ3 packet type */    
            case ROHC_P6_RND5_SEQ_5_MASK: 
                if ((IP_ID_BEHAVIOR_RANDOM == p_dsc->ipid_beh_inner) ||
                        (IP_ID_BEHAVIOR_ZERO == p_dsc->ipid_beh_inner))
		{
			/* Discriminator = 100 for RND5 */
			return ROHC_CO_P6_RND5_PKTYPE;    
		}
		else
		{
			/* SEQ 5 Compressed packet */
			return ROHC_CO_P6_SEQ5_PKTYPE;
		}
                /* Discriminator can be 0x90 for RND5 and SEQ3 
                 */    
	    case ROHC_P6_RND5_SEQ_3_MASK:
		if ((IP_ID_BEHAVIOR_RANDOM == p_dsc->ipid_beh_inner) ||
                        (IP_ID_BEHAVIOR_ZERO == p_dsc->ipid_beh_inner))
		{
			/* Discriminator = 100 for RND5 */
			return ROHC_CO_P6_RND5_PKTYPE;    
		}
		else
		{
                        /* Discriminator = 1001 for SEQ3 */
                        return ROHC_CO_P6_SEQ3_PKTYPE; 
		}
                /* Discriminator = 0 for RND3 and SEQ4 packet type */
            default: 
                if (0 == (*p_pkt & 0x80))
                {
                    if ((IP_ID_BEHAVIOR_RANDOM == p_dsc->ipid_beh_inner) ||
                            (IP_ID_BEHAVIOR_ZERO == p_dsc->ipid_beh_inner))
                    {
                        /* RND 3 Compressed packet */
                        return ROHC_CO_P6_RND3_PKTYPE;    
                    }
                    else
                    {
                        /* SEQ 4 Compressed packet */
                        return ROHC_CO_P6_SEQ4_PKTYPE;
                    }
                }
        }
    }
    ROHC_EXIT_FN("rohcGetPktTypeProfile6");
    return ROHC_MAX_PKT_TYPES;
}
/****************************************************************************
 * Name     :   rohc_packet_type_det
 * Desc     :   it determines the packet type of recieved packet
 * Params   :   Pointer to ROHC entity
            :   Pointer to ROHC packet
            :   CID length
            :   Pointer to DSC
 * Returns  :   Type of the packet
 **************************************************************************/
STATIC rohc_U8bit_t 
rohc_packet_type_det(
        rohc_entity_t          *p_entity,
        rohc_U8bit_t           *p_pkt,
        register rohc_U8bit_t  cid_len,
        rohc_dsc_common_t      *p_dsc )
{
    rohc_U8bit_t pkt_type,rnd=ROHC_RND_INVALID,rnd2=ROHC_RND_INVALID;

    register rohc_U8bit_t profile;

    ROHC_ENTER_FN("rohc_packet_type_det");
    if(ROHC_IRDYN_RES_TYPE == *p_pkt) /*0xf8 */
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,0,
            0,0,0,0, __func__,"");
        /* +- SPR 17777 */
        ROHC_LOG(LOG_INFO,"DPPM :EID[%u:%u]: CID[%u]:"\
                    "IR DYN Packet found",p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,p_dsc->cid);
                    
        ROHC_EXIT_FN("rohc_packet_type_det");
        return ROHC_IRDYN_PKTTYPE;
    }

    /* 0xfc */
    if( ROHC_IR_RES_TYPE == (*p_pkt & (ROHC_IR_RES_OR_IR_CO_RES_TYPE)))
    {
        if(ROHC_UNCOMP == (*(p_pkt+cid_len+1)))
        {
            ROHC_LOG(LOG_INFO,"DPPM :EID[%u:%u]: CID[%u]:"\
                    "IR P0 Packet found",p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,
                    (p_dsc)?p_dsc->cid:0);
            ROHC_EXIT_FN("rohc_packet_type_det");
            return ROHC_IRP0_PKTYPE;
        }
        else
        {
            if(ROHC_IP_TCP == *(p_pkt+cid_len+1)) /*Profile 6*/
            {
                if (*p_pkt&0x01)
                {
                    ROHC_LOG(LOG_INFO,"DPPM :EID[%u:%u]:CID[%u]:"\
                            "ROHC_IR_WITH_DYN_PKTTYPE Packet found",
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId,(p_dsc)?p_dsc->cid:0);
                    ROHC_EXIT_FN("rohc_packet_type_det");
                    return ROHC_IR_WITH_DYN_PKTTYPE;
                }
                else
                {
                    ROHC_LOG(LOG_INFO,"DPPM :EID[%u:%u]:CID[%u]:"\
                            "ROHC_IRP6CR_PKTYPE Packet found",
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId,(p_dsc)?p_dsc->cid:0);
                    ROHC_EXIT_FN("rohc_packet_type_det");                    
                    return ROHC_IRP6CR_PKTYPE ;
                }
            }
            else
            {
                if (*p_pkt&0x01)
                {
                    ROHC_LOG(LOG_INFO,"DPPM :EID[%u:%u]:CID[%u]:"\
                            "ROHC_IR_WITH_DYN_PKTTYPE Packet found",
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId,(p_dsc)?p_dsc->cid:0);
                    ROHC_EXIT_FN("rohc_packet_type_det");
                    return ROHC_IR_WITH_DYN_PKTTYPE;
                }
                else
                {
                    ROHC_LOG(LOG_INFO,"DPPM :EID[%u:%u]:CID[%u]:"\
                            "ROHC_IR_WITHOUT_DYN_PKTTYPE Packet found",
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId,(p_dsc)?p_dsc->cid:0);
                    ROHC_EXIT_FN("rohc_packet_type_det");
                    return ROHC_IR_WITHOUT_DYN_PKTTYPE;
                }
            }
        }
    }

    if((*p_pkt & ROHC_IR_CO_RES_TYPE) == ROHC_IR_CO_RES_TYPE) /* 0xfa */
    {
        ROHC_LOG(LOG_INFO,"DPPM :EID[%u:%u]: CID[%u]:"\
                "Compressed Packet found",p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId,
                (p_dsc)?p_dsc->cid:0);
        ROHC_EXIT_FN("rohc_packet_type_det");
        return ROHC_CO_P6_COMMON_PKTYPE;
    }
    /* coverity 54516 */
    if ( NULL != p_dsc  )
    {
        profile = p_dsc->profile;
        if (ROHC_UNCOMP == profile )
        {
            ROHC_LOG(LOG_INFO,"DPPM :EID[%u:%u]: CID[%u]:"\
                "ROHC_NORMALP0_PKTYPE Packet found",
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId,p_dsc->cid);
            ROHC_EXIT_FN("rohc_packet_type_det");
            return ROHC_NORMALP0_PKTYPE;
        }

        if ( ROHC_IP_UDP_RTP == profile )
        {
            if(6 == ROHC_GET_3MSB(*p_pkt))
            {
                if(p_dsc->inner_ip4) 
                {
                    if((*(p_pkt+cid_len+2) & 0x80) &&
                        (3 == ROHC_GET_2MSB(*(p_pkt+cid_len+3))) &&
                        (*(p_pkt+cid_len+3) & ROHC_ENC_INNERIPFLAG_EXT3FLAGP1_MASK))
                    {
                        rnd = *(p_pkt+cid_len+4) & ROHC_ENC_RND_INNERIPFLAGP1_MASK;
                    }   
                    else
                    {
                        rnd = p_dsc->rnd;
                    }
                }
                if(p_dsc->outer_ip4)
                {
                    if((*(p_pkt+cid_len+2) & 0x80) &&
                        (3 == ROHC_GET_2MSB(*(p_pkt+cid_len+3))) &&
                        (*(p_pkt+cid_len+3) & ROHC_ENC_INNERIPFLAG_EXT3FLAGP1_MASK)&&
                        (*(p_pkt+cid_len+4) & ROHC_ENC_IP2_INNERIPFLAGP1_MASK))
                    {
                        rnd2 = *(p_pkt+cid_len+5) & ROHC_ENC_RND_OUTERIPFLAGP1_MASK;
                    }
                    else
                    {
                        rnd2 = p_dsc->rnd2;
                    }
                }
                ROHC_EXIT_FN("rohc_packet_type_det");
                return rohc_funcptr_ptypes[rohc_inter_ptypes[ROHC_GET_3MSB(*p_pkt)]
                                                [p_dsc->d_mode]
                                                [rnd]
                                                [rnd2]]
                                                (p_pkt,cid_len);
            }
            else
            {
                ROHC_EXIT_FN("rohc_packet_type_det");
                return rohc_funcptr_ptypes[rohc_inter_ptypes[ROHC_GET_3MSB(*p_pkt)]
                                                [p_dsc->d_mode]
                                                [p_dsc->rnd]
                                                [p_dsc->rnd2]]
                                                (p_pkt,cid_len);
            }
        }
        else
        {
            if (ROHC_IP_TCP == profile)
            {
                return  rohcGetPktTypeProfile6(p_pkt,p_dsc);
            }
            else
            {
                pkt_type = rohc_inter_ptypes_p23[ROHC_GET_3MSB(*p_pkt)]
                                                [p_dsc->d_mode];
                ROHC_EXIT_FN("rohc_packet_type_det");
                if(ROHC_R1P23_PKTTYPE == pkt_type )
                {
                    if(*(p_pkt+cid_len+1)& 0x80)
                    {
                        switch(ROHC_GET_2MSB(*(p_pkt+cid_len+2)))
                        {
                            case 0: return ROHC_R1E0P23_PKTTYPE;
                            case 1: return ROHC_R1E1P23_PKTTYPE;
                            case 2: return ROHC_R1E2P23_PKTTYPE;
                            case 3: return ROHC_R1E3P23_PKTTYPE;
                        }
                    }
                    else
                    {
                        return ROHC_R1P23_PKTTYPE;
                    }
                }
                else if( ROHC_UOR2P23_PKTTYPE == pkt_type )
                {
                    if(*(p_pkt+cid_len+1)& 0x80)
                    {
                        switch(ROHC_GET_2MSB(*(p_pkt+cid_len+2)))
                        {
                            case 0: return ROHC_UOR2E0P23_PKTTYPE;
                            case 1: return ROHC_UOR2E1P23_PKTTYPE;
                            case 2: return ROHC_UOR2E2P23_PKTTYPE;
                            case 3: return ROHC_UOR2E3P23_PKTTYPE;
                        }
                    }
                    else
                    {
                        return ROHC_UOR2P23_PKTTYPE;
                    }
                }
            /*else
              return pkt_type;
        */
            }
        }
    }
    else
    {
       pkt_type = ROHC_MAX_PKT_TYPES; 
    }    
    /* coverity 54516 */
    /* to remove the warning it has been shifted to down*/
    return pkt_type;
}

/****************************************************************************
 * Name     :   rohc_parse_p1_static_chain
 * Desc     :   It parses the static chain of the IR packets
 * Params   :   Pointer to ROHC packet
            :   Pointer to DSC
 * Returns  :   Parsed Length
 **************************************************************************/
STATIC rohc_U32bit_t 
rohc_parse_p1_static_chain
    ROHC_PARAMS((p_pkt,p_dsc),
            rohc_U8bit_t    *p_pkt _AND_
            rohc_dsc_rtp_t  *p_dsc)
{
    rohc_U8bit_t temp_proto;
    rohc_U8bit_t *p_pkt_org = p_pkt;

    ROHC_ENTER_FN("rohc_parse_p1_static_chain");
    if( *p_pkt == 0x40 ) /* ip v4 packet */
    {
        temp_proto = p_pkt[1];
        if( (temp_proto == ROHC_IPPROTO_IP4) || 
                ( temp_proto == ROHC_IPPROTO_IP6) )
        {
            p_dsc->dpib->outer_ip4_static = (rohc_ip4_static_t*)p_pkt;
            p_pkt+= sizeof(rohc_ip4_static_t);
            if( *p_pkt == 0x40 )
            {
                p_dsc->dpib->inner_ip4_static = (rohc_ip4_static_t*)p_pkt;
                p_pkt+= sizeof(rohc_ip4_static_t);
            }
            else
            {
                p_dsc->dpib->inner_ip6_static = (rohc_ip6_static_t*)p_pkt;
                p_pkt+= sizeof(rohc_ip6_static_t);
            }
        }
        else /* ip v6 packet */
        {
            p_dsc->dpib->inner_ip4_static = (rohc_ip4_static_t*)p_pkt;
            p_pkt+= sizeof(rohc_ip4_static_t);
        }
    }
    else
    {
        temp_proto =  ((rohc_ip6_static_t*)p_pkt)->next_hdr;
        if( (temp_proto  == ROHC_IPPROTO_IP4) || 
                ( temp_proto == ROHC_IPPROTO_IP6) )
        {
            p_dsc->dpib->outer_ip6_static = (rohc_ip6_static_t*)p_pkt;
            p_pkt+= sizeof(rohc_ip6_static_t);
            if( *p_pkt == 0x40 )
            {
                p_dsc->dpib->inner_ip4_static = (rohc_ip4_static_t*)p_pkt;
                p_pkt+= sizeof(rohc_ip4_static_t);
            }
            else
            {
                p_dsc->dpib->inner_ip6_static = (rohc_ip6_static_t*)p_pkt;
                p_pkt+= sizeof(rohc_ip6_static_t);
            }
        }
        else /* ip v6 packet */
        {
            p_dsc->dpib->inner_ip6_static = (rohc_ip6_static_t*)p_pkt;
            p_pkt+= sizeof(rohc_ip6_static_t);
        }
    }

    p_dsc->dpib->udp_static = (rohc_udp_static_t*)p_pkt;
    p_pkt+= sizeof(rohc_udp_static_t);
    p_dsc->dpib->rtp_static = (rohc_rtp_static_t*)p_pkt;
    p_pkt+= sizeof(rohc_rtp_static_t);
    ROHC_EXIT_FN("rohc_parse_p1_static_chain");
    return (p_pkt - p_pkt_org);
}

/****************************************************************************
 * Name     :   rohc_parse_p2_static_chain
 * Desc     :   It parses the static chain of the IR packets
 * Params   :   Pointer to ROHC packet
            :   Pointer to DSC
 * Returns  :   Parsed Length
 **************************************************************************/
STATIC rohc_U32bit_t 
rohc_parse_p2_static_chain
    ROHC_PARAMS((p_pkt,p_dsc),
            rohc_U8bit_t    *p_pkt _AND_
            rohc_dsc_udp_t  *p_dsc)
{
    rohc_U8bit_t temp_proto;
    rohc_U8bit_t *p_pkt_org = p_pkt;

    ROHC_ENTER_FN("rohc_parse_p2_static_chain");
    if( *p_pkt == 0x40 ) /* ip v4 packet */
    {
        temp_proto = p_pkt[1];
        if( (temp_proto == ROHC_IPPROTO_IP4) || 
                ( temp_proto == ROHC_IPPROTO_IP6) )
        {
            p_dsc->dpib->outer_ip4_static = (rohc_ip4_static_t*)p_pkt;
            p_pkt+= sizeof(rohc_ip4_static_t);
            if( *p_pkt == 0x40 )
            {
                p_dsc->dpib->inner_ip4_static = (rohc_ip4_static_t*)p_pkt;
                p_pkt+= sizeof(rohc_ip4_static_t);
            }
            else
            {
                p_dsc->dpib->inner_ip6_static = (rohc_ip6_static_t*)p_pkt;
                p_pkt+= sizeof(rohc_ip6_static_t);
            }
        }
        else /* ip v6 packet */
        {
            p_dsc->dpib->inner_ip4_static = (rohc_ip4_static_t*)p_pkt;
            p_pkt+= sizeof(rohc_ip4_static_t);
        }
    }
    else
    {
        temp_proto =  ((rohc_ip6_static_t*)p_pkt)->next_hdr;
        if( (temp_proto  == ROHC_IPPROTO_IP4) || 
                ( temp_proto == ROHC_IPPROTO_IP6) )
        {
            p_dsc->dpib->outer_ip6_static = (rohc_ip6_static_t*)p_pkt;
            p_pkt+= sizeof(rohc_ip6_static_t);
            if( *p_pkt == 0x40 )
            {
                p_dsc->dpib->inner_ip4_static = (rohc_ip4_static_t*)p_pkt;
                p_pkt+= sizeof(rohc_ip4_static_t);
            }
            else
            {
                p_dsc->dpib->inner_ip6_static = (rohc_ip6_static_t*)p_pkt;
                p_pkt+= sizeof(rohc_ip6_static_t);
            }
        }
        else /* ip v6 packet */
        {
            p_dsc->dpib->inner_ip6_static = (rohc_ip6_static_t*)p_pkt;
            p_pkt+= sizeof(rohc_ip6_static_t);
        }
    }
    p_dsc->dpib->udp_static = (rohc_udp_static_t*)p_pkt;
    p_pkt+= sizeof(rohc_udp_static_t);
    ROHC_EXIT_FN("rohc_parse_p2_static_chain");
    return (p_pkt - p_pkt_org);
}

/****************************************************************************
 * Name     :   rohcParseP6StaticChain(
 * Desc     :   It parses the static chain of the IR packets
 * Params   :   Pointer to ROHC packet
            :   Pointer to DSC
 * Returns  : 	Parsed Length
 **************************************************************************/
static rohc_U32bit_t rohcParseP6StaticChain(
            rohc_U8bit_t    *p_pkt ,
            rohc_dsc_tcp_t  *p_dsc)
{
    rohc_U8bit_t temp_proto;
    rohc_U8bit_t *p_pkt_org = p_pkt;

    ROHC_ENTER_FN("rohcParseP6StaticChain");
    if( 0x00 == *p_pkt ) /* ip v4 packet */
    {
        temp_proto = p_pkt[1];
        if( ( ROHC_IPPROTO_IP4 == temp_proto) || 
                ( ROHC_IPPROTO_IP6 == temp_proto) )
        {
            p_dsc->dpib->outer_ip4_static = (rohc_ip4_static_t*)p_pkt;
            p_pkt+= sizeof(rohc_ip4_static_t);
            if( 0x00 == *p_pkt )
            {
                /*Tunneling ,with OUTER IP4 and INNER IP4 header*/
                p_dsc->dpib->inner_ip4_static = (rohc_ip4_static_t*)p_pkt;
                p_pkt+= sizeof(rohc_ip4_static_t);
            }
            else
            {
                /*Tunneling ,with OUTER IP4 and INNER IP6 header*/
                p_dsc->dpib->inner_ip4_static = (rohc_ip4_static_t*)p_pkt;
                p_dsc->dpib->inner_ip6_static = (rohc_ip6_static_t*)p_pkt;
                p_pkt+= sizeof(rohc_ip6_static_t);
            }
        }
        else /* ip v6 packet */
        {
            /*NO Tunneling ,ONLY IP4 header*/
            p_dsc->dpib->inner_ip4_static = (rohc_ip4_static_t*)p_pkt;
            p_pkt+= sizeof(rohc_ip4_static_t);
        }
    }
    else
    {
        temp_proto =  ((rohc_ip6_static_t*)p_pkt)->next_hdr;
        if( ( ROHC_IPPROTO_IP4 == temp_proto) || 
                ( ROHC_IPPROTO_IP6 == temp_proto) )
        {
            p_dsc->dpib->outer_ip6_static = (rohc_ip6_static_t*)p_pkt;
            p_pkt+= sizeof(rohc_ip6_static_t);
            if( 0x00 == *p_pkt )
            {
                /*Tunneling ,with OUTER IP6 and INNER IP4 header*/
                p_dsc->dpib->inner_ip4_static = (rohc_ip4_static_t*)p_pkt;
                p_pkt+= sizeof(rohc_ip4_static_t);
            }
            else
            {
                /*Tunneling ,with OUTER IP6 and INNER IP6 header*/
                p_dsc->dpib->inner_ip6_static = (rohc_ip6_static_t*)p_pkt;
                p_pkt+= sizeof(rohc_ip6_static_t);
            }
        }
        else /* ip v6 packet */
        {
            /*NO Tunneling ,ONLY IP6 header*/
            p_dsc->dpib->inner_ip6_static = (rohc_ip6_static_t*)p_pkt;
            p_pkt+= sizeof(rohc_ip6_static_t);
        }
    }
    /*Static TCP Chain*/
    p_dsc->dpib->tcp_static = (rohc_tcp_static_t*)p_pkt;
    p_pkt+= sizeof(rohc_tcp_static_t);
    ROHC_EXIT_FN("rohcParseP6StaticChain");
    return (p_pkt - p_pkt_org);
}


/****************************************************************************
 * Name     :   rohc_parse_p6_replicate_chain
 * Desc     :   It parses the Replicate chain of the IR CR packets
 * Params   :   Pointer to ROHC packet
            :   Pointer to DSC
 * Returns  : 	Parsed Length
 **************************************************************************/
static rohc_U32bit_t rohc_parse_p6_Replicate_chain(
            rohc_U8bit_t    *p_pkt ,
            rohc_dsc_tcp_t  *p_dsc)
{
    rohc_U8bit_t *p_pkt_org = p_pkt;
    rohc_U8bit_t xItems = 0;
    rohc_U8bit_t mbit = 0;
    rohc_U8bit_t sizeItems = 0;
    rohc_U8bit_t index = 0;
    rohc_U8bit_t len = 0;

    ROHC_ENTER_FN("rohc_parse_p6_Replicate_chain");
    
    p_dsc->dpib->ip4_replicate_partial = 
        (rohc_ip4_prof6_replicate_partial_t *)p_pkt;
    p_pkt+= sizeof(rohc_ip4_prof6_replicate_partial_t);

    /*Coverity Fix 42161 Starts*/
    if(IP_ID_BEHAVIOR_ZERO != (p_dsc->dpib->ip4_replicate_partial->
        ipidbeh_ttlflag_df & ROHC_PROF6_IP4_REP_IPIDBEH_MASK)>>2)
    /*Coverity Fix 42161 Stops*/
    {
        p_dsc->dpib->ip4_replicate_ipid = 
            (rohc_ip4_prof6_replicate_ip_id_t *)p_pkt;
        p_pkt+= sizeof(rohc_ip4_prof6_replicate_ip_id_t);
    }

    if(p_dsc->dpib->ip4_replicate_partial->ipidbeh_ttlflag_df & 
        ROHC_PROF6_IP4_REP_TTLFLAG_MASK)
    {
        p_dsc->dpib->ip4_replicate_ttlhopl = 
            (rohc_ip4_prof6_replicate_ttlhopl_t *)p_pkt;
        p_pkt+= sizeof(rohc_ip4_prof6_replicate_ttlhopl_t);
    }
    rohc_parse_p6_tcp_replicate_partial_chain(p_dsc,p_pkt,&len);
    p_pkt += len;

    /*In case List Present Flag is set TCP OPTIONS list will be
    present and need to be parsed*/
    if(p_dsc->dpib->tcp_replicate_partial->winp_listp_srcprt_dstprt_asf
        & ROHC_REP_LIS_PRES_MASK)
    {
        p_dsc->dpib->tcp_dyn_options = (rohc_tcp_dyn_options_t*)p_pkt;
        p_pkt+=sizeof(rohc_tcp_dyn_options_t);

        /*TCP Options
          ----------------------------------------   // PS = 1 -> 8 bit XI field
          |  Reserved   | PS|        M           |   // PS = 0 -> 4 bit XI field
          ----------------------------------------   // M = no of items in compressed list
          |     XI_1, .............XI_m          |   // m octets , or m * 4 bits
          |                  --------------------|   // if PS = 0 and m is odd
          |                  :     Padding       |
          ----------------------------------------
          |                                      |
          |      Items1,............Itemsn       |   //VARIABLE
          |                                      |
          ----------------------------------------

          PS = 0:                                +----------------------------+
          0   1   2   3                          |  Option Name | Table Index |
          ----------------                       +----------------------------+
          | X  |   Index |                       |     NOP      |       0     |
          ----------------                       |     EOL      |       1     |
          PS = 1:                                |     MSS      |       2     |
          0   1   2   3  4   5   6   7           | WINDOW SCALE |       3     |
          -------------------------------        |   TIMESTAMP  |       4     |
          | X  | Reserved |   Index     |        |   SACK PERM  |       5     |
          -------------------------------        |    SACK      |       6     |
          |Generic Option|    7-15      |        +----------------------------+
          X = 1 -> Item present in Compressed list         
          X = 0 -> Item Not presetn in compressed list ,will come in Irregular chain
         */
        mbit =(p_dsc->dpib->tcp_dyn_options->res_ps_m & ROHC_DYN_TCP_M_MASK);

        /*mbit represents the number of items in compressed list*/   
        if(0 != mbit)
        {
            p_dsc->dpib->tcp_dyn_options_XI = (rohc_tcp_dyn_options_XI_t*)p_pkt;

            if(!(p_dsc->dpib->tcp_dyn_options->res_ps_m & ROHC_DYN_PS_MASK))
            /*1 nibble XI*/
            {
                if(!(mbit&0x01)) /*even*/
                {
                    xItems = mbit/2;
                    p_pkt+= (xItems *sizeof(rohc_U8bit_t));
                    p_dsc->dpib->tcp_dyn_options_items =
                         (rohc_tcp_dyn_options_items_t*)p_pkt;
                    for(index=1;index<=xItems;index++)
                    {
                        /* +- SPR 17777 */
                        rohcCalOptItemSize(&sizeItems,
                                p_dsc->dpib,
                                p_dsc->dpib->tcp_dyn_options_XI->xi_m[index-1],
                                (p_dsc->dpib->tcp_dyn_options->res_ps_m
                                 & ROHC_DYN_PS_MASK),0);
                    }

                }
                else /*odd*/
                {
                    xItems = (mbit/2) + 1;
                    p_pkt+= (xItems *sizeof(rohc_U8bit_t));
                    p_dsc->dpib->tcp_dyn_options_items = 
                        (rohc_tcp_dyn_options_items_t*)p_pkt;
                    for(index=1;index<=xItems-1;index++)
                    {
                        /* +- SPR 17777 */
                        rohcCalOptItemSize(&sizeItems,
                                p_dsc->dpib,
                                p_dsc->dpib->tcp_dyn_options_XI->xi_m[index-1],
                                (p_dsc->dpib->tcp_dyn_options->res_ps_m
                                 & ROHC_DYN_PS_MASK),0);
                    }
                    /*for case when number of items is odd,extra padding is
                      present to make a word boundary*/
                    /* +- SPR 17777 */
                    rohcCalOptItemSize(&sizeItems,
                            p_dsc->dpib,
                            p_dsc->dpib->tcp_dyn_options_XI->xi_m[index-1],
                            (p_dsc->dpib->tcp_dyn_options->res_ps_m
                             & ROHC_DYN_PS_MASK),1);
                }
            }
            else /* 1 byte XI*/
            {
                xItems = mbit;
                p_pkt+= (xItems *sizeof(rohc_U8bit_t));
                p_dsc->dpib->tcp_dyn_options_items =
                         (rohc_tcp_dyn_options_items_t*)p_pkt;
                for(index=1;index<=xItems;index++)
                {
                    /* +- SPR 17777 */
                    rohcCalOptItemSize(&sizeItems,
                            p_dsc->dpib,
                            p_dsc->dpib->tcp_dyn_options_XI->xi_m[index-1],
                            (p_dsc->dpib->tcp_dyn_options->res_ps_m
                             & ROHC_DYN_PS_MASK),0);
                }

            }
        }
        p_pkt+= sizeItems;
    } 
    ROHC_EXIT_FN("rohc_parse_p6_Replicate_chain");
    return (p_pkt - p_pkt_org);
}

/****************************************************************************
 * Name     :   rohc_parse_p6_tcp_replicate_partial_chain
 * Desc     :   It parses the Tcp Replicate chain of the IR CR packets
 * Params   :   Pointer to ROHC packet
            :   Pointer to DSC
            :   Pointer to len parsed
 * Returns  :   void
 **************************************************************************/
STATIC rohc_void_t rohc_parse_p6_tcp_replicate_partial_chain(
            rohc_dsc_tcp_t  *p_dsc ,
            rohc_U8bit_t    *p_pkt ,
            rohc_U8bit_t    *p_len )
{
    *p_len = 0;
    p_dsc->dpib->tcp_replicate_partial = 
        (rohc_tcp_replicate_partial_t *)p_pkt;
    p_pkt+= sizeof(rohc_tcp_replicate_partial_t);
    *p_len += sizeof(rohc_tcp_replicate_partial_t);

    if(0x00 != (p_dsc->dpib->tcp_replicate_partial->
        winp_listp_srcprt_dstprt_asf & ROHC_REP_SRC_PORT_PRES_MASK)>>3)
    {
        p_dsc->dpib->tcp_replicate_srcptr = 
            (rohc_tcp_replicate_srcprt_t *)p_pkt;
        if(0x01 == (p_dsc->dpib->tcp_replicate_partial->
            winp_listp_srcprt_dstprt_asf & ROHC_REP_SRC_PORT_PRES_MASK)>>3)
        /*+++coverity-55035*/
        {
        /*+++coverity-55035*/
            p_pkt += sizeof(rohc_U8bit_t);
            *p_len += sizeof(rohc_U8bit_t);
        }
        if(0x02 == (p_dsc->dpib->tcp_replicate_partial->
            winp_listp_srcprt_dstprt_asf & ROHC_REP_SRC_PORT_PRES_MASK)>>3)
        /*+++coverity-55035*/
        {
        /*+++coverity-55035*/
            p_pkt+= sizeof(rohc_tcp_replicate_srcprt_t);
            *p_len += sizeof(rohc_tcp_replicate_srcprt_t);
        }
    }
    
    if(0x00 != (p_dsc->dpib->tcp_replicate_partial->
        winp_listp_srcprt_dstprt_asf & ROHC_REP_DST_PORT_PRES_MASK)>>1)
    {
        p_dsc->dpib->tcp_replicate_dstptr = 
            (rohc_tcp_replicate_dstprt_t *)p_pkt;
        if(0x01 == (p_dsc->dpib->tcp_replicate_partial->
            winp_listp_srcprt_dstprt_asf & ROHC_REP_DST_PORT_PRES_MASK)>>1)
        /*+++coverity-55035*/
        {
        /*+++coverity-55035*/
            p_pkt += sizeof(rohc_U8bit_t);
            *p_len += sizeof(rohc_U8bit_t);
        }
        if(0x02 == (p_dsc->dpib->tcp_replicate_partial->
            winp_listp_srcprt_dstprt_asf & ROHC_REP_DST_PORT_PRES_MASK)>>1)
        /*+++coverity-55035*/
        {
        /*+++coverity-55035*/
            p_pkt+= sizeof(rohc_tcp_replicate_dstprt_t);
            *p_len += sizeof(rohc_tcp_replicate_dstprt_t);
        }
    }

    if(p_dsc->dpib->tcp_replicate_partial->
        winp_listp_srcprt_dstprt_asf & ROHC_REP_WIN_PRES_MASK) 
    {
        p_dsc->dpib->tcp_replicate_win = 
            (rohc_tcp_replicate_win_t *)p_pkt;
        p_pkt+= sizeof(rohc_tcp_replicate_win_t);
        *p_len += sizeof(rohc_tcp_replicate_win_t);
    }

    if(p_dsc->dpib->tcp_replicate_partial->
        ackp_urgp_urgf_ackf_pshf_rsf_ecnu  & ROHC_REP_URP_PRES_MASK)
    {
        p_dsc->dpib->tcp_replicate_urg = 
            (rohc_tcp_replicate_urgptr_t *)p_pkt;
        p_pkt+= sizeof(rohc_tcp_replicate_urgptr_t);
        *p_len += sizeof(rohc_tcp_replicate_urgptr_t);
    }
    
    if(p_dsc->dpib->tcp_replicate_partial->
        ackp_urgp_urgf_ackf_pshf_rsf_ecnu  & ROHC_REP_ACK_PRES_MASK)
    {
        p_dsc->dpib->tcp_replicate_acknum = 
            (rohc_tcp_replicate_acknum_t *)p_pkt;
        p_pkt+= sizeof(rohc_tcp_replicate_acknum_t);
        *p_len += sizeof(rohc_tcp_replicate_acknum_t);
    }

    if(p_dsc->dpib->tcp_replicate_partial->
        ackp_urgp_urgf_ackf_pshf_rsf_ecnu  & ROHC_REP_ECN_USED_MASK)
    {
        p_dsc->dpib->tcp_replicate_ecn = 
            (rohc_tcp_replicate_ecn_t *)p_pkt;
        p_pkt+= sizeof(rohc_tcp_replicate_ecn_t);
        *p_len += sizeof(rohc_tcp_replicate_ecn_t);
    }

    p_dsc->dpib->tcp_replicate_chksum = 
    (rohc_tcp_replicate_chksum_t *)p_pkt;
    p_pkt+= sizeof(rohc_tcp_replicate_chksum_t);
    *p_len += sizeof(rohc_tcp_replicate_chksum_t);
    
    if(p_dsc->dpib->tcp_replicate_partial->winp_listp_srcprt_dstprt_asf
        & ROHC_REP_ACK_STRD_FLAG_MASK)
    {
        p_dsc->dpib->tcp_replicate_ackstride = 
            (rohc_tcp_replicate_ackstrd_t *)p_pkt;
        p_pkt+= sizeof(rohc_tcp_replicate_ackstrd_t);
        *p_len += sizeof(rohc_tcp_replicate_ackstrd_t);
    }

}

/****************************************************************************
 * Name     :   rohc_parse_p1_dynamic_chain
 * Desc     :   It parses the dynamic chain of the IR packets
 * Params   :   Pointer to ROHC packet
            :   Pointer to DSC
            :   Pointer to Entity
            :   Parsed Length
 * Returns  : 	ROHC_SUCCESS or ROHC_FAILURE
 **************************************************************************/
STATIC rohc_return_t 
rohc_parse_p1_dynamic_chain
    ROHC_PARAMS((p_pkt,p_dsc,p_entity,p_len,p_ecode),
            rohc_U8bit_t        *p_pkt      _AND_
            rohc_dsc_rtp_t      *p_dsc      _AND_
            rohc_entity_t       * p_entity  _AND_
            rohc_U32bit_t       *p_len      _AND_
            rohc_error_code_t   *p_ecode)
{
    rohc_U8bit_t *p_pkt_org = p_pkt;
    rohc_U8bit_t len =0, temp_flags = 0;
    ROHC_ENTER_FN("rohc_parse_p1_dynamic_chain");
    if(p_dsc->dpib->outer_ip4_static || p_dsc->outer_ip4)
    {
        p_dsc->dpib->outer_ip4_dyn = (rohc_ip4_dyn_t*)p_pkt;
        p_pkt+=sizeof(rohc_ip4_dyn_t);
        if(ROHC_FAILURE == rohc_list_decoding_p1(p_entity,p_dsc,
                    ROHC_OUTERIP_EXTHEADERS_LIST_FID,
                    p_len,p_pkt,p_ecode))
        {
            return ROHC_FAILURE;
        }
        p_pkt += *p_len; 
    }
    else if(p_dsc->dpib->outer_ip6_static || p_dsc->outer_ip6)
    {
        p_dsc->dpib->outer_ip6_dyn = (rohc_ip6_dyn_t*)p_pkt;
        p_pkt+=sizeof(rohc_ip6_dyn_t);
        if(ROHC_FAILURE == rohc_list_decoding_p1(p_entity,p_dsc,
                    ROHC_OUTERIP_EXTHEADERS_LIST_FID,
                    p_len,p_pkt,p_ecode))
        {
            return ROHC_FAILURE;
        }
        p_pkt+= *p_len; 
    }
    if(p_dsc->dpib->inner_ip4_static || p_dsc->inner_ip4)
    {
        p_dsc->dpib->inner_ip4_dyn = (rohc_ip4_dyn_t*)p_pkt;
        p_pkt+=sizeof(rohc_ip4_dyn_t);
        if(ROHC_FAILURE == rohc_list_decoding_p1(p_entity,p_dsc,
                    ROHC_INNERIP_EXTHEADERS_LIST_FID,
                    p_len,p_pkt,p_ecode))
        {
            return ROHC_FAILURE;
        }
        p_pkt+= *p_len; 
    }
    else if(p_dsc->dpib->inner_ip6_static || p_dsc->inner_ip6)
    {
        p_dsc->dpib->inner_ip6_dyn = (rohc_ip6_dyn_t*)p_pkt;
        p_pkt+=sizeof(rohc_ip6_dyn_t);
        /* SPR 14085 fix start */
        /* This function call is needed for list decoding 
         * even if we do not support extension headers */
        if(ROHC_FAILURE == rohc_list_decoding_p1(p_entity,p_dsc,
           ROHC_INNERIP_EXTHEADERS_LIST_FID,
           p_len,p_pkt,p_ecode))
        {
            return ROHC_FAILURE;
        }
        p_pkt+= *p_len; 
        /* SPR 14085 fix end */
    }

    p_dsc->dpib->udp_dyn = (rohc_udp_dyn_t*)p_pkt;
    p_pkt+=sizeof(rohc_udp_dyn_t);
    p_dsc->dpib->rtp_head_dyn = (rohc_rtp_head_dyn_t*)p_pkt;
    temp_flags = *p_pkt;
    p_pkt+=sizeof(rohc_rtp_head_dyn_t);
    /* Start SPR 5811 Fix*/
    if(ROHC_FAILURE == rohc_list_decoding_p1(p_entity,p_dsc,
                ROHC_RTP_CSRC_LIST_FID,
                p_len,p_pkt,p_ecode))
    {
        return ROHC_FAILURE;
    }
    p_pkt += *p_len;
    /* End SPR 5811 Fix*/
    if( temp_flags & ROHC_DYN_RX_MASK )
    {
        p_dsc->dpib->rtp_tail_dyn = (rohc_rtp_tail_dyn_t*)p_pkt;
        temp_flags = *p_pkt;
        p_pkt+=sizeof(rohc_rtp_tail_dyn_t);
        if( temp_flags & ROHC_DYN_TSS_MASK )
        {
            p_dsc->dpib->field_val[ROHC_RTP_TS_STRIDE_FID-
                ROHC_VAR_FID_BASE] = rohc_d_sdvalue_decode(p_pkt,
                        &len);
            p_pkt+= len;
        }
        if( temp_flags & ROHC_DYN_TIS_MASK )
        {
            p_dsc->dpib->field_val[ROHC_RTP_TIME_STRIDE_FID-
                ROHC_VAR_FID_BASE] = rohc_d_sdvalue_decode(p_pkt,
                        &len);
            p_pkt+= len;
        }
    }
    *p_len = p_pkt - p_pkt_org;
    ROHC_EXIT_FN("rohc_parse_p1_dynamic_chain");
    return ROHC_SUCCESS;
}

/****************************************************************************
 * Name     :   rohc_parse_p2_dynamic_chain
 * Desc     :   It parses the dynamic chain of the IR packets
 * Params   :   Pointer to ROHC packet
            :   Pointer to DSC
            :   Pointer to Entity
            :   Parsed Length
 * Returns  : 	ROHC_SUCCESS or ROHC_FAILURE
 **************************************************************************/
STATIC rohc_return_t 
rohc_parse_p2_dynamic_chain
    ROHC_PARAMS((p_pkt,p_dsc,p_entity,p_len,p_ecode),
            rohc_U8bit_t    *p_pkt _AND_
            rohc_dsc_udp_t  *p_dsc _AND_
            rohc_entity_t    * p_entity _AND_
            rohc_U32bit_t *p_len _AND_
            rohc_error_code_t   *p_ecode)
{
    rohc_U8bit_t *p_pkt_org = p_pkt;
    ROHC_ENTER_FN("rohc_parse_p2_dynamic_chain");
    if(p_dsc->dpib->outer_ip4_static || p_dsc->outer_ip4)
    {
        p_dsc->dpib->outer_ip4_dyn = (rohc_ip4_dyn_t*)p_pkt;
        p_pkt+=sizeof(rohc_ip4_dyn_t);
        if(ROHC_FAILURE == rohc_list_decoding_p2(p_entity,p_dsc,
                    ROHC_OUTERIP_EXTHEADERS_LIST_FID,
                    p_len,p_pkt,p_ecode))
        {
            return ROHC_FAILURE;
        }
        p_pkt += *p_len; 
    }
    else if(p_dsc->dpib->outer_ip6_static || p_dsc->outer_ip6)
    {
        p_dsc->dpib->outer_ip6_dyn = (rohc_ip6_dyn_t*)p_pkt;
        p_pkt+=sizeof(rohc_ip6_dyn_t);
        if(ROHC_FAILURE == rohc_list_decoding_p2(p_entity,p_dsc,
                    ROHC_OUTERIP_EXTHEADERS_LIST_FID,
                    p_len,p_pkt,p_ecode))
        {
            return ROHC_FAILURE;
        }
        p_pkt+= *p_len; 
    }
    if(p_dsc->dpib->inner_ip4_static || p_dsc->inner_ip4)
    {
        p_dsc->dpib->inner_ip4_dyn = (rohc_ip4_dyn_t*)p_pkt;
        p_pkt+=sizeof(rohc_ip4_dyn_t);
        if(ROHC_FAILURE == rohc_list_decoding_p2(p_entity,p_dsc,
                    ROHC_INNERIP_EXTHEADERS_LIST_FID,
                    p_len,p_pkt,p_ecode))
        {
            return ROHC_FAILURE;
        }
        p_pkt+= *p_len; 
    }
    else if(p_dsc->dpib->inner_ip6_static || p_dsc->inner_ip6)
    {
        p_dsc->dpib->inner_ip6_dyn = (rohc_ip6_dyn_t*)p_pkt;
        p_pkt+=sizeof(rohc_ip6_dyn_t);
        /* SPR 14085 fix start */
        /* This function call is needed for list decoding 
         * even if we do not support extension headers */
        if(ROHC_FAILURE == rohc_list_decoding_p2(p_entity,p_dsc,
                    ROHC_INNERIP_EXTHEADERS_LIST_FID,
                    p_len,p_pkt,p_ecode))
        {
            return ROHC_FAILURE;
        }
        p_pkt+= *p_len; 
        /* SPR 14085 fix end */
    }
    p_dsc->dpib->udp_dyn = (rohc_udp_dyn_t*)p_pkt;
    p_pkt+=sizeof(rohc_udp_dyn_t);
    p_dsc->dpib->field_val[ROHC_SN_FID
        - ROHC_VAR_FID_BASE] = ROHC_GET_2BYTES(p_pkt);
    p_pkt+=2;
    *p_len = p_pkt - p_pkt_org;
    ROHC_EXIT_FN("rohc_parse_p2_dynamic_chain");
    return ROHC_SUCCESS;
}

/****************************************************************************
 * Name     :   rohcParseSackPureLsb
 * Desc     :   This function updates the p_p_sizeItems with the Option size
                based on the index number passed
 * Params   :   Pointer to Pointer of sizeItems
            :   Index Value
 * Returns  : 	ROHC_SUCCESS
 **************************************************************************/
rohc_U8bit_t rohcParseSackPureLsb(
    rohc_dpib_t *dpib,
    rohc_U8bit_t **p_p_sizeItems)
{
    rohc_U8bit_t size_block=0;
    rohc_U8bit_t block_counter =0;
    rohc_U8bit_t size_ptr = **p_p_sizeItems;
    ROHC_ENTER_FN("rohcParseSackPureLsb");

    /*
       COMPRESSED {
       block_start =:=
       sack_pure_lsb(reference)          [ 16, 24, 32, 40 ];
       block_end   =:=
       sack_pure_lsb(block_start.UVALUE) [ 16, 24, 32, 40 ];
       }
     */
    /*Pure LSB encoding will be done for two times.
      once for block_start and then for block_end*/

    while(block_counter++ < 2)
    {
        /*
           COMPRESSED lsb_15 {
           ENFORCE(sack_field.CVALUE == sack_field.CVALUE <= 0x7fff);
           discriminator =:= '0'           [ 1 ];
           sack_field                      [ 15 ];
           }
         */
        if(!(((rohc_U8bit_t)(*((rohc_U8bit_t*)dpib->tcp_dyn_options_items +
                                size_ptr))) & 0x80))
        {
	        size_ptr += 2;
            size_block += 2;
        }
        /*
           COMPRESSED lsb_22 {
           ENFORCE(sack_field.CVALUE == sack_field.CVALUE <= 0x3fffff);
           discriminator =:= '10'          [ 2 ];
           sack_field                      [ 22 ];
           }
         */
        else if(!(((rohc_U8bit_t)(*((rohc_U8bit_t*)dpib->tcp_dyn_options_items +
                                size_ptr))) & 0x40))
        {
	        size_ptr += 3;
            size_block += 3;
        }
        /*
           COMPRESSED lsb_29 {
           ENFORCE(sack_field.CVALUE == sack_field.CVALUE <= 0x1fffffff);
           discriminator =:= '110'         [ 3 ];
           sack_field                      [ 29 ];
           }
         */
        else if(!(((rohc_U8bit_t)(*((rohc_U8bit_t*)dpib->tcp_dyn_options_items +
                                size_ptr))) & 0x20))
        {
	        size_ptr += 4;
            size_block += 4;
        }
        /*
           COMPRESSED full_offset {
           discriminator =:= '11111111'    [ 8 ];
           sack_field                      [ 32 ];
           }
         */
        else
        {
	        size_ptr += 5;
            size_block += 5;
        }
    }
    ROHC_EXIT_FN("rohcParseSackPureLsb");
    return size_block;
}

/****************************************************************************
 * Name     :   rohcIncSizeOptItem
 * Desc     :   This function updates the p_p_sizeItems with the Option size
                based on the index number passed
 * Params   :   Pointer to Pointer of sizeItems
            :   Index Value,
				xItems - value of XI item of Options
				dpib
 * Returns  : 	ROHC_SUCCESS
 **************************************************************************/
rohc_return_t rohcIncSizeOptItem(
            rohc_U8bit_t **p_p_sizeItems,
            rohc_U8bit_t index,
            /* +- SPR 17777 */
            rohc_dpib_t *dpib
)
{
    ROHC_ENTER_FN("rohcIncSizeOptItem");
    switch(index)
    {
        case INDEX_NOP :
            /* COMPRESSED nop_list_item{
               }*/
            break;
        case INDEX_EOL :
            /* COMPRESSED eol_list_item{
               pad_len =:= compressed_value(8) [8];
               }*/
            **p_p_sizeItems += 1;
            break;
        case INDEX_MSS :
            /* COMPRESSED mss_list_item{
               mss =:= irregular(16) [16];
               }*/
            **p_p_sizeItems += 2;
            break;
        case INDEX_WINDOW_SCAL :
            /* COMPRESSED wscale_list_item{
               wscale =:= irregular(8) [8];
               }*/
            **p_p_sizeItems += 1;
            break;
        case INDEX_TIMESTAMP :
            /* COMPRESSED tsopt_list_item{
               tsval =:= irregular(32) [32];
               ttsechosval =:= irregular(32) [32];
               }*/
            **p_p_sizeItems += 8;
            break;
        case INDEX_SACK_PERM :
            /* COMPRESSED sack_permitted_list_item{
               }*/
            break;
        case INDEX_SACK :
            /*
               COMPRESSED sack1_list_item {
               discriminator =:= '00000001';
               block_1       =:= sack_block(ack_value);
               ENFORCE(length.UVALUE == 10);
               }
             */
            if(0x01 == (*(((rohc_U8bit_t*)dpib->
                tcp_dyn_options_items) + **p_p_sizeItems)))
            {
                **p_p_sizeItems += 1;
                /*parsing Block1*/
                **p_p_sizeItems += rohcParseSackPureLsb(dpib,p_p_sizeItems);
            }
            /*
               COMPRESSED sack2_list_item {
               discriminator =:= '00000010';
               block_1       =:= sack_block(ack_value);
               block_2       =:= sack_block(block_1.UVALUE && 0xFFFFFFFF);
               ENFORCE(length.UVALUE == 18);
               }
             */
            else if(0x02 == (*(((rohc_U8bit_t*)dpib->
                tcp_dyn_options_items) + **p_p_sizeItems)))
            {
                **p_p_sizeItems += 1;

                /*parsing Block1*/
                **p_p_sizeItems += rohcParseSackPureLsb(dpib,p_p_sizeItems);
                /*parsing Block2*/
                **p_p_sizeItems += rohcParseSackPureLsb(dpib,p_p_sizeItems);
            }
            /*
               COMPRESSED sack3_list_item {
               discriminator =:= '00000011';
               block_1       =:= sack_block(ack_value);
               block_2       =:= sack_block(block_1.UVALUE && 0xFFFFFFFF);
               block_3       =:= sack_block(block_2.UVALUE && 0xFFFFFFFF);
               ENFORCE(length.UVALUE == 26);
               }
             */
            else if(0x03 == (*(((rohc_U8bit_t*)dpib->
                tcp_dyn_options_items ) + **p_p_sizeItems)))
            {
                **p_p_sizeItems += 1;
                /*parsing Block1*/
                **p_p_sizeItems += rohcParseSackPureLsb(dpib,p_p_sizeItems);
                /*parsing Block2*/
                **p_p_sizeItems += rohcParseSackPureLsb(dpib,p_p_sizeItems);
                /*parsing Block3*/
                **p_p_sizeItems += rohcParseSackPureLsb(dpib,p_p_sizeItems);
            }
            /*
               COMPRESSED sack4_list_item {
               discriminator =:= '00000100';
               block_1       =:= sack_block(ack_value);
               block_2       =:= sack_block(block_1.UVALUE && 0xFFFFFFFF);
               block_3       =:= sack_block(block_2.UVALUE && 0xFFFFFFFF);
               block_4       =:= sack_block(block_3.UVALUE && 0xFFFFFFFF);
               ENFORCE(length.UVALUE == 34);
               }
             */
            else if(0x04 == (*(((rohc_U8bit_t*)dpib->
                tcp_dyn_options_items) + **p_p_sizeItems)))
            {
                **p_p_sizeItems += 1;
                /*parsing Block1*/
                **p_p_sizeItems += rohcParseSackPureLsb(dpib,p_p_sizeItems);
                /*parsing Block2*/
                **p_p_sizeItems += rohcParseSackPureLsb(dpib,p_p_sizeItems);
                /*parsing Block3*/
                **p_p_sizeItems += rohcParseSackPureLsb(dpib,p_p_sizeItems);
                /*parsing Block4*/
                **p_p_sizeItems += rohcParseSackPureLsb(dpib,p_p_sizeItems);
            }
            break;
        default:
        /* Generic option length calculation*/
        /* Increment for Type*/    
        (**p_p_sizeItems)++;
        /* Length + content Increment*/ 
        **p_p_sizeItems +=
            (0x7F & ((*(((rohc_U8bit_t*)dpib->tcp_dyn_options_items) +
            **p_p_sizeItems)) -1 ));
    }

    ROHC_EXIT_FN("rohcIncSizeOptItem");
    return ROHC_SUCCESS;
}

/****************************************************************************
 * Name     :   rohcCalOptItemSize
 * Desc     :   This function will calculate the Option item size of belonging
 *              to a index number
 * Params   :   Pointer to  sizeItems
 *          :   value of XI item of Options
 *				Pointer to DPIB
 *          :   value of ps_flag (ps_flag will identify whether XI is 1 byte
 *              or 4bits)
 *          :   padding_boundary (will be used for case when last by of XI item
 *              list is at odd boundary and ps flag is odd)
 * Returns  : ROHC_SUCCESS
 **************************************************************************/
rohc_return_t rohcCalOptItemSize(
            rohc_U8bit_t    *p_sizeItems,
            /* +- SPR 17777 */
            rohc_dpib_t     *dpib,
            rohc_U8bit_t    xi_m ,
            rohc_U8bit_t    ps_flag,
            rohc_U8bit_t    padding_boundary)
{
    rohc_U8bit_t index;
    ROHC_ENTER_FN("rohcCalOptItemSize");
    if(ps_flag)
    {   /*1 byte XI */
        /* Check for Presence Bit X */
        if ( xi_m & 0x80  )
        {
            index = xi_m & 0x0F;
            /* +- SPR 17777 */
            rohcIncSizeOptItem(&p_sizeItems,index,dpib);
        }
    }
    else
    {
        if(padding_boundary) /*This is true only for the last byte of Odd*/
        {   /* Checking for Presence Bit X */    
            if ( xi_m & 0x80 )
            {
                index = (xi_m & 0x70)>>4;
                /* +- SPR 17777 */
                rohcIncSizeOptItem(&p_sizeItems,index,dpib);
            }
        }
        else
        {   /* Checking for Presence Bit X */    
            if ( xi_m & 0x80 )
            {
                index = (xi_m & 0x70)>>4;
                /* +- SPR 17777 */
                rohcIncSizeOptItem(&p_sizeItems,index,dpib);
            }
            /* Checking for Presence Bit X*/
            if ( xi_m & 0x08 )
            {
                index = xi_m & 0x07;
                /* +- SPR 17777 */
                rohcIncSizeOptItem(&p_sizeItems,index,dpib);
            }
        }
    }

    ROHC_EXIT_FN("rohcCalOptItemSize");
    return ROHC_SUCCESS;
}

/****************************************************************************
 * Name     :   rohcParseP6DynamicChain
 * Desc     :   It parses the dynamic chain of the IR packets
 * Params   :   Pointer to ROHC packet
            :   Pointer to DSC
            :   Pointer to Entity
            :   Parsed Length
 * Returns  : 	ROHC_SUCCESS or ROHC_FAILURE
 **************************************************************************/
static rohc_return_t 
rohcParseP6DynamicChain(
            rohc_U8bit_t    *p_pkt ,
            rohc_dsc_tcp_t  *p_dsc ,
            rohc_entity_t    * p_entity ,
            rohc_U32bit_t *p_len ,
            rohc_error_code_t   *p_ecode)
{
    rohc_U8bit_t *p_pkt_org = p_pkt;
    rohc_U8bit_t temp_flags = 0;
    rohc_U8bit_t mbit  ; 
    rohc_U8bit_t sizeItems = 0;
    rohc_U8bit_t xItems = 0;
    rohc_U8bit_t index = 0;
    ROHC_ENTER_FN("rohcParseP6DynamicChain");
    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,(ADDR)p_ecode,
            0,0,0,0, __func__,"");

/* SPR 20636 Changes End */
    if(p_dsc->dpib->outer_ip4_static || p_dsc->outer_ip4)
    {
        rohcParseCompressedHeaderChain_p6(p_dsc,
                ROHC_OUTERIP_EXTHEADERS_LIST_FID,
                &p_pkt);
    }
    else if(p_dsc->dpib->outer_ip6_static || p_dsc->outer_ip6)
    {
        rohcParseCompressedHeaderChain_p6(p_dsc,
                ROHC_OUTERIP_EXTHEADERS_LIST_FID,
                &p_pkt);
    }
    if(p_dsc->dpib->inner_ip4_static || p_dsc->inner_ip4)
    {
        rohcParseCompressedHeaderChain_p6(p_dsc,
                ROHC_INNERIP_EXTHEADERS_LIST_FID,
                &p_pkt);
    }
    else if(p_dsc->dpib->inner_ip6_static || p_dsc->inner_ip6)
    {
        rohcParseCompressedHeaderChain_p6(p_dsc,
                ROHC_INNERIP_EXTHEADERS_LIST_FID,
                &p_pkt);
    }

    p_dsc->dpib->tcp_dyn = (rohc_tcp_dyn_partial_t*)p_pkt;
    temp_flags = *p_pkt;
    p_pkt+=sizeof(rohc_tcp_dyn_partial_t);

    if(!(temp_flags & ROHC_DYN_ACK_ZERO_MASK))
        /*zero_or_irreg(ack_zero.CVALUE,22)*/
    {
        p_dsc->dpib->tcp_dyn_acknum = (rohc_tcp_dyn_acknum_t*)p_pkt;
        p_pkt+=sizeof(rohc_tcp_dyn_acknum_t);
    }

    p_dsc->dpib->tcp_dyn_windncksum = (rohc_tcp_dyn_windncksum_t*)p_pkt;
    p_pkt+=sizeof(rohc_tcp_dyn_windncksum_t);

    if(!(temp_flags & ROHC_DYN_URP_ZERO_MASK))
        /*zero_or_irreg(urp_ptr.CVALUE,16)*/
    {
        p_dsc->dpib->tcp_dyn_urgptr = (rohc_tcp_dyn_urgptr_t*)p_pkt;
        p_pkt+=sizeof(rohc_tcp_dyn_urgptr_t);
    }

    if( temp_flags & ROHC_DYN_ACK_STRIDE_MASK)
        /*static_or_irreg(ack_stride_flag.CVALUE,16)*/
    {
        p_dsc->dpib->tcp_dyn_ackstride = (rohc_tcp_dyn_ackstride_t*)p_pkt;
        p_pkt+=sizeof(rohc_tcp_dyn_ackstride_t);
    }

    p_dsc->dpib->tcp_dyn_options = (rohc_tcp_dyn_options_t*)p_pkt;
    p_pkt+=sizeof(rohc_tcp_dyn_options_t);
     

    /*TCP Options
      ----------------------------------------    PS = 1 -> 8 bit XI field,
      |  Reserved   | PS|        M           |    PS = 0 -> 4 bit XI field
      ----------------------------------------    M = no of items in comp.list
      |     XI_1, .............XI_m          |    m octets , or m * 4 bits
      |                  --------------------|    if PS = 0 and m is odd
      |                  :     Padding       |
      ----------------------------------------
      |                                      |
      |      Items1,............Itemsn       |   VARIABLE
      |                                      |
      ----------------------------------------

      PS = 0:                                 +----------------------------+
      0   1   2   3                         |  Option Name | Table Index |
      ----------------                        +----------------------------+
      | X  |   Index |                        |     NOP      |       0     |
      ----------------                        |     EOL      |       1     |
      PS = 1:                                 |     MSS      |       2     |
      0   1   2   3  4   5   6   7          | WINDOW SCALE |       3     |
      -------------------------------         |   TIMESTAMP  |       4     |
      | X  | Reserved |   Index     |         |   SACK PERM  |       5     |
      -------------------------------         |    SACK      |       6     |
      |Generic Option|    7-15     |
      X = 1 -> Item present in Compressed list +----------------------------+
      X = 0 -> Item Not presetn in compressed list ,will come in Irregular chain
     */

    /*mbit is number of items in compressed list*/
    mbit =(p_dsc->dpib->tcp_dyn_options->res_ps_m & 
            ROHC_DYN_TCP_M_MASK);
    if(0 != mbit)
    {
        p_dsc->dpib->tcp_dyn_options_XI = (rohc_tcp_dyn_options_XI_t*)p_pkt;
        if(!(p_dsc->dpib->tcp_dyn_options->res_ps_m & ROHC_DYN_PS_MASK))
            /*1 nibble XI*/
        {
            if(!(mbit&0x01)) /*even*/
            {
                xItems = mbit/2;
                p_pkt+= (xItems *sizeof(rohc_U8bit_t));
                p_dsc->dpib->tcp_dyn_options_items = (rohc_tcp_dyn_options_items_t*)p_pkt;
                for(index=1;index<=xItems;index++)
                {
                    /* +- SPR 17777 */
                    rohcCalOptItemSize(&sizeItems,
                            p_dsc->dpib,
                            p_dsc->dpib->tcp_dyn_options_XI->xi_m[index-1],
                            (p_dsc->dpib->tcp_dyn_options->res_ps_m
                             & ROHC_DYN_PS_MASK),0);
                }

            }
            else /*odd*/
            {
                xItems = (mbit/2) + 1;
                p_pkt+= (xItems *sizeof(rohc_U8bit_t));
                p_dsc->dpib->tcp_dyn_options_items = (rohc_tcp_dyn_options_items_t*)p_pkt;
                for(index=1;index<=xItems-1;index++)
                {
                    /* +- SPR 17777 */
                    rohcCalOptItemSize(&sizeItems,
                            p_dsc->dpib,
                            p_dsc->dpib->tcp_dyn_options_XI->xi_m[index-1],
                            (p_dsc->dpib->tcp_dyn_options->res_ps_m
                             & ROHC_DYN_PS_MASK),0);
                }
                /*for case when number of items is odd,extra padding is present to
                  make a word boundary*/
                /* +- SPR 17777 */
                rohcCalOptItemSize(&sizeItems,
                        p_dsc->dpib,
                        p_dsc->dpib->tcp_dyn_options_XI->xi_m[index-1],
                        (p_dsc->dpib->tcp_dyn_options->res_ps_m
                         & ROHC_DYN_PS_MASK),1);
            }
        }
        else /* 1 byte XI*/
        {
            xItems = mbit;
            p_pkt+= (xItems *sizeof(rohc_U8bit_t));
            p_dsc->dpib->tcp_dyn_options_items = (rohc_tcp_dyn_options_items_t*)p_pkt;
            for(index=1;index<=xItems;index++)
            {
                /* +- SPR 17777 */
                rohcCalOptItemSize(&sizeItems,
                        p_dsc->dpib,
                        p_dsc->dpib->tcp_dyn_options_XI->xi_m[index-1],
                        (p_dsc->dpib->tcp_dyn_options->res_ps_m
                         & ROHC_DYN_PS_MASK),0);
            }
        }
    }

    /*parse for the TCP Dynamic options items*/
    p_pkt+= sizeItems;

    *p_len = p_pkt - p_pkt_org;
    ROHC_EXIT_FN("rohcParseP6DynamicChain");
    return ROHC_SUCCESS;
}

/****************************************************************************
 * Name     :   rohcDecTCPOptionsP6
 * Desc     :   It decodes the optional fields of CO packets
 * Params   :   Pointer to ROHC packet
            :   packet length
            :   Pointer to DPIB
 * Returns  : 	ROHC_SUCCESS or ROHC_FAILURE
 **************************************************************************/
static rohc_return_t 
rohcDecTCPOptionsP6(
    rohc_U8bit_t * p_p_rohc_pkt ,
    rohc_U8bit_t * p_len ,
    rohc_dpib_t     * p_dpib)

{
    rohc_U8bit_t *p_pkt_org = p_p_rohc_pkt;
    rohc_U8bit_t sizeItems = 0;
    p_dpib->tcp_dyn_options = (rohc_tcp_dyn_options_t*)p_p_rohc_pkt;
    p_p_rohc_pkt+=sizeof(rohc_tcp_dyn_options_t);

    rohc_U8bit_t mbit=(p_dpib->tcp_dyn_options->res_ps_m & ROHC_DYN_TCP_M_MASK);
    rohc_U8bit_t xItems = 0;
    rohc_U8bit_t counter = 0;
   
    if(0 != mbit)
    {
        p_dpib->tcp_dyn_options_XI = (rohc_tcp_dyn_options_XI_t*)p_p_rohc_pkt;
        if(!(p_dpib->tcp_dyn_options->res_ps_m & ROHC_DYN_PS_MASK))/*1 nibble XI*/
        {        
            if(!(mbit&0x01)) /*even*/
            {
                xItems = mbit/2;
                p_p_rohc_pkt+= (xItems *sizeof(rohc_U8bit_t));
                p_dpib->tcp_dyn_options_items = (rohc_tcp_dyn_options_items_t*)p_p_rohc_pkt;
                for(counter=1;counter<=xItems;counter++)
                { 
                    /* +- SPR 17777 */
                    rohcCalOptItemSize(&sizeItems,
                            p_dpib,
                            p_dpib->tcp_dyn_options_XI->xi_m[counter-1],
                            (p_dpib->tcp_dyn_options->res_ps_m
                             & ROHC_DYN_PS_MASK),0);
                }

            }
            else /*odd*/
            {
                xItems = (mbit/2) + 1;
                p_p_rohc_pkt+= (xItems *sizeof(rohc_U8bit_t));
                p_dpib->tcp_dyn_options_items = (rohc_tcp_dyn_options_items_t*)p_p_rohc_pkt;
                for(counter=1;counter<=xItems-1;counter++)
                { 
                    /* +- SPR 17777 */
                    rohcCalOptItemSize(&sizeItems,
                            p_dpib,
                            p_dpib->tcp_dyn_options_XI->xi_m[counter-1],
                            (p_dpib->tcp_dyn_options->res_ps_m
                             & ROHC_DYN_PS_MASK),0);
                }
                /*for case when number of items is odd,extra padding is present to
                  make a word boundary*/
                /* +- SPR 17777 */
                rohcCalOptItemSize(&sizeItems,
                        p_dpib,
                        p_dpib->tcp_dyn_options_XI->xi_m[counter-1],
                        (p_dpib->tcp_dyn_options->res_ps_m
                         & ROHC_DYN_PS_MASK),1);
            }
        }
        else /* 1 byte XI*/
        {
               xItems = mbit;
               p_p_rohc_pkt+= (xItems *sizeof(rohc_U8bit_t));
               p_dpib->tcp_dyn_options_items = (rohc_tcp_dyn_options_items_t*)p_p_rohc_pkt;
               for(counter=1;counter<=xItems;counter++)
               { 
                   /* +- SPR 17777 */
                rohcCalOptItemSize(&sizeItems,
                        p_dpib,
                        p_dpib->tcp_dyn_options_XI->xi_m[counter-1],
                        (p_dpib->tcp_dyn_options->res_ps_m
                         & ROHC_DYN_PS_MASK),0);
               }
        }
    }
    /*parse for the TCP Dynamic options items*/
    p_p_rohc_pkt+= sizeItems * sizeof(rohc_U8bit_t);

    *p_len = p_p_rohc_pkt - p_pkt_org;
    ROHC_EXIT_FN("rohcDecTCPOptionsP6");
    return ROHC_SUCCESS;
}


/****************************************************************************
 * Name     :   parse_ir_irdyn_pkts
 * Desc     :   It parses the IR and IRDYN packets
 * Params   :   Pointer to Entity,
 *				Type of the Rohc Packet,
 *          :   Pointer to ROHC packet,
 *          :   Pointer to DSC,
 *				Pointer to Error Code
 * Returns  : 	ROHC_SUCCESS or ROHC_FAILURE
 **************************************************************************/
STATIC rohc_return_t 
parse_ir_irdyn_pkts
    ROHC_PARAMS ((p_entity,pkt_type,p_pkt,p_dsc,p_ecode),
        rohc_entity_t       *p_entity _AND_
        rohc_U8bit_t        pkt_type _AND_
        rohc_U8bit_t        *p_pkt _AND_
        rohc_dsc_common_t   *p_dsc _AND_
        rohc_error_code_t   *p_ecode)
{
    rohc_U32bit_t len = 0;
    ROHC_ENTER_FN("parse_ir_irdyn_pkts");
    p_dsc->dpib->profile = *p_pkt++;
    p_pkt++; /*for CRC Skip */

    switch(p_dsc->dpib->profile)
    {
        case ROHC_IP_UDP :
        {
            if( pkt_type != ROHC_IRDYN_PKTTYPE)
            {
                /* + SPR 17439 */
                len = rohc_parse_p2_static_chain(p_pkt,(rohc_dsc_udp_t*)p_dsc);
                /* - SPR 17439 */
                p_pkt+=len;
            }
            if( pkt_type == ROHC_IRDYN_PKTTYPE  ||
                pkt_type == ROHC_IR_WITH_DYN_PKTTYPE)
            {
                /* + SPR 17439 */
                if( ROHC_FAILURE == rohc_parse_p2_dynamic_chain(p_pkt,(rohc_dsc_udp_t*)p_dsc,
                /* - SPR 17439 */
                                       p_entity,&len,p_ecode))
                {
                    return ROHC_FAILURE;
                }
                p_pkt+=len;
            }
            break;
        }

        case ROHC_IP_UDP_RTP :
        {
            if( pkt_type != ROHC_IRDYN_PKTTYPE)
            {
                /* + SPR 17439 */
                len = rohc_parse_p1_static_chain(p_pkt,(rohc_dsc_rtp_t*)p_dsc);
                /* - SPR 17439 */
                p_pkt+=len;
            }
            if( pkt_type == ROHC_IRDYN_PKTTYPE  ||
                pkt_type == ROHC_IR_WITH_DYN_PKTTYPE)
            {
                /* + SPR 17439 */
                if( ROHC_FAILURE == rohc_parse_p1_dynamic_chain(p_pkt,(rohc_dsc_rtp_t *)p_dsc,
                                       p_entity,&len,p_ecode))
                /* - SPR 17439 */
                {
                    return ROHC_FAILURE;
                }
                p_pkt+=len;
            }
            break;
        }
        case ROHC_IP_TCP :
        {
            /*static chain will be parsed only for the ROHC_IR_WITH_DYN_PKTTYPE*/
            if( ROHC_IRDYN_PKTTYPE != pkt_type)
            {
                len = rohcParseP6StaticChain(p_pkt,(rohc_dsc_tcp_t *)p_dsc);
                p_pkt+=len;
            }

            rohcParseP6DynamicChain(p_pkt,(rohc_dsc_tcp_t *)p_dsc,
                       p_entity,&len,p_ecode);
            p_pkt+=len;
            break;
        }
        default :
             ROHC_LOG(LOG_INFO,"EID[%u:%u]:"
                        " Invalid profilr received [%d]",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,
                        p_dsc->dpib->profile);
             break;
    }

    p_dsc->dpib->payload = p_pkt;
    ROHC_EXIT_FN("parse_ir_irdyn_pkts");
    return ROHC_SUCCESS;
}


/****************************************************************************
 * Name     :   parse_ir_cr_pkts
 * Desc     :   It parses the IR CR packets
 * Params   :   Pointer to Entity,
 *				Type of the Rohc Packet,
 *          :   Pointer to ROHC packet,
 *          :   Pointer to DSC,
 *				Pointer to Error Code
 * Returns  : 	ROHC_SUCCESS or ROHC_FAILURE
 **************************************************************************/
static rohc_return_t 
parse_ir_cr_pkts
    ROHC_PARAMS ((p_entity,pkt_type,p_pkt,p_dsc,p_ecode),
        rohc_entity_t       *p_entity _AND_
        rohc_U8bit_t        pkt_type _AND_
        rohc_U8bit_t        *p_pkt _AND_
        rohc_dsc_common_t   *p_dsc _AND_
        rohc_error_code_t   *p_ecode)
{
    rohc_U32bit_t len = 0;
    ROHC_ENTER_FN("parse_ir_cr_pkts");
    rohc_U8bit_t profile = 0;
    rohc_U8bit_t crc7_cr = 0;
    profile = *p_pkt++;
    
    p_pkt++; /*for CRC Skip */
 
    /*---------------------------------------
    |           Add - Cid Octet             |    if for small CIDs nad (CID !=0)
    -----------------------------------------
    |  1    1    1    1    1    1    0    0 |     IR-CR type octed
    -----------------------------------------
    \                                       \
    :          0-2 Octets of CID            :     1-2 octets if for large CIDs
    \                                       \
    -----------------------------------------
    |           Profile = 0x06              |     1 octet
    -----------------------------------------
    |               CRC                     |     1 octet
    -----------------------------------------
    |   B  |            CRC 7               |     1 octet
    -----------------------------------------
    |      Reserved     |     Base CID      |     1 octet,for small CID,if B=1
    -----------------------------------------
    \                                       \ 
    :              Base CID                 :     1-2 octets,for large CIDs,
    \                                       \     if B=1
    -----------------------------------------
    |           Replicate Chain             |     Variable length
    -----------------------------------------
    |               Payload                 |     Variable length
    ---------------------------------------*/
   
   /*Lower 4 bits represent the Base CID which should not be zero*/
    if(*(p_pkt + 1) & 0x0F)
    {
        /*saving Base CID CRC to dpib*/
        crc7_cr = (*p_pkt & 0x7F) ;
        
        p_pkt++; /*for B and CRC7  skip*/

         rohc_U16bit_t cid = p_dsc->dpib->field_val[ROHC_CID_FID-ROHC_VAR_FID_BASE];
         rohc_U16bit_t   BaseCid = *p_pkt & 0x0F;
         rohc_dsc_tcp_t   *p_dsc_base;
         rohc_dpib_t   *p_dpib_base = ROHC_NULL;
         /*Finding if the base CID exist in the Decompressor context*/
         p_dsc->dpib->field_val[ROHC_CID_FID-ROHC_VAR_FID_BASE] = BaseCid;
        
        /*Finding if the base CID exist in the Decompressor context*/
        if(ROHC_NULL == (p_dsc_base = rohc_db_get_dsc(p_dsc->dpib,p_entity)))
        {
            /*Base Context Not Found - Return Failure*/
            ROHC_LOG(LOG_INFO,"DPPM :Base Context Does not exist"
                        " In IR-CR packet\n")
            *p_ecode = ROHC_INVALID_PKT;
            return ROHC_FAILURE;
        }
        else
        {
            /*replace new decompressor context with the Base decompressor
            context*/
            p_dsc->dpib->field_val[ROHC_CID_FID-ROHC_VAR_FID_BASE] = cid;

            p_dpib_base = (rohc_dpib_t *)p_dsc->dpib;
            ROHC_MEMCPY(p_dsc,p_dsc_base,sizeof(rohc_dsc_tcp_t));
            p_pkt++; /*for Reseved and Base CID */
            p_dsc->dpib = p_dpib_base;
            
            /*Parsing the P6 replicate Chain*/
            len = rohc_parse_p6_Replicate_chain(p_pkt,(rohc_dsc_tcp_t *)p_dsc);
            p_pkt+=len;
            p_dpib_base->rohc_tcp_baseCID_CRC7 = crc7_cr;
            p_dsc->dpib->rohc_tcp_baseCID = BaseCid ;
            p_dsc->dpib->profile = profile;
            p_dsc->dpib->rcv_pkt_typ = pkt_type;
            p_dsc->dpib->payload = p_pkt;
            
        }
    }
    else
    {
        ROHC_LOG(LOG_INFO,"No CR , without a Base CID"
                        " Rejecting the Pacekt\n");
        *p_ecode = ROHC_INVALID_PKT;
        return ROHC_FAILURE;
    }
    
    ROHC_EXIT_FN("parse_ir_cr_pkts");
    return ROHC_SUCCESS;
}

/************************************************************************
 * Name     :   rohc_d_decode_pkt
 * Desc     :   D-PPM
 * Params   :   p_pkt    -  Pointer to packet
 *              length   - length of packet/Datagram length 
 *              p_entity - Pointer to ROHC entity context
 *              p_p_dsc  - Pointer to DSC
 *              p_ecode  - Error code Pointer
 * Returns  :   ROHC_SUCCESS or ROHC_FAILURE
 **************************************************************************/
rohc_return_t
rohc_d_decode_pkt(
            rohc_U8bit_t         * p_pkt,
            rohc_U32bit_t        length,
            struct rohc_entity_t * p_entity,
            rohc_dsc_common_t    **  p_p_dsc,
            rohc_error_code_t    *p_ecode) 
{
    rohc_U16bit_t cid = 0;
    rohc_U8bit_t add_cid_found=0;
    rohc_U8bit_t *p_org = p_pkt;
    rohc_U8bit_t cid_len=0;
    rohc_U32bit_t feedback_size=0;
    rohc_return_t return_value = ROHC_SUCCESS;

    rohc_U8bit_t    pkt_type;

    rohc_dpib_t          *p_dpib = ROHC_NULL;
    struct rohc_protocol_node_t * feedback_node = ROHC_NULL;
    *p_p_dsc = ROHC_NULL;
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
	/* + PERF_CA */
	InternalCellIndex internalCellIndex =  PDCP_GET_CELL_INDEX(p_entity->rohc_entity_id.ueIndex);
    LteCellPdcpULPerfStats * lteCellPdcpULPerfStats =
      		   &(gPdcpCellPerfStats_p[internalCellIndex]->lteCellPdcpULPerfStats);
	/* - PERF_CA */
    rohc_U8bit_t                    ip_version=ROHC_GET_UPPER_NIBBLE(*p_pkt);
#endif

    ROHC_ENTER_FN("rohc_d_decode_pkt");
    while(*p_pkt == 0xE0)
    {
        p_pkt++;
        if(length <= (rohc_U32bit_t)(p_pkt - p_org) )
            return ROHC_FAILURE;
          
    }   /* end while skip padding*/
    
    while(1)
    {
        /* ADD-CID octet is present only if CID inclusion info is true */
        if(ROHC_GET_UPPER_NIBBLE(*p_pkt) == 0x0E)
        {
            add_cid_found=1;
            cid=ROHC_GET_LOWER_NIBBLE(*p_pkt);
            if((p_entity->large_cids != ROHC_TRUE))
            {
               
               if((p_entity->profilesSupported == ROHC_UNCOMP) && cid !=0)
               {
                  return ROHC_FAILURE;
               }   
            }  
            p_pkt++;
        }/* end if ADD CID octet*/

        if(ROHC_GET_5MSB(*p_pkt) == 0x1e)
        {
            if(cid)
            {
                ROHC_LOG(LOG_INFO,"DPPM :EID[%u:%u]: CID[%u] :"\
                    "Invalid ROHC Packet",p_entity->rohc_entity_id.ueIndex,
                     p_entity->rohc_entity_id.lcId,cid);
                *p_ecode = ROHC_INVALID_PKT;
                return ROHC_FAILURE;        
            }
            else
            {
                feedback_size=ROHC_GET_3LSB(*p_pkt);
                p_pkt++;
                if(!feedback_size)
                {
                    feedback_size=*(p_pkt);
                    p_pkt++;    
                }
                feedback_node=(struct rohc_protocol_node_t *)
                        ROHC_MALLOC(sizeof(struct rohc_protocol_node_t));
                if(!feedback_node)
                {
                    ROHC_LOG(LOG_INFO,"DPPM :EID[%u:%u]:CID[%u]:"
                                " Unable to allocate memory for feedback node",
                                p_entity->rohc_entity_id.ueIndex,
                                p_entity->rohc_entity_id.lcId,cid);

                    *p_ecode = ROHC_MEMORY_FAILURE;
                    /* Coverity Changes for 54516 */
                    /* Coverity Changes for 54516 */
                    return ROHC_FAILURE;
                }

                feedback_node->ptr=p_pkt;       
                feedback_node->length=feedback_size;        
                rohc_db_add_last_node((rohc_list_t *)
                            &ROHC_ENT_GET_FB_RCV_LIST(p_entity),
                            (rohc_list_node_t *)feedback_node); 
                p_pkt+=feedback_size;
                continue;
            }/* end else feedback packet found*/
        }/* end if*/    
        else if ( ROHC_GET_7MSB(*p_pkt) == 0x7f)
        {
            /* segment packet */
            /*TRACE segmetation is not supported in this release */
            ROHC_LOG(LOG_INFO,"DPPM :EID[%u:%u]:CID[%u]:"
                        " segment packet is recieved",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,cid);
                        
            *p_ecode = ROHC_INVALID_PKT;            
            return ROHC_FAILURE;
        }
        break;
    }   /* end while */
    if(length <= (rohc_U32bit_t)(p_pkt - p_org) )
        return ROHC_FAILURE;
        /* Forward information*/
    /* ADD-CID octet is present only if CID inclusion info is false */
    if(p_entity->large_cids == ROHC_TRUE)
    {
        if( (((*(p_pkt+1) & 0x80) == 0x00) || 
            ((*(p_pkt+1) & 0xc0) == 0x80)) &&
                !(add_cid_found))
            cid = rohc_d_sdvalue_decode((p_pkt+1),&cid_len);
        else
        {
            ROHC_LOG(LOG_INFO,"DPPM :EID[%u:%u]:"
                        " Invalid Packet recieved",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId);
            *p_ecode = ROHC_INVALID_PKT;
            return ROHC_FAILURE;
        }
    }

    p_dpib = &rohc_global.dpib;
    ROHC_MEMSET(p_dpib , 0 , sizeof(rohc_dpib_t));
    p_dpib->field_val[ROHC_CID_FID-ROHC_VAR_FID_BASE] = cid;
    if(ROHC_FAILURE == rohc_get_pkt_type(p_entity,cid,p_p_dsc,p_dpib,&pkt_type,
        p_pkt,cid_len,p_ecode))
    {
        return ROHC_FAILURE;
    }
    
    (*p_p_dsc)->dpib->rcv_pkt_typ = pkt_type;

#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
   /*for ROHC Profile ZERO */
    if(p_entity->profilesSupported == ROHC_UNCOMP)
    {
        switch(ip_version)
        {
            case 4:
                if(p_entity->rohc_entity_id.qci != 0)
                {
                     /* Update KPI Counter for the Total uncompressed header
                     * volume that has been transmitted in the Uplink 
                     * direction in the PDCP layer */
                    lteCellPdcpULPerfStats->totalRohcUnCmpHdrVolUL\
                        [p_entity->rohc_entity_id.qci-1] += 
                            sizeof(struct rohc_ip4_t);
                }
                break;

            case 6:
                if(p_entity->rohc_entity_id.qci != 0)
                {
                    /*for ROHC Profile ZERO:
                     * Update KPI Counter for the Total uncompressed header 
                     * volume that has been transmitted in the uplink 
                     * direction in the PDCP layer */
                    lteCellPdcpULPerfStats->totalRohcUnCmpHdrVolUL\
                        [p_entity->rohc_entity_id.qci-1] += 
                            sizeof(struct rohc_ip6_t);
                }
                break;
        }
    }
#endif

    return_value = rohc_dppm_decode_all_pkt_types(p_entity, p_p_dsc, p_pkt,
                                             &cid_len, &add_cid_found, p_ecode);
    if (ROHC_FAILURE == return_value)
    {
        return ROHC_FAILURE;
    }

    (*p_p_dsc)->dpib->payload_len = length - 
	    ((*p_p_dsc)->dpib->payload - p_org);
#if defined(PERF_STATS) && !defined(UE_SIM_TESTING)
   /*for ROHC Profile ONE and TWO */
    if(p_entity->profilesSupported != ROHC_UNCOMP)
    {
        if(p_entity->rohc_entity_id.qci != 0)
        {
            /* Update KPI Counter for the Total compressed header volume that has been
             * received in the Uplink  direction in the PDCP layer */
            lteCellPdcpULPerfStats->totalRohcCmpHdrVolUL\
                [p_entity->rohc_entity_id.qci - 1] += ((*p_p_dsc)->dpib->payload - p_org);
        }
    }
#endif

    /* Incrementing Stats */
    /*SPR 3444 changes starts */
    if(pkt_type < ROHC_MAX_PKT_TYPES)
    {
        rohcStatsPktTypeUpdate(rohc_global_pkt_types_to_stats_pr[pkt_type],ROHC_DIR_RX,
                (*p_p_dsc)->profile);
    }
    /*SPR 3444 changes ends */

    ROHC_LOG(LOG_INFO,"DPPM :EID[%u:%u]:CID[%u]: Packet "\
                    " type [%d] is parsed ",
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,cid,pkt_type);

    ROHC_EXIT_FN("rohc_d_decode_pkt");
    return ROHC_SUCCESS;
} /* end function rohc_d_decode_pkt */

/************************************************************************
 * Name     :   rohc_get_pkt_type
 * Desc     :   D-PPM
 * Params   :   p_entity - Pointer to ROHC entity context
 *              cid    -   context id
 *              p_p_dsc  - Pointer to DSC
 *              p_dpib  - Pointer to DPIB
 *              p_pkt_type  - Pointer to Pkt type
 *              p_pkt  -  Pointer to Pkt 
 *              cid_len   - length of cid 
 *              p_ecode  - Error code Pointer
 * Returns  :   ROHC_SUCCESS or ROHC_FAILURE
 **************************************************************************/
STATIC rohc_return_t
rohc_get_pkt_type(
            struct rohc_entity_t * p_entity,
            rohc_U16bit_t cid,
            rohc_dsc_common_t    **  p_p_dsc,
            rohc_dpib_t          *p_dpib,
            rohc_U8bit_t         *p_pkt_type,
            rohc_U8bit_t         * p_pkt,
            rohc_U8bit_t         cid_len,
            rohc_error_code_t    *p_ecode)
{
    register rohc_U8bit_t    pkt_type;

    if( (p_entity->uncomp_dsc) && 
            (cid == ((rohc_dsc_common_t*)p_entity->uncomp_dsc)->cid) )
    {
        *p_p_dsc = p_entity->uncomp_dsc;
        (*p_p_dsc)->dpib = p_dpib;
        pkt_type = rohc_packet_type_det(p_entity,p_pkt,cid_len,*p_p_dsc);
    }
    else if((*p_p_dsc = rohc_db_get_dsc(p_dpib,p_entity)) == ROHC_NULL)
    {
        if( ((*p_pkt&0xfe) == ROHC_IR_RES_TYPE) &&
                ((pkt_type = rohc_packet_type_det(p_entity,p_pkt,cid_len,
                               *p_p_dsc))!= ROHC_IR_WITHOUT_DYN_PKTTYPE))
        {
            if((*p_p_dsc = rohc_db_create_new_dsc(cid,p_entity)) == ROHC_NULL)
            {
                ROHC_LOG(LOG_INFO,"DPPM :EID[%u:%u]:CID[%u]:"
                        " Unable to allocate memory for dsc",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,cid);

                *p_ecode = ROHC_MEMORY_FAILURE;
                return ROHC_FAILURE;
            }
            (*p_p_dsc)->dpib = p_dpib;
        }
        else
        {
            ROHC_LOG(LOG_INFO,"DPPM :EID[%u:%u]:CID[%u]:"
                    " Invalid Packet recieved",
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,cid);
            *p_ecode = ROHC_INVALID_PKT;
            return ROHC_FAILURE;

        }
    }
    else
    {
        (*p_p_dsc)->dpib = p_dpib;
        pkt_type = rohc_packet_type_det(p_entity,p_pkt,cid_len,*p_p_dsc);
        if(ROHC_FAILURE == rohc_dcpm_validate_pkt(p_entity,*p_p_dsc,pkt_type))
        {
            ROHC_LOG(LOG_INFO,"DPPM :EID[%u:%u]:CID[%u]:"
                    " Invalid Packet recieved",
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,cid);

            *p_ecode = ROHC_INVALID_PKT;
            (*p_p_dsc)->dpib->rcv_pkt_typ = pkt_type;
            return ROHC_FAILURE;
        }
    }
    *p_pkt_type = pkt_type;

    return ROHC_SUCCESS;
}

/************************************************************************
 * Name     :   rohc_dppm_decode_all_pkt_types
 * Desc     :   D-PPM
 * Params   : 
 *              ROHC entity context
 *              DSC 
 *              IP Datagram
 *              CID length 
 *              ADD CID found
 *              Error code
 * Returns  :   ROHC_SUCCESS or ROHC_FAILURE
 **************************************************************************/
rohc_return_t
rohc_dppm_decode_all_pkt_types
        ROHC_PARAMS((p_entity,p_p_dsc,p_pkt,cid_length,add_cid_f,p_ecode),
            struct rohc_entity_t * p_entity     _AND_
            rohc_dsc_common_t    **  p_p_dsc    _AND_
            rohc_U8bit_t         * p_pkt        _AND_
            rohc_U8bit_t         * cid_length   _AND_
            rohc_U8bit_t         * add_cid_f    _AND_
            rohc_error_code_t    *p_ecode) 
{
    rohc_U8bit_t    pkt_type=0;
    rohc_U8bit_t    crc=0;
    rohc_U32bit_t   len = 0;
    rohc_U8bit_t    add_cid_found=*add_cid_f;
    rohc_U8bit_t    cid_len=*cid_length;

    pkt_type = (*p_p_dsc)->dpib->rcv_pkt_typ;
    switch(pkt_type)
    {
        case ROHC_NORMALP0_PKTYPE:
            switch(cid_len)
            {
                case 0: (*p_p_dsc)->dpib->payload = p_pkt;
                        break;
                case 1: *(p_pkt+1) = *p_pkt;
                        (*p_p_dsc)->dpib->payload = p_pkt+1;
                        break;
                case 2: *(p_pkt+2) = *p_pkt;
                        (*p_p_dsc)->dpib->payload = p_pkt+2;
                        break;
            }
            break;
        case ROHC_IRP0_PKTYPE:
            if(*(p_pkt+cid_len+2) ==
                    rohc_compute_crc(ROHC_CRC_TYPE_8,
                        (p_pkt-add_cid_found),
                        (add_cid_found+cid_len+2),0xff))
            {
                (*p_p_dsc)->dpib->payload = p_pkt+cid_len+3;
                (*p_p_dsc)->dpib->profile = ROHC_UNCOMP;
                p_entity->uncomp_dsc = *p_p_dsc;
                ((rohc_dsc_uncomp_t*)p_entity->uncomp_dsc)->profile = 
                    ROHC_UNCOMP;
                /* START_SPR_562_FIX */
                /*
                 ** Decompressor state was updated incorrectly, there was no 
                 ** consideration for ROHC Profile 0. Hence introduced a 
                 ** specific check for profile-0 here
                 ** and corrected macro usage to ROHC_DUN_ST_NC
                 */
                (*p_p_dsc)->d_state = ROHC_DUN_ST_NC; /* for ROHC 0 */
                /* END_SPR_562_FIX */
                (*p_p_dsc)->decom_res = ROHC_SUCCESS;
                break;
            }
            else
            {
                ROHC_UT_TRACE((stderr,"DPPM :EID[%u:%u]:CID[%u]:"
                            " CRC Check failed for IRP0 packet\n",
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId,cid));
                /* SPR 9591 fix start */
                ROHC_LOG(LOG_INFO,"DPPM :EID[%u:%u]:CID[%u]:"
                        " CRC Check failed for IRP0 packet",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,
                        ROHC_GET_DSC_CID(*p_p_dsc));
                /* SPR 9591 fix end */

                *p_ecode = ROHC_CRC_CHECK_FAILED;
                (*p_p_dsc)->decom_res = ROHC_FAILURE;

                /* Incrementing Stats */
                pdcpStats.statsRohc.numberOfTimesCrcFailed++;

                return ROHC_FAILURE;
            }
        case ROHC_IR_WITHOUT_DYN_PKTTYPE:
        case ROHC_IR_WITH_DYN_PKTTYPE:
        case ROHC_IRDYN_PKTTYPE:
        case ROHC_IRP6CR_PKTYPE :
                if(ROHC_IRP6CR_PKTYPE == pkt_type)
                {
                    /*In case the packet is of type IR_CR for Profile 6*/
                    if( ROHC_FAILURE == parse_ir_cr_pkts(p_entity,pkt_type,
                                    p_pkt+cid_len+1,*p_p_dsc,p_ecode) )
                    {
                        (*p_p_dsc)->decom_res = ROHC_FAILURE;
                        return ROHC_FAILURE;
                    }
                }
                else
                {
                    if( ROHC_FAILURE == parse_ir_irdyn_pkts(p_entity,pkt_type,
                                p_pkt+cid_len+1,*p_p_dsc,p_ecode) )
                    {
                        (*p_p_dsc)->decom_res = ROHC_FAILURE;
                        return ROHC_FAILURE;
                    }
                }
                crc = *(p_pkt+cid_len+2);
                *(p_pkt+cid_len+2) = 0;
                if(crc !=  rohc_compute_crc(ROHC_CRC_TYPE_8,
                            (p_pkt-add_cid_found),
                            (add_cid_found+ 
                                ((*p_p_dsc)->dpib->payload - p_pkt)),0xff) )
                {
                    (*p_p_dsc)->decom_res = ROHC_FAILURE;
                    ROHC_UT_TRACE((stderr,"DPPM :EID[%u:%u]:CID[%u]:"
                            " CRC Check failed for IR packet\n",
                            p_entity->rohc_entity_id.ueIndex,
                            p_entity->rohc_entity_id.lcId,
                            cid));
                    /* SPR 9591 fix start */
                    ROHC_LOG(LOG_INFO,"DPPM :EID[%u:%u]:CID[%u]:"
                        " CRC Check failed for IR packet",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,
                        ROHC_GET_DSC_CID(*p_p_dsc));
                    *p_ecode = ROHC_CRC_CHECK_FAILED;
                    /* SPR 9591 fix end */

                /* Incrementing Stats */
                pdcpStats.statsRohc.numberOfTimesCrcFailed++;

                return ROHC_FAILURE;
            }
            if((*p_p_dsc)->profile != (*p_p_dsc)->dpib->profile)
            {
                (*p_p_dsc)->profile = (*p_p_dsc)->dpib->profile;
                /* SPR 6174 changes start*/
                if ( ( ROHC_IR_WITHOUT_DYN_PKTTYPE == pkt_type
                       || ( ROHC_IR_WITH_DYN_PKTTYPE == pkt_type )
                       || ( ROHC_IRP6CR_PKTYPE == pkt_type )))
                {
                        (*p_p_dsc)->d_state = ROHC_D_ST_NC; 
                        /* for ROHC Profile 1 , 2 ,3  */
                }
                else
                {
                    (*p_p_dsc)->d_state = ROHC_D_ST_SC;
                }
                /* SPR 6174 changes end*/
                /* SPR 3444 changes start */
                         
                if (!(( ROHC_IP_UDP_RTP  == (*p_p_dsc)->profile)
                || ( ROHC_UNCOMP  == (*p_p_dsc)->profile )
                || ( ROHC_IP_UDP == (*p_p_dsc)->profile )
                || ( ROHC_IP_TCP  == (*p_p_dsc)->profile )))
        	/* SPR 3444 changes end */
                    /* SPR 3444 changes ends*/
                {
                    return ROHC_FAILURE;
                }

                if((*rohc_create_dsc_sub_blocks_fpt
                            [(*p_p_dsc)->profile - ROHC_UNCOMP]) != NULL)
                {
                    (*rohc_create_dsc_sub_blocks_fpt
                     [(*p_p_dsc)->profile - ROHC_UNCOMP])
                        (*p_p_dsc);
                }
                /* these to be shifted to DB*/
            }
            break;
        case ROHC_CO_P6_COMMON_PKTYPE:
        case ROHC_CO_P6_RND1_PKTYPE :
        case ROHC_CO_P6_RND2_PKTYPE :
        case ROHC_CO_P6_RND3_PKTYPE :
        case ROHC_CO_P6_RND4_PKTYPE :
        case ROHC_CO_P6_RND5_PKTYPE :
        case ROHC_CO_P6_RND6_PKTYPE :
        case ROHC_CO_P6_RND7_PKTYPE :
        case ROHC_CO_P6_RND8_PKTYPE :
        case ROHC_CO_P6_SEQ1_PKTYPE :
        case ROHC_CO_P6_SEQ2_PKTYPE :
        case ROHC_CO_P6_SEQ3_PKTYPE :
        case ROHC_CO_P6_SEQ4_PKTYPE :
        case ROHC_CO_P6_SEQ5_PKTYPE :
        case ROHC_CO_P6_SEQ6_PKTYPE :
        case ROHC_CO_P6_SEQ7_PKTYPE :
        case ROHC_CO_P6_SEQ8_PKTYPE :
                (*p_p_dsc)->dpib->profile = ROHC_IP_TCP;
                if(ROHC_FAILURE == (*rohc_d_decode_value_fpt
                          [(*p_p_dsc)->dpib->profile - ROHC_UNCOMP])
                          (*p_p_dsc,p_pkt,pkt_type,&len,p_entity,p_ecode))
                {
                    /* SPR 9591 fix start */
                    ROHC_LOG(LOG_INFO,"Decode value :EID[%u:%u]:CID[%u]:" \
                                  "Decomp decode value fails" \
                                  " for Profile [%d]  ",
                                  p_entity->rohc_entity_id.ueIndex,
                                  p_entity->rohc_entity_id.lcId,
                                  ROHC_GET_DSC_CID(*p_p_dsc),
                                  (*p_p_dsc)->profile);
                    /* SPR 9591 fix end */
                    return ROHC_FAILURE;
                 }
            p_pkt+=(len);
            (*p_p_dsc)->dpib->payload = p_pkt;
            break;
        default:
            /* SPR 3444 changes start */
            if((*p_p_dsc)->profile >2)
                /* SPR 3444 changes end */
            {
                return ROHC_FAILURE;
            }
            if(ROHC_FAILURE == (*rohc_d_decode_value_fpt
                        [(*p_p_dsc)->profile - ROHC_UNCOMP])
                    (*p_p_dsc,p_pkt,pkt_type,&len,p_entity,p_ecode))
            {
                /* SPR 9591 fix start */
                ROHC_LOG(LOG_INFO,"Decode value :EID[%u:%u]:CID[%u]:" \
                        "Decomp decode value fails" \
                        " for Profile [%d]  ",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,
                        ROHC_GET_DSC_CID(*p_p_dsc),
                        (*p_p_dsc)->profile);
                /* SPR 9591 fix end */
                return ROHC_FAILURE;
            }
            p_pkt+=(len);
            /* parsing the remaining header */

            if((*p_p_dsc)->rnd2 == ROHC_RND_PRESENT)
            {
                (*p_p_dsc)->dpib->field_val[ROHC_OUTERIP_ID_FID
                    - ROHC_VAR_FID_BASE] = ROHC_GET_2BYTES(p_pkt);
                (*p_p_dsc)->dpib->field_len[ROHC_OUTERIP_ID_FID
                    - ROHC_VAR_FID_BASE] =16;
                p_pkt+=2;
            }   
            if(((*p_p_dsc)->dpib->outer_ah_ptr)&&
                    ((((((rohc_ah_t*)(*p_p_dsc)->dpib->outer_ah_ptr)->len)+2)*4)
                     - sizeof(rohc_ah_t)))
            {
                ROHC_MEMCPY((*p_p_dsc)->dpib->outer_ah_ptr + sizeof(rohc_ah_t),
                        p_pkt,
                        ((rohc_ah_t*)(*p_p_dsc)->dpib->outer_ah_ptr)->len);
                p_pkt+=((rohc_ah_t*)(*p_p_dsc)->dpib->outer_ah_ptr)->len;
            }
            if(((rohc_gre_t*)(*p_p_dsc)->dpib->outer_gre)&&
                    (((rohc_gre_t*)(*p_p_dsc)->dpib->outer_gre)->flags & ROHC_GRE_CP))
            {
                ROHC_ASSIGN_2_OCTS(p_pkt,((*p_p_dsc)->dpib->outer_gre 
                            + sizeof(rohc_gre_t)));
                p_pkt+=2;
            }

            /* SPR5814 Fix Start */
            /* IP ID as uncompressed should be decoded when
             * No ext3 with IP flag set and context has RND bit set or
             * Ext3 with IP flag is present with RND bit as 1 regardless 
             * of the store context value 
             */

            if(((0 == (((*p_p_dsc)->dpib->field_val[ROHC_EXT3_FLAGS_FID - 
                    ROHC_VAR_FID_BASE]) & ROHC_ENC_INNERIPFLAG_EXT3FLAGP1_MASK))
                && ((*p_p_dsc)->rnd == ROHC_RND_PRESENT)) 
              || ((((*p_p_dsc)->dpib->field_val[ROHC_EXT3_FLAGS_FID-
                    ROHC_VAR_FID_BASE]) & ROHC_ENC_INNERIPFLAG_EXT3FLAGP1_MASK)
              && (((*p_p_dsc)->dpib->field_val[ROHC_INNERIP_FLAGS_FID-
                    ROHC_VAR_FID_BASE]) & ROHC_ENC_RND_INNERIPFLAGP1_MASK)))
            {
                /* SPR5814 Fix End */
                (*p_p_dsc)->dpib->field_val[ROHC_INNERIP_ID_FID
                    - ROHC_VAR_FID_BASE] = ROHC_GET_2BYTES(p_pkt);
                (*p_p_dsc)->dpib->field_len[ROHC_INNERIP_ID_FID
                    - ROHC_VAR_FID_BASE]=16;
                p_pkt+=2;
            }
            if(((*p_p_dsc)->dpib->inner_ah_ptr)&&
                    ((((((rohc_ah_t*)(*p_p_dsc)->dpib->inner_ah_ptr)->len)+2)*4)
                     - sizeof(rohc_ah_t)))
            {
                ROHC_MEMCPY((*p_p_dsc)->dpib->inner_ah_ptr + sizeof(rohc_ah_t),
                        p_pkt,
                        ((rohc_ah_t*)(*p_p_dsc)->dpib->inner_ah_ptr)->len);
                p_pkt+=((rohc_ah_t*)(*p_p_dsc)->dpib->inner_ah_ptr)->len;
            }
            if(((rohc_gre_t*)(*p_p_dsc)->dpib->inner_gre)&&
                    (((rohc_gre_t*)(*p_p_dsc)->dpib->inner_gre)->flags & ROHC_GRE_CP))
            {
                ROHC_ASSIGN_2_OCTS(p_pkt,((*p_p_dsc)->dpib->inner_gre 
                            + sizeof(rohc_gre_t)));
                p_pkt+=2;
            }
            if((*p_p_dsc)->udp_cksum_present)
            {
                (*p_p_dsc)->dpib->field_val[ROHC_UDP_CKSUM_FID -
                    ROHC_VAR_FID_BASE]=ROHC_GET_2BYTES(p_pkt);
                p_pkt+=2;
            }

            (*p_p_dsc)->dpib->payload = p_pkt;
    }
    return ROHC_SUCCESS;
}
/****************************************************************************
 * Name     :   rohc_sdvl_len
 * Desc     :   determine no. of bits in self describinf variable length value
 * Params   :   value
 * Returns  :   no . of bytes in value passed
 **************************************************************************/
rohc_U8bit_t 
rohc_sdvl_len
    ROHC_PARAMS((val),
    rohc_U8bit_t * val)
{
    ROHC_ENTER_FN("rohc_sdvl_len");
    if(!(*val&0x80))
        return 7;
    if((*val&0xc0) == 0x80)
        return 14;
    if((*val&0xe0) == 0xc0)
        return 21;
    if((*val&0xe0) == 0xe0)
        return 29;
    ROHC_EXIT_FN("rohc_sdvl_len");
    return  0;  
} /* end function rohc_sdvl_len */


/*Parsing the field lengths and field values ********/
/****************************************************************************
 * Name     :   rohcP6COParseFID
 * Desc     :   Function to parse the compressed and Common compressed packets
 * Params   :   Pointer to TCP - DSC context
 *          :   Pointer to ROHC Packet received
 *          :   Number of bits of field type
            :   Field Identifier
 *          :   no_of_bits_occ for Split type FID
 * Returns  :   - parsed length
 **************************************************************************/
rohc_return_t  
rohcP6COParseFID(
    struct rohc_dsc_tcp_t   * p_dsc,
    rohc_U8bit_t            ** p_rohc_pkt,
    rohc_U8bit_t            no_of_bits,
    rohc_U8bit_t            field_id,
    rohc_U8bit_t            len)
{
    rohc_U8bit_t temp_len=0,temp_field_val=0,n=0,no_of_bytes=0;
    static rohc_U8bit_t    from_bit_pos=0;

    /* Traverse each field in packet type */

    if ( ( ROHC_ENC_MAX_FIXED_BITS_IN_FIELD >= no_of_bits)
      || (ROHC_ENCTYPE_SPLIT_FIELD == no_of_bits))
    {   
        /* fixed length encoding */
        /* SPR 3532 - Klocwork Fix */ 
        if(ROHC_VAR_FID_BASE <= field_id)
        {   /*Field Values dependent on stream context */
            if (ROHC_ENCTYPE_SPLIT_FIELD != no_of_bits) 
            {
                n=no_of_bits;
                if(field_id >= ROHC_PROF6_VAR_CTRL_FID_BASE)
                {
                    p_dsc->dpib->field_len[ROHC_MAX_FIELD_TYPES
                    -ROHC_VAR_FID_BASE + field_id -
                    ROHC_PROF6_VAR_CTRL_FID_BASE]+= no_of_bits;
                }
                else
                {
                    /*coverity Fix 40442 and 40445 START*/
                    if((field_id >= ROHC_VAR_FID_BASE) &&
                            (field_id <= ROHC_TCP_ACKNO_FID))
                    {
                        p_dsc->dpib->field_len[field_id-ROHC_VAR_FID_BASE]
                            +=no_of_bits;
                    }
                    else
                    {
                        return ROHC_FAILURE;
                    }
                    /*coverity Fix 40442 and 40445 STOP*/

                }
                no_of_bytes=(no_of_bits+7)/8; 
            }
            else
            {
                if(field_id >= ROHC_PROF6_VAR_CTRL_FID_BASE)
                {
                    p_dsc->dpib->field_len[ROHC_MAX_FIELD_TYPES
                    -ROHC_VAR_FID_BASE + field_id -  
                    ROHC_PROF6_VAR_CTRL_FID_BASE]+= len;
                }
                else
                {
                    /*coverity Fix 40446 START*/
                    if((field_id >= ROHC_VAR_FID_BASE) &&
                            (field_id <= ROHC_TCP_ACKNO_FID))
                    {
                        p_dsc->dpib->field_len[field_id-ROHC_VAR_FID_BASE]+= len;
                    }
                    else
                    {
                        return ROHC_FAILURE;
                    }
                    /*coverity Fix 40446 STOP*/
                }
                n=len;
                no_of_bytes=(len+7)/8; 
             }
                
            while(no_of_bytes)
            {
                if(n%8)
                {
                    temp_field_val=(1U<<(n%8))-1;
                    temp_field_val&=((**p_rohc_pkt)>>(8-(n%8)-from_bit_pos));
                    if(field_id >= ROHC_PROF6_VAR_CTRL_FID_BASE)
                    {
                        p_dsc->dpib->field_val[ROHC_MAX_FIELD_TYPES 
                        -ROHC_VAR_FID_BASE + field_id -
                        ROHC_PROF6_VAR_CTRL_FID_BASE]=
                            (p_dsc->dpib->field_val[ROHC_MAX_FIELD_TYPES
                             -ROHC_VAR_FID_BASE + field_id -
                             ROHC_PROF6_VAR_CTRL_FID_BASE]
                             <<(n%8)) | temp_field_val;                        
                    }
                    else
                    {
                        /*coverity 40446 START*/
                        if((field_id >= ROHC_VAR_FID_BASE) &&
                                (field_id <= ROHC_TCP_ACKNO_FID))
                        {
                        p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]=
                            (p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]
                             <<(n%8)) | temp_field_val;
                        }
                        else
                        {
                            return ROHC_FAILURE;
                        }
                        /*coverity FIX 40446 STOP*/
                    }
                    (*p_rohc_pkt)+=(from_bit_pos+n%8)/8;
                    from_bit_pos=(from_bit_pos+n%8)%8;
                    n-=n%8;
                }
                else
                {
                    if(field_id >= ROHC_PROF6_VAR_CTRL_FID_BASE)
                    {
                        p_dsc->dpib->field_val[ROHC_MAX_FIELD_TYPES
                        -ROHC_VAR_FID_BASE + field_id -
                        ROHC_PROF6_VAR_CTRL_FID_BASE]=
                            (p_dsc->dpib->field_val[ROHC_MAX_FIELD_TYPES
                            -ROHC_VAR_FID_BASE + field_id -
                            ROHC_PROF6_VAR_CTRL_FID_BASE]
                             <<8) | (**p_rohc_pkt);
                    }
                    else
                    {
                        /*coverity 40442 and 40446 START*/
                        if((field_id >= ROHC_VAR_FID_BASE) &&
                                (field_id <= ROHC_TCP_ACKNO_FID))
                        {
                            p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]=
                                (p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]
                                 <<8) | (**p_rohc_pkt);
                        }
                        else
                        {
                            return ROHC_FAILURE;
                        }
                        /*coverity FIX 40442 and 40446 STOP*/
                    }
                    (*p_rohc_pkt)++;
                }
                no_of_bytes--;
            }
        } /*end if */
        /* SPR 3532 - Klocwork Fix */
        else if((ROHC_MAX_CONSTANT_FID < field_id) 
                && (ROHC_VAR_FID_BASE > field_id))
        {
            ROHC_LOG(LOG_INFO,"Invalid value of field_id");
            return ROHC_FAILURE;
        }
        else
        {
            from_bit_pos+=no_of_bits;
        }
    }/* end if fixed no of bits encoding */
    /* This evaluated to true only in case of RND 8 where TCP options are 
     * present in base header of compressed packet */
    else if(ROHC_ENCTYPE_TCP_OPTIONS_LIST == no_of_bits)
    {
        /* Checking for Presence Indicator */
        if (((p_dsc)->dpib->field_val
                [ROHC_TCP_LIST_PRESENT_FID- ROHC_TCP_CTRL_FID_BASE]))
        {
            /* Parsing the tcp options for RND 8 */
            rohcDecTCPOptionsP6(*p_rohc_pkt,&temp_len,
                p_dsc->dpib);
            (*p_rohc_pkt)+=temp_len;
        }

    }
    return ROHC_SUCCESS;
}/* end function rohcP6COParseFID*/

/*Generic Packet Decoding Routine for irregular chain of TCP options ********/
/****************************************************************************
 * Name     :   rohcTcpTsOptionIrregular
 * Desc     :   Function to parse the TS tcp option present in irregular chain
 *              of IP/TCP packet
 * Params   :   p_dsc - Pointer to DSC context
 *          :   p_rohc_pkt -Pointer to ROHC Packet received
 *          :   field_id - Field Identifier
 * Returns  :   - parsed length
 **************************************************************************/
rohc_return_t
rohcTcpTsOptionIrregular(
    rohc_dsc_tcp_t          * p_dsc,
    rohc_U8bit_t            ** p_rohc_pkt,
    rohc_U8bit_t            field_id)
{

    ROHC_ENTER_FN("rohcTcpTsOptionIrregular");

    /* ROHC_TS_VAL_7_DISC = 0x00, ROHC_TS_VAL_7_DESC_MASK = 0x80 */
    if (ROHC_TS_VAL_7_DISC == (**p_rohc_pkt & ROHC_TS_VAL_7_DESC_MASK))         
    {
	    p_dsc->dpib->field_val[field_id -ROHC_VAR_FID_BASE]=
		    (**p_rohc_pkt) & ROHC_TS_7_NUM_BITS;
	    (*p_rohc_pkt)++;

	    p_dsc->dpib->field_len[field_id -ROHC_VAR_FID_BASE]=7;
    }
      /* ROHC_TS_VAL_14_DISC = 0x80, ROHC_TS_VAL_14_DESC_MASK = 0xC0*/
    else if (ROHC_TS_VAL_14_DISC == (**p_rohc_pkt & ROHC_TS_VAL_14_DESC_MASK)) 
    {
        p_dsc->dpib->field_val[field_id -ROHC_VAR_FID_BASE]=
            (((**p_rohc_pkt) & 0x3F)<<8);  
        (*p_rohc_pkt)++;
        p_dsc->dpib->field_val[field_id -ROHC_VAR_FID_BASE] |=
            **p_rohc_pkt;  
        (*p_rohc_pkt)++;
        p_dsc->dpib->field_len[field_id -ROHC_VAR_FID_BASE]=14;
    } /* ROHC_TS_VAL_21_DISC = 0xC0, ROHC_TS_VAL_21_29_DISC = 0xE0*/
    else if (ROHC_TS_VAL_21_DISC == (**p_rohc_pkt & ROHC_TS_VAL_21_29_DISC)) 
    {
        p_dsc->dpib->field_val[ field_id -ROHC_VAR_FID_BASE]=
          ( ((**p_rohc_pkt) & 0x1F)<<16);  
        (*p_rohc_pkt)++;
        p_dsc->dpib->field_val[ field_id -ROHC_VAR_FID_BASE]|=
          (**p_rohc_pkt) <<8 ; 
        (*p_rohc_pkt)++;
        p_dsc->dpib->field_val[ field_id -ROHC_VAR_FID_BASE]|=
             **p_rohc_pkt;  
        (*p_rohc_pkt)++;
        p_dsc->dpib->field_len[ field_id - ROHC_VAR_FID_BASE]=21;
    }                /* ROHC_TS_VAL_21_29_DISC = 0xE0 */
    else if (ROHC_TS_VAL_21_29_DISC == (**p_rohc_pkt & ROHC_TS_VAL_21_29_DISC) ) 
    {
        p_dsc->dpib->field_val[field_id  -ROHC_VAR_FID_BASE]=
          (((**p_rohc_pkt) & 0x1F)<<24 );  
        (*p_rohc_pkt)++;
        p_dsc->dpib->field_val[ field_id -ROHC_VAR_FID_BASE]|=
          (**p_rohc_pkt) <<16; 
        (*p_rohc_pkt)++;
        p_dsc->dpib->field_val[ field_id -ROHC_VAR_FID_BASE]|=
            **p_rohc_pkt <<8; 
        (*p_rohc_pkt)++;
        p_dsc->dpib->field_val[ field_id -ROHC_VAR_FID_BASE]|=
            **p_rohc_pkt;  
        (*p_rohc_pkt)++;
        p_dsc->dpib->field_len[ field_id -ROHC_VAR_FID_BASE]=29;
    }

    ROHC_EXIT_FN("rohcTcpTsOptionIrregular");
    return ROHC_SUCCESS;
}/* end of rohcTCP_TS_OptionIrregular function */


/*Generic Packet Decoding Routine for compressed packets ********/
/****************************************************************************
 * Name     :   rohc_d_decode_p1_value
 * Desc     :   D-PPM
 * Params   :   Pointer to DSC context
 *          :   Pointer to ROHC Packet received
 *          :   Packet type
 *          :   ROHC Packet length(Output parameter)
 *          :   Pointer to ROHC entity context
 *			: 	p_ecode  - Error code Pointer
 * Returns  :   - parsed length
 **************************************************************************/
rohc_return_t  
rohc_d_decode_p1_value
    ROHC_PARAMS((p_dsc,p_rohc_pkt,pkt_type,p_len,p_entity,p_ecode),
     /* + SPR 17439 */
    rohc_dsc_common_t   * p_dsc         _AND_
     /* - SPR 17439 */
    rohc_U8bit_t            * p_rohc_pkt    _AND_
    register rohc_U8bit_t   pkt_type        _AND_
    rohc_U32bit_t           *p_len          _AND_
    struct rohc_entity_t    * p_entity      _AND_
    rohc_error_code_t       *p_ecode)
{
    rohc_U32bit_t   i=0,n=0;
    rohc_U8bit_t    no_of_bits=0,no_of_bytes=0;
    rohc_U8bit_t    from_bit_pos=0;
    rohc_U8bit_t    * p_rohc_org_pkt=p_rohc_pkt;
    rohc_U8bit_t    field_id,temp_len=0;    
    rohc_U32bit_t   temp_field_val=0;
    ROHC_ENTER_FN("rohc_d_decode_p1_value");
    for(i=0;i<ROHC_MAX_NO_OF_FIELDS_IN_A_PACKET && 
        rohc_c_pkt_type[pkt_type][i].field_id!=ROHC_INVALID_FID;i++)
    {

        /* Traverse each field in packet type */
        no_of_bits=rohc_c_pkt_type[pkt_type][i].no_of_bits; 
        field_id=rohc_c_pkt_type[pkt_type][i].field_id;
        
        if(no_of_bits<=ROHC_ENC_MAX_FIXED_BITS_IN_FIELD)
        {   
            /* fixed length encoding */
       		/* SPR 3532 - Klocwork Fix */ 
            if(field_id >= ROHC_VAR_FID_BASE)
            {   /*Field Values dependent on stream context */
                n=no_of_bits;
                no_of_bytes=(no_of_bits+7)/8;
                while(no_of_bytes)
                {
                    if(n%8)
                    {
                        temp_field_val=(1U<<(n%8))-1;
                        temp_field_val&=((*p_rohc_pkt)>>(8-(n%8)-from_bit_pos));
                        p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]=
                        (p_dsc->dpib->field_val[field_id-
                               ROHC_VAR_FID_BASE]<<(n%8))|temp_field_val;

                        p_rohc_pkt+=(from_bit_pos+n%8)/8;
                        from_bit_pos=(from_bit_pos+n%8)%8;
                        n-=n%8;
                    }
                    else
                    {
                        p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]=
                        (p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]<<8)
                                        |(*p_rohc_pkt);
                        p_rohc_pkt++;
                    }
                    no_of_bytes--;
                }
                p_dsc->dpib->field_len[field_id-ROHC_VAR_FID_BASE]+=no_of_bits;
/*              temp_field_val=(1U<<no_of_bits)-1;
                temp_field_val= temp_field_val << 
                        (no_of_bits>8?(32-no_of_bits-from_bit_pos):
                        (8-no_of_bits-from_bit_pos));
                temp_field_val&=*(p_rohc_pkt);      
                p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]=
                (p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]<<
                no_of_bits)|temp_field_val;
                p_rohc_pkt+=(from_bit_pos+no_of_bits)/8;
                from_bit_pos=(from_bit_pos+no_of_bits)%8;
*/
            } /*end if */
			/* SPR 3532 - Klocwork Fix */
			else if((field_id > ROHC_MAX_CONSTANT_FID) && 
                    (field_id < ROHC_VAR_FID_BASE))
			{
			  ROHC_LOG(LOG_INFO,"Invalid value of field_id");
			  return ROHC_FAILURE;
			}
            else
            {
                from_bit_pos+=no_of_bits;
            }
        }/* end if fixed no of bits encoding */
        else if(no_of_bits == ROHC_ENCTYPE_VAR_NO_BITS)
        {
            if((field_id != ROHC_CID_FID)||
                (p_entity->large_cids == ROHC_TRUE))
            {
                p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE] =
                         rohc_d_sdvalue_decode( p_rohc_pkt,&temp_len);
                p_rohc_pkt+=temp_len;
            }
        }
        else if(no_of_bits == ROHC_ENCTYPE_EXT3FLAGSP1)
        {
            /*Encoding based on extension 3 flags */
            /* +- SPR 17777 */
            rohc_dec_ele_p1_ext3flagsp1(&p_rohc_pkt,
                            field_id,p_dsc->dpib);
        }   /* end if no_of_bits == EXT 3 flags */
        else if(no_of_bits == ROHC_ENCTYPE_INNERIPFLAGSP1)
        {
     /* + SPR 17439 */
            /*Encoding based on inenr ip flags */
            /* +- SPR 17777 */
            if(ROHC_FAILURE == rohc_dec_ele_p1_inneripflagsp1(&p_rohc_pkt,
                    field_id,(rohc_dsc_rtp_t*)p_dsc,p_entity,p_ecode))
                return ROHC_FAILURE;
        }   /* end if no_of_bits == ROHC_ENCTYPE_INNERIPFLAGSP1*/
        else if(no_of_bits == ROHC_ENCTYPE_OUTERIPFLAGSP1)
        {
            /*Encoding based on outer ip flags */
            if(ROHC_FAILURE == rohc_dec_ele_p1_outeripflagsp1(&p_rohc_pkt,
                    field_id,(rohc_dsc_rtp_t*)p_dsc,p_entity,p_ecode))
                return ROHC_FAILURE;
        }/* end if no_of_bits == ROHC_ENCTYPE_OUTERIPFLAGSP1*/
        else if(no_of_bits == ROHC_ENCTYPE_RTPFLAGSP1)
        {
            /*Encoding based on rtp ip flags */
            if(ROHC_FAILURE == rohc_dec_ele_p1_flagsp1(&p_rohc_pkt,
                    field_id,(rohc_dsc_rtp_t*)p_dsc,p_entity,p_ecode))
     /* - SPR 17439 */
                return ROHC_FAILURE;
        }/*end if no_of_bits == ROHC_ENCTYPE_RTPFLAGSP1 */
        /* +- SPR 17777 */
        
        else if(no_of_bits == ROHC_ENCTYPE_SPLIT_FIELD)
        { /* split field always in base headers */
            rohc_U32bit_t   len;
            len=rohc_c_pkt_type[pkt_type][i].no_of_bits_occ;
            temp_field_val=(1U<<len)-1;
            temp_field_val&=((*p_rohc_pkt)>>(8-len-from_bit_pos));      
            p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]=
            (p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]<<len)
                                        |temp_field_val;
            p_rohc_pkt+=(from_bit_pos+len)/8;
            from_bit_pos=(from_bit_pos+len)%8;
            p_dsc->dpib->field_len[field_id-ROHC_VAR_FID_BASE]+=len;
        }/*end if split field found*/
        
    }/* end for traversal field type in packet type array */

    ROHC_EXIT_FN("rohc_d_decode_p1_value");
    *p_len = (p_rohc_pkt - p_rohc_org_pkt);
    return ROHC_SUCCESS;
}/* end function rohc_decode_p1_value*/

/*Generic Packet Decoding Routine for compressed packets ********/
/****************************************************************************
 * Name     :   rohc_d_decode_p2_value
 * Desc     :   D-PPM
 * Params   :   p_dsc - Pointer to DSC context
 *          :   p_rohc_pkt - Pointer to ROHC Packet received
 *          :   pkt_type - Packet type
            :   p_len - ROHC Packet length(Output parameter)
 *          :   p_entity -Pointer to ROHC entity context
 * 			:   p_ecode  - Error code Pointer
 * Returns  :   - parsed length
 **************************************************************************/
rohc_return_t  
rohc_d_decode_p2_value
    ROHC_PARAMS((p_dsc,p_rohc_pkt,pkt_type,p_len,p_entity,p_ecode),
     /* + SPR 17439 */
    rohc_dsc_common_t  * p_dsc       _AND_
     /* - SPR 17439 */
    rohc_U8bit_t            * p_rohc_pkt  _AND_
    register rohc_U8bit_t   pkt_type      _AND_
    rohc_U32bit_t           *p_len        _AND_
    struct rohc_entity_t    * p_entity    _AND_
    rohc_error_code_t       *p_ecode)
{
    rohc_U32bit_t   i=0,n=0;
    rohc_U8bit_t    no_of_bits=0,no_of_bytes=0;
    rohc_U8bit_t    from_bit_pos=0;
    rohc_U8bit_t    * p_rohc_org_pkt=p_rohc_pkt;
    rohc_U8bit_t    field_id,temp_len=0;    
    rohc_U32bit_t   temp_field_val=0;
    ROHC_ENTER_FN("rohc_d_decode_p2_value");
    for(i=0;i<ROHC_MAX_NO_OF_FIELDS_IN_A_PACKET && 
        rohc_c_pkt_type[pkt_type][i].field_id!=ROHC_INVALID_FID;i++)
    {

        /* Traverse each field in packet type */
        no_of_bits=rohc_c_pkt_type[pkt_type][i].no_of_bits; 
        field_id=rohc_c_pkt_type[pkt_type][i].field_id;
        
        if(no_of_bits<=ROHC_ENC_MAX_FIXED_BITS_IN_FIELD)
        {   
            /* fixed length encoding */
       		/* SPR 3532 - Klocwork Fix */ 
            if(field_id >= ROHC_VAR_FID_BASE)
            {   /*Field Values dependent on stream context */
                n=no_of_bits;
                no_of_bytes=(no_of_bits+7)/8;
                while(no_of_bytes)
                {
                    if(n%8)
                    {
                        temp_field_val=(1U<<(n%8))-1;
                        temp_field_val&=((*p_rohc_pkt)>>(8-(n%8)-from_bit_pos)); 
                        p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]=
                        (p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]<<(n%8))
                                        |temp_field_val;
                        p_rohc_pkt+=(from_bit_pos+n%8)/8;
                        from_bit_pos=(from_bit_pos+n%8)%8;
                        n-=n%8;
                    }
                    else
                    {
                        p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]=
                        (p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]<<8)
                                        |(*p_rohc_pkt);
                        p_rohc_pkt++;
                    }
                    no_of_bytes--;
                }
                p_dsc->dpib->field_len[field_id-ROHC_VAR_FID_BASE]+=no_of_bits;
/*              temp_field_val=(1U<<no_of_bits)-1;
                temp_field_val= temp_field_val << 
                        (no_of_bits>8?(32-no_of_bits-from_bit_pos):
                        (8-no_of_bits-from_bit_pos));
                temp_field_val&=*(p_rohc_pkt);      
                p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]=
                (p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]<<
                no_of_bits)|temp_field_val;
                p_rohc_pkt+=(from_bit_pos+no_of_bits)/8;
                from_bit_pos=(from_bit_pos+no_of_bits)%8;
*/
            } /*end if */
			/* SPR 3532 - Klocwork Fix */
			else if((field_id > ROHC_MAX_CONSTANT_FID) && 
                (field_id < ROHC_VAR_FID_BASE))
			{
			  ROHC_LOG(LOG_INFO,"Invalid value of field_id");
			  return ROHC_FAILURE;
			}
            else
            {
                from_bit_pos+=no_of_bits;
            }
        }/* end if fixed no of bits encoding */
        else if(no_of_bits == ROHC_ENCTYPE_VAR_NO_BITS)
        {
            if((field_id != ROHC_CID_FID)||
                (p_entity->large_cids == ROHC_TRUE))
            {
                p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE] =
                         rohc_d_sdvalue_decode( p_rohc_pkt,&temp_len);
                p_rohc_pkt+=temp_len;
            }
        }
        /* +- SPR 17777 */
        else if(no_of_bits == ROHC_ENCTYPE_EXT3FLAGSP1)
        {
            /*Encoding based on extension 3 flags */
            /* +- SPR 17777 */
            rohc_dec_ele_p2_ext3flagsp1(&p_rohc_pkt,
                            field_id,p_dsc->dpib);
        }   /* end if no_of_bits == EXT 3 flags */
        else if(no_of_bits == ROHC_ENCTYPE_INNERIPFLAGSP1)
        {
            /*Encoding based on inenr ip flags */
            /* + SPR 17439 */
            if(ROHC_FAILURE == rohc_dec_ele_p2_inneripflagsp1(&p_rohc_pkt,
                    field_id,(rohc_dsc_udp_t*)p_dsc,p_entity,p_ecode))
            /* - SPR 17439 */
                return ROHC_FAILURE;
        }   /* end if no_of_bits == ROHC_ENCTYPE_INNERIPFLAGSP1*/
        else if(no_of_bits == ROHC_ENCTYPE_OUTERIPFLAGSP1)
        {
            /*Encoding based on outer ip flags */
            /* + SPR 17439 */
            if(ROHC_FAILURE == rohc_dec_ele_p2_outeripflagsp1(&p_rohc_pkt,
                    field_id,(rohc_dsc_udp_t*)p_dsc,p_entity,p_ecode))
              /* - SPR 17439 */  
                return ROHC_FAILURE;
        }/* end if no_of_bits == ROHC_ENCTYPE_OUTERIPFLAGSP1*/
        /* +- SPR 17777 */
        else if(no_of_bits == ROHC_ENCTYPE_SPLIT_FIELD)
        { /* split field always in base headers */
            rohc_U32bit_t   len;
            len=rohc_c_pkt_type[pkt_type][i].no_of_bits_occ;
            temp_field_val=(1U<<len)-1;
            temp_field_val&=((*p_rohc_pkt)>>(8-len-from_bit_pos));      
            p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]=
            (p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]<<len)
                                        |temp_field_val;
            p_rohc_pkt+=(from_bit_pos+len)/8;
            from_bit_pos=(from_bit_pos+len)%8;
            p_dsc->dpib->field_len[field_id-ROHC_VAR_FID_BASE]+=len;
        }/*end if split field found*/
        
    }/* end for traversal field type in packet type array */

    ROHC_EXIT_FN("rohc_d_decode_p2_value");
    *p_len = (p_rohc_pkt - p_rohc_org_pkt);
    return ROHC_SUCCESS;
}/* end function rohc_d_decode_p2_value*/

/*Generic Packet Decoding Routine for compressed packets ********/
/****************************************************************************
 * Name     :   rohc_d_DecodeP6Value
 * Desc     :   Function to parse the field values for profile 6
 * Params   :   p_dsc - Pointer to DSC context
 *          :   p_rohc_pkt - Pointer to ROHC Packet received
 *          :   pkt_type - Packet type
 *          :   p_len - ROHC Packet length(Output parameter)
 *          :   p_entity - Pointer to ROHC entity context
 *			: 	p_ecode  - Error code Pointer
 * Returns  :   - parsed length
 **************************************************************************/
rohc_return_t  
rohc_d_DecodeP6Value
    ROHC_PARAMS((p_dsc,p_rohc_pkt,pkt_type,p_len,p_entity,p_ecode),
     /* + SPR 17439 */
    rohc_dsc_common_t          * p_dsc      _AND_ 
     /* - SPR 17439 */
    rohc_U8bit_t            * p_rohc_pkt _AND_
    register rohc_U8bit_t   pkt_type     _AND_
    rohc_U32bit_t           *p_len       _AND_
    struct rohc_entity_t    * p_entity   _AND_
    rohc_error_code_t       *p_ecode)
{
    rohc_U8bit_t  len = 0,irregular_chain_type=0;
    rohc_U32bit_t index=0;
    rohc_U32bit_t Index=0;
    rohc_U8bit_t    XI = 0x00;
    rohc_U8bit_t no_of_bits=0,field_id;
    rohc_U8bit_t next_header = ROHC_IPPROTO_TCP;
    rohc_U8bit_t   mCount = 0 , mCounter  =0;
    rohc_U8bit_t    * p_rohc_org_pkt=p_rohc_pkt;
    
    /* +- SPR 17777 */
/* SPR 20636 Changes Start */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,p_entity->rohc_entity_id.ueIndex,(ADDR)p_ecode,
            0,0,0,0, __func__,"");
    ROHC_ENTER_FN("rohc_d_DecodeP6Value");

/* SPR 20636 Changes End */
    /* Traverse through each field of IP/TCP Compressed packet, 
     * End of list is validated by ROHC_INVALID_FID */

    for(index=0;((index<ROHC_MAX_NO_OF_FIELDS_IN_A_PACKET) && 
        rohc_c_pkt_type[pkt_type][index].field_id!=ROHC_INVALID_FID);index++)
    {
        no_of_bits=rohc_c_pkt_type[pkt_type][index].no_of_bits; 
        field_id=rohc_c_pkt_type[pkt_type][index].field_id;

        if(ROHC_ENCTYPE_SPLIT_FIELD == no_of_bits)
        {
            len=rohc_c_pkt_type[pkt_type][index].no_of_bits_occ;
        }

        /* Calculation Number of Bits for variable length Parameters
         * of Common Compressed Packet
         */
        if( ROHC_CO_P6_COMMON_PKTYPE == pkt_type)
        {
           rohcCalCommCompPktVarlenFieldSize( field_id , &no_of_bits , 
                ((rohc_dsc_tcp_t*)p_dsc)); 
        }
        rohcP6COParseFID(((rohc_dsc_tcp_t*)p_dsc),&p_rohc_pkt,
                no_of_bits,field_id,len);

    }/* end for traversal field type in packet type array */
     /* + SPR 17439 */
    irregular_chain_type = rohcP6GetIrregChainType((rohc_dsc_tcp_t*)p_dsc, &next_header);
     /* - SPR 17439 */

    /* Traverse through each field of the irregular chain of IPv4/IPv6 packet, 
     * End of list is validated by ROHC_INVALID_FID */
     /* + SPR 17439 */
    rohcP6DecodeIPIrregChain((rohc_dsc_tcp_t*)p_dsc, irregular_chain_type, pkt_type,&p_rohc_pkt);
     /* - SPR 17439 */

    /* Traverse through each field of the irregular chain of TCP packet, 
     * End of list is validated by ROHC_INVALID_FID */
    if (ROHC_IPPROTO_TCP == next_header)
    {
        /* + SPR 17439 */
        rohcP6DecodeTCPIrregChain((rohc_dsc_tcp_t*)p_dsc, pkt_type,&p_rohc_pkt);
        /* - SPR 17439 */

        if (!(((rohc_dsc_tcp_t*)p_dsc)->dpib->
                    field_val[ROHC_TCP_LIST_PRESENT_FID- ROHC_TCP_CTRL_FID_BASE]))
        {
            /* + SPR 17439 */
           rohcP6DecodeTCPOptionsIrregChain((rohc_dsc_tcp_t*)p_dsc , &p_rohc_pkt);
            /* - SPR 17439 */
        }
        else
        {   
            /*Coverity Fix 40846 start*/
            rohc_U8bit_t          tcpOptLen[256] = {0x00};
            /*Coverity Fix 40846 end */
            /* Reteriving Generic Option Length*/
            /* + SPR 17439 */
            rohcGetGenericOptionLen((rohc_dsc_tcp_t*)p_dsc , tcpOptLen );
            /* - SPR 17439 */
            mCount =  (((rohc_dsc_tcp_t*)p_dsc)->
                    dpib->tcp_dyn_options->res_ps_m & ROHC_DYN_TCP_M_MASK );
            if ( mCount > 0 )
            {
                if (!(((rohc_dsc_tcp_t*)p_dsc)->
                            dpib->tcp_dyn_options->res_ps_m & ROHC_DYN_PS_MASK ))
                {
                    for  ( mCounter = 0 ; mCounter < mCount ; mCounter++)
                    {
                        XI = ((rohc_dsc_tcp_t*)p_dsc)->dpib->tcp_dyn_options_XI->xi_m[Index]>>4;
                        if(!(XI & 0x08))
                        {
                            XI = XI & 0x07;
                            /* + SPR 17439 */
                            rohcParseIrregularChain((rohc_dsc_tcp_t*)p_dsc , &p_rohc_pkt ,XI,tcpOptLen);
                            /* - SPR 17439 */
                        }

                        mCounter++;
                        XI = (p_dsc->dpib->tcp_dyn_options_XI->xi_m[Index]
                                & 0x0F);
                        if((mCount & 0x01) && ((mCount-mCounter)==1))
                        {
                            p_rohc_pkt++; //increase the size for padding
                            break;
                        }

                        if (!(XI &  0x08 ))
                        {
                            XI = XI & 0x07;
                            /* + SPR 17439 */
                            rohcParseIrregularChain((rohc_dsc_tcp_t*)p_dsc , &p_rohc_pkt ,XI,tcpOptLen);
                            /* - SPR 17439 */
                        }
                        Index++;
                    }
                }
                else
                {
                    for  ( mCounter = 0 ; mCounter < mCount ; mCounter++)
                    {
                        if(!(0x80 & (((rohc_dsc_tcp_t*)p_dsc)->dpib->
                                        tcp_dyn_options_XI->xi_m[mCounter]))) 
                        {
                            /* + SPR 17439 */
                            rohcParseIrregularChain((rohc_dsc_tcp_t*)p_dsc , &p_rohc_pkt ,
                            /* - SPR 17439 */
                                    (((0x0F & ((rohc_dsc_tcp_t*)p_dsc)->dpib->
                                       tcp_dyn_options_XI->xi_m[mCounter]))) ,tcpOptLen);   

                        } 
                    }
                }     
            }
        }    
    }

    ROHC_EXIT_FN("rohc_d_DecodeP6Value");
    *p_len = (p_rohc_pkt - p_rohc_org_pkt);
    return ROHC_SUCCESS;
}/* end function rohc_d_DecodeP6Value*/


/****************************************************************************
 * Name     :   rohcParseSackIrregularBlock
 * Desc     :   Function to decode SACK irregular blocks                    
 * Params   :   p_dsc - Pointer to DSC,
 *			:	p_rohc_pkt - Pointer to ROHC Packet received
 *			:	field_id - Field Identifier 
 * Returns  :   None
 **************************************************************************/
static void rohcParseSackIrregularBlock(
            rohc_dsc_tcp_t           *p_dsc ,
            rohc_U8bit_t            **p_rohc_pkt  ,
            rohc_U8bit_t             field_id) 
        
{
     /* Check for the type of SACK blocks received*/ 
     if ( !(**p_rohc_pkt & 0x80) )
     {      
         p_dsc->dpib->
             field_val[ field_id - ROHC_VAR_FID_BASE]
             = ((0x7F & (**p_rohc_pkt))<< 8);
         (*p_rohc_pkt)++;
         p_dsc->dpib->
             field_val[ field_id - ROHC_VAR_FID_BASE]
             |= (**p_rohc_pkt);
         (*p_rohc_pkt)++;
         p_dsc->dpib->
             field_len[ field_id - ROHC_VAR_FID_BASE] = 15;
             
     }   /*  */
     else if ( 0x80 == (**p_rohc_pkt & 0xC0)  ) 
     {
         p_dsc->dpib->
             field_val[ field_id - ROHC_VAR_FID_BASE]
             = ((0x3F & (**p_rohc_pkt))<< 16); 
         (*p_rohc_pkt)++;
         p_dsc->dpib->
             field_val[ field_id - ROHC_VAR_FID_BASE]
             |= ((**p_rohc_pkt)<<8);
         (*p_rohc_pkt)++;
         p_dsc->dpib->
             field_val[ field_id - ROHC_VAR_FID_BASE]
             |= (**p_rohc_pkt);
         (*p_rohc_pkt)++;
         p_dsc->dpib->
             field_len[ field_id - ROHC_VAR_FID_BASE] = 22;
     }       
     else if ( 0xC0 == ( **p_rohc_pkt & 0xC0 ) ) 
     {
         p_dsc->dpib->
             field_val[ field_id - ROHC_VAR_FID_BASE]
             = ((0x3F & (**p_rohc_pkt))<< 24); 
         (*p_rohc_pkt)++;
         p_dsc->dpib->
             field_val[ field_id - ROHC_VAR_FID_BASE]
             |= (((**p_rohc_pkt))<<16);
         (*p_rohc_pkt)++;
         p_dsc->dpib->
             field_val[ field_id - ROHC_VAR_FID_BASE]
             |= ((**p_rohc_pkt)<<8);
         (*p_rohc_pkt)++;
         p_dsc->dpib->
             field_val[ field_id - ROHC_VAR_FID_BASE]
             |= (**p_rohc_pkt);
         (*p_rohc_pkt)++;
         p_dsc->dpib->
             field_len[ field_id - ROHC_VAR_FID_BASE] = 30;
        
     }
}

/****************************************************************************
 * Name     :   rohcParseIrregularChain
 * Desc     :   This function parse Irregular Chain on the basis of absence  
                of items in Compressed List     
 * Params   :   p_dsc :- ROHC Decompressors Context,
 * 				p_rohc_pkt - pointer to ROHC packet,
 *   			XI,
 * 				pLen - packet lenght
 * Returns  :   None
 **************************************************************************/
static void rohcParseIrregularChain( 
                rohc_dsc_tcp_t          *p_dsc,
                rohc_U8bit_t            **p_rohc_pkt ,
                rohc_U8bit_t            XI  ,
                rohc_U8bit_t            *pLen)
{
    
    switch(XI)
    {
        case INDEX_MSS :   
        case INDEX_WINDOW_SCAL :
        case INDEX_SACK_PERM :
        case INDEX_NOP :
        case INDEX_EOL :
          break;
        case INDEX_SACK :  
              rohcParseSackIregularChain( p_dsc, p_rohc_pkt);
              break;
        case INDEX_TIMESTAMP :
              /*Parsing the TCP OPTIONS Irregular chain*/
              rohcTcpTsOptionIrregular( p_dsc ,
                  p_rohc_pkt, ROHC_TS_FID );
              /*TS Options Parsing */
              rohcTcpTsOptionIrregular( p_dsc ,
                  p_rohc_pkt, ROHC_TCP_TS_ECHO_FID );
              break;
      
        default:
        if(XI < ROHC_MAX_ENTRIES_TCP_OPT_IDX)
        {
            /* Handling of Generic Options*/
            if (!((p_dsc)->tcpOptGenStaticFlag & ( 0x01 <<
                            p_dsc->optionTypeMap[XI - INDEX_SACK] )))
            {   /* Generic Full Irregular  */
                (p_dsc)->dpib->tcpOptGen[
                    (p_dsc)->optionTypeMap[XI -INDEX_SACK]].genOptDisc =
                    *(*p_rohc_pkt);
                if ( 0 ==  **p_rohc_pkt )
                {
                    (*p_rohc_pkt)++;
                    ROHC_MEMCPY ((p_dsc)->dpib->tcpOptGen
                            [(p_dsc)->optionTypeMap[XI -INDEX_SACK]].optionString ,
                            *p_rohc_pkt , *(pLen +
                                (p_dsc)->optionTypeMap[XI -INDEX_SACK]) -2) ;
                    (p_dsc)->dpib->tcpOptGen
                        [(p_dsc)->optionTypeMap[XI -INDEX_SACK]].len = *(pLen +
                                (p_dsc)->optionTypeMap[XI -INDEX_SACK]) -1 ;
                    *p_rohc_pkt += *(pLen +
                            (p_dsc)->optionTypeMap[XI -INDEX_SACK]) -2;
                } // An item that can change, but currently is unchanged
                else
                {
                    (*p_rohc_pkt)++;
                    (p_dsc)->dpib->tcpOptGen
                        [(p_dsc)->optionTypeMap[XI -INDEX_SACK]].len =
                        *(pLen + (p_dsc)->optionTypeMap[XI -INDEX_SACK]) -1;
                }
            } // Used when context of option has option_static set to one
        }
        
    /*++coverity 56626 - indentation corrected*/
        break;
    }            
    /*--coverity 56626*/
} 

/****************************************************************************
 * Name     :   rohcParseSackIregularChain
 * Desc     :   Function to decode SACK irregular blocks 
                    
 * Params   :   p_dsc :- ROHC Decompressors Context,
 * 				p_rohc_pkt - pointer to ROHC packet,
 * Returns  :   None
 **************************************************************************/
static void rohcParseSackIregularChain(
                rohc_dsc_tcp_t          *p_dsc,
                rohc_U8bit_t            **p_rohc_pkt)
{
    switch (**p_rohc_pkt)
    {   /* In case of SACK Block Addition in 
         * in Irregular chain EOL Pad len doesn't 
         * as size of each SACK Block is of 8 byte 
         */
        case ROHC_SACK_0_DISC: /* ROHC_SACK_0_DISC = 0x00 */
            (*p_rohc_pkt)++;
	    break;
        case ROHC_SACK_1_DISC: /* ROHC_SACK_1_DISC = 0x01 */
            (*p_rohc_pkt)++;
            rohcParseSackIrregularBlock(p_dsc ,
                p_rohc_pkt , ROHC_TCP_SACK_BLOCK_START_1);
            rohcParseSackIrregularBlock(p_dsc ,
                p_rohc_pkt , ROHC_TCP_SACK_BLOCK_END_1);
            break;    
            
        case ROHC_SACK_2_DISC: /* ROHC_SACK_2_DISC = 0x02 */
            (*p_rohc_pkt)++;
            rohcParseSackIrregularBlock(p_dsc ,
                p_rohc_pkt , ROHC_TCP_SACK_BLOCK_START_1);
            rohcParseSackIrregularBlock(p_dsc ,
                p_rohc_pkt , ROHC_TCP_SACK_BLOCK_END_1);
            rohcParseSackIrregularBlock(p_dsc ,
                p_rohc_pkt , ROHC_TCP_SACK_BLOCK_START_2);
            rohcParseSackIrregularBlock(p_dsc ,
                p_rohc_pkt , ROHC_TCP_SACK_BLOCK_END_2);
            break;

        case ROHC_SACK_3_DISC: /* ROHC_SACK_3_DISC = 0x03 */
            (*p_rohc_pkt)++;
            rohcParseSackIrregularBlock( p_dsc ,
                p_rohc_pkt , ROHC_TCP_SACK_BLOCK_START_1);
            rohcParseSackIrregularBlock( p_dsc ,
                p_rohc_pkt , ROHC_TCP_SACK_BLOCK_END_1);
            rohcParseSackIrregularBlock( p_dsc ,
                p_rohc_pkt , ROHC_TCP_SACK_BLOCK_START_2);
            rohcParseSackIrregularBlock( p_dsc ,
                p_rohc_pkt , ROHC_TCP_SACK_BLOCK_END_2);
            rohcParseSackIrregularBlock( p_dsc ,
                p_rohc_pkt , ROHC_TCP_SACK_BLOCK_START_3);
            rohcParseSackIrregularBlock( p_dsc ,
                p_rohc_pkt , ROHC_TCP_SACK_BLOCK_END_3);
            break;
        case ROHC_SACK_4_DISC: /* ROHC_SACK_4_DISC = 0x04 */
            (*p_rohc_pkt)++;
            rohcParseSackIrregularBlock(p_dsc ,
                p_rohc_pkt , ROHC_TCP_SACK_BLOCK_START_1);
            rohcParseSackIrregularBlock(p_dsc ,
                p_rohc_pkt , ROHC_TCP_SACK_BLOCK_END_1);
            rohcParseSackIrregularBlock(p_dsc ,
                p_rohc_pkt , ROHC_TCP_SACK_BLOCK_START_2);
            rohcParseSackIrregularBlock(p_dsc ,
                p_rohc_pkt , ROHC_TCP_SACK_BLOCK_END_2);
            rohcParseSackIrregularBlock(p_dsc ,
                p_rohc_pkt , ROHC_TCP_SACK_BLOCK_START_3);
            rohcParseSackIrregularBlock(p_dsc ,
                p_rohc_pkt , ROHC_TCP_SACK_BLOCK_END_3);
            rohcParseSackIrregularBlock(p_dsc ,
                p_rohc_pkt , ROHC_TCP_SACK_BLOCK_START_4);
            rohcParseSackIrregularBlock(p_dsc ,
                p_rohc_pkt , ROHC_TCP_SACK_BLOCK_END_4);
            break;
    }
    return;
}
/****************************************************************************
 * Name     :   rohcRfc4996GetEncVarType32 
 * Desc     :   Function to calculate length of number of bits Variable type 
                32 bit    
 * Params   :   choice
 * Returns  :   number of bits
 **************************************************************************/
static inline rohc_U8bit_t  rohcRfc4996GetEncVarType32 ( rohc_U8bit_t choice)
{
   switch (choice)
   {
       case 0:
           return  0;
       case 1:
            return 8;
       case 2:
           return 16;
       case 3:
           return 32;
   }
   return 0; 
}

/****************************************************************************
 * Name     :   rohcCalCommCompPktVarlenFieldSize   
 * Desc     :   Function to calculate Variable length Parameters of Common 
                Compressed Packet
 * Params   :   field_id - Field Id
                no_of_bits - Return Number of Bits received for particular
                p_dsc :- ROHC Decompressors Context             
 * Returns  :   None
 **************************************************************************/
static void rohcCalCommCompPktVarlenFieldSize( 
            rohc_U8bit_t   field_id,
            rohc_U8bit_t * no_of_bits , 
            rohc_dsc_tcp_t* p_dsc)
{
     /*ack_stride_indicator or window_indicator or urg_ptr_indicator*/
   switch ( field_id ) 
   {
       case ROHC_SN_FID:
           *no_of_bits = rohcRfc4996GetEncVarType32(((rohc_dsc_tcp_t*)p_dsc)->dpib->
               field_val[ROHC_TCP_SEQ_IND_FID- ROHC_TCP_CTRL_FID_BASE]);
           break;

       case ROHC_TCP_ACKNO_FID: 
           *no_of_bits =  rohcRfc4996GetEncVarType32(((rohc_dsc_tcp_t*)p_dsc)->dpib->
               field_val[ROHC_TCP_ACK_IND_FID- ROHC_TCP_CTRL_FID_BASE]);
           break;

       case ROHC_TCP_ACK_STRIDE_FID:
           *no_of_bits = ((((rohc_dsc_tcp_t*)p_dsc)->dpib->field_val[
               ROHC_TCP_ACK_STRIDE_FLAG_FID-ROHC_VAR_FID_BASE] > 0 )? 16 : 0); 
           break;
       
       case ROHC_TCP_WINDOW_FID:
           *no_of_bits = ((((rohc_dsc_tcp_t*)p_dsc)->dpib->field_val[
            ROHC_TCP_WINDOW_IND_FID- ROHC_TCP_CTRL_FID_BASE ] > 0 )? 16 : 0); 
           break;

       case ROHC_TCP_URG_PTR_FID:
           *no_of_bits = ((((rohc_dsc_tcp_t*)p_dsc)->dpib->field_val[
             ROHC_TCP_URG_PTR_PRESENT_FID - ROHC_TCP_CTRL_FID_BASE] > 0 )? 16 : 0); 

           break;

       case ROHC_INNERIP4_TTL_FID:
           *no_of_bits =  ((((rohc_dsc_tcp_t*)p_dsc)->dpib->field_val[
             ROHC_INNERIP4_TTL_HPL_PRESENT_FID - ROHC_TCP_CTRL_FID_BASE ] > 0 )? 8 : 0); 
           break;
      
       case ROHC_INNERIP4_DSCP_FID:
           *no_of_bits = ((((rohc_dsc_tcp_t*)p_dsc)->dpib->field_val[
           ROHC_INNERIP4_DSCP_PRESENT_FID- ROHC_TCP_CTRL_FID_BASE]) ? 8:0);
           break;
       
       case ROHC_INNERIP_ID_FID:
           if((IP_ID_BEHAVIOR_SEQUENTIAL == ((rohc_dsc_tcp_t*)p_dsc)->dpib->
                            field_val[ROHC_INNERIP_BEH_FID-ROHC_VAR_FID_BASE]) 
               || (IP_ID_BEHAVIOR_SEQUENTIAL_SWAPPED == ((rohc_dsc_tcp_t*)p_dsc)->dpib->
                   field_val[ROHC_INNERIP_BEH_FID-ROHC_VAR_FID_BASE] ))
           {
               *no_of_bits = ((((rohc_dsc_tcp_t*)p_dsc)->dpib->field_val[
                   ROHC_TCP_IPID_IND_FID- ROHC_TCP_CTRL_FID_BASE]) ? 16: 8 ) ;      
           } 
           else
           {
               *no_of_bits = 0;  
           }
           break;
    }
}

/****************************************************************************
 * Name     :   rohc_dec_ele_p1_ext3flagsp1
 * Desc     :   decode elements based on EXT 3 flags
 * Params   :   Pointer to ROHC packet
            :   bit position form where value is to be read
            :   Field identifier
            :   Pointer to DPIB
 * Returns  :   -
 **************************************************************************/
/* +- SPR 17777 */
void
rohc_dec_ele_p1_ext3flagsp1
    ROHC_PARAMS((p_p_rohc_pkt,field_id,p_dpib),
    rohc_U8bit_t        * * p_p_rohc_pkt    _AND_
    rohc_U8bit_t        field_id            _AND_
    rohc_dpib_t         * p_dpib)
{
    rohc_U8bit_t ext3_flags=p_dpib->field_val[ROHC_EXT3_FLAGS_FID-
                    ROHC_VAR_FID_BASE];
    rohc_U32bit_t temp_field_val=0;
    rohc_U8bit_t len=0,no_of_bits=0;
    ROHC_ENTER_FN("rohc_dec_ele_p1_ext3flagsp1");
    switch(field_id)
    {
        case ROHC_SN_FID:
            if( ext3_flags&ROHC_ENC_SN_EXT3FLAGP1_MASK)
            {
                p_dpib->field_val[ROHC_SN_FID-ROHC_VAR_FID_BASE]=
                (p_dpib->field_val[ROHC_SN_FID-ROHC_VAR_FID_BASE]<<8)
                                                        |**p_p_rohc_pkt;
                p_dpib->field_len[ROHC_SN_FID-ROHC_VAR_FID_BASE]+=8;
                (*p_p_rohc_pkt)++;
            }
            break;
        case ROHC_TS_FID:
            if( ext3_flags& ROHC_ENC_RTS_EXT3FLAGP1_MASK)
            {
                temp_field_val = rohc_d_sdvalue_decode(*p_p_rohc_pkt,&len);
                no_of_bits = rohc_sdvl_len(*p_p_rohc_pkt);
                p_dpib->field_len[ROHC_TS_FID-ROHC_VAR_FID_BASE]+=
                                    no_of_bits;
                p_dpib->field_val[ROHC_TS_FID-ROHC_VAR_FID_BASE]=
                (p_dpib->field_val[ROHC_TS_FID-ROHC_VAR_FID_BASE]<<no_of_bits)
                    |(temp_field_val&((1U<<no_of_bits)-1));
                (*p_p_rohc_pkt)+=len;
            }
            break;
        case ROHC_INNERIP_ID_FID:
            if( ext3_flags&ROHC_ENC_I_EXT3FLAGP1_MASK)
            {
                p_dpib->field_val[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE]=
                ((p_dpib->field_val[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE]<<16)
                             | (ROHC_GET_2BYTES(*p_p_rohc_pkt)));
                p_dpib->field_len[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE]=16;
                *p_p_rohc_pkt+=2;
            }
            break;  
        case ROHC_INNERIP_FLAGS_FID:
            if( ext3_flags &  ROHC_ENC_INNERIPFLAG_EXT3FLAGP1_MASK)
            {
                p_dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE]=
                    **p_p_rohc_pkt;
                    (*p_p_rohc_pkt)++;  
            }
            break;  
        case ROHC_RTP_FLAGS_FID:
            if( ext3_flags &  ROHC_ENC_RTPFLAG_EXT3FLAGP1_MASK)
            {
                p_dpib->field_val[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE]=
                    **p_p_rohc_pkt;
                    (*p_p_rohc_pkt)++;  
            }
            break;  
        case ROHC_OUTERIP_FLAGS_FID:
            if( ext3_flags &  ROHC_ENC_OUTERIPFLAG_EXT3FLAGP1_MASK)
            {
                p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE]=
                    **p_p_rohc_pkt;
                    (*p_p_rohc_pkt)++;  
            }
            break;
        default :
            ROHC_LOG(LOG_INFO,"DPPM:invalid field found"\
                    "while decoding the ext3flags D-PPM");
            break;
    }/*end switch on field id */
    ROHC_EXIT_FN("rohc_dec_ele_p1_ext3flagsp1");
}/* end function rohc_dec_ele_p1_ext3flagsp1*/
/****************************************************************************
 * Name     :   rohc_dec_ele_p2_ext3flagsp1
 * Desc     :   decode elements based on EXT 3 flags
 * Params   :   Pointer to ROHC packet
            :   bit position form where value is to be read
            :   Field identifier
            :   Pointer to DPIB
 * Returns  :   -
 **************************************************************************/
/* +- SPR 17777 */
void
rohc_dec_ele_p2_ext3flagsp1
    ROHC_PARAMS((p_p_rohc_pkt,field_id,p_dpib),
    rohc_U8bit_t * * p_p_rohc_pkt _AND_
    rohc_U8bit_t    field_id _AND_
    rohc_dpib_t     * p_dpib)
{
    rohc_U8bit_t ext3_flags=p_dpib->field_val[ROHC_EXT3_FLAGS_FID-
                    ROHC_VAR_FID_BASE];
    rohc_U32bit_t temp_field_val=0;
    rohc_U8bit_t len=0,no_of_bits=0;
    ROHC_ENTER_FN("rohc_dec_ele_p2_ext3flagsp1");
    switch(field_id)
    {
        case ROHC_SN_FID:
            if( ext3_flags&ROHC_ENC_SN_EXT3FLAGP1_MASK)
            {
                p_dpib->field_val[ROHC_SN_FID-ROHC_VAR_FID_BASE]=
                (p_dpib->field_val[ROHC_SN_FID-ROHC_VAR_FID_BASE]<<8)
                                                        |**p_p_rohc_pkt;
                p_dpib->field_len[ROHC_SN_FID-ROHC_VAR_FID_BASE]+=8;
                (*p_p_rohc_pkt)++;
            }
            break;
        case ROHC_TS_FID:
            if( ext3_flags& ROHC_ENC_RTS_EXT3FLAGP1_MASK)
            {
                temp_field_val = rohc_d_sdvalue_decode(*p_p_rohc_pkt,&len);
                no_of_bits = rohc_sdvl_len(*p_p_rohc_pkt);
                p_dpib->field_len[ROHC_TS_FID-ROHC_VAR_FID_BASE]+=
                                    no_of_bits;
                p_dpib->field_val[ROHC_TS_FID-ROHC_VAR_FID_BASE]=
                (p_dpib->field_val[ROHC_TS_FID-ROHC_VAR_FID_BASE]<<no_of_bits)
                    |(temp_field_val&((1U<<no_of_bits)-1));
                (*p_p_rohc_pkt)+=len;
            }
            break;
        case ROHC_INNERIP_ID_FID:
            if( ext3_flags&ROHC_ENC_I_EXT3FLAGP1_MASK)
            {
                p_dpib->field_val[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE]=
                ((p_dpib->field_val[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE]<<16)
                             | (ROHC_GET_2BYTES(*p_p_rohc_pkt)));
                p_dpib->field_len[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE]=16;
                *p_p_rohc_pkt+=2;
            }
            break;  
        case ROHC_INNERIP_FLAGS_FID:
            if( ext3_flags &  ROHC_ENC_INNERIPFLAG_EXT3FLAGP1_MASK)
            {
                p_dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE]=
                    **p_p_rohc_pkt;
                    (*p_p_rohc_pkt)++;  
            }
            break;  
        case ROHC_RTP_FLAGS_FID:
            if( ext3_flags &  ROHC_ENC_RTPFLAG_EXT3FLAGP1_MASK)
            {
                p_dpib->field_val[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE]=
                    **p_p_rohc_pkt;
                    (*p_p_rohc_pkt)++;  
            }
            break;  
        case ROHC_OUTERIP_FLAGS_FID:
            if( ext3_flags &  ROHC_ENC_OUTERIPFLAG_EXT3FLAGP1_MASK)
            {
                p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE]=
                    **p_p_rohc_pkt;
                    (*p_p_rohc_pkt)++;  
            }
            break;
        default :
            ROHC_LOG(LOG_INFO,"DPPM:invalid field found"\
                    "while decoding the ext3flags D-PPM");
            break;
    }/*end switch on field id */
    ROHC_EXIT_FN("rohc_dec_ele_p2_ext3flagsp1");
}/* end function rohc_dec_ele_p2_ext3flagsp1*/

/****************************************************************************
 * Name     :   rohc_dec_ele_p1_inneripflagsp1
 * Desc     :   decode elements based on Inner IP flags
 * Params   :   Pointer to ROHC packet
 *          :   bit position form where value is to be read
 *          :   Field identifier,
 *				p_dsc - ROHC Decompressors Context
 *				p_entity - Pointer to ROHC entity context
 * 				p_ecode - Error code Pointer
 * Returns  :   -
 **************************************************************************/
/* +- SPR 17777 */
rohc_return_t
rohc_dec_ele_p1_inneripflagsp1
    ROHC_PARAMS((p_p_rohc_pkt,field_id,p_dsc,p_entity,p_ecode),
    rohc_U8bit_t        * * p_p_rohc_pkt    _AND_
    rohc_U8bit_t        field_id            _AND_
    rohc_dsc_rtp_t      * p_dsc             _AND_
    rohc_entity_t       * p_entity          _AND_
    rohc_error_code_t   *p_ecode)
{
    rohc_U8bit_t innerip_flags=p_dsc->dpib->field_val[ROHC_INNERIP_FLAGS_FID-
                    ROHC_VAR_FID_BASE];
    ROHC_ENTER_FN("rohc_dec_ele_p1_inneripflagsp1");
    switch(field_id)        
    {
        case ROHC_INNERIP4_TOS_FID :
            if(innerip_flags&ROHC_ENC_TOS_INNERIPFLAGP1_MASK)
            {
                p_dsc->dpib->field_val[ROHC_INNERIP4_TOS_FID-
                        ROHC_VAR_FID_BASE]=
                    **p_p_rohc_pkt;
                    (*p_p_rohc_pkt)++;  
            }   
            break;  
                
        case ROHC_INNERIP4_TTL_FID :
            if(innerip_flags&ROHC_ENC_TTL_INNERIPFLAGP1_MASK)
            {
                p_dsc->dpib->field_val[ROHC_INNERIP4_TTL_FID-
                        ROHC_VAR_FID_BASE]=
                    **p_p_rohc_pkt;
                    (*p_p_rohc_pkt)++;  
            }
            break;  
        case ROHC_INNERIP4_PROTO_FID :
            if(innerip_flags&ROHC_ENC_PROTO_INNERIPFLAGP1_MASK)
            {
                p_dsc->dpib->field_val[ROHC_INNERIP4_PROTO_FID-
                        ROHC_VAR_FID_BASE]=
                    **p_p_rohc_pkt;
                    (*p_p_rohc_pkt)++;  
            }
            break;
        case ROHC_INNERIP_EXTHEADERS_LIST_FID :
            if(innerip_flags&ROHC_ENC_IPX_INNERIPFLAGP1_MASK)
            {
                rohc_U32bit_t   no_of_bytes_in_list;
                if(ROHC_FAILURE == rohc_list_decoding_p1(p_entity,p_dsc,
                    ROHC_INNERIP_EXTHEADERS_LIST_FID,&no_of_bytes_in_list,
                    *p_p_rohc_pkt,p_ecode))
                    return ROHC_FAILURE;
                *p_p_rohc_pkt+=no_of_bytes_in_list;
            }
            break;
        case ROHC_OUTERIP_FLAGS_FID :
            if(innerip_flags&ROHC_ENC_IP2_INNERIPFLAGP1_MASK)
            {
                p_dsc->dpib->field_val[ROHC_OUTERIP_FLAGS_FID-
                        ROHC_VAR_FID_BASE]=
                    **p_p_rohc_pkt;
                    (*p_p_rohc_pkt)++;  
            }   
            break;
        default :
            ROHC_LOG(LOG_INFO,"DPPM:invalid field found"\
                    "while decoding the inneripflags D-PPM");
            break;  
    }   /* end switch field id */
    ROHC_EXIT_FN("rohc_dec_ele_p1_inneripflagsp1");
    return ROHC_SUCCESS;
}/* end function rohc_dec_ele_p1_inneripflagsp1 */
/****************************************************************************
 * Name     :   rohc_dec_ele_p2_inneripflagsp1
 * Desc     :   decode elements based on Inner IP flags
 * Params   :   Pointer to ROHC packet
 *          :   bit position form where value is to be read
 *          :   Field identifier,
 *			:	p_dsc - ROHC Decompressors Context,
 *			:	p_entity - Pointer to ROHC entity context,
 * 			:	p_ecode - Error code Pointer,
 * Returns  :   ROHC_SUCCESS/ROHC_FAIL
 **************************************************************************/
/* +- SPR 17777 */
rohc_return_t
rohc_dec_ele_p2_inneripflagsp1
    ROHC_PARAMS((p_p_rohc_pkt,field_id,p_dsc,p_entity,p_ecode),
    rohc_U8bit_t * * p_p_rohc_pkt _AND_
    rohc_U8bit_t    field_id _AND_
    rohc_dsc_udp_t      * p_dsc _AND_
    rohc_entity_t   * p_entity _AND_
    rohc_error_code_t *p_ecode)
{
    rohc_U8bit_t innerip_flags=p_dsc->dpib->field_val[ROHC_INNERIP_FLAGS_FID-
                    ROHC_VAR_FID_BASE];
    ROHC_ENTER_FN("rohc_dec_ele_p2_inneripflagsp1");
    switch(field_id)        
    {
        case ROHC_INNERIP4_TOS_FID :
            if(innerip_flags&ROHC_ENC_TOS_INNERIPFLAGP1_MASK)
            {
                p_dsc->dpib->field_val[ROHC_INNERIP4_TOS_FID-
                        ROHC_VAR_FID_BASE]=
                    **p_p_rohc_pkt;
                    (*p_p_rohc_pkt)++;  
            }   
            break;  
                
        case ROHC_INNERIP4_TTL_FID :
            if(innerip_flags&ROHC_ENC_TTL_INNERIPFLAGP1_MASK)
            {
                p_dsc->dpib->field_val[ROHC_INNERIP4_TTL_FID-
                        ROHC_VAR_FID_BASE]=
                    **p_p_rohc_pkt;
                    (*p_p_rohc_pkt)++;  
            }
            break;  
        case ROHC_INNERIP4_PROTO_FID :
            if(innerip_flags&ROHC_ENC_PROTO_INNERIPFLAGP1_MASK)
            {
                p_dsc->dpib->field_val[ROHC_INNERIP4_PROTO_FID-
                        ROHC_VAR_FID_BASE]=
                    **p_p_rohc_pkt;
                    (*p_p_rohc_pkt)++;  
            }
            break;
        case ROHC_INNERIP_EXTHEADERS_LIST_FID :
            if(innerip_flags&ROHC_ENC_IPX_INNERIPFLAGP1_MASK)
            {
                rohc_U32bit_t   no_of_bytes_in_list;
                if(ROHC_FAILURE == rohc_list_decoding_p2(p_entity,p_dsc,
                    ROHC_INNERIP_EXTHEADERS_LIST_FID,&no_of_bytes_in_list,
                    *p_p_rohc_pkt,p_ecode))
                    return ROHC_FAILURE;
                *p_p_rohc_pkt+=no_of_bytes_in_list;
            }
            break;
        case ROHC_OUTERIP_FLAGS_FID :
            if(innerip_flags&ROHC_ENC_IP2_INNERIPFLAGP1_MASK)
            {
                p_dsc->dpib->field_val[ROHC_OUTERIP_FLAGS_FID-
                        ROHC_VAR_FID_BASE]=
                    **p_p_rohc_pkt;
                    (*p_p_rohc_pkt)++;  
            }   
            break;
        default :
            ROHC_LOG(LOG_INFO,"DPPM:invalid field found"\
                    "while decoding the inneripflags D-PPM");
            break;  
    }   /* end switch field id */
    ROHC_EXIT_FN("rohc_dec_ele_p2_inneripflagsp1");
    return ROHC_SUCCESS;
}/* end function rohc_dec_ele_p2_inneripflagsp1 */

/****************************************************************************
 * Name     :   rohc_dec_ele_p1_outeripflagsp1
 * Desc     :   decode elements based on Outer IP flags
 * Params   :   Pointer to ROHC packet
 *          :   bit position form where value is to be read
 *          :   Field identifier
 *			:	p_dsc - ROHC Decompressors Context,
 *			:	p_entity - Pointer to ROHC entity context,
 * 			:	p_ecode - Error code Pointer,
 * Returns  :   ROHC_SUCCESS 
 **************************************************************************/
/* +- SPR 17777 */
rohc_return_t
rohc_dec_ele_p1_outeripflagsp1
    ROHC_PARAMS((p_p_rohc_pkt,field_id,p_dsc,p_entity,p_ecode),
    rohc_U8bit_t        * * p_p_rohc_pkt    _AND_
    rohc_U8bit_t        field_id            _AND_
    rohc_dsc_rtp_t      * p_dsc             _AND_
    rohc_entity_t       * p_entity          _AND_
    rohc_error_code_t   *p_ecode)
{
    rohc_U8bit_t outerip_flags=p_dsc->dpib->field_val[ROHC_OUTERIP_FLAGS_FID-
                    ROHC_VAR_FID_BASE];
    ROHC_ENTER_FN("rohc_dec_ele_p1_outeripflagsp1");
    switch(field_id)        
    {
        case ROHC_OUTERIP4_TOS_FID :
            if(outerip_flags&ROHC_ENC_TOS_OUTERIPFLAGP1_MASK)
            {
                p_dsc->dpib->field_val[ROHC_OUTERIP4_TOS_FID-
                        ROHC_VAR_FID_BASE]=
                    **p_p_rohc_pkt;
                    (*p_p_rohc_pkt)++;  
            }   
            break;  
                
        case ROHC_OUTERIP4_TTL_FID :
            if(outerip_flags&ROHC_ENC_TTL_OUTERIPFLAGP1_MASK)
            {
                p_dsc->dpib->field_val[ROHC_OUTERIP4_TTL_FID-
                        ROHC_VAR_FID_BASE]=
                    **p_p_rohc_pkt;
                    (*p_p_rohc_pkt)++;  
            }
            break;  
        case ROHC_OUTERIP4_PROTO_FID :
            if(outerip_flags&ROHC_ENC_PROTO_OUTERIPFLAGP1_MASK)
            {
                p_dsc->dpib->field_val[ROHC_OUTERIP4_PROTO_FID-
                        ROHC_VAR_FID_BASE]=
                    **p_p_rohc_pkt;
                    (*p_p_rohc_pkt)++;  
            }
            break;
        case ROHC_OUTERIP_EXTHEADERS_LIST_FID :
            if(outerip_flags&ROHC_ENC_IPX_OUTERIPFLAGP1_MASK)
            {
                rohc_U32bit_t   no_of_bytes_in_list;
                if(ROHC_FAILURE == rohc_list_decoding_p1(p_entity,p_dsc,
                    ROHC_OUTERIP_EXTHEADERS_LIST_FID,&no_of_bytes_in_list,
                    *p_p_rohc_pkt,p_ecode))
                    return ROHC_FAILURE;
                *p_p_rohc_pkt+=no_of_bytes_in_list;
            }
            break;
        case ROHC_OUTERIP_ID_FID    :
            if(outerip_flags&ROHC_ENC_IP2_INNERIPFLAGP1_MASK)
            {
                p_dsc->dpib->field_val[ROHC_OUTERIP_ID_FID
                    -ROHC_VAR_FID_BASE]=
                ((p_dsc->dpib->field_val[ROHC_OUTERIP_ID_FID
                    -ROHC_VAR_FID_BASE])<<16)
                    |(ROHC_GET_2BYTES(*p_p_rohc_pkt));
                p_dsc->dpib->field_len[ROHC_OUTERIP_ID_FID
                    -ROHC_VAR_FID_BASE]=16;
                    *p_p_rohc_pkt+=2;   
            }   
            break;
        default :
            ROHC_LOG(LOG_INFO,"DPPM:invalid field found"\
                "while decoding the outeripflags D-PPM");
            break;  
    }   /* end switch field id */
    ROHC_EXIT_FN("rohc_dec_ele_p1_outeripflagsp1");
    return ROHC_SUCCESS;
}/* end function rohc_dec_ele_p1_outeripflagsp1 */
/****************************************************************************
 * Name     :   rohc_dec_ele_p2_outeripflagsp1
 * Desc     :   decode elements based on Outer IP flags
 * Params   :   Pointer to ROHC packet
 *          :   bit position form where value is to be read
 *          :   Field identifier
 *			:	p_dsc - ROHC Decompressors Context,
 *			:	p_entity - Pointer to ROHC entity context,
 * 			:	p_ecode - Error code Pointer,
 * Returns  :   ROHC_SUCCESS/ROHC_FAILURE
 **************************************************************************/
/* +- SPR 17777 */
rohc_return_t
rohc_dec_ele_p2_outeripflagsp1
    ROHC_PARAMS((p_p_rohc_pkt,field_id,p_dsc,p_entity,p_ecode),
    rohc_U8bit_t * * p_p_rohc_pkt _AND_
    rohc_U8bit_t    field_id _AND_
    rohc_dsc_udp_t      * p_dsc _AND_
    rohc_entity_t   * p_entity _AND_
    rohc_error_code_t   *p_ecode)
{
    rohc_U8bit_t outerip_flags=p_dsc->dpib->field_val[ROHC_OUTERIP_FLAGS_FID-
                    ROHC_VAR_FID_BASE];
    ROHC_ENTER_FN("rohc_dec_ele_p2_outeripflagsp1");
    switch(field_id)        
    {
        case ROHC_OUTERIP4_TOS_FID :
            if(outerip_flags&ROHC_ENC_TOS_OUTERIPFLAGP1_MASK)
            {
                p_dsc->dpib->field_val[ROHC_OUTERIP4_TOS_FID-
                        ROHC_VAR_FID_BASE]=
                    **p_p_rohc_pkt;
                    (*p_p_rohc_pkt)++;  
            }   
            break;  
                
        case ROHC_OUTERIP4_TTL_FID :
            if(outerip_flags&ROHC_ENC_TTL_OUTERIPFLAGP1_MASK)
            {
                p_dsc->dpib->field_val[ROHC_OUTERIP4_TTL_FID-
                        ROHC_VAR_FID_BASE]=
                    **p_p_rohc_pkt;
                    (*p_p_rohc_pkt)++;  
            }
            break;  
        case ROHC_OUTERIP4_PROTO_FID :
            if(outerip_flags&ROHC_ENC_PROTO_OUTERIPFLAGP1_MASK)
            {
                p_dsc->dpib->field_val[ROHC_OUTERIP4_PROTO_FID-
                        ROHC_VAR_FID_BASE]=
                    **p_p_rohc_pkt;
                    (*p_p_rohc_pkt)++;  
            }
            break;
        case ROHC_OUTERIP_EXTHEADERS_LIST_FID :
            if(outerip_flags&ROHC_ENC_IPX_OUTERIPFLAGP1_MASK)
            {
                rohc_U32bit_t   no_of_bytes_in_list;
                if(ROHC_FAILURE == rohc_list_decoding_p2(p_entity,p_dsc,
                    ROHC_OUTERIP_EXTHEADERS_LIST_FID,&no_of_bytes_in_list,
                    *p_p_rohc_pkt,p_ecode))
                    return ROHC_FAILURE;
                *p_p_rohc_pkt+=no_of_bytes_in_list;
            }
            break;
        case ROHC_OUTERIP_ID_FID    :
            if(outerip_flags&ROHC_ENC_IP2_INNERIPFLAGP1_MASK)
            {
                p_dsc->dpib->field_val[ROHC_OUTERIP_ID_FID
                    -ROHC_VAR_FID_BASE]=
                ((p_dsc->dpib->field_val[ROHC_OUTERIP_ID_FID
                    -ROHC_VAR_FID_BASE])<<16)
                    |(ROHC_GET_2BYTES(*p_p_rohc_pkt));
                p_dsc->dpib->field_len[ROHC_OUTERIP_ID_FID
                    -ROHC_VAR_FID_BASE]=16;
                    *p_p_rohc_pkt+=2;   
            }   
            break;
        default :
            ROHC_LOG(LOG_INFO,"DPPM:invalid field found"\
                "while decoding the outeripflags D-PPM");
            break;  
    }   /* end switch field id */
    ROHC_EXIT_FN("rohc_dec_ele_p2_outeripflagsp1");
    return ROHC_SUCCESS;
}/* end function rohc_dec_ele_p2_outeripflagsp1 */

/****************************************************************************
 * Name     :   rohc_dec_ele_p1_flagsp1
 * Desc     :   decode elements based on RTP flags
 * Params   :   Pointer to ROHC packet
 *          :   bit position form where value is to be read
 *          :   Field identifier
 *			:	p_dsc - ROHC Decompressors Context,
 *			:	p_entity - Pointer to ROHC entity context,
 * 			:	p_ecode - Error code Pointer,
 * Returns  :   ROHC_SUCCESS/ROHC_FAILURE
 **************************************************************************/
/* +- SPR 17777 */
rohc_return_t 
rohc_dec_ele_p1_flagsp1
    ROHC_PARAMS((p_p_rohc_pkt,field_id,p_dsc,p_entity,p_ecode),
    rohc_U8bit_t        * * p_p_rohc_pkt    _AND_
    rohc_U8bit_t        field_id            _AND_
    rohc_dsc_rtp_t      * p_dsc             _AND_
    rohc_entity_t       * p_entity          _AND_
    rohc_error_code_t   *p_ecode)
{
    rohc_U8bit_t rtp_flags=
        p_dsc->dpib->field_val[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE];
    rohc_U32bit_t temp_field_val=0;
    rohc_U8bit_t len=0,no_of_bits=0;
    ROHC_ENTER_FN("rohc_dec_ele_p1_flagsp1");
    switch(field_id)
    {
        case ROHC_RTP_PT_FID :
            if(rtp_flags&ROHC_ENC_R_PT_RTPFLAGP1_MASK)
            {
                p_dsc->dpib->field_val[ROHC_RTP_R_P_FID-
                    ROHC_VAR_FID_BASE]=(**p_p_rohc_pkt&0x80)>>7;
                p_dsc->dpib->field_val[ROHC_RTP_PT_FID-
                    ROHC_VAR_FID_BASE]=(**p_p_rohc_pkt&0x7f);
                (*p_p_rohc_pkt)++;  
            }
            break;  
        case ROHC_RTP_CSRC_LIST_FID  :
            if(rtp_flags&ROHC_ENC_CSRC_RTPFLAGP1_MASK)
            {
                rohc_U32bit_t   no_of_bytes_in_list;
                if(ROHC_FAILURE == rohc_list_decoding_p1(p_entity,p_dsc,
                    ROHC_RTP_CSRC_LIST_FID,&no_of_bytes_in_list,
                    *p_p_rohc_pkt,p_ecode))
                    return ROHC_FAILURE;
                *p_p_rohc_pkt+=no_of_bytes_in_list;
            }
            break;
        case ROHC_RTP_TS_STRIDE_FID :
            if(rtp_flags&ROHC_ENC_TSS_RTPFLAGP1_MASK)
            {
                temp_field_val = rohc_d_sdvalue_decode(*p_p_rohc_pkt,&len);
                no_of_bits = rohc_sdvl_len(*p_p_rohc_pkt);
                p_dsc->dpib->field_len[field_id-ROHC_VAR_FID_BASE]
                                +=no_of_bits;
                p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]=
                (p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]<<no_of_bits)
                    |(temp_field_val&((1U<<no_of_bits)-1));
                (*p_p_rohc_pkt)+=len;
            }
            break;
        /* SPR 5813 Fix start */
        case ROHC_RTP_TIME_STRIDE_FID :
            if(rtp_flags&ROHC_ENC_TIS_RTPFLAGP1_MASK)
            {
                temp_field_val = rohc_d_sdvalue_decode(*p_p_rohc_pkt,&len);
                no_of_bits = rohc_sdvl_len(*p_p_rohc_pkt);
                p_dsc->dpib->field_len[field_id-ROHC_VAR_FID_BASE]
                                +=no_of_bits;
                p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]=
                (p_dsc->dpib->field_val[field_id-ROHC_VAR_FID_BASE]<<no_of_bits)
                    |(temp_field_val&((1U<<no_of_bits)-1));
                (*p_p_rohc_pkt)+=len;
            }
            break;
        /* SPR 5813 Fix start */
    }/* end switch field id*/                   
    ROHC_EXIT_FN("rohc_dec_ele_p1_flagsp1");
    return ROHC_SUCCESS;
}/* end function rohc_dec_ele_p1_flagsp1*/

/****************************************************************************
 * Name     :   rohc_inter_r0_ptype_func
 * Desc     :   it determines the packet type 
 * Params   :   Pointer to ROHC packet
            :   CID length
 * Returns  : 	Type of the packet
 **************************************************************************/
rohc_U8bit_t 
rohc_inter_r0_ptype_func
                ROHC_PARAMS((p_pkt,cid_len),
                rohc_U8bit_t    *p_pkt _AND_
                rohc_U8bit_t    cid_len)
{
    ROHC_ENTER_FN("rohc_inter_r0_ptype_func");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,sizeof(p_pkt),cid_len,
            0,0,0,0, __func__,"");
    ROHC_EXIT_FN("rohc_inter_r0_ptype_func");
    return ROHC_R0P1_PKTTYPE;
}
/****************************************************************************
 * Name     :   rohc_inter_r0crc_ptype_func
 * Desc     :   it determines the packet type 
 * Params   :   Pointer to ROHC packet
            :   CID length
 * Returns  : 	Type of the packet
 **************************************************************************/
rohc_U8bit_t 
rohc_inter_r0crc_ptype_func
                ROHC_PARAMS((p_pkt,cid_len),
                rohc_U8bit_t    *p_pkt _AND_
                rohc_U8bit_t    cid_len)
{
    ROHC_ENTER_FN("rohc_inter_r0crc_ptype_func");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,sizeof(p_pkt),cid_len,
            0,0,0,0, __func__,"");
    ROHC_EXIT_FN("rohc_inter_r0crc_ptype_func");
    return ROHC_R0CRCP1_PKTTYPE;
}
/****************************************************************************
 * Name     :   rohc_inter_uo0_ptype_func
 * Desc     :   it determines the packet type 
 * Params   :   Pointer to ROHC packet
            :   CID length
 * Returns  :	Type of the packet
 **************************************************************************/
rohc_U8bit_t 
rohc_inter_uo0_ptype_func
                ROHC_PARAMS((p_pkt,cid_len),
                rohc_U8bit_t    *p_pkt _AND_
                rohc_U8bit_t    cid_len)
{
    ROHC_ENTER_FN("rohc_inter_uo0_ptype_func");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,sizeof(p_pkt),cid_len,
            0,0,0,0, __func__,"");
    ROHC_EXIT_FN("rohc_inter_uo0_ptype_func");
    return ROHC_UO0P1_PKTTYPE;
}
/****************************************************************************
 * Name     :   rohc_inter_r1_ptype_func
 * Desc     :   it determines the packet type 
 * Params   :   Pointer to ROHC packet
            :   CID length
 * Returns  : 	Type of the packet
 **************************************************************************/
rohc_U8bit_t 
rohc_inter_r1_ptype_func
                ROHC_PARAMS((p_pkt,cid_len),
                rohc_U8bit_t    *p_pkt _AND_
                rohc_U8bit_t    cid_len)
{
    ROHC_ENTER_FN("rohc_inter_r1_ptype_func");
    ROHC_EXIT_FN("rohc_inter_r1_ptype_func");   
    if(*(p_pkt+cid_len+1)& 0x40)
    {
        switch(ROHC_GET_2MSB(*(p_pkt+cid_len+2)))
        {
            case 0: return ROHC_R1E0P1_PKTTYPE;
            case 1: return ROHC_R1E1P1_PKTTYPE;
            case 2: return ROHC_R1E2P1_PKTTYPE;
            case 3: return ROHC_R1E3P1_PKTTYPE;
        }
    }
    return ROHC_R1P1_PKTTYPE;
}
/****************************************************************************
 * Name     :   rohc_inter_r1idts_ptype_func
 * Desc     :   it determines the packet type 
 * Params   :   Pointer to ROHC packet
            :   CID length
 * Returns  : 	Type of the packet
 **************************************************************************/
rohc_U8bit_t 
rohc_inter_r1idts_ptype_func
                ROHC_PARAMS((p_pkt,cid_len),
                rohc_U8bit_t    *p_pkt _AND_
                rohc_U8bit_t    cid_len)
{
    ROHC_ENTER_FN("rohc_inter_r1idts_ptype_func");
    ROHC_EXIT_FN("rohc_inter_r1idts_ptype_func");
    if(*(p_pkt+cid_len+1) & 0x40)
    {
        if(*(p_pkt+cid_len+1) & 0x20)
        {
            switch(ROHC_GET_2MSB(*(p_pkt+cid_len+2)))
            {
                case 0: return ROHC_R1TSE0P1_PKTTYPE;
                case 1: return ROHC_R1TSE1P1_PKTTYPE;
                case 2: return ROHC_R1TSE2P1_PKTTYPE;
                case 3: return ROHC_R1TSE3P1_PKTTYPE;
            }
        }
        else
        {
            switch(ROHC_GET_2MSB(*(p_pkt+cid_len+2)))
            {
                case 0: return ROHC_R1IDE0P1_PKTTYPE;
                case 1: return ROHC_R1IDE1P1_PKTTYPE;
                case 2: return ROHC_R1IDE2P1_PKTTYPE;
                case 3: return ROHC_R1IDE3P1_PKTTYPE;
            }
        }
    }
    if(*(p_pkt+cid_len+1) & 0x20)
        return ROHC_R1TSP1_PKTTYPE;
    else
        return ROHC_R1IDP1_PKTTYPE;
}
/****************************************************************************
 * Name     :   rohc_inter_uo1_ptype_func
 * Desc     :   it determines the packet type 
 * Params   :   Pointer to ROHC packet
            :   CID length
 * Returns  : 	Type of the packet
 **************************************************************************/
rohc_U8bit_t 
rohc_inter_uo1_ptype_func
                ROHC_PARAMS((p_pkt,cid_len),
                rohc_U8bit_t    *p_pkt _AND_
                rohc_U8bit_t    cid_len)
{
    ROHC_ENTER_FN("rohc_inter_uo1_ptype_func");
    /* +- SPR 17777 */
    LOG_PDCP_MSG( ROHC_CONTEXT_UNUSED, LOGDEBUG, PDCP_RRCOAM, pdcpCurrentTime_g,
            __LINE__,sizeof(p_pkt),cid_len,
            0,0,0,0, __func__,"");
    ROHC_EXIT_FN("rohc_inter_uo1_ptype_func");
    return ROHC_UO1P1_PKTTYPE;
}
/****************************************************************************
 * Name     :   rohc_inter_uo1idts_ptype_func
 * Desc     :   it determines the packet type 
 * Params   :   Pointer to ROHC packet
            :   CID length
 * Returns  : 	Type of the packet
 **************************************************************************/
rohc_U8bit_t 
rohc_inter_uo1idts_ptype_func
                ROHC_PARAMS((p_pkt,cid_len),
                rohc_U8bit_t    *p_pkt _AND_
                rohc_U8bit_t    cid_len)
{
    ROHC_ENTER_FN("rohc_inter_uo1idts_ptype_func");
    ROHC_EXIT_FN("rohc_inter_uo1idts_ptype_func");
    if(*p_pkt & 0x20)
    {
        return ROHC_UO1TSP1_PKTTYPE;
    }
    if(*(p_pkt+cid_len+1) & 0x80)
    {
        switch(ROHC_GET_2MSB(*(p_pkt+cid_len+2)))
        {
            case 0: return ROHC_UO1IDE0P1_PKTTYPE;
            case 1: return ROHC_UO1IDE1P1_PKTTYPE;
            case 2: return ROHC_UO1IDE2P1_PKTTYPE;
            case 3: return ROHC_UO1IDE3P1_PKTTYPE;
        }
    }
    return ROHC_UO1IDP1_PKTTYPE;
}
/****************************************************************************
 * Name     :   rohc_inter_uor2_ptype_func
 * Desc     :   it determines the packet type 
 * Params   :   Pointer to ROHC packet
            :   CID length
 * Returns  : 	Type of the packet
 **************************************************************************/
rohc_U8bit_t 
rohc_inter_uor2_ptype_func
                ROHC_PARAMS((p_pkt,cid_len),
                rohc_U8bit_t    *p_pkt _AND_
                rohc_U8bit_t    cid_len)
{
    ROHC_ENTER_FN("rohc_inter_uor2_ptype_func");
    ROHC_EXIT_FN("rohc_inter_uor2_ptype_func");
    if(*(p_pkt+cid_len+2) & 0x80)
    {
        switch(ROHC_GET_2MSB(*(p_pkt+cid_len+3)))
        {
            case 0: return ROHC_UOR2E0P1_PKTTYPE;
            case 1: return ROHC_UOR2E1P1_PKTTYPE;
            case 2: return ROHC_UOR2E2P1_PKTTYPE;
            case 3: return ROHC_UOR2E3P1_PKTTYPE;
        }
    }
    return ROHC_UOR2P1_PKTTYPE;
}
/****************************************************************************
 * Name     :   rohc_inter_uor2idts_ptype_func
 * Desc     :   it determines the packet type 
 * Params   :   Pointer to ROHC packet
            :   CID length
 * Returns  : 	Type of the packet
 **************************************************************************/
rohc_U8bit_t 
rohc_inter_uor2idts_ptype_func
                ROHC_PARAMS((p_pkt,cid_len),
                rohc_U8bit_t    *p_pkt _AND_
                rohc_U8bit_t    cid_len)
{
    ROHC_ENTER_FN("rohc_inter_uor2idts_ptype_func");
    ROHC_EXIT_FN("rohc_inter_uor2idts_ptype_func");
    if(*(p_pkt+cid_len+2) & 0x80)
    {
        if(*(p_pkt+cid_len+1) & 0x80)
        {
            switch(ROHC_GET_2MSB(*(p_pkt+cid_len+3)))
            {
                case 0: return ROHC_UOR2TSE0P1_PKTTYPE;
                case 1: return ROHC_UOR2TSE1P1_PKTTYPE;
                case 2: return ROHC_UOR2TSE2P1_PKTTYPE;
                case 3: return ROHC_UOR2TSE3P1_PKTTYPE;
            }
        }
        else
        {
            switch(ROHC_GET_2MSB(*(p_pkt+cid_len+3)))
            {
                case 0: return ROHC_UOR2IDE0P1_PKTTYPE;
                case 1: return ROHC_UOR2IDE1P1_PKTTYPE;
                case 2: return ROHC_UOR2IDE2P1_PKTTYPE;
                case 3: return ROHC_UOR2IDE3P1_PKTTYPE;
            }
        }
    }
    if(*(p_pkt+cid_len+1) & 0x80)
        return ROHC_UOR2TSP1_PKTTYPE;
    else
        return ROHC_UOR2IDP1_PKTTYPE;
}


/****************************************************************************
 * Name     :   rohc_d_ppm_init
 * Desc     :   It initialize the all function pointers and staic data
 * Params   :   void
 * Returns  :   void
 **************************************************************************/
void 
rohc_d_ppm_init
    ROHC_NO_PARAMS()
{
    rohc_U8bit_t j,k;
/* for profile 1 */
/*for type 0 pkts */
    ROHC_ENTER_FN("rohc_d_ppm_init");
    for(j=ROHC_RND_ABSENT;j<ROHC_MAX_RND_TYPES;j++)
        for(k=ROHC_RND_ABSENT;k<ROHC_MAX_RND_TYPES;k++)
        {
            rohc_inter_ptypes[0][ROHC_MODE_U][j][k]=ROHC_INTER_UO0_PTYPE;
            rohc_inter_ptypes[1][ROHC_MODE_U][j][k]=ROHC_INTER_UO0_PTYPE;
            rohc_inter_ptypes[2][ROHC_MODE_U][j][k]=ROHC_INTER_UO0_PTYPE;
            rohc_inter_ptypes[3][ROHC_MODE_U][j][k]=ROHC_INTER_UO0_PTYPE;
            rohc_inter_ptypes[0][ROHC_MODE_O][j][k]=ROHC_INTER_UO0_PTYPE;
            rohc_inter_ptypes[1][ROHC_MODE_O][j][k]=ROHC_INTER_UO0_PTYPE;
            rohc_inter_ptypes[2][ROHC_MODE_O][j][k]=ROHC_INTER_UO0_PTYPE;
            rohc_inter_ptypes[3][ROHC_MODE_O][j][k]=ROHC_INTER_UO0_PTYPE;
            rohc_inter_ptypes[0][ROHC_MODE_R][j][k]=ROHC_INTER_R0_PTYPE;
            rohc_inter_ptypes[1][ROHC_MODE_R][j][k]=ROHC_INTER_R0_PTYPE;
            rohc_inter_ptypes[2][ROHC_MODE_R][j][k]=ROHC_INTER_R0CRC_PTYPE;
            rohc_inter_ptypes[3][ROHC_MODE_R][j][k]=ROHC_INTER_R0CRC_PTYPE;
        }

/*for type 1 pkts */

    rohc_inter_ptypes[4][ROHC_MODE_U][ROHC_RND_ABSENT][ROHC_RND_ABSENT]=
    rohc_inter_ptypes[4][ROHC_MODE_U][ROHC_RND_ABSENT][ROHC_RND_PRESENT]=
    rohc_inter_ptypes[4][ROHC_MODE_U][ROHC_RND_ABSENT][ROHC_RND_INVALID]=
    rohc_inter_ptypes[4][ROHC_MODE_U][ROHC_RND_PRESENT][ROHC_RND_ABSENT]=
    rohc_inter_ptypes[4][ROHC_MODE_U][ROHC_RND_INVALID][ROHC_RND_ABSENT]=
    rohc_inter_ptypes[4][ROHC_MODE_O][ROHC_RND_ABSENT][ROHC_RND_ABSENT]=
    rohc_inter_ptypes[4][ROHC_MODE_O][ROHC_RND_ABSENT][ROHC_RND_PRESENT]=
    rohc_inter_ptypes[4][ROHC_MODE_O][ROHC_RND_ABSENT][ROHC_RND_INVALID]=
    rohc_inter_ptypes[4][ROHC_MODE_O][ROHC_RND_PRESENT][ROHC_RND_ABSENT]=
    rohc_inter_ptypes[4][ROHC_MODE_O][ROHC_RND_INVALID][ROHC_RND_ABSENT]=
    rohc_inter_ptypes[5][ROHC_MODE_U][ROHC_RND_ABSENT][ROHC_RND_ABSENT]=
    rohc_inter_ptypes[5][ROHC_MODE_U][ROHC_RND_ABSENT][ROHC_RND_PRESENT]=
    rohc_inter_ptypes[5][ROHC_MODE_U][ROHC_RND_ABSENT][ROHC_RND_INVALID]=
    rohc_inter_ptypes[5][ROHC_MODE_U][ROHC_RND_PRESENT][ROHC_RND_ABSENT]=
    rohc_inter_ptypes[5][ROHC_MODE_U][ROHC_RND_INVALID][ROHC_RND_ABSENT]=
    rohc_inter_ptypes[5][ROHC_MODE_O][ROHC_RND_ABSENT][ROHC_RND_ABSENT]=
    rohc_inter_ptypes[5][ROHC_MODE_O][ROHC_RND_ABSENT][ROHC_RND_PRESENT]=
    rohc_inter_ptypes[5][ROHC_MODE_O][ROHC_RND_ABSENT][ROHC_RND_INVALID]=
    rohc_inter_ptypes[5][ROHC_MODE_O][ROHC_RND_PRESENT][ROHC_RND_ABSENT]=
    rohc_inter_ptypes[5][ROHC_MODE_O][ROHC_RND_INVALID][ROHC_RND_ABSENT]=
                                                    ROHC_INTER_UO1IDTS_PTYPE;

    rohc_inter_ptypes[4][ROHC_MODE_U][ROHC_RND_PRESENT][ROHC_RND_PRESENT]=
    rohc_inter_ptypes[4][ROHC_MODE_U][ROHC_RND_PRESENT][ROHC_RND_INVALID]=
    rohc_inter_ptypes[4][ROHC_MODE_U][ROHC_RND_INVALID][ROHC_RND_PRESENT]=
    rohc_inter_ptypes[4][ROHC_MODE_U][ROHC_RND_INVALID][ROHC_RND_INVALID]=
    rohc_inter_ptypes[4][ROHC_MODE_O][ROHC_RND_PRESENT][ROHC_RND_PRESENT]=
    rohc_inter_ptypes[4][ROHC_MODE_O][ROHC_RND_PRESENT][ROHC_RND_INVALID]=
    rohc_inter_ptypes[4][ROHC_MODE_O][ROHC_RND_INVALID][ROHC_RND_PRESENT]=
    rohc_inter_ptypes[4][ROHC_MODE_O][ROHC_RND_INVALID][ROHC_RND_INVALID]=
    rohc_inter_ptypes[5][ROHC_MODE_U][ROHC_RND_PRESENT][ROHC_RND_PRESENT]=
    rohc_inter_ptypes[5][ROHC_MODE_U][ROHC_RND_PRESENT][ROHC_RND_INVALID]=
    rohc_inter_ptypes[5][ROHC_MODE_U][ROHC_RND_INVALID][ROHC_RND_PRESENT]=
    rohc_inter_ptypes[5][ROHC_MODE_U][ROHC_RND_INVALID][ROHC_RND_INVALID]=
    rohc_inter_ptypes[5][ROHC_MODE_O][ROHC_RND_PRESENT][ROHC_RND_PRESENT]=
    rohc_inter_ptypes[5][ROHC_MODE_O][ROHC_RND_PRESENT][ROHC_RND_INVALID]=
    rohc_inter_ptypes[5][ROHC_MODE_O][ROHC_RND_INVALID][ROHC_RND_PRESENT]=
    rohc_inter_ptypes[5][ROHC_MODE_O][ROHC_RND_INVALID][ROHC_RND_INVALID]=
                                                    ROHC_INTER_UO1_PTYPE;


    rohc_inter_ptypes[4][ROHC_MODE_R][ROHC_RND_ABSENT][ROHC_RND_ABSENT]=
    rohc_inter_ptypes[4][ROHC_MODE_R][ROHC_RND_ABSENT][ROHC_RND_PRESENT]=
    rohc_inter_ptypes[4][ROHC_MODE_R][ROHC_RND_ABSENT][ROHC_RND_INVALID]=
    rohc_inter_ptypes[4][ROHC_MODE_R][ROHC_RND_PRESENT][ROHC_RND_ABSENT]=
    rohc_inter_ptypes[4][ROHC_MODE_R][ROHC_RND_INVALID][ROHC_RND_ABSENT]=
    rohc_inter_ptypes[5][ROHC_MODE_R][ROHC_RND_ABSENT][ROHC_RND_ABSENT]=
    rohc_inter_ptypes[5][ROHC_MODE_R][ROHC_RND_ABSENT][ROHC_RND_PRESENT]=
    rohc_inter_ptypes[5][ROHC_MODE_R][ROHC_RND_ABSENT][ROHC_RND_INVALID]=
    rohc_inter_ptypes[5][ROHC_MODE_R][ROHC_RND_PRESENT][ROHC_RND_ABSENT]=
    rohc_inter_ptypes[5][ROHC_MODE_R][ROHC_RND_INVALID][ROHC_RND_ABSENT]=
                                                    ROHC_INTER_R1IDTS_PTYPE;

    rohc_inter_ptypes[4][ROHC_MODE_R][ROHC_RND_PRESENT][ROHC_RND_PRESENT]=
    rohc_inter_ptypes[4][ROHC_MODE_R][ROHC_RND_PRESENT][ROHC_RND_INVALID]=
    rohc_inter_ptypes[4][ROHC_MODE_R][ROHC_RND_INVALID][ROHC_RND_PRESENT]=
    rohc_inter_ptypes[4][ROHC_MODE_R][ROHC_RND_INVALID][ROHC_RND_INVALID]=
    rohc_inter_ptypes[5][ROHC_MODE_R][ROHC_RND_PRESENT][ROHC_RND_PRESENT]=
    rohc_inter_ptypes[5][ROHC_MODE_R][ROHC_RND_PRESENT][ROHC_RND_INVALID]=
    rohc_inter_ptypes[5][ROHC_MODE_R][ROHC_RND_INVALID][ROHC_RND_PRESENT]=
    rohc_inter_ptypes[5][ROHC_MODE_R][ROHC_RND_INVALID][ROHC_RND_INVALID]=
                                                ROHC_INTER_R1_PTYPE;

/*for type 2 pkts */

    for(j=ROHC_MODE_U;j<ROHC_MAX_MODE_TYPES;j++)
    {
        rohc_inter_ptypes[6][j][ROHC_RND_ABSENT][ROHC_RND_ABSENT]=
        rohc_inter_ptypes[6][j][ROHC_RND_ABSENT][ROHC_RND_PRESENT]=
        rohc_inter_ptypes[6][j][ROHC_RND_ABSENT][ROHC_RND_INVALID]=
        rohc_inter_ptypes[6][j][ROHC_RND_PRESENT][ROHC_RND_ABSENT]=
        rohc_inter_ptypes[6][j][ROHC_RND_INVALID][ROHC_RND_ABSENT]=
                                                ROHC_INTER_UOR2IDTS_PTYPE;

        rohc_inter_ptypes[6][j][ROHC_RND_PRESENT][ROHC_RND_PRESENT]=
        rohc_inter_ptypes[6][j][ROHC_RND_PRESENT][ROHC_RND_INVALID]=
        rohc_inter_ptypes[6][j][ROHC_RND_INVALID][ROHC_RND_PRESENT]=
        rohc_inter_ptypes[6][j][ROHC_RND_INVALID][ROHC_RND_INVALID]=
                                                ROHC_INTER_UOR2_PTYPE;
    }

/*function pointers init */

    rohc_funcptr_ptypes[ROHC_INTER_R0_PTYPE] = rohc_inter_r0_ptype_func;
    rohc_funcptr_ptypes[ROHC_INTER_R0CRC_PTYPE] = rohc_inter_r0crc_ptype_func;
    rohc_funcptr_ptypes[ROHC_INTER_UO0_PTYPE] = rohc_inter_uo0_ptype_func;
    rohc_funcptr_ptypes[ROHC_INTER_R1_PTYPE] = rohc_inter_r1_ptype_func;
    rohc_funcptr_ptypes[ROHC_INTER_R1IDTS_PTYPE] = rohc_inter_r1idts_ptype_func;
    rohc_funcptr_ptypes[ROHC_INTER_UO1_PTYPE] = rohc_inter_uo1_ptype_func;
    rohc_funcptr_ptypes[ROHC_INTER_UO1IDTS_PTYPE] = 
                                            rohc_inter_uo1idts_ptype_func;
    rohc_funcptr_ptypes[ROHC_INTER_UOR2_PTYPE] = rohc_inter_uor2_ptype_func;
    rohc_funcptr_ptypes[ROHC_INTER_UOR2IDTS_PTYPE] = 
                                            rohc_inter_uor2idts_ptype_func;

/* for profile 2 and 3 */
/* for type 0 */
    for(j=0;j< 4;j++)
    {
        rohc_inter_ptypes_p23[j][ROHC_MODE_U]=ROHC_UO0P1_PKTTYPE;
        rohc_inter_ptypes_p23[j][ROHC_MODE_O]=ROHC_UO0P1_PKTTYPE;
    }
    rohc_inter_ptypes_p23[0][ROHC_MODE_R]=ROHC_R0P1_PKTTYPE;
    rohc_inter_ptypes_p23[1][ROHC_MODE_R]=ROHC_R0P1_PKTTYPE;
    rohc_inter_ptypes_p23[2][ROHC_MODE_R]=ROHC_R0CRCP1_PKTTYPE;
    rohc_inter_ptypes_p23[3][ROHC_MODE_R]=ROHC_R0CRCP1_PKTTYPE;

/*for type 1 */
    rohc_inter_ptypes_p23[4][ROHC_MODE_U]=ROHC_UO1P23_PKTTYPE;
    rohc_inter_ptypes_p23[4][ROHC_MODE_O]=ROHC_UO1P23_PKTTYPE;
    rohc_inter_ptypes_p23[5][ROHC_MODE_U]=ROHC_UO1P23_PKTTYPE;
    rohc_inter_ptypes_p23[5][ROHC_MODE_O]=ROHC_UO1P23_PKTTYPE;

    rohc_inter_ptypes_p23[4][ROHC_MODE_R]=ROHC_R1P23_PKTTYPE;
    rohc_inter_ptypes_p23[5][ROHC_MODE_R]=ROHC_R1P23_PKTTYPE;

/*for type 2 */
    rohc_inter_ptypes_p23[6][ROHC_MODE_U]=ROHC_UOR2P23_PKTTYPE;
    rohc_inter_ptypes_p23[6][ROHC_MODE_O]=ROHC_UOR2P23_PKTTYPE;
    rohc_inter_ptypes_p23[6][ROHC_MODE_R]=ROHC_UOR2P23_PKTTYPE;


    ROHC_EXIT_FN("rohc_d_ppm_init");
}


static void rohcGetGenericOptionLen ( 
            rohc_dsc_tcp_t          *p_dsc ,
            rohc_U8bit_t            *pRohcGenricOptionListVar )
{
    rohc_U8bit_t *pTcpOptOrig =  
        (((rohc_U8bit_t *)(p_dsc->tcp)) + sizeof(rohc_tcp_t));
    rohc_U8bit_t *pTcpOpt =
        (((rohc_U8bit_t *)(p_dsc->tcp)) + sizeof(rohc_tcp_t));
    rohc_U8bit_t optType;
    /*Update TCP buffer in */
    rohc_U8bit_t len;
    for( ; ((*pTcpOpt != TCP_EOL_OPT) && 
                (((( p_dsc->tcp->offset >>4)*4) - 20) !=(pTcpOpt - pTcpOptOrig))) ; )
    { 
        /* Traverse each field of irregular chain */
        switch(*pTcpOpt)
        {  /*  */
            case TCP_MSS_OPT:
                (p_dsc)->dpib->mss[0] = *(pTcpOpt + 2);     
                (p_dsc)->dpib->mss[1] = *(pTcpOpt + 3);     
                pTcpOpt+= 4;
                break;    
            case TCP_WS_OPT:
                (p_dsc)->dpib->wscale = *(pTcpOpt + 2);     
                pTcpOpt += 3;
                break;
            case TCP_SACK_PERMTD_OPT:
                pTcpOpt += 2;
                break;
            case TCP_NOP_OPT:
                pTcpOpt += 1;
                break;
            case TCP_SACK_OPT:
                pTcpOpt++;
                if (((p_dsc)->dpib->
                            field_val[ROHC_TCP_LIST_PRESENT_FID- ROHC_TCP_CTRL_FID_BASE]))
                {
                    ROHC_MEMCPY( (p_dsc)->dpib->sackBlockUnchanged ,
                            pTcpOpt , (*pTcpOpt - 1  ));
                }
                pTcpOpt += (*pTcpOpt - 1  );
                break;   
            case TCP_TS_OPT:
                pTcpOpt++;
                pTcpOpt += (*pTcpOpt - 1 );
                break;

            default:
                /* Handling of Generic Options*/
                optType = *pTcpOpt++;
                /*Update TCP buffer in */
                len=  *pTcpOpt++;
                *(pRohcGenricOptionListVar + optType ) = len;
                ROHC_MEMCPY ((p_dsc)->dpib->tcpOptGen
                        [optType].optionString , pTcpOpt 
                        , len -2 ) ;
                pTcpOpt += (len - 2 );
                break;     
        }
    }
	return;  
}

/****************************************************************************
 * Name     :   rohcP6GetIrregChainType
 * Desc     :   Function to get Irregular chain type for profile 6
 * Params   :   Pointer to DSC context
 * Returns  :   Irregular chain type
 **************************************************************************/
STATIC rohc_U8bit_t  
rohcP6GetIrregChainType(
    rohc_dsc_tcp_t          * p_dsc ,
    rohc_U8bit_t            *p_next_header)
{
    rohc_U8bit_t irregular_chain_type = 0;
    if (p_dsc->outer_ip4)
    {
        if (p_dsc->ttl_irregular_chain_flag)
        {
            irregular_chain_type = ROHC_P6_IPV4_OUTER_WITH_TTL_IRREG_CHAIN;
        }
        else
        {
            irregular_chain_type = ROHC_P6_IPV4_OUTER_WITHOUT_TTL_IRREG_CHAIN;
        }
        *p_next_header = ((rohc_dsc_tcp_t*)p_dsc)->outer_ip4->proto;
    }
    else
    {   /* Checking for Inner Ip Version 4 */
        if (p_dsc->inner_ip4)
        {
            irregular_chain_type = ROHC_P6_IPV4_INNERMOST_IRREG_CHAIN;
            *p_next_header = (p_dsc)->inner_ip4->proto;
        }
        else
        {
            if (p_dsc->outer_ip6)
            {
                if (p_dsc->ttl_irregular_chain_flag)
                {
                    irregular_chain_type = ROHC_P6_IPV6_OUTER_WITH_TTL_IRREG_CHAIN;
                }
                else
                {
                    irregular_chain_type = ROHC_P6_IPV6_OUTER_WITHOUT_TTL_IRREG_CHAIN;
                }
                *p_next_header = (p_dsc)->outer_ip6->
                    ip6_ctlun.ip6_un1.un1_nxt;
            }
            else	
            {
                if (p_dsc->inner_ip6)
                {
                    irregular_chain_type = ROHC_P6_IPV6_INNERMOST_IRREG_CHAIN;
                    *p_next_header = (p_dsc)->inner_ip6
                        ->ip6_ctlun.ip6_un1.un1_nxt;
                }
            }
        }
    }
    return irregular_chain_type;
}

/****************************************************************************
 * Name     :   rohcP6DecodeIPIrregChain
 * Desc     :   Function to decode Irregular chain for ip 
 * Params   :   Pointer to DSC context,
 *				Irregular chain type,
 *				pkt_type - type of Packet,
 * 				p_rohc_pkt - a pointer to ROHC Packet
 * Returns  :   void
 **************************************************************************/
STATIC rohc_void_t  
rohcP6DecodeIPIrregChain(
    rohc_dsc_tcp_t          * p_dsc ,
    rohc_U8bit_t            irregular_chain_type,
    register rohc_U8bit_t   pkt_type ,
    rohc_U8bit_t            **p_rohc_pkt)
{
    rohc_U32bit_t index=0;
    rohc_U16bit_t ip_id;
    rohc_U8bit_t no_of_bits=0,field_id;
    for(index=0;((index< ROHC_MAX_NO_OF_FIELDS_IN_A_IRREG_CHAIN) &&
                (rohc_c_irreg_chain_type[irregular_chain_type][index].field_id!=
                 ROHC_INVALID_FID));index++)
    {
        /* Traverse each field of irregular chain */
        no_of_bits = rohc_c_irreg_chain_type[irregular_chain_type][index].
            no_of_bits;
        field_id = rohc_c_irreg_chain_type[irregular_chain_type][index].
            field_id;

        if ((ROHC_INNERIP_ID_FID == field_id) || (ROHC_OUTERIP_ID_FID == 
                    field_id))
        {

            if(ROHC_CO_P6_COMMON_PKTYPE == pkt_type )
            {
                ip_id =(p_dsc)->dpib->
                    field_val[ROHC_INNERIP_BEH_FID-ROHC_VAR_FID_BASE];
            }
            else
            {
                ip_id = ((rohc_dsc_tcp_t*)p_dsc)->ipid_beh_inner;
            }

            if (IP_ID_BEHAVIOR_RANDOM == ip_id)
            {
                rohcP6COParseFID(((rohc_dsc_tcp_t*)p_dsc),p_rohc_pkt,
                        no_of_bits,field_id,0);
            }
            else if (IP_ID_BEHAVIOR_ZERO == ((rohc_dsc_tcp_t*)p_dsc)->ipid_beh_inner)
            {
                /* NO BUFFER WILL BE FILLED FOR BEHAVIOUR ZERO */
            }
            continue;
        }
        else if ( (ROHC_OUTERIP4_TTL_FID == field_id) 
                || (ROHC_OUTERIP6_HLIM_FID == field_id) )
        {
            rohcP6COParseFID(((rohc_dsc_tcp_t*)p_dsc),p_rohc_pkt,
                    no_of_bits,field_id,0);
            break;
        }
        if (((rohc_dsc_tcp_t*)p_dsc)->ecn_used)
        {
            rohcP6COParseFID((rohc_dsc_tcp_t*)p_dsc,p_rohc_pkt,no_of_bits,
                    field_id,0);
        }
    }/* end for traversal field type in IPv4/IPv6 irregular chain */
}

/****************************************************************************
 * Name     :   rohcP6DecodeTCPIrregChain
 * Desc     :   Function to decode Irregular chain for TCP
 * Params   :   Pointer to DSC context, 
 *				pkt type,
 *				p_rohc_pkt -  a pointer to ROHC Packet
 * Returns  :   void
 **************************************************************************/
STATIC rohc_void_t  
rohcP6DecodeTCPIrregChain(
    rohc_dsc_tcp_t          * p_dsc  ,
    register rohc_U8bit_t   pkt_type ,
    rohc_U8bit_t            ** p_rohc_pkt )
{
    rohc_U32bit_t index=0;
    rohc_U8bit_t no_of_bits=0,field_id;
    for(index=0;((index < ROHC_MAX_NO_OF_FIELDS_IN_A_IRREG_CHAIN) && 
                (rohc_c_irreg_chain_type[ROHC_P6_TCP_IRREG_CHAIN][index].field_id
                 !=ROHC_INVALID_FID)) ;index++)
    {
        no_of_bits = rohc_c_irreg_chain_type[ROHC_P6_TCP_IRREG_CHAIN]
            [index].no_of_bits;
        field_id = rohc_c_irreg_chain_type[ROHC_P6_TCP_IRREG_CHAIN][index].
            field_id;

        if (field_id == ROHC_TCP_CKSUM_FID)
        {
            rohcP6COParseFID(p_dsc,p_rohc_pkt,
                    no_of_bits,field_id,0);
            break;
        }
        if ( (ROHC_CO_P6_RND8_PKTYPE == pkt_type) || 
                (ROHC_CO_P6_SEQ8_PKTYPE == pkt_type) || 
                (ROHC_CO_P6_COMMON_PKTYPE == pkt_type) )
        {
            if (((rohc_dsc_tcp_t*)p_dsc)->dpib->field_val
                    [ROHC_TCP_ECN_USED_FID - ROHC_VAR_FID_BASE])
            {
                rohcP6COParseFID((rohc_dsc_tcp_t*)p_dsc,p_rohc_pkt,
                        no_of_bits,field_id,0);
            }
        }
        else
        {
            if (((rohc_dsc_tcp_t*)p_dsc)->ecn_used)
            {
                rohcP6COParseFID((rohc_dsc_tcp_t*)p_dsc,p_rohc_pkt,
                        no_of_bits,field_id,0);
            }
            else
            {
                /* NO BUFFER WILL BE FILLED IF INNER ECN AND ECN FLAGS ARE 
                   NOT SET*/
            }
        }
    }/* end for traversal field type in TCP irregular chain */
}

/****************************************************************************
 * Name     :   rohcP6DecodeTCPOptionsIrregChain
 * Desc     :   Function to decode Tcp Options Irregular chain
 * Params   :   Pointer to DSC context, 
 *				rohc_pkt
 * Returns  :   void
 **************************************************************************/
STATIC rohc_void_t  
rohcP6DecodeTCPOptionsIrregChain(
    rohc_dsc_tcp_t          * p_dsc ,
    rohc_U8bit_t            ** p_rohc_pkt )
{
    rohc_U8bit_t *pTcpOptOrig, *p_point;
    /* Traverse through each TCP options of irregular 
     * chain of CO packet, End of list is validated by EOL
     * i.e when *p_point reaches TCP_EOL_OPT*/
    for(pTcpOptOrig = p_point=((rohc_U8bit_t *)(p_dsc)->tcp + 
                sizeof(rohc_tcp_t)); ((*p_point != TCP_EOL_OPT ) && 
                ( ((( p_dsc->tcp->offset >>4)*4) - 20) !=
                  (p_point - pTcpOptOrig))) ;)
    {
        /* Traverse each field of irregular chain */
        switch(*p_point)
        {  /*  */
            case TCP_MSS_OPT:   
                p_point += 4;
                break;    
            case TCP_WS_OPT:
                p_point += 3;
                break;
            case TCP_SACK_PERMTD_OPT:
                p_point += 2;
                break;
            case TCP_NOP_OPT:
                p_point += 1;
                break;
            case TCP_SACK_OPT: 
                /* SACK Irregular Chain */ 
                rohcParseSackIregularChain( p_dsc, p_rohc_pkt);
                p_point += 1;
                p_point += (*p_point) -1;
                break;
            case TCP_TS_OPT:
                /*Parsing the TCP OPTIONS Irregular chain*/
                rohcTcpTsOptionIrregular( p_dsc ,
                        p_rohc_pkt, ROHC_TS_FID );
                /*TS Options Parsing */
                rohcTcpTsOptionIrregular( (rohc_dsc_tcp_t*)p_dsc ,
                        p_rohc_pkt, ROHC_TCP_TS_ECHO_FID );
                p_point +=10;
                break;
            default:
                /* Handling of Generic Options*/
                if(*p_point < ROHC_MAX_GEN_OPTION_SUPP)
                {
                    if (!(((rohc_dsc_tcp_t*)p_dsc)->tcpOptGenStaticFlag & ( 0x01 << (*p_point) )))
                    {
                        (p_dsc)->dpib->tcpOptGen[*p_point].genOptDisc =
                            **p_rohc_pkt;
                        if ( 0 ==  **p_rohc_pkt )
                        {
                            *p_rohc_pkt +=1;
                            ROHC_MEMCPY(((rohc_dsc_tcp_t*)p_dsc)->dpib->tcpOptGen
                                    [*p_point].optionString , *p_rohc_pkt , 
                                    (*(p_point + 1)  -1));
                            ((rohc_dsc_tcp_t*)p_dsc)->dpib->tcpOptGen
                                [*p_point].len = *(p_point + 1)  -1;
                            *p_rohc_pkt += *(p_point + 1) -2;
                        }
                        else
                        {
                            *p_rohc_pkt +=1;
                        }
                    }
                    p_point += 1;
                    p_point += (*p_point) -1;
                }
                break;     
        }
    }/* end for traversal field type in TCP options irregular chain */
}
