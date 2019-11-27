/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_d_pbm.c,v 1.3.34.8 2010/11/24 03:42:49 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains D-PBM packet encoding routines.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_d_pbm.c,v $
 * Revision 1.3.34.8  2010/11/24 03:42:49  gur22059
 * Removal of conditional checks w.r.t profile1
 *
 * Revision 1.3.34.7  2010/11/09 05:22:35  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.3.34.6  2010/10/13 03:54:53  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.3.34.5  2010/10/01 14:10:32  gur22059
 * Reduction in the size of context block at decompressor w.r.t profile2
 *
 * Revision 1.3.34.4  2010/08/19 08:55:23  gur22059
 * SPR 569: Introduced the handling for ROHC_R0P1_PKTTYPE type packets
 *          in profile-1 (RTP)for different IP packets.
 *
 * Revision 1.3  2009/05/28 03:35:55  gur19836
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
#include "rohc_d_pbm.h"
#include "rohc_ed_ext_if.h"
#include "rohc_crc_ext_if.h"
#include "rohc_main_ex.h"
#include "rohc_trace_mcr.h"
#include "rohc_gcom_mcr.h"
#include "rohc_dprm_update_p6.h"
/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/



/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
rohc_void_t rohcDprmP6IrIrdynTcpOption(
        rohc_dprm_update_blk_t *p_dprm_update_blk,
        rohc_dsc_tcp_t         *p_dsc,
        rohc_U8bit_t XI,
        rohc_U8bit_t *p_size,
        rohc_U8bit_t *p_opt_list_index);
static void rohcFormTcpOptionfromCompressList(
        rohc_dprm_update_blk_t *p_dprm_update_blk,
        rohc_dsc_tcp_t         *p_dsc);
static void  rohcProcessRsfFlag(
                rohc_dprm_update_blk_t *p_dprm_update_blk,
                rohc_dsc_tcp_t         *p_dsc);
/* + SPR 17439 */
rohc_void_t rohc_update_p2_innerip_from_ext3
    ROHC_PROTO((
    rohc_U8bit_t      * p_pkt,
    rohc_dsc_udp_t    * p_dsc));
rohc_void_t rohc_update_p1_innerip_from_ext3
    ROHC_PROTO((
    rohc_U8bit_t      * p_pkt,
    rohc_dsc_rtp_t    * p_dsc));
rohc_void_t rohc_update_p1_outerip_from_ext3
    ROHC_PROTO((
    rohc_U8bit_t      * p_pkt,
    rohc_dsc_rtp_t    * p_dsc));
rohc_void_t rohc_update_p2_outerip_from_ext3
    ROHC_PROTO((
    rohc_U8bit_t      * p_pkt,
    rohc_dsc_udp_t    * p_dsc));
rohc_void_t rohc_update_p1_from_ext3
    ROHC_PROTO((
    rohc_U8bit_t      * p_pkt,
    rohc_dpib_t   * p_dpib));
/* +- SPR 17777 */
static rohc_U8bit_t rohc_dpbm_make_outer_ip4 ROHC_PROTO((
    rohc_U8bit_t            ** p_pkt             ,
    struct rohc_dsc_rtp_t   * p_dsc));

static rohc_U8bit_t rohc_dpbm_make_outer_ip6 ROHC_PROTO((
    rohc_U8bit_t            ** p_pkt             ,
    struct rohc_dsc_rtp_t   * p_dsc));

static rohc_U8bit_t rohc_dpbm_make_inner_ip4 ROHC_PROTO((
    rohc_U8bit_t            ** p_pkt             ,
    struct rohc_dsc_rtp_t   * p_dsc));

static rohc_U8bit_t rohc_dpbm_make_inner_ip6 ROHC_PROTO((
    rohc_U8bit_t            ** p_pkt             ,
    struct rohc_dsc_rtp_t   * p_dsc));

static rohc_U8bit_t rohc_dpbm_make_udp_rtp ROHC_PROTO((
    rohc_U8bit_t            ** p_pkt             ,
    struct rohc_dsc_rtp_t   * p_dsc             ,
    rohc_U8bit_t              rcv_pkt_typ       
    ));

static void rohc_dpbm_make_p2_outer_ip4 ROHC_PROTO((
    rohc_U8bit_t            ** p_pkt             ,
    struct rohc_dsc_udp_t   * p_dsc));

static void rohc_dpbm_make_p2_outer_ip6 ROHC_PROTO((
    rohc_U8bit_t            ** p_pkt             ,
    struct rohc_dsc_udp_t   * p_dsc));

static void rohc_dpbm_make_p2_inner_ip4 ROHC_PROTO((
    rohc_U8bit_t            ** p_pkt             ,
    struct rohc_dsc_udp_t   * p_dsc));

static void rohc_dpbm_make_p2_inner_ip6 ROHC_PROTO((
    rohc_U8bit_t            ** p_pkt             ,
    struct rohc_dsc_udp_t   * p_dsc));
/* +- SPR 17777 */

STATIC rohc_void_t rohc_d_p1_outer_ip_ext_list_cons ROHC_PROTO((
    struct rohc_dsc_rtp_t   * p_dsc             ,
    rohc_U8bit_t            **p_ext_hdr         ,
    rohc_U8bit_t            **p_ext_hdr2        ,
    rohc_U8bit_t            * p_pkt             ,
    rohc_U8bit_t            *p_out_ip4          ,
    rohc_U8bit_t            *p_out_ip6          ,
    rohc_U8bit_t            *p_len  ));

STATIC rohc_void_t rohc_d_p1_inner_ip_ext_list_cons ROHC_PROTO((
    struct rohc_dsc_rtp_t   * p_dsc             ,
    rohc_U8bit_t            **p_ext_hdr         ,
    rohc_U8bit_t            **p_ext_hdr2        ,
    rohc_U8bit_t            * p_pkt             ,
    rohc_U8bit_t            *p_in_ip4          ,
    rohc_U8bit_t            *p_in_ip6          ,
    rohc_U8bit_t            *p_len  ));

STATIC rohc_void_t rohc_d_p1_update_ip_len_cksum ROHC_PROTO((
    struct rohc_dsc_rtp_t   * p_dsc             ,
    rohc_U8bit_t            *p_in_ip4          ,
    rohc_U8bit_t            *p_in_ip6          ,
    rohc_U8bit_t            *p_out_ip4          ,
    rohc_U8bit_t            *p_out_ip6          ,
    rohc_U8bit_t            * p_pkt ));

STATIC rohc_void_t rohc_d_p2_outer_ip_ext_list_cons ROHC_PROTO((
    struct rohc_dsc_udp_t   * p_dsc             ,
    rohc_U8bit_t            **p_ext_hdr         ,
    rohc_U8bit_t            **p_ext_hdr2        ,
    rohc_U8bit_t            * p_pkt             ,
    rohc_U8bit_t            *p_out_ip4          ,
    rohc_U8bit_t            *p_out_ip6          ,
    rohc_U8bit_t            *p_len  ));

STATIC rohc_void_t rohc_d_p2_inner_ip_ext_list_cons ROHC_PROTO((
    struct rohc_dsc_udp_t   * p_dsc             ,
    rohc_U8bit_t            **p_ext_hdr         ,
    rohc_U8bit_t            **p_ext_hdr2        ,
    rohc_U8bit_t            * p_pkt             ,
    rohc_U8bit_t            *p_in_ip4          ,
    rohc_U8bit_t            *p_in_ip6          ,
    rohc_U8bit_t            *p_len  ));

STATIC rohc_void_t rohc_d_p2_update_ip_len_cksum ROHC_PROTO((
    struct rohc_dsc_udp_t   * p_dsc             ,
    rohc_U8bit_t            *p_in_ip4          ,
    rohc_U8bit_t            *p_in_ip6          ,
    rohc_U8bit_t            *p_out_ip4          ,
    rohc_U8bit_t            *p_out_ip6          ,
    rohc_U8bit_t            * p_pkt ));
/* - SPR 17439 */
STATIC rohc_void_t rohcUpdateTcpHdrCoCommonPkt(
        struct rohc_dsc_tcp_t   * p_dsc ,
        rohc_dprm_update_blk_t  *p_dprm_update_blk);

STATIC rohc_void_t rohcUpdateTcpHdrIRP6CRPkt(
        struct rohc_dsc_tcp_t   * p_dsc ,
        rohc_dprm_update_blk_t  *p_dprm_update_blk);

STATIC rohc_void_t rohcUpdateTcpHdrRND1RND2SEQ1SEQ2Pkt(
        struct rohc_dsc_tcp_t   * p_dsc ,
        rohc_dprm_update_blk_t  *p_dprm_update_blk);

STATIC rohc_void_t rohcUpdateTcpHdrRND3RND4SEQ3SEQ4Pkt(
        struct rohc_dsc_tcp_t   * p_dsc ,
        rohc_dprm_update_blk_t  *p_dprm_update_blk);

STATIC rohc_void_t rohcUpdateTcpHdrRND5RND6SEQ5SEQ6Pkt(
        struct rohc_dsc_tcp_t   * p_dsc ,
        rohc_dprm_update_blk_t  *p_dprm_update_blk);

STATIC rohc_void_t rohcUpdateTcpHdrRND7Pkt(
        struct rohc_dsc_tcp_t   * p_dsc ,
        rohc_dprm_update_blk_t  *p_dprm_update_blk);

STATIC rohc_void_t rohcUpdateTcpHdrSEQ7Pkt(
        struct rohc_dsc_tcp_t   * p_dsc ,
        rohc_dprm_update_blk_t  *p_dprm_update_blk);

STATIC rohc_void_t rohcUpdateTcpHdrRND8SEQ8Pkt(
        struct rohc_dsc_tcp_t   * p_dsc ,
        rohc_dprm_update_blk_t  *p_dprm_update_blk);

STATIC rohc_void_t rohcFormTCPOptionsFromIrregChain(
                struct rohc_dsc_tcp_t   * p_dsc    ,   
                rohc_dprm_update_blk_t  *p_dprm_update_blk );

STATIC rohc_void_t rohcProcessSackBlock4(
                struct rohc_dsc_tcp_t   * p_dsc,
                rohc_dprm_update_blk_t  *p_dprm_update_blk ,
                rohc_U8bit_t *pTcpOpt,
                rohc_U8bit_t *p_len);

STATIC rohc_void_t rohcProcessSackBlock3(
                 struct rohc_dsc_tcp_t   * p_dsc,
                rohc_dprm_update_blk_t  *p_dprm_update_blk ,
                rohc_U8bit_t *pTcpOpt,
                rohc_U8bit_t *p_len);

STATIC rohc_void_t rohcProcessSackBlock2(
                struct rohc_dsc_tcp_t   * p_dsc,
                rohc_dprm_update_blk_t  *p_dprm_update_blk ,
                rohc_U8bit_t *pTcpOpt,
                rohc_U8bit_t *p_len);

STATIC rohc_void_t rohcProcessSackBlock1(
                 struct rohc_dsc_tcp_t   * p_dsc,
                rohc_dprm_update_blk_t  *p_dprm_update_blk ,
                rohc_U8bit_t *pTcpOpt,
                rohc_U8bit_t *p_len);

STATIC rohc_void_t rohcProcessTSOpt(
                struct rohc_dsc_tcp_t   * p_dsc ,
                rohc_U8bit_t *pTcpOpt,
                rohc_U8bit_t *p_len);
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/



/* D-PBM  */
/************************************************************************
 * Name     :   rohc_update_p1_innerip_from_ext3
 * Desc     :   It Updates inner IP Flags from ext3 for non updating pkts
 * Params   :   Pointer to IP Packet
 *        		Pointer to Decompressor Stream context
 * Returns  :   void
***********************************************************************/
rohc_void_t rohc_update_p1_innerip_from_ext3
    ROHC_PARAMS((p_pkt,p_dsc),
    rohc_U8bit_t      * p_pkt _AND_
    rohc_dsc_rtp_t    * p_dsc)
{
    rohc_ip4_t  *p_ip4_pkt;
    rohc_ip6_t  *p_ip6_pkt;
    rohc_dpib_t * p_dpib = p_dsc->dpib;
    
    if(p_dpib->field_val[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE] &
        ROHC_ENC_INNERIPFLAG_EXT3FLAGP1_MASK)
    {   
        /* inner ip present 4/6 */
        /* ip6 */
      if(p_dsc->inner_ip6 != ROHC_NULL)
      {
        p_ip6_pkt = (rohc_ip6_t *)p_pkt;
        if(p_dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] & 
                ROHC_ENC_TOS_INNERIPFLAGP1_MASK )
        {
            p_ip6_pkt->ip6_ctlun.ip6_un1.un1_flow |= 
                (p_dpib->field_val[ROHC_INNERIP6_TCLASS_FID-ROHC_VAR_FID_BASE] & \
                    ROHC_IP6_TRAFFICCLASS_MASK);
        } 
        if(p_dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_TTL_INNERIPFLAGP1_MASK)
        {
            p_ip6_pkt->ip6_ctlun.ip6_un1.un1_hlim = 
                p_dpib->field_val[ROHC_INNERIP6_HLIM_FID-ROHC_VAR_FID_BASE];
        }
        if(p_dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_PROTO_INNERIPFLAGP1_MASK)
        {
            p_ip6_pkt->ip6_ctlun.ip6_un1.un1_nxt = 
                p_dpib->field_val[ROHC_INNERIP6_NHEADER_FID-ROHC_VAR_FID_BASE]  ;
        }
      }
      else
      { 
        /* ip4 */   
        p_ip4_pkt = (rohc_ip4_t *)p_pkt;
        if(p_dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
            ROHC_ENC_TOS_INNERIPFLAGP1_MASK)
        {
            p_ip4_pkt->tos |= 
                (p_dpib->field_val[ROHC_INNERIP4_TOS_FID-ROHC_VAR_FID_BASE] );
        } 
        
        if(p_dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
            ROHC_ENC_TTL_INNERIPFLAGP1_MASK )
        {
            p_ip4_pkt->ttl = 
                p_dpib->field_val[ROHC_INNERIP4_TTL_FID-ROHC_VAR_FID_BASE]  ;
        }
        if(p_dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
            ROHC_ENC_PROTO_INNERIPFLAGP1_MASK)
        {
            p_ip4_pkt->proto = 
                p_dpib->field_val[ROHC_INNERIP4_PROTO_FID-ROHC_VAR_FID_BASE];
        }
        p_ip4_pkt->off |= 
                    (p_dpib->field_val[ROHC_INNERIP_DF_FID-ROHC_VAR_FID_BASE] &\
                                         ROHC_IP4_DF);
      } 
    }
    if( (p_dpib->field_val[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_I_EXT3FLAGP1_MASK) && 
            (p_dsc->inner_ip4 != ROHC_NULL) )
    {
        ((rohc_ip4_t*)p_pkt)->id = 
            p_dpib->field_val[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE];
    }   
}

/************************************************************************
 * Name     :   rohc_update_p2_innerip_from_ext3
 * Desc     :   It Updates inner IP Flags from ext3 for non updating pkts
 * Params   :   Pointer to IP Packet
 *        		Pointer to Decompressor Stream context
 * Returns  :   void
***********************************************************************/
rohc_void_t rohc_update_p2_innerip_from_ext3
    ROHC_PARAMS((p_pkt,p_dsc),
    rohc_U8bit_t      * p_pkt _AND_
    rohc_dsc_udp_t    * p_dsc)
{
    rohc_ip4_t  *p_ip4_pkt;
    rohc_ip6_t  *p_ip6_pkt;
    rohc_dpib_t       * p_dpib = p_dsc->dpib;
    
    if(p_dpib->field_val[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE] &
        ROHC_ENC_INNERIPFLAG_EXT3FLAGP1_MASK)
    {   
        /* inner ip present 4/6 */
        /* ip6 */
      if(p_dsc->inner_ip6 != ROHC_NULL)
      {
        p_ip6_pkt = (rohc_ip6_t *)p_pkt;
        if(p_dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] & 
                ROHC_ENC_TOS_INNERIPFLAGP1_MASK )
        {
            p_ip6_pkt->ip6_ctlun.ip6_un1.un1_flow |= 
                (p_dpib->field_val[ROHC_INNERIP6_TCLASS_FID-ROHC_VAR_FID_BASE] & \
                    ROHC_IP6_TRAFFICCLASS_MASK);
        } 
        if(p_dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_TTL_INNERIPFLAGP1_MASK)
        {
            p_ip6_pkt->ip6_ctlun.ip6_un1.un1_hlim = 
                p_dpib->field_val[ROHC_INNERIP6_HLIM_FID-ROHC_VAR_FID_BASE];
        }
        if(p_dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_PROTO_INNERIPFLAGP1_MASK)
        {
            p_ip6_pkt->ip6_ctlun.ip6_un1.un1_nxt = 
                p_dpib->field_val[ROHC_INNERIP6_NHEADER_FID-ROHC_VAR_FID_BASE]  ;
        }
      }
      else
      { 
        /* ip4 */   
        p_ip4_pkt = (rohc_ip4_t *)p_pkt;
        if(p_dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
            ROHC_ENC_TOS_INNERIPFLAGP1_MASK)
        {
            p_ip4_pkt->tos |= 
                (p_dpib->field_val[ROHC_INNERIP4_TOS_FID-ROHC_VAR_FID_BASE] );
        } 
        
        if(p_dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
            ROHC_ENC_TTL_INNERIPFLAGP1_MASK )
        {
            p_ip4_pkt->ttl = 
                p_dpib->field_val[ROHC_INNERIP4_TTL_FID-ROHC_VAR_FID_BASE]  ;
        }
        if(p_dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
            ROHC_ENC_PROTO_INNERIPFLAGP1_MASK)
        {
            p_ip4_pkt->proto = 
                p_dpib->field_val[ROHC_INNERIP4_PROTO_FID-ROHC_VAR_FID_BASE];
        }
        p_ip4_pkt->off |= 
                    (p_dpib->field_val[ROHC_INNERIP_DF_FID-ROHC_VAR_FID_BASE] &\
                                         ROHC_IP4_DF);
      } 
    }
    if( (p_dpib->field_val[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_I_EXT3FLAGP1_MASK) && 
            (p_dsc->inner_ip4 != ROHC_NULL) )
    {
        ((rohc_ip4_t*)p_pkt)->id = 
            p_dpib->field_val[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE];
    }   
}
/* D-PBM  */

/************************************************************************
 * Name     :   rohc_update_p1_outerip_from_ext3
 * Desc     :   It Updates outer IP Flags from ext3 for non updating pkts
 * Params   :   Pointer to IP Packet
 *        		Pointer to Decompressor Stream context
 * Returns  :   void
***********************************************************************/
rohc_void_t rohc_update_p1_outerip_from_ext3
    ROHC_PARAMS((p_pkt,p_dsc),
    rohc_U8bit_t      * p_pkt _AND_
    rohc_dsc_rtp_t    * p_dsc)
{
    rohc_ip4_t      *p_ip4_pkt;
    rohc_ip6_t      *p_ip6_pkt;
    rohc_dpib_t     * p_dpib = p_dsc->dpib;
    /* Either of outer ip4 or ip4 would be present OR  */
    /* none will be present */
    /* outer ip present */
    if((p_dpib->field_val[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE] &
        ROHC_ENC_INNERIPFLAG_EXT3FLAGP1_MASK) &&
        (p_dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
        ROHC_ENC_IP2_INNERIPFLAGP1_MASK ) )
    {   
        /* ip-6*/
      if(p_dsc->outer_ip6 != ROHC_NULL)
      {
        p_ip6_pkt = (rohc_ip6_t *)p_pkt;
        if(p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_TOS_OUTERIPFLAGP1_MASK)
        {
            p_ip6_pkt->ip6_ctlun.ip6_un1.un1_flow |= 
                (p_dpib->field_val[ROHC_OUTERIP6_TCLASS_FID-ROHC_VAR_FID_BASE] & \
                    ROHC_IP6_TRAFFICCLASS_MASK);
        } 
        if(p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_TTL_OUTERIPFLAGP1_MASK)
        {
            p_ip6_pkt->ip6_ctlun.ip6_un1.un1_hlim = 
                p_dpib->field_val[ROHC_OUTERIP6_HLIM_FID-ROHC_VAR_FID_BASE];
        }
        if(p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_PROTO_OUTERIPFLAGP1_MASK)
        {
            p_ip6_pkt->ip6_ctlun.ip6_un1.un1_nxt = 
                p_dpib->field_val[ROHC_OUTERIP6_NHEADER_FID-ROHC_VAR_FID_BASE];
        }
      }
      else if(p_dsc->outer_ip4 != ROHC_NULL)
      {
        /*ip-4 */
        p_ip4_pkt = (rohc_ip4_t *)p_pkt;
        if(p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_TOS_OUTERIPFLAGP1_MASK)
        {
            p_ip4_pkt->tos |= 
                (p_dpib->field_val[ROHC_OUTERIP4_TOS_FID-ROHC_VAR_FID_BASE]);
        } 
        
        if(p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_TTL_OUTERIPFLAGP1_MASK)
        {
            p_ip4_pkt->ttl = 
                p_dpib->field_val[ROHC_OUTERIP4_TTL_FID-ROHC_VAR_FID_BASE];
        }
        if(p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_PROTO_OUTERIPFLAGP1_MASK)
        {
            p_ip4_pkt->proto = 
                p_dpib->field_val[ROHC_OUTERIP4_PROTO_FID-ROHC_VAR_FID_BASE];
        }
        if(p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_I_OUTERIPFLAGP1_MASK)
        {
            p_ip4_pkt->id = 
                p_dpib->field_val[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE];
        }
        p_ip4_pkt->off |= 
                    (p_dpib->field_val[ROHC_OUTERIP_DF_FID-ROHC_VAR_FID_BASE] &\
                                         ROHC_IP4_DF);
      }
    }
}

/************************************************************************
 * Name     :   rohc_update_p2_outerip_from_ext3
 * Desc     :   It Updates outer IP Flags from ext3 for non updating pkts
 * Params   :   Pointer to IP Packet 
 *        		Pointer to Decompressor Stream context
 * Returns  :   void
***********************************************************************/
rohc_void_t rohc_update_p2_outerip_from_ext3
    ROHC_PARAMS((p_pkt,p_dsc),
    rohc_U8bit_t      * p_pkt _AND_
    rohc_dsc_udp_t    * p_dsc)
{
    rohc_ip4_t  *p_ip4_pkt;
    rohc_ip6_t  *p_ip6_pkt;
    rohc_dpib_t       * p_dpib = p_dsc->dpib;
    /* Either of outer ip4 or ip4 would be present OR  */
    /* none will be present */
    /* outer ip present */
    if((p_dpib->field_val[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE] &
        ROHC_ENC_INNERIPFLAG_EXT3FLAGP1_MASK) &&
        (p_dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
        ROHC_ENC_IP2_INNERIPFLAGP1_MASK ) )
    {   
        /* ip-6*/
      if(p_dsc->outer_ip6 != ROHC_NULL)
      {
        p_ip6_pkt = (rohc_ip6_t *)p_pkt;
        if(p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_TOS_OUTERIPFLAGP1_MASK)
        {
            p_ip6_pkt->ip6_ctlun.ip6_un1.un1_flow |= 
                (p_dpib->field_val[ROHC_OUTERIP6_TCLASS_FID-ROHC_VAR_FID_BASE] & \
                    ROHC_IP6_TRAFFICCLASS_MASK);
        } 
        if(p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_TTL_OUTERIPFLAGP1_MASK)
        {
            p_ip6_pkt->ip6_ctlun.ip6_un1.un1_hlim = 
                p_dpib->field_val[ROHC_OUTERIP6_HLIM_FID-ROHC_VAR_FID_BASE];
        }
        if(p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_PROTO_OUTERIPFLAGP1_MASK)
        {
            p_ip6_pkt->ip6_ctlun.ip6_un1.un1_nxt = 
                p_dpib->field_val[ROHC_OUTERIP6_NHEADER_FID-ROHC_VAR_FID_BASE];
        }
      }
      else if(p_dsc->outer_ip4 != ROHC_NULL)
      {
        /*ip-4 */
        p_ip4_pkt = (rohc_ip4_t *)p_pkt;
        if(p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_TOS_OUTERIPFLAGP1_MASK)
        {
            p_ip4_pkt->tos |= 
                (p_dpib->field_val[ROHC_OUTERIP4_TOS_FID-ROHC_VAR_FID_BASE]);
        } 
        
        if(p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_TTL_OUTERIPFLAGP1_MASK)
        {
            p_ip4_pkt->ttl = 
                p_dpib->field_val[ROHC_OUTERIP4_TTL_FID-ROHC_VAR_FID_BASE];
        }
        if(p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_PROTO_OUTERIPFLAGP1_MASK)
        {
            p_ip4_pkt->proto = 
                p_dpib->field_val[ROHC_OUTERIP4_PROTO_FID-ROHC_VAR_FID_BASE];
        }
        if(p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_I_OUTERIPFLAGP1_MASK)
        {
            p_ip4_pkt->id = 
                p_dpib->field_val[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE];
        }
        p_ip4_pkt->off |= 
                    (p_dpib->field_val[ROHC_OUTERIP_DF_FID-ROHC_VAR_FID_BASE] &\
                                         ROHC_IP4_DF);
      }
    }
}

/************************************************************************
 * Name     :   rohc_update_p6_outerip_from_ext3
 * Desc     :   It Updates outer IP Flags from ext3 for non updating pkts
 * Params   :   Pointer to IP Packet
 *        		Pointer to Decompressor Stream context
 * Returns  :   void
***********************************************************************/
#ifdef ROHC_FUNC_ENHANCEMENT  
rohc_void_t rohc_update_p6_outerip_from_ext3(
    rohc_U8bit_t      * p_pkt ,
    rohc_dsc_tcp_t    * p_dsc)
{
    rohc_ip4_t  *p_ip4_pkt;
    rohc_ip6_t  *p_ip6_pkt;
    rohc_dpib_t       * p_dpib = p_dsc->dpib;
    /* Either of outer ip4 or ip4 would be present OR  */
    /* none will be present */
    /* outer ip present */
    if((p_dpib->field_val[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE] &
        ROHC_ENC_INNERIPFLAG_EXT3FLAGP1_MASK) &&
        (p_dpib->field_val[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
        ROHC_ENC_IP2_INNERIPFLAGP1_MASK ) )
    {   
        /* ip-6*/
      if(p_dsc->outer_ip6 != ROHC_NULL)
      {
        p_ip6_pkt = (rohc_ip6_t *)p_pkt;
        if(p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_TOS_OUTERIPFLAGP1_MASK)
        {
            p_ip6_pkt->ip6_ctlun.ip6_un1.un1_flow |= 
                (p_dpib->field_val[ROHC_OUTERIP6_TCLASS_FID-ROHC_VAR_FID_BASE] & \
                    ROHC_IP6_TRAFFICCLASS_MASK);
        } 
        if(p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_TTL_OUTERIPFLAGP1_MASK)
        {
            p_ip6_pkt->ip6_ctlun.ip6_un1.un1_hlim = 
                p_dpib->field_val[ROHC_OUTERIP6_HLIM_FID-ROHC_VAR_FID_BASE];
        }
        if(p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_PROTO_OUTERIPFLAGP1_MASK)
        {
            p_ip6_pkt->ip6_ctlun.ip6_un1.un1_nxt = 
                p_dpib->field_val[ROHC_OUTERIP6_NHEADER_FID-ROHC_VAR_FID_BASE];
        }
      }
      else if(p_dsc->outer_ip4 != ROHC_NULL)
      {
        /*ip-4 */
        p_ip4_pkt = (rohc_ip4_t *)p_pkt;
        if(p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_TOS_OUTERIPFLAGP1_MASK)
        {
            p_ip4_pkt->tos |= 
                (p_dpib->field_val[ROHC_OUTERIP4_TOS_FID-ROHC_VAR_FID_BASE]);
        } 
        
        if(p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_TTL_OUTERIPFLAGP1_MASK)
        {
            p_ip4_pkt->ttl = 
                p_dpib->field_val[ROHC_OUTERIP4_TTL_FID-ROHC_VAR_FID_BASE];
        }
        if(p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_PROTO_OUTERIPFLAGP1_MASK)
        {
            p_ip4_pkt->proto = 
                p_dpib->field_val[ROHC_OUTERIP4_PROTO_FID-ROHC_VAR_FID_BASE];
        }
        if(p_dpib->field_val[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] &
                ROHC_ENC_I_OUTERIPFLAGP1_MASK)
        {
            p_ip4_pkt->id = 
                p_dpib->field_val[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE];
        }
        p_ip4_pkt->off |= 
                    (p_dpib->field_val[ROHC_OUTERIP_DF_FID-ROHC_VAR_FID_BASE] &\
                                         ROHC_IP4_DF);
      }
    }
}
#endif

/************************************************************************
 * Name     :   rohc_update_p1_from_ext3
 * Desc     :   It Updates RTP Flags from ext3 for non updating pkts
 * Params   :   Pointer to IP Packet
 *          :   Pointer to dpib
 * Returns  :   void
***********************************************************************/
rohc_void_t rohc_update_p1_from_ext3
    ROHC_PARAMS((p_pkt,p_dpib),
    rohc_U8bit_t      * p_pkt _AND_
    rohc_dpib_t   * p_dpib)
{
    /* update for RTP  */
    rohc_rtp_t *p_rtp_pkt = (rohc_rtp_t *)p_pkt;
    
    if(p_dpib->field_val[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE] &
                    ROHC_ENC_RTPFLAG_EXT3FLAGP1_MASK )
    {
        if(p_dpib->field_val[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE]
            & ROHC_ENC_R_PT_RTPFLAGP1_MASK)
        {
            if(p_dpib->field_val[ROHC_RTP_R_P_FID-ROHC_VAR_FID_BASE])
                p_rtp_pkt->v_p_x_cc |= ROHC_PAD_MASK;
            p_rtp_pkt->m_pt |= (ROHC_PT_MASK &
                p_dpib->field_val[ROHC_RTP_PT_FID-ROHC_VAR_FID_BASE]);  
        }   
        if(p_dpib->field_val[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE]
            & ROHC_ENC_R_X_RTPFLAGP1_MASK)
        {
            p_rtp_pkt->v_p_x_cc |=  ROHC_X_MASK; 
        }
        if(p_dpib->field_val[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE]
            & ROHC_ENC_M_RTPFLAGP1_MASK)
        {
            p_rtp_pkt->m_pt |= ROHC_M_MASK;
        } 
    }               
}
/************************************************************************
 * Name     :   rohc_cal_ip_checksum
 * Desc     :   It calculates the IP header checksum
 * Params   :   Pointer to IP Packet
 *          :   Size of buffer
 * Returns  :   rohc_U16bit_t
***********************************************************************/
rohc_U16bit_t rohc_cal_ip_checksum ROHC_PARAMS((a_buffer,a_size),
                                            rohc_U8bit_t  *a_buffer _AND_ 
                                            rohc_U32bit_t a_size)
{
static rohc_U32bit_t cksum;
rohc_U16bit_t * buffer = (rohc_U16bit_t *)a_buffer;
cksum=0L;
/* Sum up all the words together.*/
while (a_size>1){
cksum += *buffer++;
a_size -= sizeof(rohc_U16bit_t);
}
/* Adds the final byte if the initial size is odd */
if (a_size)
cksum += *(rohc_U8bit_t*)buffer;
cksum = (cksum >> 16) + (cksum & 0xFFFF);
cksum += (cksum >> 16);
/* Return the bitwise complement of the resulting mishmash*/
return (rohc_U16bit_t )(~cksum);
}

/*****************************************************************
**
**    FUNCTION NAME
**        rohc_d_make_p1_ip_pkt
**
*******************************************************************
**
**    DESCRIPTION
**         This function is the entry function for 
**         DECOMPRESSOR PACKET SENTINEL MODULE
**         This function picks up required fields for regenerating
**         uncompressed IP datagram from DSC.
**
**    INPUTS
**        p_pkt - Pointer to memory location where IP datagram has to be reasssembled 
**        p_len	- Length of regenrated IP datagram (OUTPUT PARAM)
**        p_dsc - Pointer to Decompressor Stream context
**        p_entity - Pointer to ROHC entity context    
**        p_dprm_update_blk - Pointer to ROHC global context    
**        p_ecode - Pointer to error code     
**    RETURNS
**    -    
**
******************************************************************/
/* +- SPR 17777 */
rohc_return_t rohc_d_make_p1_ip_pkt
    ROHC_PARAMS((p_pkt,p_len,p_dsc,p_dprm_update_blk),
    rohc_U8bit_t            * p_pkt             _AND_
    rohc_U32bit_t           * p_len             _AND_
    struct rohc_dsc_rtp_t   * p_dsc             _AND_
    rohc_dprm_update_blk_t  * p_dprm_update_blk)
{
    /* + SPR 17439 */
    rohc_U8bit_t len=0;
    /* - SPR 17439 */
    rohc_U8bit_t *p_temp,*p_ext_hdr = NULL,*p_ext_hdr2 = NULL,*p_in_ip4 = NULL,
            *p_out_ip4 = NULL,*p_in_ip6 = NULL,*p_out_ip6 = NULL,*p_org=p_pkt;
    rohc_U8bit_t flag = 1;

    register rohc_U8bit_t rcv_pkt_typ;
    rcv_pkt_typ = p_dsc->dpib->rcv_pkt_typ;
 
    ROHC_ENTER_FN("rohc_d_make_p1_ip_pkt");
    
    /* Outer IP 4 Datagram*/
    if(p_dsc->outer_ip4)
    {
        p_dprm_update_blk->outer_ip4 = (rohc_ip4_t*)p_pkt;
        p_out_ip4 = p_pkt;
        ROHC_MEMCPY(p_pkt,ROHC_DSC_GET_OUTER_IP4(p_dsc),
                sizeof(struct rohc_ip4_t));

        /* + Coverity 73075 */
        rohc_dpbm_make_outer_ip4(&p_pkt, p_dsc);
        /* +- SPR 17777 */
        /* - Coverity 73075 */
    }/* end if outer IP4*/
    else if(p_dsc->outer_ip6)
    {
        /* Outer IP 6 Datagram*/
        p_dprm_update_blk->outer_ip6 = (rohc_ip6_t*)p_pkt;
        p_out_ip6 = p_pkt;
        ROHC_MEMCPY(p_pkt,ROHC_DSC_GET_OUTER_IP6(p_dsc),
                sizeof(struct rohc_ip6_t));
        /* + Coverity 73140 */
        rohc_dpbm_make_outer_ip6(&p_pkt, p_dsc);
        /* +- SPR 17777 */
        /* - Coverity 73140 */
    }/*end if outer IP 6 */
    rohc_d_p1_outer_ip_ext_list_cons(p_dsc,&p_ext_hdr,&p_ext_hdr2,p_pkt,
            p_out_ip4,p_out_ip6,&len);
    p_pkt += len;
    
    /* inner IP 4 Datagram*/
    flag = 1;
    if(p_dsc->inner_ip4)
    {
        if(p_ext_hdr)
        {
            *p_ext_hdr = *p_ext_hdr2 = ROHC_IPPROTO_IP4;
            p_ext_hdr = NULL;
        }
        p_dprm_update_blk->inner_ip4 = (rohc_ip4_t*)p_pkt;
        p_in_ip4 = p_pkt;
        ROHC_MEMCPY(p_pkt,ROHC_DSC_GET_INNER_IP4(p_dsc),
                            sizeof(struct rohc_ip4_t));
         /* + Coverity 72855 */
        rohc_dpbm_make_inner_ip4(&p_pkt, p_dsc);
        /* +- SPR 17777 */
         /* - Coverity 72855 */
    } /* end if inner ip4 */
    else if(p_dsc->inner_ip6)
    {
    /* inner IP 6 Datagram*/
        if(p_ext_hdr)
        {
            *p_ext_hdr = *p_ext_hdr2 = ROHC_IPPROTO_IP6;
            p_ext_hdr = NULL;
        }
        p_dprm_update_blk->inner_ip6 = (rohc_ip6_t*)p_pkt;
        p_in_ip6 = p_pkt;
        ROHC_MEMCPY(p_pkt,ROHC_DSC_GET_INNER_IP6(p_dsc),sizeof(struct rohc_ip6_t));
       
       /* + Coverity 72812 */
        rohc_dpbm_make_inner_ip6(&p_pkt, p_dsc);
        /* +- SPR 17777 */
       /* - Coverity 72812 */
    } /* end if inner ip6 */
    rohc_d_p1_inner_ip_ext_list_cons(p_dsc,&p_ext_hdr,&p_ext_hdr2,p_pkt,
            p_in_ip4,p_in_ip6,&len);
    p_pkt += len;

    {
        if(p_ext_hdr)
        {
            *p_ext_hdr = *p_ext_hdr2 = ROHC_IPPROTO_UDP;
        }
        /*Cov 72981 + -*/
        p_dprm_update_blk->udp = (rohc_udp_t*)p_pkt;
        ROHC_MEMCPY(p_pkt,ROHC_DSC_GET_UDP(p_dsc),
                sizeof(struct rohc_udp_t));
        p_temp = p_pkt;
        len =sizeof(struct rohc_udp_t);
        p_pkt+=len;
        
        len=sizeof(struct rohc_rtp_t);
        p_dprm_update_blk->rtp = (rohc_rtp_t*)p_pkt;
        ROHC_MEMCPY(p_pkt,ROHC_DSC_GET_RTP(p_dsc),len);

        flag = rohc_dpbm_make_udp_rtp(&p_pkt, p_dsc, rcv_pkt_typ);

        ((rohc_rtp_t*)p_pkt)->sn = ROHC_HTONS(((rohc_rtp_t*)p_pkt)->sn);
        ((rohc_rtp_t*)p_pkt)->ts = ROHC_HTONL(((rohc_rtp_t*)p_pkt)->ts);
        p_pkt += len;

        if(flag)
        {
            len = ((((struct rohc_rtp_t *)ROHC_DSC_GET_RTP(p_dsc))->v_p_x_cc&
                                ROHC_CC_MASK)*4);
            /* SPR 4808 Fix Start */
            ROHC_MEMCPY(p_pkt,ROHC_DSC_GET_RTP(p_dsc)+ 1,len);
            /* SPR 4808 Fix End */
            p_pkt += len;
        }
        else
        {
            len = p_dsc->dpib->csrc_new_item_count * 4;
            ROHC_MEMCPY(p_pkt,p_dsc->dpib->final_csrc_list,len);
            p_pkt += len;
        }
        ((rohc_udp_t*)p_temp)->len = ROHC_HTONS((rohc_U16bit_t)
                    ((p_pkt - p_temp) + p_dsc->dpib->payload_len));
    }/* end if rtp profile*/
    rohc_d_p1_update_ip_len_cksum(p_dsc,p_in_ip4,p_in_ip6,p_out_ip4,
            p_out_ip6,p_pkt);

    /* 
    ** START_SPR_569_FIX : Store received pkt type into previous pkt type.
    */
   p_dsc->pre_pkt_type = rcv_pkt_typ;
   /* END_SPR_569_FIX */
    *p_len=(p_pkt - p_org);
    ROHC_EXIT_FN("rohc_d_make_p1_ip_pkt");
    return ROHC_SUCCESS;    
}/* end function rohc_d_make_p1_ip_pkt */

/*****************************************************************
**
**    FUNCTION NAME
**        rohc_d_p1_outer_ip_ext_list_cons
**
*******************************************************************
**
**    DESCRIPTION
**         This function is the entry function for 
**         DECOMPRESSOR PACKET SENTINEL MODULE
**
**    INPUTS
**        p_dsc - Pointer to Decompressor Stream context
**        p_ext_hdr - Pointer to Rohc Header
**		  p_ext_hdr2 - Pointer to Rohc Header
**		  p_pkt - Pointer to memory location where IP datagram has to be reasssembled 
**		  p_out_ip4 - 
**		  p_out_ip6 -	
**        p_len - Length of regenrated IP datagram (OUTPUT PARAM)
**        profile
**    RETURNS
**    -    
**
******************************************************************/
    /* + SPR 17439 */
STATIC rohc_void_t rohc_d_p1_outer_ip_ext_list_cons
    ROHC_PARAMS((p_dsc,p_ext_hdr,p_ext_hdr2,p_pkt,p_out_ip4,p_out_ip6,p_len
            ),
        struct rohc_dsc_rtp_t   *p_dsc             _AND_
        rohc_U8bit_t            **p_ext_hdr         _AND_
        rohc_U8bit_t            **p_ext_hdr2        _AND_
        rohc_U8bit_t            * p_pkt             _AND_
        rohc_U8bit_t            *p_out_ip4          _AND_
        rohc_U8bit_t            *p_out_ip6          _AND_
        rohc_U8bit_t            *p_len)
    /* - SPR 17439 */
{
    *p_len = 0;
    rohc_list_node_t * node=NULL;
    /*Outer IP extension list construction */
    node=rohc_db_get_first_node(&(p_dsc->dpib->outeripextlist));
    if(node)
    {
        if(p_dsc->outer_ip4)
        {
            ((rohc_ip4_t*)p_out_ip4)->proto = 
                ((rohc_protocol_node_t*)node)->protocol;
        }
        else if(p_dsc->outer_ip6) /* Coverity 10109 Fix */
        {
            ((rohc_ip6_t*)p_out_ip6)->ip6_ctlun.ip6_un1.un1_nxt 
                = ((rohc_protocol_node_t*)node)->protocol;
        }
    }
    while(node)
    {
        if((((rohc_protocol_node_t*)node)->protocol !=ROHC_IPPROTO_GRE) &&
                (((rohc_protocol_node_t*)node)->protocol != ROHC_IPPROTO_ESP))
        {
            *p_ext_hdr = p_pkt;
            *p_ext_hdr2 = (rohc_U8bit_t*)((rohc_protocol_node_t *)node)->ptr;
        }
        else
            *p_ext_hdr = NULL;
        ROHC_MEMCPY(p_pkt,((struct rohc_protocol_node_t *)node)->ptr,
                ((struct rohc_protocol_node_t *)node)->length);
        *p_len =((struct rohc_protocol_node_t *)node)->length;
        p_pkt += *p_len; //see
        node=rohc_db_get_next_node(node);
    }/* end while outer IP extension list traversal*/
}

/*****************************************************************
**
**    FUNCTION NAME
**        rohc_d_p1_inner_ip_ext_list_cons
**
*******************************************************************
**
**    DESCRIPTION
**         This function is the entry function for 
**         DECOMPRESSOR PACKET SENTINEL MODULE
**
**    INPUTS
**        p_dsc - Pointer to Decompressor Stream context
**        p_ext_hdr - Pointer to Rohc Header
**		  p_ext_hdr2 - Pointer to Rohc Header
**		  p_pkt - Pointer to memory location where IP datagram has to be reasssembled 
**		  p_out_ip4 - 
**		  p_out_ip6 -	
**        p_len - Length of regenrated IP datagram (OUTPUT PARAM)
**    RETURNS
**    -    
**
******************************************************************/
STATIC rohc_void_t rohc_d_p1_inner_ip_ext_list_cons
    ROHC_PARAMS((p_dsc,p_ext_hdr,p_ext_hdr2,p_pkt,p_in_ip4,p_in_ip6,p_len),
    struct rohc_dsc_rtp_t   * p_dsc             _AND_
    rohc_U8bit_t            **p_ext_hdr         _AND_
    rohc_U8bit_t            **p_ext_hdr2        _AND_
    rohc_U8bit_t            * p_pkt             _AND_
    rohc_U8bit_t            *p_in_ip4          _AND_
    rohc_U8bit_t            *p_in_ip6          _AND_
    rohc_U8bit_t            *p_len  )
{
    rohc_list_node_t * node=NULL;
    *p_len = 0;
    /* inner IP extension list construction */
    node=rohc_db_get_first_node(&(p_dsc->dpib->inneripextlist));
    if(node)
    {
        if(p_dsc->inner_ip4)
        {
            ((rohc_ip4_t*)p_in_ip4)->proto = 
                ((rohc_protocol_node_t *)node)->protocol;
        }
        else if(p_dsc->inner_ip6)/* Start Coverity 10110 Fix*/
        {
            if(NULL != p_in_ip6)
            {
                ((rohc_ip6_t*)p_in_ip6)->ip6_ctlun.ip6_un1.un1_nxt = 
                    ((rohc_protocol_node_t *)node)->protocol;
            }
            /* End Coverity 10110 Fix*/
        }
    }
    while(node)
    {
        if((((rohc_protocol_node_t*)node)->protocol !=ROHC_IPPROTO_GRE) &&
            (((rohc_protocol_node_t*)node)->protocol != ROHC_IPPROTO_ESP))
        {
            *p_ext_hdr = p_pkt;
            *p_ext_hdr2 = (rohc_U8bit_t*)((rohc_protocol_node_t *)node)->ptr;
        }
        else
            *p_ext_hdr = NULL;
        ROHC_MEMCPY(p_pkt,((struct rohc_protocol_node_t *)node)->ptr,
                ((struct rohc_protocol_node_t *)node)->length);
        *p_len =((struct rohc_protocol_node_t *)node)->length;
        p_pkt += *p_len;
        node=rohc_db_get_next_node(node);
    }/* end while inner IP extension list traversal*/
}

/*****************************************************************
**
**    FUNCTION NAME
**        rohc_d_p1_update_ip_len_cksum
**
*******************************************************************
**
**    DESCRIPTION
**         This function is the entry function for 
**         DECOMPRESSOR PACKET SENTINEL MODULE
**
**    INPUTS
**        p_dsc - Pointer to Decompressor Stream context
**    	  p_in_ip4 - pointer to inner IPv4 header
**    	  p_in_ip6 - pointer to inner IPv6 header
**		  p_out_ip4 - pointer to outer IPv4 header
**		  p_out_ip6 - pointer to outer IPv6 header
**        p_pkt - Pointer to memory location where IP datagram has to be reasssembled 
**
**	  RETURNS
**    -    
**
******************************************************************/
STATIC rohc_void_t rohc_d_p1_update_ip_len_cksum
    ROHC_PARAMS((p_dsc,p_in_ip4,p_in_ip6,p_out_ip4,p_out_ip6,p_pkt),
    struct rohc_dsc_rtp_t   * p_dsc             _AND_
    rohc_U8bit_t            *p_in_ip4          _AND_
    rohc_U8bit_t            *p_in_ip6          _AND_
    rohc_U8bit_t            *p_out_ip4          _AND_
    rohc_U8bit_t            *p_out_ip6          _AND_
    rohc_U8bit_t            * p_pkt )
{

    if(p_dsc->inner_ip4)
    {
        ((rohc_ip4_t*)p_in_ip4)->len = ROHC_HTONS((rohc_U16bit_t)
                        ((p_pkt - p_in_ip4) + p_dsc->dpib->payload_len));
        ((rohc_ip4_t*)p_in_ip4)->cksum = 0;
        ((rohc_ip4_t*)p_in_ip4)->cksum = 
                (rohc_cal_ip_checksum(p_in_ip4,
                    ROHC_IP4_HL((rohc_ip4_t*)p_in_ip4)*4));
    }
    else if(p_dsc->inner_ip6)
    {
        /* SPR 4808 Fix Start */
        if(NULL != p_in_ip6)
        {
            ((rohc_ip6_t*)p_in_ip6)->ip6_ctlun.ip6_un1.un1_plen =
                ROHC_HTONS((rohc_U16bit_t)((p_pkt - p_in_ip6) - sizeof(rohc_ip6_t) +
                                p_dsc->dpib->payload_len));
        /* SPR 4808 Fix End */
        }
    }   
    
    if(p_dsc->outer_ip4)
    {
        ((rohc_ip4_t*)p_out_ip4)->len = ROHC_HTONS((rohc_U16bit_t)
                            ((p_pkt - p_out_ip4) + p_dsc->dpib->payload_len));
        ((rohc_ip4_t*)p_out_ip4)->cksum = 0;
        ((rohc_ip4_t*)p_out_ip4)->cksum = 
                (rohc_cal_ip_checksum(p_out_ip4,
                    ROHC_IP4_HL((rohc_ip4_t*)p_out_ip4)*4));
    }
    else  if(p_dsc->outer_ip6)
    {
            ((rohc_ip6_t*)p_out_ip6)->ip6_ctlun.ip6_un1.un1_plen =
                ROHC_HTONS((rohc_U16bit_t)((p_pkt - p_out_ip6) - sizeof(rohc_ip6_t) +
                            p_dsc->dpib->payload_len));
    }
}

/*****************************************************************
**
**    FUNCTION NAME
**        rohc_d_p2_outer_ip_ext_list_cons
**
*******************************************************************
**
**    DESCRIPTION
**         This function is the entry function for 
**         DECOMPRESSOR PACKET SENTINEL MODULE
**
**    INPUTS
**        p_dsc - Pointer to Decompressor Stream context
**        p_ext_hdr - Pointer to Rohc Header
**		  p_ext_hdr2 - Pointer to Rohc Header
**		  p_pkt - Pointer to memory location where IP datagram has to be reasssembled 
**		  p_out_ip4 - Pointer to outer IPv4 header
**		  p_out_ip6 - Pointer to outer IPv6 header
**        p_len - Length of regenrated IP datagram (OUTPUT PARAM)
**        profile
**    RETURNS
**    -    
**
******************************************************************/
    /* + SPR 17439 */
STATIC rohc_void_t rohc_d_p2_outer_ip_ext_list_cons
    ROHC_PARAMS((p_dsc,p_ext_hdr,p_ext_hdr2,p_pkt,p_out_ip4,p_out_ip6,p_len),
        struct rohc_dsc_udp_t   *p_dsc             _AND_
        rohc_U8bit_t            **p_ext_hdr         _AND_
        rohc_U8bit_t            **p_ext_hdr2        _AND_
        rohc_U8bit_t            * p_pkt             _AND_
        rohc_U8bit_t            *p_out_ip4          _AND_
        rohc_U8bit_t            *p_out_ip6          _AND_
        rohc_U8bit_t            *p_len)
    /* - SPR 17439 */
{
    *p_len = 0;
    rohc_list_node_t * node=NULL;
    /*Outer IP extension list construction */
    node=rohc_db_get_first_node(&(p_dsc->dpib->outeripextlist));
    if(node)
    {
        if(p_dsc->outer_ip4)
        {
            ((rohc_ip4_t*)p_out_ip4)->proto = 
                ((rohc_protocol_node_t*)node)->protocol;
        }
        else if(p_dsc->outer_ip6) /* Coverity 10109 Fix */
        {
            ((rohc_ip6_t*)p_out_ip6)->ip6_ctlun.ip6_un1.un1_nxt 
                = ((rohc_protocol_node_t*)node)->protocol;
        }
    }
    while(node)
    {
        if((((rohc_protocol_node_t*)node)->protocol !=ROHC_IPPROTO_GRE) &&
                (((rohc_protocol_node_t*)node)->protocol != ROHC_IPPROTO_ESP))
        {
            *p_ext_hdr = p_pkt;
            *p_ext_hdr2 = (rohc_U8bit_t*)((rohc_protocol_node_t *)node)->ptr;
        }
        else
            *p_ext_hdr = NULL;
        ROHC_MEMCPY(p_pkt,((struct rohc_protocol_node_t *)node)->ptr,
                ((struct rohc_protocol_node_t *)node)->length);
        *p_len =((struct rohc_protocol_node_t *)node)->length;
        p_pkt += *p_len; //see
        node=rohc_db_get_next_node(node);
    }/* end while outer IP extension list traversal*/
}

/*****************************************************************
**
**    FUNCTION NAME
**        rohc_d_p2_inner_ip_ext_list_cons
**
*******************************************************************
**
**    DESCRIPTION
**         This function is the entry function for 
**         DECOMPRESSOR PACKET SENTINEL MODULE
**
**    INPUTS
**        p_dsc - Pointer to Decompressor Stream context
**        p_ext_hdr - Pointer to Rohc Header
**		  p_ext_hdr2 - Pointer to Rohc Header
**		  p_pkt - Pointer to memory location where IP datagram has to be reasssembled 
**		  p_out_ip4 - Pointer to Outer IPv4 header
**		  p_out_ip6 - Pointer to Outer IPv6 header
**        p_len - Length of regenrated IP datagram (OUTPUT PARAM)
**    RETURNS
**    -    
**
******************************************************************/
STATIC rohc_void_t rohc_d_p2_inner_ip_ext_list_cons
    ROHC_PARAMS((p_dsc,p_ext_hdr,p_ext_hdr2,p_pkt,p_in_ip4,p_in_ip6,p_len),
    struct rohc_dsc_udp_t   * p_dsc             _AND_
    rohc_U8bit_t            **p_ext_hdr         _AND_
    rohc_U8bit_t            **p_ext_hdr2        _AND_
    rohc_U8bit_t            * p_pkt             _AND_
    rohc_U8bit_t            *p_in_ip4          _AND_
    rohc_U8bit_t            *p_in_ip6          _AND_
    rohc_U8bit_t            *p_len  )
{
    rohc_list_node_t * node=NULL;
    *p_len = 0;
    /* inner IP extension list construction */
    node=rohc_db_get_first_node(&(p_dsc->dpib->inneripextlist));
    if(node)
    {
        if(p_dsc->inner_ip4)
        {
            ((rohc_ip4_t*)p_in_ip4)->proto = 
                ((rohc_protocol_node_t *)node)->protocol;
        }
        else if(p_dsc->inner_ip6)/* Start Coverity 10110 Fix*/
        {
            if(NULL != p_in_ip6)
            {
                ((rohc_ip6_t*)p_in_ip6)->ip6_ctlun.ip6_un1.un1_nxt = 
                    ((rohc_protocol_node_t *)node)->protocol;
            }
            /* End Coverity 10110 Fix*/
        }
    }
    while(node)
    {
        if((((rohc_protocol_node_t*)node)->protocol !=ROHC_IPPROTO_GRE) &&
            (((rohc_protocol_node_t*)node)->protocol != ROHC_IPPROTO_ESP))
        {
            *p_ext_hdr = p_pkt;
            *p_ext_hdr2 = (rohc_U8bit_t*)((rohc_protocol_node_t *)node)->ptr;
        }
        else
            *p_ext_hdr = NULL;
        ROHC_MEMCPY(p_pkt,((struct rohc_protocol_node_t *)node)->ptr,
                ((struct rohc_protocol_node_t *)node)->length);
        *p_len =((struct rohc_protocol_node_t *)node)->length;
        p_pkt += *p_len;
        node=rohc_db_get_next_node(node);
    }/* end while inner IP extension list traversal*/
}

/*****************************************************************
**
**    FUNCTION NAME
**        rohc_d_p2_update_ip_len_cksum
**
*******************************************************************
**
**    DESCRIPTION
**         This function is the entry function for 
**         DECOMPRESSOR PACKET SENTINEL MODULE
**
**    INPUTS
**        p_dsc - Pointer to Decompressor Stream context
**    	  p_in_ip4 - Pointer to inner IPv4 header
**    	  p_in_ip6 - Pointer to inner IPv6 header
**		  p_out_ip4 - Pointer to outer IPv4 header
**		  p_out_ip6 - Pointer to outer IPv6 header
**        p_pkt - Pointer to memory location where IP datagram has to be reasssembled 
**
**    RETURNS
**    -    
**
******************************************************************/
STATIC rohc_void_t rohc_d_p2_update_ip_len_cksum
    ROHC_PARAMS((p_dsc,p_in_ip4,p_in_ip6,p_out_ip4,p_out_ip6,p_pkt),
    struct rohc_dsc_udp_t   * p_dsc             _AND_
    rohc_U8bit_t            *p_in_ip4          _AND_
    rohc_U8bit_t            *p_in_ip6          _AND_
    rohc_U8bit_t            *p_out_ip4          _AND_
    rohc_U8bit_t            *p_out_ip6          _AND_
    rohc_U8bit_t            * p_pkt )
{

    if(p_dsc->inner_ip4)
    {
        ((rohc_ip4_t*)p_in_ip4)->len = ROHC_HTONS((rohc_U16bit_t)
                        ((p_pkt - p_in_ip4) + p_dsc->dpib->payload_len));
        ((rohc_ip4_t*)p_in_ip4)->cksum = 0;
        ((rohc_ip4_t*)p_in_ip4)->cksum = 
                (rohc_cal_ip_checksum(p_in_ip4,
                    ROHC_IP4_HL((rohc_ip4_t*)p_in_ip4)*4));
    }
    else if(p_dsc->inner_ip6)
    {
        /* SPR 4808 Fix Start */
        if(NULL != p_in_ip6)
        {
            ((rohc_ip6_t*)p_in_ip6)->ip6_ctlun.ip6_un1.un1_plen =
                ROHC_HTONS((rohc_U16bit_t)((p_pkt - p_in_ip6) - sizeof(rohc_ip6_t) +
                                p_dsc->dpib->payload_len));
        /* SPR 4808 Fix End */
        }
    }   
    
    if(p_dsc->outer_ip4)
    {
        ((rohc_ip4_t*)p_out_ip4)->len = ROHC_HTONS((rohc_U16bit_t)
                            ((p_pkt - p_out_ip4) + p_dsc->dpib->payload_len));
        ((rohc_ip4_t*)p_out_ip4)->cksum = 0;
        ((rohc_ip4_t*)p_out_ip4)->cksum = 
                (rohc_cal_ip_checksum(p_out_ip4,
                    ROHC_IP4_HL((rohc_ip4_t*)p_out_ip4)*4));
    }
    else  if(p_dsc->outer_ip6)
    {
            ((rohc_ip6_t*)p_out_ip6)->ip6_ctlun.ip6_un1.un1_plen =
                ROHC_HTONS((rohc_U16bit_t)((p_pkt - p_out_ip6) - sizeof(rohc_ip6_t) +
                            p_dsc->dpib->payload_len));
    }
}

/*****************************************************************
**
**    FUNCTION NAME
**        rohc_dpbm_make_udp_rtp
**
*******************************************************************
**
**    DESCRIPTION
**        This function picks up required fields for regenerating
**        uncompressed IP datagram from DSC for inner IP v4 header.
**
**    INPUTS
**        p_rohc_pkt - Pointer to memory location where IP datagram has to be reasssembled 
**        p_dsc - Pointer to Decompressor Stream context
**        rcv_pkt_typ - Received packet type
**    RETURNS
**    	  Flag    
**
******************************************************************/
static rohc_U8bit_t rohc_dpbm_make_udp_rtp 
    ROHC_PARAMS((p_rohc_pkt,p_dsc,rcv_pkt_typ),
    rohc_U8bit_t            ** p_rohc_pkt             _AND_
    struct rohc_dsc_rtp_t   * p_dsc             _AND_
    rohc_U8bit_t              rcv_pkt_typ
    )
{
    rohc_U8bit_t flag = 1;
    rohc_U8bit_t  **p_pkt=p_rohc_pkt;

    switch(rcv_pkt_typ)
    {
        case ROHC_R0P1_PKTTYPE :
            ((rohc_udp_t*)(*p_pkt-8))->cksum=
                ROHC_HTONS((rohc_U16bit_t)p_dsc->dpib->field_val
                        [ROHC_UDP_CKSUM_FID - ROHC_VAR_FID_BASE]);
            ((rohc_rtp_t*)*p_pkt)->sn = p_dsc->dpib->decoded_sn;
            /* 
             ** START_SPR_569_FIX : Introduced the handling for
             ** ROHC_R0P1_PKTTYPE type packets in profile-1 (RTP).
             */
            if( p_dsc->pre_pkt_type != ROHC_IRDYN_PKTTYPE )
            {
                ((rohc_rtp_t*)*p_pkt)->ts = ROHC_HTONL(((rohc_rtp_t*)*p_pkt)->ts); 
            }
            else
            {
                p_dsc->rtp->ts = ROHC_HTONL(p_dsc->rtp->ts);
            }
            /* END_SPR_569_FIX */
            break;
        case ROHC_R1P1_PKTTYPE :
        case ROHC_R1E0P1_PKTTYPE:
        case ROHC_R1E1P1_PKTTYPE:
        case ROHC_R1E2P1_PKTTYPE:
        case ROHC_R1IDP1_PKTTYPE :
        case ROHC_R1IDE0P1_PKTTYPE:
        case ROHC_R1IDE1P1_PKTTYPE:
        case ROHC_R1IDE2P1_PKTTYPE:
        case ROHC_R1TSP1_PKTTYPE :
        case ROHC_R1TSE0P1_PKTTYPE:
        case ROHC_R1TSE1P1_PKTTYPE:
        case ROHC_R1TSE2P1_PKTTYPE:
            ((rohc_rtp_t*)*p_pkt)->sn = p_dsc->dpib->decoded_sn;
            ((rohc_rtp_t*)*p_pkt)->ts = p_dsc->dpib->decoded_ts;
            /* SPR 4808 Fix Start */
            break;
            /* SPR 4808 Fix End */
        case ROHC_UO1P1_PKTTYPE:
        case ROHC_UO1TSP1_PKTTYPE:
            ((rohc_udp_t*)(*p_pkt-8))->cksum=
                ROHC_HTONS((rohc_U16bit_t)p_dsc->dpib->field_val
                        [ROHC_UDP_CKSUM_FID - ROHC_VAR_FID_BASE]);
            if(p_dsc->dpib->field_val[ROHC_RTP_M_FID - 
                    ROHC_VAR_FID_BASE])
                ((rohc_rtp_t*)*p_pkt)->m_pt |= ROHC_M_MASK;
            break;
        case ROHC_R1E3P1_PKTTYPE:
        case ROHC_R1IDE3P1_PKTTYPE:
        case ROHC_R1TSE3P1_PKTTYPE:
            ((rohc_rtp_t*)*p_pkt)->sn = p_dsc->dpib->decoded_sn;
            ((rohc_rtp_t*)*p_pkt)->ts = p_dsc->dpib->decoded_ts;
            if(p_dsc->dpib->field_val[ROHC_RTP_M_FID-ROHC_VAR_FID_BASE])
                ((rohc_rtp_t*)*p_pkt)->m_pt |= ROHC_M_MASK;
            /* SPR 4808 Fix Start */
            break;
            /* SPR 4808 Fix End */
        case ROHC_UO1IDE3P1_PKTTYPE:
            ((rohc_udp_t*)(*p_pkt-8))->cksum=
                ROHC_HTONS((rohc_U16bit_t)p_dsc->dpib->field_val
                        [ROHC_UDP_CKSUM_FID - ROHC_VAR_FID_BASE]);
            rohc_update_p1_from_ext3(*p_pkt,p_dsc->dpib);
            flag = 0;
            break;
    }
    return flag;
}
/*****************************************************************
**
**    FUNCTION NAME
**        rohc_dpbm_make_inner_ip4
**
*******************************************************************
**
**    DESCRIPTION
**         This function picks up required fields for regenerating
**         uncompressed IP datagram from DSC for inner IP v4 header.
**
**    INPUTS
**		  p_rohc_pkt - Pointer to Rohc packet
**        p_dsc -Pointer to Decompressor Stream context
**		  p_entity - Pointer to ROHC Entity
**		  p_in_ip4 - Pointer to inner IPv4 header
**        p_ecode - Pointer to error code     
**
**    RETURNS
**    -    
**
******************************************************************/
/* +- SPR 17777 */
static rohc_U8bit_t rohc_dpbm_make_inner_ip4
    ROHC_PARAMS((p_rohc_pkt,p_dsc),
    rohc_U8bit_t            ** p_rohc_pkt             _AND_
    struct rohc_dsc_rtp_t   * p_dsc )
{
    rohc_U32bit_t len=0;
    rohc_U8bit_t rcv_pkt_typ=0;
    rohc_U8bit_t flag = 1;
    rohc_U8bit_t  **p_pkt=p_rohc_pkt;

    rcv_pkt_typ = p_dsc->dpib->rcv_pkt_typ;
    switch(rcv_pkt_typ)
    {
        /* 
         ** START_SPR_1197_FIX : Introduced the  handling of 
         ** ROHC_R1P1_PKTTYPE type packets in profile-1 (RTP) for 
         ** inner_ip4.
         */
        case ROHC_R1P1_PKTTYPE :
            /* END_SPR_1197_FIX */
        case ROHC_R0P1_PKTTYPE :

            /* 
             ** START_SPR_569_FIX : Introduced the  handling of
             ** ROHC_R0P1_PKTTYPE type packets in profile-1 (RTP)
             ** for inner_ip4.
             */
            if( p_dsc->pre_pkt_type != ROHC_IRDYN_PKTTYPE )
            {
                ((rohc_ip4_t*)*p_pkt)->id = ROHC_HTONS(((rohc_ip4_t*)*p_pkt)->id);
            }
            else
            {
                p_dsc->inner_ip4->id = ROHC_HTONS(p_dsc->inner_ip4->id);
            }
            /* END_SPR_569_FIX */
        case ROHC_R1IDP1_PKTTYPE :
        case ROHC_R1IDE0P1_PKTTYPE:
        case ROHC_R1IDE1P1_PKTTYPE:
        case ROHC_R1IDE2P1_PKTTYPE:
        case ROHC_R1TSP1_PKTTYPE :
        case ROHC_R1TSE0P1_PKTTYPE:
        case ROHC_R1TSE1P1_PKTTYPE:
        case ROHC_R1TSE2P1_PKTTYPE:
            if( p_dsc->rnd != ROHC_RND_PRESENT)
                ((rohc_ip4_t*)*p_pkt)->id = p_dsc->dpib->decoded_ipid;
            break;
        case ROHC_R1IDE3P1_PKTTYPE:
        case ROHC_R1TSE3P1_PKTTYPE:
            if(p_dsc->rnd != ROHC_RND_PRESENT)
                ((rohc_ip4_t*)*p_pkt)->id = p_dsc->dpib->decoded_ipid;
            /* COVERITY 10129 */
            break;
            /* COVERITY 10129 */
        case ROHC_UO1IDE3P1_PKTTYPE:
            rohc_update_p1_innerip_from_ext3(*p_pkt,p_dsc);
            flag = 0;
            break;

    }
    ((rohc_ip4_t*)*p_pkt)->id = ROHC_HTONS(((rohc_ip4_t*)*p_pkt)->id);
    /* Fix for RND Bits */ 
    if( ( ROHC_RND_ABSENT  == p_dsc->rnd )  && (p_dsc->nbo == ROHC_FALSE))
    {
        ((rohc_ip4_t*)*p_pkt)->id = 
            (((rohc_ip4_t*)*p_pkt)->id >> 8) |
            ((((rohc_ip4_t*)*p_pkt)->id << 8) & 0xff00);
    }
    len =sizeof(struct rohc_ip4_t);
    *p_pkt+=len;

    return flag;
}


/*****************************************************************
**
**    FUNCTION NAME
**        rohc_dpbm_make_inner_ip6
**
*******************************************************************
**
**    DESCRIPTION
**         This function picks up required fields for regenerating
**         uncompressed IP datagram from DSC for inner IP v6 header.
**
**    INPUTS
**		  p_rohc_pkt - Pointer to Rohc packet
**        p_dsc -Pointer to Decompressor Stream context
**		  p_entity - Pointer to ROHC Entity
**		  p_in_ip6 - Pointer to inner IPv6 header
**        p_ecode - Pointer to error code     
**    RETURNS
**    -    
**
******************************************************************/
/* +- SPR 17777 */
static rohc_U8bit_t rohc_dpbm_make_inner_ip6
    ROHC_PARAMS((p_rohc_pkt,p_dsc),
    rohc_U8bit_t            ** p_rohc_pkt       _AND_
    struct rohc_dsc_rtp_t   * p_dsc)
{
    rohc_U32bit_t len=0;
    rohc_U8bit_t rcv_pkt_typ=0;
    rohc_U8bit_t flag = 1;
    rohc_U8bit_t  **p_pkt=p_rohc_pkt;

    rcv_pkt_typ = p_dsc->dpib->rcv_pkt_typ;
    /* 
     ** START_SPR_569_FIX : Introduced the  handling of ROHC_R0P1_PKTTYPE
     ** type packets in profile-1 (RTP) for inner_ip6. This part has not 
     ** been tested as yet and shall be tested in future releases.
     */
    if (rcv_pkt_typ == ROHC_R0P1_PKTTYPE )
    {
        if( p_dsc->pre_pkt_type != ROHC_IRDYN_PKTTYPE )
        {
            ((rohc_ip4_t*)*p_pkt)->id = ROHC_HTONS(((rohc_ip4_t*)*p_pkt)->id);
        }
        else
        {
            /* SPR 3444 chnages starts */
            if ((p_dsc->inner_ip4) && (p_dsc->inner_ip4->id))
            {
                p_dsc->inner_ip4->id = ROHC_HTONS(p_dsc->inner_ip4->id);
            }
            /* SPR 3444 changes ends */
        }
    }
    /* END_SPR_569_FIX */
    if( (rcv_pkt_typ == ROHC_R1IDE3P1_PKTTYPE ) ||
            (rcv_pkt_typ == ROHC_R1TSE3P1_PKTTYPE ) ||
            (rcv_pkt_typ == ROHC_UO1IDE3P1_PKTTYPE ) )             
    {
        rohc_update_p1_innerip_from_ext3(*p_pkt,p_dsc);
        flag = 0;
    }
    len=sizeof(struct rohc_ip6_t);
    *p_pkt+=len;

    return flag;
}

/*****************************************************************
**
**    FUNCTION NAME
**        rohc_dpbm_make_outer_ip4
**
*******************************************************************
**
**    DESCRIPTION
**         This function picks up required fields for regenerating
**         uncompressed IP datagram from DSC for outer IP v4 header.
**
**    INPUTS
**		  p_rohc_pkt - Pointer to Rohc packet
**        p_dsc -Pointer to Decompressor Stream context
**		  p_entity - Pointer to ROHC Entity
**		  p_out_ip4 - Pointer to outer IPv4 header
**        p_ecode - Pointer to error code     
**    RETURNS
**    -    
**
******************************************************************/
/* +- SPR 17777 */
static rohc_U8bit_t rohc_dpbm_make_outer_ip4
    /* + SPR 17439 */
    ROHC_PARAMS((p_rohc_pkt,p_dsc),
    /* - SPR 17439 */
    rohc_U8bit_t            ** p_rohc_pkt             _AND_
    struct rohc_dsc_rtp_t   * p_dsc)
{
    rohc_U32bit_t len=0;
    rohc_U8bit_t rcv_pkt_typ=0;
    rohc_U8bit_t flag = 1;
    rohc_U8bit_t  **p_pkt=p_rohc_pkt;

    rcv_pkt_typ = p_dsc->dpib->rcv_pkt_typ;

    switch(rcv_pkt_typ)
    {
        /* 
         ** START_SPR_1197_FIX : Introduced the  handling of 
         ** ROHC_R1P1_PKTTYPE type packets in profile-1 (RTP) for 
         ** outer_ip4. This part has not been tested as yet and shall
         ** be tested in future releases.
         */
        case ROHC_R1P1_PKTTYPE :
        /* END_SPR_1197_FIX */
        case ROHC_R0P1_PKTTYPE :

        /* 
         ** START_SPR_569_FIX : Introduced the  handling of 
         ** ROHC_R0P1_PKTTYPE type packets in profile-1 (RTP) for 
         ** outer_ip4. This part has not been tested as yet and shall
         ** be tested in future releases.
         */
        if( p_dsc->pre_pkt_type != ROHC_IRDYN_PKTTYPE )
        {
            ((rohc_ip4_t*)*p_pkt)->id = ROHC_HTONS(((rohc_ip4_t*)*p_pkt)->id);
        }
        else
        {
            p_dsc->inner_ip4->id = ROHC_HTONS(p_dsc->inner_ip4->id);
        }
        break;
        /* END_SPR_569_FIX */
        case ROHC_R1IDP1_PKTTYPE :
        case ROHC_R1IDE0P1_PKTTYPE:
        case ROHC_R1IDE1P1_PKTTYPE:
        case ROHC_R1IDE2P1_PKTTYPE:
        case ROHC_R1TSP1_PKTTYPE :
        case ROHC_R1TSE0P1_PKTTYPE:
        case ROHC_R1TSE1P1_PKTTYPE:
        case ROHC_R1TSE2P1_PKTTYPE:
        if( (p_dsc->inner_ip4) || (p_dsc->rnd == ROHC_RND_PRESENT) )
            ((rohc_ip4_t*)*p_pkt)->id |= p_dsc->dpib->decoded_ipid2;
        break;
        case ROHC_R1IDE3P1_PKTTYPE:
        case ROHC_R1TSE3P1_PKTTYPE:
        if( (p_dsc->inner_ip4) || (p_dsc->rnd == ROHC_RND_PRESENT) )
            ((rohc_ip4_t*)*p_pkt)->id |= p_dsc->dpib->decoded_ipid2;
        /* COVERITY 10130 */
        rohc_update_p1_outerip_from_ext3(*p_pkt,p_dsc);
        flag = 0;
        break;
        /* COVERITY 10130 */
        case ROHC_UO1IDE3P1_PKTTYPE:
        rohc_update_p1_outerip_from_ext3(*p_pkt,p_dsc);
        flag = 0;
        break;
    }
    ((rohc_ip4_t*)*p_pkt)->id = ROHC_HTONS(((rohc_ip4_t*)*p_pkt)->id);
    /* Fix for RND Bits */ 
    if( ( ROHC_RND_ABSENT  == p_dsc->rnd2 )  &&  (p_dsc->nbo2 == ROHC_FALSE))
    {
        ((rohc_ip4_t*)*p_pkt)->id = 
            (((rohc_ip4_t*)*p_pkt)->id >> 8) |
            ((((rohc_ip4_t*)*p_pkt)->id << 8) & 0xff00);
    }
    len = sizeof(struct rohc_ip4_t);
    *p_pkt+=len;

    return flag;
}


/*****************************************************************
**
**    FUNCTION NAME
**        rohc_dpbm_make_outer_ip6
**
*******************************************************************
**
**    DESCRIPTION
**         This function picks up required fields for regenerating
**         uncompressed IP datagram from DSC for outer IP v6 header.
**
**    INPUTS
**		  p_rohc_pkt - Pointer to ROHC Packet
**        p_dsc - Pointer to Decompressor Stream context
**        p_entity - Pointer to ROHC entity context    
**        p_dprm_update_blk -  Pointer to ROHC global context
**		  p_out_ip6 - Pointer to Outer IPv6 header    
**        p_ecode - Pointer to error code     
**    RETURNS
**    -    
**
******************************************************************/
/* +- SPR 17777 */
static rohc_U8bit_t rohc_dpbm_make_outer_ip6
    ROHC_PARAMS((p_rohc_pkt,p_dsc),
    rohc_U8bit_t           ** p_rohc_pkt        _AND_
    struct rohc_dsc_rtp_t   * p_dsc)
{
    rohc_U32bit_t len=0;
    rohc_U8bit_t rcv_pkt_typ=0;
    rohc_U8bit_t flag = 1;
    rohc_U8bit_t **p_pkt=p_rohc_pkt;

    rcv_pkt_typ = p_dsc->dpib->rcv_pkt_typ;


    /* 
     ** START_SPR_569_FIX : Introduced the  handling of ROHC_R0P1_PKTTYPE
     ** type packets in profile-1 (RTP) for outer_ip6. This part has not 
     ** been tested as yet and shall be tested in future releases.
     */
    if (rcv_pkt_typ == ROHC_R0P1_PKTTYPE )
    {
        if( p_dsc->pre_pkt_type != ROHC_IRDYN_PKTTYPE )
        {
            ((rohc_ip4_t*)*p_pkt)->id = ROHC_HTONS(((rohc_ip4_t*)*p_pkt)->id);
        }
        else
        {
            p_dsc->inner_ip4->id = ROHC_HTONS(p_dsc->inner_ip4->id);
        }
    }
    /* END_SPR_569_FIX */
    if( (rcv_pkt_typ == ROHC_R1IDE3P1_PKTTYPE ) ||
            (rcv_pkt_typ == ROHC_R1TSE3P1_PKTTYPE ) ||
            (rcv_pkt_typ == ROHC_UO1IDE3P1_PKTTYPE ) )
    {
        rohc_update_p1_outerip_from_ext3(*p_pkt,p_dsc);
        flag = 0;
    }
    len = sizeof(struct rohc_ip6_t);
    *p_pkt+=len;

    return flag;
}
/*****************************************************************
**
**    FUNCTION NAME
**        rohc_d_make_p2_ip_pkt
**
*******************************************************************
**
**    DESCRIPTION
**         This function is the entry function for 
**         DECOMPRESSOR PACKET SENTINEL MODULE
**         This function picks up required fields for regenerating
**         uncompressed IP datagram from DSC.
**
**    INPUTS
**        p_pkt - Pointer to memory location where IP datagram has to be reasssembled 
**        p_len - Length of regenrated IP datagram
**		  p_dsc - Pointer to Decompressor Stream context
**        p_entity - Pointer to ROHC entity context    
**        p_dprm_update_blk - Pointer to ROHC global context    
**        p_ecode - Pointer to error code     
**    RETURNS
**    -    
**
******************************************************************/
/* +- SPR 17777 */
rohc_return_t rohc_d_make_p2_ip_pkt
    ROHC_PARAMS((p_pkt,p_len,p_dsc,p_dprm_update_blk),
    rohc_U8bit_t            * p_pkt             _AND_
    rohc_U32bit_t           * p_len             _AND_
    struct rohc_dsc_udp_t   * p_dsc             _AND_
    rohc_dprm_update_blk_t  *p_dprm_update_blk)
{
    /* + SPR 17439 */
    rohc_U8bit_t len=0;
    /* - SPR 17439 */
    rohc_U8bit_t *p_ext_hdr = NULL,*p_ext_hdr2 = NULL,*p_in_ip4 = NULL,
            *p_out_ip4 = NULL,*p_in_ip6 = NULL,*p_out_ip6 = NULL,*p_org=p_pkt;
    
    register rohc_U8bit_t rcv_pkt_typ;
    rcv_pkt_typ = p_dsc->dpib->rcv_pkt_typ;

    ROHC_ENTER_FN("rohc_d_make_p2_ip_pkt");
    
    /* Outer IP 4 Datagram*/
    if(p_dsc->outer_ip4)
    {
        p_dprm_update_blk->outer_ip4 = (rohc_ip4_t*)p_pkt;
        p_out_ip4 = p_pkt;
        ROHC_MEMCPY(p_pkt,ROHC_DSC_GET_OUTER_IP4(p_dsc),
                sizeof(struct rohc_ip4_t));

        rohc_dpbm_make_p2_outer_ip4(&p_pkt, p_dsc);
        /* +- SPR 17777 */
    }/* end if outer IP4*/
    else if(p_dsc->outer_ip6)
    {
        /* Outer IP 6 Datagram*/
        p_dprm_update_blk->outer_ip6 = (rohc_ip6_t*)p_pkt;
        p_out_ip6 = p_pkt;
        ROHC_MEMCPY(p_pkt,ROHC_DSC_GET_OUTER_IP6(p_dsc),
                sizeof(struct rohc_ip6_t));

        rohc_dpbm_make_p2_outer_ip6(&p_pkt, p_dsc);
        /* +- SPR 17777 */
    }/*end if outer IP 6 */
    rohc_d_p2_outer_ip_ext_list_cons(p_dsc,&p_ext_hdr,&p_ext_hdr2,p_pkt,
            p_out_ip4,p_out_ip6,&len);
    p_pkt += len;
    
    /* inner IP 4 Datagram*/
    if(p_dsc->inner_ip4)
    {
        if(p_ext_hdr)
        {
            *p_ext_hdr = *p_ext_hdr2 = ROHC_IPPROTO_IP4;
            p_ext_hdr = NULL;
        }
        p_dprm_update_blk->inner_ip4 = (rohc_ip4_t*)p_pkt;
        p_in_ip4 = p_pkt;
        ROHC_MEMCPY(p_pkt,ROHC_DSC_GET_INNER_IP4(p_dsc),
                sizeof(struct rohc_ip4_t));

        rohc_dpbm_make_p2_inner_ip4(&p_pkt, p_dsc);
        /* +- SPR 17777 */
    } /* end if inner ip4 */
    else if(p_dsc->inner_ip6)
    {
        /* inner IP 6 Datagram*/
        if(p_ext_hdr)
        {
            *p_ext_hdr = *p_ext_hdr2 = ROHC_IPPROTO_IP6;
            p_ext_hdr = NULL;
        }
        p_dprm_update_blk->inner_ip6 = (rohc_ip6_t*)p_pkt;
        p_in_ip6 = p_pkt;
        ROHC_MEMCPY(p_pkt,ROHC_DSC_GET_INNER_IP6(p_dsc),
                sizeof(struct rohc_ip6_t));

        rohc_dpbm_make_p2_inner_ip6(&p_pkt, p_dsc);
        /* +- SPR 17777 */
    } /* end if inner ip6 */
    rohc_d_p2_inner_ip_ext_list_cons(p_dsc,&p_ext_hdr,&p_ext_hdr2,p_pkt,
            p_in_ip4,p_in_ip6,&len);
    p_pkt += len;
    {
        if(p_ext_hdr)
        {
            *p_ext_hdr = *p_ext_hdr2 = ROHC_IPPROTO_UDP;
        }
        p_dprm_update_blk->udp = (rohc_udp_t*)p_pkt;
        ROHC_MEMCPY(p_pkt,ROHC_DSC_GET_UDP(p_dsc),
                sizeof(struct rohc_udp_t));
        len=sizeof(struct rohc_udp_t);
        ((rohc_udp_t*)p_pkt)->len = ROHC_HTONS((rohc_U16bit_t)
                                    (len + p_dsc->dpib->payload_len));
        p_pkt+=len;
    } /* end if udp profile */
    rohc_d_p2_update_ip_len_cksum(p_dsc,p_in_ip4,p_in_ip6,p_out_ip4,
            p_out_ip6,p_pkt);
    /* 
    ** START_SPR_569_FIX : Store received pkt type into previous pkt type.
    */
    p_dsc->pre_pkt_type = rcv_pkt_typ;
   /* END_SPR_569_FIX */
    *p_len=(p_pkt - p_org);
    ROHC_EXIT_FN("rohc_d_make_p2_ip_pkt");
    return ROHC_SUCCESS;    
}/* end function rohc_d_make_ip_pkt */

/*****************************************************************
**
**    FUNCTION NAME
**        rohc_dpbm_make_p2_outer_ip4
**
*******************************************************************
**
**    DESCRIPTION
**         This function picks up required fields for regenerating
**         uncompressed IP datagram from DSC for outer IP v4 header.
**
**    INPUTS
**		  p_rohc_pkt - Pointer to ROHC Packet 
**        p_dsc - Pointer to Decompressor Stream context
**        p_entity - Pointer to ROHC entity context    
**        p_dprm_update_blk - Pointer to ROHC global context    
**		  p_out_ip4 - Pointer to Outer IPv4 header
**        p_ecode - Pointer to error code     
**    RETURNS
**    -    
**
******************************************************************/
/* +- SPR 17777 */
static void rohc_dpbm_make_p2_outer_ip4
    ROHC_PARAMS((p_rohc_pkt,p_dsc),
    rohc_U8bit_t            ** p_rohc_pkt             _AND_
    struct rohc_dsc_udp_t   * p_dsc)
{
    rohc_U32bit_t len=0;
    rohc_U8bit_t rcv_pkt_typ=0;
    rohc_U8bit_t  **p_pkt=p_rohc_pkt;

    rcv_pkt_typ = p_dsc->dpib->rcv_pkt_typ;

    switch(rcv_pkt_typ)
    {
        case ROHC_R0P1_PKTTYPE :

            /* 
             ** START_SPR_569_FIX : Introduced the  handling of 
             ** ROHC_R0P1_PKTTYPE type packets in profile-1 (RTP) for 
             ** outer_ip4. This part has not been tested as yet and shall
             ** be tested in future releases.
             */
            if( p_dsc->pre_pkt_type != ROHC_IRDYN_PKTTYPE )
            {
                ((rohc_ip4_t*)*p_pkt)->id = ROHC_HTONS(((rohc_ip4_t*)*p_pkt)->id);
            }
            else
            {
                p_dsc->inner_ip4->id = ROHC_HTONS(p_dsc->inner_ip4->id);
            }
            break;
            /* END_SPR_569_FIX */
        case ROHC_R1IDP1_PKTTYPE :
        case ROHC_R1IDE0P1_PKTTYPE:
        case ROHC_R1IDE1P1_PKTTYPE:
        case ROHC_R1IDE2P1_PKTTYPE:
        case ROHC_R1TSP1_PKTTYPE :
        case ROHC_R1TSE0P1_PKTTYPE:
        case ROHC_R1TSE1P1_PKTTYPE:
        case ROHC_R1TSE2P1_PKTTYPE:
            if( (p_dsc->inner_ip4) || (p_dsc->rnd == ROHC_RND_PRESENT) )
                ((rohc_ip4_t*)*p_pkt)->id |= p_dsc->dpib->decoded_ipid2;
            break;
        case ROHC_R1IDE3P1_PKTTYPE:
        case ROHC_R1TSE3P1_PKTTYPE:
            if( (p_dsc->inner_ip4) || (p_dsc->rnd == ROHC_RND_PRESENT) )
                ((rohc_ip4_t*)*p_pkt)->id |= p_dsc->dpib->decoded_ipid2;
            /* SPR 4808 Fix Start */
            break;
            /* SPR 4808 Fix End */
        case ROHC_UO1IDE3P1_PKTTYPE:
            rohc_update_p2_outerip_from_ext3(*p_pkt,p_dsc);
            break;
    }

    ((rohc_ip4_t*)*p_pkt)->id = ROHC_HTONS(((rohc_ip4_t*)*p_pkt)->id);
    /* Fix for RND Bits */ 
    if( ( ROHC_RND_ABSENT  == p_dsc->rnd2 )  && ( p_dsc->nbo2 == ROHC_FALSE ))
    {
        ((rohc_ip4_t*)*p_pkt)->id = 
            (((rohc_ip4_t*)*p_pkt)->id >> 8) |
            ((((rohc_ip4_t*)*p_pkt)->id << 8) & 0xff00);
    }
    len = sizeof(struct rohc_ip4_t);
    *p_pkt+=len;

}

/*****************************************************************
**
**    FUNCTION NAME
**        rohc_dpbm_make_p2_outer_ip6
**
*******************************************************************
**
**    DESCRIPTION
**         This function picks up required fields for regenerating
**         uncompressed IP datagram from DSC for outer IP v6 header.
**
**    INPUTS
**		  p_rohc_pkt - Pointer to ROHC Packet 
**        p_dsc - Pointer to Decompressor Stream context
**        p_entity - Pointer to ROHC entity context    
**        p_dprm_update_blk - Pointer to ROHC global context    
**		  p_out_ip6 - Pointer to Outer IPv6 header
**        p_ecode - Pointer to error code     
**    RETURNS
**    -    
**
******************************************************************/
/* +- SPR 17777 */
static void rohc_dpbm_make_p2_outer_ip6
    ROHC_PARAMS((p_rohc_pkt,p_dsc),
    rohc_U8bit_t            **p_rohc_pkt             _AND_
    struct rohc_dsc_udp_t   * p_dsc)
{
    rohc_U32bit_t len=0;
    rohc_U8bit_t rcv_pkt_typ=0;
    rohc_U8bit_t **p_pkt=p_rohc_pkt;

    rcv_pkt_typ = p_dsc->dpib->rcv_pkt_typ;

    if( (rcv_pkt_typ == ROHC_R1IDE3P1_PKTTYPE ) ||
            (rcv_pkt_typ == ROHC_R1TSE3P1_PKTTYPE ) ||
            (rcv_pkt_typ == ROHC_UO1IDE3P1_PKTTYPE ) )
    {
        rohc_update_p2_outerip_from_ext3(*p_pkt,p_dsc);
    }

    len = sizeof(struct rohc_ip6_t);
    *p_pkt+=len;
}


/*****************************************************************
**
**    FUNCTION NAME
**        rohc_dpbm_make_p2_inner_ip4
**
*******************************************************************
**
**    DESCRIPTION
**         This function picks up required fields for regenerating
**         uncompressed IP datagram from DSC for inner IP v4 header.
**
**    INPUTS
**		  p_rohc_pkt - Pointer to ROHC Packet 
**        p_dsc - Pointer to Decompressor Stream context
**        p_entity - Pointer to ROHC entity context    
**        p_dprm_update_blk - Pointer to ROHC global context    
**		  p_in_ip4 - Pointer to inner IPv4 header
**        p_ecode - Pointer to error code     
**    RETURNS
**    -    
**
******************************************************************/
/* +- SPR 17777 */
static void rohc_dpbm_make_p2_inner_ip4
    ROHC_PARAMS((p_rohc_pkt,p_dsc),
    rohc_U8bit_t            ** p_rohc_pkt             _AND_
    struct rohc_dsc_udp_t   * p_dsc)
{
    rohc_U32bit_t len=0;
    rohc_U8bit_t rcv_pkt_typ=0;
    rohc_U8bit_t  **p_pkt=p_rohc_pkt;

    rcv_pkt_typ = p_dsc->dpib->rcv_pkt_typ;

    switch(rcv_pkt_typ)
    {
        case ROHC_R0P1_PKTTYPE :

            /* 
             ** START_SPR_569_FIX : Introduced the  handling of
             ** ROHC_R0P1_PKTTYPE type packets in profile-1 (RTP)
             ** for inner_ip4.
             */
            if( p_dsc->pre_pkt_type != ROHC_IRDYN_PKTTYPE )
            {
                ((rohc_ip4_t*)*p_pkt)->id = ROHC_HTONS(((rohc_ip4_t*)*p_pkt)->id);
            }
            else
            {
                p_dsc->inner_ip4->id = ROHC_HTONS(p_dsc->inner_ip4->id);
            }
            /* END_SPR_569_FIX */
        case ROHC_R1IDP1_PKTTYPE :
        case ROHC_R1IDE0P1_PKTTYPE:
        case ROHC_R1IDE1P1_PKTTYPE:
        case ROHC_R1IDE2P1_PKTTYPE:
        case ROHC_R1TSP1_PKTTYPE :
        case ROHC_R1TSE0P1_PKTTYPE:
        case ROHC_R1TSE1P1_PKTTYPE:
        case ROHC_R1TSE2P1_PKTTYPE:
            if( p_dsc->rnd != ROHC_RND_PRESENT)
                ((rohc_ip4_t*)*p_pkt)->id = p_dsc->dpib->decoded_ipid;
            break;
        case ROHC_R1IDE3P1_PKTTYPE:
        case ROHC_R1TSE3P1_PKTTYPE:
            if(p_dsc->rnd != ROHC_RND_PRESENT)
                ((rohc_ip4_t*)*p_pkt)->id = p_dsc->dpib->decoded_ipid;
            /* COVERITY 10129 */
            rohc_update_p2_innerip_from_ext3(*p_pkt,p_dsc);
            break;
            /* COVERITY 10129 */
        case ROHC_UO1IDE3P1_PKTTYPE:
            rohc_update_p2_innerip_from_ext3(*p_pkt,p_dsc);
            break;

    }

    ((rohc_ip4_t*)*p_pkt)->id = ROHC_HTONS(((rohc_ip4_t*)*p_pkt)->id);
    /* Fix for RND Bits */ 
    if( ( ROHC_RND_ABSENT  == p_dsc->rnd ) && ( p_dsc->nbo == ROHC_FALSE))
    {
        ((rohc_ip4_t*)*p_pkt)->id = 
            (((rohc_ip4_t*)*p_pkt)->id >> 8) |
            ((((rohc_ip4_t*)*p_pkt)->id << 8) & 0xff00);
    }
    len =sizeof(struct rohc_ip4_t);
    *p_pkt+=len;
}

/*****************************************************************
**
**    FUNCTION NAME
**        rohc_dpbm_make_p2_inner_ip6
**
*******************************************************************
**
**    DESCRIPTION
**         This function picks up required fields for regenerating
**         uncompressed IP datagram from DSC for inner IP v6 header.
**
**    INPUTS
**		  p_rohc_pkt - Pointer to ROHC Packet 
**        p_dsc - Pointer to Decompressor Stream context
**        p_entity - Pointer to ROHC entity context    
**        p_dprm_update_blk - Pointer to ROHC global context    
**		  p_in_ip6 - Pointer to inner IPv6 header
**        p_ecode - Pointer to error code     
**    RETURNS
**    -    
**
******************************************************************/
/* +- SPR 17777 */
static void rohc_dpbm_make_p2_inner_ip6
    ROHC_PARAMS((p_rohc_pkt,p_dsc),
    rohc_U8bit_t            ** p_rohc_pkt             _AND_
    struct rohc_dsc_udp_t   * p_dsc)
{
    rohc_U32bit_t len=0;
    rohc_U8bit_t rcv_pkt_typ=0;
    rohc_U8bit_t **p_pkt=p_rohc_pkt;

    rcv_pkt_typ = p_dsc->dpib->rcv_pkt_typ;

    if( (rcv_pkt_typ == ROHC_R1IDE3P1_PKTTYPE ) ||
            (rcv_pkt_typ == ROHC_R1TSE3P1_PKTTYPE ) ||
            (rcv_pkt_typ == ROHC_UO1IDE3P1_PKTTYPE ) )             
    {
        rohc_update_p2_innerip_from_ext3(*p_pkt,p_dsc);
    }

    len=sizeof(struct rohc_ip6_t);
    *p_pkt+=len;
}
/*****************************************************************
**
**    FUNCTION NAME
**        rohcDMakeP6IpPkt
**
*******************************************************************
**
**    DESCRIPTION
**         This function is the entry function for 
**         DECOMPRESSOR PACKET SENTINEL MODULE
**         This function picks up required fields for regenerating
**         uncompressed IP datagram from DSC. It also regenerates the Base
**         header fields.
**
**    INPUTS
**        p_pkt - Pointer to memory location where IP datagram has to be reasssembled 
**        p_len - Length of regenrated IP datagram
**		  p_dsc - Pointer to Decompressor Stream context
**        p_entity - Pointer to ROHC entity context    
**        p_dprm_update_blk - Pointer to ROHC global context    
**        p_ecode -Pointer to error code     
**    RETURNS
**    -    
**
******************************************************************/
/* +- SPR 17777 */
rohc_return_t rohcDMakeP6IpPkt(
    rohc_U8bit_t            * p_pkt             ,
    rohc_U32bit_t           * p_len             ,
    struct rohc_dsc_tcp_t   * p_dsc             ,
    rohc_dprm_update_blk_t  *p_dprm_update_blk)
/* +- SPR 17777 */
{
    rohc_U32bit_t len=0;
    rohc_U8bit_t *p_in_ip4 = NULL,
                  *p_org=p_pkt;

    /*Timestamp option. The first 4 bytes(TSval) are the time that the packet 
      was sent,the Remaining 4 (TSecr) echo the TSval of a packet that was 
      received.TSecr is only valid when ACK bit is set.*/

    register rohc_U8bit_t rcv_pkt_typ = p_dsc->dpib->rcv_pkt_typ;

    ROHC_ENTER_FN("rohcDMakeP6IpPkt");

    /*
       Outer Outer IP hader exist,i.e it is case of tunneled ip packet
       --------------------------------------------------------------
       |          |               |---------------------------------|
       | Ethernet |   Outer IP    |  Inner IP     |     Data        |
       |          |               |---------------------------------|
       --------------------------------------------------------------
     */

    if(p_dsc->inner_ip4)
    {
      /*
          If inner ip header is IPv4 
      */
        p_dprm_update_blk->inner_ip4 = (rohc_ip4_t*)p_pkt;
        p_in_ip4 = p_pkt;
       /*
          Memcpy the Ip4v structure from DSC(p_dsc) to the packet buffer
          p_pkt
          */
        ROHC_MEMCPY(p_pkt,ROHC_DSC_GET_INNER_IP4(p_dsc),
               sizeof(struct rohc_ip4_t));

        rohcDecompMakeInnerIp4BlockP6( p_dsc  ,p_dprm_update_blk );

        len =sizeof(struct rohc_ip4_t);
        p_pkt+=len;
    } /* end if inner ip4 */

    /*
       Appending TCP  header to the Packet;
       +
       Appending TCP Options header to the Packet;
       sizeof Option header is 
       (value of Offset - 5)*4
     */
    p_dprm_update_blk->tcp = (rohc_tcp_t*)p_pkt;
    /*
       Memcpy the TCP structure from DSC(p_dsc)to the packet buffer
       p_pkt
       */
    ROHC_MEMCPY(p_pkt,ROHC_DSC_GET_TCP(p_dsc),
            sizeof(struct rohc_tcp_t) + (( (p_dsc->tcp->offset >> 4) -5)*4));

    rohcDecompMakeTcpBlockP6( p_dsc  ,p_dprm_update_blk );
    
    if ( ROHC_IR_WITHOUT_DYN_PKTTYPE > rcv_pkt_typ)
    {
        rohcDecompMakeTcpIrregChain( p_dsc , p_dprm_update_blk );         
    }

    len = sizeof(struct rohc_tcp_t) + (((p_dprm_update_blk->tcp->offset>>4) -5)*4);
    p_pkt+=len;

    if(p_dsc->inner_ip4)
    {
        ((rohc_ip4_t*)p_in_ip4)->len = ROHC_HTONS((rohc_U16bit_t)
            ((p_pkt - p_in_ip4) + p_dsc->dpib->payload_len));
        ((rohc_ip4_t*)p_in_ip4)->cksum = 0;
        ((rohc_ip4_t*)p_in_ip4)->cksum = 
            (rohc_cal_ip_checksum(p_in_ip4,
                ROHC_IP4_HL((rohc_ip4_t*)p_in_ip4)*4));
    }
    
    p_dsc->pre_pkt_type = rcv_pkt_typ;
    *p_len=(p_pkt - p_org);
    ROHC_EXIT_FN("rohcDMakeP6IpPkt");
    return ROHC_SUCCESS;    
}/* end function rohcDMakeP6IpPkt */

/******Feedback Packet Fromation***************************************/
/*****************************************************************************
**      FUNCTION:
**              rohcDP6MakeFeedbackPacket
******************************************************************************
**
**      DESCRIPTION:
**          Compose Feedback packet 
**      ARGUMENTS:
**          p_entity - Pointer to ROHC entity     
**          p_feedback - Pointer to feedback packet     
**          p_ecode - Pointer to error code     
**      RETURN VALUE:
**          ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**      This function composes feedback buffer for profile 6 from Feedback 
**      structure when invoked by DPM .
**      D-PPM queues the received feedback buffers    
*****************************************************************************/
rohc_return_t 
rohcDP6MakeFeedbackPacket (
        struct rohc_entity_t * p_entity,
        struct rohc_feedback_t * p_feedback,   
        rohc_error_code_t   *p_ecode)
{
    rohc_U8bit_t *p_feedback_pkt=NULL,*p_out_pkt;
    rohc_U32bit_t len=0;
    /* SPR 13478 FiX START */
    ROHC_ENTER_FN("rohcDP6MakeFeedbackPacket"); 
    rohc_U8bit_t crc_index = 0;
    /* SPR 13478 FiX END */
    /*
     * FEEDBACK PACKET STRUCTURE
     *          
     *              -------------------------------
     *              | 1 | 1 | 1 | 1 | 0 |   Code   |
     *              -------------------------------
     *              |           SIZE               |
     *              -------------------------------
     *              |        ADD CID OCTET         |
     *              -------------------------------
     *              /         Large CID            /
     *              -------------------------------
     *              |       Feedback Data          |
     *              -------------------------------
     */

    p_out_pkt = 
     p_feedback_pkt=(rohc_U8bit_t *)ROHC_MALLOC(ROHC_MAX_FEEDBACK_PKT_SIZE);
    if(p_feedback_pkt == NULL)
    {
        ROHC_LOG(LOG_INFO,"CPBM :EID[%u:%u]:CID[%u]: Unable"\
                    " to allocate memory for feedback packet",
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,
                    p_entity->context_id);
        *p_ecode = ROHC_MEMORY_FAILURE;
        return ROHC_FAILURE;
    }
    p_feedback_pkt[0]=0xf0;
    p_feedback_pkt[1]=0; /* Setting SIZE=0, as it will be filled later */
    /*now the feedback data field starts*/
    len =0;
    p_feedback_pkt+=2;
    if(!p_entity->large_cids)
    {
        if(p_feedback->cid!=0)
        {
            p_feedback_pkt[len]=0xe0|(p_feedback->cid&0x0f);
            len+=1;
      }
    }
    else
    {
         len += rohc_c_encode_sdvl((p_feedback_pkt),
                                            p_feedback->cid);
    }/* end else large cid's*/

    if(ROHC_FB_1 == p_feedback->feedback_type)
    {
        p_out_pkt[0] |= 0x01;

        /*
         *      FEEDBACK TYPE 1 DATA
         *
         *              -------------------------------
         *              |       8 - bit MSN           |
         *              -------------------------------
         *
         */
        p_feedback_pkt[len]=(rohc_U8bit_t)p_feedback->msn;
        len += 1;

        ROHC_MEMMOVE((p_out_pkt + 1),
                (p_out_pkt + 2),
                len);
        p_out_pkt[ROHC_MAX_API_ELMS_LEN + ROHC_MAX_ELEM_LEN] |= len;
        ROHC_MEMMOVE(
                (rohc_U8bit_t*)(p_out_pkt +( ROHC_MAX_API_ELMS_LEN+
                        ROHC_MAX_ELEM_LEN + 1)),
                (rohc_U8bit_t*)(p_out_pkt +( ROHC_MAX_API_ELMS_LEN+
                        ROHC_MAX_ELEM_LEN + 2)),
                len);

        /* START_COVERITY_25502_FIX*/
        p_entity->fb_info.p_buff = p_out_pkt;
        p_entity->fb_info.len = (len + 1);
        /* END_COVERITY_25502_FIX*/
        return ROHC_SUCCESS;
    }
    else /* feedback-2 */
    {
        /*
         *      FEEDBACK TYPE 1 DATA
         *
         *              -------------------------------
         *              |Ack type|    6 - bit MSN     |
         *              -------------------------------
         *              |       8 - bit MSN           |
         *              -------------------------------
         *              |           CRC               |
         *              -------------------------------
         *              /    Feedback Options         /
         *              -------------------------------
         *
         */
        p_feedback_pkt[len] =
                (((p_feedback->ack_type<<6)|( ( (p_feedback->msn >> 8) & 0x3F) )));
        /*  SN TBV */
        len += 1;
        p_feedback_pkt[len] = (rohc_U8bit_t)(p_feedback->msn);

        /* SPR 13478 FiX START */
        len += 1;

        /* crc index will be after 8- bit MSN Value */
        crc_index = len;

        /* set/move the CRC index as it will filled later */
        p_feedback_pkt[len] = 0;
        len += 1;
        /* len is being moved 1 step ahead to negotiate for CRC-8 calcuation at 
         * the end of packet formation */
        /* SPR 13478 FiX END */

        /* if we are compelled to send 16 bits of MSN rather than 14 bits */
        if( 0x0004000 < p_feedback->msn)            
        {
            p_feedback->opt_type[0] = ROHC_FBO_SN;
        }

        if(ROHC_MEMORY_FAILURE == *p_ecode)
        {
            p_feedback->opt_type[0] = ROHC_FBO_CONTEXT_MEM;
        }

        if(ROHC_MSN_NOT_VALID == *p_ecode)
        {
            p_feedback->opt_type[0] = ROHC_FBO_SN_NOT_VALID;
        }

        switch(p_feedback->opt_type[0])
        {
            case ROHC_FBO_REJECT :
                /*
                 *              -------------------------------
                 *              | Opt type =2 | Opt len = 0   |
                 *              -------------------------------
                 */
                p_feedback_pkt[len]=0x20U;
                len+=1;
                break;

            case ROHC_FBO_SN_NOT_VALID :
                /*
                 *              -------------------------------
                 *              | Opt type =3 | Opt len = 0   |
                 *              -------------------------------
                 */
                p_feedback_pkt[len]=0x30U;
                len+=1;
                break;

            case ROHC_FBO_SN :
                /*
                *              -------------------------------
                 *              | Opt type = 4 | Opt len = 1  |
                 *              -------------------------------
                 *              |2-bit MSN|     Reserved      |
                 *              -------------------------------
                 */
                p_feedback_pkt[len]=0x41U;
                p_feedback_pkt[len+1]=p_feedback->msn&(0xC0);
                len+=2;
                break;

            case ROHC_FBO_CONTEXT_MEM :
                /*
                 *              -------------------------------
                 *              | Opt type =9 | Opt len = 0   |
                 *              -------------------------------
                 */
                p_feedback_pkt[len]=0x90U;
                len+=1;
                break;
        }   /* end switch*/
    }
    if( len <= 7)
    {
        /* SPR 13478 FiX START */
        /* As SIZE Octet will not be present the crc index 
         * will be shifted to one octet prior to current value */ 
        crc_index -= 1; 

        ROHC_MEMMOVE((p_out_pkt + 1), (p_out_pkt + 2), len);

        /* As Crc is to be counted only for feedback data removing the length for CID */
        /* Refer section 2.3. CRC Coverage in CRC Feedback Options of RFC 4815
         * CRC should include the Add-CID octet also aling with feedabck data */
        p_out_pkt[crc_index]=rohc_compute_crc(ROHC_CRC_TYPE_8,(p_out_pkt+1), len, 0xff);
        p_out_pkt[0] |= len; /* Setting length of Add-CID + feedback data in code field of first byte */
        len+=1; /* This len is incremented to include the first byte with type and code */
        /* SPR 13478 FiX END */
    }
    else
    {
        /* SPR 13478 FiX START */
        p_out_pkt[1] = len; /* Setting length of Add-CID + feedback data in SIZE field of first byte */
        /* As Crc is to be counted only for feedback data removing the length for CID */
        /* Refer section 2.3. CRC Coverage in CRC Feedback Options of RFC 4815
         * CRC should include the Add-CID octet also aling with feedabck data */
        p_out_pkt[crc_index]=rohc_compute_crc(ROHC_CRC_TYPE_8,(p_out_pkt+2), len, 0xff);
        len+=2; /* This len is incremented to include the first byte with type and code */
        /* SPR 13478 FiX END */
    }

    p_entity->fb_info.p_buff = p_out_pkt;
    p_entity->fb_info.len = len;
    ROHC_EXIT_FN("rohcDP6MakeFeedbackPacket");
    return ROHC_SUCCESS;
}/* end function rohcDP6MakeFeedbackPacket*/

/*****************************************************************************
**      FUNCTION:
**              processCompressedItemIrregTcpOptElement
******************************************************************************
**
**      DESCRIPTION:
**              This function process the Compressed List with Tcp option 
**              that is present as irregular chain                 
**      ARGUMENTS:
**          	p_dprm_update_blk - Pointer of Dprm Update Block
**          	p_dsc - Pointer to DSC Context     
**          	XI items     
**          	p_size - Pointer to size of tcp option (Uncompressed)
**		Returns :
**				Void   
*****************************************************************************/
static void  processCompressedItemIrregTcpOptElement(
        rohc_dprm_update_blk_t *p_dprm_update_blk,
        rohc_dsc_tcp_t         *p_dsc,
        rohc_U8bit_t XI,
        rohc_U8bit_t *p_size)
/* +- SPR 17777 */

{
    rohc_U8bit_t padLen; 
    switch(XI)
    {
       case INDEX_NOP:
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 1;
            break;
       case INDEX_EOL:
            *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 0;
			padLen = ( 4 -((*p_size)%4)); 
            ROHC_MEMSET ((((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) , 0 , padLen);
           (*p_size)+=padLen;  	
            break;
       case INDEX_MSS:
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 2;   
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 4;
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = p_dsc->dpib->mss[0]; 
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = p_dsc->dpib->mss[1];
            break;
 
        case INDEX_TIMESTAMP:
            *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 8;
            *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 10;
            *((rohc_U32bit_t *)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) +
                (*p_size))) = ROHC_HTONL(p_dsc->dpib->decoded_ts);     
            *p_size +=4;
             *(rohc_U32bit_t *)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) 
                    + (*p_size)) = ROHC_HTONL(p_dsc->dpib->decodedTsEchoVal);     
            *p_size +=4;

            break;
       case INDEX_WINDOW_SCAL:
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 3;
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 3;
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = p_dsc->dpib->wscale;
            break;
       
         case INDEX_SACK_PERM:
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 4;
                *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 2;
           break;
       
    
        case INDEX_SACK:
            *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 5;
	    	
            if (p_dsc->dpib->decodedSackBlockStart4) {
                    *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 0x22;
                    *(rohc_U32bit_t*)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) = 
                      ROHC_HTONL(p_dsc->dpib->decodedSackBlockStart1);  
                    *p_size +=4;
                    *(rohc_U32bit_t*)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) = 
                      ROHC_HTONL(p_dsc->dpib->decodedSackBlockEnd1);  
                    *p_size +=4;
                    *(rohc_U32bit_t*)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) = 
                      ROHC_HTONL(p_dsc->dpib->decodedSackBlockStart2);  
                    *p_size +=4;
                    *(rohc_U32bit_t*)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) = 
                      ROHC_HTONL(p_dsc->dpib->decodedSackBlockEnd2);  
                    *p_size +=4;
                    *(rohc_U32bit_t*)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) = 
                      ROHC_HTONL(p_dsc->dpib->decodedSackBlockStart3);  
                    *p_size +=4;
                    *(rohc_U32bit_t*)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) = 
                      ROHC_HTONL(p_dsc->dpib->decodedSackBlockEnd3);  
                    *p_size +=4;
                    *(rohc_U32bit_t*)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) = 
                      ROHC_HTONL(p_dsc->dpib->decodedSackBlockStart4);  
                    *p_size +=4;
                    *(rohc_U32bit_t*)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) = 
                      ROHC_HTONL(p_dsc->dpib->decodedSackBlockEnd4);  
                    *p_size +=4;
		    } 
		    else if (p_dsc->dpib->decodedSackBlockStart3 )
		    {	     
                    *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 0x1A;
                    *(rohc_U32bit_t*)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) = 
                      ROHC_HTONL(p_dsc->dpib->decodedSackBlockStart1);  
                    *p_size +=4;
                    *(rohc_U32bit_t*)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) = 
                      ROHC_HTONL(p_dsc->dpib->decodedSackBlockEnd1);  
                    *p_size +=4;
                    *(rohc_U32bit_t*)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) = 
                      ROHC_HTONL(p_dsc->dpib->decodedSackBlockStart2);  
                    *p_size +=4;
                    *(rohc_U32bit_t*)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) = 
                      ROHC_HTONL(p_dsc->dpib->decodedSackBlockEnd2);  
                    *p_size +=4;
                    *(rohc_U32bit_t*)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) = 
                      ROHC_HTONL(p_dsc->dpib->decodedSackBlockStart3);  
                    *p_size +=4;
                    *(rohc_U32bit_t*)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) = 
                      ROHC_HTONL(p_dsc->dpib->decodedSackBlockEnd3);  
                    *p_size +=4;
		    } 
		    else if (p_dsc->dpib->decodedSackBlockStart2 )
		    {	     

                    *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 0x12;
                        *(rohc_U32bit_t*)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) = 
                      ROHC_HTONL(p_dsc->dpib->decodedSackBlockStart1);  
                    *p_size +=4;
                    *(rohc_U32bit_t*)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) = 
                      ROHC_HTONL(p_dsc->dpib->decodedSackBlockEnd1);  
                    *p_size +=4;
                    *(rohc_U32bit_t*)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) = 
                      ROHC_HTONL(p_dsc->dpib->decodedSackBlockStart2);  
                    *p_size +=4;
                    *(rohc_U32bit_t*)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) = 
                      ROHC_HTONL(p_dsc->dpib->decodedSackBlockEnd2);  
                    *p_size +=4;
		    } 
		    else if (p_dsc->dpib->decodedSackBlockStart1 )
		    {	     

                /* Block1 Start And Block1 End*/    
                    *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) = 0x0A;
                    *(rohc_U32bit_t*)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) = 
                      ROHC_HTONL(p_dsc->dpib->decodedSackBlockStart1);  
                    *p_size +=4;
                    *(rohc_U32bit_t*)(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) = 
                      ROHC_HTONL(p_dsc->dpib->decodedSackBlockEnd1);  
                    *p_size +=4;
             }
             else
             {
                 if( (p_dsc->dpib->sackBlockUnchanged[0] == 10) || (p_dsc->dpib->sackBlockUnchanged[0] == 18)
                         || (p_dsc->dpib->sackBlockUnchanged[0] == 26) || (p_dsc->dpib->sackBlockUnchanged[0] == 34))
                 {
                     ROHC_MEMCPY((((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)) ,
                             p_dsc->dpib->sackBlockUnchanged,
                             (p_dsc->dpib->sackBlockUnchanged[0] -1));
                     *p_size += (p_dsc->dpib->sackBlockUnchanged[0] -1) ;
                 }
             }
             break;

        default:
            /* DataStructure needs to be updated to get TCP Option length */
            if(XI < ROHC_MAX_ENTRIES_TCP_OPT_IDX)
            {
                if (!((p_dsc)->tcpOptGenStaticFlag & (0x01 << (p_dsc)->optionTypeMap
                                [XI -INDEX_SACK])))
                {
                    if(((p_dsc)->dpib->tcpOptGen[(p_dsc)->optionTypeMap[XI -INDEX_SACK]].len ) > 0 &&
                            ((p_dsc)->dpib->tcpOptGen[(p_dsc)->optionTypeMap[XI -INDEX_SACK]].len ) <= MAX_TCP_OPT_SIZE)
                    {
                        *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) =
                            (p_dsc)->optionTypeMap[XI -INDEX_SACK];
                        *(((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size)++) =
                            (p_dsc)->dpib->tcpOptGen[(p_dsc)->optionTypeMap[XI -INDEX_SACK]].len + 1;
                        ROHC_MEMCPY( (((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + (*p_size))
                                , ((rohc_dsc_tcp_t*)p_dsc)->dpib->tcpOptGen
                                [(p_dsc)->optionTypeMap[XI -INDEX_SACK]].optionString,
                                ((rohc_dsc_tcp_t*)p_dsc)->dpib->tcpOptGen
                                [(p_dsc)->optionTypeMap[XI -INDEX_SACK]].len -1);
                        (*p_size) +=
                            (p_dsc)->dpib->tcpOptGen[(p_dsc)->optionTypeMap[XI -INDEX_SACK]].len -1;
                    }
                }
            }
            break; 
    }
}




/*****************************************************************************
**      FUNCTION: rohcFormTcpOptionfromCompressList
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function process the Compressed List with Tcp option 
**              that is present as irregular chain                 
**      ARGUMENTS:
**          	p_dprm_update_blk - Pointer of Dprm Update Block
**          	p_dsc - Pointer to DSC Context  
**		RETURNS:
**				Void   
*****************************************************************************/
static void rohcFormTcpOptionfromCompressList(
        rohc_dprm_update_blk_t *p_dprm_update_blk,
        rohc_dsc_tcp_t         *p_dsc)
{
    rohc_U8bit_t    ps_mask = 0;
    rohc_U8bit_t    m_count = 0;
    rohc_U8bit_t    XI = 0x00;
    rohc_U8bit_t    i = 0;
    rohc_U8bit_t    index = 0;
    rohc_U8bit_t    size_opt = sizeof(rohc_tcp_t);
    rohc_U8bit_t    opt_item_list_index = 0;
    
    if(p_dsc->dpib->tcp_dyn_options != ROHC_NULL)
    {
        ps_mask = p_dsc->dpib->tcp_dyn_options->res_ps_m & ROHC_DYN_PS_MASK;
        m_count = p_dsc->dpib->tcp_dyn_options->res_ps_m & ROHC_DYN_TCP_M_MASK;
    }
    if(0 < m_count)
    {
        XI = 0x00;
        for(i=0;i<m_count;i++)
        {
            if(!ps_mask)/*4bit XI*/
            {
                XI = (p_dsc->dpib->tcp_dyn_options_XI->xi_m[index]
                        >> 4);
                if ( XI & 0x08 )
                {
                    /*clearing the X bit of XI item*/
                    XI = XI & 0x07;
                    rohcDprmP6IrIrdynTcpOption(p_dprm_update_blk,
                        p_dsc,XI,&size_opt,&opt_item_list_index);
                }
                else
                {
                    processCompressedItemIrregTcpOptElement(
                            p_dprm_update_blk ,
                            p_dsc,XI,&size_opt);
                    /* +- SPR 17777 */
                }
                i++;
                XI = (p_dsc->dpib->tcp_dyn_options_XI->xi_m[index] 
                        & 0x0F);
                if((m_count & 0x01) && ((m_count-i)==0))
                {
                      size_opt++; //increase the size for padding
                      break;
                }

                if ( XI &  0x08 )
                {
                    /*clearing the X bit of XI item*/
                    XI = XI & 0x07;
                    rohcDprmP6IrIrdynTcpOption(p_dprm_update_blk,
                            p_dsc,XI,&size_opt,&opt_item_list_index);
                }
                else
                {
                    processCompressedItemIrregTcpOptElement(
                            p_dprm_update_blk ,
                            p_dsc,XI,&size_opt);
                    /* +- SPR 17777 */
                }    
                index ++;
            }
            else/*1byte XI*/
            {
                XI = p_dsc->dpib->tcp_dyn_options_XI->xi_m[i];
                if ( XI & 0x80  )
                {
                    XI = XI & 0x0F;
                    rohcDprmP6IrIrdynTcpOption(p_dprm_update_blk,
                        p_dsc,XI,&size_opt,&opt_item_list_index);
                }
                else
                {
                    processCompressedItemIrregTcpOptElement(
                            p_dprm_update_blk ,
                            p_dsc,XI,&size_opt);
                    /* +- SPR 17777 */

                }
            }
        }
    }

    if(p_dsc->dpib->tcp_dyn_options != ROHC_NULL)
    {
    /*+- SPR 20899*/
        p_dprm_update_blk->tcp->offset =(DIVIDE_BY_FOUR(size_opt)) 
                << 4; 
    /*+- SPR 20899*/
    }
}

/*****************************************************************************
**      FUNCTION: rohcProcessRsfFlag 
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function updates RST/SYN/FIN flag of TCP header on the 
**              basis of RSF flag                 
**      ARGUMENTS:
**          	p_dprm_update_blk - Pointer of Dprm Update Block
**          	p_dsc - Pointer to DSC Context     
**		RETURNS:
**				Void   
*****************************************************************************/
static void  rohcProcessRsfFlag(
                rohc_dprm_update_blk_t *p_dprm_update_blk,
                rohc_dsc_tcp_t         *p_dsc)
{    
    /*RSF FLAG*/
    switch ( p_dsc->dpib->field_val[ROHC_TCP_RSF_FLAGS_FID
                - ROHC_TCP_CTRL_FID_BASE])
    {
       case 0x00:
           p_dprm_update_blk->tcp->flags &= ~(ROHC_TCP_RST_MASK);
           p_dprm_update_blk->tcp->flags &= ~(ROHC_TCP_SYN_MASK);
           p_dprm_update_blk->tcp->flags &= ~(ROHC_TCP_FIN_MASK);
           break; 
       case 0x01: 
           p_dprm_update_blk->tcp->flags |=  ROHC_TCP_RST_MASK;
           p_dprm_update_blk->tcp->flags &= ~(ROHC_TCP_SYN_MASK);
           p_dprm_update_blk->tcp->flags &= ~(ROHC_TCP_FIN_MASK);
           break;
       case 0x02: 
           p_dprm_update_blk->tcp->flags |= ROHC_TCP_SYN_MASK;
           p_dprm_update_blk->tcp->flags &= ~(ROHC_TCP_RST_MASK);
           p_dprm_update_blk->tcp->flags &= ~(ROHC_TCP_FIN_MASK);
           break;
       case 0x03:     
           p_dprm_update_blk->tcp->flags |= ROHC_TCP_FIN_MASK;
           p_dprm_update_blk->tcp->flags &= ~(ROHC_TCP_RST_MASK);
           p_dprm_update_blk->tcp->flags &= ~(ROHC_TCP_SYN_MASK);
           break;
   }
}

/*****************************************************************************
**      FUNCTION: rohcDecompMakeTcpIrregChain 
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function form TCP Header from Irregular chain          
**      ARGUMENTS:
**          	p_dsc - Pointer of Dprm Update Block
**          	p_dprm_update_blk - Pointer to DSC Context     
**		RETURNS:
**				Void   
*****************************************************************************/
void rohcDecompMakeTcpIrregChain(
                struct rohc_dsc_tcp_t   * p_dsc             ,
                rohc_dprm_update_blk_t  *p_dprm_update_blk )
{
   /* Processing Irregular chain of TCP */
    rohc_U8bit_t  ecnUsed = ROHC_FALSE;
    register rohc_U8bit_t rcv_pkt_typ = p_dsc->dpib->rcv_pkt_typ;
    if ( (ROHC_CO_P6_RND8_PKTYPE == rcv_pkt_typ) || 
            (ROHC_CO_P6_SEQ8_PKTYPE == rcv_pkt_typ) ||
            (ROHC_CO_P6_COMMON_PKTYPE == rcv_pkt_typ) )
	{
		if (((rohc_dsc_tcp_t*)p_dsc)->dpib->field_val
                [ROHC_TCP_ECN_USED_FID - ROHC_VAR_FID_BASE])
		{
			ecnUsed = ROHC_TRUE;
		}
	}
	else
	{
        if (p_dsc->ecn_used)
        {
			ecnUsed = ROHC_TRUE;
		}
	}

    if ( ROHC_TRUE == ecnUsed )
    {
        p_dprm_update_blk->inner_ip4->tos =
            ( p_dprm_update_blk->inner_ip4->tos &  ~(0x03))
            | (p_dsc->dpib->field_val
                [ROHC_INNERIP4_ECN_FID - ROHC_VAR_FID_BASE]) ;
        p_dprm_update_blk->tcp->flags =
            (p_dprm_update_blk->tcp->flags & ~(ROHC_TCP_ECN_MASK))    
            | ( (p_dsc->dpib->field_val
            [ROHC_TCP_ECN_FLAG_FID-ROHC_VAR_FID_BASE]) << 6);
        p_dprm_update_blk->tcp->offset =
            ((p_dprm_update_blk->tcp->offset & ~(ROHC_TCP_RES_MASK)) 
            | (p_dsc->dpib->field_val
           [ROHC_TCP_RES_FLAG_FID-ROHC_VAR_FID_BASE] & ROHC_TCP_RES_MASK));
    }
    p_dprm_update_blk->tcp->checksum = 
       ROHC_HTONS (p_dsc->dpib->field_val[ROHC_TCP_CKSUM_FID-ROHC_VAR_FID_BASE]);

    /* Processing irregular chain of TCP options */        

    if (!(p_dsc->dpib->field_val[ROHC_TCP_LIST_PRESENT_FID- ROHC_TCP_CTRL_FID_BASE]))
    {   /*  */
        rohcFormTCPOptionsFromIrregChain(p_dsc,p_dprm_update_blk);
    }
    return;
}

/*****************************************************************************
**      FUNCTION: rohcDecompMakeTcpBlockP6 
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function form TCP Header from Irregular chain          
**      ARGUMENTS:
**          p_dsc - Pointer of Dprm Update Block
**          p_dprm_update_blk - Pointer to DSC Context     
**		RETURNS:
**				Void   
*****************************************************************************/
void rohcDecompMakeTcpBlockP6(
                struct rohc_dsc_tcp_t   * p_dsc             ,
                rohc_dprm_update_blk_t  *p_dprm_update_blk )
{
    register rohc_U8bit_t rcv_pkt_typ = p_dsc->dpib->rcv_pkt_typ;
    switch(rcv_pkt_typ)
    {
        case ROHC_CO_P6_COMMON_PKTYPE      :
            {
                rohcUpdateTcpHdrCoCommonPkt(p_dsc,p_dprm_update_blk);
            }
            break;

        case  ROHC_IRP6CR_PKTYPE :
            rohcUpdateTcpHdrIRP6CRPkt(p_dsc,p_dprm_update_blk);
            break;
        case  ROHC_CO_P6_RND1_PKTYPE         :     
        case  ROHC_CO_P6_RND2_PKTYPE         :    /*SN,MSN,PSH,CRC*/
        case  ROHC_CO_P6_SEQ1_PKTYPE         :    /*IPID,SN,MSN,PSH,CRC */
        case  ROHC_CO_P6_SEQ2_PKTYPE         : 
            {
                rohcUpdateTcpHdrRND1RND2SEQ1SEQ2Pkt(p_dsc,p_dprm_update_blk);
            }
            break;
        case  ROHC_CO_P6_RND3_PKTYPE         : 
        case  ROHC_CO_P6_RND4_PKTYPE         :    /*AN,MSN,PSH,CRC*/
        case  ROHC_CO_P6_SEQ3_PKTYPE         :    /*IPID,AN,MSN,PSH,CRC*/
        case  ROHC_CO_P6_SEQ4_PKTYPE         : 
            {
                rohcUpdateTcpHdrRND3RND4SEQ3SEQ4Pkt(p_dsc,p_dprm_update_blk);
            }
            break; 
        case  ROHC_CO_P6_RND5_PKTYPE         :    
        case  ROHC_CO_P6_RND6_PKTYPE         :    /*SN,AN,MSN,PSH,CRC*/
        case  ROHC_CO_P6_SEQ5_PKTYPE         :    /*IPID,SN,AN,MSN,PSH,CRC*/
        case  ROHC_CO_P6_SEQ6_PKTYPE         : 
            {
                rohcUpdateTcpHdrRND5RND6SEQ5SEQ6Pkt(p_dsc,p_dprm_update_blk);
            }
            break; 
        case  ROHC_CO_P6_RND7_PKTYPE         :    /*AN,MSN,PSH,CRC,WND*/ 
            {
                rohcUpdateTcpHdrRND7Pkt(p_dsc,p_dprm_update_blk);
            }
            break; 
        case  ROHC_CO_P6_SEQ7_PKTYPE         :    /*IPID,AN,MSN,PSH,CRC,WND*/
            {
                rohcUpdateTcpHdrSEQ7Pkt(p_dsc,p_dprm_update_blk);
            }
            break;
        case  ROHC_CO_P6_RND8_PKTYPE         :    /*RSF,LP,CRC,MSN,PSH,TTL,
                                                    ECNUSED,SN,AN,OPTIONS*/
        case  ROHC_CO_P6_SEQ8_PKTYPE         :    /*IPID,RSF,LP,CRC,MSN,PSH,
                                                    TTL,ECNUSED,SN,AN,OPTIONS*/
            {
                rohcUpdateTcpHdrRND8SEQ8Pkt(p_dsc,p_dprm_update_blk);
            }
            break;
    }
    return;
}


/*****************************************************************************
**      FUNCTION: rohcDecompMakeInnerIpV4BlockP6 
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function form Inner IP V4 Header from Compressed Base 
**              Header          
**      ARGUMENTS:
**          	p_dsc - Pointer to DSC Context     
**          	p_dprm_update_blk - Pointer of Dprm Update Block
**		RETURNS:
**				Void   
*****************************************************************************/
void rohcDecompMakeInnerIp4BlockP6(
                struct rohc_dsc_tcp_t   * p_dsc             ,
                rohc_dprm_update_blk_t  *p_dprm_update_blk )
{
      /*
          Depending on the Type CO packet recieved ,fields of IPv4 packets
          will be updated
       */
    register rohc_U8bit_t rcv_pkt_typ = p_dsc->dpib->rcv_pkt_typ;
    switch(rcv_pkt_typ)
    {

        case  ROHC_CO_P6_COMMON_PKTYPE              :
           {
               /*Updating the IP TOS for DSCP*/
               if(p_dsc->dpib->field_val
                       [ROHC_INNERIP4_DSCP_PRESENT_FID- ROHC_TCP_CTRL_FID_BASE])
                {
                    /*Updating TOS field */
                    ((p_dprm_update_blk->inner_ip4)->tos =  
                        ((p_dprm_update_blk->inner_ip4)->tos & (0x03))
                        |( p_dsc->dpib->
                        field_val[ROHC_INNERIP4_DSCP_FID-ROHC_VAR_FID_BASE]));
                    
                }

               /*Updating the IP IPID*/
               if((IP_ID_BEHAVIOR_SEQUENTIAL ==  p_dsc->dpib->
                           field_val[ ROHC_INNERIP_BEH_FID - ROHC_VAR_FID_BASE] ) ||
                   (IP_ID_BEHAVIOR_SEQUENTIAL_SWAPPED == p_dsc->dpib->
                           field_val[ ROHC_INNERIP_BEH_FID - ROHC_VAR_FID_BASE]  ))
               {
                   if(0 == p_dsc->dpib->field_val
                           [ROHC_TCP_IPID_IND_FID- ROHC_TCP_CTRL_FID_BASE])
                   {
                       (p_dprm_update_blk->inner_ip4)->id =
                           ROHC_HTONS(p_dsc->dpib->decoded_ipid);
                   }
                   else
                   {
                       (p_dprm_update_blk->inner_ip4)->id = ROHC_HTONS(p_dsc->dpib->
                           field_val[ ROHC_INNERIP_ID_FID - ROHC_VAR_FID_BASE]);
                   }
               }
               else
               {
                   (p_dprm_update_blk->inner_ip4)->id = p_dsc->dpib->
                        field_val[ ROHC_INNERIP_ID_FID - ROHC_VAR_FID_BASE];
                   (p_dprm_update_blk->inner_ip4)->id = 
                        ROHC_HTONS((p_dprm_update_blk->inner_ip4)->id);
               }

               /*Updating the IP TTL-Hopl*/
               if(p_dsc->dpib->field_val
                       [ROHC_INNERIP4_TTL_HPL_PRESENT_FID- ROHC_TCP_CTRL_FID_BASE])
                   (p_dprm_update_blk->inner_ip4)->ttl = 
                       p_dsc->dpib->field_val[ROHC_INNERIP4_TTL_FID-ROHC_VAR_FID_BASE];


               /*Updating the IP TOS for DSCP*/
               if(p_dsc->dpib->field_val[ROHC_INNERIP_DF_FID-ROHC_VAR_FID_BASE])
                   (p_dprm_update_blk->inner_ip4)->off |= ROHC_HTONS(ROHC_IP4_DF);
               else
               {
                   (p_dprm_update_blk->inner_ip4)->off &=
                            (rohc_U16bit_t)~(ROHC_HTONS(ROHC_IP4_DF));
               }

           }
           break;
        case ROHC_IRP6CR_PKTYPE:
                 /*Update IP- IpId*/
                 if( (IP_ID_BEHAVIOR_SEQUENTIAL ==  ((p_dsc->dpib->
                        ip4_replicate_partial->ipidbeh_ttlflag_df
                         & ROHC_PROF6_IP4_REP_IPIDBEH_MASK)>>2)) ||
                     (IP_ID_BEHAVIOR_RANDOM == ((p_dsc->dpib->
                        ip4_replicate_partial->ipidbeh_ttlflag_df
                         & ROHC_PROF6_IP4_REP_IPIDBEH_MASK)>>2)))
                 {
                     (p_dprm_update_blk->inner_ip4)->id = 
                         p_dsc->dpib->ip4_replicate_ipid->ip_id;
                 }
                 else if((IP_ID_BEHAVIOR_SEQUENTIAL_SWAPPED == 
                            ((p_dsc->dpib->ip4_replicate_partial->ipidbeh_ttlflag_df
                                 & ROHC_PROF6_IP4_REP_IPIDBEH_MASK)>>2)))
                 {
                        (p_dprm_update_blk->inner_ip4)->id = ROHC_HTONS(p_dsc->
                                dpib->ip4_replicate_ipid->ip_id);
                 }
                 else
                 {
                     (p_dprm_update_blk->inner_ip4)->id = 0x0000;
                 }

                 /*Update IP - ttl_hopl*/
                 if((p_dsc->dpib->ip4_replicate_partial->ipidbeh_ttlflag_df
                         & ROHC_PROF6_IP4_REP_TTLFLAG_MASK))
                     (p_dprm_update_blk->inner_ip4)->ttl =
                         p_dsc->dpib->ip4_replicate_ttlhopl->ttlhopl;

                 /*Update IP - dscp + ip ecn flags*/
                (p_dprm_update_blk->inner_ip4)->tos = p_dsc->dpib->
                         ip4_replicate_partial->dscp_ipecnflag;

                 /*Update IP - df*/
                if(p_dsc->dpib->ip4_replicate_partial->ipidbeh_ttlflag_df
                         & ROHC_PROF6_IP4_REP_DFFLAG_MASK)
                    (p_dprm_update_blk->inner_ip4)->off |= ROHC_HTONS(ROHC_IP4_DF);
                else
                {
                    (p_dprm_update_blk->inner_ip4)->off &=
                             (rohc_U16bit_t)~(ROHC_HTONS(ROHC_IP4_DF));
                }
                 /*update TCP*/
            break;


       case  ROHC_CO_P6_RND1_PKTYPE         :
       case  ROHC_CO_P6_RND2_PKTYPE         :
       case  ROHC_CO_P6_RND3_PKTYPE         :
       case  ROHC_CO_P6_RND4_PKTYPE         :
       case  ROHC_CO_P6_RND5_PKTYPE         :
       case  ROHC_CO_P6_RND6_PKTYPE         :
       case  ROHC_CO_P6_RND7_PKTYPE         :
           if (IP_ID_BEHAVIOR_ZERO != (p_dsc->ipid_beh_inner))
           {
               (p_dprm_update_blk->inner_ip4)->id = ROHC_HTONS(p_dsc->dpib->field_val
                   [ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE]);
           }
           break; 
       case  ROHC_CO_P6_RND8_PKTYPE         :
            if (IP_ID_BEHAVIOR_ZERO != (p_dsc->ipid_beh_inner))
            {
                (p_dprm_update_blk->inner_ip4)->id = ROHC_HTONS(p_dsc->dpib->field_val
                   [ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE]);
            }
            (p_dprm_update_blk->inner_ip4)->ttl = p_dsc->dpib->decoded_ttl;   
		    break;

       case  ROHC_CO_P6_SEQ1_PKTYPE         :    /*IPID*/
       case  ROHC_CO_P6_SEQ2_PKTYPE         : 
       case  ROHC_CO_P6_SEQ3_PKTYPE         :    /*IPID*/
       case  ROHC_CO_P6_SEQ4_PKTYPE         : 
       case  ROHC_CO_P6_SEQ5_PKTYPE         :    /*IPID*/
       case  ROHC_CO_P6_SEQ6_PKTYPE         :
       case  ROHC_CO_P6_SEQ7_PKTYPE         :    /*IPID*/
           {
               (p_dprm_update_blk->inner_ip4)->id = 
                    ROHC_HTONS(p_dsc->dpib->decoded_ipid);
           }
           break;
         
       case  ROHC_CO_P6_SEQ8_PKTYPE         : 
           {
               (p_dprm_update_blk->inner_ip4)->id = 
                ROHC_HTONS(p_dsc->dpib->decoded_ipid);
           }
           (p_dprm_update_blk->inner_ip4)->ttl = p_dsc->dpib->decoded_ttl;   
           break;

    }
    return;    
}

/*****************************************************************************
**      FUNCTION: rohcUpdateTcpHdrCoCommonPkt
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function updated tcp hdr for co-common pkt
**      ARGUMENTS:
**          	p_dsc - Pointer to DSC Context     
**          	p_dprm_update_blk - Pointer of Dprm Update Block
**		RETURNS:
**				Void
*****************************************************************************/
STATIC rohc_void_t rohcUpdateTcpHdrCoCommonPkt(
        struct rohc_dsc_tcp_t   * p_dsc ,
        rohc_dprm_update_blk_t  *p_dprm_update_blk)
{

    /*Updating the TCP Sequence Number*/
    if(p_dsc->dpib->field_val[ROHC_TCP_SEQ_IND_FID - ROHC_TCP_CTRL_FID_BASE])
    {
        p_dprm_update_blk->tcp->seq = ROHC_HTONL(p_dsc->dpib->decoded_sn);
    }

    /*Updating the TCP Acknolegement Number*/
    if(p_dsc->dpib->field_val[ROHC_TCP_ACK_IND_FID - ROHC_TCP_CTRL_FID_BASE])
    {
        p_dprm_update_blk->tcp->ack = ROHC_HTONL(p_dsc->dpib->decoded_ack_num);
    }

    /*Updating the TCP Window Size*/
    if(p_dsc->dpib->field_val[ROHC_TCP_WINDOW_IND_FID- ROHC_TCP_CTRL_FID_BASE])
    {
        p_dprm_update_blk->tcp->window = 
            ROHC_HTONS(p_dsc->dpib->field_val
                    [ROHC_TCP_WINDOW_FID- ROHC_VAR_FID_BASE]);
    }

    /*Updating the TCP Urgent Pointer*/
    if(p_dsc->dpib->field_val[ROHC_TCP_URG_PTR_PRESENT_FID - ROHC_TCP_CTRL_FID_BASE])
    {
        p_dprm_update_blk->tcp->urg_ptr =  ROHC_HTONS(p_dsc->dpib->field_val
                [ROHC_TCP_URG_PTR_FID-ROHC_VAR_FID_BASE]);
    }

    /*updating the TCP Flags*/
    /*URG FLAG*/
    if( p_dsc->dpib->field_val[ROHC_TCP_URG_FLAG_FID-ROHC_VAR_FID_BASE])
    {
        p_dprm_update_blk->tcp->flags |= ROHC_TCP_URG_MASK;
    }
    else
    {
        p_dprm_update_blk->tcp->flags &= ~(ROHC_TCP_URG_MASK);
    }

    /*ACK FLAG*/
    if( p_dsc->dpib->field_val[ROHC_TCP_ACK_FLAG_FID-ROHC_VAR_FID_BASE])
    {
        p_dprm_update_blk->tcp->flags |= ROHC_TCP_ACK_MASK;
    }
    else
    {
        p_dprm_update_blk->tcp->flags &= ~(ROHC_TCP_ACK_MASK);
    }

    /*PSH FLAG*/
    if( p_dsc->dpib->field_val[ROHC_TCP_PSH_FLAG_FID-ROHC_VAR_FID_BASE])
    {
        p_dprm_update_blk->tcp->flags |= ROHC_TCP_PSH_MASK;
    }
    else
    {
        p_dprm_update_blk->tcp->flags &= ~(ROHC_TCP_PSH_MASK);
    }
    /* This function updates RST/SYN/FIN flags of TCP header*/
    rohcProcessRsfFlag( p_dprm_update_blk,p_dsc);

    if(p_dsc->dpib->field_val[ROHC_TCP_LIST_PRESENT_FID - 
            ROHC_TCP_CTRL_FID_BASE])
    {
        rohcFormTcpOptionfromCompressList(p_dprm_update_blk , p_dsc); 
    }
}

/*****************************************************************************
**      FUNCTION: rohcUpdateTcpHdrIRP6CRPkt
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function updated tcp hdr for IR-CR
**      ARGUMENTS:
**          	p_dsc - Pointer to DSC Context     
**          	p_dprm_update_blk - Pointer of Dprm Update Block
**		RETURNS:
**				Void
*****************************************************************************/
STATIC rohc_void_t rohcUpdateTcpHdrIRP6CRPkt(
        struct rohc_dsc_tcp_t   * p_dsc ,
        rohc_dprm_update_blk_t  *p_dprm_update_blk)
{
    /*Updating the TCP Sequence Number*/
    ROHC_ASSIGN_4_OCTS((p_dsc->dpib->tcp_replicate_partial->seqnum),
            ((rohc_U8bit_t *)&p_dprm_update_blk->tcp->seq));

    /*Updating the TCP Acknolegement Number*/
    if(p_dsc->dpib->tcp_replicate_partial->
            ackp_urgp_urgf_ackf_pshf_rsf_ecnu & ROHC_REP_ACK_PRES_MASK)
    {
        ROHC_ASSIGN_4_OCTS((p_dsc->dpib->tcp_replicate_acknum->ack_num),
                ((rohc_U8bit_t *)&p_dprm_update_blk->tcp->ack));
    }

    /*Updating the TCP src port*/
    if(p_dsc->dpib->tcp_replicate_partial->winp_listp_srcprt_dstprt_asf &
            ROHC_REP_SRC_PORT_PRES_MASK )
    {
        if(0x01 == (p_dsc->dpib->tcp_replicate_partial->
                    winp_listp_srcprt_dstprt_asf & 
                    ROHC_REP_SRC_PORT_PRES_MASK)>>3)
        {
            p_dprm_update_blk->tcp->sport =
                ROHC_HTONS(p_dsc->dpib->decoded_sport);
        }
        else
        {
            p_dprm_update_blk->tcp->sport =
                ROHC_HTONS(ROHC_GET_2BYTES( p_dsc->dpib->
                            tcp_replicate_srcptr->src_port));
        }
    }

    /*Updating the TCP dst port*/
    if(p_dsc->dpib->tcp_replicate_partial->winp_listp_srcprt_dstprt_asf &
            ROHC_REP_DST_PORT_PRES_MASK )
    {
        if(0x01 == (p_dsc->dpib->tcp_replicate_partial->
                    winp_listp_srcprt_dstprt_asf & 
                    ROHC_REP_DST_PORT_PRES_MASK)>>1)
        {
            p_dprm_update_blk->tcp->dport =
                ROHC_HTONS(p_dsc->dpib->decoded_dport);
        }
        else
        {
            p_dprm_update_blk->tcp->dport =
                ROHC_HTONS(ROHC_GET_2BYTES( p_dsc->dpib->
                            tcp_replicate_dstptr->dst_port));
        }
    }

    /*Updating the TCP window */
    if(p_dsc->dpib->tcp_replicate_partial->winp_listp_srcprt_dstprt_asf &
            ROHC_REP_WIN_PRES_MASK)
    {
        p_dprm_update_blk->tcp->window =
            ROHC_HTONS(ROHC_GET_2BYTES( p_dsc->dpib->
                        tcp_replicate_win->win));
    }

    /*Updating the TCP Urg Ptr*/
    if(p_dsc->dpib->tcp_replicate_partial->ackp_urgp_urgf_ackf_pshf_rsf_ecnu &
            ROHC_REP_URP_PRES_MASK)
    {
        p_dprm_update_blk->tcp->urg_ptr =
            ROHC_GET_2BYTES( p_dsc->dpib->tcp_replicate_urg->urg_ptr);
    }

    /*Updating the TCP ecn_padding + res flags + ecn flags*/
    if(p_dsc->dpib->tcp_replicate_partial->ackp_urgp_urgf_ackf_pshf_rsf_ecnu &
            ROHC_REP_ECN_USED_MASK )
    {
        p_dprm_update_blk->tcp->offset |=
            ((p_dsc->dpib->tcp_replicate_ecn->ecn &
              ROHC_REP_TCP_RES_MASK) >> 2);
        p_dprm_update_blk->tcp->flags |=
            ((p_dsc->dpib->tcp_replicate_ecn->ecn &
              ROHC_REP_TCP_ECN_MASK) <<6 );
    }

    /*Updating the TCP checksum*/
    p_dprm_update_blk->tcp->checksum  =
        ROHC_GET_2BYTES( p_dsc->dpib->tcp_replicate_chksum->chksum);


    if(p_dsc->dpib->tcp_replicate_partial->winp_listp_srcprt_dstprt_asf &
            ROHC_REP_LIS_PRES_MASK )
    {
        rohcFormTcpOptionfromCompressList(p_dprm_update_blk , p_dsc);
    }
}

/*****************************************************************************
**      FUNCTION: rohcUpdateTcpHdrRND1RND2SEQ1SEQ2Pkt
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function updated tcp hdr for RND1-2 and SEQ1-2 pkt
**      ARGUMENTS:
**          	p_dsc - Pointer to DSC Context     
**          	p_dprm_update_blk - Pointer of Dprm Update Block
**		RETURNS:
**				Void
*****************************************************************************/
STATIC rohc_void_t rohcUpdateTcpHdrRND1RND2SEQ1SEQ2Pkt(
        struct rohc_dsc_tcp_t   * p_dsc ,
        rohc_dprm_update_blk_t  *p_dprm_update_blk)
{
    (p_dprm_update_blk->tcp)->seq =  ROHC_HTONL(p_dsc->dpib->decoded_sn);
    if ( p_dsc->dpib->field_val[ROHC_TCP_PSH_FLAG_FID-
            ROHC_VAR_FID_BASE] )
    {
        (p_dprm_update_blk->tcp)->flags |= ROHC_TCP_PSH_MASK;
    }
    else
    {
        p_dprm_update_blk->tcp->flags &= ~(ROHC_TCP_PSH_MASK);
    }
}

/*****************************************************************************
**      FUNCTION: rohcUpdateTcpHdrRND3RND4SEQ3SEQ4Pkt
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function updated tcp hdr for RND3-4 and SEQ3-4 pkt
**      ARGUMENTS:
**          	p_dsc - Pointer to DSC Context     
**          	p_dprm_update_blk - Pointer of Dprm Update Block
**		RETURNS:
**				Void
*****************************************************************************/
STATIC rohc_void_t rohcUpdateTcpHdrRND3RND4SEQ3SEQ4Pkt(
        struct rohc_dsc_tcp_t   * p_dsc ,
        rohc_dprm_update_blk_t  *p_dprm_update_blk)
{
    (p_dprm_update_blk->tcp)->ack = ROHC_HTONL(p_dsc->dpib->decoded_ack_num);
    if ( p_dsc->dpib->field_val[ROHC_TCP_PSH_FLAG_FID-
            ROHC_VAR_FID_BASE] )
    {
        (p_dprm_update_blk->tcp)->flags |= ROHC_TCP_PSH_MASK ;
    }
    else
    {
        p_dprm_update_blk->tcp->flags &= ~(ROHC_TCP_PSH_MASK);
    }
}

/*****************************************************************************
**      FUNCTION: rohcUpdateTcpHdrRND5RND6SEQ5SEQ6Pkt
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function updated tcp hdr for RND5-6 and SEQ5-6 pkt
**      ARGUMENTS:
**          	p_dsc - Pointer to DSC Context     
**          	p_dprm_update_blk - Pointer of Dprm Update Block
**		RETURNS:
**				Void
*****************************************************************************/
STATIC rohc_void_t rohcUpdateTcpHdrRND5RND6SEQ5SEQ6Pkt(
        struct rohc_dsc_tcp_t   * p_dsc ,
        rohc_dprm_update_blk_t  *p_dprm_update_blk)
{
    (p_dprm_update_blk->tcp)->seq = ROHC_HTONL(p_dsc->dpib->decoded_sn);
    (p_dprm_update_blk->tcp)->ack = ROHC_HTONL(p_dsc->dpib->decoded_ack_num);
    if ( p_dsc->dpib->field_val[ROHC_TCP_PSH_FLAG_FID-
            ROHC_VAR_FID_BASE])
    {        
        (p_dprm_update_blk->tcp)->flags |= ROHC_TCP_PSH_MASK;
    }
    else
    {
        p_dprm_update_blk->tcp->flags &= ~(ROHC_TCP_PSH_MASK);
    }
}

/*****************************************************************************
**      FUNCTION: rohcUpdateTcpHdrRND7Pkt
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function updated tcp hdr for RND7 pkt
**      ARGUMENTS:
**          	p_dsc - Pointer to DSC Context     
**          	p_dprm_update_blk - Pointer of Dprm Update Block
**		RETURNS:
**				Void
*****************************************************************************/
STATIC rohc_void_t rohcUpdateTcpHdrRND7Pkt(
        struct rohc_dsc_tcp_t   * p_dsc ,
        rohc_dprm_update_blk_t  *p_dprm_update_blk)
{
    (p_dprm_update_blk->tcp)->ack = ROHC_HTONL(p_dsc->dpib->decoded_ack_num);
    if ( p_dsc->dpib->field_val[ROHC_TCP_PSH_FLAG_FID-
            ROHC_VAR_FID_BASE] )
    {
        (p_dprm_update_blk->tcp)->flags |= ROHC_TCP_PSH_MASK;
    }
    else
    {
        p_dprm_update_blk->tcp->flags &= ~(ROHC_TCP_PSH_MASK);
    }
    (p_dprm_update_blk->tcp)->window = 
        ROHC_HTONS(p_dsc->dpib->field_val[ROHC_TCP_WINDOW_FID-
                ROHC_VAR_FID_BASE]);
}

/*****************************************************************************
**      FUNCTION: rohcUpdateTcpHdrSEQ7Pkt
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function updated tcp hdr for SEQ7 pkt
**      ARGUMENTS:
**          	p_dsc - Pointer to DSC Context     
**          	p_dprm_update_blk - Pointer of Dprm Update Block
**		RETURNS:
**				Void
*****************************************************************************/
STATIC rohc_void_t rohcUpdateTcpHdrSEQ7Pkt(
        struct rohc_dsc_tcp_t   * p_dsc ,
        rohc_dprm_update_blk_t  *p_dprm_update_blk)
{
    (p_dprm_update_blk->tcp)->ack = ROHC_HTONL(p_dsc->dpib->decoded_ack_num);
    if ( p_dsc->dpib->field_val[ROHC_TCP_PSH_FLAG_FID-
            ROHC_VAR_FID_BASE] )
    {
        (p_dprm_update_blk->tcp)->flags |= ROHC_TCP_PSH_MASK;
    }
    else
    {
        p_dprm_update_blk->tcp->flags &= ~(ROHC_TCP_PSH_MASK);
    }
    (p_dprm_update_blk->tcp)->window = 
        ROHC_HTONS(p_dsc->dpib->decoded_win);
}

/*****************************************************************************
**      FUNCTION: rohcUpdateTcpHdrRND8SEQ8Pkt
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function updated tcp hdr for RND8 and SEQ8 pkt
**      ARGUMENTS:
**          	p_dsc - Pointer to DSC Context     
**          	p_dprm_update_blk - Pointer of Dprm Update Block
**		RETURNS:
**				Void
*****************************************************************************/
STATIC rohc_void_t rohcUpdateTcpHdrRND8SEQ8Pkt(
        struct rohc_dsc_tcp_t   * p_dsc ,
        rohc_dprm_update_blk_t  *p_dprm_update_blk)
{
    (p_dprm_update_blk->tcp)->seq = ROHC_HTONL(p_dsc->dpib->decoded_sn);
    if ( p_dsc->dpib->field_val[ROHC_TCP_PSH_FLAG_FID-
            ROHC_VAR_FID_BASE])
    {    
        (p_dprm_update_blk->tcp)->flags |= ROHC_TCP_PSH_MASK; 
    }
    else
    {
        p_dprm_update_blk->tcp->flags &= ~(ROHC_TCP_PSH_MASK);
    }
    (p_dprm_update_blk->tcp)->ack = ROHC_HTONL(p_dsc->dpib->decoded_ack_num);

    /* This function updates RST/SYN/FIN flags of TCP header*/
    rohcProcessRsfFlag( p_dprm_update_blk,p_dsc);

    /* Processing Options field of RND8 and SEQ8 */
    if(p_dsc->dpib->field_val[ROHC_TCP_LIST_PRESENT_FID-
            ROHC_TCP_CTRL_FID_BASE])
    {
        rohcFormTcpOptionfromCompressList(p_dprm_update_blk , p_dsc); 
    }
}

/*****************************************************************************
**      FUNCTION: rohcFormTCPOptionsFromIrregChain 
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function form TCP Options from Irregular chain          
**      ARGUMENTS:
**          	p_dsc - Pointer to DSC Context     
**          	p_dprm_update_blk - Pointer of Dprm Update Block
**		RETURNS:
**				Void
*****************************************************************************/
STATIC rohc_void_t rohcFormTCPOptionsFromIrregChain(
                struct rohc_dsc_tcp_t   * p_dsc  ,
                rohc_dprm_update_blk_t  *p_dprm_update_blk )
{
    rohc_U8bit_t *pTcpOptOrig =  
        (((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + sizeof(rohc_tcp_t));
    rohc_U8bit_t *pTcpOpt = 
        (((rohc_U8bit_t *)(p_dprm_update_blk->tcp)) + sizeof(rohc_tcp_t));
    for ( ; ((TCP_EOL_OPT != *pTcpOpt) && 
                (((( p_dsc->tcp->offset >>4)*4) -20 ) !=
                 (pTcpOpt - pTcpOptOrig))) ; )
    {
        rohc_U8bit_t len = 0;
        /* Traverse each field of irregular chain */
        switch(*pTcpOpt)
        {  /*  */
            case TCP_MSS_OPT:   
            pTcpOpt+= 4;
            break;    
            case TCP_WS_OPT:
            pTcpOpt += 3;
            break;
            case TCP_SACK_PERMTD_OPT:
            pTcpOpt += 2;
            break;
            case TCP_NOP_OPT:
            pTcpOpt += 1;
            break;
            case TCP_SACK_OPT:
            if ( p_dsc->dpib->decodedSackBlockStart4 )
            {
                rohcProcessSackBlock4(p_dsc,p_dprm_update_blk,pTcpOpt,
                        &len);
                pTcpOpt += len;
            }
            /* Processing for SACK block 3*/
            else if ( p_dsc->dpib->decodedSackBlockStart3  )
            {
                rohcProcessSackBlock3(p_dsc,p_dprm_update_blk,pTcpOpt,
                        &len);
                pTcpOpt += len;
            }
            /* Processing for SACK block 2*/
            else if ( p_dsc->dpib->decodedSackBlockStart2  )
            {
                rohcProcessSackBlock2(p_dsc,p_dprm_update_blk,pTcpOpt,
                        &len);
                pTcpOpt += len;
            }
            /* Processing for SACK block 1*/
            else if ( p_dsc->dpib->decodedSackBlockStart1  )
            {
                rohcProcessSackBlock1(p_dsc,p_dprm_update_blk,pTcpOpt,
                        &len);
                pTcpOpt += len;
            }
            else
            {
                pTcpOpt++;
                pTcpOpt += *pTcpOpt -1;
            }
            break;   
            case TCP_TS_OPT:
            pTcpOpt += 2;
            rohcProcessTSOpt(p_dsc,pTcpOpt,&len);
            pTcpOpt += len;
            break;
            default:
            if(*pTcpOpt < ROHC_MAX_GEN_OPTION_SUPP)
            {
                if (!((p_dsc)->tcpOptGenStaticFlag & (0x01 << *pTcpOpt)))
                { 

                    if (!(p_dsc)->dpib->tcpOptGen[*pTcpOpt].genOptDisc )
                    {
                        ROHC_MEMCPY((pTcpOpt + 2)  
                                ,(((rohc_dsc_tcp_t*)p_dsc)->dpib->tcpOptGen
                                    [*pTcpOpt].optionString),
                                (*(pTcpOpt + 1) -2));
                    }
                }
                pTcpOpt++;
                pTcpOpt += *pTcpOpt -1;
            }
            break;     
        }/*end switch*/
    }/*end for*/
}

/*****************************************************************************
**      FUNCTION: rohcProcessSackBlock4
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function process sack block 4      
**      ARGUMENTS:
**          	p_dsc - Pointer to DSC Context     
**          	p_dprm_update_blk - Pointer of Dprm Update Block
**          	pTcpOpt - Pointer to tcp opt     
**          	p_len - Pointer to len parsed     
**		RETURNS:
**				Void
*****************************************************************************/
STATIC rohc_void_t rohcProcessSackBlock4(
                struct rohc_dsc_tcp_t   * p_dsc ,
                rohc_dprm_update_blk_t  *p_dprm_update_blk ,
                rohc_U8bit_t *pTcpOpt,
                rohc_U8bit_t *p_len)
{
    *p_len = 0;
    if ( 34 != *(pTcpOpt+1)  ) 
    {
        switch(*(pTcpOpt+1))
        {
            case 0x0a : 
                p_dprm_update_blk->tcp->offset += 0x80;
                break;
            case 0x12 : 
                p_dprm_update_blk->tcp->offset += 0x40;
                break;
            case 0x1a :
                p_dprm_update_blk->tcp->offset += 0x20;
        }
        ROHC_MEMMOVE((pTcpOpt+34 ) , 
                (pTcpOpt +  *(pTcpOpt+1)) ,
                ((((p_dsc->tcp->offset>>4))*4) - sizeof(rohc_tcp_t) -
                 ( (pTcpOpt +  *(pTcpOpt+1)) - (((rohc_U8bit_t *)
                                                 (p_dprm_update_blk->tcp)) + 
                                                sizeof(rohc_tcp_t))) ));
        *(pTcpOpt+1) = 34;
    }
    pTcpOpt += 2;
    *p_len += 2;
    *(((rohc_U32bit_t *)pTcpOpt)) = 
        ROHC_HTONL(p_dsc->dpib->decodedSackBlockStart1);  
    pTcpOpt +=4;   
    *p_len += 4;
    *(((rohc_U32bit_t *)pTcpOpt)) = 
        ROHC_HTONL(p_dsc->dpib->decodedSackBlockEnd1);     
    pTcpOpt +=4;   
    *p_len += 4;
    *(((rohc_U32bit_t *)pTcpOpt)) = 
        ROHC_HTONL(p_dsc->dpib->decodedSackBlockStart2);     
    pTcpOpt +=4;   
    *p_len += 4;
    *(((rohc_U32bit_t *)pTcpOpt)) = 
        ROHC_HTONL(p_dsc->dpib->decodedSackBlockEnd2);     
    pTcpOpt +=4;   
    *p_len += 4;
    *(((rohc_U32bit_t *)pTcpOpt)) = 
        ROHC_HTONL(p_dsc->dpib->decodedSackBlockStart3);     
    pTcpOpt +=4;   
    *p_len += 4;
    *(((rohc_U32bit_t *)pTcpOpt)) = 
        ROHC_HTONL(p_dsc->dpib->decodedSackBlockEnd3);     
    pTcpOpt +=4;   
    *p_len += 4;
    *(((rohc_U32bit_t *)pTcpOpt)) = 
        ROHC_HTONL(p_dsc->dpib->decodedSackBlockStart4);     
    pTcpOpt +=4;   
    *p_len += 4;
    *(((rohc_U32bit_t *)pTcpOpt)) = 
        ROHC_HTONL(p_dsc->dpib->decodedSackBlockEnd4);     
    pTcpOpt +=4;   
    *p_len += 4;
}

/*****************************************************************************
**      FUNCTION: rohcProcessSackBlock3
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function process sack block 3
**      ARGUMENTS:
**          	p_dsc - Pointer to DSC Context     
**          	p_dprm_update_blk - Pointer of Dprm Update Block
**          	pTcpOpt - Pointer to tcp opt     
**          	p_len - Pointer to len parsed    
**		RETURNS:
**				Void 
*****************************************************************************/
STATIC rohc_void_t rohcProcessSackBlock3(
                struct rohc_dsc_tcp_t   * p_dsc,
                rohc_dprm_update_blk_t  *p_dprm_update_blk ,
                rohc_U8bit_t *pTcpOpt,
                rohc_U8bit_t *p_len)
{
    *p_len = 0;
    if ( 26 != *(pTcpOpt+1)  )
    {
        switch(*(pTcpOpt+1))
        {
            case 0x0a : 
                p_dprm_update_blk->tcp->offset += 0x40;
                break;
            case 0x12 : 
                p_dprm_update_blk->tcp->offset += 0x20;
                break;
            case 0x22 : 
                p_dprm_update_blk->tcp->offset -= 0x20;
        }
        ROHC_MEMMOVE((pTcpOpt+26 ) , 
                (pTcpOpt +  *(pTcpOpt+1)) ,
                ((((p_dsc->tcp->offset>>4))*4) - sizeof(rohc_tcp_t) -
                 ( (pTcpOpt +  *(pTcpOpt+1)) - (((rohc_U8bit_t *)
                  (p_dprm_update_blk->tcp)) + sizeof(rohc_tcp_t))) ));
        *(pTcpOpt+1) = 26;
    }   
    pTcpOpt += 2;
    *p_len += 2;
    *(((rohc_U32bit_t *)pTcpOpt)) = 
        ROHC_HTONL(p_dsc->dpib->decodedSackBlockStart1);  
    pTcpOpt +=4;   
    *p_len += 4;
    *(((rohc_U32bit_t *)pTcpOpt)) = 
        ROHC_HTONL(p_dsc->dpib->decodedSackBlockEnd1);     
    pTcpOpt +=4;   
    *p_len += 4;
    *(((rohc_U32bit_t *)pTcpOpt)) = 
        ROHC_HTONL(p_dsc->dpib->decodedSackBlockStart2);     
    pTcpOpt +=4;   
    *p_len += 4;
    *(((rohc_U32bit_t *)pTcpOpt)) = 
        ROHC_HTONL(p_dsc->dpib->decodedSackBlockEnd2);     
    pTcpOpt +=4;   
    *p_len += 4;
    *(((rohc_U32bit_t *)pTcpOpt)) = 
        ROHC_HTONL(p_dsc->dpib->decodedSackBlockStart3);     
    pTcpOpt +=4;   
    *p_len += 4;
    *(((rohc_U32bit_t *)pTcpOpt)) = 
        ROHC_HTONL(p_dsc->dpib->decodedSackBlockEnd3);     
    pTcpOpt +=4;  
    *p_len += 4;
}

/*****************************************************************************
**      FUNCTION: rohcProcessSackBlock2
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function process sack block 2
**      ARGUMENTS:
**          	p_dsc - Pointer to DSC Context     
**          	p_dprm_update_blk - Pointer of Dprm Update Block
**          	pTcpOpt - Pointer to tcp opt     
**          	p_len - Pointer to len parsed    
**		RETURNS:
**				Void 
*****************************************************************************/
STATIC rohc_void_t rohcProcessSackBlock2(
                 struct rohc_dsc_tcp_t   * p_dsc,
                rohc_dprm_update_blk_t  *p_dprm_update_blk ,
                rohc_U8bit_t *pTcpOpt,
                rohc_U8bit_t *p_len)
{
    *p_len = 0;
    if ( 18 != *(pTcpOpt+1)  )
    {
        switch(*(pTcpOpt+1))
        {
            case 0x0a : 
                p_dprm_update_blk->tcp->offset += 0x20;
                break;
            case 0x1a : 
                p_dprm_update_blk->tcp->offset -= 0x20;
                break;
            case 0x22 : 
                p_dprm_update_blk->tcp->offset -= 0x40;
        }
        ROHC_MEMMOVE((pTcpOpt+18 ) , 
                (pTcpOpt +  *(pTcpOpt+1)) ,
                ((((p_dsc->tcp->offset>>4))*4) -
                 ( (pTcpOpt +  *(pTcpOpt+1)) - (((rohc_U8bit_t *)
                                                 (p_dprm_update_blk->tcp)) 
                                                + sizeof(rohc_tcp_t))) ));
        *(pTcpOpt + 1) = 18;
    }   
    pTcpOpt += 2;
    *p_len +=2;
    *(((rohc_U32bit_t *)pTcpOpt)) = 
        ROHC_HTONL(p_dsc->dpib->decodedSackBlockStart1);  
    pTcpOpt +=4;   
    *p_len +=4;
    *(((rohc_U32bit_t *)pTcpOpt)) = 
        ROHC_HTONL(p_dsc->dpib->decodedSackBlockEnd1);     
    pTcpOpt +=4;   
    *p_len +=4;
    *(((rohc_U32bit_t *)pTcpOpt)) = 
        ROHC_HTONL(p_dsc->dpib->decodedSackBlockStart2);     
    pTcpOpt +=4;   
    *p_len +=4;
    *(((rohc_U32bit_t *)pTcpOpt)) = 
        ROHC_HTONL(p_dsc->dpib->decodedSackBlockEnd2);     
    pTcpOpt +=4;   
    *p_len +=4;
}

/*****************************************************************************
**      FUNCTION: rohcProcessSackBlock1
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function process sack block 1
**      ARGUMENTS:
**          	p_dsc - Pointer to DSC Context     
**          	p_dprm_update_blk - Pointer of Dprm Update Block
**          	pTcpOpt - Pointer to tcp opt     
**          	p_len - Pointer to len parsed    
**		RETURNS:
**				Void 
*****************************************************************************/
STATIC rohc_void_t rohcProcessSackBlock1(
                struct rohc_dsc_tcp_t   * p_dsc,
                rohc_dprm_update_blk_t  *p_dprm_update_blk ,
                rohc_U8bit_t *pTcpOpt,
                rohc_U8bit_t *p_len)
{
    *p_len = 0;
    if ( 10 != *(pTcpOpt+1)  )
    {
        switch(*(pTcpOpt+1))
        {
            case 0x12 : 
                p_dprm_update_blk->tcp->offset -= 0x20;
                break;
            case 0x1a : 
                p_dprm_update_blk->tcp->offset -= 0x40;
                break;
            case 0x22 : 
                p_dprm_update_blk->tcp->offset -= 0x80;
        }
        ROHC_MEMMOVE((pTcpOpt+10 ) , 
                (pTcpOpt +  *(pTcpOpt+1)) ,
                ((((p_dsc->tcp->offset>>4))*4) - sizeof(rohc_tcp_t) -
                 ( (pTcpOpt +  *(pTcpOpt+1)) - (((rohc_U8bit_t *)
                  (p_dprm_update_blk->tcp)) + sizeof(rohc_tcp_t))) ));
        *(pTcpOpt+1) = 10;
    }   
    pTcpOpt += 2;
    *p_len +=2;
    *(((rohc_U32bit_t *)pTcpOpt)) = 
        ROHC_HTONL(p_dsc->dpib->decodedSackBlockStart1);  
    pTcpOpt +=4;   
    *p_len +=4;
    *(((rohc_U32bit_t *)pTcpOpt)) = 
        ROHC_HTONL(p_dsc->dpib->decodedSackBlockEnd1);     
    pTcpOpt +=4;   
    *p_len +=4;
}

/*****************************************************************************
**      FUNCTION:  rohcProcessTSOpt
**              
******************************************************************************
**
**      DESCRIPTION:
**              This function process ts option
**      ARGUMENTS:
**          	p_dsc - Pointer to DSC Context     
**          	pTcpOpt - Pointer to tcp opt     
**          	p_len - Pointer to len parsed    
**		RETURNS:
**				Void 
*****************************************************************************/
STATIC rohc_void_t rohcProcessTSOpt(
                struct rohc_dsc_tcp_t   * p_dsc ,
                rohc_U8bit_t *pTcpOpt,
                rohc_U8bit_t *p_len)
{
    if ( p_dsc->dpib->field_len[ROHC_TS_FID - ROHC_VAR_FID_BASE] )
    {
        *((rohc_U32bit_t *)pTcpOpt) = ROHC_HTONL(p_dsc->dpib->decoded_ts); 
        pTcpOpt +=4;
        *p_len +=4;
    }   

    if ( p_dsc->dpib->field_len[ROHC_TCP_TS_ECHO_FID - ROHC_VAR_FID_BASE] )
    {
        *((rohc_U32bit_t *)pTcpOpt) = ROHC_HTONL(
                p_dsc->dpib->decodedTsEchoVal); 
        pTcpOpt +=4;
        *p_len +=4;
    }   
}
