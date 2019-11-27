/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_c_pbm.c,v 1.6.32.12 2010/11/09 07:21:31 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains C-PBM packet encoding routines.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_c_pbm.c,v $
 * Revision 1.6.32.12  2010/11/09 07:21:31  gur22059
 * Removed Warning while calling rohc_make_rtp_dyn
 *
 * Revision 1.6.32.11  2010/11/09 05:22:24  gur22059
 * Segregation of code w.r.t profile 1
 *
 * Revision 1.6.32.10  2010/10/13 03:54:31  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.6.32.9  2010/09/23 05:18:50  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.6.32.8  2010/09/10 10:09:55  gur22059
 * Locality of reference for better performamce
 *
 * Revision 1.6.32.7  2010/08/30 08:22:14  gur22059
 * Removed enhancement that was done to bypass msgInsert
 *
 * Revision 1.6.32.6  2010/08/26 05:41:41  gur22059
 * Enhancement for removing msgInsert ZCB interface from compressor
 *
 * Revision 1.6.32.5  2010/08/13 05:53:29  gur22059
 * SPR 567: Corrected the creation of RTP IR-DYN packets
 *
 * Revision 1.6.32.2  2010/07/08 09:37:52  gur21145
 * code review comments of prof 0 , 2 enhancement activity
 *
 * Revision 1.6.32.1  2010/07/07 11:33:23  gur21145
 * PROFILE2_comments_and PROFILE0_comments_inc
 *
 * Revision 1.6  2009/05/28 13:50:10  gur19140
 * *** empty log message ***
 *
 * Revision 1.5  2009/05/28 03:35:55  gur19836
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
#include "rohc_c_pbm.h"
#include "rohc_ed_ext_if.h"
#include "rohc_crc_ext_if.h"
#include "rohc_gcom_mcr.h"
#include "rohc_main_ex.h"
#include "rohc_stats_def.h"
#include "lteFramework.h"
#include "rohc_opt_proto.h"
#include "rohc_db_ext_if.h"
#include "rohc_led_mcr.h"
#include "rohc_cprm_def_p6.h"
#include "rohc_ed_def.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/
/* structure used to describe ROHC general packet*/
struct rohc_pbm_pkt_field_info_t   rohc_c_pkt_type[ROHC_MAX_PKT_TYPES]
                                    [ROHC_MAX_NO_OF_FIELDS_IN_A_PACKET];

struct rohc_pbm_pkt_field_info_t   rohc_c_irreg_chain_type[ROHC_MAX_IRREG_CHAINS]
                                            [ROHC_MAX_NO_OF_FIELDS_IN_A_IRREG_CHAIN];

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* + SPR 17439 */                                            
static void
rohc_make_p6_tcp_options_irregular_chain
    ROHC_PROTO((
    struct rohc_entity_t  * p_rohc_entity,
    rohc_U8bit_t    * p_rohc_pkt ,
    struct rohc_csc_tcp_t   * p_csc,
    /* +- SPR 17777 */
    rohc_U8bit_t    *p_len_tcp_opt_irrg));
static rohc_return_t 
rohc_make_p6_IR_pkts
    ROHC_PROTO((
    rohc_U8bit_t            * p_location,
    struct rohc_csc_tcp_t   * p_csc,
    rohc_U8bit_t            pkt_type,
    struct rohc_entity_t    * p_entity,
    rohc_U8bit_t            *cid_len ,
    rohc_U32bit_t           *p_len  ,
    rohc_error_code_t       *p_ecode));
STATIC rohc_return_t 
rohc_make_p2_IR_pkts
    ROHC_PROTO((
    rohc_U8bit_t            * p_location,
    struct rohc_csc_udp_t   * p_csc,
    rohc_U8bit_t            pkt_type,
    struct rohc_entity_t    * p_entity,
    rohc_U8bit_t            *cid_len,
    rohc_U32bit_t           *p_len,
    rohc_error_code_t       *p_ecode));
STATIC rohc_return_t 
rohc_make_p1_IR_pkts
    ROHC_PROTO((
    rohc_U8bit_t            * p_location,
    struct rohc_csc_rtp_t   * p_csc,
    rohc_U8bit_t            pkt_type,
    struct rohc_entity_t    * p_entity,
    rohc_U8bit_t            *cid_len ,
    rohc_U32bit_t           *p_len,  
    rohc_error_code_t       *p_ecode));
STATIC rohc_return_t 
rohc_make_uncomp_IRP0_pkt_typ
    ROHC_PROTO((
    rohc_U8bit_t               * p_location,
    struct rohc_csc_uncomp_t   * p_csc,    
    rohc_U8bit_t               pkt_type,  
    struct rohc_entity_t       * p_entity,
    rohc_U8bit_t               *cid_len ,
    rohc_U32bit_t              *p_len,  
    rohc_U8bit_t               *p_in_buff));
static rohc_U8bit_t
rohc_p6_inner_ip_irreg_chain_type
    ROHC_PROTO((
    struct rohc_csc_tcp_t   * p_csc  ));
static void
rohc_make_p6_tcp_irregular_chain
    ROHC_PROTO((
    rohc_U8bit_t    ** p_p_rohc_pkt,
    struct rohc_csc_tcp_t   * p_csc,
    rohc_U8bit_t irreg_chain_type));
static void
rohc_make_p6_inner_ip_irregular_chain
    ROHC_PROTO((
    rohc_U8bit_t    * * p_p_rohc_pkt,
    struct rohc_csc_tcp_t   * p_csc));
    /* +- SPR 17777 */
static rohc_return_t rohc_p1_pbm_fill_pkt_bits ROHC_PROTO(( 
        struct rohc_entity_t    * p_entity              ,
        struct rohc_csc_rtp_t   * p_csc                 ,
        rohc_U8bit_t            ** p_rohc_pkt            ,
        rohc_U8bit_t              pkt_type                ,
        rohc_error_code_t       * p_ecode));

static rohc_return_t rohc_p2_pbm_fill_pkt_bits ROHC_PROTO(( 
        struct rohc_entity_t    * p_entity              ,
        struct rohc_csc_udp_t   * p_csc                 ,
        rohc_U8bit_t            ** p_rohc_pkt            ,
        rohc_U8bit_t              pkt_type                ,
        rohc_error_code_t       * p_ecode));

static rohc_return_t rohc_p6_pbm_fill_pkt_bits ROHC_PROTO(( 
        struct rohc_entity_t    * p_entity              ,
        struct rohc_csc_tcp_t   * p_csc                 ,
        rohc_U8bit_t            ** p_rohc_pkt            ,
        rohc_U8bit_t              pkt_type ));

static void
rohc_set_ts_opt_bits ROHC_PROTO((
    rohc_encoded_value_t    *p_en_val            ,
    rohc_U8bit_t            *p_rohc_pkt          ,
    rohc_U8bit_t            *p_len ));

static void
rohc_fill_sack_opt ROHC_PROTO((
    struct rohc_csc_tcp_t   * p_csc         ,
    rohc_U8bit_t            *p_rohc_pkt     ,
    rohc_U8bit_t            *p_len ));

static void
rohc_fill_generic_opt ROHC_PROTO((
            struct rohc_csc_tcp_t   * p_csc         ,
            rohc_U8bit_t            *p_rohc_pkt     ,
            rohc_U8bit_t            *p_len          ,
            rohc_U8bit_t            kind ));
/* + SPR 17439 */
STATIC void rohc_c_make_r1p23_pkt (rohc_U8bit_t pkt_type);
STATIC void rohc_c_make_uo1p23_pkt(rohc_U8bit_t pkt_type);
STATIC void rohc_c_make_uor2p23_pkt (rohc_U8bit_t pkt_type);
STATIC void rohc_c_make_r1p1_pkt (rohc_U8bit_t pkt_type);
STATIC void rohc_c_make_r1idp1_pkt (rohc_U8bit_t pkt_type);
STATIC void rohc_c_make_r1tsp1_pkt (rohc_U8bit_t pkt_type);
STATIC void rohc_c_make_uo1idp1_pkt(rohc_U8bit_t pkt_type);
STATIC void rohc_c_make_uor2p1_pkt (rohc_U8bit_t pkt_type);
STATIC void rohc_c_make_uor2idp1_pkt (rohc_U8bit_t pkt_type);
STATIC void rohc_c_make_uor2tsp1_pkt (rohc_U8bit_t pkt_type);
STATIC void rohc_c_make_ext0_tsp1_pkt (rohc_U8bit_t pkt_type, rohc_U8bit_t index);
STATIC void rohc_c_make_ext0_p23_pkt (rohc_U8bit_t pkt_type , rohc_U8bit_t index);
STATIC void rohc_c_make_ext1_p23_pkt (rohc_U8bit_t pkt_type , rohc_U8bit_t index);
STATIC void rohc_c_make_ext2_p23_pkt (rohc_U8bit_t pkt_type , rohc_U8bit_t index);
STATIC void rohc_c_make_ext3p23_pkt (rohc_U8bit_t pkt_type , rohc_U8bit_t index);
STATIC void rohc_c_make_ext0_idp1_pkt (rohc_U8bit_t pkt_type ,rohc_U8bit_t index);
STATIC void rohc_c_make_ext1_t1p1_pkt (rohc_U8bit_t pkt_type ,rohc_U8bit_t index);
STATIC void rohc_c_make_ext1_t0p1_pkt (rohc_U8bit_t pkt_type ,rohc_U8bit_t index);
STATIC void rohc_c_make_ext1_notp1_pkt(rohc_U8bit_t pkt_type ,rohc_U8bit_t index);
STATIC void rohc_c_make_ext2_t1p1_pkt (rohc_U8bit_t pkt_type ,rohc_U8bit_t index);
STATIC void rohc_c_make_ext2_t0p1_pkt (rohc_U8bit_t pkt_type ,rohc_U8bit_t index);
STATIC void rohc_c_make_ext2_notp1_pkt(rohc_U8bit_t pkt_type ,rohc_U8bit_t index);
STATIC void rohc_c_make_ext3p1_pkt (rohc_U8bit_t pkt_type , rohc_U8bit_t index);
STATIC void
rohc_set8bit
        (rohc_U8bit_t * * to ,
         rohc_U8bit_t * p_to_bit_pos ,
         rohc_U8bit_t from ,
         rohc_U8bit_t n);
STATIC void
rohc_setbits
        (rohc_U8bit_t * * to ,
         rohc_U8bit_t * p_to_bit_pos ,
         rohc_U32bit_t from ,
         rohc_U8bit_t from_bit_offset_in_32bit ,
         /* from_bit_offset_in_32bit is from
          * L.S.B. */
         rohc_U8bit_t no_of_bits);
static void
rohc_encode_rsf_field
        (rohc_U8bit_t * * p_p_rohc_pkt ,
         rohc_csc_tcp_t  *p_csc      ,
         rohc_U8bit_t * p_to_bit_pos
         );

static rohc_return_t
rohc_c_prof6_calc_bits
        (rohc_U32bit_t  no_of_bits);
static void
rohc_p6_variable_length_32_enc
        (rohc_U8bit_t            ** p_p_rohc_pkt ,
         rohc_U8bit_t            * p_to_bit_pos  ,
         rohc_U8bit_t            field_id        ,
         rohc_csc_tcp_t  * p_csc);
static void
rohc_p6_static_or_irregular_enc
        (rohc_U8bit_t            ** p_p_rohc_pkt ,
         rohc_U8bit_t            * p_to_bit_pos  ,
         rohc_U8bit_t            field_id        ,
         struct  rohc_csc_tcp_t  * p_csc);

static void
rohc_p6_opt_ip_id_lsb_enc(void);
/* +- SPR 17777 */

static rohc_return_t
rohc_tcp_opt_list_compressed_pkt
        (rohc_entity_t   * p_entity   ,
         rohc_U8bit_t            * p_rohc_pkt ,
         rohc_csc_tcp_t  * p_csc      ,
         rohc_U32bit_t  *p_length);
STATIC void 
rohc_add_ele_p1_ext3flagsp1
        (rohc_U8bit_t            ** p_p_rohc_pkt ,
        rohc_U8bit_t            * p_ext_flag    ,
        rohc_U8bit_t            * p_to_bit_pos  ,
        rohc_U8bit_t            field_id        ,
        struct  rohc_csc_rtp_t  * p_csc         ,
        rohc_U8bit_t            pkt_type);
STATIC void
rohc_add_ele_p2_ext3flagsp1
        (rohc_U8bit_t           ** p_p_rohc_pkt ,
        rohc_U8bit_t           * p_ext_flag    ,
        rohc_U8bit_t           * p_to_bit_pos  ,
        rohc_U8bit_t           field_id        ,
        struct  rohc_csc_udp_t * p_csc);
rohc_return_t
rohc_add_ele_p1_inneripflagsp1
        (rohc_U8bit_t            ** p_p_rohc_pkt ,
        rohc_U8bit_t            * p_to_bit_pos  ,
        rohc_U8bit_t            field_id        ,
        struct  rohc_csc_rtp_t  * p_csc         ,
        struct rohc_entity_t    * p_entity,
        rohc_error_code_t       *p_ecode);

rohc_return_t
        rohc_add_ele_p2_inneripflagsp1
            (rohc_U8bit_t            ** p_p_rohc_pkt ,
            rohc_U8bit_t            * p_to_bit_pos  ,
            rohc_U8bit_t            field_id        ,
            struct  rohc_csc_udp_t  * p_csc         ,
            struct rohc_entity_t    * p_entity      ,
            rohc_error_code_t       *p_ecode);
rohc_return_t
rohc_add_ele_p1_outeripflagsp1
        (rohc_U8bit_t            ** p_p_rohc_pkt ,
        rohc_U8bit_t            * p_to_bit_pos  ,
        rohc_U8bit_t            field_id        ,
        struct  rohc_csc_rtp_t  * p_csc         ,
        struct rohc_entity_t    * p_entity      ,
        rohc_error_code_t       *p_ecode);
rohc_return_t
        rohc_add_ele_p2_outeripflagsp1
            (rohc_U8bit_t    ** p_p_rohc_pkt ,
            rohc_U8bit_t    * p_to_bit_pos ,
            rohc_U8bit_t    field_id ,
            struct  rohc_csc_udp_t  * p_csc ,
            struct rohc_entity_t * p_entity ,
            rohc_error_code_t   *p_ecode);

rohc_return_t
rohc_add_ele_p1_rtpflagsp1
            (rohc_U8bit_t            ** p_p_rohc_pkt ,
            rohc_U8bit_t            * p_to_bit_pos  ,
            rohc_U8bit_t            field_id        ,
            struct  rohc_csc_rtp_t  * p_csc         ,
            struct rohc_entity_t    * p_entity      ,
            rohc_error_code_t       *p_ecode);
        STATIC void
        rohc_make_p1_ip4_static
                (rohc_ip4_static_t   *p_location ,
                rohc_csc_rtp_t      *p_csc      ,
                rohc_U8bit_t        protocol);
        STATIC void
        rohc_make_p2_ip4_static
                (rohc_ip4_static_t  *p_location ,
                rohc_csc_udp_t     *p_csc      ,
                rohc_U8bit_t       protocol);
        static void
        rohc_make_p6_ip4_static
                (rohc_ip4_static_t  *p_location ,
                rohc_csc_tcp_t     *p_csc      ,
                rohc_U8bit_t       protocol);
        STATIC void
        rohc_make_p1_ip6_static
                (rohc_ip6_static_t   * p_location ,
                rohc_csc_rtp_t      * p_csc      ,
                rohc_U8bit_t        protocol);
        STATIC void
        rohc_make_p2_ip6_static
                (rohc_ip6_static_t * p_location ,
                rohc_csc_udp_t    * p_csc      ,
                rohc_U8bit_t      protocol);
        STATIC void
        rohc_make_p1_udp_static
                (rohc_udp_static_t   * p_location ,
                rohc_csc_rtp_t      * p_csc );
        STATIC void
        rohc_make_p2_udp_static
                (rohc_udp_static_t * p_location ,
                rohc_csc_udp_t * p_csc );
        STATIC void
        rohc_make_p1_rtp_static
                (rohc_U8bit_t    * p_location ,
                rohc_csc_rtp_t  * p_csc );
        static void
        rohc_make_p6_tcp_static
                (rohc_tcp_static_t   * p_location ,
                rohc_csc_tcp_t      * p_csc );
STATIC rohc_return_t
rohc_make_p1_ip4_dyn
        (struct rohc_ip4_dyn_t   * p_location    ,
         struct rohc_csc_rtp_t   * p_csc         ,
         rohc_U8bit_t            ip_type         ,
         struct rohc_entity_t    * p_entity      ,
         rohc_U32bit_t           *p_len          ,
         rohc_error_code_t       *p_ecode);
                
STATIC rohc_return_t
rohc_make_p2_ip4_dyn
               (struct rohc_ip4_dyn_t * p_location ,
               struct rohc_csc_udp_t * p_csc      ,
               rohc_U8bit_t          ip_type      ,
               struct rohc_entity_t  * p_entity   ,
               rohc_U32bit_t         *p_len       ,
               rohc_error_code_t     *p_ecode);
static rohc_return_t
rohc_make_p6_ip4_dyn
        (rohc_U8bit_t   * p_location    ,
        struct rohc_csc_tcp_t   * p_csc         ,
        rohc_U8bit_t            ip_type         ,
        /* +- SPR 17777 */
        rohc_U32bit_t           *p_len);
/* SPR 20913 Start */                   
STATIC rohc_return_t
/* SPR 20913 End */
rohc_make_p1_ip6_dyn
       (struct rohc_ip6_dyn_t   * p_location ,
        struct rohc_csc_rtp_t   * p_csc      ,
        rohc_U8bit_t            ip_type      ,
        struct rohc_entity_t    * p_entity   ,
        rohc_U32bit_t           *p_len       ,
        rohc_error_code_t       *p_ecode);

/* SPR 20913 Start */                   
STATIC rohc_return_t
/* SPR 20913 End */                   
rohc_make_p2_ip6_dyn
       (struct rohc_ip6_dyn_t * p_location ,
        struct rohc_csc_udp_t * p_csc      ,
       rohc_U8bit_t          ip_type      ,
       struct rohc_entity_t  * p_entity   ,
       rohc_U32bit_t         *p_len       ,
       rohc_error_code_t     *p_ecode);

/* SPR 20913 Start */                   
STATIC void
/* SPR 20913 End */                   
rohc_make_p1_udp_dyn
      (rohc_U8bit_t    * p_location ,
       rohc_csc_rtp_t  * p_csc );


/* SPR 20913 Start */                   
STATIC void
/* SPR 20913 End */                   
rohc_make_p2_udp_dyn
        ( rohc_U8bit_t * p_location ,
         rohc_csc_udp_t * p_csc );

/* SPR 20913 Start */                   
STATIC rohc_return_t
/* SPR 20913 End */                   
rohc_make_p1_rtp_dyn
         (rohc_U8bit_t            * p_location    ,
          struct rohc_csc_rtp_t   * p_csc         ,
          struct rohc_entity_t    * p_entity      ,
          rohc_U32bit_t           *p_len          ,
          rohc_error_code_t       *p_ecode);
static rohc_return_t
rohc_make_p6_tcp_dyn
           (struct rohc_tcp_dyn_partial_t     * p_location    ,
            struct rohc_csc_tcp_t   * p_csc         ,
            rohc_U32bit_t           *p_len);
/* +- SPR 17777 */
static void
rohc_make_p6_tcp_dyn_ack
          (rohc_U8bit_t    * p_location ,
           rohc_csc_tcp_t  * p_csc );
static void
rohc_make_p6_tcp_dyn_urgptr
          (rohc_U8bit_t    * p_location ,
          rohc_csc_tcp_t  * p_csc );
static void
rohc_make_p6_tcp_dyn_ackstride
          (rohc_U8bit_t    * p_location ,
                        rohc_csc_tcp_t  * p_csc );
static void
rohc_make_p6_tcp_dyn_wind_cksum
           (rohc_tcp_dyn_windncksum_t   * p_location ,
                        rohc_csc_tcp_t  * p_csc );

/* SPR 20913 Start */                   
STATIC rohc_return_t
/* SPR 20913 End */                   
rohc_make_p1_dyn_chain
        (rohc_U8bit_t            * p_location    ,
        struct rohc_csc_rtp_t   * p_csc         ,
        struct rohc_entity_t    * p_entity      ,
        rohc_U32bit_t           *p_len          ,
        rohc_error_code_t       *p_ecode);

STATIC rohc_return_t
rohc_make_p2_dyn_chain
        (rohc_U8bit_t            * p_location ,
         struct rohc_csc_udp_t   * p_csc      ,
         struct rohc_entity_t    * p_entity   ,
         rohc_U32bit_t           *p_len       ,
         rohc_error_code_t       *p_ecode);
static rohc_return_t
rohc_make_p6_dyn_chain
         (rohc_U8bit_t            * p_location    ,
          struct rohc_csc_tcp_t   * p_csc         ,
          struct rohc_entity_t    * p_entity      ,
          rohc_U32bit_t           *p_len          ,
          rohc_error_code_t       *p_ecode);

/* SPR 20913 Start */                   
STATIC rohc_U32bit_t
/* SPR 20913 End */                   
rohc_make_p1_static_chain
          (rohc_U8bit_t            * p_location ,
          struct rohc_csc_rtp_t   * p_csc);
    /* +- SPR 17777 */
/* SPR 20913 Start*/
STATIC rohc_U32bit_t
/* SPR 20913 End */                   
rohc_make_p2_static_chain
          (rohc_U8bit_t            * p_location ,
          struct rohc_csc_udp_t   * p_csc);
    /* +- SPR 17777 */
static rohc_U32bit_t
rohc_make_p6_static_chain
          (rohc_U8bit_t            * p_location ,
           struct rohc_csc_tcp_t   * p_csc);
    /* +- SPR 17777 */
rohc_return_t
rohc_make_p0_rohc_compr_pkt
             (struct rohc_entity_t       * p_entity ,
              struct rohc_csc_uncomp_t   * p_csc ,
              rohc_U8bit_t        pkt_type ,
              rohc_U8bit_t        * p_in_buff ,
              rohc_U32bit_t       * p_pkt_len ,
              rohc_U8bit_t        * p_rohc_pkt ,
              rohc_U8bit_t        * p_len_skip_from_pl);
    /* +- SPR 17777 */
rohc_return_t
rohc_make_p1_rohc_compr_pkt
              (struct rohc_entity_t    * p_entity              ,
               struct rohc_csc_rtp_t   * p_csc                 ,
               rohc_U8bit_t            pkt_type                ,
               rohc_U8bit_t            * p_in_buff             ,
               rohc_U32bit_t           * p_pkt_len             ,
               rohc_U8bit_t            * p_rohc_pkt            ,
               rohc_U8bit_t            * p_len_skip_from_pl    ,
               rohc_error_code_t       * p_ecode);
static rohc_return_t
rohc_p1_pbm_fill_pkt_bits
               (struct rohc_entity_t    * p_entity              ,
               struct rohc_csc_rtp_t   * p_csc                 ,
               rohc_U8bit_t            ** p_rohc_pkt            ,
               rohc_U8bit_t              pkt_type                ,
               rohc_error_code_t       * p_ecode);
rohc_return_t
rohc_make_p2_rohc_compr_pkt
              (struct rohc_entity_t  * p_entity           ,
               struct rohc_csc_udp_t * p_csc              ,
               rohc_U8bit_t          pkt_type             ,
               rohc_U8bit_t          * p_in_buff          ,
               rohc_U32bit_t         * p_pkt_len          ,
               rohc_U8bit_t          * p_rohc_pkt         ,
               rohc_U8bit_t          * p_len_skip_from_pl ,
               rohc_error_code_t     * p_ecode);
static rohc_return_t
rohc_p2_pbm_fill_pkt_bits
               (struct rohc_entity_t    * p_entity              ,
               struct rohc_csc_udp_t   * p_csc                 ,
               rohc_U8bit_t            ** p_rohc_pkt            ,
               rohc_U8bit_t              pkt_type                ,
               rohc_error_code_t       * p_ecode);

/* - SPR 17439 */
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
rohc_U32bit_t rohc_global_pkt_types_to_stats_pr[ROHC_MAX_PKT_TYPES];

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
static rohc_U32bit_t rohc_global_constant_field_values[ROHC_MAX_CONSTANT_FID];
static rohc_U32bit_t rohc_global_pkt_types_to_stats_cr[ROHC_MAX_PKT_TYPES];
rohc_U8bit_t eol_list_present;
rohc_U8bit_t mss_list_present;
rohc_U8bit_t ws_list_present;

rohc_U8bit_t gen_opt_list_present;

rohc_U8bit_t eol_opt_present;
rohc_U8bit_t mss_opt_present;
rohc_U8bit_t ws_opt_present;

rohc_U8bit_t sack_opt_present;
rohc_U8bit_t alt_cksum_req_opt_present;

rohc_U8bit_t rohc_c_encode_sdvl_ts
(  rohc_U8bit_t *dest ,
   rohc_U32bit_t value ,
   rohc_U32bit_t length);


/*****************************************************************************
**      FUNCTION:
**              rohc_fill_global_pkt_types_to_stats
******************************************************************************
**
**      DESCRIPTION:
**              Initializes the globally pkt types to stats
**      ARGUMENTS:
**               None
**      RETURN VALUE:
**              void
**      NOTES:
**
*****************************************************************************/
STATIC void
rohc_fill_global_pkt_types_to_stats
    ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_fill_global_pkt_types_to_stats");
    /* for created stats */
    rohc_global_pkt_types_to_stats_cr[ROHC_R0P1_PKTTYPE]= 
                                                ROHC_NO_OF_R_0_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_R0CRCP1_PKTTYPE] =
                                                ROHC_NO_R_0_CRC_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_UO0P1_PKTTYPE     ]=     
                                                ROHC_NO_UO_0_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_R1P1_PKTTYPE      ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_R1E0P1_PKTTYPE    ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_R1E1P1_PKTTYPE    ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_R1E2P1_PKTTYPE    ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_R1E3P1_PKTTYPE    ]=     
                                                ROHC_NO_OF_R_1_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_R1IDP1_PKTTYPE    ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_R1IDE0P1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_R1IDE1P1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_R1IDE2P1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_R1IDE3P1_PKTTYPE  ]=     
                                                ROHC_NO_R_1_ID_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_R1TSP1_PKTTYPE    ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_R1TSE0P1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_R1TSE1P1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_R1TSE2P1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_R1TSE3P1_PKTTYPE  ]=     
                                                ROHC_NO_R_1_TS_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_UO1P1_PKTTYPE     ]=     
                                                ROHC_NO_OF_UO_1_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_UO1IDP1_PKTTYPE   ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_UO1IDE0P1_PKTTYPE ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_UO1IDE1P1_PKTTYPE ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_UO1IDE2P1_PKTTYPE ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_UO1IDE3P1_PKTTYPE ]=     
                                                ROHC_NO_UO_1_ID_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_UO1TSP1_PKTTYPE   ]=     
                                                ROHC_NO_UO_1_TS_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_UOR2P1_PKTTYPE    ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_UOR2E0P1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_UOR2E1P1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_UOR2E2P1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_UOR2E3P1_PKTTYPE  ]=     
                                                ROHC_NO_OF_UOR_2_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_UOR2IDP1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_UOR2IDE0P1_PKTTYPE]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_UOR2IDE1P1_PKTTYPE]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_UOR2IDE2P1_PKTTYPE]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_UOR2IDE3P1_PKTTYPE]=     
                                                ROHC_NO_UOR_2_ID_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_UOR2TSP1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_UOR2TSE0P1_PKTTYPE]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_UOR2TSE1P1_PKTTYPE]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_UOR2TSE2P1_PKTTYPE]=     
    rohc_global_pkt_types_to_stats_cr[ROHC_UOR2TSE3P1_PKTTYPE]=     
                                                ROHC_NO_UOR_2_ID_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_R1P23_PKTTYPE          ]=
    rohc_global_pkt_types_to_stats_cr[ROHC_R1E0P23_PKTTYPE        ]=
    rohc_global_pkt_types_to_stats_cr[ROHC_R1E1P23_PKTTYPE        ]=
    rohc_global_pkt_types_to_stats_cr[ROHC_R1E2P23_PKTTYPE        ]=
    rohc_global_pkt_types_to_stats_cr[ROHC_R1E3P23_PKTTYPE        ]=
                                                ROHC_NO_OF_R_1_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_UO1P23_PKTTYPE         ]=
                                                ROHC_NO_OF_UO_1_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_UOR2P23_PKTTYPE        ]=
    rohc_global_pkt_types_to_stats_cr[ROHC_UOR2E0P23_PKTTYPE      ]=
    rohc_global_pkt_types_to_stats_cr[ROHC_UOR2E1P23_PKTTYPE      ]=
    rohc_global_pkt_types_to_stats_cr[ROHC_UOR2E2P23_PKTTYPE      ]=
    rohc_global_pkt_types_to_stats_cr[ROHC_UOR2E3P23_PKTTYPE      ]=
                                                ROHC_NO_OF_UOR_2_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_NORMALP0_PKTYPE        ]=
                                                ROHC_NO_OF_NORMALP0_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_IRP0_PKTYPE            ]=
    rohc_global_pkt_types_to_stats_cr[ROHC_IR_WITHOUT_DYN_PKTTYPE ]=
    rohc_global_pkt_types_to_stats_cr[ROHC_IR_WITH_DYN_PKTTYPE    ]=
                                                ROHC_NO_OF_IR_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_IRDYN_PKTTYPE          ]=
                                                ROHC_NO_OF_IR_DYN_CREATED;
    
    /* Created IR CR Packet for Profile 6 */
    rohc_global_pkt_types_to_stats_cr[ROHC_IRP6CR_PKTYPE          ]=
                                                ROHC_NO_OF_IR_CR_P6_CREATED;
    /* Created Common Compressed (CO) Packet for Profile 6 */
    rohc_global_pkt_types_to_stats_cr[ROHC_CO_P6_COMMON_PKTYPE    ]=
                                                ROHC_NO_OF_CO_P6_CREATED;
    /* Created Compressed Packets for Profile 6 */
    rohc_global_pkt_types_to_stats_cr[ROHC_CO_P6_RND1_PKTYPE      ]= 
                                                ROHC_NO_OF_RND1_P6_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_CO_P6_RND2_PKTYPE      ]= 
                                                ROHC_NO_OF_RND2_P6_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_CO_P6_RND3_PKTYPE      ]= 
                                                ROHC_NO_OF_RND3_P6_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_CO_P6_RND4_PKTYPE      ]=
                                                ROHC_NO_OF_RND4_P6_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_CO_P6_RND5_PKTYPE      ]=
                                                ROHC_NO_OF_RND5_P6_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_CO_P6_RND6_PKTYPE      ]=
                                                ROHC_NO_OF_RND6_P6_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_CO_P6_RND7_PKTYPE      ]=
                                                ROHC_NO_OF_RND7_P6_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_CO_P6_RND8_PKTYPE      ]=
                                                ROHC_NO_OF_RND8_P6_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_CO_P6_SEQ1_PKTYPE      ]=
                                                ROHC_NO_OF_SEQ1_P6_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_CO_P6_SEQ2_PKTYPE      ]=
                                                ROHC_NO_OF_SEQ2_P6_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_CO_P6_SEQ3_PKTYPE      ]=
                                                ROHC_NO_OF_SEQ3_P6_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_CO_P6_SEQ4_PKTYPE      ]=
                                                ROHC_NO_OF_SEQ4_P6_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_CO_P6_SEQ5_PKTYPE      ]=
                                                ROHC_NO_OF_SEQ5_P6_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_CO_P6_SEQ6_PKTYPE      ]=
                                                ROHC_NO_OF_SEQ6_P6_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_CO_P6_SEQ7_PKTYPE      ]=
                                                ROHC_NO_OF_SEQ7_P6_CREATED;
    rohc_global_pkt_types_to_stats_cr[ROHC_CO_P6_SEQ8_PKTYPE      ]=
                                                ROHC_NO_OF_SEQ8_P6_CREATED;
    
    
    /* for processed stats */
    rohc_global_pkt_types_to_stats_pr[ROHC_R0P1_PKTTYPE]= 
                                                ROHC_NO_OF_R_0_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_R0CRCP1_PKTTYPE] =
                                                ROHC_NO_R_0_CRC_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_UO0P1_PKTTYPE     ]=     
                                                ROHC_NO_UO_0_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_R1P1_PKTTYPE      ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_R1E0P1_PKTTYPE    ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_R1E1P1_PKTTYPE    ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_R1E2P1_PKTTYPE    ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_R1E3P1_PKTTYPE    ]=     
                                                ROHC_NO_OF_R_1_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_R1IDP1_PKTTYPE    ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_R1IDE0P1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_R1IDE1P1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_R1IDE2P1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_R1IDE3P1_PKTTYPE  ]=     
                                                ROHC_NO_R_1_ID_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_R1TSP1_PKTTYPE    ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_R1TSE0P1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_R1TSE1P1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_R1TSE2P1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_R1TSE3P1_PKTTYPE  ]=     
                                                ROHC_NO_R_1_TS_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_UO1P1_PKTTYPE     ]=     
                                                ROHC_NO_OF_UO_1_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_UO1IDP1_PKTTYPE   ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_UO1IDE0P1_PKTTYPE ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_UO1IDE1P1_PKTTYPE ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_UO1IDE2P1_PKTTYPE ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_UO1IDE3P1_PKTTYPE ]=     
                                                ROHC_NO_UO_1_ID_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_UO1TSP1_PKTTYPE   ]=     
                                                ROHC_NO_UO_1_TS_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_UOR2P1_PKTTYPE    ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_UOR2E0P1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_UOR2E1P1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_UOR2E2P1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_UOR2E3P1_PKTTYPE  ]=     
                                                ROHC_NO_OF_UOR_2_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_UOR2IDP1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_UOR2IDE0P1_PKTTYPE]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_UOR2IDE1P1_PKTTYPE]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_UOR2IDE2P1_PKTTYPE]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_UOR2IDE3P1_PKTTYPE]=     
                                                ROHC_NO_UOR_2_ID_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_UOR2TSP1_PKTTYPE  ]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_UOR2TSE0P1_PKTTYPE]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_UOR2TSE1P1_PKTTYPE]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_UOR2TSE2P1_PKTTYPE]=     
    rohc_global_pkt_types_to_stats_pr[ROHC_UOR2TSE3P1_PKTTYPE]=     
                                                ROHC_NO_UOR_2_ID_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_R1P23_PKTTYPE          ]=
    rohc_global_pkt_types_to_stats_pr[ROHC_R1E0P23_PKTTYPE        ]=
    rohc_global_pkt_types_to_stats_pr[ROHC_R1E1P23_PKTTYPE        ]=
    rohc_global_pkt_types_to_stats_pr[ROHC_R1E2P23_PKTTYPE        ]=
    rohc_global_pkt_types_to_stats_pr[ROHC_R1E3P23_PKTTYPE        ]=
                                                ROHC_NO_OF_R_1_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_UO1P23_PKTTYPE         ]=
                                                ROHC_NO_OF_UO_1_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_UOR2P23_PKTTYPE        ]=
    rohc_global_pkt_types_to_stats_pr[ROHC_UOR2E0P23_PKTTYPE      ]=
    rohc_global_pkt_types_to_stats_pr[ROHC_UOR2E1P23_PKTTYPE      ]=
    rohc_global_pkt_types_to_stats_pr[ROHC_UOR2E2P23_PKTTYPE      ]=
    rohc_global_pkt_types_to_stats_pr[ROHC_UOR2E3P23_PKTTYPE      ]=
                                                ROHC_NO_OF_UOR_2_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_NORMALP0_PKTYPE        ]=
                                                ROHC_NO_OF_NORMALP0_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_IRP0_PKTYPE            ]=
    rohc_global_pkt_types_to_stats_pr[ROHC_IR_WITHOUT_DYN_PKTTYPE ]=
    rohc_global_pkt_types_to_stats_pr[ROHC_IR_WITH_DYN_PKTTYPE    ]=
                                                ROHC_NO_OF_IR_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_IRDYN_PKTTYPE          ]=
                                                ROHC_NO_OF_IR_DYN_PROCESSED;
    
    /* Processed IR CR Packet for Profile 6 */
    rohc_global_pkt_types_to_stats_pr[ROHC_IRP6CR_PKTYPE          ]=
                                                ROHC_NO_OF_IR_CR_P6_PROCESSED;
    /* Processed Common Compressed (CO) Packet for Profile 6 */
    rohc_global_pkt_types_to_stats_pr[ROHC_CO_P6_COMMON_PKTYPE    ]=
                                                ROHC_NO_OF_CO_P6_PROCESSED;
    /* Processed Compressed Packets for Profile 6 */
    rohc_global_pkt_types_to_stats_pr[ROHC_CO_P6_RND1_PKTYPE      ]=
                                                ROHC_NO_OF_RND1_P6_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_CO_P6_RND2_PKTYPE      ]=
                                                ROHC_NO_OF_RND2_P6_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_CO_P6_RND3_PKTYPE      ]=
                                                ROHC_NO_OF_RND3_P6_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_CO_P6_RND4_PKTYPE      ]=
                                                ROHC_NO_OF_RND4_P6_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_CO_P6_RND5_PKTYPE      ]=
                                                ROHC_NO_OF_RND5_P6_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_CO_P6_RND6_PKTYPE      ]=
                                                ROHC_NO_OF_RND6_P6_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_CO_P6_RND7_PKTYPE      ]=
                                                ROHC_NO_OF_RND7_P6_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_CO_P6_RND8_PKTYPE      ]=
                                                ROHC_NO_OF_RND8_P6_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_CO_P6_SEQ1_PKTYPE      ]=
                                                ROHC_NO_OF_SEQ1_P6_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_CO_P6_SEQ2_PKTYPE      ]=
                                                ROHC_NO_OF_SEQ2_P6_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_CO_P6_SEQ3_PKTYPE      ]=
                                                ROHC_NO_OF_SEQ3_P6_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_CO_P6_SEQ4_PKTYPE      ]=
                                                ROHC_NO_OF_SEQ4_P6_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_CO_P6_SEQ5_PKTYPE      ]=
                                                ROHC_NO_OF_SEQ5_P6_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_CO_P6_SEQ6_PKTYPE      ]=
                                                ROHC_NO_OF_SEQ6_P6_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_CO_P6_SEQ7_PKTYPE      ]=
                                                ROHC_NO_OF_SEQ7_P6_PROCESSED;
    rohc_global_pkt_types_to_stats_pr[ROHC_CO_P6_SEQ8_PKTYPE      ]=
                                                ROHC_NO_OF_SEQ8_P6_PROCESSED;
    
    ROHC_EXIT_FN("rohc_fill_global_pkt_types_to_stats");
}
/*****************************************************************************
**      FUNCTION:
**              rohc_fill_global_constant_field_values
******************************************************************************
**
**      DESCRIPTION:
**              Initializes the globally constant filed value table
**      ARGUMENTS:
**               None
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_fill_global_constant_field_values
    ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_fill_global_constant_field_values");
    rohc_global_constant_field_values[ROHC_UO0_FID]=0U;
    rohc_global_constant_field_values[ROHC_R0CRC_FID]=1U;
    rohc_global_constant_field_values[ROHC_R0_FID]=0U;
    rohc_global_constant_field_values[ROHC_R1_FID]=2U;
    rohc_global_constant_field_values[ROHC_R1ID_FID]=2U;
    rohc_global_constant_field_values[ROHC_R1TS_FID]=2U;
    rohc_global_constant_field_values[ROHC_UO1_FID]=2U;
    rohc_global_constant_field_values[ROHC_UO1ID_FID]=2U;
    rohc_global_constant_field_values[ROHC_UO1TS_FID]=2U;
    rohc_global_constant_field_values[ROHC_UOR2_FID]=6U;
    rohc_global_constant_field_values[ROHC_UOR2ID_FID]=6U;
    rohc_global_constant_field_values[ROHC_UOR2T1_FID]=6U;
    rohc_global_constant_field_values[ROHC_T0_FID]=0;
    rohc_global_constant_field_values[ROHC_T1_FID]=1;
    rohc_global_constant_field_values[ROHC_EXT0_FID]=0;
    rohc_global_constant_field_values[ROHC_EXT1_FID]=1;
    rohc_global_constant_field_values[ROHC_EXT2_FID]=2;
    rohc_global_constant_field_values[ROHC_EXT3_FID]=3;
    rohc_global_constant_field_values[ROHC_RND1_FID]=46U;
    rohc_global_constant_field_values[ROHC_RND2_FID]=12U;
    rohc_global_constant_field_values[ROHC_RND3_FID]=0U;
    rohc_global_constant_field_values[ROHC_RND4_FID]=13U;
    rohc_global_constant_field_values[ROHC_RND5_FID]=4U;
    rohc_global_constant_field_values[ROHC_RND6_FID]=10U;
    rohc_global_constant_field_values[ROHC_RND7_FID]=47U;
    rohc_global_constant_field_values[ROHC_RND8_FID]=22U;
    rohc_global_constant_field_values[ROHC_COMMON_FID]=125U;
    
    ROHC_EXIT_FN("rohc_fill_global_constant_field_values");
}/* end function rohc_fill_global_constant_field_values */  
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_r0p1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose RO packet for RTP Profile 
**      ARGUMENTS:
**               None
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_r0p1_pkt
     ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_r0p1_pkt");
    rohc_c_pkt_type[ROHC_R0P1_PKTTYPE][0].field_id=ROHC_R0_FID;
    rohc_c_pkt_type[ROHC_R0P1_PKTTYPE][0].no_of_bits=2;
    rohc_c_pkt_type[ROHC_R0P1_PKTTYPE][1].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[ROHC_R0P1_PKTTYPE][1].no_of_bits=6;
    rohc_c_pkt_type[ROHC_R0P1_PKTTYPE][2].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[ROHC_R0P1_PKTTYPE][2].no_of_bits=ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[ROHC_R0P1_PKTTYPE][3].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_r0p1_pkt");
}/* end function rohc_c_make_r0p1_pkt */
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_r0crcp1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose RO CRC packet for RTP Profile 
**      ARGUMENTS:
**               None
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_r0crcp1_pkt
     ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_r0crcp1_pkt");
    rohc_c_pkt_type[ROHC_R0CRCP1_PKTTYPE][0].field_id=ROHC_R0CRC_FID;
    rohc_c_pkt_type[ROHC_R0CRCP1_PKTTYPE][0].no_of_bits=2;
    rohc_c_pkt_type[ROHC_R0CRCP1_PKTTYPE][1].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[ROHC_R0CRCP1_PKTTYPE][1].no_of_bits=
            ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_R0CRCP1_PKTTYPE][1].no_of_bits_occ=6;
/*  rohc_c_pkt_type[ROHC_R0CRCP1_PKTTYPE][1].cur_occ=ROHC_OCC1; */
    rohc_c_pkt_type[ROHC_R0CRCP1_PKTTYPE][2].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[ROHC_R0CRCP1_PKTTYPE][2].no_of_bits=
            ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[ROHC_R0CRCP1_PKTTYPE][3].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[ROHC_R0CRCP1_PKTTYPE][3].no_of_bits=
            ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_R0CRCP1_PKTTYPE][3].no_of_bits_occ=1;
/*  rohc_c_pkt_type[ROHC_R0CRCP1_PKTTYPE][3].cur_occ=ROHC_OCC2; */
    rohc_c_pkt_type[ROHC_R0CRCP1_PKTTYPE][4].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[ROHC_R0CRCP1_PKTTYPE][4].no_of_bits=7;
    rohc_c_pkt_type[ROHC_R0CRCP1_PKTTYPE][5].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_r0crcp1_pkt");
}/* end function rohc_c_make_r0crcp1_pkt */
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_uo0p1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose UO 0  packet for RTP Profile 
**      ARGUMENTS:
**               None
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_uo0p1_pkt
     ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_uo0p1_pkt");
    rohc_c_pkt_type[ROHC_UO0P1_PKTTYPE][0].field_id=ROHC_UO0_FID;
    rohc_c_pkt_type[ROHC_UO0P1_PKTTYPE][0].no_of_bits=1;
    rohc_c_pkt_type[ROHC_UO0P1_PKTTYPE][1].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[ROHC_UO0P1_PKTTYPE][1].no_of_bits=4;
    rohc_c_pkt_type[ROHC_UO0P1_PKTTYPE][2].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[ROHC_UO0P1_PKTTYPE][2].no_of_bits=3;
    rohc_c_pkt_type[ROHC_UO0P1_PKTTYPE][3].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[ROHC_UO0P1_PKTTYPE][3].no_of_bits=ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[ROHC_UO0P1_PKTTYPE][4].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_uo0p1_pkt");
} /* end function rohc_c_make_uo0p1_pkt */
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_r1p23_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose R1 CRC packet for UDP/ESP Profile 
**      ARGUMENTS:
**               Packet type
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_r1p23_pkt
    ROHC_PARAMS((pkt_type),rohc_U8bit_t pkt_type)
{
    ROHC_ENTER_FN("rohc_c_make_r1p23_pkt");
    rohc_c_pkt_type[pkt_type][0].field_id=ROHC_R1_FID;
    rohc_c_pkt_type[pkt_type][0].no_of_bits=2U;
    rohc_c_pkt_type[pkt_type][1].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][1].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[pkt_type][1].no_of_bits_occ=6;
/*  rohc_c_pkt_type[pkt_type][1].cur_occ=ROHC_OCC1; */
    rohc_c_pkt_type[pkt_type][2].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[pkt_type][2].no_of_bits=ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[pkt_type][3].field_id=ROHC_EXT_FID;
    rohc_c_pkt_type[pkt_type][3].no_of_bits=1;
    rohc_c_pkt_type[pkt_type][4].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[pkt_type][4].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[pkt_type][4].no_of_bits_occ=7;
/*  rohc_c_pkt_type[pkt_type][4].cur_occ=ROHC_OCC1; */
    rohc_c_pkt_type[pkt_type][5].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_r1p23_pkt");
}/* end function rohc_c_make_r1p23_pkt */
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_uo1p23_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose UO 1 packet for UDP/ESP Profile 
**      ARGUMENTS:
**               Packet type
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_uo1p23_pkt
    ROHC_PARAMS((pkt_type),rohc_U8bit_t pkt_type)
{
    ROHC_ENTER_FN("rohc_c_make_uo1p23_pkt");
    rohc_c_pkt_type[pkt_type][0].field_id=ROHC_UO1_FID;
    rohc_c_pkt_type[pkt_type][0].no_of_bits=2U;
    rohc_c_pkt_type[pkt_type][1].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[pkt_type][1].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[pkt_type][1].no_of_bits_occ=6;
/*  rohc_c_pkt_type[pkt_type][1].cur_occ=ROHC_OCC1; */
    rohc_c_pkt_type[pkt_type][2].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[pkt_type][2].no_of_bits=ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[pkt_type][3].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][3].no_of_bits=5;
    rohc_c_pkt_type[pkt_type][4].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[pkt_type][4].no_of_bits=3;
    rohc_c_pkt_type[pkt_type][5].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_uo1p23_pkt");
}/* end function rohc_c_make_uo1p23_pkt */
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_uor2p23_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose UOR 2 CRC packet for RTP/ESP Profile 
**      ARGUMENTS:
**               Packet type
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_uor2p23_pkt
    ROHC_PARAMS((pkt_type),rohc_U8bit_t pkt_type)
{
    ROHC_ENTER_FN("rohc_c_make_uor2p23_pkt");
    rohc_c_pkt_type[pkt_type][0].field_id=ROHC_UOR2_FID;
    rohc_c_pkt_type[pkt_type][0].no_of_bits=3U;
    rohc_c_pkt_type[pkt_type][1].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][1].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[pkt_type][1].no_of_bits_occ=5;
    rohc_c_pkt_type[pkt_type][2].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[pkt_type][2].no_of_bits=ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[pkt_type][3].field_id=ROHC_EXT_FID;
    rohc_c_pkt_type[pkt_type][3].no_of_bits=1;
    rohc_c_pkt_type[pkt_type][4].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[pkt_type][4].no_of_bits=7;
    rohc_c_pkt_type[pkt_type][5].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_uor2p23_pkt");
}/* end function rohc_c_make_uor2p23_pkt */
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_r1p1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose R1  packet for RTP Profile 
**      ARGUMENTS:
**               Packet type
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_r1p1_pkt
    ROHC_PARAMS((pkt_type),rohc_U8bit_t pkt_type)
{
    ROHC_ENTER_FN("rohc_c_make_r1p1_pkt");
    rohc_c_pkt_type[pkt_type][0].field_id=ROHC_R1_FID;
    rohc_c_pkt_type[pkt_type][0].no_of_bits=2U;
    rohc_c_pkt_type[pkt_type][1].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][1].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[pkt_type][1].no_of_bits_occ=6;
/*  rohc_c_pkt_type[pkt_type][1].cur_occ=ROHC_OCC1; */
    rohc_c_pkt_type[pkt_type][2].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[pkt_type][2].no_of_bits=ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[pkt_type][3].field_id=ROHC_RTP_M_FID;
    rohc_c_pkt_type[pkt_type][3].no_of_bits=1;
    rohc_c_pkt_type[pkt_type][4].field_id=ROHC_EXT_FID;
    rohc_c_pkt_type[pkt_type][4].no_of_bits=1;
    rohc_c_pkt_type[pkt_type][5].field_id=ROHC_TS_FID;
    rohc_c_pkt_type[pkt_type][5].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[pkt_type][5].no_of_bits_occ=6;
/*  rohc_c_pkt_type[pkt_type][5].cur_occ=ROHC_OCC1; */
    rohc_c_pkt_type[pkt_type][6].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_r1p1_pkt");
} /* end function rohc_c_make_r1p1_pkt*/
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_r1idp1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose R1 ID packet for RTP Profile 
**      ARGUMENTS:
**               Packet type
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_r1idp1_pkt
    ROHC_PARAMS((pkt_type),rohc_U8bit_t pkt_type)
{
    ROHC_ENTER_FN("rohc_c_make_r1idp1_pkt");
    rohc_c_pkt_type[pkt_type][0].field_id=ROHC_R1_FID;
    rohc_c_pkt_type[pkt_type][0].no_of_bits=2U;
    rohc_c_pkt_type[pkt_type][1].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][1].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[pkt_type][1].no_of_bits_occ=6;
/*  rohc_c_pkt_type[pkt_type][1].cur_occ=ROHC_OCC1; */
    rohc_c_pkt_type[pkt_type][2].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[pkt_type][2].no_of_bits=ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[pkt_type][3].field_id=ROHC_RTP_M_FID;
    rohc_c_pkt_type[pkt_type][3].no_of_bits=1;
    rohc_c_pkt_type[pkt_type][4].field_id=ROHC_EXT_FID;
    rohc_c_pkt_type[pkt_type][4].no_of_bits=1;
    rohc_c_pkt_type[pkt_type][5].field_id=ROHC_T0_FID;
    rohc_c_pkt_type[pkt_type][5].no_of_bits=1;
    rohc_c_pkt_type[pkt_type][6].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[pkt_type][6].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[pkt_type][6].no_of_bits_occ=5;
/*  rohc_c_pkt_type[pkt_type][6].cur_occ=ROHC_OCC1; */
    rohc_c_pkt_type[pkt_type][7].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_r1idp1_pkt");
} /* end function rohc_c_make_r1idp1_pkt*/
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_r1tsp1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose R1 TS packet for RTP Profile 
**      ARGUMENTS:
**               PAcket type
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_r1tsp1_pkt
    ROHC_PARAMS((pkt_type),rohc_U8bit_t pkt_type)
{
    ROHC_ENTER_FN("rohc_c_make_r1tsp1_pkt");
    rohc_c_pkt_type[pkt_type][0].field_id=ROHC_R1_FID;
    rohc_c_pkt_type[pkt_type][0].no_of_bits=2U;
    rohc_c_pkt_type[pkt_type][1].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][1].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[pkt_type][1].no_of_bits_occ=6;
/*  rohc_c_pkt_type[pkt_type][1].cur_occ=ROHC_OCC1; */
    rohc_c_pkt_type[pkt_type][2].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[pkt_type][2].no_of_bits=ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[pkt_type][3].field_id=ROHC_RTP_M_FID;
    rohc_c_pkt_type[pkt_type][3].no_of_bits=1;
    rohc_c_pkt_type[pkt_type][4].field_id=ROHC_EXT_FID;
    rohc_c_pkt_type[pkt_type][4].no_of_bits=1;
    rohc_c_pkt_type[pkt_type][5].field_id=ROHC_T1_FID;
    rohc_c_pkt_type[pkt_type][5].no_of_bits=1;
    rohc_c_pkt_type[pkt_type][6].field_id=ROHC_TS_FID;
    rohc_c_pkt_type[pkt_type][6].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[pkt_type][6].no_of_bits_occ=5;
/*  rohc_c_pkt_type[pkt_type][6].cur_occ=ROHC_OCC1; */
    rohc_c_pkt_type[pkt_type][7].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_r1tsp1_pkt");
} /* end function rohc_c_make_r1tsp1_pkt */
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_uo1p1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose UO1 packet for RTP Profile 
**      ARGUMENTS:
**               None
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_uo1p1_pkt
     ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_uo1p1_pkt");
    rohc_c_pkt_type[ROHC_UO1P1_PKTTYPE][0].field_id=ROHC_UO1_FID;
    rohc_c_pkt_type[ROHC_UO1P1_PKTTYPE][0].no_of_bits=2U;
    rohc_c_pkt_type[ROHC_UO1P1_PKTTYPE][1].field_id=ROHC_TS_FID;
    rohc_c_pkt_type[ROHC_UO1P1_PKTTYPE][1].no_of_bits=6;
    rohc_c_pkt_type[ROHC_UO1P1_PKTTYPE][2].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[ROHC_UO1P1_PKTTYPE][2].no_of_bits=ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[ROHC_UO1P1_PKTTYPE][3].field_id=ROHC_RTP_M_FID;
    rohc_c_pkt_type[ROHC_UO1P1_PKTTYPE][3].no_of_bits=1;
    rohc_c_pkt_type[ROHC_UO1P1_PKTTYPE][4].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[ROHC_UO1P1_PKTTYPE][4].no_of_bits=4;
    rohc_c_pkt_type[ROHC_UO1P1_PKTTYPE][5].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[ROHC_UO1P1_PKTTYPE][5].no_of_bits=3;
    rohc_c_pkt_type[ROHC_UO1P1_PKTTYPE][6].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_uo1p1_pkt");
}/* end function rohc_c_make_uo1p1_pkt */
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_uo1idp1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose UO1 ID packet for RTP Profile 
**      ARGUMENTS:
**               Packet type
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_uo1idp1_pkt
    ROHC_PARAMS((pkt_type),rohc_U8bit_t pkt_type)
{
    ROHC_ENTER_FN("rohc_c_make_uo1idp1_pkt");
    rohc_c_pkt_type[pkt_type][0].field_id=ROHC_UO1_FID;
    rohc_c_pkt_type[pkt_type][0].no_of_bits=2U;
    rohc_c_pkt_type[pkt_type][1].field_id=ROHC_T0_FID;
    rohc_c_pkt_type[pkt_type][1].no_of_bits=1U;
    rohc_c_pkt_type[pkt_type][2].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[pkt_type][2].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[pkt_type][2].no_of_bits_occ=5;
/*  rohc_c_pkt_type[pkt_type][2].cur_occ=ROHC_OCC1; */
    rohc_c_pkt_type[pkt_type][3].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[pkt_type][3].no_of_bits=ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[pkt_type][4].field_id=ROHC_EXT_FID;
    rohc_c_pkt_type[pkt_type][4].no_of_bits=1;
    rohc_c_pkt_type[pkt_type][5].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][5].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[pkt_type][5].no_of_bits_occ=4;
/*  rohc_c_pkt_type[pkt_type][5].cur_occ=ROHC_OCC1; */
    rohc_c_pkt_type[pkt_type][6].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[pkt_type][6].no_of_bits=3;
    rohc_c_pkt_type[pkt_type][7].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_uo1idp1_pkt");
}/* end function rohc_c_make_uo1idp1_pkt */
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_uo1tsp1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose UO1 TS packet for RTP Profile 
**      ARGUMENTS:
**               None
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_uo1tsp1_pkt
     ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_uo1tsp1_pkt");
    rohc_c_pkt_type[ROHC_UO1TSP1_PKTTYPE][0].field_id=ROHC_UO1_FID;
    rohc_c_pkt_type[ROHC_UO1TSP1_PKTTYPE][0].no_of_bits=2U;
    rohc_c_pkt_type[ROHC_UO1TSP1_PKTTYPE][1].field_id=ROHC_T1_FID;
    rohc_c_pkt_type[ROHC_UO1TSP1_PKTTYPE][1].no_of_bits=1U;
    rohc_c_pkt_type[ROHC_UO1TSP1_PKTTYPE][2].field_id=ROHC_TS_FID;
    rohc_c_pkt_type[ROHC_UO1TSP1_PKTTYPE][2].no_of_bits=
            ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_UO1TSP1_PKTTYPE][2].no_of_bits_occ=5;
/*  rohc_c_pkt_type[ROHC_UO1TSP1_PKTTYPE][2].cur_occ=ROHC_OCC1; */
    rohc_c_pkt_type[ROHC_UO1TSP1_PKTTYPE][3].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[ROHC_UO1TSP1_PKTTYPE][3].no_of_bits=
            ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[ROHC_UO1TSP1_PKTTYPE][4].field_id=ROHC_RTP_M_FID;
    rohc_c_pkt_type[ROHC_UO1TSP1_PKTTYPE][4].no_of_bits=1;
    rohc_c_pkt_type[ROHC_UO1TSP1_PKTTYPE][5].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[ROHC_UO1TSP1_PKTTYPE][5].no_of_bits=
            ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_UO1TSP1_PKTTYPE][5].no_of_bits_occ=4;
/*  rohc_c_pkt_type[ROHC_UO1TSP1_PKTTYPE][5].cur_occ=ROHC_OCC1; */
    rohc_c_pkt_type[ROHC_UO1TSP1_PKTTYPE][6].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[ROHC_UO1TSP1_PKTTYPE][6].no_of_bits=3;
    rohc_c_pkt_type[ROHC_UO1TSP1_PKTTYPE][7].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_uo1tsp1_pkt");
}/* end function rohc_c_make_uo1tsp1_pkt */
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_uor2p1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose UOR 2 packet for RTP Profile 
**      ARGUMENTS:
**               packet type
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_uor2p1_pkt
    ROHC_PARAMS((pkt_type),rohc_U8bit_t pkt_type)
{
    ROHC_ENTER_FN("rohc_c_make_uor2p1_pkt");
    rohc_c_pkt_type[pkt_type][0].field_id=ROHC_UOR2_FID;
    rohc_c_pkt_type[pkt_type][0].no_of_bits=3U;
    rohc_c_pkt_type[pkt_type][1].field_id=ROHC_TS_FID;
    rohc_c_pkt_type[pkt_type][1].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[pkt_type][1].no_of_bits_occ=5;
/*  rohc_c_pkt_type[pkt_type][1].cur_occ=ROHC_OCC1; */
    rohc_c_pkt_type[pkt_type][2].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[pkt_type][2].no_of_bits=ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[pkt_type][3].field_id=ROHC_TS_FID;
    rohc_c_pkt_type[pkt_type][3].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[pkt_type][3].no_of_bits_occ=1;
/*  rohc_c_pkt_type[pkt_type][3].cur_occ=ROHC_OCC2; */ 
    rohc_c_pkt_type[pkt_type][4].field_id=ROHC_RTP_M_FID;
    rohc_c_pkt_type[pkt_type][4].no_of_bits=1;
    rohc_c_pkt_type[pkt_type][5].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][5].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[pkt_type][5].no_of_bits_occ=6;
/*  rohc_c_pkt_type[pkt_type][5].cur_occ=ROHC_OCC1; */
    rohc_c_pkt_type[pkt_type][6].field_id=ROHC_EXT_FID;
    rohc_c_pkt_type[pkt_type][6].no_of_bits=1;
    rohc_c_pkt_type[pkt_type][7].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[pkt_type][7].no_of_bits=7;
    rohc_c_pkt_type[pkt_type][8].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_uor2p1_pkt");
} /* end function rohc_c_make_uor2p1_pkt*/
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_uor2idp1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose UOR 2 ID packet for RTP Profile 
**      ARGUMENTS:
**               Packet type
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_uor2idp1_pkt
    ROHC_PARAMS((pkt_type),rohc_U8bit_t pkt_type)
{
    ROHC_ENTER_FN("rohc_c_make_uor2idp1_pkt");
    rohc_c_pkt_type[pkt_type][0].field_id=ROHC_UOR2_FID;
    rohc_c_pkt_type[pkt_type][0].no_of_bits=3U;
    rohc_c_pkt_type[pkt_type][1].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[pkt_type][1].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[pkt_type][1].no_of_bits_occ=5;
/*  rohc_c_pkt_type[pkt_type][1].cur_occ=ROHC_OCC1; */
    rohc_c_pkt_type[pkt_type][2].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[pkt_type][2].no_of_bits=ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[pkt_type][3].field_id=ROHC_T0_FID;
    rohc_c_pkt_type[pkt_type][3].no_of_bits=1;
    rohc_c_pkt_type[pkt_type][4].field_id=ROHC_RTP_M_FID;
    rohc_c_pkt_type[pkt_type][4].no_of_bits=1;
    rohc_c_pkt_type[pkt_type][5].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][5].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[pkt_type][5].no_of_bits_occ=6;
/*  rohc_c_pkt_type[pkt_type][5].cur_occ=ROHC_OCC1; */
    rohc_c_pkt_type[pkt_type][6].field_id=ROHC_EXT_FID;
    rohc_c_pkt_type[pkt_type][6].no_of_bits=1;
    rohc_c_pkt_type[pkt_type][7].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[pkt_type][7].no_of_bits=7;
    rohc_c_pkt_type[pkt_type][8].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_uor2idp1_pkt");
} /* end function rohc_c_make_uor2idp1_pkt*/
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_uor2tsp1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose RO CRC packet for RTP Profile 
**      ARGUMENTS:
**               packet type
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_uor2tsp1_pkt
    ROHC_PARAMS((pkt_type),rohc_U8bit_t pkt_type)
{
    ROHC_ENTER_FN("rohc_c_make_uor2tsp1_pkt");
    rohc_c_pkt_type[pkt_type][0].field_id=ROHC_UOR2_FID;
    rohc_c_pkt_type[pkt_type][0].no_of_bits=3U;
    rohc_c_pkt_type[pkt_type][1].field_id=ROHC_TS_FID;
    rohc_c_pkt_type[pkt_type][1].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[pkt_type][1].no_of_bits_occ=5;
/*  rohc_c_pkt_type[pkt_type][1].cur_occ=ROHC_OCC1; */
    rohc_c_pkt_type[pkt_type][2].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[pkt_type][2].no_of_bits=ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[pkt_type][3].field_id=ROHC_T1_FID;
    rohc_c_pkt_type[pkt_type][3].no_of_bits=1;
    rohc_c_pkt_type[pkt_type][4].field_id=ROHC_RTP_M_FID;
    rohc_c_pkt_type[pkt_type][4].no_of_bits=1;
    rohc_c_pkt_type[pkt_type][5].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][5].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[pkt_type][5].no_of_bits_occ=6;
/*  rohc_c_pkt_type[pkt_type][5].cur_occ=ROHC_OCC1; */
    rohc_c_pkt_type[pkt_type][6].field_id=ROHC_EXT_FID;
    rohc_c_pkt_type[pkt_type][6].no_of_bits=1;
    rohc_c_pkt_type[pkt_type][7].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[pkt_type][7].no_of_bits=7;
    rohc_c_pkt_type[pkt_type][8].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_uor2tsp1_pkt");
} /* end function rohc_c_make_uor2idp1_pkt*/
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_ext0_tsp1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose EXT 0 TS packet for RTP Profile 
**      ARGUMENTS:
**               Packet type , index
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_ext0_tsp1_pkt
    ROHC_PARAMS((pkt_type,index),
    rohc_U8bit_t pkt_type _AND_
    rohc_U8bit_t index)
{
    ROHC_ENTER_FN("rohc_c_make_ext0_tsp1_pkt");
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_EXT0_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=2;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=3;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_TS_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=3;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_ext0_tsp1_pkt");
} /* end function rohc_c_make_ext0_tsp1_pkt*/
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_ext0_p23_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose EXT 0  packet for UDP/ESP Profile 
**      ARGUMENTS:
**               Packet type , index
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_ext0_p23_pkt
    ROHC_PARAMS((pkt_type,index),
    rohc_U8bit_t pkt_type _AND_
    rohc_U8bit_t index)
{
    ROHC_ENTER_FN("rohc_c_make_ext0_p23_pkt");
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_EXT0_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=2;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=3;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=3;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_ext0_p23_pkt");
} /* end function rohc_c_make_ext0_p23_pkt*/
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_ext1_p23_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose EXT 1 packet for UDP/ESP Profile 
**      ARGUMENTS:
**          Packet type, index              
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_ext1_p23_pkt
    ROHC_PARAMS((pkt_type,index),
    rohc_U8bit_t pkt_type _AND_
    rohc_U8bit_t index)
{
    ROHC_ENTER_FN("rohc_c_make_ext1_p23_pkt");
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_EXT1_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=2;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=3;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=11;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_ext1_p23_pkt");
} /* end function rohc_c_make_ext1_p23_pkt*/
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_ext2_p23_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose EXT 2  packet for UDP/ESP Profile 
**      ARGUMENTS:
**               Packet type,index
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_ext2_p23_pkt
    ROHC_PARAMS((pkt_type,index),
    rohc_U8bit_t pkt_type _AND_
    rohc_U8bit_t index)
{
    ROHC_ENTER_FN("rohc_c_make_ext2_p23_pkt");
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_EXT2_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=2;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=3;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_OUTERIP_ID_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=11;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=8;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_ext2_p23_pkt");
} /* end function rohc_c_make_ext2_p23_pkt*/

/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_ext3p23_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose EXT 3 packet for UDP/ESP Profile 
**      ARGUMENTS:
**               Packet type,index
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_ext3p23_pkt
    ROHC_PARAMS((pkt_type,index),
    rohc_U8bit_t pkt_type _AND_
    rohc_U8bit_t index)
{
    ROHC_ENTER_FN("rohc_c_make_ext3p23_pkt");
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_EXT3_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=2;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_EXT3_FLAGS_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=6;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INNERIP_FLAGS_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_EXT3FLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_OUTERIP_FLAGS_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_EXT3FLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_EXT3FLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INNERIP4_TOS_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_INNERIPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INNERIP4_TTL_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_INNERIPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INNERIP4_PROTO_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_INNERIPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INNERIP_EXTHEADERS_LIST_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_INNERIPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_EXT3FLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_OUTERIP4_TOS_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_OUTERIPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_OUTERIP4_TTL_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_OUTERIPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_OUTERIP4_PROTO_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_OUTERIPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_OUTERIP_EXTHEADERS_LIST_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_OUTERIPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_OUTERIP_ID_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_OUTERIPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_ext3p23_pkt");
}/* end function rohc_c_make_ext3p23_pkt */


/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_ext0_idp1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose EXT 0 ID packet for RTP Profile 
**      ARGUMENTS:
**               Packet type,index
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_ext0_idp1_pkt
    ROHC_PARAMS((pkt_type,index),
    rohc_U8bit_t pkt_type _AND_
    rohc_U8bit_t index)
{
    ROHC_ENTER_FN("rohc_c_make_ext0_idp1_pkt");
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_EXT0_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=2;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=3;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=3;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_ext0_idp1_pkt");
}/* end function rohc_c_make_ext0_idp1_pkt*/
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_ext1_t1p1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose EXT 1 packet for RTP Profile 
**      ARGUMENTS:
**               Packet type,index
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_ext1_t1p1_pkt
    ROHC_PARAMS((pkt_type,index),
    rohc_U8bit_t pkt_type _AND_
    rohc_U8bit_t index)
{
    ROHC_ENTER_FN("rohc_c_make_ext1_t1p1_pkt");
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_EXT1_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=2;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=3;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_TS_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=3;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=8;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_ext1_t1p1_pkt");
}/* end functionrohc_c_make_ext1_t1p1_pkt*/
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_ext1_t0p1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose EXT 1 packet for RTP Profile 
**      ARGUMENTS:
**               Packet type,index
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_ext1_t0p1_pkt
    ROHC_PARAMS((pkt_type,index),
    rohc_U8bit_t pkt_type _AND_
    rohc_U8bit_t index)
{
    ROHC_ENTER_FN("rohc_c_make_ext1_t0p1_pkt");
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_EXT1_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=2;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=3;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=3;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_TS_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=8;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_ext1_t0p1_pkt");
}/* end function rohc_c_make_ext1_t0p1_pkt*/
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_ext1_notp1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose EXT 1 packet for RTP Profile 
**      ARGUMENTS:
**               Packet type, index
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_ext1_notp1_pkt
    ROHC_PARAMS((pkt_type,index),
    rohc_U8bit_t pkt_type _AND_
    rohc_U8bit_t index)
{
    ROHC_ENTER_FN("rohc_c_make_ext1_notp1_pkt");
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_EXT1_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=2;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=3;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_TS_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=11;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_ext1_notp1_pkt");
}/* end function rohc_c_make_ext1_notp1_pkt */
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_ext2_t1p1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose EXT 2 packet for RTP Profile 
**      ARGUMENTS:
**               Packet type, index
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_ext2_t1p1_pkt
    ROHC_PARAMS((pkt_type,index),
    rohc_U8bit_t pkt_type _AND_
    rohc_U8bit_t index)
{
    ROHC_ENTER_FN("rohc_c_make_ext2_t1p1_pkt");
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_EXT2_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=2;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=3;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_TS_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=11;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=8;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_ext2_t1p1_pkt");
}/* end function rohc_c_make_ext2_t1p1_pkt */
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_ext2_t0p1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose EXT 2 packet for RTP Profile 
**      ARGUMENTS:
**               Packet type,index
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_ext2_t0p1_pkt
    ROHC_PARAMS((pkt_type,index),
    rohc_U8bit_t pkt_type _AND_
    rohc_U8bit_t index)
{
    ROHC_ENTER_FN("rohc_c_make_ext2_t0p1_pkt");
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_EXT2_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=2;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=3;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=11;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_TS_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=8;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_ext2_t0p1_pkt");
}/* end function rohc_c_make_ext2_t0p1_pkt */
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_ext2_notp1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose EXT 2 packet for RTP Profile 
**      ARGUMENTS:
**               Packet type,index
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_ext2_notp1_pkt
    ROHC_PARAMS((pkt_type,index),
    rohc_U8bit_t pkt_type _AND_
    rohc_U8bit_t index)
{
    ROHC_ENTER_FN("rohc_c_make_ext2_notp1_pkt");
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_EXT2_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=2;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=3;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_TS_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=19;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_ext2_notp1_pkt");
}/* end function rohc_c_make_ext2_notp1_pkt */
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_ext3p1_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose EXT 3 packet for RTP Profile 
**      ARGUMENTS:
**               Packet type,index
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_c_make_ext3p1_pkt
    ROHC_PARAMS((pkt_type,index),
    rohc_U8bit_t pkt_type _AND_
    rohc_U8bit_t index)
{
    ROHC_ENTER_FN("rohc_c_make_ext3p1_pkt");
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_EXT3_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=2;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_EXT3_FLAGS_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=6;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INNERIP_FLAGS_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_EXT3FLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_OUTERIP_FLAGS_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_INNERIPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_EXT3FLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_TS_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_EXT3FLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INNERIP4_TOS_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_INNERIPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INNERIP4_TTL_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_INNERIPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INNERIP4_PROTO_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_INNERIPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INNERIP_EXTHEADERS_LIST_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_INNERIPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_EXT3FLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_OUTERIP4_TOS_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_OUTERIPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_OUTERIP4_TTL_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_OUTERIPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_OUTERIP4_PROTO_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_OUTERIPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_OUTERIP_EXTHEADERS_LIST_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_OUTERIPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_OUTERIP_ID_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_OUTERIPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_RTP_FLAGS_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_EXT3FLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_RTP_PT_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_RTPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_RTP_CSRC_LIST_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_RTPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_RTP_TS_STRIDE_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_RTPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_RTP_TIME_STRIDE_FID;
    rohc_c_pkt_type[pkt_type][index++].no_of_bits=ROHC_ENCTYPE_RTPFLAGSP1;
    rohc_c_pkt_type[pkt_type][index].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_ext3p1_pkt");
}/* end function rohc_c_make_ext3p1_pkt */

/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_co_common_pkt();
******************************************************************************
**
**      DESCRIPTION:
**          compose Common Compressed packet for TCP Profile
**      ARGUMENTS:
**				void
**      RETURN VALUE:
**              void
**      NOTES:
**

*****************************************************************************/
/* + SPR 17439 */
static void
rohc_c_make_co_common_pkt(void)
/* - SPR 17439 */
{
    ROHC_ENTER_FN("rohc_c_make_co_common_pkt");
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][0].field_id= ROHC_COMMON_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][0].no_of_bits=7U;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][1].field_id= ROHC_TTL_HOPL_OUTER_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][1].no_of_bits=1U;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][2].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][2].no_of_bits= ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][3].field_id= ROHC_TCP_ACK_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][3].no_of_bits=1U;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][4].field_id= ROHC_TCP_PSH_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][4].no_of_bits=1U;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][5].field_id= ROHC_TCP_RSF_FLAGS_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][5].no_of_bits=2U;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][6].field_id= ROHC_TCP_MSN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][6].no_of_bits=4U;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][7].field_id= ROHC_TCP_SEQ_IND_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][7].no_of_bits=2U;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][8].field_id= ROHC_TCP_ACK_IND_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][8].no_of_bits=2U;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][9].field_id= ROHC_TCP_ACK_STRIDE_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][9].no_of_bits=1U;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][10].field_id= ROHC_TCP_WINDOW_IND_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][10].no_of_bits=1U;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][11].field_id= ROHC_TCP_IPID_IND_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][11].no_of_bits=1U;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][12].field_id= ROHC_TCP_URG_PTR_PRESENT_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][12].no_of_bits=1U;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][13].field_id= ROHC_TCP_RESERVED_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][13].no_of_bits=1U;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][14].field_id= ROHC_TCP_ECN_USED_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][14].no_of_bits=1U;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][15].field_id= ROHC_INNERIP4_DSCP_PRESENT_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][15].no_of_bits=1U;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][16].field_id= ROHC_INNERIP4_TTL_HPL_PRESENT_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][16].no_of_bits=1U;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][17].field_id= ROHC_TCP_LIST_PRESENT_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][17].no_of_bits=1U;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][18].field_id= ROHC_INNERIP_BEH_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][18].no_of_bits=2U;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][19].field_id= ROHC_TCP_URG_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][19].no_of_bits=1U;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][20].field_id= ROHC_INNERIP_DF_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][20].no_of_bits=1U;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][21].field_id= ROHC_CRC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][21].no_of_bits=7U;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][22].field_id= ROHC_SN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][22].no_of_bits=ROHC_ENCTYPE_VAR_32_BITS;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][23].field_id= ROHC_TCP_ACKNO_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][23].no_of_bits=ROHC_ENCTYPE_VAR_32_BITS;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][24].field_id= ROHC_TCP_ACK_STRIDE_FID;           
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][24].no_of_bits=ROHC_ENCTYPE_STATIC_OR_IRREG;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][25].field_id= ROHC_TCP_WINDOW_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][25].no_of_bits=ROHC_ENCTYPE_STATIC_OR_IRREG;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][26].field_id= ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][26].no_of_bits=ROHC_ENCTYPE_OPT_IP_ID_LSB;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][27].field_id= ROHC_TCP_URG_PTR_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][27].no_of_bits=ROHC_ENCTYPE_STATIC_OR_IRREG;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][28].field_id= ROHC_INNERIP4_DSCP_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][28].no_of_bits=ROHC_ENCTYPE_STATIC_OR_IRREG;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][29].field_id= ROHC_INNERIP4_TTL_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][29].no_of_bits=ROHC_ENCTYPE_STATIC_OR_IRREG;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][30].field_id= ROHC_TCP_OPT_LIST_FID;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][30].no_of_bits=ROHC_ENCTYPE_TCP_OPTIONS_LIST;
    rohc_c_pkt_type[ROHC_CO_P6_COMMON_PKTYPE][31].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_co_common_pkt");
} /* end function rohc_c_make_co_common_pkt*/
    
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_co_rnd1p6_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose RND1 packet for TCP Profile
**          i.e Send LSBs of sequence number
**      ARGUMENTS:
**              void 
**      RETURN VALUE:
**              void
**      NOTES:
**
*****************************************************************************/
static void
rohc_c_make_co_rnd1p6_pkt
    ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_co_rnd1p6_pkt");
    rohc_c_pkt_type[ROHC_CO_P6_RND1_PKTYPE][0].field_id=ROHC_RND1_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND1_PKTYPE][0].no_of_bits=6U;
    rohc_c_pkt_type[ROHC_CO_P6_RND1_PKTYPE][1].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND1_PKTYPE][1].no_of_bits=
            ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_CO_P6_RND1_PKTYPE][1].no_of_bits_occ=2;
    rohc_c_pkt_type[ROHC_CO_P6_RND1_PKTYPE][2].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND1_PKTYPE][2].no_of_bits=
            ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[ROHC_CO_P6_RND1_PKTYPE][3].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND1_PKTYPE][3].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_CO_P6_RND1_PKTYPE][3].no_of_bits_occ=16;
    rohc_c_pkt_type[ROHC_CO_P6_RND1_PKTYPE][4].field_id=ROHC_TCP_MSN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND1_PKTYPE][4].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_RND1_PKTYPE][5].field_id=ROHC_TCP_PSH_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND1_PKTYPE][5].no_of_bits=1;
    rohc_c_pkt_type[ROHC_CO_P6_RND1_PKTYPE][6].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND1_PKTYPE][6].no_of_bits=3;
    rohc_c_pkt_type[ROHC_CO_P6_RND1_PKTYPE][7].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_co_rnd1p6_pkt");
} /* end function rohc_c_make_co_rnd1p6_pkt*/
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_co_rnd2p6_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose RND2 packet for TCP Profile
**          i.e Send scaled sequence number LSBs
**      ARGUMENTS:
**              void
**      RETURN VALUE:
**              void
**      NOTES:
**
*****************************************************************************/
static void
rohc_c_make_co_rnd2p6_pkt
    ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_co_rnd2p6_pkt");
    rohc_c_pkt_type[ROHC_CO_P6_RND2_PKTYPE][0].field_id=ROHC_RND2_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND2_PKTYPE][0].no_of_bits=4U;
    rohc_c_pkt_type[ROHC_CO_P6_RND2_PKTYPE][1].field_id=ROHC_TCP_SCALED_SN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND2_PKTYPE][1].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_RND2_PKTYPE][2].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND2_PKTYPE][2].no_of_bits=
            ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[ROHC_CO_P6_RND2_PKTYPE][3].field_id=ROHC_TCP_MSN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND2_PKTYPE][3].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_RND2_PKTYPE][4].field_id=ROHC_TCP_PSH_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND2_PKTYPE][4].no_of_bits=1;
    rohc_c_pkt_type[ROHC_CO_P6_RND2_PKTYPE][5].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND2_PKTYPE][5].no_of_bits=3;
    rohc_c_pkt_type[ROHC_CO_P6_RND2_PKTYPE][6].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_co_rnd2p6_pkt");
} /* end function rohc_c_make_co_rnd2p6_pkt*/
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_co_rnd3p6_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose RND3 packet for TCP Profile
**          i.e Send acknowledgment number LSBs
**      ARGUMENTS:
**               packet type
**      RETURN VALUE:
**              void
**      NOTES:
**
*****************************************************************************/
static void
rohc_c_make_co_rnd3p6_pkt
    ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_co_rnd3p6_pkt");
    rohc_c_pkt_type[ROHC_CO_P6_RND3_PKTYPE][0].field_id=ROHC_RND3_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND3_PKTYPE][0].no_of_bits=1U;
    rohc_c_pkt_type[ROHC_CO_P6_RND3_PKTYPE][1].field_id=ROHC_TCP_ACKNO_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND3_PKTYPE][1].no_of_bits=
            ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_CO_P6_RND3_PKTYPE][1].no_of_bits_occ=7;
    rohc_c_pkt_type[ROHC_CO_P6_RND3_PKTYPE][2].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND3_PKTYPE][2].no_of_bits=
            ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[ROHC_CO_P6_RND3_PKTYPE][3].field_id=ROHC_TCP_ACKNO_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND3_PKTYPE][3].no_of_bits=
            ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_CO_P6_RND3_PKTYPE][3].no_of_bits_occ=8;
    rohc_c_pkt_type[ROHC_CO_P6_RND3_PKTYPE][4].field_id=ROHC_TCP_MSN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND3_PKTYPE][4].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_RND3_PKTYPE][5].field_id=ROHC_TCP_PSH_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND3_PKTYPE][5].no_of_bits=1;
    rohc_c_pkt_type[ROHC_CO_P6_RND3_PKTYPE][6].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND3_PKTYPE][6].no_of_bits=3;
    rohc_c_pkt_type[ROHC_CO_P6_RND3_PKTYPE][7].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_co_rnd3p6_pkt");
} /* end function rohc_c_make_co_rnd3p6_pkt*/
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_co_rnd4p6_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose RND4 packet for TCP Profile
**          i.e Send acknowledgment number scaled
**      ARGUMENTS:
**               packet type
**      RETURN VALUE:
**              void
**      NOTES:
**
*****************************************************************************/
static void
rohc_c_make_co_rnd4p6_pkt
    ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_co_rnd4p6_pkt");
    rohc_c_pkt_type[ROHC_CO_P6_RND4_PKTYPE][0].field_id=ROHC_RND4_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND4_PKTYPE][0].no_of_bits=4U;
    rohc_c_pkt_type[ROHC_CO_P6_RND4_PKTYPE][1].field_id=ROHC_TCP_SCALED_ACKNO_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND4_PKTYPE][1].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_RND4_PKTYPE][2].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND4_PKTYPE][2].no_of_bits=
            ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[ROHC_CO_P6_RND4_PKTYPE][3].field_id=ROHC_TCP_MSN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND4_PKTYPE][3].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_RND4_PKTYPE][4].field_id=ROHC_TCP_PSH_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND4_PKTYPE][4].no_of_bits=1;
    rohc_c_pkt_type[ROHC_CO_P6_RND4_PKTYPE][5].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND4_PKTYPE][5].no_of_bits=3;
    rohc_c_pkt_type[ROHC_CO_P6_RND4_PKTYPE][6].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_co_rnd4p6_pkt");
} /* end function rohc_c_make_co_rnd4p6_pkt*/
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_co_rnd5p6_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose RND5 packet for TCP Profile
**          i.e Send ACK and sequence number
**      ARGUMENTS:
**               packet type
**      RETURN VALUE:
**              void
**      NOTES:
**
*****************************************************************************/
static void
rohc_c_make_co_rnd5p6_pkt
    ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_co_rnd5p6_pkt");
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][0].field_id=ROHC_RND5_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][0].no_of_bits=3U;
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][1].field_id=ROHC_TCP_PSH_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][1].no_of_bits=1;
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][2].field_id=ROHC_TCP_MSN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][2].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][3].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][3].no_of_bits=
            ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][4].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][4].no_of_bits=3;
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][5].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][5].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][5].no_of_bits_occ=5;
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][6].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][6].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][6].no_of_bits_occ=8;
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][7].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][7].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][7].no_of_bits_occ=1;
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][8].field_id=ROHC_TCP_ACKNO_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][8].no_of_bits=15;
    rohc_c_pkt_type[ROHC_CO_P6_RND5_PKTYPE][9].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_co_rnd5p6_pkt");
} /* end function rohc_c_make_co_rnd5p6_pkt*/
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_co_rnd6p6_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose RND6 packet for TCP Profile
**          i.e Send both ACK and scaled sequence number LSBs
**      ARGUMENTS:
**               packet type
**      RETURN VALUE:
**              void
**      NOTES:
**
*****************************************************************************/
static void
rohc_c_make_co_rnd6p6_pkt
    ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_co_rnd6p6_pkt");
    rohc_c_pkt_type[ROHC_CO_P6_RND6_PKTYPE][0].field_id=ROHC_RND6_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND6_PKTYPE][0].no_of_bits=4U;
    rohc_c_pkt_type[ROHC_CO_P6_RND6_PKTYPE][1].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND6_PKTYPE][1].no_of_bits=3;
    rohc_c_pkt_type[ROHC_CO_P6_RND6_PKTYPE][2].field_id=ROHC_TCP_PSH_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND6_PKTYPE][2].no_of_bits=1;
    rohc_c_pkt_type[ROHC_CO_P6_RND6_PKTYPE][3].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND6_PKTYPE][3].no_of_bits=
            ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[ROHC_CO_P6_RND6_PKTYPE][4].field_id=ROHC_TCP_ACKNO_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND6_PKTYPE][4].no_of_bits=16;
    rohc_c_pkt_type[ROHC_CO_P6_RND6_PKTYPE][5].field_id=ROHC_TCP_MSN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND6_PKTYPE][5].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_RND6_PKTYPE][6].field_id=ROHC_TCP_SCALED_SN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND6_PKTYPE][6].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_RND6_PKTYPE][7].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_co_rnd6p6_pkt");
} /* end function rohc_c_make_co_rnd6p6_pkt*/
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_co_rnd7p6_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose RND7 packet for TCP Profile
**          i.e Send ACK and window
**      ARGUMENTS:
**               packet type
**      RETURN VALUE:
**              void
**      NOTES:
**
*****************************************************************************/
static void
rohc_c_make_co_rnd7p6_pkt
    ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_co_rnd7p6_pkt");
    rohc_c_pkt_type[ROHC_CO_P6_RND7_PKTYPE][0].field_id=ROHC_RND7_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND7_PKTYPE][0].no_of_bits=6U;
    rohc_c_pkt_type[ROHC_CO_P6_RND7_PKTYPE][1].field_id=ROHC_TCP_ACKNO_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND7_PKTYPE][1].no_of_bits=
            ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_CO_P6_RND7_PKTYPE][1].no_of_bits_occ=2;
    rohc_c_pkt_type[ROHC_CO_P6_RND7_PKTYPE][2].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND7_PKTYPE][2].no_of_bits=
            ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[ROHC_CO_P6_RND7_PKTYPE][3].field_id=ROHC_TCP_ACKNO_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND7_PKTYPE][3].no_of_bits=
            ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_CO_P6_RND7_PKTYPE][3].no_of_bits_occ=16;
    rohc_c_pkt_type[ROHC_CO_P6_RND7_PKTYPE][4].field_id=ROHC_TCP_WINDOW_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND7_PKTYPE][4].no_of_bits=16;
    rohc_c_pkt_type[ROHC_CO_P6_RND7_PKTYPE][5].field_id=ROHC_TCP_MSN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND7_PKTYPE][5].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_RND7_PKTYPE][6].field_id=ROHC_TCP_PSH_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND7_PKTYPE][6].no_of_bits=1;
    rohc_c_pkt_type[ROHC_CO_P6_RND7_PKTYPE][7].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND7_PKTYPE][7].no_of_bits=3;
    rohc_c_pkt_type[ROHC_CO_P6_RND7_PKTYPE][8].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_co_rnd7p6_pkt");
} /* end function rohc_c_make_co_rnd7p6_pkt*/
/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_co_rnd8p6_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose RND8 packet for TCP Profile
**          i.e An extended packet type for seldom-changing fields Can send LSBs of TTL, 
**          RSF flags, change ECN behavior, and options list
**      ARGUMENTS:
**               packet type
**      RETURN VALUE:
**              void
**      NOTES:
**
*****************************************************************************/
static void
rohc_c_make_co_rnd8p6_pkt
    ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_co_rnd8p6_pkt");
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][0].field_id=ROHC_RND8_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][0].no_of_bits=5U;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][1].field_id=ROHC_TCP_RSF_FLAGS_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][1].no_of_bits=2;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][2].field_id=ROHC_TCP_LIST_PRESENT_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][2].no_of_bits=1;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][3].field_id=ROHC_CID_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][3].no_of_bits=
            ROHC_ENCTYPE_VAR_NO_BITS;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][4].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][4].no_of_bits=7;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][5].field_id=ROHC_TCP_MSN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][5].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][5].no_of_bits_occ=1;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][6].field_id=ROHC_TCP_MSN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][6].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][6].no_of_bits_occ=3;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][7].field_id=ROHC_TCP_PSH_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][7].no_of_bits=1;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][8].field_id=ROHC_INNERIP4_TTL_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][8].no_of_bits=3;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][9].field_id=ROHC_TCP_ECN_USED_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][9].no_of_bits=1;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][10].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][10].no_of_bits=16;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][11].field_id=ROHC_TCP_ACKNO_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][11].no_of_bits=16;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][12].field_id=ROHC_TCP_OPT_LIST_FID;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][12].no_of_bits=ROHC_ENCTYPE_TCP_OPTIONS_LIST;
    rohc_c_pkt_type[ROHC_CO_P6_RND8_PKTYPE][13].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_co_rnd8p6_pkt");
} /* end function rohc_c_make_co_rnd8p6_pkt*/



/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_co_seq1_p6_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose SEQ 1 packet for TCP Profile 
**      ARGUMENTS:
**              void
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
static void 
rohc_c_make_co_seq1_p6_pkt
     ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_co_seq1_p6_pkt");
    rohc_c_pkt_type[ROHC_CO_P6_SEQ1_PKTYPE][0].field_id=ROHC_TCP_DISC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ1_PKTYPE][0].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ1_PKTYPE][1].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ1_PKTYPE][1].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ1_PKTYPE][2].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ1_PKTYPE][2].no_of_bits=16;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ1_PKTYPE][3].field_id=ROHC_TCP_MSN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ1_PKTYPE][3].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ1_PKTYPE][4].field_id=ROHC_TCP_PSH_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ1_PKTYPE][4].no_of_bits=1;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ1_PKTYPE][5].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ1_PKTYPE][5].no_of_bits=3;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ1_PKTYPE][6].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_co_seq1_p6_pkt");
}/* end function rohc_c_make_co_seq1_p6_pkt */

/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_co_seq2_p6_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose SEQ 2 packet for TCP Profile 
**      ARGUMENTS:
**              void 
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
static void 
rohc_c_make_co_seq2_p6_pkt
     ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_co_seq2_p6_pkt");
    rohc_c_pkt_type[ROHC_CO_P6_SEQ2_PKTYPE][0].field_id=ROHC_TCP_DISC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ2_PKTYPE][0].no_of_bits=5;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ2_PKTYPE][1].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ2_PKTYPE][1].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ2_PKTYPE][1].no_of_bits_occ=3;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ2_PKTYPE][2].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ2_PKTYPE][2].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ2_PKTYPE][2].no_of_bits_occ=4;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ2_PKTYPE][3].field_id=ROHC_TCP_SCALED_SN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ2_PKTYPE][3].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ2_PKTYPE][4].field_id=ROHC_TCP_MSN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ2_PKTYPE][4].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ2_PKTYPE][5].field_id=ROHC_TCP_PSH_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ2_PKTYPE][5].no_of_bits=1;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ2_PKTYPE][6].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ2_PKTYPE][6].no_of_bits=3;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ2_PKTYPE][7].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_co_seq2_p6_pkt");
}/* end function rohc_c_make_co_seq2_p6_pkt */

/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_co_seq3_p6_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose SEQ 3 packet for TCP Profile 
**      ARGUMENTS:
**              void 
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
static void 
rohc_c_make_co_seq3_p6_pkt
     ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_co_seq3_p6_pkt");
    rohc_c_pkt_type[ROHC_CO_P6_SEQ3_PKTYPE][0].field_id=ROHC_TCP_DISC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ3_PKTYPE][0].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ3_PKTYPE][1].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ3_PKTYPE][1].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ3_PKTYPE][2].field_id=ROHC_TCP_ACKNO_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ3_PKTYPE][2].no_of_bits=16;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ3_PKTYPE][3].field_id=ROHC_TCP_MSN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ3_PKTYPE][3].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ3_PKTYPE][4].field_id=ROHC_TCP_PSH_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ3_PKTYPE][4].no_of_bits=1;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ3_PKTYPE][5].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ3_PKTYPE][5].no_of_bits=3;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ3_PKTYPE][6].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_co_seq3_p6_pkt");
}/* end function rohc_c_make_co_seq3_p6_pkt */

/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_co_seq4_p6_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose SEQ 4 packet for TCP Profile 
**      ARGUMENTS:
**              void 
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
static void 
rohc_c_make_co_seq4_p6_pkt
     ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_co_seq4_p6_pkt");
    rohc_c_pkt_type[ROHC_CO_P6_SEQ4_PKTYPE][0].field_id=ROHC_TCP_DISC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ4_PKTYPE][0].no_of_bits=1;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ4_PKTYPE][1].field_id=ROHC_TCP_SCALED_ACKNO_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ4_PKTYPE][1].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ4_PKTYPE][2].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ4_PKTYPE][2].no_of_bits=3;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ4_PKTYPE][3].field_id=ROHC_TCP_MSN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ4_PKTYPE][3].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ4_PKTYPE][4].field_id=ROHC_TCP_PSH_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ4_PKTYPE][4].no_of_bits=1;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ4_PKTYPE][5].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ4_PKTYPE][5].no_of_bits=3;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ4_PKTYPE][6].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_co_seq4_p6_pkt");
}/* end function rohc_c_make_co_seq4_p6_pkt */

/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_co_seq5_p6_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose SEQ 5 packet for TCP Profile 
**      ARGUMENTS:
**              void
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
static void 
rohc_c_make_co_seq5_p6_pkt
     ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_co_seq5_p6_pkt");
    rohc_c_pkt_type[ROHC_CO_P6_SEQ5_PKTYPE][0].field_id=ROHC_TCP_DISC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ5_PKTYPE][0].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ5_PKTYPE][1].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ5_PKTYPE][1].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ5_PKTYPE][2].field_id=ROHC_TCP_ACKNO_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ5_PKTYPE][2].no_of_bits=16;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ5_PKTYPE][3].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ5_PKTYPE][3].no_of_bits=16;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ5_PKTYPE][4].field_id=ROHC_TCP_MSN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ5_PKTYPE][4].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ5_PKTYPE][5].field_id=ROHC_TCP_PSH_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ5_PKTYPE][5].no_of_bits=1;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ5_PKTYPE][6].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ5_PKTYPE][6].no_of_bits=3;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ5_PKTYPE][7].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_co_seq5_p6_pkt");
}/* end function rohc_c_make_co_seq5_p6_pkt */

/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_co_seq6_p6_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose SEQ 6 packet for TCP Profile 
**      ARGUMENTS:
**              void
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
static void 
rohc_c_make_co_seq6_p6_pkt
     ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_co_seq6_p6_pkt");
    rohc_c_pkt_type[ROHC_CO_P6_SEQ6_PKTYPE][0].field_id=ROHC_TCP_DISC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ6_PKTYPE][0].no_of_bits=5;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ6_PKTYPE][1].field_id=ROHC_TCP_SCALED_SN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ6_PKTYPE][1].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ6_PKTYPE][1].no_of_bits_occ=3;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ6_PKTYPE][2].field_id=ROHC_TCP_SCALED_SN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ6_PKTYPE][2].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ6_PKTYPE][2].no_of_bits_occ=1;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ6_PKTYPE][3].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ6_PKTYPE][3].no_of_bits=7;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ6_PKTYPE][4].field_id=ROHC_TCP_ACKNO_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ6_PKTYPE][4].no_of_bits=16;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ6_PKTYPE][5].field_id=ROHC_TCP_MSN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ6_PKTYPE][5].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ6_PKTYPE][6].field_id=ROHC_TCP_PSH_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ6_PKTYPE][6].no_of_bits=1;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ6_PKTYPE][7].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ6_PKTYPE][7].no_of_bits=3;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ6_PKTYPE][8].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_co_seq6_p6_pkt");
}/* end function rohc_c_make_co_seq6_p6_pkt */

/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_co_seq7_p6_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose SEQ 7 packet for TCP Profile 
**      ARGUMENTS:
**              void 
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
static void 
rohc_c_make_co_seq7_p6_pkt
     ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_co_seq7_p6_pkt");
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][0].field_id=ROHC_TCP_DISC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][0].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][1].field_id=ROHC_TCP_WINDOW_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][1].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][1].no_of_bits_occ=4;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][2].field_id=ROHC_TCP_WINDOW_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][2].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][2].no_of_bits_occ=8;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][3].field_id=ROHC_TCP_WINDOW_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][3].no_of_bits=ROHC_ENCTYPE_SPLIT_FIELD;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][3].no_of_bits_occ=3;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][4].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][4].no_of_bits=5;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][5].field_id=ROHC_TCP_ACKNO_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][5].no_of_bits=16;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][6].field_id=ROHC_TCP_MSN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][6].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][7].field_id=ROHC_TCP_PSH_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][7].no_of_bits=1;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][8].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][8].no_of_bits=3;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ7_PKTYPE][9].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_co_seq7_p6_pkt");
}/* end function rohc_c_make_co_seq7_p6_pkt */

/*****************************************************************************
**      FUNCTION:
**              rohc_c_make_co_seq8_p6_pkt
******************************************************************************
**
**      DESCRIPTION:
**          compose SEQ 8 packet for TCP Profile 
**      ARGUMENTS:
**              void 
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
static void 
rohc_c_make_co_seq8_p6_pkt
     ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_co_seq8_p6_pkt");
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][0].field_id=ROHC_TCP_DISC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][0].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][1].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][1].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][2].field_id=ROHC_TCP_LIST_PRESENT_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][2].no_of_bits=1;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][3].field_id=ROHC_CRC_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][3].no_of_bits=7;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][4].field_id=ROHC_TCP_MSN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][4].no_of_bits=4;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][5].field_id=ROHC_TCP_PSH_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][5].no_of_bits=1;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][6].field_id=ROHC_INNERIP4_TTL_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][6].no_of_bits=3;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][7].field_id=ROHC_TCP_ECN_FLAG_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][7].no_of_bits=1;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][8].field_id=ROHC_TCP_ACKNO_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][8].no_of_bits=15;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][9].field_id=ROHC_TCP_RSF_FLAGS_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][9].no_of_bits=2;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][10].field_id=ROHC_SN_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][10].no_of_bits=14;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][11].field_id=ROHC_TCP_OPT_LIST_FID;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][11].no_of_bits=ROHC_ENCTYPE_TCP_OPTIONS_LIST;
    rohc_c_pkt_type[ROHC_CO_P6_SEQ8_PKTYPE][12].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_co_seq8_p6_pkt");
}/* end function rohc_c_make_co_seq8_p6_pkt */
/*****************************************************************************
 * **      FUNCTION:
 * **              rohc_c_make_irreg_chain_ipv6_outer_without_ttl
 * **              IRREGULAR CHAIN TYPE INFO FILLING
 * ******************************************************************************
 * **
 * **      DESCRIPTION:
 * **
 * **      ARGUMENTS:
 * **               None
 * **      RETURN VALUE:
 * **              void
 * **      NOTES:
 * **
 * *****************************************************************************/
static void
rohc_c_make_irreg_chain_ipv6_outer_without_ttl
     ROHC_NO_PARAMS()
{

    ROHC_ENTER_FN("rohc_c_make_irreg_chain_ipv6_outer_without_ttl");
    rohc_c_irreg_chain_type[ROHC_P6_IPV6_OUTER_WITHOUT_TTL_IRREG_CHAIN][0].field_id=ROHC_INVALID_FID;
    rohc_c_irreg_chain_type[ROHC_P6_IPV6_OUTER_WITHOUT_TTL_IRREG_CHAIN][0].no_of_bits=2;
    ROHC_EXIT_FN("rohc_c_make_irreg_chain_ipv6_outer_without_ttl");
}/* end function rohc_c_make_irreg_chain_ipv6_outer_without_ttl*/

/*****************************************************************************
 * **      FUNCTION:
 * **              rohc_c_make_irreg_chain_ipv6_outer_with_ttl
 * **              IRREGULAR CHAIN TYPE INFO FILLING
 * ******************************************************************************
 * **
 * **      DESCRIPTION:
 * **
 * **      ARGUMENTS:
 * **               None
 * **      RETURN VALUE:
 * **              void
 * **      NOTES:
 * **
 * *****************************************************************************/
static void
rohc_c_make_irreg_chain_ipv6_outer_with_ttl
     ROHC_NO_PARAMS()
{

    ROHC_ENTER_FN("rohc_c_make_irreg_chain_ipv6_outer_with_ttl");
    rohc_c_irreg_chain_type[ROHC_P6_IPV6_OUTER_WITH_TTL_IRREG_CHAIN][0].field_id=ROHC_INVALID_FID;
    rohc_c_irreg_chain_type[ROHC_P6_IPV6_OUTER_WITH_TTL_IRREG_CHAIN][0].no_of_bits=2;
    ROHC_EXIT_FN("rohc_c_make_irreg_chain_ipv6_outer_with_ttl");
}/* end function rohc_c_make_irreg_chain_ipv6_outer_with_ttl */

/*****************************************************************************
 * **      FUNCTION:
 * **              rohc_c_make_irreg_chain_ipv6_innermost
 * **              IRREGULAR CHAIN TYPE INFO FILLING
 * ******************************************************************************
 * **
 * **      DESCRIPTION:
 * **
 * **      ARGUMENTS:
 * **               None
 * **      RETURN VALUE:
 * **              void
 * **      NOTES:
 * **
 * *****************************************************************************/
static void
rohc_c_make_irreg_chain_ipv6_innermost
     ROHC_NO_PARAMS()
{

    ROHC_ENTER_FN("rohc_c_make_irreg_chain_ipv6_innermost");
    rohc_c_irreg_chain_type[ROHC_P6_IPV6_INNERMOST_IRREG_CHAIN][0].field_id=ROHC_INVALID_FID;
    rohc_c_irreg_chain_type[ROHC_P6_IPV6_INNERMOST_IRREG_CHAIN][0].no_of_bits=2;
    ROHC_EXIT_FN("rohc_c_make_irreg_chain_ipv6_innermost");
}/* end function rohc_c_make_irreg_chain_ipv6_innermost */

/*****************************************************************************
 * **      FUNCTION:
 * **              rohc_c_make_irreg_chain_ipv4_outer_without_ttl
 * **              IRREGULAR CHAIN TYPE INFO FILLING
 * ******************************************************************************
 * **
 * **      DESCRIPTION:
 * **
 * **      ARGUMENTS:
 * **               None
 * **      RETURN VALUE:
 * **              void
 * **      NOTES:
 * **
 * *****************************************************************************/
static void
rohc_c_make_irreg_chain_ipv4_outer_without_ttl
     ROHC_NO_PARAMS()
{

    ROHC_ENTER_FN("rohc_c_make_irreg_chain_ipv4_outer_without_ttl");
    rohc_c_irreg_chain_type[ROHC_P6_IPV4_OUTER_WITHOUT_TTL_IRREG_CHAIN][0].field_id=ROHC_INVALID_FID;
    rohc_c_irreg_chain_type[ROHC_P6_IPV4_OUTER_WITHOUT_TTL_IRREG_CHAIN][0].no_of_bits=2;
    ROHC_EXIT_FN("rohc_c_make_irreg_chain_ipv4_outer_without_ttl");
}/* end function rohc_c_make_irreg_chain_ipv4_outer_without_ttl*/

/*****************************************************************************
 * **      FUNCTION:
 * **              rohc_c_make_irreg_chain_ipv4_outer_with_ttl
 * **              IRREGULAR CHAIN TYPE INFO FILLING
 * ******************************************************************************
 * **
 * **      DESCRIPTION:
 * **
 * **      ARGUMENTS:
 * **               None
 * **      RETURN VALUE:
 * **              void
 * **      NOTES:
 * **
 * *****************************************************************************/
static void
rohc_c_make_irreg_chain_ipv4_outer_with_ttl
     ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_make_irreg_chain_ipv4_outer_with_ttl");
    rohc_c_irreg_chain_type[ROHC_P6_IPV4_OUTER_WITH_TTL_IRREG_CHAIN][0].field_id=ROHC_INVALID_FID;
    rohc_c_irreg_chain_type[ROHC_P6_IPV4_OUTER_WITH_TTL_IRREG_CHAIN][0].no_of_bits=2;
    ROHC_EXIT_FN("rohc_c_make_irreg_chain_ipv4_outer_with_ttl");
}/* end function rohc_c_make_irreg_chain_ipv4_outer_with_ttl */

/*****************************************************************************
 * **      FUNCTION:
 * **              rohc_c_make_irreg_chain_ipv4_innermost
 * **              IRREGULAR CHAIN TYPE INFO FILLING
 * ******************************************************************************
 * **
 * **      DESCRIPTION:
 * **
 * **      ARGUMENTS:
 * **               None
 * **      RETURN VALUE:
 * **              void
 * **      NOTES:
 * **
 * *****************************************************************************/
static void
rohc_c_make_irreg_chain_ipv4_innermost
     ROHC_NO_PARAMS()
{

    ROHC_ENTER_FN("rohc_c_make_irreg_chain_ipv4_innermost");
    rohc_c_irreg_chain_type[ROHC_P6_IPV4_INNERMOST_IRREG_CHAIN][0].field_id=ROHC_INNERIP_ID_FID;
    rohc_c_irreg_chain_type[ROHC_P6_IPV4_INNERMOST_IRREG_CHAIN][0].no_of_bits=16;
    rohc_c_irreg_chain_type[ROHC_P6_IPV4_INNERMOST_IRREG_CHAIN][1].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_irreg_chain_ipv4_innermost");
}/* end function rohc_c_make_irreg_chain_ipv6_innermost */

/*****************************************************************************
 * **      FUNCTION:
 * **              rohc_c_make_irreg_chain_tcp
 * **              IRREGULAR CHAIN TYPE INFO FILLING
 * ******************************************************************************
 * **
 * **      DESCRIPTION:
 * **
 * **      ARGUMENTS:
 * **               None
 * **      RETURN VALUE:
 * **              void
 * **      NOTES:
 * **
 * *****************************************************************************/
static void
rohc_c_make_irreg_chain_tcp
     ROHC_NO_PARAMS()
{

    ROHC_ENTER_FN("rohc_c_make_irreg_chain_tcp");
    rohc_c_irreg_chain_type[ROHC_P6_TCP_IRREG_CHAIN][0].field_id=ROHC_INNERIP4_ECN_FID;
    rohc_c_irreg_chain_type[ROHC_P6_TCP_IRREG_CHAIN][0].no_of_bits=2;
    rohc_c_irreg_chain_type[ROHC_P6_TCP_IRREG_CHAIN][1].field_id=ROHC_TCP_RES_FLAG_FID;
    rohc_c_irreg_chain_type[ROHC_P6_TCP_IRREG_CHAIN][1].no_of_bits=4;
    rohc_c_irreg_chain_type[ROHC_P6_TCP_IRREG_CHAIN][2].field_id=ROHC_TCP_ECN_FLAG_FID;
    rohc_c_irreg_chain_type[ROHC_P6_TCP_IRREG_CHAIN][2].no_of_bits=2;
    rohc_c_irreg_chain_type[ROHC_P6_TCP_IRREG_CHAIN][3].field_id=ROHC_TCP_CKSUM_FID;
    rohc_c_irreg_chain_type[ROHC_P6_TCP_IRREG_CHAIN][3].no_of_bits=16;
    rohc_c_irreg_chain_type[ROHC_P6_TCP_IRREG_CHAIN][4].field_id=ROHC_INVALID_FID;
    ROHC_EXIT_FN("rohc_c_make_irreg_chain_tcp");
}/* end function rohc_c_make_irreg_chain_tcp */

/*****************************************************************************
**      FUNCTION:
**              rohc_c_pbm_init
******************************************************************************
**
**      DESCRIPTION:
**          C-PBM Initalisation function 
**      ARGUMENTS:
**               None
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
void 
rohc_c_pbm_init
    ROHC_NO_PARAMS()
{
    ROHC_ENTER_FN("rohc_c_pbm_init");

    rohc_fill_global_pkt_types_to_stats();
    rohc_fill_global_constant_field_values();
    
    rohc_c_make_r0p1_pkt();
    
    rohc_c_make_r0crcp1_pkt();
    
    rohc_c_make_uo0p1_pkt();
 

    rohc_c_make_r1p1_pkt(ROHC_R1P1_PKTTYPE);
    rohc_c_make_r1p1_pkt(ROHC_R1E0P1_PKTTYPE);
    rohc_c_make_ext0_tsp1_pkt(ROHC_R1E0P1_PKTTYPE,6);
    rohc_c_make_r1p1_pkt(ROHC_R1E1P1_PKTTYPE);
    rohc_c_make_ext1_notp1_pkt(ROHC_R1E1P1_PKTTYPE,6);
    rohc_c_make_r1p1_pkt(ROHC_R1E2P1_PKTTYPE);
    rohc_c_make_ext2_notp1_pkt(ROHC_R1E2P1_PKTTYPE,6);
    rohc_c_make_r1p1_pkt(ROHC_R1E3P1_PKTTYPE);
    rohc_c_make_ext3p1_pkt(ROHC_R1E3P1_PKTTYPE,6);

    rohc_c_make_r1idp1_pkt(ROHC_R1IDP1_PKTTYPE);
    rohc_c_make_r1idp1_pkt(ROHC_R1IDE0P1_PKTTYPE);
    rohc_c_make_ext0_idp1_pkt(ROHC_R1IDE0P1_PKTTYPE,7);
    rohc_c_make_r1idp1_pkt(ROHC_R1IDE1P1_PKTTYPE);
    rohc_c_make_ext1_t0p1_pkt(ROHC_R1IDE1P1_PKTTYPE,7);
    rohc_c_make_r1idp1_pkt(ROHC_R1IDE2P1_PKTTYPE);
    rohc_c_make_ext2_t0p1_pkt(ROHC_R1IDE2P1_PKTTYPE,7);
    rohc_c_make_r1idp1_pkt(ROHC_R1IDE3P1_PKTTYPE);
    rohc_c_make_ext3p1_pkt(ROHC_R1IDE3P1_PKTTYPE,7);

    rohc_c_make_r1tsp1_pkt(ROHC_R1TSP1_PKTTYPE);
    rohc_c_make_r1tsp1_pkt(ROHC_R1TSE0P1_PKTTYPE);
    rohc_c_make_ext0_tsp1_pkt(ROHC_R1TSE0P1_PKTTYPE,7);
    rohc_c_make_r1tsp1_pkt(ROHC_R1TSE1P1_PKTTYPE);
    rohc_c_make_ext1_t1p1_pkt(ROHC_R1TSE1P1_PKTTYPE,7);
    rohc_c_make_r1tsp1_pkt(ROHC_R1TSE2P1_PKTTYPE);
    rohc_c_make_ext2_t1p1_pkt(ROHC_R1TSE2P1_PKTTYPE,7);
    rohc_c_make_r1tsp1_pkt(ROHC_R1TSE3P1_PKTTYPE);
    rohc_c_make_ext3p1_pkt(ROHC_R1TSE3P1_PKTTYPE,7);

    rohc_c_make_uo1p1_pkt();

    rohc_c_make_uo1idp1_pkt(ROHC_UO1IDP1_PKTTYPE);
    rohc_c_make_uo1idp1_pkt(ROHC_UO1IDE0P1_PKTTYPE);
    rohc_c_make_ext0_idp1_pkt(ROHC_UO1IDE0P1_PKTTYPE,7);
    rohc_c_make_uo1idp1_pkt(ROHC_UO1IDE1P1_PKTTYPE);
    rohc_c_make_ext1_t0p1_pkt(ROHC_UO1IDE1P1_PKTTYPE,7);
    rohc_c_make_uo1idp1_pkt(ROHC_UO1IDE2P1_PKTTYPE);
    rohc_c_make_ext2_t0p1_pkt(ROHC_UO1IDE2P1_PKTTYPE,7);
    rohc_c_make_uo1idp1_pkt(ROHC_UO1IDE3P1_PKTTYPE);
    rohc_c_make_ext3p1_pkt(ROHC_UO1IDE3P1_PKTTYPE,7);
    
    rohc_c_make_uo1tsp1_pkt();
    
    rohc_c_make_uor2p1_pkt(ROHC_UOR2P1_PKTTYPE);
    rohc_c_make_uor2p1_pkt(ROHC_UOR2E0P1_PKTTYPE);
    rohc_c_make_ext0_tsp1_pkt(ROHC_UOR2E0P1_PKTTYPE,8);
    rohc_c_make_uor2p1_pkt(ROHC_UOR2E1P1_PKTTYPE);
    rohc_c_make_ext1_notp1_pkt(ROHC_UOR2E1P1_PKTTYPE,8);
    rohc_c_make_uor2p1_pkt(ROHC_UOR2E2P1_PKTTYPE);
    rohc_c_make_ext2_notp1_pkt(ROHC_UOR2E2P1_PKTTYPE,8);
    rohc_c_make_uor2p1_pkt(ROHC_UOR2E3P1_PKTTYPE);
    rohc_c_make_ext3p1_pkt(ROHC_UOR2E3P1_PKTTYPE,8);

    rohc_c_make_uor2idp1_pkt(ROHC_UOR2IDP1_PKTTYPE);
    rohc_c_make_uor2idp1_pkt(ROHC_UOR2IDE0P1_PKTTYPE);
    rohc_c_make_ext0_idp1_pkt(ROHC_UOR2IDE0P1_PKTTYPE,8);
    rohc_c_make_uor2idp1_pkt(ROHC_UOR2IDE1P1_PKTTYPE);
    rohc_c_make_ext1_t0p1_pkt(ROHC_UOR2IDE1P1_PKTTYPE,8);
    rohc_c_make_uor2idp1_pkt(ROHC_UOR2IDE2P1_PKTTYPE);
    rohc_c_make_ext2_t0p1_pkt(ROHC_UOR2IDE2P1_PKTTYPE,8);
    rohc_c_make_uor2idp1_pkt(ROHC_UOR2IDE3P1_PKTTYPE);
    rohc_c_make_ext3p1_pkt(ROHC_UOR2IDE3P1_PKTTYPE,8);

    rohc_c_make_uor2tsp1_pkt(ROHC_UOR2TSP1_PKTTYPE);
    rohc_c_make_uor2tsp1_pkt(ROHC_UOR2TSE0P1_PKTTYPE);
    /* SPR 20222 Changes Start */
    rohc_c_make_ext0_tsp1_pkt(ROHC_UOR2TSE0P1_PKTTYPE,8);
    /* SPR 20222 Changes End */
    rohc_c_make_uor2tsp1_pkt(ROHC_UOR2TSE1P1_PKTTYPE);
    rohc_c_make_ext1_t1p1_pkt(ROHC_UOR2TSE1P1_PKTTYPE,8);
    rohc_c_make_uor2tsp1_pkt(ROHC_UOR2TSE2P1_PKTTYPE);
    rohc_c_make_ext2_t1p1_pkt(ROHC_UOR2TSE2P1_PKTTYPE,8);
    rohc_c_make_uor2tsp1_pkt(ROHC_UOR2TSE3P1_PKTTYPE);
    rohc_c_make_ext3p1_pkt(ROHC_UOR2TSE3P1_PKTTYPE,8);
    
    rohc_c_make_r1p23_pkt(ROHC_R1P23_PKTTYPE);
    rohc_c_make_r1p23_pkt(ROHC_R1E0P23_PKTTYPE);
    rohc_c_make_ext0_p23_pkt(ROHC_R1E0P23_PKTTYPE,5);
    rohc_c_make_r1p23_pkt(ROHC_R1E1P23_PKTTYPE);
    rohc_c_make_ext1_p23_pkt(ROHC_R1E1P23_PKTTYPE,5);
    rohc_c_make_r1p23_pkt(ROHC_R1E2P23_PKTTYPE);
    rohc_c_make_ext2_p23_pkt(ROHC_R1E2P23_PKTTYPE,5);
    rohc_c_make_r1p23_pkt(ROHC_R1E3P23_PKTTYPE);
    rohc_c_make_ext3p23_pkt(ROHC_R1E3P23_PKTTYPE,5);
    
    rohc_c_make_uo1p23_pkt(ROHC_UO1P23_PKTTYPE);
    
    rohc_c_make_uor2p23_pkt(ROHC_UOR2P23_PKTTYPE);
    rohc_c_make_uor2p23_pkt(ROHC_UOR2E0P23_PKTTYPE);
    rohc_c_make_ext0_p23_pkt( ROHC_UOR2E0P23_PKTTYPE,5);
    rohc_c_make_uor2p23_pkt(ROHC_UOR2E1P23_PKTTYPE);
    rohc_c_make_ext1_p23_pkt( ROHC_UOR2E1P23_PKTTYPE,5);
    rohc_c_make_uor2p23_pkt(ROHC_UOR2E2P23_PKTTYPE);
    rohc_c_make_ext2_p23_pkt( ROHC_UOR2E2P23_PKTTYPE,5);
    rohc_c_make_uor2p23_pkt(ROHC_UOR2E3P23_PKTTYPE);
    rohc_c_make_ext3p23_pkt(ROHC_UOR2E3P23_PKTTYPE,5);
    /* PROFILE 6 CHANGES */
    rohc_c_make_co_common_pkt();
    rohc_c_make_co_rnd1p6_pkt();
    rohc_c_make_co_rnd2p6_pkt();
    rohc_c_make_co_rnd3p6_pkt();
    rohc_c_make_co_rnd4p6_pkt();
    rohc_c_make_co_rnd5p6_pkt();
    rohc_c_make_co_rnd6p6_pkt();
    rohc_c_make_co_rnd7p6_pkt();
    rohc_c_make_co_rnd8p6_pkt();
    rohc_c_make_co_seq1_p6_pkt();
    rohc_c_make_co_seq2_p6_pkt();
    rohc_c_make_co_seq3_p6_pkt();
    rohc_c_make_co_seq4_p6_pkt();
    rohc_c_make_co_seq5_p6_pkt();
    rohc_c_make_co_seq6_p6_pkt();
    rohc_c_make_co_seq7_p6_pkt();
    rohc_c_make_co_seq8_p6_pkt();
    /* INITIALIZING IRREGULAR CHAIN TYPE FIELD VALUES */
    rohc_c_make_irreg_chain_ipv6_outer_without_ttl();
    rohc_c_make_irreg_chain_ipv6_outer_with_ttl();
    rohc_c_make_irreg_chain_ipv6_innermost();
    rohc_c_make_irreg_chain_ipv4_outer_without_ttl();
    rohc_c_make_irreg_chain_ipv4_outer_with_ttl();
    rohc_c_make_irreg_chain_ipv4_innermost();
    rohc_c_make_irreg_chain_tcp();
    //rohc_c_make_irreg_chain_tcp_options();
    ROHC_EXIT_FN("rohc_c_pbm_init");
} /* end function rohc_c_pbm_init */
/*****************************************************************************
**      FUNCTION:
**              rohc_set8bit
******************************************************************************
**
**      DESCRIPTION:
**          Set bits in buffer  
**      ARGUMENTS:
**               Pointer to character buffer
**               Pointer to Bit offset in to buffer
**               Value  
**               No. of bits
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_set8bit
    ROHC_PARAMS((to,p_to_bit_pos,from,n),
    rohc_U8bit_t * * to _AND_
    rohc_U8bit_t * p_to_bit_pos _AND_
    rohc_U8bit_t from _AND_
    rohc_U8bit_t n)
{
    rohc_U8bit_t *p=*to,temp;
    ROHC_ENTER_FN("rohc_set8bit");    
    /* Get n L.S.B of from at to_bit_position */
    temp=(1<<n)-1;
    from&=temp;
    from<<=8-*p_to_bit_pos-n;
    /* Clear n bits in to starting from to_bit_position */
    temp=~(~0<<n);
    temp=~(temp<<(8-*p_to_bit_pos-n));
    *p=*p&temp;
    *p=*p|from;
    *p_to_bit_pos=(*p_to_bit_pos+n)%8;
    if(*p_to_bit_pos == 0)
    {
        (*to)++;
    }        
    ROHC_EXIT_FN("rohc_set8bit");
}   /*end function rohc_set8bit() */    
/*****************************************************************************
**      FUNCTION:
**              rohc_setbits
******************************************************************************
**
**      DESCRIPTION:
**          Set bits in buffer  
**      ARGUMENTS:
**               Pointer to character buffer
**               Pointer to Bit offset in to buffer
**               Value  
**               from bit osset from which value is to be read
**               No. of bits
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_setbits
    ROHC_PARAMS((to,p_to_bit_pos,from,from_bit_offset_in_32bit,no_of_bits),
    rohc_U8bit_t * * to _AND_
    rohc_U8bit_t * p_to_bit_pos _AND_
    rohc_U32bit_t from _AND_
    rohc_U8bit_t from_bit_offset_in_32bit _AND_
    /* from_bit_offset_in_32bit is from L.S.B. */
    rohc_U8bit_t no_of_bits)
{
    ROHC_ENTER_FN("rohc_setbits");
    if(!no_of_bits)
    {
        return;
    } 
    /*copy in msb form*/
    /* whenever bit offset is given
     < 8 bits are to be copied to "to" */    
    if(from_bit_offset_in_32bit)
    {
        from=from>>from_bit_offset_in_32bit;
        rohc_set8bit(to,p_to_bit_pos,(rohc_U8bit_t)from,no_of_bits);
    }   /*end if bit offset */
    else
    { 
        /*no bit offset can be given when more than 8 bits are to
       be copied, */
       rohc_U8bit_t no_of_bytes=(no_of_bits+7)/8;
       rohc_U32bit_t val;
       while(no_of_bytes)
       {
            val=from;   
            if(no_of_bits%8)
            {
                val=from>>(no_of_bytes-1)*8;
                rohc_set8bit(to,p_to_bit_pos,(rohc_U8bit_t)val,(rohc_U8bit_t)
                        (no_of_bits%8));
                no_of_bits-=no_of_bits%8;
            }    /*end if bit copy*/  
            else
            {   
                **to=val>>((no_of_bytes-1)*8);
                (*to)++;
                no_of_bits-=8;        
            } /* end else byte copy*/ 
            no_of_bytes--;
       } /*end while byte */
    }   /*end else no bit offset present */
    ROHC_EXIT_FN("rohc_setbits");
}   /*end function rohc_setbits() */

/*****************************************************************************
**      FUNCTION: rohc_encode_rsf_field
**              
******************************************************************************
**
**      DESCRIPTION:
**          Set bits in buffer  
**      ARGUMENTS:
**               Pointer to character buffer
**               Pointer to Bit offset in to buffer
**               Value  
**               from bit osset from which value is to be read
**               No. of bits
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
static void 
rohc_encode_rsf_field
    ROHC_PARAMS((p_p_rohc_pkt,p_csc,p_to_bit_pos),
    rohc_U8bit_t * * p_p_rohc_pkt _AND_
    rohc_csc_tcp_t  *p_csc      _AND_
    rohc_U8bit_t * p_to_bit_pos
    )
{
    ROHC_ENTER_FN("rohc_encode_rsf_field");
	if(p_csc->field_value[ROHC_TCP_RST_FLAG_FID-ROHC_VAR_FID_BASE])
	{
		rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
				1,0,2);
	}
	else if(p_csc->field_value[ROHC_TCP_SYN_FLAG_FID-ROHC_VAR_FID_BASE])
	{
		rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
				2,0,2);
	}
	else if(p_csc->field_value[ROHC_TCP_FIN_FLAG_FID-ROHC_VAR_FID_BASE])
	{
		rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
				3,0,2);
	}
	else
	{
		rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
				0,0,2);
	}
}

/*****************************************************************************
**      FUNCTION: rohc_c_prof6_calc_bits
**              
******************************************************************************
**
**      DESCRIPTION:
**          Calculate self descibing indicator values for sn and ack 
**          based on lsb encoded no of bits
**      ARGUMENTS:
**               No. of bits after lsb encoding
**      RETURN VALUE:
**              Self describing Indicator
**      NOTES:
**               
*****************************************************************************/
static rohc_return_t 
rohc_c_prof6_calc_bits
    ROHC_PARAMS((no_of_bits),
    rohc_U32bit_t  no_of_bits)
{
    ROHC_ENTER_FN("rohc_c_prof6_calc_bits");
    rohc_U8bit_t ret_val;
    if(no_of_bits == 0)  
    {
        ret_val = 0;
    }
    else if(no_of_bits <= 8) 
    {
        ret_val = 1;
    }
    else if(no_of_bits <= 16)
    {
        ret_val = 2;
    }
    else
    {
        ret_val =  3;
    }
    return ret_val;
}

/*****************************************************************************
**      FUNCTION:
**              rohc_p6_variable_length_32_enc
******************************************************************************
**
**      DESCRIPTION:
**              set buffer based on variable length 32 bit encoding scheme  
**      ARGUMENTS:
**               Pointer to ROHC packet buffer
**               Bit position in above buffer
**               Field identifier
**               Pointer to CSC
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
static void 
rohc_p6_variable_length_32_enc
    ROHC_PARAMS((p_p_rohc_pkt,p_to_bit_pos,field_id,p_csc),
    rohc_U8bit_t            ** p_p_rohc_pkt _AND_
    rohc_U8bit_t            * p_to_bit_pos  _AND_
    rohc_U8bit_t            field_id        _AND_
    rohc_csc_tcp_t  * p_csc)
{
    ROHC_ENTER_FN("rohc_p6_variable_length_32_enc");
    rohc_U8bit_t flag;

    switch(field_id)
    {
        case ROHC_SN_FID:
            flag = p_csc->ctrl_field_val_p6[ROHC_TCP_SEQ_IND_FID
                - ROHC_PROF6_VAR_CTRL_FID_BASE];
            if(flag !=0)
            {   
                if(1 == flag )
                {
                    rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                            p_csc->field_value[ROHC_SN_FID-
                            ROHC_VAR_FID_BASE],0,ROHC_P6_8_BITS);
                }
                else if (2 == flag)
                {
                    rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                            p_csc->field_value[ROHC_SN_FID-
                            ROHC_VAR_FID_BASE],0,ROHC_P6_16_BITS);
                }
                else
                {

                    rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                            p_csc->field_value[ROHC_SN_FID-
                            ROHC_VAR_FID_BASE],0,ROHC_P6_32_BITS);
                }
            }
            break;
        case ROHC_TCP_ACKNO_FID:
            flag = p_csc->ctrl_field_val_p6[ROHC_TCP_ACK_IND_FID
                - ROHC_PROF6_VAR_CTRL_FID_BASE];
            if(flag !=0)
            {   
                if(1 == flag )
                {
                    rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                            p_csc->field_value[ROHC_TCP_ACKNO_FID-
                            ROHC_VAR_FID_BASE],0,ROHC_P6_8_BITS);
                }
                else if (2 == flag)
                {
                    rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                            p_csc->field_value[ROHC_TCP_ACKNO_FID-
                            ROHC_VAR_FID_BASE],0,ROHC_P6_16_BITS);
                }
                else
                {

                    rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                            p_csc->field_value[ROHC_TCP_ACKNO_FID-
                            ROHC_VAR_FID_BASE],0,ROHC_P6_32_BITS);
                }
            }
            break;
        default:
            ROHC_LOG(LOG_INFO,"CPBM :CID[%u]:"\
                    " Invalid field id",
                    p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE]);
            break;
    }/*end switch on field id */
    ROHC_EXIT_FN("rohc_p6_variable_length_32_enc");
}

/*****************************************************************************
**      FUNCTION:
**              rohc_p6_static_or_irregular_enc
******************************************************************************
**
**      DESCRIPTION:
**              set buffer based on static_or_irregular encoding scheme  
**      ARGUMENTS:
**               Pointer to ROHC packet buffer
**               Bit position in above buffer
**               Field identifier
**               Pointer to CSC
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
static void 
rohc_p6_static_or_irregular_enc
    ROHC_PARAMS((p_p_rohc_pkt,p_to_bit_pos,field_id,p_csc),
    rohc_U8bit_t            ** p_p_rohc_pkt _AND_
    rohc_U8bit_t            * p_to_bit_pos  _AND_
    rohc_U8bit_t            field_id        _AND_
    struct  rohc_csc_tcp_t  * p_csc)
{
    ROHC_ENTER_FN("rohc_p6_static_or_irregular_enc");
    rohc_U8bit_t flag;

    switch(field_id)
    {
        case ROHC_TCP_ACK_STRIDE_FID:
            flag = p_csc->field_value[ROHC_TCP_ACK_STRIDE_FLAG_FID - 
                ROHC_VAR_FID_BASE];
            if(flag && (p_csc->field_value[ROHC_TCP_ACK_STRIDE_FID - 
                    ROHC_VAR_FID_BASE]))
            {   
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                        p_csc->field_value[ROHC_TCP_ACK_STRIDE_FID -
                        ROHC_VAR_FID_BASE],0,ROHC_P6_16_BITS);
            }
            break;
        case ROHC_TCP_WINDOW_FID:
            flag = p_csc->ctrl_field_val_p6[ROHC_TCP_WINDOW_IND_FID - 
                ROHC_PROF6_VAR_CTRL_FID_BASE];
            if(flag)
            {   
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                        p_csc->field_value[ROHC_TCP_WINDOW_FID -
                        ROHC_VAR_FID_BASE],0,ROHC_P6_16_BITS);
            }
            break;
        case ROHC_TCP_URG_PTR_FID:
            flag = p_csc->ctrl_field_val_p6[ROHC_TCP_URG_PTR_PRESENT_FID -
                ROHC_PROF6_VAR_CTRL_FID_BASE];
            if(flag)
            {   
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                        p_csc->field_value[ROHC_TCP_URG_PTR_FID -
                        ROHC_VAR_FID_BASE],0,ROHC_P6_16_BITS);
            }
            break;
        case ROHC_INNERIP4_TTL_FID:
            flag = p_csc->ctrl_field_val_p6[ROHC_INNERIP4_TTL_HPL_PRESENT_FID -
                ROHC_PROF6_VAR_CTRL_FID_BASE];
            if(flag)
            {   
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                        p_csc->field_value[ROHC_INNERIP4_TTL_FID-
                        ROHC_VAR_FID_BASE],0,ROHC_P6_8_BITS);
            }
            break;
        case ROHC_INNERIP4_DSCP_FID:
            flag = p_csc->ctrl_field_val_p6[ROHC_INNERIP4_DSCP_PRESENT_FID -
                ROHC_PROF6_VAR_CTRL_FID_BASE];
            if(flag)
            {   
                /* includes 6 bits of dscp and 2 bits of padding */
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                        p_csc->field_value[ROHC_INNERIP4_DSCP_FID-
                        ROHC_VAR_FID_BASE],0,6);
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                        0,0,2);
            }
            break;
        default:
            ROHC_LOG(LOG_INFO,"CPBM :CID[%u]:"\
                    " Invalid field id",
                    p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE]);
            break;
    }/*end switch on field id */
    ROHC_EXIT_FN("rohc_p6_static_or_irregular_enc");
}

/*****************************************************************************
**      FUNCTION:
**              rohc_p6_opt_ip_id_lsb_enc
******************************************************************************
**
**      DESCRIPTION:
**              set buffer based on optional ip id lsb  encoding scheme  
**      ARGUMENTS:
**               Pointer to ROHC packet buffer
**               Bit position in above buffer
**               Field identifier
**               Pointer to CSC
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
static void 
rohc_p6_opt_ip_id_lsb_enc(void)
    /* +- SPR 17777 */
{
    ROHC_ENTER_FN("rohc_p6_opt_ip_id_lsb_enc");
    /* This func will be implemented later when seq or seq-swapped beh. 
     * of ip-id will be supported. Currently for Random or Zero behaviour,
     * nothing will be sent here*/
    ROHC_EXIT_FN("rohc_p6_opt_ip_id_lsb_enc");
}

/*****************************************************************************
**      FUNCTION:
**              rohc_tcp_opt_list_compressed_pkt
******************************************************************************
**
**      DESCRIPTION:
**              prepares tcp options list for compressed packet
**      ARGUMENTS:
**               Pointer to ROHC packet buffer,
**               Pointer to CSC,
**				 Pointer to ROHC Entity,
** 				 Pointer to packet length
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
static rohc_return_t 
rohc_tcp_opt_list_compressed_pkt
    ROHC_PARAMS((p_entity,p_rohc_pkt,p_csc,p_length),
    rohc_entity_t   * p_entity   _AND_
    rohc_U8bit_t            * p_rohc_pkt _AND_
    rohc_csc_tcp_t  * p_csc      _AND_
    rohc_U32bit_t  *p_length)
{
    ROHC_ENTER_FN("rohc_tcp_opt_list_compressed_pkt");
    rohc_U8bit_t        index = 0;
    rohc_U8bit_t        curr_index = 0;
    rohc_U8bit_t        kind = 0;
    rohc_U8bit_t        numXI_items = 0;
    /* SPR 19690 fix start */
    rohc_U8bit_t        item_list[MAX_TCP_OPT_SIZE];
    rohc_U8bit_t        *p_item_list = NULL;
    /* SPR 19690 fix end */
    /*+ Coverity 83201 Changes */
    rohc_U8bit_t        XI_list[ROHC_MAX_ENTRIES_TCP_OPT_IDX] = {0};
    /*- Coverity 83201 Changes */
    rohc_U8bit_t        len = 0;
    rohc_bool_t         PS_flag = ROHC_FALSE;
    rohc_U8bit_t        item_list_len = 0;
    rohc_U8bit_t        l_index = 0;
    rohc_c_generic_opt_elem_t    generic_opt_elem;
    rohc_U8bit_t temp_XI_index[ROHC_MAX_ENTRIES_TCP_OPT_IDX] = {0};
    p_item_list = item_list;
    rohc_U8bit_t gen_opt_idx = 7;
    while(p_csc->rohc_c_tcp_opt.tcp_opt_kind[index]!= 0xff && 
            index < ROHC_MAX_ENTRIES_TCP_OPT_IDX)
    {
        kind = p_csc->rohc_c_tcp_opt.tcp_opt_kind[index];
        /* SPR 19690 fix start */
        curr_index = getOptionIndex(p_entity,kind,&(p_csc->rohc_c_tcp_opt),
                p_item_list, p_csc,&len,&gen_opt_idx);
        /* SPR 19690 fix end */
        if(curr_index > 7)
        {
            PS_flag = ROHC_TRUE;
        }
        temp_XI_index[index] = curr_index;
        index++;
        p_item_list = p_item_list + len;
        item_list_len += len;
    }
    /* update XI list */
    numXI_items = index;
    for(index = 0; index < numXI_items; index++)
    {
        if(INDEX_EOL == temp_XI_index[index])
        {
            if(eol_list_present)
            {
              rohc_p6_update_XI_list(&XI_list[index], temp_XI_index[index], 1 ,
                PS_flag);
            }
            else
            {
              rohc_p6_update_XI_list(&XI_list[index], temp_XI_index[index], 0 ,
                PS_flag);
            }
        }
        else if((INDEX_NOP == temp_XI_index[index])||(INDEX_SACK_PERM == 
                    temp_XI_index[index])||(temp_XI_index[index]>=7 ) ||
                (INDEX_TIMESTAMP == temp_XI_index[index]))
        {
              rohc_p6_update_XI_list(&XI_list[index], temp_XI_index[index], 1 ,
                PS_flag);
        }
        else if(INDEX_MSS == temp_XI_index[index])
        {
            if(mss_list_present)
            {
              rohc_p6_update_XI_list(&XI_list[index], temp_XI_index[index], 1 ,
                PS_flag);
            }
            else
            {
              rohc_p6_update_XI_list(&XI_list[index], temp_XI_index[index], 0 ,
                PS_flag);
            }
        }
        else if(INDEX_WINDOW_SCAL == temp_XI_index[index])
        {
            if(ws_list_present)
         
            {
              rohc_p6_update_XI_list(&XI_list[index], temp_XI_index[index], 1 ,
                PS_flag);
            }
            else
            {
              rohc_p6_update_XI_list(&XI_list[index], temp_XI_index[index], 0 ,
                PS_flag);
            }
        }
        else
        { /* send item in irregular chain */
              rohc_p6_update_XI_list(&XI_list[index], temp_XI_index[index], 0 ,
                PS_flag);
        }
    }
    *p_length = rohc_c_list_enc_tcp_opt_pkt(PS_flag,
                        (rohc_U8bit_t)numXI_items, XI_list, item_list,
                                    (rohc_U8bit_t)item_list_len, p_rohc_pkt);
    
    /* store updated values of generic options in context */
    if(gen_opt_list_present)
    {
        /* first free the existing generic options in the context*/
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
        /* store new values */
            rohc_c_generic_opt_t *p_generic_opt = &(p_csc->rohc_c_tcp_opt.
                    rohc_c_generic_opt);
        l_index = p_csc->p_cpib->
            rohc_c_temp_tcp_opt.rohc_c_generic_opt.next_free_index;
        for(index = 0; index < l_index; index++)
        {
            generic_opt_elem =p_csc->p_cpib->rohc_c_temp_tcp_opt.rohc_c_generic_opt.
                rohc_generic_opt_elem[index];
            rohc_c_generic_opt_elem_t    *p_generic_opt_elem_ctxt = 
                &(ROHC_CSC_GET_GENERIC_ELEM_INDEX(p_generic_opt));
            p_generic_opt_elem_ctxt->kind = generic_opt_elem.kind;
            p_generic_opt_elem_ctxt->length = generic_opt_elem.length;
            p_generic_opt_elem_ctxt->option_static =
                generic_opt_elem.option_static;
                p_generic_opt_elem_ctxt->opt_val =
                    (rohc_U8bit_t *)ROHC_MALLOC(sizeof(rohc_U8bit_t)*
                            (generic_opt_elem.length-2));

                if(ROHC_NULL == p_generic_opt_elem_ctxt->opt_val)
                {
                    ROHC_UT_TRACE((stderr,"ROHC_MEMORY_FAILURE \n "));
                    ROHC_EXIT_FN("getOptionIndex");
                    return ROHC_FAILURE;
                }
                ROHC_MEMSET(p_generic_opt_elem_ctxt->opt_val, 0,
                        sizeof(rohc_U8bit_t)*(generic_opt_elem.length-2));
                ROHC_MEMCPY(p_generic_opt_elem_ctxt->opt_val,
                        generic_opt_elem.opt_val,(generic_opt_elem.length-2) );
                ROHC_CSC_SET_GENERIC_ELEM_INDEX(p_generic_opt);
        }
    }
    else
    {
        /* free the old generic options from context in exist*/
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
    }

    clear_options_from_context(p_csc);
    gen_opt_list_present = 0;
    ROHC_EXIT_FN("rohc_tcp_opt_list_compressed_pkt");
    return ROHC_SUCCESS;
}

rohc_void_t clear_options_from_context(rohc_csc_tcp_t *p_csc)
{
    rohc_U8bit_t index = 0;    
    if(!eol_opt_present)
    {
        p_csc->rohc_c_tcp_opt.tcp_opt_len[EOL_LEN_ID] = 0;
    }
    if(!mss_opt_present)
    {
         p_csc->rohc_c_tcp_opt.mss = 0;
    }
    if(!ws_opt_present)
    {
         p_csc->rohc_c_tcp_opt.ws = 0;
    }

    if(!sack_opt_present)
    {
        for(index = 0; index < 8; index++)
        {
            p_csc->rohc_c_tcp_opt.sack_ptr[index] = 0;
        }
    }
    if(!alt_cksum_req_opt_present)
    {
         p_csc->rohc_c_tcp_opt.alt_cksum_req = 0;
    }
    eol_opt_present = 0;
    mss_opt_present = 0;
    ws_opt_present = 0;
   
    sack_opt_present = 0;
    alt_cksum_req_opt_present = 0;
}
/*****************************************************************************
**      FUNCTION:
**              getOptionIndex
******************************************************************************
**
**      DESCRIPTION:
**              Return index for option kind
**      ARGUMENTS:
**               option kind
**               tcp opt structure
**				 Pointer to Rohc entity
**				 Pointer to item List
**				 Pointer to packet length
**				 Pointer to general option index
**      RETURN VALUE:
**              index
**      NOTES:
**               
*****************************************************************************/

rohc_return_t getOptionIndex
    ROHC_PARAMS((p_entity,kind,p_tcp_opt,p_item_list,p_csc,p_len,p_gen_opt_idx),
    rohc_entity_t   * p_entity   _AND_
    rohc_U8bit_t            kind _AND_
    rohc_c_tcp_opt_t  *p_tcp_opt _AND_
    rohc_U8bit_t *p_item_list    _AND_
    rohc_csc_tcp_t  * p_csc      _AND_
    rohc_U8bit_t *p_len          _AND_
    rohc_U8bit_t *p_gen_opt_idx 
    )
{
    ROHC_ENTER_FN("getOptionIndex");
    rohc_U8bit_t i, l_index;
    rohc_U8bit_t ret_index;
    rohc_c_generic_opt_elem_t    generic_opt_elem;
    *p_len = 0;
    switch(kind)
    {
        case TCP_NOP_OPT :
            ret_index = INDEX_NOP; /* will be sent in list*/
            break;

        case TCP_EOL_OPT :
            ret_index = INDEX_EOL;
            if(p_tcp_opt->tcp_opt_len[EOL_LEN_ID] != p_csc->p_cpib->rohc_c_temp_tcp_opt.eol_len)
            {
                eol_list_present = 1;
                p_tcp_opt->tcp_opt_len[EOL_LEN_ID] = p_csc->p_cpib->
                    rohc_c_temp_tcp_opt.eol_len;
                *p_item_list = (p_tcp_opt->tcp_opt_len[EOL_LEN_ID]-1)*8; /* in bits*/
                *p_len = 1;
            }
            else
            {
                eol_list_present = 0;
                /* item will be sent as part of irregular chain */
            }
            eol_opt_present = 1;
            break;

        case TCP_MSS_OPT :
            ret_index = INDEX_MSS;
            /* check if mss opt present in context and value is changed*/
            if(p_tcp_opt->mss == p_csc->p_cpib->rohc_c_temp_tcp_opt.mss)
            {
                mss_list_present = 0;
            }
            else
            {
                mss_list_present = 1;
                /*update mss val in ctxt*/
                p_tcp_opt->mss = p_csc->p_cpib->rohc_c_temp_tcp_opt.mss;
                ROHC_MEMCPY(p_item_list,&(p_tcp_opt->mss),2);
                *p_len = 2;
            }
            mss_opt_present = 1;
            break;

        case TCP_WS_OPT :
            ret_index = INDEX_WINDOW_SCAL;
            /* check if ws opt present in context and value is changed*/
            if(p_tcp_opt->ws == p_csc->p_cpib->rohc_c_temp_tcp_opt.ws)
            {
                ws_list_present = 0;
            }
            else
            {
                ws_list_present = 1;
                p_tcp_opt->ws = p_csc->p_cpib->rohc_c_temp_tcp_opt.ws;
                *p_item_list = p_tcp_opt->ws;
                *p_len = 1;
            }
            ws_opt_present = 1;
            break;

        case TCP_TS_OPT :
            /* whenever list structure changes, TS opt is sent in list*/
            ret_index = INDEX_TIMESTAMP;
           
                ROHC_MEMCPY(p_tcp_opt->ts, 
                        p_csc->p_cpib->rohc_c_temp_tcp_opt.ts, 8);
                ROHC_MEMCPY(p_item_list,p_tcp_opt->ts,8);
                *p_len = 8;
            rohc_ed_p6_update_gsw_ts_val(p_entity,p_csc,
                    ROHC_NTOHL(p_tcp_opt->ts[0]));
            rohc_ed_p6_update_gsw_ts_echo(p_entity,p_csc,
                    ROHC_NTOHL(p_tcp_opt->ts[1]));
            
            break;

        case TCP_SACK_PERMTD_OPT :
            ret_index = INDEX_SACK_PERM;
            break;

        case TCP_SACK_OPT :
            ret_index = INDEX_SACK;
            sack_opt_present = 1;
            break;

        case TCP_ALT_CKSUM_REQ_OPT :

            ret_index = *p_gen_opt_idx;
            *p_item_list++ = TCP_ALT_CKSUM_REQ_OPT;
            *p_item_list++ = TCP_ALT_CKSUM_REQ_OPT_LEN;
            *p_item_list = p_csc->p_cpib->rohc_c_temp_tcp_opt.alt_cksum_req;
            *p_len = 3;
            (*p_gen_opt_idx)++;
            p_tcp_opt->alt_cksum_req = 
                p_csc->p_cpib->rohc_c_temp_tcp_opt.alt_cksum_req;
            alt_cksum_req_opt_present = 1;
            break;
        default :
            /* Generic options */
            ret_index = *p_gen_opt_idx;
            gen_opt_list_present = 1;
            /*fill generic option fron pib */
            l_index = p_csc->p_cpib->
                rohc_c_temp_tcp_opt.rohc_c_generic_opt.next_free_index;
            for(i = 0; i<l_index; i++)
            {
                if(kind == p_csc->p_cpib->rohc_c_temp_tcp_opt.rohc_c_generic_opt.
                        rohc_generic_opt_elem[i].kind)
                {
                    break;
                }
            }
            /* list encoding of generic options */
            generic_opt_elem =p_csc->p_cpib->rohc_c_temp_tcp_opt.rohc_c_generic_opt.
                rohc_generic_opt_elem[i];
            *p_item_list++ = generic_opt_elem.kind;
            (*p_len)++;
            *p_item_list = generic_opt_elem.option_static << 7 |
                generic_opt_elem.length;
            *p_len = *p_len +1;
            p_item_list++;
            ROHC_MEMCPY(p_item_list,generic_opt_elem.opt_val,
                    generic_opt_elem.length-2);
            *p_len = *p_len + generic_opt_elem.length-2;
            (*p_gen_opt_idx)++;

            break;
    }
    return ret_index;
}

/*****************************************************************************
**      FUNCTION:
**              rohc_add_ele_p1_ext3flagsp1
******************************************************************************
**
**      DESCRIPTION:
**              set buffer based on EXt 3 flags 
**      ARGUMENTS:
**               Pointer to ROHC packet buffer
**               Bit position in above buffer
**               Field identifier
**               Pointer to CSC
**			 	 Packet Type
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
/* Start SPR# 6563 Fix*/
STATIC void 
rohc_add_ele_p1_ext3flagsp1
    ROHC_PARAMS((p_p_rohc_pkt,p_ext_flag,p_to_bit_pos,field_id,p_csc,pkt_type),
    rohc_U8bit_t            ** p_p_rohc_pkt _AND_
    rohc_U8bit_t            * p_ext_flag    _AND_
    rohc_U8bit_t            * p_to_bit_pos  _AND_
    rohc_U8bit_t            field_id        _AND_
    struct  rohc_csc_rtp_t  * p_csc         _AND_
    rohc_U8bit_t            pkt_type)
/* End SPR# 6563 Fix*/
{
    rohc_U8bit_t ext3_flags=p_csc->field_value[ROHC_EXT3_FLAGS_FID-
                                                ROHC_VAR_FID_BASE];
    ROHC_ENTER_FN("rohc_add_ele_p1_ext3flagsp1");
    switch(field_id)
    {
        case ROHC_SN_FID:
            if(p_csc->field_len[ROHC_SN_FID-ROHC_VAR_FID_BASE] !=0)
            {   
                *p_ext_flag|=ROHC_ENC_SN_EXT3FLAGP1_MASK;
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_SN_FID-
                        ROHC_VAR_FID_BASE],0,8);
            }
            break;
        case ROHC_TS_FID:
            if(p_csc->field_len[ROHC_TS_FID-ROHC_VAR_FID_BASE] !=0)
            {   
                *p_ext_flag|=ROHC_ENC_RTS_EXT3FLAGP1_MASK;
                /* Start SPR 5828 fix */
                /*  Case 1: When Ts stride is  0 , set Tsc = 0 ( Time Stamp Unscaled )
                 *  Case 2: When Ts stride is changing, set Tsc = 0 ( Time Stamp Unscaled )
                 *  For other case always Scaled TS will be send with Tsc =1 
                 * */  
                
                /* Start SPR# 6563 Fix*/
                if (( ( 0 == p_csc->field_value[ROHC_RTP_TSS_FID-ROHC_VAR_FID_BASE]) &&  
                        ( 0 != p_csc->ts_stride)) && (ROHC_FALSE == p_csc->ts_wrap_flag)) 
                {
                    *p_ext_flag|=ROHC_ENC_TSC_EXT3FLAGP1_MASK;
                }
                /* End SPR 5828 fix */                
                if ( ROHC_UOR2E3P1_PKTTYPE != pkt_type  ) {
                (*p_p_rohc_pkt)+=rohc_c_encode_sdvl(*p_p_rohc_pkt,
                        ((p_csc->field_value[ROHC_TS_FID-ROHC_VAR_FID_BASE]) &
                        ((1<<p_csc->field_len[field_id-ROHC_VAR_FID_BASE])-1)));
                }
                else
                {
                   (*p_p_rohc_pkt)+=rohc_c_encode_sdvl_ts(*p_p_rohc_pkt,
                        ((p_csc->field_value[ROHC_TS_FID-ROHC_VAR_FID_BASE])&  
                        ((1<<p_csc->field_len[field_id-ROHC_VAR_FID_BASE])-1)), 
                        (p_csc->field_len[field_id-ROHC_VAR_FID_BASE]));
            
                }    
                /* End SPR# 6563 Fix*/
/*
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_TS_FID-
                        ROHC_VAR_FID_BASE],
                        0,p_csc->field_len[field_id-ROHC_VAR_FID_BASE]);
*/
            }
            break;
        case ROHC_INNERIP_ID_FID:
            if(p_csc->new_inner_rnd == 0 &&
               p_csc->field_len[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE] > 0)
            {   
            /*if( ext3_flags&ROHC_ENC_I_EXT3FLAGP1_MASK)
             {*/
                 *p_ext_flag|=ROHC_ENC_I_EXT3FLAGP1_MASK;
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_INNERIP_ID_FID-
                        ROHC_VAR_FID_BASE],0,16);
            }
            break;  
        case ROHC_INNERIP_FLAGS_FID:
            if( ext3_flags &  ROHC_ENC_INNERIPFLAG_EXT3FLAGP1_MASK)
            {
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_INNERIP_FLAGS_FID-
                        ROHC_VAR_FID_BASE],0,8);
            }
            break;
        case ROHC_RTP_FLAGS_FID:
            if( ext3_flags &  ROHC_ENC_RTPFLAG_EXT3FLAGP1_MASK)
            {
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_RTP_FLAGS_FID-
                        ROHC_VAR_FID_BASE],0,8);
            }
            break;
        case ROHC_OUTERIP_FLAGS_FID:
            if( ext3_flags &  ROHC_ENC_OUTERIPFLAG_EXT3FLAGP1_MASK)
            {
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-
                        ROHC_VAR_FID_BASE],0,8);
                if(p_csc->new_outer_rnd == 0 &&
                    p_csc->field_len[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE]>0)
                    *(*p_p_rohc_pkt-1) |= ROHC_ENC_I_OUTERIPFLAGP1_MASK;
            }
            break;  
        default :
            ROHC_LOG(LOG_INFO,"CPBM :CID[%u]:"\
                        " Invalid field id in ext3",
                        p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE]);
            break;
    }/*end switch on field id */
    ROHC_EXIT_FN("rohc_add_ele_p1_ext3flagsp1");
}   /* end function rohc_add_p1_ext3flagsp1*/

/*****************************************************************************
**      FUNCTION:
**              rohc_add_ele_p2_ext3flagsp1
******************************************************************************
**
**      DESCRIPTION:
**              set buffer based on EXt 3 flags 
**      ARGUMENTS:
**               Pointer to ROHC packet buffer
**               Bit position in above buffer
**               Field identifier
**               Pointer to CSC
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_add_ele_p2_ext3flagsp1
    ROHC_PARAMS((p_p_rohc_pkt,p_ext_flag,p_to_bit_pos,field_id,p_csc),
    rohc_U8bit_t           ** p_p_rohc_pkt _AND_
    rohc_U8bit_t           * p_ext_flag    _AND_
    rohc_U8bit_t           * p_to_bit_pos  _AND_
    rohc_U8bit_t           field_id        _AND_
    struct  rohc_csc_udp_t * p_csc)
{
    rohc_U8bit_t ext3_flags=p_csc->field_value[ROHC_EXT3_FLAGS_FID-
                                                ROHC_VAR_FID_BASE];
    ROHC_ENTER_FN("rohc_add_ele_p2_ext3flagsp1");
    switch(field_id)
    {
        case ROHC_SN_FID:
            if(p_csc->field_len[ROHC_SN_FID-ROHC_VAR_FID_BASE] !=0)
            {   
                *p_ext_flag|=ROHC_ENC_SN_EXT3FLAGP1_MASK;
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_SN_FID-
                        ROHC_VAR_FID_BASE],0,8);
            }
            break;
        case ROHC_TS_FID:
            if(p_csc->field_len[ROHC_TS_FID-ROHC_VAR_FID_BASE] !=0)
            {   
                *p_ext_flag|=ROHC_ENC_RTS_EXT3FLAGP1_MASK;
                (*p_p_rohc_pkt)+=rohc_c_encode_sdvl(*p_p_rohc_pkt,
                        ((p_csc->field_value[ROHC_TS_FID-ROHC_VAR_FID_BASE]) &
                        ((1<<p_csc->field_len[field_id-ROHC_VAR_FID_BASE])-1)));
/*
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_TS_FID-
                        ROHC_VAR_FID_BASE],
                        0,p_csc->field_len[field_id-ROHC_VAR_FID_BASE]);
*/
            }
            break;
        case ROHC_INNERIP_ID_FID:
            if(p_csc->new_inner_rnd == 0 &&
               p_csc->field_len[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE] > 0)
            {   
            /*if( ext3_flags&ROHC_ENC_I_EXT3FLAGP1_MASK)*/
                 *p_ext_flag|=ROHC_ENC_I_EXT3FLAGP1_MASK;
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_INNERIP_ID_FID-
                        ROHC_VAR_FID_BASE],0,16);
            }
            break;  
        case ROHC_INNERIP_FLAGS_FID:
            if( ext3_flags &  ROHC_ENC_INNERIPFLAG_EXT3FLAGP1_MASK)
            {
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_INNERIP_FLAGS_FID-
                        ROHC_VAR_FID_BASE],0,8);
            }
            break;
        case ROHC_RTP_FLAGS_FID:
            if( ext3_flags &  ROHC_ENC_RTPFLAG_EXT3FLAGP1_MASK)
            {
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_RTP_FLAGS_FID-
                        ROHC_VAR_FID_BASE],0,8);
            }
            break;
        case ROHC_OUTERIP_FLAGS_FID:
            if( ext3_flags &  ROHC_ENC_OUTERIPFLAG_EXT3FLAGP1_MASK)
            {
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-
                        ROHC_VAR_FID_BASE],0,8);
                if(p_csc->new_outer_rnd == 0 &&
                    p_csc->field_len[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE]>0)
                    *(*p_p_rohc_pkt-1) |= ROHC_ENC_I_OUTERIPFLAGP1_MASK;
            }
            break;  
        default :
            ROHC_LOG(LOG_INFO,"CPBM :CID[%u]:"\
                        " Invalid field id in ext3",
                        p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE]);
            break;
    }/*end switch on field id */
    ROHC_EXIT_FN("rohc_add_ele_p2_ext3flagsp1");
}   /* end function rohc_add_ele_p2_ext3flagsp1*/

/*****************************************************************************
**      FUNCTION:
**              rohc_add_ele_p1_inneripflagsp1
******************************************************************************
**
**      DESCRIPTION:
**              set buffer based on Inner ip flags 
**      ARGUMENTS:
**               Pointer to ROHC packet buffer
**               Bit position in above buffer
**               Field identifier
**               Pointer to CSC
**               Pointer to ROHC entity
**				 Pointer to Error Code
**      RETURN VALUE:
**              ROHC_SUCCESS/ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
rohc_return_t 
rohc_add_ele_p1_inneripflagsp1
    ROHC_PARAMS((p_p_rohc_pkt,p_to_bit_pos,field_id,p_csc,p_entity,p_ecode),
        rohc_U8bit_t            ** p_p_rohc_pkt _AND_
        rohc_U8bit_t            * p_to_bit_pos  _AND_
        rohc_U8bit_t            field_id        _AND_
        struct  rohc_csc_rtp_t  * p_csc         _AND_
        struct rohc_entity_t    * p_entity      _AND_
        rohc_error_code_t       *p_ecode)
{

    rohc_U8bit_t innerip_flags=
        p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE];
    ROHC_ENTER_FN("rohc_add_ele_p1_inneripflagsp1");
    switch(field_id)        
    {
        case ROHC_INNERIP4_TOS_FID :
            if(innerip_flags&ROHC_ENC_TOS_INNERIPFLAGP1_MASK)
            {
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_INNERIP4_TOS_FID-
                    ROHC_VAR_FID_BASE],0,8);
            }   
            break;  
                
        case ROHC_INNERIP4_TTL_FID :
            if(innerip_flags&ROHC_ENC_TTL_INNERIPFLAGP1_MASK)
            {
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_INNERIP4_TTL_FID-
                    ROHC_VAR_FID_BASE],0,8);
            }
            break;  
        case ROHC_INNERIP4_PROTO_FID :
            if(innerip_flags&ROHC_ENC_PROTO_INNERIPFLAGP1_MASK)
            {
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_INNERIP4_PROTO_FID-
                    ROHC_VAR_FID_BASE],0,8);

            }
            break;
        case ROHC_INNERIP_EXTHEADERS_LIST_FID :
            if(innerip_flags&ROHC_ENC_IPX_INNERIPFLAGP1_MASK)
            {
    		/* Coverity 11401 starts*/
                rohc_U32bit_t   no_of_bytes_in_list = 0;
    		/* Coverity 11401 ends*/
                if(ROHC_FAILURE == rohc_list_encoding_p1(p_entity,
                        ROHC_INNERIP_EXTHEADERS_LIST_FID,
                        p_csc,&no_of_bytes_in_list,*p_p_rohc_pkt,p_ecode))
                    return ROHC_FAILURE;        
                *p_p_rohc_pkt+=no_of_bytes_in_list;
            }
            break;
        case ROHC_OUTERIP_FLAGS_FID :
            if(innerip_flags&ROHC_ENC_IP2_INNERIPFLAGP1_MASK)
            {
                    rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                        p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-
                        ROHC_VAR_FID_BASE],0,8);
                    if(p_csc->new_outer_rnd == 0 &&
                      p_csc->field_len[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE]>0)
                    *(*p_p_rohc_pkt-1) |= ROHC_ENC_I_OUTERIPFLAGP1_MASK;
            }   
            break;
        default :
            ROHC_LOG(LOG_INFO,"CPBM :EID[%u:%u] :CID[%u]:"\
                        " Invalid field id in inner ip flags",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,
                        p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE]);
            break;  
    }   /* end switch field id */
    ROHC_EXIT_FN("rohc_add_ele_p1_inneripflagsp1");
    return ROHC_SUCCESS;
}/* end function rohc_add_ele_p1_inneripflagsp1 */

/*****************************************************************************
**      FUNCTION:
**              rohc_add_ele_p2_inneripflagsp1
******************************************************************************
**
**      DESCRIPTION:
**              set buffer based on Inner ip flags 
**      ARGUMENTS:
**               Pointer to ROHC packet buffer
**               Bit position in above buffer
**               Field identifier
**               Pointer to CSC
**               Pointer to ROHC entity
**      RETURN VALUE:
**              ROHC_SUCCESS/ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
rohc_return_t 
rohc_add_ele_p2_inneripflagsp1
    ROHC_PARAMS((p_p_rohc_pkt,p_to_bit_pos,field_id,p_csc,p_entity,p_ecode),
        rohc_U8bit_t            ** p_p_rohc_pkt _AND_
        rohc_U8bit_t            * p_to_bit_pos  _AND_
        rohc_U8bit_t            field_id        _AND_
        struct  rohc_csc_udp_t  * p_csc         _AND_
        struct rohc_entity_t    * p_entity      _AND_
        rohc_error_code_t       *p_ecode)
{

    rohc_U8bit_t innerip_flags=
        p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE];
    ROHC_ENTER_FN("rohc_add_ele_p2_inneripflagsp1");
    switch(field_id)        
    {
        case ROHC_INNERIP4_TOS_FID :
            if(innerip_flags&ROHC_ENC_TOS_INNERIPFLAGP1_MASK)
            {
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_INNERIP4_TOS_FID-
                    ROHC_VAR_FID_BASE],0,8);
            }   
            break;  
                
        case ROHC_INNERIP4_TTL_FID :
            if(innerip_flags&ROHC_ENC_TTL_INNERIPFLAGP1_MASK)
            {
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_INNERIP4_TTL_FID-
                    ROHC_VAR_FID_BASE],0,8);
            }
            break;  
        case ROHC_INNERIP4_PROTO_FID :
            if(innerip_flags&ROHC_ENC_PROTO_INNERIPFLAGP1_MASK)
            {
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_INNERIP4_PROTO_FID-
                    ROHC_VAR_FID_BASE],0,8);

            }
            break;
        case ROHC_INNERIP_EXTHEADERS_LIST_FID :
            if(innerip_flags&ROHC_ENC_IPX_INNERIPFLAGP1_MASK)
            {
                /* Coverity Fix 32542 start*/
                rohc_U32bit_t   no_of_bytes_in_list = 0;
                /* Coverity Fix 32542 end*/
                if(ROHC_FAILURE == rohc_list_encoding_p2(p_entity,
                        ROHC_INNERIP_EXTHEADERS_LIST_FID,
                        p_csc,&no_of_bytes_in_list,*p_p_rohc_pkt,p_ecode))
                    return ROHC_FAILURE;        
                *p_p_rohc_pkt+=no_of_bytes_in_list;
            }
            break;
        case ROHC_OUTERIP_FLAGS_FID :
            if(innerip_flags&ROHC_ENC_IP2_INNERIPFLAGP1_MASK)
            {
                    rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                        p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-
                        ROHC_VAR_FID_BASE],0,8);
                    if(p_csc->new_outer_rnd == 0 && p_csc->field_len[
                        ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE]>0)
                    *(*p_p_rohc_pkt-1) |= ROHC_ENC_I_OUTERIPFLAGP1_MASK;
            }   
            break;
        default :
            ROHC_LOG(LOG_INFO,"CPBM :EID[%u:%u] :CID[%u]:"\
                        " Invalid field id in inner ip flags",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,
                        p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE]);
            break;  
    }   /* end switch field id */
    ROHC_EXIT_FN("rohc_add_ele_p2_inneripflagsp1");
    return ROHC_SUCCESS;
}/* end function rohc_add_ele_p2_inneripflagsp1 */

/*****************************************************************************
**      FUNCTION:
**              rohc_add_ele_p1_outeripflagsp1
******************************************************************************
**
**      DESCRIPTION:
**              set buffer based on outer ip flags 
**      ARGUMENTS:
**               Pointer to ROHC packet buffer
**               Bit position in above buffer
**               Field identifier
**               Pointer to CSC
**               Pointer to ROHC entity
**				 Pointer to Error Code
**      RETURN VALUE:
**               ROHC_SUCCESS/ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
rohc_return_t
rohc_add_ele_p1_outeripflagsp1
    ROHC_PARAMS((p_p_rohc_pkt,p_to_bit_pos,field_id,p_csc,p_entity,p_ecode),
        rohc_U8bit_t            ** p_p_rohc_pkt _AND_
        rohc_U8bit_t            * p_to_bit_pos  _AND_
        rohc_U8bit_t            field_id        _AND_
        struct  rohc_csc_rtp_t  * p_csc         _AND_
        struct rohc_entity_t    * p_entity      _AND_
        rohc_error_code_t       *p_ecode)

{
    rohc_U8bit_t outerip_flags=
        p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE];
    rohc_U8bit_t innerip_flags=
        p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE];
    ROHC_ENTER_FN("rohc_add_ele_p1_outeripflagsp1");
    switch(field_id)
    {
        case ROHC_OUTERIP4_TOS_FID :
            if(outerip_flags&ROHC_ENC_TOS_OUTERIPFLAGP1_MASK)
            {
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_OUTERIP4_TOS_FID-
                    ROHC_VAR_FID_BASE],0,8);
            }   
            break;  
        case ROHC_OUTERIP4_TTL_FID :
            if(outerip_flags&ROHC_ENC_TTL_OUTERIPFLAGP1_MASK)
            {
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_OUTERIP4_TTL_FID-
                    ROHC_VAR_FID_BASE],0,8);
            }   
            break;  
        case ROHC_OUTERIP4_PROTO_FID :
            if(outerip_flags&ROHC_ENC_PROTO_OUTERIPFLAGP1_MASK)
            {
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_OUTERIP4_PROTO_FID-
                    ROHC_VAR_FID_BASE],0,8);
            }   
            break;  
        case ROHC_OUTERIP_EXTHEADERS_LIST_FID :
            if(innerip_flags&ROHC_ENC_PROTO_INNERIPFLAGP1_MASK)
            {
    		/* Coverity 11402 starts*/
                rohc_U32bit_t   no_of_bytes_in_list = 0;
    		/* Coverity 11402 ends*/
                if( ROHC_FAILURE == rohc_list_encoding_p1(p_entity,
                        ROHC_OUTERIP_EXTHEADERS_LIST_FID,p_csc,
                        &no_of_bytes_in_list,*p_p_rohc_pkt,p_ecode))
                    return ROHC_FAILURE;
                *p_p_rohc_pkt+=no_of_bytes_in_list;
            }
            break;
        case ROHC_OUTERIP_ID_FID :
            if(p_csc->new_outer_rnd == 0 &&
               p_csc->field_len[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE] > 0)
            {   
/*            if(outerip_flags&ROHC_ENC_I_OUTERIPFLAGP1_MASK)
            {*/
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                p_csc->field_value[ROHC_OUTERIP_ID_FID-
                    ROHC_VAR_FID_BASE],0,16);
            }   
            break;  
        default :
            ROHC_LOG(LOG_INFO,"CPBM :EID[%u:%u] :CID[%u]:"\
                        " Invalid field id in outer ip flags",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,
                        p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE]);
            break;  
    }/* end switch field id */
    ROHC_EXIT_FN("rohc_add_ele_p1_outeripflagsp1");
    return ROHC_SUCCESS;
}/* end funciton rohc_add_ele_p1_outeripflagsp1 */
/*****************************************************************************
**      FUNCTION:
**              rohc_add_ele_p2_outeripflagsp1
******************************************************************************
**
**      DESCRIPTION:
**              set buffer based on outer ip flags 
**      ARGUMENTS:
**               Pointer to ROHC packet buffer
**               Bit position in above buffer
**               Field identifier
**               Pointer to CSC
**               Pointer to ROHC entity
**				 Pointer to Error Code
**      RETURN VALUE:
**               ROHC_SUCCESS/ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
rohc_return_t
rohc_add_ele_p2_outeripflagsp1
    ROHC_PARAMS((p_p_rohc_pkt,p_to_bit_pos,field_id,p_csc,p_entity,p_ecode),
        rohc_U8bit_t    ** p_p_rohc_pkt _AND_
        rohc_U8bit_t    * p_to_bit_pos _AND_
        rohc_U8bit_t    field_id _AND_
        struct  rohc_csc_udp_t  * p_csc _AND_
        struct rohc_entity_t * p_entity _AND_
        rohc_error_code_t   *p_ecode)

{
    rohc_U8bit_t outerip_flags=
        p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE];
    rohc_U8bit_t innerip_flags=
        p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE];
    ROHC_ENTER_FN("rohc_add_ele_p2_outeripflagsp1");
    switch(field_id)
    {
        case ROHC_OUTERIP4_TOS_FID :
            if(outerip_flags&ROHC_ENC_TOS_OUTERIPFLAGP1_MASK)
            {
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_OUTERIP4_TOS_FID-
                    ROHC_VAR_FID_BASE],0,8);
            }   
            break;  
        case ROHC_OUTERIP4_TTL_FID :
            if(outerip_flags&ROHC_ENC_TTL_OUTERIPFLAGP1_MASK)
            {
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_OUTERIP4_TTL_FID-
                    ROHC_VAR_FID_BASE],0,8);
            }   
            break;  
        case ROHC_OUTERIP4_PROTO_FID :
            if(outerip_flags&ROHC_ENC_PROTO_OUTERIPFLAGP1_MASK)
            {
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_OUTERIP4_PROTO_FID-
                    ROHC_VAR_FID_BASE],0,8);
            }   
            break;  
        case ROHC_OUTERIP_EXTHEADERS_LIST_FID :
            if(innerip_flags&ROHC_ENC_PROTO_INNERIPFLAGP1_MASK)
            {
                /* Coverity fix 32543 Start*/
                rohc_U32bit_t   no_of_bytes_in_list = 0;
                /* Coverity fix 32543 End*/
                if( ROHC_FAILURE == rohc_list_encoding_p2(p_entity,
                        ROHC_OUTERIP_EXTHEADERS_LIST_FID,p_csc,
                        &no_of_bytes_in_list,*p_p_rohc_pkt,p_ecode))
                    return ROHC_FAILURE;
                *p_p_rohc_pkt+=no_of_bytes_in_list;
            }
            break;
        case ROHC_OUTERIP_ID_FID :
            if(p_csc->new_outer_rnd == 0 &&
               p_csc->field_len[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE] > 0)
            {   
/*            if(outerip_flags&ROHC_ENC_I_OUTERIPFLAGP1_MASK)
            {*/
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                p_csc->field_value[ROHC_OUTERIP_ID_FID-
                    ROHC_VAR_FID_BASE],0,16);
            }   
            break;  
        default :
            ROHC_LOG(LOG_INFO,"CPBM :EID[%u:%u] :CID[%u]:"\
                        " Invalid field id in outer ip flags",
                        p_entity->rohc_entity_id.ueIndex,
                        p_entity->rohc_entity_id.lcId,
                        p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE]);
            break;  
    }/* end switch field id */
    ROHC_EXIT_FN("rohc_add_ele_p2_outeripflagsp1");
    return ROHC_SUCCESS;
}/* end funciton rohc_add_ele_p2_outeripflagsp1 */

/*****************************************************************************
**      FUNCTION:
**              rohc_add_ele_p1_rtpflagsp1
******************************************************************************
**
**      DESCRIPTION:
**              set buffer based on RTP flags 
**      ARGUMENTS:
**               Pointer to ROHC packet buffer
**               Bit position in above buffer
**               Field identifier
**               Pointer to CSC
**               Pointer to ROHC entity
**				 Pointer to Error Code
**      RETURN VALUE:
**               ROHC_SUCCESS/ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
rohc_return_t
rohc_add_ele_p1_rtpflagsp1
    ROHC_PARAMS((p_p_rohc_pkt,p_to_bit_pos,field_id,p_csc,p_entity,p_ecode),
        rohc_U8bit_t            ** p_p_rohc_pkt _AND_
        rohc_U8bit_t            * p_to_bit_pos  _AND_
        rohc_U8bit_t            field_id        _AND_
        struct  rohc_csc_rtp_t  * p_csc         _AND_
        struct rohc_entity_t    * p_entity      _AND_
        rohc_error_code_t       *p_ecode)
{
    rohc_U8bit_t rtp_flags=
        p_csc->field_value[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE];
    ROHC_ENTER_FN("rohc_add_ele_p1_rtpflagsp1");
    switch(field_id)
    {
        case ROHC_RTP_PT_FID :
            if(rtp_flags&ROHC_ENC_R_PT_RTPFLAGP1_MASK)
            {
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                p_csc->field_value[ROHC_RTP_R_P_FID-
                    ROHC_VAR_FID_BASE],0,1);
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                p_csc->field_value[ROHC_RTP_PT_FID-
                    ROHC_VAR_FID_BASE],0,7);
            }
            break;  
        case ROHC_RTP_CSRC_LIST_FID  :
            if(rtp_flags&ROHC_ENC_CSRC_RTPFLAGP1_MASK)
            {
    		/* Coverity 11403 starts*/
                rohc_U32bit_t   no_of_bytes_in_list = 0 ;
    		/* Coverity 11403 ends*/
                if( ROHC_FAILURE == rohc_list_encoding_p1(p_entity,
                        ROHC_RTP_CSRC_LIST_FID,p_csc,
                        &no_of_bytes_in_list,*p_p_rohc_pkt,p_ecode))
                    return ROHC_FAILURE;
                *p_p_rohc_pkt+=no_of_bytes_in_list;
            }
            break;
        case ROHC_RTP_TS_STRIDE_FID :
            if(rtp_flags&ROHC_ENC_TSS_RTPFLAGP1_MASK)
            {
                /*Start SPR 6191 Fix: Corrected encoding of TS_Stride*/
                (*p_p_rohc_pkt)+=rohc_c_encode_sdvl(*p_p_rohc_pkt,
                        ((p_csc->field_value[ROHC_RTP_TS_STRIDE_FID-
                          ROHC_VAR_FID_BASE])));
                /*End SPR 6191 Fix*/
/*                          
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_RTP_TS_STRIDE_FID-
                    ROHC_VAR_FID_BASE],0,
                    p_csc->field_len[ROHC_RTP_TS_STRIDE_FID-
                    ROHC_VAR_FID_BASE]);
*/
            }
            break;
        case ROHC_RTP_TIME_STRIDE_FID :
            if(rtp_flags&ROHC_ENC_TIS_RTPFLAGP1_MASK)
            {
                /*Start SPR 6191 Fix: Corrected encoding of TIME_STRIDE*/
                (*p_p_rohc_pkt)+=rohc_c_encode_sdvl(*p_p_rohc_pkt,
                        ((p_csc->field_value[ROHC_RTP_TIME_STRIDE_FID-
                        ROHC_VAR_FID_BASE])));
                /*End SPR 6191 Fix*/
/*
                rohc_setbits(p_p_rohc_pkt,p_to_bit_pos,
                    p_csc->field_value[ROHC_RTP_TIME_STRIDE_FID-
                    ROHC_VAR_FID_BASE],0,
                p_csc->field_len[ROHC_RTP_TIME_STRIDE_FID-
                ROHC_VAR_FID_BASE]);
*/
            }
            break;
    }/* end switch field id*/                   
    ROHC_EXIT_FN("rohc_add_ele_p1_rtpflagsp1");
    return ROHC_SUCCESS;
}/* end function rohc_add_ele_p1_rtpflagsp1*/

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p1_ip4_static
******************************************************************************
**
**      DESCRIPTION:
**          Compose IP4 static buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to IP4 datagram     
**          Inner IP/Outer IP
**			Pointer to CSC
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_make_p1_ip4_static
    ROHC_PARAMS((p_location,p_csc,protocol),
    rohc_ip4_static_t   *p_location _AND_    
    rohc_csc_rtp_t      *p_csc      _AND_
    rohc_U8bit_t        protocol)
{
    ROHC_ENTER_FN("rohc_make_p1_ip4_static");
    p_location->ver =0x40;
    p_location->proto = protocol;
    if((protocol == ROHC_IPPROTO_IP4) ||
        (protocol == ROHC_IPPROTO_IP6) )
    {
        ROHC_SET_4_OCTS((p_location->src),p_csc->field_value[
                ROHC_OUTERIP4_SRC_FID-ROHC_VAR_FID_BASE]);
        ROHC_SET_4_OCTS((p_location->dst),p_csc->field_value[
                ROHC_OUTERIP4_DST_FID-ROHC_VAR_FID_BASE]);
    }
    else
    {
        ROHC_SET_4_OCTS((p_location->src),p_csc->field_value[
                ROHC_INNERIP4_SRC_FID-ROHC_VAR_FID_BASE]);
        ROHC_SET_4_OCTS((p_location->dst),p_csc->field_value[
                ROHC_INNERIP4_DST_FID-ROHC_VAR_FID_BASE]);
    }
    
    ROHC_EXIT_FN("rohc_make_p1_ip4_static");
}/* end function rohc_make_p1_ip4_static */
/*****************************************************************************
**      FUNCTION:
**              rohc_make_p2_ip4_static
******************************************************************************
**
**      DESCRIPTION:
**          Compose IP4 static buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to IP4 datagram     
**          Inner IP/Outer IP   
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_make_p2_ip4_static
    ROHC_PARAMS((p_location,p_csc,protocol),
    rohc_ip4_static_t  *p_location _AND_    
    rohc_csc_udp_t     *p_csc      _AND_
    rohc_U8bit_t       protocol)
{
    ROHC_ENTER_FN("rohc_make_p2_ip4_static");
    p_location->ver =0x40;
    p_location->proto = protocol;
    if((protocol == ROHC_IPPROTO_IP4) ||
        (protocol == ROHC_IPPROTO_IP6) )
    {
        ROHC_SET_4_OCTS((p_location->src),p_csc->field_value[
                ROHC_OUTERIP4_SRC_FID-ROHC_VAR_FID_BASE]);
        ROHC_SET_4_OCTS((p_location->dst),p_csc->field_value[
                ROHC_OUTERIP4_DST_FID-ROHC_VAR_FID_BASE]);
    }
    else
    {
        ROHC_SET_4_OCTS((p_location->src),p_csc->field_value[
                ROHC_INNERIP4_SRC_FID-ROHC_VAR_FID_BASE]);
        ROHC_SET_4_OCTS((p_location->dst),p_csc->field_value[
                ROHC_INNERIP4_DST_FID-ROHC_VAR_FID_BASE]);
    }
    
    ROHC_EXIT_FN("rohc_make_p2_ip4_static");
}/* end function rohc_make_p2_ip4_static */

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p6_ip4_static
******************************************************************************
**
**      DESCRIPTION:
**          Compose IP4 static buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to IP4 datagram     
**          Inner IP/Outer IP   
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
static void 
rohc_make_p6_ip4_static
    ROHC_PARAMS((p_location,p_csc,protocol),
    rohc_ip4_static_t  *p_location _AND_    
    rohc_csc_tcp_t     *p_csc      _AND_
    rohc_U8bit_t       protocol)
{
    ROHC_ENTER_FN("rohc_make_p6_ip4_static");
    p_location->ver =0x00; /* For Prof6, IPv4 version is the MSB of this field set to 0*/
    p_location->proto = protocol;
    if((protocol == ROHC_IPPROTO_IP4) ||
        (protocol == ROHC_IPPROTO_IP6) )
    {
        ROHC_SET_4_OCTS((p_location->src),p_csc->field_value[
                ROHC_OUTERIP4_SRC_FID-ROHC_VAR_FID_BASE]);
        ROHC_SET_4_OCTS((p_location->dst),p_csc->field_value[
                ROHC_OUTERIP4_DST_FID-ROHC_VAR_FID_BASE]);
    }
    else
    {
        ROHC_SET_4_OCTS((p_location->src),p_csc->field_value[
                ROHC_INNERIP4_SRC_FID-ROHC_VAR_FID_BASE]);
        ROHC_SET_4_OCTS((p_location->dst),p_csc->field_value[
                ROHC_INNERIP4_DST_FID-ROHC_VAR_FID_BASE]);
    }
    
    ROHC_EXIT_FN("rohc_make_p6_ip4_static");
}/* end function rohc_make_p6_ip4_static */

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p1_ip6_static
******************************************************************************
**
**      DESCRIPTION:
**          Compose IP6 static buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to IP6 datagram     
**          Inner IP/Outer IP   
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_make_p1_ip6_static
    ROHC_PARAMS((p_location,p_csc,protocol),
    rohc_ip6_static_t   * p_location _AND_    
    rohc_csc_rtp_t      * p_csc      _AND_
    rohc_U8bit_t        protocol)
{
    ROHC_ENTER_FN("rohc_make_p1_ip6_static");
    p_location->next_hdr= protocol;
    if((protocol == ROHC_IPPROTO_IP4) ||
        (protocol == ROHC_IPPROTO_IP6) )
    {
        p_location->v_flowlabel=0x60 | (p_csc->field_value[
                ROHC_OUTERIP6_FLOWLABEL_FID-ROHC_VAR_FID_BASE])>>16;
        ROHC_SET_2_OCTS((p_location->flowlabel),p_csc->field_value[
                ROHC_OUTERIP6_FLOWLABEL_FID-ROHC_VAR_FID_BASE]);
        
    ROHC_MEMCPY(p_location->src,p_csc->outerip6_src,16);        
    ROHC_MEMCPY(p_location->dst,p_csc->outerip6_dst,16);        
        
    }
    else
    {
        p_location->v_flowlabel=0x60 | (p_csc->field_value[
                ROHC_INNERIP6_FLOWLABEL_FID-ROHC_VAR_FID_BASE])>>16;
        ROHC_SET_2_OCTS((p_location->flowlabel),p_csc->field_value[
                ROHC_INNERIP6_FLOWLABEL_FID-ROHC_VAR_FID_BASE]);
                
    ROHC_MEMCPY(p_location->src,p_csc->innerip6_src,16);        
    ROHC_MEMCPY(p_location->dst,p_csc->innerip6_dst,16);        

    }
    ROHC_EXIT_FN("rohc_make_p1_ip6_static");
}/* end function rohc_make_p1_ip6_static */
/*****************************************************************************
**      FUNCTION:
**              rohc_make_p2_ip6_static
******************************************************************************
**
**      DESCRIPTION:
**          Compose IP6 static buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to IP6 datagram     
**          Inner IP/Outer IP   
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_make_p2_ip6_static
    ROHC_PARAMS((p_location,p_csc,protocol),
    rohc_ip6_static_t * p_location _AND_    
    rohc_csc_udp_t    * p_csc      _AND_
    rohc_U8bit_t      protocol)
{
    ROHC_ENTER_FN("rohc_make_p2_ip6_static");
    p_location->next_hdr= protocol;
    if((protocol == ROHC_IPPROTO_IP4) ||
        (protocol == ROHC_IPPROTO_IP6) )
    {
        p_location->v_flowlabel=0x60 | (p_csc->field_value[
                ROHC_OUTERIP6_FLOWLABEL_FID-ROHC_VAR_FID_BASE])>>16;
        ROHC_SET_2_OCTS((p_location->flowlabel),p_csc->field_value[
                ROHC_OUTERIP6_FLOWLABEL_FID-ROHC_VAR_FID_BASE]);
        
    ROHC_MEMCPY(p_location->src,p_csc->outerip6_src,16);        
    ROHC_MEMCPY(p_location->dst,p_csc->outerip6_dst,16);        
        
    }
    else
    {
        p_location->v_flowlabel=0x60 | (p_csc->field_value[
                ROHC_INNERIP6_FLOWLABEL_FID-ROHC_VAR_FID_BASE])>>16;
        ROHC_SET_2_OCTS((p_location->flowlabel),p_csc->field_value[
                ROHC_INNERIP6_FLOWLABEL_FID-ROHC_VAR_FID_BASE]);
                
    ROHC_MEMCPY(p_location->src,p_csc->innerip6_src,16);        
    ROHC_MEMCPY(p_location->dst,p_csc->innerip6_dst,16);        

    }
    ROHC_EXIT_FN("rohc_make_p2_ip6_static");
}/* end function rohc_make_p2_ip6_static */

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p1_udp_static
******************************************************************************
**
**      DESCRIPTION:
**          Compose UDP static buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to UDP datagram     
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_make_p1_udp_static
    ROHC_PARAMS((p_location,p_csc),
    rohc_udp_static_t   * p_location _AND_    
    rohc_csc_rtp_t      * p_csc )
{
    ROHC_ENTER_FN("rohc_make_p1_udp_static");
    ROHC_SET_2_OCTS((p_location->src),p_csc->field_value[
                ROHC_UDP_SRC_FID-ROHC_VAR_FID_BASE]);
    ROHC_SET_2_OCTS((p_location->dst),p_csc->field_value[
                ROHC_UDP_DST_FID-ROHC_VAR_FID_BASE]);
    ROHC_EXIT_FN("rohc_make_p1_udp_static");
}/* end function rohc_make_p1_udp_static */

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p2_udp_static
******************************************************************************
**
**      DESCRIPTION:
**          Compose UDP static buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to UDP datagram     
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_make_p2_udp_static
    ROHC_PARAMS((p_location,p_csc),
    rohc_udp_static_t * p_location _AND_    
    rohc_csc_udp_t * p_csc )
{
    ROHC_ENTER_FN("rohc_make_p2_udp_static");
    ROHC_SET_2_OCTS((p_location->src),p_csc->field_value[
                ROHC_UDP_SRC_FID-ROHC_VAR_FID_BASE]);
    ROHC_SET_2_OCTS((p_location->dst),p_csc->field_value[
                ROHC_UDP_DST_FID-ROHC_VAR_FID_BASE]);
    ROHC_EXIT_FN("rohc_make_p2_udp_static");
}/* end function rohc_make_p2_udp_static */

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p1_rtp_static
******************************************************************************
**
**      DESCRIPTION:
**          Compose RTP static buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to RTP datagram     
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_make_p1_rtp_static
    ROHC_PARAMS((p_location,p_csc),
    rohc_U8bit_t    * p_location _AND_ 
    rohc_csc_rtp_t  * p_csc )
{
    ROHC_ENTER_FN("rohc_make_p1_rtp_static");
    ROHC_SET_4_OCTS(p_location,p_csc->field_value[
                ROHC_RTP_SSRC_FID-ROHC_VAR_FID_BASE]);
    ROHC_EXIT_FN("rohc_make_p1_rtp_static");
}/* end function rohc_make_p1_rtp_static */

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p6_tcp_static
******************************************************************************
**
**      DESCRIPTION:
**          Compose TCP static buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to TCP datagram     
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
static void 
rohc_make_p6_tcp_static
    ROHC_PARAMS((p_location,p_csc),
    rohc_tcp_static_t   * p_location _AND_    
    rohc_csc_tcp_t      * p_csc )
{
    ROHC_ENTER_FN("rohc_make_p6_tcp_static");
    ROHC_SET_2_OCTS((p_location->src),p_csc->field_value[
                ROHC_TCP_SRC_PORT_FID-ROHC_VAR_FID_BASE]);
    ROHC_SET_2_OCTS((p_location->dst),p_csc->field_value[
                ROHC_TCP_DST_PORT_FID-ROHC_VAR_FID_BASE]);
    ROHC_EXIT_FN("rohc_make_p6_tcp_static");
}/* end function rohc_make_p6_tcp_static */
/*****************************************************************************
**      FUNCTION:
**              rohc_make_p1_ip4_dyn
******************************************************************************
**
**      DESCRIPTION:
**          Compose IP4 dynamic buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to IP4 datagram     
**          Pointer to CSC     
**          Inner/Outer IP     
**          Pointer to entity 
**			Pointer to packet length
**			Pointer to Error Code    
**      RETURN VALUE:
**              ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
STATIC rohc_return_t 
rohc_make_p1_ip4_dyn
    ROHC_PARAMS((p_location,p_csc,ip_type,p_entity,p_len,p_ecode),
    struct rohc_ip4_dyn_t   * p_location    _AND_    
    struct rohc_csc_rtp_t   * p_csc         _AND_
    rohc_U8bit_t            ip_type         _AND_
    struct rohc_entity_t    * p_entity      _AND_
    rohc_U32bit_t           *p_len          _AND_
    rohc_error_code_t       *p_ecode)
{
    ROHC_ENTER_FN("rohc_make_p1_ip4_dyn");
    
    if(ip_type == ROHC_INNERIP)
    {
        p_location->tos=p_csc->field_value[
                    ROHC_INNERIP4_TOS_FID-ROHC_VAR_FID_BASE];
        p_location->ttl=p_csc->field_value[
                    ROHC_INNERIP4_TTL_FID-ROHC_VAR_FID_BASE];   
        ROHC_SET_2_OCTS((p_location->id),p_csc->field_value[
                ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE]);
        p_location->flags= p_csc->field_value[
                    ROHC_INNERIP_DF_FID-ROHC_VAR_FID_BASE] << 7;
        p_location->flags|=p_csc->new_inner_rnd<<6;
        p_location->flags|=p_csc->new_inner_nbo<<5;
        if(ROHC_FAILURE == rohc_list_encoding_p1(p_entity,
                ROHC_INNERIP_EXTHEADERS_LIST_FID,
                p_csc,p_len,
                (rohc_U8bit_t *)p_location+sizeof(rohc_ip4_dyn_t),
                p_ecode))
        {
            return ROHC_FAILURE;
        }
    }
    else
    {
        p_location->tos=p_csc->field_value[
                    ROHC_OUTERIP4_TOS_FID-ROHC_VAR_FID_BASE];
        p_location->ttl=p_csc->field_value[
                    ROHC_OUTERIP4_TTL_FID-ROHC_VAR_FID_BASE];   
        ROHC_SET_2_OCTS((p_location->id),p_csc->field_value[
                ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE]);
        p_location->flags= p_csc->field_value[
                    ROHC_OUTERIP_DF_FID-ROHC_VAR_FID_BASE] << 7;
        /*Start SPR 6103 Fix*/
        p_location->flags|=p_csc->new_outer_rnd<<6;
        /*End SPR 6103 Fix*/
        p_location->flags|=p_csc->new_outer_nbo<<5;
        if(ROHC_FAILURE == rohc_list_encoding_p1(p_entity,
                ROHC_OUTERIP_EXTHEADERS_LIST_FID,
                p_csc,p_len,
                (rohc_U8bit_t *)p_location+sizeof(rohc_ip4_dyn_t),
                p_ecode))
        {
            return ROHC_FAILURE;
        }
    }
    *p_len += sizeof(rohc_ip4_dyn_t);
    ROHC_EXIT_FN("rohc_make_p1_ip4_dyn");
    return ROHC_SUCCESS;
}/* end function rohc_make_p1_ip4_dyn */
/*****************************************************************************
**      FUNCTION:
**              rohc_make_p2_ip4_dyn
******************************************************************************
**
**      DESCRIPTION:
**          Compose IP4 dynamic buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to IP4 datagram     
**          Pointer to CSC     
**          Inner/Outer IP     
**          Pointer to Rohc entity
**			Pointer to packet Lenght
**			Pointer to Error Code  
**      RETURN VALUE:
**              ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
STATIC rohc_return_t 
rohc_make_p2_ip4_dyn
    ROHC_PARAMS((p_location,p_csc,ip_type,p_entity,p_len,p_ecode),
    struct rohc_ip4_dyn_t * p_location _AND_    
    struct rohc_csc_udp_t * p_csc      _AND_
    rohc_U8bit_t          ip_type      _AND_
    struct rohc_entity_t  * p_entity   _AND_
    rohc_U32bit_t         *p_len       _AND_
    rohc_error_code_t     *p_ecode)
{
    ROHC_ENTER_FN("rohc_make_p2_ip4_dyn");
    
    if(ip_type == ROHC_INNERIP)
    {
        p_location->tos=p_csc->field_value[
                    ROHC_INNERIP4_TOS_FID-ROHC_VAR_FID_BASE];
        p_location->ttl=p_csc->field_value[
                    ROHC_INNERIP4_TTL_FID-ROHC_VAR_FID_BASE];   
        ROHC_SET_2_OCTS((p_location->id),p_csc->field_value[
                ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE]);
        p_location->flags= p_csc->field_value[
                    ROHC_INNERIP_DF_FID-ROHC_VAR_FID_BASE] << 7;
        p_location->flags|=p_csc->new_inner_rnd<<6;
        p_location->flags|=p_csc->new_inner_nbo<<5;
        if(ROHC_FAILURE == rohc_list_encoding_p2(p_entity,
                ROHC_INNERIP_EXTHEADERS_LIST_FID,
                p_csc,p_len,
                (rohc_U8bit_t *)p_location+sizeof(rohc_ip4_dyn_t),
                p_ecode))
        {
            return ROHC_FAILURE;
        }
    }
    else
    {
        p_location->tos=p_csc->field_value[
                    ROHC_OUTERIP4_TOS_FID-ROHC_VAR_FID_BASE];
        p_location->ttl=p_csc->field_value[
                    ROHC_OUTERIP4_TTL_FID-ROHC_VAR_FID_BASE];   
        ROHC_SET_2_OCTS((p_location->id),p_csc->field_value[
                ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE]);
        p_location->flags= p_csc->field_value[
                    ROHC_OUTERIP_DF_FID-ROHC_VAR_FID_BASE] << 7;
        /*Start SPR 6103 Fix*/
        p_location->flags|=p_csc->new_outer_rnd<<6;
        /*End SPR 6103 Fix*/
        p_location->flags|=p_csc->new_outer_nbo<<5;
        if(ROHC_FAILURE == rohc_list_encoding_p2(p_entity,
                ROHC_OUTERIP_EXTHEADERS_LIST_FID,
                p_csc,p_len,
                (rohc_U8bit_t *)p_location+sizeof(rohc_ip4_dyn_t),
                p_ecode))
        {
            return ROHC_FAILURE;
        }
    }
    *p_len += sizeof(rohc_ip4_dyn_t);
    ROHC_EXIT_FN("rohc_make_p2_ip4_dyn");
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p6_ip4_dyn
******************************************************************************
**
**      DESCRIPTION:
**          Compose IP4 dynamic buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to IP4 datagram     
**          Pointer to CSC     
**          Inner/Outer IP     
**          Pointer to entity  
**			Pointer to Packet Length   
**      RETURN VALUE:
**              ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
/* +- SPR 17777 */
static rohc_return_t 
rohc_make_p6_ip4_dyn
    ROHC_PARAMS((p_location,p_csc,ip_type,p_len),
    rohc_U8bit_t   * p_location    _AND_    
    struct rohc_csc_tcp_t   * p_csc         _AND_
    rohc_U8bit_t            ip_type         _AND_
    rohc_U32bit_t           *p_len)
{
    ROHC_ENTER_FN("rohc_make_p6_ip4_dyn");
    rohc_ip4_prof6_dyn_t *rohc_p6_ip4_dyn = NULL;
    rohc_p6_ip4_dyn = (rohc_ip4_prof6_dyn_t*)p_location;

    if(ip_type == ROHC_INNERIP)
    {
        rohc_p6_ip4_dyn->res_df_ipid_beh = 0;
        rohc_p6_ip4_dyn->res_df_ipid_beh|= p_csc->field_value[
            ROHC_INNERIP_DF_FID-ROHC_VAR_FID_BASE] << 2;
        rohc_p6_ip4_dyn->res_df_ipid_beh|= p_csc->field_value[
            ROHC_INNERIP_BEH_FID-ROHC_VAR_FID_BASE];
        rohc_p6_ip4_dyn->dscp_ecn = 0;
        rohc_p6_ip4_dyn->dscp_ecn |= p_csc->field_value[
            ROHC_INNERIP4_DSCP_FID-ROHC_VAR_FID_BASE]<<2;
        rohc_p6_ip4_dyn->dscp_ecn |= p_csc->field_value[
            ROHC_INNERIP4_ECN_FID-ROHC_VAR_FID_BASE];
        rohc_p6_ip4_dyn->ttl_hopl=p_csc->field_value[
            ROHC_INNERIP4_TTL_FID-ROHC_VAR_FID_BASE];   
        p_location += sizeof(struct rohc_ip4_prof6_dyn_t);
        *p_len += sizeof(rohc_ip4_prof6_dyn_t);
        /* currently compressor supporting only random or zero ip-id beh*/
        if(IP_ID_BEHAVIOR_RANDOM == p_csc->field_value[ROHC_INNERIP_BEH_FID-
                ROHC_VAR_FID_BASE])
        {

            ROHC_SET_2_OCTS((rohc_U8bit_t*)p_location,(rohc_U16bit_t)p_csc->field_value[
                    ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE]);
            *p_len += sizeof(rohc_ip4_prof6_dyn_id_t); 
        }
        else
        {
            /* ip id behavior is zero so need to send ip-id*/
        }
    }
    else
    {
        /* Tunneling part will be done later*/
    }
    ROHC_EXIT_FN("rohc_make_p6_ip4_dyn");
    return ROHC_SUCCESS;
}/* end function rohc_make_p6_ip4_dyn */

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p1_ip6_dyn
******************************************************************************
**
**      DESCRIPTION:
**          Compose IP6 dynamic buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to IP6 datagram     
**          Pointer to CSC     
**          Inner/Outer IP     
**          Pointer to Rohc entity     
**          Pointer to Packet Length   
**          Pointer to Error Code 
**      RETURN VALUE:
**              ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
STATIC rohc_return_t 
rohc_make_p1_ip6_dyn
    ROHC_PARAMS((p_location,p_csc,ip_type,p_entity,p_len,p_ecode),
    struct rohc_ip6_dyn_t   * p_location _AND_    
    struct rohc_csc_rtp_t   * p_csc      _AND_
    rohc_U8bit_t            ip_type      _AND_
    struct rohc_entity_t    * p_entity   _AND_
    rohc_U32bit_t           *p_len       _AND_
    rohc_error_code_t       *p_ecode)
{
    ROHC_ENTER_FN("rohc_make_p1_ip6_dyn");
    
    if(ip_type == ROHC_INNERIP)
    {
        p_location->traffic_class=p_csc->field_value[
                    ROHC_INNERIP6_TCLASS_FID-ROHC_VAR_FID_BASE];
        p_location->hop_limit=p_csc->field_value[
                    ROHC_INNERIP6_HLIM_FID-ROHC_VAR_FID_BASE];
	/* SPR 14085 fix start */
	/* Removed check */
	/* SPR 14085 fix end */

        if(ROHC_FAILURE == rohc_list_encoding_p1(p_entity,
                    ROHC_INNERIP_EXTHEADERS_LIST_FID,
                    p_csc,p_len,
                    (rohc_U8bit_t *)p_location+sizeof(rohc_ip6_dyn_t),
                    p_ecode))
        {
            return ROHC_FAILURE;
        }
    }
    else
    {
        p_location->traffic_class=p_csc->field_value[
                    ROHC_OUTERIP6_TCLASS_FID-ROHC_VAR_FID_BASE];
        p_location->hop_limit=p_csc->field_value[
                    ROHC_OUTERIP6_HLIM_FID-ROHC_VAR_FID_BASE];
        if(ROHC_FAILURE == rohc_list_encoding_p1(p_entity,
                    ROHC_OUTERIP_EXTHEADERS_LIST_FID,
                    p_csc,p_len,
                    (rohc_U8bit_t *)p_location+sizeof(rohc_ip6_dyn_t),
                    p_ecode))
        {
            return ROHC_FAILURE;
        }
    }
    *p_len += sizeof(rohc_ip6_dyn_t);
    ROHC_EXIT_FN("rohc_make_p1_ip6_dyn");
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p2_ip6_dyn
******************************************************************************
**
**      DESCRIPTION:
**          Compose IP6 dynamic buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to IP6 datagram     
**          Pointer to CSC     
**          Inner/Outer IP     
**          Pointer to Rohc entity     
**          Pointer to Packet Length     
**          Pointer to Error Code     
**      RETURN VALUE:
**              ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
STATIC rohc_return_t 
rohc_make_p2_ip6_dyn
    ROHC_PARAMS((p_location,p_csc,ip_type,p_entity,p_len,p_ecode),
    struct rohc_ip6_dyn_t * p_location _AND_    
    struct rohc_csc_udp_t * p_csc      _AND_
    rohc_U8bit_t          ip_type      _AND_
    struct rohc_entity_t  * p_entity   _AND_
    rohc_U32bit_t         *p_len       _AND_
    rohc_error_code_t     *p_ecode)
{
    ROHC_ENTER_FN("rohc_make_p2_ip6_dyn");
    
    if(ip_type == ROHC_INNERIP)
    {
        p_location->traffic_class=p_csc->field_value[
                    ROHC_INNERIP6_TCLASS_FID-ROHC_VAR_FID_BASE];
        p_location->hop_limit=p_csc->field_value[
                    ROHC_INNERIP6_HLIM_FID-ROHC_VAR_FID_BASE];
	/* SPR 14085 fix start */
	/* Removed check */
	/* SPR 14085 fix end */
        if(ROHC_FAILURE == rohc_list_encoding_p2(p_entity,
                    ROHC_INNERIP_EXTHEADERS_LIST_FID,
                    p_csc,p_len,
                    (rohc_U8bit_t *)p_location+sizeof(rohc_ip6_dyn_t),
                    p_ecode))
        {
            return ROHC_FAILURE;
        }
    }
    else
    {
        p_location->traffic_class=p_csc->field_value[
                    ROHC_OUTERIP6_TCLASS_FID-ROHC_VAR_FID_BASE];
        p_location->hop_limit=p_csc->field_value[
                    ROHC_OUTERIP6_HLIM_FID-ROHC_VAR_FID_BASE];
        if(ROHC_FAILURE == rohc_list_encoding_p2(p_entity,
                    ROHC_OUTERIP_EXTHEADERS_LIST_FID,
                    p_csc,p_len,
                    (rohc_U8bit_t *)p_location+sizeof(rohc_ip6_dyn_t),
                    p_ecode))
        {
            return ROHC_FAILURE;
        }
    }
    *p_len += sizeof(rohc_ip6_dyn_t);
    ROHC_EXIT_FN("rohc_make_p2_ip6_dyn");
    return ROHC_SUCCESS;
}   

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p1_udp_dyn
******************************************************************************
**
**      DESCRIPTION:
**          Compose UDP dynamic buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to UDP datagram     
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_make_p1_udp_dyn
    ROHC_PARAMS((p_location,p_csc),
    rohc_U8bit_t    * p_location _AND_ 
    rohc_csc_rtp_t  * p_csc )
{
    ROHC_ENTER_FN("rohc_make_p1_udp_dyn");
    ROHC_SET_2_OCTS(p_location,p_csc->field_value[
                ROHC_UDP_CKSUM_FID-ROHC_VAR_FID_BASE]);
    ROHC_EXIT_FN("rohc_make_p1_udp_dyn");
}/* end function rohc_make_p1_udp_dyn */

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p2_udp_dyn
******************************************************************************
**
**      DESCRIPTION:
**          Compose UDP dynamic buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to UDP datagram     
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
STATIC void 
rohc_make_p2_udp_dyn
    ROHC_PARAMS((p_location,p_csc),
    rohc_U8bit_t * p_location _AND_ 
    rohc_csc_udp_t * p_csc )
{
    ROHC_ENTER_FN("rohc_make_p2_udp_dyn");
    ROHC_SET_2_OCTS(p_location,p_csc->field_value[
                ROHC_UDP_CKSUM_FID-ROHC_VAR_FID_BASE]);
    ROHC_EXIT_FN("rohc_make_p2_udp_dyn");
}/* end function rohc_make_p2_udp_dyn */
   
/*****************************************************************************
**      FUNCTION:
**              rohc_make_p1_rtp_dyn
******************************************************************************
**
**      DESCRIPTION:
**          Compose rtp dynamic buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to rtp datagram     
**          Pointer to CSC     
**          Pointer to Rohc entity     
**          Pointer to Packet Length     
**          Pointer to Error Code 
**      RETURN VALUE:
**              ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
STATIC rohc_return_t 
rohc_make_p1_rtp_dyn
    ROHC_PARAMS((p_location,p_csc,p_entity,p_len,p_ecode),
    rohc_U8bit_t            * p_location    _AND_   
    struct rohc_csc_rtp_t   * p_csc         _AND_
    struct rohc_entity_t    * p_entity      _AND_
    rohc_U32bit_t           *p_len          _AND_
    rohc_error_code_t       *p_ecode)
{
    struct rohc_rtp_tail_dyn_t * p_rtp_tail_dyn;
    struct rohc_rtp_head_dyn_t * p_rtp_head_dyn;
    rohc_U8bit_t    * p_org=(rohc_U8bit_t *)p_location;
    ROHC_ENTER_FN("rohc_make_p1_rtp_dyn");
    *p_len = 0;

    /* 
    ** START_SPR_567_FIX : Corrected the creation of RTP IR-DYN packets.
    */
     p_rtp_head_dyn=(struct rohc_rtp_head_dyn_t *)p_location;
     p_rtp_head_dyn->ver_p_rx_cc=0x90;
     p_rtp_head_dyn->ver_p_rx_cc|=p_csc->field_value[
                     ROHC_RTP_R_P_FID-ROHC_VAR_FID_BASE]<<5;
     p_rtp_head_dyn->ver_p_rx_cc|=p_csc->field_value[
                     ROHC_RTP_CC_FID-ROHC_VAR_FID_BASE];
     p_rtp_head_dyn->m_pt=p_csc->field_value[
                     ROHC_RTP_M_FID-ROHC_VAR_FID_BASE]<<7 |
                     p_csc->field_value[ROHC_RTP_PT_FID-ROHC_VAR_FID_BASE];
     ROHC_SET_2_OCTS((p_rtp_head_dyn->sn),p_csc->field_value[
                 ROHC_SN_FID-ROHC_VAR_FID_BASE]);
     ROHC_SET_4_OCTS((p_rtp_head_dyn->ts),p_csc->field_value[
                 ROHC_TS_FID-ROHC_VAR_FID_BASE]);
     /* SPR 3548 */
     p_location += sizeof(rohc_rtp_head_dyn_t); 
    if(p_rtp_head_dyn->ver_p_rx_cc & ROHC_DYN_CC_MASK )
    /* END_SPR_567_FIX */
    {
        if(ROHC_FAILURE == rohc_list_encoding_p1(p_entity,
                    ROHC_RTP_CSRC_LIST_FID ,
                    p_csc,p_len,
                    (rohc_U8bit_t *)p_location,
                    p_ecode))
        {
            return ROHC_FAILURE;
        }
		/* SPR 3548 */
        p_location += *p_len;   
    }
    /*Start SPR 5811 Fix*/
    /*Generic CSRC list  is always at least one octet in size, even if the
     *list is empty*/
    else
    {
        p_location += 1; 
    }
    /*End SPR 5811 Fix*/
    p_rtp_tail_dyn=(struct rohc_rtp_tail_dyn_t *)p_location;
    p_rtp_tail_dyn->x_mode_tis_tss=0;
    p_rtp_tail_dyn->x_mode_tis_tss|=p_csc->field_value[
                ROHC_RTP_X_FID-ROHC_VAR_FID_BASE]<<4;
    p_rtp_tail_dyn->x_mode_tis_tss|=ROHC_CSC_GET_MODE(p_csc)<<2;
    p_rtp_tail_dyn->x_mode_tis_tss|=
    (p_csc->field_value[ROHC_RTP_TIS_FID-ROHC_VAR_FID_BASE]?0x02:0);
    p_rtp_tail_dyn->x_mode_tis_tss|=
    (p_csc->field_value[ROHC_RTP_TSS_FID-ROHC_VAR_FID_BASE]?0x01:0);
    p_location += sizeof(rohc_rtp_tail_dyn_t);
    if(p_csc->field_value[ROHC_RTP_TSS_FID-ROHC_VAR_FID_BASE])
    {
        p_location += rohc_c_encode_sdvl(
            (rohc_U8bit_t*)p_location,
            p_csc->field_value[ROHC_RTP_TS_STRIDE_FID-ROHC_VAR_FID_BASE]);
    }
    if(p_csc->field_value[ROHC_RTP_TIS_FID-ROHC_VAR_FID_BASE])
    {
        p_location += rohc_c_encode_sdvl(
            (rohc_U8bit_t*)p_location,
            p_csc->field_value[ROHC_RTP_TIME_STRIDE_FID-ROHC_VAR_FID_BASE]);
    }
    *p_len = ((rohc_U8bit_t *)p_location-p_org);
    ROHC_EXIT_FN("rohc_make_p1_rtp_dyn");
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p6_tcp_dyn
******************************************************************************
**
**      DESCRIPTION:
**          Compose TCP dynamic buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to TCP datagram  
**			Pointer to CSC   
**			Pointer to Packet Length
**			Pointer to Error Code   
**      RETURN VALUE:
**              ROHC_SUCCESS/ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
/* +- SPR 17777 */
static rohc_return_t
rohc_make_p6_tcp_dyn
    ROHC_PARAMS((p_location,p_csc,p_len),
    struct rohc_tcp_dyn_partial_t     * p_location    _AND_   
    struct rohc_csc_tcp_t   * p_csc         _AND_
    rohc_U32bit_t           *p_len)
    /* +- SPR 17777 */
{
    ROHC_ENTER_FN("rohc_make_p6_tcp_dyn");
    *p_len = 0;
    p_location->ecnu_acksf_ackz_urpz_resr = 0;
    p_location->ecnu_acksf_ackz_urpz_resr|=p_csc->field_value[
                     ROHC_TCP_ECN_USED_FID-ROHC_VAR_FID_BASE]<<7;
    p_location->ecnu_acksf_ackz_urpz_resr|=p_csc->field_value[
                     ROHC_TCP_ACK_STRIDE_FLAG_FID-ROHC_VAR_FID_BASE]<<6;
    p_location->ecnu_acksf_ackz_urpz_resr|=p_csc->field_value[
                     ROHC_TCP_ACK_ZERO_FID-ROHC_VAR_FID_BASE]<<5;
    p_location->ecnu_acksf_ackz_urpz_resr|=p_csc->field_value[
                     ROHC_TCP_URP_ZERO_FID-ROHC_VAR_FID_BASE]<<4;
    p_location->ecnu_acksf_ackz_urpz_resr|=p_csc->field_value[
                     ROHC_TCP_RES_FLAG_FID-ROHC_VAR_FID_BASE];
    p_location->ecnf_urgf_ackf_pshf_rsf = 0;
    p_location->ecnf_urgf_ackf_pshf_rsf|=p_csc->field_value[
                     ROHC_TCP_ECN_FLAG_FID-ROHC_VAR_FID_BASE]<<6;
    p_location->ecnf_urgf_ackf_pshf_rsf|=p_csc->field_value[
                     ROHC_TCP_URG_FLAG_FID-ROHC_VAR_FID_BASE]<<5;
    p_location->ecnf_urgf_ackf_pshf_rsf|=p_csc->field_value[
                     ROHC_TCP_ACK_FLAG_FID-ROHC_VAR_FID_BASE]<<4;
    p_location->ecnf_urgf_ackf_pshf_rsf|=p_csc->field_value[
                     ROHC_TCP_PSH_FLAG_FID-ROHC_VAR_FID_BASE]<<3;
    p_location->ecnf_urgf_ackf_pshf_rsf|=p_csc->field_value[
                     ROHC_TCP_RST_FLAG_FID-ROHC_VAR_FID_BASE]<<2;
    p_location->ecnf_urgf_ackf_pshf_rsf|=p_csc->field_value[
                     ROHC_TCP_SYN_FLAG_FID-ROHC_VAR_FID_BASE]<<1;
    p_location->ecnf_urgf_ackf_pshf_rsf|=p_csc->field_value[
                     ROHC_TCP_FIN_FLAG_FID-ROHC_VAR_FID_BASE];
     ROHC_SET_2_OCTS((p_location->msn),(rohc_U16bit_t)p_csc->field_value[
                 ROHC_TCP_MSN_FID-ROHC_VAR_FID_BASE]);
     ROHC_SET_4_OCTS((p_location->seq_num),p_csc->field_value[
                 ROHC_SN_FID-ROHC_VAR_FID_BASE]);
     *p_len += sizeof(rohc_tcp_dyn_partial_t);
     ROHC_EXIT_FN("rohc_make_p6_tcp_dyn");
    return ROHC_SUCCESS;
}/* end function rohc_make_p6_tcp_dyn */

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p6_tcp_dyn_ack
******************************************************************************
**
**      DESCRIPTION:
**          Compose dynamic buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to CSC     
**      RETURN VALUE:
**              ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
static void 
rohc_make_p6_tcp_dyn_ack
    ROHC_PARAMS((p_location,p_csc),
    rohc_U8bit_t    * p_location _AND_ 
    rohc_csc_tcp_t  * p_csc )
{
    ROHC_ENTER_FN("rohc_make_p6_tcp_dyn_ack");
    ROHC_SET_4_OCTS(p_location,p_csc->field_value[
                ROHC_TCP_ACKNO_FID-ROHC_VAR_FID_BASE]);
    ROHC_EXIT_FN("rohc_make_p6_tcp_dyn_ack");
}/* end function rohc_make_p6_tcp_dyn_ack */

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p6_tcp_dyn_urgptr
******************************************************************************
**
**      DESCRIPTION:
**          Compose dynamic buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to CSC     
**      RETURN VALUE:
**              ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
static void 
rohc_make_p6_tcp_dyn_urgptr
    ROHC_PARAMS((p_location,p_csc),
    rohc_U8bit_t    * p_location _AND_ 
    rohc_csc_tcp_t  * p_csc )
{
    ROHC_ENTER_FN("rohc_make_p6_tcp_dyn_urgptr");
    ROHC_SET_2_OCTS(p_location,(rohc_U16bit_t)p_csc->field_value[
                ROHC_TCP_URG_PTR_FID-ROHC_VAR_FID_BASE]);
    ROHC_EXIT_FN("rohc_make_p6_tcp_dyn_urgptr");
}/* end function rohc_make_p6_tcp_dyn_urgptr */

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p6_tcp_dyn_ackstride
******************************************************************************
**
**      DESCRIPTION:
**          Compose dynamic buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to CSC     
**      RETURN VALUE:
**              ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
static void 
rohc_make_p6_tcp_dyn_ackstride
    ROHC_PARAMS((p_location,p_csc),
    rohc_U8bit_t    * p_location _AND_ 
    rohc_csc_tcp_t  * p_csc )
{
    ROHC_ENTER_FN("rohc_make_p6_tcp_dyn_ackstride");
    ROHC_SET_2_OCTS(p_location,(rohc_U16bit_t)p_csc->field_value[
                ROHC_TCP_ACK_STRIDE_FID-ROHC_VAR_FID_BASE]);
    ROHC_EXIT_FN("rohc_make_p6_tcp_dyn_ackstride");
}/* end function rohc_make_p6_tcp_dyn_ackstride */

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p6_tcp_dyn_wind_cksum
******************************************************************************
**
**      DESCRIPTION:
**          Compose dynamic buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to CSC     
**      RETURN VALUE:
**              ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
static void 
rohc_make_p6_tcp_dyn_wind_cksum
    ROHC_PARAMS((p_location,p_csc),
    rohc_tcp_dyn_windncksum_t   * p_location _AND_ 
    rohc_csc_tcp_t  * p_csc )
{
    ROHC_ENTER_FN("rohc_make_p6_tcp_dyn_ack");
    ROHC_SET_2_OCTS((p_location->window),(rohc_U16bit_t)p_csc->field_value[
                 ROHC_TCP_WINDOW_FID-ROHC_VAR_FID_BASE]);
    ROHC_SET_2_OCTS((p_location->checksum),(rohc_U16bit_t)p_csc->field_value[
                 ROHC_TCP_CKSUM_FID-ROHC_VAR_FID_BASE]);
}
/*****************************************************************************
**      FUNCTION:
**              rohc_make_p1_dyn_chain
******************************************************************************
**
**      DESCRIPTION:
**          Compose dynamic buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to CSC     
**          Pointer to entity
** 			Pointer to Packet Length 
**			Pointer to Error Code     
**      RETURN VALUE:
**              ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
STATIC rohc_return_t 
rohc_make_p1_dyn_chain
    ROHC_PARAMS((p_location,p_csc,p_entity,p_len,p_ecode),
    rohc_U8bit_t            * p_location    _AND_  
    struct rohc_csc_rtp_t   * p_csc         _AND_
    struct rohc_entity_t    * p_entity      _AND_
    rohc_U32bit_t           *p_len          _AND_
    rohc_error_code_t       *p_ecode)
{
    /* Coverity 11404 starts*/
    rohc_U32bit_t len = 0;
    /* Coverity 11404 ends*/
    *p_len = 0;
    ROHC_ENTER_FN("rohc_make_p1_dyn_chain");   
    if(p_csc->p_cpib->outer_ip4)
    {
        if(ROHC_FAILURE == rohc_make_p1_ip4_dyn(
                            (struct rohc_ip4_dyn_t *)p_location,
                            p_csc,ROHC_OUTERIP,p_entity,p_len,p_ecode))
        {
            return ROHC_FAILURE;
        }
    }
    else if(p_csc->p_cpib->outer_ip6)
    {
        if(ROHC_FAILURE == rohc_make_p1_ip6_dyn(
                        (struct rohc_ip6_dyn_t *) p_location,
                        p_csc,ROHC_OUTERIP,p_entity,p_len,p_ecode))
        {
            return ROHC_FAILURE;
        }
    }

    if(p_csc->p_cpib->inner_ip4)
    {
        if(ROHC_FAILURE == rohc_make_p1_ip4_dyn(
                        (struct rohc_ip4_dyn_t *)(p_location+*p_len),
                        p_csc,ROHC_INNERIP,p_entity,&len,p_ecode))
        {
            return ROHC_FAILURE;
        }
        *p_len += len;
    }
    else if(p_csc->p_cpib->inner_ip6)
    {
        if(ROHC_FAILURE == rohc_make_p1_ip6_dyn(
                    (struct rohc_ip6_dyn_t *)(p_location+*p_len),
                    p_csc,ROHC_INNERIP,p_entity,&len,p_ecode))
        {
            return ROHC_FAILURE;
        }
        *p_len += len;
    }
    rohc_make_p1_udp_dyn((p_location+*p_len),p_csc);
    *p_len += sizeof(rohc_udp_dyn_t);
           
    /*
    ** START_SPR_567_FIX : Corrected the creation of RTP IR-DYN packets.
    */
 
    if(ROHC_FAILURE == rohc_make_p1_rtp_dyn((p_location+*p_len),
            p_csc,p_entity,&len,p_ecode))

    /* END_SPR_567_FIX */
    {
        return ROHC_FAILURE;
    }
    *p_len += len;
    ROHC_EXIT_FN("rohc_make_p1_dyn_chain");
    return ROHC_SUCCESS;
} /* end function rohc_make_p1_dyn_chain */
/*****************************************************************************
**      FUNCTION:
**              rohc_make_p2_dyn_chain
******************************************************************************
**
**      DESCRIPTION:
**          Compose dynamic buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to CSC     
**          Pointer to entity     
**      RETURN VALUE:
**              ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
STATIC rohc_return_t 
rohc_make_p2_dyn_chain
    ROHC_PARAMS((p_location,p_csc,p_entity,p_len,p_ecode),
    rohc_U8bit_t            * p_location _AND_  
    struct rohc_csc_udp_t   * p_csc      _AND_
    struct rohc_entity_t    * p_entity   _AND_
    rohc_U32bit_t           *p_len       _AND_
    rohc_error_code_t       *p_ecode)
{
    rohc_U32bit_t len = 0;
    *p_len = 0;
    ROHC_ENTER_FN("rohc_make_p2_dyn_chain");   
    if(p_csc->p_cpib->outer_ip4)
    {
        if(ROHC_FAILURE == rohc_make_p2_ip4_dyn(
                            (struct rohc_ip4_dyn_t *)p_location,
                            p_csc,ROHC_OUTERIP,p_entity,p_len,p_ecode))
        {
            return ROHC_FAILURE;
        }
    }
    else if(p_csc->p_cpib->outer_ip6)
    {
        if(ROHC_FAILURE == rohc_make_p2_ip6_dyn(
                        (struct rohc_ip6_dyn_t *) p_location,
                        p_csc,ROHC_OUTERIP,p_entity,p_len,p_ecode))
        {
            return ROHC_FAILURE;
        }
    }

    if(p_csc->p_cpib->inner_ip4)
    {
        if(ROHC_FAILURE == rohc_make_p2_ip4_dyn(
                        (struct rohc_ip4_dyn_t *)(p_location+*p_len),
                        p_csc,ROHC_INNERIP,p_entity,&len,p_ecode))
        {
            return ROHC_FAILURE;
        }
        *p_len += len;
    }
    else if(p_csc->p_cpib->inner_ip6)
    {
        if(ROHC_FAILURE == rohc_make_p2_ip6_dyn(
                    (struct rohc_ip6_dyn_t *)(p_location+*p_len),
                    p_csc,ROHC_INNERIP,p_entity,&len,p_ecode))
        {
            return ROHC_FAILURE;
        }
        *p_len += len;
    }

    rohc_make_p2_udp_dyn((p_location+*p_len),p_csc);
            
    *p_len+=sizeof(rohc_udp_dyn_t);
    ROHC_SET_2_OCTS((p_location + *p_len),p_csc->field_value[
        ROHC_SN_FID-ROHC_VAR_FID_BASE]);
    *p_len+=2;

    ROHC_EXIT_FN("rohc_make_p2_dyn_chain");
    return ROHC_SUCCESS;
} /* end function rohc_make_p2_dyn_chain */

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p6_dyn_chain
******************************************************************************
**
**      DESCRIPTION:
**          Compose dynamic buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to CSC     
**          Pointer to entity     
**      RETURN VALUE:
**              ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
static rohc_return_t 
rohc_make_p6_dyn_chain
    ROHC_PARAMS((p_location,p_csc,p_entity,p_len,p_ecode),
    rohc_U8bit_t            * p_location    _AND_  
    struct rohc_csc_tcp_t   * p_csc         _AND_
    struct rohc_entity_t    * p_entity      _AND_
    rohc_U32bit_t           *p_len          _AND_
    rohc_error_code_t       *p_ecode)
{
    rohc_U32bit_t len = 0;
    *p_len = 0;
    ROHC_ENTER_FN("rohc_make_p6_dyn_chain");   
    if(p_csc->p_cpib->outer_ip4)
    {
        if(ROHC_FAILURE == rohc_make_p6_ip4_dyn(
                    p_location,
                    p_csc,ROHC_OUTERIP,p_len))
            /* +- SPR 17777 */
        {
            return ROHC_FAILURE;
        }
    }
    else if(p_csc->p_cpib->outer_ip6)
    {
        /* if(ROHC_FAILURE == rohc_make_p6_ip6_dyn(
           (struct rohc_ip6_dyn_t *) p_location,
           p_csc,ROHC_OUTERIP,p_entity,p_len))
           {
           return ROHC_FAILURE;
           }*/
    }

    if(p_csc->p_cpib->inner_ip4)
    {
        if(ROHC_FAILURE == rohc_make_p6_ip4_dyn(
                    (p_location+*p_len),
                    p_csc,ROHC_INNERIP,&len))
            /* +- SPR 17777 */
        {
            return ROHC_FAILURE;
        }
        *p_len += len;
    }
    else if(p_csc->p_cpib->inner_ip6)
    {
        /* if(ROHC_FAILURE == rohc_make_p1_ip6_dyn(
           (struct rohc_ip6_dyn_t *)(p_location+*p_len),
           p_csc,ROHC_INNERIP,p_entity,&len))
           {
           return ROHC_FAILURE;
           }
         *p_len += len;*/
    }

    if(ROHC_FAILURE == rohc_make_p6_tcp_dyn(
                (struct rohc_tcp_dyn_partial_t *)(p_location+*p_len),
                p_csc,&len))
        /* +- SPR 17777 */

    {
        return ROHC_FAILURE;
    }
    *p_len += len;
    /* If ACK_ZERO field is set means ack no is 0, so no need to send it */
    if(!(p_csc->field_value[ROHC_TCP_ACK_ZERO_FID-ROHC_VAR_FID_BASE]))
    {
        /* ack no is sent*/
        rohc_make_p6_tcp_dyn_ack((p_location+*p_len),p_csc);
        *p_len += sizeof(rohc_tcp_dyn_acknum_t);
    }

    rohc_make_p6_tcp_dyn_wind_cksum(
            (struct rohc_tcp_dyn_windncksum_t *)(p_location+*p_len),
            p_csc);
    *p_len += sizeof(rohc_tcp_dyn_windncksum_t);
    /* If URP_ZERO field is set means value of urgptr is 0, so no need to
       send it */
    if(!(p_csc->field_value[ROHC_TCP_URP_ZERO_FID-ROHC_VAR_FID_BASE]))
    {
        /* urgent pointer is sent*/
        rohc_make_p6_tcp_dyn_urgptr((p_location+*p_len),p_csc);
        *p_len += sizeof(rohc_tcp_dyn_urgptr_t);
    }
    /* ack stride is sent only if ack_stride flag is set*/
    if(p_csc->field_value[ROHC_TCP_ACK_STRIDE_FLAG_FID-ROHC_VAR_FID_BASE])
    {
        /* ack stride is sent*/
        rohc_make_p6_tcp_dyn_ackstride((p_location+*p_len),p_csc);
        *p_len += sizeof(rohc_tcp_dyn_ackstride_t);
    }
    if(ROHC_GET_UPPER_NIBBLE(((rohc_tcp_t*)p_csc->p_cpib->tcp)->offset) >5)
    {
        /* tcp options are present */

        if(ROHC_FAILURE == rohc_list_encoding_p6(p_entity,
                    p_csc,&len,
                    (rohc_U8bit_t *)p_location+*p_len,
                    p_ecode))
        {
            return ROHC_FAILURE;
        }
        else
        {
            *p_len += len;
        }
    }
    else
    {
        *(p_location + *p_len) = 0;
        (*p_len)++;
        /* set presence of TS opt false for current pkt */
        p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].
            ts_opt_recvd = ROHC_FALSE;
    }
    ROHC_EXIT_FN("rohc_make_p6_dyn_chain");
    return ROHC_SUCCESS;
} /* end function rohc_make_p6_dyn_chain */

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p1_static_chain
******************************************************************************
**
**      DESCRIPTION:
**          Compose static buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to CSC     
**          Pointer to entity     
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
/* +- SPR 17777 */
STATIC rohc_U32bit_t 
rohc_make_p1_static_chain
    ROHC_PARAMS((p_location,p_csc),
    rohc_U8bit_t            * p_location _AND_  
    struct rohc_csc_rtp_t   * p_csc)
{
    rohc_U8bit_t *p_orig = p_location;
    rohc_U8bit_t temp_proto =
        (p_csc->p_cpib->inner_ip4)?ROHC_IPPROTO_IP4:ROHC_IPPROTO_IP6;
    ROHC_ENTER_FN("rohc_make_p1_static_chain");
    if(p_csc->p_cpib->outer_ip4)
    {
        rohc_make_p1_ip4_static((rohc_ip4_static_t*)p_location,
                                    p_csc,temp_proto);
        p_location += sizeof(rohc_ip4_static_t);                                
    }
    else if(p_csc->p_cpib->outer_ip6 )
    {
        rohc_make_p1_ip6_static((rohc_ip6_static_t*)p_location,
                            p_csc,temp_proto);
        p_location += sizeof(rohc_ip6_static_t);
    }
    
    temp_proto = 
        (p_csc->profile == ROHC_IP_ESP)?ROHC_IPPROTO_ESP:ROHC_IPPROTO_UDP;

    if(p_csc->p_cpib->inner_ip4)
    {
        rohc_make_p1_ip4_static((rohc_ip4_static_t*)p_location,
                                p_csc,temp_proto);
        p_location +=sizeof(rohc_ip4_static_t);
    }
    else if(p_csc->p_cpib->inner_ip6)
    {
        rohc_make_p1_ip6_static((rohc_ip6_static_t*)p_location,
                            p_csc,temp_proto);
        p_location +=sizeof(rohc_ip6_static_t);
    }
    rohc_make_p1_udp_static((rohc_udp_static_t*)p_location,p_csc);
    p_location +=sizeof(rohc_udp_static_t);
    rohc_make_p1_rtp_static(p_location,p_csc);
    p_location+=sizeof(rohc_rtp_static_t);
    ROHC_EXIT_FN("rohc_make_p1_static_chain");
    return (p_location-p_orig);
} /* end function rohc_make_static_chain */

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p2_static_chain
******************************************************************************
**
**      DESCRIPTION:
**          Compose static buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to CSC     
**          Pointer to entity     
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
    /* +- SPR 17777 */
STATIC rohc_U32bit_t 
rohc_make_p2_static_chain
    ROHC_PARAMS((p_location,p_csc),
    rohc_U8bit_t            * p_location _AND_  
    struct rohc_csc_udp_t   * p_csc)
{
    rohc_U8bit_t *p_orig = p_location;
    rohc_U8bit_t temp_proto =
        (p_csc->p_cpib->inner_ip4)?ROHC_IPPROTO_IP4:ROHC_IPPROTO_IP6;
    ROHC_ENTER_FN("rohc_make_p2_static_chain");
    if(p_csc->p_cpib->outer_ip4)
    {
        rohc_make_p2_ip4_static((rohc_ip4_static_t*)p_location,
                                    p_csc,temp_proto);
        p_location += sizeof(rohc_ip4_static_t);                                
    }
    else if(p_csc->p_cpib->outer_ip6 )
    {
        rohc_make_p2_ip6_static((rohc_ip6_static_t*)p_location,
                            p_csc,temp_proto);
        p_location += sizeof(rohc_ip6_static_t);
    }
    
    temp_proto = 
        (p_csc->profile == ROHC_IP_ESP)?ROHC_IPPROTO_ESP:ROHC_IPPROTO_UDP;

    if(p_csc->p_cpib->inner_ip4)
    {
        rohc_make_p2_ip4_static((rohc_ip4_static_t*)p_location,
                                p_csc,temp_proto);
        p_location +=sizeof(rohc_ip4_static_t);
    }
    else if(p_csc->p_cpib->inner_ip6)
    {
        rohc_make_p2_ip6_static((rohc_ip6_static_t*)p_location,
                            p_csc,temp_proto);
        p_location +=sizeof(rohc_ip6_static_t);
    }
    rohc_make_p2_udp_static((rohc_udp_static_t*)p_location,p_csc);
    p_location+=sizeof(rohc_udp_static_t);
    ROHC_EXIT_FN("rohc_make_p2_static_chain");
    return (p_location-p_orig);
} /* end function rohc_make_p2_static_chain */

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p6_static_chain
******************************************************************************
**
**      DESCRIPTION:
**          Compose static buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to CSC     
**          Pointer to entity     
**      RETURN VALUE:
**              void
**      NOTES:
**               
*****************************************************************************/
/* +- SPR 17777 */
static rohc_U32bit_t 
rohc_make_p6_static_chain
    ROHC_PARAMS((p_location,p_csc),
    rohc_U8bit_t            * p_location _AND_  
    struct rohc_csc_tcp_t   * p_csc)
{
    rohc_U8bit_t *p_orig = p_location;
    rohc_U8bit_t temp_proto =
        (p_csc->p_cpib->inner_ip4)?ROHC_IPPROTO_IP4:ROHC_IPPROTO_IP6;
    ROHC_ENTER_FN("rohc_make_p6_static_chain");
    if(p_csc->p_cpib->outer_ip4)
    {
        rohc_make_p6_ip4_static((rohc_ip4_static_t*)p_location,
                                    p_csc,temp_proto);
        p_location += sizeof(rohc_ip4_static_t);                                
    }
    else if(p_csc->p_cpib->outer_ip6 )
    {
    }
    
    temp_proto = ROHC_IPPROTO_TCP;/* next header for inner ip */

    if(p_csc->p_cpib->inner_ip4)
    {
        rohc_make_p6_ip4_static((rohc_ip4_static_t*)p_location,
                                p_csc,temp_proto);
        p_location +=sizeof(rohc_ip4_static_t);
    }
    else if(p_csc->p_cpib->inner_ip6)
    {
    }
    rohc_make_p6_tcp_static((rohc_tcp_static_t*)p_location,p_csc);
    p_location +=sizeof(rohc_tcp_static_t);
    ROHC_EXIT_FN("rohc_make_p6_static_chain");
    return (p_location-p_orig);
} /* end function rohc_make_static_chain */
/*****************************************************************************
**      FUNCTION:
**              rohc_make_uncomp_IRP0_pkt_typ
******************************************************************************
**
**      DESCRIPTION:
**          Compose IRP0 or Un comp buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to CSC     
**          Packet type     
**          Pointer to Rohc entity     
**          Pointer to CID Length     
**          Pointer to Packet Length
**			Pointer to Input Buffer
**      RETURN VALUE:
**              ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
STATIC rohc_return_t 
rohc_make_uncomp_IRP0_pkt_typ
    ROHC_PARAMS((p_location,p_csc,pkt_type,p_entity,cid_len,p_len,p_in_buff),
    rohc_U8bit_t               * p_location _AND_  
    struct rohc_csc_uncomp_t   * p_csc      _AND_
    rohc_U8bit_t               pkt_type     _AND_
    struct rohc_entity_t       * p_entity   _AND_
    rohc_U8bit_t               *cid_len     _AND_
    rohc_U32bit_t              *p_len       _AND_
    rohc_U8bit_t               *p_in_buff)
{
    rohc_U8bit_t * p_org=p_location;
    ROHC_ENTER_FN("rohc_make_uncomp_IRP0_pkt_typ");
    if(pkt_type == ROHC_IRP0_PKTYPE)
    {
        *p_location++=0xfc;
    }
    else
    {
        *p_location++ = *p_in_buff;
    }

    if( p_entity->large_cids == ROHC_TRUE )
    {
        *cid_len = rohc_c_encode_sdvl(p_location,
        p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE]);
        p_location += *cid_len;
    }

    if(pkt_type == ROHC_IRP0_PKTYPE)
    {
        *p_location++=p_csc->profile;
        *p_location++=0;
    }
    *p_len = (p_location-p_org);
    
    ROHC_EXIT_FN("rohc_make_uncomp_IRP0_pkt_typ");
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p1_IR_pkts
******************************************************************************
**
**      DESCRIPTION:
**          Compose IR buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to CSC     
**          Packet type     
**          Pointer to Rohc entity     
**          Pointer to CID Length     
**          Pointer to Packet Length
**          Pointer to entity     
**      RETURN VALUE:
**              ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
STATIC rohc_return_t 
rohc_make_p1_IR_pkts
    ROHC_PARAMS((p_location,p_csc,pkt_type,p_entity,cid_len,p_len,p_ecode),
    rohc_U8bit_t            * p_location _AND_  
    struct rohc_csc_rtp_t   * p_csc      _AND_
    rohc_U8bit_t            pkt_type     _AND_
    struct rohc_entity_t    * p_entity   _AND_
    rohc_U8bit_t            *cid_len     _AND_
    rohc_U32bit_t           *p_len       _AND_
    rohc_error_code_t       *p_ecode)
{
    rohc_U8bit_t * p_org=p_location;
    ROHC_ENTER_FN("rohc_make_p1_IR_pkts");
    *p_location++=0xfc;
    if( p_entity->large_cids == ROHC_TRUE )
    {
        *cid_len = rohc_c_encode_sdvl(p_location,
            p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE]);
        p_location += *cid_len;
    }
    *p_location++=p_csc->profile;
    *p_location++=0;
    if(pkt_type != ROHC_IRDYN_PKTTYPE)
    {
        *p_len = rohc_make_p1_static_chain(p_location,p_csc);
        /* +- SPR 17777 */
        p_location += *p_len;
    }
    if( pkt_type == ROHC_IRDYN_PKTTYPE  ||
        pkt_type == ROHC_IR_WITH_DYN_PKTTYPE)
    {
        if( rohc_make_p1_dyn_chain(p_location,p_csc,p_entity,p_len,p_ecode) 
                                                    == ROHC_FAILURE)
        {
            return ROHC_FAILURE;
        }
        if(pkt_type == ROHC_IRDYN_PKTTYPE )
            *p_org = 0xf8;
        else
            *p_org |= 0x01;
        p_location += *p_len;
    }
    *p_len = (p_location-p_org);
    ROHC_EXIT_FN("rohc_make_p1_IR_pkts");
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p2_IR_pkts
******************************************************************************
**
**      DESCRIPTION:
**          Compose IR buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to CSC     
**          Packet type     
**          Pointer to entity     
**      RETURN VALUE:
**              ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
STATIC rohc_return_t 
rohc_make_p2_IR_pkts
    ROHC_PARAMS((p_location,p_csc,pkt_type,p_entity,cid_len,p_len,p_ecode),
    rohc_U8bit_t            * p_location _AND_  
    struct rohc_csc_udp_t   * p_csc      _AND_
    rohc_U8bit_t            pkt_type     _AND_
    struct rohc_entity_t    * p_entity   _AND_
    rohc_U8bit_t            *cid_len     _AND_
    rohc_U32bit_t           *p_len       _AND_
    rohc_error_code_t       *p_ecode)
{
    rohc_U8bit_t * p_org=p_location;
    ROHC_ENTER_FN("rohc_make_p2_IR_pkts");
    *p_location++=0xfc;
    if( p_entity->large_cids == ROHC_TRUE )
    {
        *cid_len = rohc_c_encode_sdvl(p_location,
            p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE]);
        p_location += *cid_len;
    }
    *p_location++=p_csc->profile;
    *p_location++=0;
    if(pkt_type != ROHC_IRDYN_PKTTYPE)
    {
        *p_len = rohc_make_p2_static_chain(p_location,p_csc);
        /* +- SPR 17777 */
        p_location += *p_len;
    }
    if( pkt_type == ROHC_IRDYN_PKTTYPE  ||
        pkt_type == ROHC_IR_WITH_DYN_PKTTYPE)
    {
        if( rohc_make_p2_dyn_chain(p_location,p_csc,p_entity,p_len,p_ecode) 
                                                    == ROHC_FAILURE)
        {
            return ROHC_FAILURE;
        }
        if(pkt_type == ROHC_IRDYN_PKTTYPE )
            *p_org = 0xf8;
        else
            *p_org |= 0x01;
        p_location += *p_len;
    }
    *p_len = (p_location-p_org);
    ROHC_EXIT_FN("rohc_make_p2_IR_pkts");
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p6_IR_pkts
******************************************************************************
**
**      DESCRIPTION:
**          Compose IR buffer 
**      ARGUMENTS:
**          Pointer to ROHC buffer     
**          Pointer to CSC     
**          Packet type     
**          Pointer to Rohc entity     
**          Pointer to CID Length     
**          Pointer to Packet Length
**			Pointer to Error Code
**      RETURN VALUE:
**              ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
static rohc_return_t 
rohc_make_p6_IR_pkts
    ROHC_PARAMS((p_location,p_csc,pkt_type,p_entity,cid_len,p_len,p_ecode),
    rohc_U8bit_t            * p_location _AND_  
    struct rohc_csc_tcp_t   * p_csc      _AND_
    rohc_U8bit_t            pkt_type     _AND_
    struct rohc_entity_t    * p_entity   _AND_
    rohc_U8bit_t            *cid_len     _AND_
    rohc_U32bit_t           *p_len       _AND_
    rohc_error_code_t       *p_ecode)
{
    rohc_U8bit_t * p_org=p_location;
    ROHC_ENTER_FN("rohc_make_p6_IR_pkts");
    if(ROHC_IR_WITH_DYN_PKTTYPE == pkt_type)
    {
        *p_location++=0xfd;
    }
    else if(ROHC_IRDYN_PKTTYPE == pkt_type)
    {
        *p_location++=0xf8;
    }
    if( ROHC_TRUE == p_entity->large_cids )
    {
        *cid_len = rohc_c_encode_sdvl(p_location,
            p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE]);
        p_location += *cid_len;
    }
    *p_location++=p_csc->profile;
    *p_location++=0;
    if(pkt_type != ROHC_IRDYN_PKTTYPE)
    {
        *p_len = rohc_make_p6_static_chain(p_location,p_csc);
        /* +- SPR 17777 */
        p_location += *p_len;
    }
    if((ROHC_IRDYN_PKTTYPE == pkt_type)  || 
            (ROHC_IR_WITH_DYN_PKTTYPE == pkt_type))
    {
        if( rohc_make_p6_dyn_chain(p_location,p_csc,p_entity,p_len,p_ecode) 
                                                    == ROHC_FAILURE)
        {
            return ROHC_FAILURE;
        }
        p_location += *p_len;
    }
    *p_len = (p_location-p_org);
    ROHC_EXIT_FN("rohc_make_p6_IR_pkts");
    return ROHC_SUCCESS;
}
/****************************************************************/
/*Generic Packet Encoding Routine for compressed packets ********/
/*****************************************************************************
**      FUNCTION:
**              rohc_make_p0_rohc_compr_pkt
******************************************************************************
**
**      DESCRIPTION:
**          Compose ROHC Packet 
**      ARGUMENTS:
**          Pointer to CSC     
**          Pointer to ROHC Packet      
**          Packet type     
**          Pointer to Packet length     
**          Pointer to ROHC entity     
**          Pointer to global context
**			Pointer to Error Code 
**      RETURN VALUE:
**          ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
        /* +- SPR 17777 */
rohc_return_t  
rohc_make_p0_rohc_compr_pkt
    ROHC_PARAMS((p_entity,p_csc,pkt_type,p_in_buff,p_pkt_len,p_rohc_pkt,p_len_skip_from_pl),
    struct rohc_entity_t       * p_entity _AND_
    struct rohc_csc_uncomp_t   * p_csc _AND_
    rohc_U8bit_t        pkt_type _AND_
    rohc_U8bit_t        * p_in_buff _AND_
    rohc_U32bit_t       * p_pkt_len _AND_
    rohc_U8bit_t        * p_rohc_pkt _AND_
    rohc_U8bit_t        * p_len_skip_from_pl)
{
    rohc_U32bit_t len=0;
    rohc_U8bit_t cid_len=0;
    rohc_U8bit_t    * p_rohc_org_pkt=p_rohc_pkt;
    ROHC_ENTER_FN("rohc_make_p0_rohc_compr_pkt");
    *p_len_skip_from_pl = 0;

    if(p_entity->large_cids == ROHC_FALSE &&
        p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE])
    {
        *p_rohc_pkt++=0xE0|p_csc->field_value[ROHC_CID_FID- 
                ROHC_VAR_FID_BASE];
        *p_pkt_len = 1;
    }/* Small CID is being used*/
    else
        *p_pkt_len = 0;

    /* Uncompressed Packet */
    if(ROHC_FAILURE == rohc_make_uncomp_IRP0_pkt_typ(p_rohc_pkt,p_csc,
                        pkt_type,p_entity,&cid_len,&len,p_in_buff))
    {
        return ROHC_FAILURE;
    }
    *p_pkt_len += len;
    if(pkt_type == ROHC_IRP0_PKTYPE )
    {
        p_rohc_pkt[2+cid_len]=
            rohc_compute_crc(ROHC_CRC_TYPE_8,p_rohc_org_pkt,*p_pkt_len -1,0xff); 
    }
    else
    {
        *p_len_skip_from_pl = 1;
    }

    /* Incrementing Stats */
    /*Klocwork Fix */
    if(pkt_type < ROHC_MAX_PKT_TYPES)
    {
        rohcStatsPktTypeUpdate(rohc_global_pkt_types_to_stats_cr[pkt_type],ROHC_DIR_TX,
                ROHC_CSC_GET_PROFILE(p_csc));
    }

    ROHC_LOG(LOG_INFO,"CPBM :EID[%u:%u]:CID[%u]: packet "\
                    " type [%d] is created ",
                    p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                    p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE],
                    pkt_type);
    ROHC_EXIT_FN("rohc_make_p0_rohc_compr_pkt");
    return ROHC_SUCCESS;
}/* end function rohc_encode_packet*/

/****************************************************************/
/*Generic Packet Encoding Routine for compressed packets ********/
/*****************************************************************************
**      FUNCTION:
**              rohc_make_p1_rohc_compr_pkt
******************************************************************************
**
**      DESCRIPTION:
**          Compose ROHC Packet 
**      ARGUMENTS:
**          Pointer to CSC     
**          Pointer to ROHC Packet      
**          Packet type     
**          Pointer to Packet length     
**          Pointer to ROHC entity     
**          Pointer to global context
**			Pointer to Error Code
**      RETURN VALUE:
**          ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
rohc_return_t  
rohc_make_p1_rohc_compr_pkt
    ROHC_PARAMS((p_entity,p_csc,pkt_type,p_in_buff,p_pkt_len,p_rohc_pkt,p_len_skip_from_pl,p_ecode),
    struct rohc_entity_t    * p_entity              _AND_
    struct rohc_csc_rtp_t   * p_csc                 _AND_
    rohc_U8bit_t            pkt_type                _AND_
    rohc_U8bit_t            * p_in_buff             _AND_
    rohc_U32bit_t           * p_pkt_len             _AND_
    rohc_U8bit_t            * p_rohc_pkt            _AND_
    rohc_U8bit_t            * p_len_skip_from_pl    _AND_
    rohc_error_code_t       * p_ecode)
{
    rohc_U32bit_t   len=0;
    rohc_U8bit_t    cid_len=0;
    rohc_U8bit_t    * p_rohc_org_pkt=p_rohc_pkt;
    rohc_return_t    returnVal = ROHC_SUCCESS;
    ROHC_ENTER_FN("rohc_make_p1_rohc_compr_pkt");
    *p_len_skip_from_pl = 0;

    if(p_entity->large_cids == ROHC_FALSE &&
        p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE])
    {
        *p_rohc_pkt++=0xE0|p_csc->field_value[ROHC_CID_FID- 
                ROHC_VAR_FID_BASE];
        *p_pkt_len = 1;
    }/* Small CID is being used*/
    else
        *p_pkt_len = 0;

    if(pkt_type >= ROHC_IR_WITHOUT_DYN_PKTTYPE  )
    {   /* IR & IR DYN pkts for p1, p2 & p3 */
        if(ROHC_FAILURE == rohc_make_p1_IR_pkts(p_rohc_pkt,p_csc,
                            pkt_type,p_entity,&cid_len,&len,p_ecode))
        {
            return ROHC_FAILURE;
        }
        *p_pkt_len += len;
        p_rohc_pkt[2+cid_len]=
            rohc_compute_crc(ROHC_CRC_TYPE_8,p_rohc_org_pkt,*p_pkt_len,0xff); 

       /* Incrementing Stats */
       rohcStatsPktTypeUpdate(rohc_global_pkt_types_to_stats_cr[pkt_type],ROHC_DIR_TX,
               ROHC_CSC_GET_PROFILE(p_csc));

       ROHC_LOG(LOG_INFO,"CPBM :EID[%u:%u]:CID[%u]: packet "\
                    " type [%d] is created ",
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,
                    p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE],
                    pkt_type);

        return ROHC_SUCCESS;
    }
    else if( pkt_type >= ROHC_IRP0_PKTYPE )
    {   /* Uncompressed Packet */
        /* + SPR 17439 */
        if(ROHC_FAILURE == rohc_make_uncomp_IRP0_pkt_typ(p_rohc_pkt,(rohc_csc_uncomp_t *)p_csc,
                            pkt_type,p_entity,&cid_len,&len,p_in_buff))
        /* - SPR 17439 */
        {
            return ROHC_FAILURE;
        }
        *p_pkt_len += len;
        if(pkt_type == ROHC_IRP0_PKTYPE )
        {
            p_rohc_pkt[2+cid_len]=
              rohc_compute_crc(ROHC_CRC_TYPE_8,p_rohc_org_pkt,*p_pkt_len -1,0xff); 
        }
        else
        {
            *p_len_skip_from_pl = 1;
        }

       /* Incrementing Stats */
       rohcStatsPktTypeUpdate(rohc_global_pkt_types_to_stats_cr[pkt_type],ROHC_DIR_TX,
               ROHC_CSC_GET_PROFILE(p_csc));

        ROHC_LOG(LOG_INFO,"CPBM :EID[%u:%u]:CID[%u]: packet "\
                    " type [%d] is created ",
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,
                    p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE],
                    pkt_type);
        return ROHC_SUCCESS;
    }

    returnVal = rohc_p1_pbm_fill_pkt_bits(p_entity, p_csc, 
                                   &p_rohc_pkt, pkt_type, p_ecode);
    if (ROHC_FAILURE == returnVal)
    {
        return ROHC_FAILURE;
    }

    if(p_csc->p_cpib->outer_ip4)
    {
        /*
        ** START_SPR_1040_FIX : Corrected the handling of the IP-ID field while
        ** making the compressor packet. This code is not tested as of now and 
        ** shall be tested when IP tunneling is supported in ROHC library.
        */
        ROHC_SET_16BIT(p_rohc_pkt,p_csc->p_cpib->temp_ipid2);
        /* END_SPR_1040_FIX */

        p_rohc_pkt+=2;
    }
    if( p_csc->outer_ah_data_len )
    {
        ROHC_MEMMOVE(p_rohc_pkt,p_csc->p_outer_ah_data,
                                        p_csc->outer_ah_data_len);
        p_rohc_pkt+= p_csc->outer_ah_data_len;
    }
    if(p_csc->outer_gre_chksum_present)
    {
        ROHC_SET_2_OCTS(p_rohc_pkt,p_csc->outer_gre_chksum);
        p_rohc_pkt+=2;
    }

    if(p_csc->p_cpib->inner_ip4)
    {
        /*
        ** START_SPR_1040_FIX : Corrected the handling of the IP-ID field while
        ** making the compressor packet. 
        */
        ROHC_SET_16BIT(p_rohc_pkt,p_csc->p_cpib->temp_ipid);
        /* END_SPR_1040_FIX */

        p_rohc_pkt+=2;
    }
    if( p_csc->inner_ah_data_len )
    {
        ROHC_MEMMOVE(p_rohc_pkt,p_csc->p_inner_ah_data,
                                    p_csc->inner_ah_data_len);
        p_rohc_pkt+= p_csc->inner_ah_data_len;
    }
    if(p_csc->inner_gre_chksum_present)
    {
        ROHC_SET_2_OCTS(p_rohc_pkt,p_csc->inner_gre_chksum);
        p_rohc_pkt+=2;
    }
    if(p_csc->static_delta & ROHC_UDP_CHECK_SUM_PRESENT)
    {
        ROHC_SET_2_OCTS(p_rohc_pkt,p_csc->field_value[ROHC_UDP_CKSUM_FID
                    - ROHC_VAR_FID_BASE]);
        p_rohc_pkt+=2;
    }

    *p_pkt_len=p_rohc_pkt-p_rohc_org_pkt;

    /* Incrementing Stats */
    rohcStatsPktTypeUpdate(rohc_global_pkt_types_to_stats_cr[pkt_type],ROHC_DIR_TX,
            ROHC_CSC_GET_PROFILE(p_csc));

 ROHC_LOG(LOG_INFO,"CPBM :EID[%u:%u]:CID[%u]: packet "\
                    " type [%d] is created ",
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,
                    p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE],
                    pkt_type);
    ROHC_EXIT_FN("rohc_make_p1_rohc_compr_pkt");
    return ROHC_SUCCESS;
}/* end function rohc_encode_p1_packet*/


/*****************************************************************************
**      FUNCTION:
**              rohc_p1_pbm_fill_pkt_bits
******************************************************************************
**
**      DESCRIPTION:
**          Compose ROHC Packet 
**      ARGUMENTS:
**          Pointer to ROHC entity     
**          Pointer to CSC     
**          Pointer to ROHC Packet
**          Packet type     
**			Pointer to Error Code      
**      RETURN VALUE:
**          ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
STATIC rohc_return_t  
rohc_p1_pbm_fill_pkt_bits
    ROHC_PARAMS((p_entity,p_csc,p_rohc_pkt,pkt_type,p_ecode),
    struct rohc_entity_t    * p_entity              _AND_
    struct rohc_csc_rtp_t   * p_csc                 _AND_
    rohc_U8bit_t            ** p_rohc_pkt            _AND_
    rohc_U8bit_t              pkt_type                _AND_
    rohc_error_code_t       * p_ecode)
{
    rohc_U32bit_t   i=0;
    rohc_U8bit_t    no_of_bits=0;
    rohc_U8bit_t    field_id;   
    rohc_U8bit_t    to_bit_pos=0;
    rohc_U8bit_t    *p_ext_flag = NULL;

    for(i=0;i<ROHC_MAX_NO_OF_FIELDS_IN_A_PACKET &&
            rohc_c_pkt_type[pkt_type][i].field_id!=ROHC_INVALID_FID;i++)
    {
        no_of_bits=rohc_c_pkt_type[pkt_type][i].no_of_bits;
        field_id=rohc_c_pkt_type[pkt_type][i].field_id;


        if (field_id == ROHC_EXT3_FLAGS_FID)
        {
            p_ext_flag = *p_rohc_pkt;    
        }
        if((field_id == ROHC_INNERIP_ID_FID) && 
                (p_csc->new_inner_rnd == ROHC_RND_PRESENT) &&
                (p_csc->new_outer_rnd == ROHC_RND_ABSENT) )
        {
            field_id = ROHC_OUTERIP_ID_FID;
        }

        if(no_of_bits<=ROHC_ENC_MAX_FIXED_BITS_IN_FIELD)
        {   /* fixed length encoding */
            /* The check should not be <= but < as otherwise ABR can occur */
            if(field_id < ROHC_MAX_CONSTANT_FID)
            {
                rohc_setbits(p_rohc_pkt,&to_bit_pos,
                        rohc_global_constant_field_values[field_id],
                        0,no_of_bits);
            }   /* end if constant width encoding for field 
                   types with globally constant values*/
            else if(field_id >= ROHC_VAR_FID_BASE)
            {   /*Field Values dependent on stream context */
                rohc_setbits(p_rohc_pkt,&to_bit_pos,
                        p_csc->field_value[field_id-ROHC_VAR_FID_BASE],
                        0,no_of_bits);
            }
            else 
            {
                ROHC_LOG(LOG_INFO,"Field_id shall not be b/w 26-49 ");
                return ROHC_FAILURE;
            }

        }/* end if fixed no of bits encoding */
        else if( no_of_bits == ROHC_ENCTYPE_VAR_NO_BITS)
        {
            if((field_id != ROHC_CID_FID)||
                    (p_entity->large_cids == ROHC_TRUE))
            {
                *p_rohc_pkt += rohc_c_encode_sdvl(*p_rohc_pkt,
                        p_csc->field_value[field_id-ROHC_VAR_FID_BASE]);
            }
        }/* end if variable length encoding like large context-id */
        else if(no_of_bits == ROHC_ENCTYPE_EXT3FLAGSP1)
        {
            /* SPR 4808 Fix Start */
            if(NULL != p_ext_flag)
            {
                rohc_add_ele_p1_ext3flagsp1(p_rohc_pkt,p_ext_flag,
                        &to_bit_pos,field_id,p_csc, pkt_type );
            }
            else
            {  
                ROHC_LOG(LOG_INFO," Rohc Encoding Failed for Large EXT3 Length\n");
                return ROHC_FAILURE;
            }
            /* SPR 4808 Fix End */
        }   /* end if no_of_bits == EXT 3 flags */
        else if(no_of_bits == ROHC_ENCTYPE_INNERIPFLAGSP1)
        {
            if(ROHC_FAILURE == rohc_add_ele_p1_inneripflagsp1(p_rohc_pkt,
                        &to_bit_pos,field_id,p_csc,p_entity,p_ecode))
                return ROHC_FAILURE;
        }   /* end if no_of_bits == ROHC_ENCTYPE_INNERIPFLAGSP1*/
        else if(no_of_bits == ROHC_ENCTYPE_OUTERIPFLAGSP1)
        {
            if(ROHC_FAILURE == rohc_add_ele_p1_outeripflagsp1(p_rohc_pkt,
                        &to_bit_pos,field_id,p_csc,p_entity,p_ecode))
                return ROHC_FAILURE;
        }/* end if no_of_bits == ROHC_ENCTYPE_OUTERIPFLAGSP1*/
        else if(no_of_bits == ROHC_ENCTYPE_RTPFLAGSP1)
        {
            if(ROHC_FAILURE == rohc_add_ele_p1_rtpflagsp1(p_rohc_pkt,
                        &to_bit_pos,field_id,p_csc,p_entity,p_ecode))
                return ROHC_FAILURE;
        }/*end if no_of_bits == ROHC_ENCTYPE_RTPFLAGSP1 */
        else if(no_of_bits == ROHC_ENCTYPE_SPLIT_FIELD)
        { /* split field always in base headers */
            rohc_U32bit_t bit_offset;
            rohc_U32bit_t   len;
            /*          cur_occ=rohc_c_pkt_type[pkt_type][i].cur_occ; */
            len=rohc_c_pkt_type[pkt_type][i].no_of_bits_occ;
            bit_offset=p_csc->field_len[field_id-
                ROHC_VAR_FID_BASE]-len;
            rohc_setbits(p_rohc_pkt,&to_bit_pos,
                    p_csc->field_value[field_id-ROHC_VAR_FID_BASE],
                    (rohc_U8bit_t)bit_offset,(rohc_U8bit_t)len);
            p_csc->field_len[field_id-ROHC_VAR_FID_BASE]-=len;
        }/*end if split field found*/
    }/* end for traversal field type in packet type array */
    return ROHC_SUCCESS;
}

/****************************************************************/
/*Generic Packet Encoding Routine for compressed packets ********/
/*****************************************************************************
**      FUNCTION:
**              rohc_make_p2_rohc_compr_pkt
******************************************************************************
**
**      DESCRIPTION:
**          Compose ROHC Packet 
**      ARGUMENTS:
**          Pointer to CSC     
**          Pointer to ROHC Packet      
**          Packet type     
**          Pointer to Packet length     
**          Pointer to ROHC entity     
**          Pointer to global context
**			Pointer to Error Code
**      RETURN VALUE:
**          ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
rohc_return_t  
rohc_make_p2_rohc_compr_pkt
    ROHC_PARAMS((p_entity,p_csc,pkt_type,p_in_buff,p_pkt_len,p_rohc_pkt,p_len_skip_from_pl,p_ecode),
    struct rohc_entity_t  * p_entity           _AND_
    struct rohc_csc_udp_t * p_csc              _AND_
    rohc_U8bit_t          pkt_type             _AND_
    rohc_U8bit_t          * p_in_buff          _AND_
    rohc_U32bit_t         * p_pkt_len          _AND_
    rohc_U8bit_t          * p_rohc_pkt         _AND_
    rohc_U8bit_t          * p_len_skip_from_pl _AND_
    rohc_error_code_t     * p_ecode)
{
    rohc_U32bit_t   len=0;
    rohc_U8bit_t    cid_len=0;
    rohc_U8bit_t    * p_rohc_org_pkt=p_rohc_pkt;
    rohc_return_t    returnVal = ROHC_SUCCESS;
    ROHC_ENTER_FN("rohc_make_p2_rohc_compr_pkt");
    *p_len_skip_from_pl = 0;

    if(p_entity->large_cids == ROHC_FALSE &&
        p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE])
    {
        *p_rohc_pkt++=0xE0|p_csc->field_value[ROHC_CID_FID- 
                ROHC_VAR_FID_BASE];
        *p_pkt_len = 1;
    }/* Small CID is being used*/
    else
        *p_pkt_len = 0;

    if(pkt_type >= ROHC_IR_WITHOUT_DYN_PKTTYPE  )
    {   /* IR & IR DYN pkts for p1, p2 & p3 */
        if(ROHC_FAILURE == rohc_make_p2_IR_pkts(p_rohc_pkt,p_csc,
                            pkt_type,p_entity,&cid_len,&len,p_ecode))
        {
            return ROHC_FAILURE;
        }
        *p_pkt_len += len;
        p_rohc_pkt[2+cid_len]=
            rohc_compute_crc(ROHC_CRC_TYPE_8,p_rohc_org_pkt,*p_pkt_len,0xff); 

       /* Incrementing Stats */
       rohcStatsPktTypeUpdate(rohc_global_pkt_types_to_stats_cr[pkt_type],
                                  ROHC_DIR_TX, ROHC_CSC_GET_PROFILE(p_csc));
       ROHC_LOG(LOG_INFO,"CPBM :EID[%u:%u]:CID[%u]: packet "\
                    " type [%d] is created ",
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,
                    p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE],
                    pkt_type);

        return ROHC_SUCCESS;
    }
    else if( pkt_type >= ROHC_IRP0_PKTYPE )
    {   /* Uncompressed Packet */
        /* + SPR 17439 */
        if(ROHC_FAILURE == rohc_make_uncomp_IRP0_pkt_typ(p_rohc_pkt,(rohc_csc_uncomp_t *)p_csc,
                            pkt_type,p_entity,&cid_len,&len,p_in_buff))
        /* - SPR 17439 */
        {
            return ROHC_FAILURE;
        }
        *p_pkt_len += len;
        if(pkt_type == ROHC_IRP0_PKTYPE )
        {
            p_rohc_pkt[2+cid_len]=
                rohc_compute_crc(ROHC_CRC_TYPE_8,
                                p_rohc_org_pkt,*p_pkt_len -1,0xff); 
        }
        else
        {
            *p_len_skip_from_pl = 1;
        }

        /* Incrementing Stats */
        rohcStatsPktTypeUpdate(rohc_global_pkt_types_to_stats_cr[pkt_type],
                                   ROHC_DIR_TX, ROHC_CSC_GET_PROFILE(p_csc));
        ROHC_LOG(LOG_INFO,"CPBM :EID[%u:%u]:CID[%u]: packet "\
                    " type [%d] is created ",
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,
                    p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE],
                    pkt_type);
        return ROHC_SUCCESS;
    }
    returnVal = rohc_p2_pbm_fill_pkt_bits(p_entity, p_csc, 
                                   &p_rohc_pkt, pkt_type, p_ecode);
    if (ROHC_FAILURE == returnVal)
    {
        return ROHC_FAILURE;
    }

    if(p_csc->p_cpib->outer_ip4)
    {
        /*
        ** START_SPR_1040_FIX : Corrected the handling of the IP-ID field while
        ** making the compressor packet. This code is not tested as of now and 
        ** shall be tested when IP tunneling is supported in ROHC library.
        */
        ROHC_SET_16BIT(p_rohc_pkt,p_csc->p_cpib->temp_ipid2);
        /* END_SPR_1040_FIX */

        ROHC_SET_2_OCTS(p_rohc_pkt, 
                p_csc->field_value[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE]);
        p_rohc_pkt+=2;
    }
    /*
    Commented: Temp Fix ROHC Crash
    if( p_csc->outer_ah_data_len )
    {
        ROHC_MEMMOVE(p_rohc_pkt,p_csc->p_outer_ah_data,
                                        p_csc->outer_ah_data_len);
        p_rohc_pkt+= p_csc->outer_ah_data_len;
    }
    */
    if(p_csc->outer_gre_chksum_present)
    {
        ROHC_SET_2_OCTS(p_rohc_pkt,p_csc->outer_gre_chksum);
        p_rohc_pkt+=2;
    }

    if(p_csc->p_cpib->inner_ip4)
    {
        /*
        ** START_SPR_1040_FIX : Corrected the handling of the IP-ID field while
        ** making the compressor packet.  
        */
        ROHC_SET_16BIT(p_rohc_pkt,p_csc->p_cpib->temp_ipid);
        /* END_SPR_1040_FIX */

        p_rohc_pkt+=2;
    }
    /*
    Commented: Temp Fix ROHC Crash
    if( p_csc->inner_ah_data_len )
    {
        ROHC_MEMMOVE(p_rohc_pkt,p_csc->p_inner_ah_data,
                                    p_csc->inner_ah_data_len);
        p_rohc_pkt+= p_csc->inner_ah_data_len;
    }
    */
    if(p_csc->inner_gre_chksum_present)
    {
        ROHC_SET_2_OCTS(p_rohc_pkt,p_csc->inner_gre_chksum);
        p_rohc_pkt+=2;
    }
    if(p_csc->static_delta & ROHC_UDP_CHECK_SUM_PRESENT)
    {
        ROHC_SET_2_OCTS(p_rohc_pkt,p_csc->field_value[ROHC_UDP_CKSUM_FID
                    - ROHC_VAR_FID_BASE]);
        p_rohc_pkt+=2;
    }

    *p_pkt_len=p_rohc_pkt-p_rohc_org_pkt;

    /* Incrementing Stats */
    rohcStatsPktTypeUpdate(rohc_global_pkt_types_to_stats_cr[pkt_type],ROHC_DIR_TX,
            ROHC_CSC_GET_PROFILE(p_csc));

 ROHC_LOG(LOG_INFO,"CPBM :EID[%u:%u]:CID[%u]: packet "\
                    " type [%d] is created ",
                    p_entity->rohc_entity_id.ueIndex,
                    p_entity->rohc_entity_id.lcId,
                    p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE],
                    pkt_type);
    ROHC_EXIT_FN("rohc_make_p2_rohc_compr_pkt");
    return ROHC_SUCCESS;
}/* end function rohc_encode_packet*/

/*****************************************************************************
**      FUNCTION:
**              rohc_p2_pbm_fill_pkt_bits
******************************************************************************
**
**      DESCRIPTION:
**          Compose ROHC Packet 
**      ARGUMENTS:
**          Pointer to ROHC entity     
**          Pointer to CSC     
**          Pointer to ROHC Packet      
**          Packet type
**			Pointer to Error Code     
**      RETURN VALUE:
**          ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
static rohc_return_t  
rohc_p2_pbm_fill_pkt_bits
    ROHC_PARAMS((p_entity,p_csc,p_rohc_pkt,pkt_type,p_ecode),
    struct rohc_entity_t    * p_entity              _AND_
    struct rohc_csc_udp_t   * p_csc                 _AND_
    rohc_U8bit_t            ** p_rohc_pkt            _AND_
    rohc_U8bit_t              pkt_type                _AND_
    rohc_error_code_t       * p_ecode)
{
    rohc_U32bit_t   i=0;
    rohc_U8bit_t    no_of_bits=0;
    rohc_U8bit_t    field_id;   
    rohc_U8bit_t    to_bit_pos=0;
    rohc_U8bit_t    *p_ext_flag = NULL;

    for(i=0;i<ROHC_MAX_NO_OF_FIELDS_IN_A_PACKET && 
            rohc_c_pkt_type[pkt_type][i].field_id!=ROHC_INVALID_FID;i++)
    {
        no_of_bits=rohc_c_pkt_type[pkt_type][i].no_of_bits; 
        field_id=rohc_c_pkt_type[pkt_type][i].field_id;
        if (field_id == ROHC_EXT3_FLAGS_FID)
        {
            p_ext_flag = *p_rohc_pkt;    
        }
        if((field_id == ROHC_INNERIP_ID_FID) && 
                (p_csc->new_inner_rnd == ROHC_RND_PRESENT) &&
                (p_csc->new_outer_rnd == ROHC_RND_ABSENT) )
        {
            field_id = ROHC_OUTERIP_ID_FID;
        }

        if(no_of_bits<=ROHC_ENC_MAX_FIXED_BITS_IN_FIELD)
        {   /* fixed length encoding */
            /* The check should not be <= but < as otherwise ABR can occur */
            if(field_id < ROHC_MAX_CONSTANT_FID)
            {
                rohc_setbits(p_rohc_pkt,&to_bit_pos,
                        rohc_global_constant_field_values[field_id],
                        0,no_of_bits);
            }   /* end if constant width encoding for field 
                   types with globally constant values*/
            else if(field_id >= ROHC_VAR_FID_BASE)
            {   /*Field Values dependent on stream context */
                rohc_setbits(p_rohc_pkt,&to_bit_pos,
                        p_csc->field_value[field_id-ROHC_VAR_FID_BASE],
                        0,no_of_bits);
            }
            else 
            {
                ROHC_LOG(LOG_INFO,"Field_id shall not be b/w 26-49 ");
                return ROHC_FAILURE;
            }

        }/* end if fixed no of bits encoding */
        else if( no_of_bits == ROHC_ENCTYPE_VAR_NO_BITS)
        {
            if((field_id != ROHC_CID_FID)||
                    (p_entity->large_cids == ROHC_TRUE))
            {
                *p_rohc_pkt += rohc_c_encode_sdvl(*p_rohc_pkt,
                        p_csc->field_value[field_id-ROHC_VAR_FID_BASE]);
            }
        }/* end if variable length encoding like large context-id */
        else if(no_of_bits == ROHC_ENCTYPE_EXT3FLAGSP1)
        {
            /* SPR 4808 Fix Start */
            if(NULL != p_ext_flag)
            {
                rohc_add_ele_p2_ext3flagsp1(p_rohc_pkt,p_ext_flag,
                        &to_bit_pos,field_id,p_csc);
            }
            else
            {
                ROHC_LOG(LOG_INFO," Rohc Encoding Failed for Large Ext3 Length\n");
                return ROHC_FAILURE;
            }
            /* SPR 4808 Fix End */
        }   /* end if no_of_bits == EXT 3 flags */
        else if(no_of_bits == ROHC_ENCTYPE_INNERIPFLAGSP1)
        {
            if(ROHC_FAILURE == rohc_add_ele_p2_inneripflagsp1(p_rohc_pkt,
                        &to_bit_pos,field_id,p_csc,p_entity,p_ecode))
                return ROHC_FAILURE;
        }   /* end if no_of_bits == ROHC_ENCTYPE_INNERIPFLAGSP1*/
        else if(no_of_bits == ROHC_ENCTYPE_OUTERIPFLAGSP1)
        {
            if(ROHC_FAILURE == rohc_add_ele_p2_outeripflagsp1(p_rohc_pkt,
                        &to_bit_pos,field_id,p_csc,p_entity,p_ecode))
                return ROHC_FAILURE;
        }/* end if no_of_bits == ROHC_ENCTYPE_OUTERIPFLAGSP1*/
        else if(no_of_bits == ROHC_ENCTYPE_SPLIT_FIELD)
        { /* split field always in base headers */
            rohc_U32bit_t bit_offset;
            rohc_U32bit_t   len;
            /*          cur_occ=rohc_c_pkt_type[pkt_type][i].cur_occ; */
            len=rohc_c_pkt_type[pkt_type][i].no_of_bits_occ;
            bit_offset=p_csc->field_len[field_id-
                ROHC_VAR_FID_BASE]-len;
            rohc_setbits(p_rohc_pkt,&to_bit_pos,
                    p_csc->field_value[field_id-ROHC_VAR_FID_BASE],
                    (rohc_U8bit_t)bit_offset,(rohc_U8bit_t)len);
            p_csc->field_len[field_id-ROHC_VAR_FID_BASE]-=len;
        }/*end if split field found*/
    }/* end for traversal field type in packet type array */
    return ROHC_SUCCESS;
}
/*****************************************************************************
**      FUNCTION:
**              rohc_p6_outer_ip_irreg_chain_type
******************************************************************************
**
**      DESCRIPTION:
**              tells outer ip irregular chain type
**      ARGUMENTS:
**               None
**      RETURN VALUE:
**              void
**      NOTES:
**
*****************************************************************************/
/* Since tunneling is not supported as of now so this function is commented */
#ifdef ROHC_FUNC_ENHANCEMENT 
static rohc_U8bit_t
rohc_p6_outer_ip_irreg_chain_type
    ROHC_PARAMS((p_csc),
    struct rohc_csc_tcp_t   * p_csc  )
{
    rohc_U8bit_t     irreg_chain_type = 0;
    return (irreg_chain_type) ;
}
#endif

/*****************************************************************************
**      FUNCTION:
**              rohc_p6_inner_ip_irreg_chain_type
******************************************************************************
**
**      DESCRIPTION:
**              tells inner ip irregular chain type
**      ARGUMENTS:
**               Pointer to CSC
**      RETURN VALUE:
**              void
**      NOTES:
**
*****************************************************************************/

static rohc_U8bit_t
rohc_p6_inner_ip_irreg_chain_type
    ROHC_PARAMS((p_csc),
    struct rohc_csc_tcp_t   * p_csc  )
{

    rohc_U8bit_t     irreg_chain_type;
    /*if(p_csc->p_cpib->inner_ip6)
    {
        if (ip_inner_ecn == ip_ecn_flags.UVALUE)
        {
            irreg_chain_type = ROHC_P6_IPV6_INNERMOST_IRREG_CHAIN;
        }
        else
        {
            return ROHC_P6_NO_IRREG_CHAIN;
        }
    }*/
    if(p_csc->p_cpib->inner_ip4)
    {
            irreg_chain_type = ROHC_P6_IPV4_INNERMOST_IRREG_CHAIN;
    }
    else
    {
        return ROHC_P6_NO_IRREG_CHAIN;
    }
    return (irreg_chain_type);
}

/*****************************************************************************
 * **      FUNCTION:
 * **              rohc_make_p6_outer_ip_irregular_chain
 * ******************************************************************************
 * **
 * **      DESCRIPTION:
 * **              constructs outer ip irregular chain based on type
 * **      ARGUMENTS:
 * **               None
 * **      RETURN VALUE:
 * **              void
 * **      NOTES:
 * **
 * *****************************************************************************/
/* Since tunneling is not supported as of now so this function is commented */
#ifdef ROHC_FUNC_ENHANCEMENT 
static rohc_return_t
rohc_make_p6_outer_ip_irregular_chain
    ROHC_PARAMS((irreg_chain_type),
    rohc_U8bit_t irreg_chain_type)
{
    ROHC_ENTER_FN("rohc_make_p6_outer_ip_irregular_chain");
    return ROHC_SUCCESS;
    ROHC_EXIT_FN("rohc_make_p6_outer_ip_irregular_chain");
}
#endif

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p6_inner_ip_irregular_chain
******************************************************************************
**
**      DESCRIPTION:
**              constructs inner ip irregular chain based on type
**              currently supporting only ROHC_P6_IPV4_INNERMOST_IRREG_CHAIN
**      ARGUMENTS:
**               None
**      RETURN VALUE:
**              void
**      NOTES:
**
*****************************************************************************/
/* +- SPR 17777 */
static void
rohc_make_p6_inner_ip_irregular_chain
    ROHC_PARAMS((p_p_rohc_pkt,p_csc),
    rohc_U8bit_t    * * p_p_rohc_pkt       _AND_
    struct rohc_csc_tcp_t   * p_csc)
{
    if (IP_ID_BEHAVIOR_RANDOM == 
            p_csc->field_value[ROHC_INNERIP_BEH_FID-ROHC_VAR_FID_BASE])
    {
        ROHC_SET_2_OCTS(*p_p_rohc_pkt,
                p_csc->field_value[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE]);
        *p_p_rohc_pkt = *p_p_rohc_pkt + 2;
    }
    else if (IP_ID_BEHAVIOR_ZERO == 
            p_csc->field_value[ROHC_INNERIP_BEH_FID-ROHC_VAR_FID_BASE])
    {
        /*NO BUFFER WILL BE FILLED FOR BEHAVIOUR ZERO*/
    }
}
/*****************************************************************************
**      FUNCTION:
**              rohc_make_p6_tcp_irregular_chain
******************************************************************************
**
**      DESCRIPTION:
**              constructs tcp irregular chain 
**      ARGUMENTS:
**               None
**      RETURN VALUE:
**              void
**      NOTES:
**
*****************************************************************************/
static void
rohc_make_p6_tcp_irregular_chain
    ROHC_PARAMS((p_p_rohc_pkt,p_csc,irreg_chain_type),
    rohc_U8bit_t    ** p_p_rohc_pkt         _AND_
    struct rohc_csc_tcp_t   * p_csc      _AND_
    rohc_U8bit_t irreg_chain_type)
{
    rohc_U32bit_t   i=0;
    rohc_U8bit_t    no_of_bits=0;
    rohc_U8bit_t    field_id;
    rohc_U8bit_t    to_bit_pos = 0;
   
    for (i=0; rohc_c_irreg_chain_type[irreg_chain_type][i].field_id!=
            ROHC_INVALID_FID;i++)
    {
        no_of_bits=rohc_c_irreg_chain_type[irreg_chain_type][i].no_of_bits;
        field_id=rohc_c_irreg_chain_type[irreg_chain_type][i].field_id;
        if (ROHC_INNERIP4_ECN_FID == field_id)
        {
            if (p_csc->field_value[ROHC_TCP_ECN_USED_FID-ROHC_VAR_FID_BASE])
            {
                rohc_setbits(p_p_rohc_pkt,&to_bit_pos,
                        (((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->tos) & 
                        ROHC_PROF6_IP4_ECN_FLAG_MASK,0,no_of_bits);
            }
        }
        else if (ROHC_TCP_RES_FLAG_FID == field_id)
        {
            if (p_csc->field_value[ROHC_TCP_ECN_USED_FID-ROHC_VAR_FID_BASE])
            {
                rohc_setbits(p_p_rohc_pkt,&to_bit_pos,
                        (((rohc_tcp_t*)p_csc->p_cpib->tcp)->offset) &
                        ROHC_TCP_RES_MASK,0,no_of_bits);
            }
        }
        else if(ROHC_TCP_ECN_FLAG_FID == field_id)
        {
            if (p_csc->field_value[ROHC_TCP_ECN_USED_FID-ROHC_VAR_FID_BASE])
            {
                rohc_setbits(p_p_rohc_pkt,&to_bit_pos,
                        (((rohc_tcp_t*)p_csc->p_cpib->tcp)->flags & 
                         ROHC_TCP_ECN_MASK )>>6 ,0,no_of_bits);
            }
        }
        else if(ROHC_TCP_CKSUM_FID == field_id)
        {
            ROHC_SET_2_OCTS(*p_p_rohc_pkt,(p_csc->field_value
                        [ROHC_TCP_CKSUM_FID-ROHC_VAR_FID_BASE]));
            *p_p_rohc_pkt = *p_p_rohc_pkt + 2;
        }
    }
}

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p6_tcp_options_irregular_chain
******************************************************************************
**
**      DESCRIPTION:
**              constructs tcp options irregular chain
**      ARGUMENTS:
**               None
**      RETURN VALUE:
**              void
**      NOTES:
**
*****************************************************************************/
/* +- SPR 17777 */
static void
rohc_make_p6_tcp_options_irregular_chain
    ROHC_PARAMS((p_rohc_entity,p_rohc_pkt,p_csc,
                p_len_tcp_opt_irrg),
    struct rohc_entity_t  * p_rohc_entity     _AND_
    rohc_U8bit_t    * p_rohc_pkt         _AND_
    struct rohc_csc_tcp_t   * p_csc      _AND_
    rohc_U8bit_t    *p_len_tcp_opt_irrg)
{
    rohc_U32bit_t   ts_val=0;
    rohc_U32bit_t   ts_echo=0;
    rohc_U8bit_t    kind = 0;
    rohc_U8bit_t    index = 0;
    rohc_U8bit_t    length = 0;
    rohc_encoded_value_t p_en_val;
    ROHC_MEMSET((rohc_void_tp)&p_en_val,0,
                  sizeof(rohc_encoded_value_t));
 
    rohc_cpib_t   *p_cpib = ROHC_CSC_GET_CPIB(p_csc);
    while(p_csc->rohc_c_tcp_opt.tcp_opt_kind[index]!= 0xff && 
            index < ROHC_MAX_ENTRIES_TCP_OPT_IDX)
    {
        length = 0;
        kind = p_csc->rohc_c_tcp_opt.tcp_opt_kind[index];
        switch(kind)
        {
            case TCP_NOP_OPT :

            case TCP_EOL_OPT :

            case TCP_MSS_OPT :

            case TCP_SACK_PERMTD_OPT:

            case TCP_WS_OPT :
                /* empty irregular chain */
                break;

            case TCP_TS_OPT :
                if(p_csc->ctrl_field_val_p6[ROHC_TCP_LIST_PRESENT_FID -
                        ROHC_PROF6_VAR_CTRL_FID_BASE])
                {
                    /* list structure changed so TS will be sent in list */
                }
                else
                {
                    ROHC_MEMCPY(p_csc->rohc_c_tcp_opt.ts,
                            p_csc->p_cpib->rohc_c_temp_tcp_opt.ts, 8);
                    ts_val =  ROHC_NTOHL(p_csc->rohc_c_tcp_opt.ts[0]); 
                    ts_echo = ROHC_NTOHL(p_csc->rohc_c_tcp_opt.ts[1]); 
                    /*TS VAL ENCODING AND FILLING */
                    rohc_encode_p6_ts_opt(p_rohc_entity, ts_val, &p_en_val,
                            p_csc->rohc_gsw.min_ts_val,p_csc->rohc_gsw.max_ts_val);
                    rohc_set_ts_opt_bits(&p_en_val,p_rohc_pkt,
                            &length);
                    *p_len_tcp_opt_irrg += length;
                    p_rohc_pkt+= length;
                    rohc_ed_p6_update_gsw_ts_val(p_rohc_entity,p_csc,ts_val);

                    ROHC_MEMSET((rohc_void_tp)&p_en_val,0,
                            sizeof(rohc_encoded_value_t));

                    /*TS ECHO ENCODING AND FILLING */ 
                    rohc_encode_p6_ts_opt(p_rohc_entity,  
                            ts_echo, &p_en_val,p_csc->rohc_gsw.min_ts_echo,
                            p_csc->rohc_gsw.max_ts_echo);
                    rohc_set_ts_opt_bits(&p_en_val,p_rohc_pkt,
                            &length);
                    *p_len_tcp_opt_irrg += length;
                    p_rohc_pkt+= length;
                    rohc_ed_p6_update_gsw_ts_echo(p_rohc_entity,p_csc,ts_echo);
                }
                break;

            case TCP_SACK_OPT :
                /* check if SACK is unchanged */
                if(p_csc->p_cpib->rohc_c_temp_tcp_opt.sack_len == 
                        p_csc->rohc_c_tcp_opt.tcp_opt_len[SACK_LEN_ID])
                {
                    if(ROHC_MEMCMP(p_csc->p_cpib->rohc_c_temp_tcp_opt.sack_ptr,
                                p_csc->rohc_c_tcp_opt.sack_ptr,
                                p_csc->rohc_c_tcp_opt.tcp_opt_len
                                [SACK_LEN_ID] - 2) == 0)
                    {
                        /* fill only discriminator as sack is static */
                        *p_rohc_pkt++ = 0;
                        *p_len_tcp_opt_irrg += 1;
                        break;
                    }
                }
                /* sack is changed so update its value in context and 
                 * encode sack option*/
                p_csc->rohc_c_tcp_opt.tcp_opt_len[SACK_LEN_ID] = 
                    p_csc->p_cpib->rohc_c_temp_tcp_opt.sack_len;
                ROHC_MEMCPY(p_csc->rohc_c_tcp_opt.sack_ptr, 
                        p_csc->p_cpib->rohc_c_temp_tcp_opt.sack_ptr,
                        p_csc->rohc_c_tcp_opt.tcp_opt_len[SACK_LEN_ID]-2);
                rohc_fill_sack_opt(p_csc,p_rohc_pkt,&length);
                p_rohc_pkt+= length;
                *p_len_tcp_opt_irrg += length;
                break;

            case TCP_ALT_CKSUM_REQ_OPT:

                if(p_csc->ctrl_field_val_p6[ROHC_TCP_LIST_PRESENT_FID -
                        ROHC_PROF6_VAR_CTRL_FID_BASE])
                {
                    /* Structure of list is changed so generic options are sent
                     * as part of list item */
                }
                else
                {
                    /* Alternate ChkSum Request option is non static */
                    if(p_cpib->rohc_c_temp_tcp_opt.alt_cksum_req ==
                            p_csc->rohc_c_tcp_opt.alt_cksum_req)
                    {
                        /*option is unchanged so discriminator 
                         * 11111111 is filled*/
                        *p_rohc_pkt++ = 255;
                        *p_len_tcp_opt_irrg += 1;
                    }
                    else
                    {
                        p_csc->rohc_c_tcp_opt.alt_cksum_req = 
                            p_cpib->rohc_c_temp_tcp_opt.alt_cksum_req;
                        *p_rohc_pkt++ = 0;
                        *p_len_tcp_opt_irrg += 1;
                        *p_rohc_pkt = p_csc->rohc_c_tcp_opt.alt_cksum_req;
                        p_rohc_pkt += TCP_ALT_CKSUM_REQ_OPT_LEN - 2;
                        *p_len_tcp_opt_irrg += TCP_ALT_CKSUM_REQ_OPT_LEN - 2;
                    }
                }
                break;
            default :
                /* Generic options */
                if(p_csc->ctrl_field_val_p6[ROHC_TCP_LIST_PRESENT_FID -
                        ROHC_PROF6_VAR_CTRL_FID_BASE])
                {
                    /* Structure of list is changed so generic options are sent
                     * as part of list item */
                }
                else
                {
                    rohc_fill_generic_opt(p_csc,p_rohc_pkt,&length,kind);
                    p_rohc_pkt+= length;
                    *p_len_tcp_opt_irrg += length;
                }
                break;
        }
        index++;
    }
}

/*****************************************************************************
**      FUNCTION:
**              rohc_set_ts_opt_bits
******************************************************************************
**
**      DESCRIPTION:
**              sets ts option bits in compressed header
**      ARGUMENTS:
**               encoded value, compressed pkt,len
**      RETURN VALUE:
**              void
**      NOTES:
**
*****************************************************************************/
STATIC void
rohc_set_ts_opt_bits
    ROHC_PARAMS((p_en_val,p_rohc_pkt,p_len),
    rohc_encoded_value_t    *p_en_val            _AND_
    rohc_U8bit_t            *p_rohc_pkt          _AND_
    rohc_U8bit_t            *p_len )
{
    *p_len = 0;
    if (p_en_val->bits <= 7)
    {
        *p_rohc_pkt = (0x00  | (0x7F & p_en_val->value));
        *p_len = 1;
    }
    else if (p_en_val->bits <= 14)
    {
        ROHC_SET_2_OCTS(p_rohc_pkt,((0x02 << 14) | 
                    (0x3FFF & p_en_val->value)));
        *p_len = 2;
    }
    else if (p_en_val->bits <=21)
    {
        ROHC_SET_3_OCTS(p_rohc_pkt,((0x03 << 22) | 
                    (0x1FFFFF & p_en_val->value)));
        *p_len = 3;
    }
    else
    {
        ROHC_SET_4_OCTS(p_rohc_pkt,((0x07 << 29) | 
                    (0x1FFFFFFF & p_en_val->value)));
        *p_len = 4;
    }
}

/*****************************************************************************
**      FUNCTION:
**             rohc_fill_sack_opt
******************************************************************************
**
**      DESCRIPTION:
**             fills sack option in compressed header
**      ARGUMENTS:
**               p_csc, compressed pkt,len
**      RETURN VALUE:
**              void
**      NOTES:
**
*****************************************************************************/
STATIC void
rohc_fill_sack_opt
    ROHC_PARAMS((p_csc,p_rohc_pkt,p_len),
    struct rohc_csc_tcp_t   * p_csc         _AND_
    rohc_U8bit_t            *p_rohc_pkt     _AND_
    rohc_U8bit_t            *p_len )
{
    rohc_U8bit_t    sack_len = 0;
    *p_len = 0;
    sack_len = p_csc->rohc_c_tcp_opt.tcp_opt_len[SACK_LEN_ID];
    if(10 == sack_len)
    {
        /* 1 sack block */
        sack1_item_enc(p_csc->rohc_c_tcp_opt.sack_ptr,
                p_csc->field_value[ROHC_TCP_ACKNO_FID-
                ROHC_VAR_FID_BASE],
                p_rohc_pkt,p_len);
    }
    else if(18 == sack_len)
    {
        /* 2 sack blocks */
        sack2_item_enc(p_csc->rohc_c_tcp_opt.sack_ptr,
                p_csc->field_value[ROHC_TCP_ACKNO_FID-
                ROHC_VAR_FID_BASE],
                p_rohc_pkt,p_len);
    }
    else if(26 == sack_len)
    {
        /* 3 sack blocks */
        sack3_item_enc(p_csc->rohc_c_tcp_opt.sack_ptr,
                p_csc->field_value[ROHC_TCP_ACKNO_FID-
                ROHC_VAR_FID_BASE],
                p_rohc_pkt,p_len);
    }
    else
    {
        /* 4 sack blocks */
        sack4_item_enc(p_csc->rohc_c_tcp_opt.sack_ptr,
                p_csc->field_value[ROHC_TCP_ACKNO_FID-
                ROHC_VAR_FID_BASE],
                p_rohc_pkt,p_len);
    }
}

/*****************************************************************************
**      FUNCTION:
**             rohc_fill_generic_opt
******************************************************************************
**
**      DESCRIPTION:
**             prepare irregular chain of generic tcp options
**      ARGUMENTS:
**               p_csc, compressed pkt,len
**      RETURN VALUE:
**              void
**      NOTES:
**
*****************************************************************************/
STATIC void
rohc_fill_generic_opt
    ROHC_PARAMS((p_csc,p_rohc_pkt,p_len,kind),
    struct rohc_csc_tcp_t   * p_csc         _AND_
    rohc_U8bit_t            *p_rohc_pkt     _AND_
    rohc_U8bit_t            *p_len          _AND_
    rohc_U8bit_t            kind )
{
    rohc_U8bit_t    i, l_index = 0;
    rohc_c_generic_opt_elem_t temp_gen_option;
    rohc_c_generic_opt_elem_t gen_option_context;
    *p_len = 0;
    /* prepare irregular chain of generic tcp options and
     * update changed values in context*/
    l_index = p_csc->rohc_c_tcp_opt.rohc_c_generic_opt.
        next_free_index;
    for(i = 0; i<l_index; i++)
    {
        if(kind == p_csc->rohc_c_tcp_opt.rohc_c_generic_opt.
                rohc_generic_opt_elem[i].kind)
        {
            break;
        }
    }
    temp_gen_option = p_csc->p_cpib->rohc_c_temp_tcp_opt.
        rohc_c_generic_opt.rohc_generic_opt_elem[i];
    gen_option_context = p_csc->rohc_c_tcp_opt.
        rohc_c_generic_opt.rohc_generic_opt_elem[i];
    if(gen_option_context.option_static)
    {
        /* generic_static_irregular case so option is
         * unchanged */
    }
    else
    {
        if(temp_gen_option.length == gen_option_context.
                length)
        {
            if(ROHC_MEMCMP(temp_gen_option.opt_val,
                        gen_option_context.opt_val,
                        temp_gen_option.length-2) == 0)
            {
                /*option is unchanged so discriminator 
                 * 11111111 is filled*/
                *p_rohc_pkt = 255;
                *p_len = 1;
            }
            else
            {
                /* content of options are changed */
                ROHC_MEMCPY(ROHC_CSC_GET_GENERIC_OPT_VAL(
                            p_csc,i),temp_gen_option.opt_val
                        ,temp_gen_option.length-2);
                *p_rohc_pkt++ = 0;
                *p_len += 1;
                ROHC_MEMCPY(p_rohc_pkt,temp_gen_option.opt_val,
                        temp_gen_option.length-2);
                *p_len += temp_gen_option.length - 2;
            }
        }
        else
        {
            /* if length of generic option is changed
             * they are sent as part of list*/
        }
    }
}

/*****************************************************************************
**      FUNCTION:
**              rohc_make_p6_rohc_compr_pkt
******************************************************************************
**
**      DESCRIPTION:
**          Compose ROHC Packet 
**      ARGUMENTS:
**          Pointer to CSC     
**          Pointer to ROHC Packet      
**          Packet type     
**          Pointer to Packet length     
**          Pointer to ROHC entity     
**          Pointer to global context
**      RETURN VALUE:
**          ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
/* +- SPR 17777 */
rohc_return_t  
rohc_make_p6_rohc_compr_pkt
    ROHC_PARAMS((p_entity,p_csc,pkt_type,p_pkt_len,
                p_rohc_pkt,p_ecode),
    struct rohc_entity_t  * p_entity           _AND_
    struct rohc_csc_tcp_t * p_csc              _AND_
    rohc_U8bit_t          pkt_type             _AND_
    rohc_U32bit_t         * p_pkt_len          _AND_
    rohc_U8bit_t          * p_rohc_pkt         _AND_
    rohc_error_code_t     * p_ecode)
/* +- SPR 17777 */
{
    rohc_U32bit_t   len=0;
    rohc_U8bit_t    cid_len=0;
    rohc_U8bit_t    * p_rohc_org_pkt=p_rohc_pkt;
    rohc_U8bit_t irreg_chain_type;
    rohc_U8bit_t    len_tcp_opt_irrg = 0;   
    rohc_return_t    returnVal = ROHC_SUCCESS;
    ROHC_ENTER_FN("rohc_make_p6_rohc_compr_pkt");

    if(ROHC_FALSE == p_entity->large_cids &&
            p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE])
    {
        *p_rohc_pkt++=0xE0|p_csc->field_value[ROHC_CID_FID- 
            ROHC_VAR_FID_BASE];
        *p_pkt_len = 1;
    }/* Small CID is being used*/
    else
    {
        *p_pkt_len = 0;
    }

    if(pkt_type >= ROHC_IR_WITH_DYN_PKTTYPE )
    {  
        if(ROHC_FAILURE == rohc_make_p6_IR_pkts(p_rohc_pkt,p_csc,
                    pkt_type,p_entity,&cid_len,&len,p_ecode))
        {
            return ROHC_FAILURE;
        }
        *p_pkt_len += len;
        p_rohc_pkt[2+cid_len]=
            rohc_compute_crc(ROHC_CRC_TYPE_8,p_rohc_org_pkt,*p_pkt_len,0xff); 

        /* Incrementing Stats */
        rohcStatsPktTypeUpdate(rohc_global_pkt_types_to_stats_cr[pkt_type],
                ROHC_DIR_TX, ROHC_CSC_GET_PROFILE(p_csc));
        ROHC_LOG(LOG_INFO,"CPBM :EID[%u:%u]:CID[%u]: packet "\
                " type [%d] is created ",
                p_entity->rohc_entity_id.ueIndex,
                p_entity->rohc_entity_id.lcId,
                p_csc->field_value[ROHC_CID_FID-ROHC_VAR_FID_BASE],
                pkt_type);

        return ROHC_SUCCESS;
    }

    /* PACKET FORMATION STARTS OTHER THAN IR PACKETS */
    returnVal = rohc_p6_pbm_fill_pkt_bits(p_entity, p_csc, 
                                   &p_rohc_pkt, pkt_type);
    if (ROHC_FAILURE == returnVal)
    {
        return ROHC_FAILURE;
    }
    /* Since tunneling is not supported as of now 
     * so this part is commented*/
#ifdef ROHC_FUNC_ENHANCEMENT  
    irreg_chain_type = 
                rohc_p6_outer_ip_irreg_chain_type(p_csc);

    if (ROHC_P6_NO_IRREG_CHAIN != irreg_chain_type)
    {
        rohc_make_p6_outer_ip_irregular_chain(irreg_chain_type);
    }
#endif

    irreg_chain_type = 
                rohc_p6_inner_ip_irreg_chain_type(p_csc);
    if (ROHC_P6_NO_IRREG_CHAIN != irreg_chain_type)
    {
        rohc_make_p6_inner_ip_irregular_chain(&p_rohc_pkt,p_csc);
        /* +- SPR 17777 */
    }
     /* Incrementing Stats */
    rohcStatsPktTypeUpdate(rohc_global_pkt_types_to_stats_cr[pkt_type],
                              ROHC_DIR_TX, ROHC_CSC_GET_PROFILE(p_csc));

    rohc_make_p6_tcp_irregular_chain(&p_rohc_pkt,p_csc,ROHC_P6_TCP_IRREG_CHAIN);

    if(ROHC_GET_UPPER_NIBBLE(((rohc_tcp_t*)p_csc->p_cpib->tcp)->offset) >5)
    {
        rohc_make_p6_tcp_options_irregular_chain(p_entity,p_rohc_pkt,
                p_csc,&len_tcp_opt_irrg);
        /* +- SPR 17777 */
    }
    else
    {
        /* set presence of TS opt false for current pkt */
        p_csc->rohc_gsw.rohc_gsw_elem[ROHC_ED_GET_CUR_INDEX(p_csc)].
            ts_opt_recvd = ROHC_FALSE;
    }
    p_rohc_pkt+=len_tcp_opt_irrg;
    *p_pkt_len=p_rohc_pkt-p_rohc_org_pkt;

    rohcStatsPktTypeUpdate(rohc_global_pkt_types_to_stats_cr[pkt_type],
                        ROHC_DIR_TX, ROHC_CSC_GET_PROFILE(p_csc));
    return ROHC_SUCCESS;
}

/*****************************************************************************
**      FUNCTION:
**              rohc_p6_pbm_fill_pkt_bits
******************************************************************************
**
**      DESCRIPTION:
**          Compose ROHC Packet 
**      ARGUMENTS:
**          Pointer to ROHC entity     
**          Pointer to CSC     
**          Pointer to ROHC Packet      
**          Packet type     
**      RETURN VALUE:
**          ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**               
*****************************************************************************/
STATIC rohc_return_t  
    /* + SPR 17439 */
rohc_p6_pbm_fill_pkt_bits
    ROHC_PARAMS((p_entity,p_csc,p_rohc_pkt,pkt_type),
    struct rohc_entity_t    * p_entity              _AND_
    struct rohc_csc_tcp_t   * p_csc                 _AND_
    rohc_U8bit_t            ** p_rohc_pkt            _AND_
    rohc_U8bit_t              pkt_type)
    /* - SPR 17439 */
{
    rohc_U32bit_t   i=0;
    rohc_U8bit_t    no_of_bits=0;
    rohc_U8bit_t    field_id;   
    rohc_U8bit_t    to_bit_pos=0;
    rohc_U8bit_t    tmp;   
    for(i=0;i<ROHC_MAX_NO_OF_FIELDS_IN_A_PACKET &&
            rohc_c_pkt_type[pkt_type][i].field_id!=ROHC_INVALID_FID;i++)
    {
        no_of_bits=rohc_c_pkt_type[pkt_type][i].no_of_bits;
        field_id=rohc_c_pkt_type[pkt_type][i].field_id;
        if(no_of_bits<=ROHC_ENC_MAX_FIXED_BITS_IN_FIELD)
        {   /* fixed length encoding */
            /* The check should not be <= but < as otherwise ABR can occur */
            if(field_id < ROHC_MAX_CONSTANT_FID)
            {
                rohc_setbits(p_rohc_pkt,&to_bit_pos,
                        rohc_global_constant_field_values[field_id],
                        0,no_of_bits);
            }   /* end if constant width encoding for field
                   types with globally constant values*/
            else if(field_id >= ROHC_VAR_FID_BASE && field_id <=
                    (ROHC_MAX_FIELD_TYPES - 1))
            {   /*Field Values dependent on stream context */
                rohc_setbits(p_rohc_pkt,&to_bit_pos,
                        p_csc->field_value[field_id-ROHC_VAR_FID_BASE],
                        0,no_of_bits);
            }
            else if(field_id >= ROHC_PROF6_VAR_CTRL_FID_BASE && field_id < 
                ROHC_PROF6_MAX_CTRL_FID) 
            {
                if(ROHC_TCP_RSF_FLAGS_FID == field_id)
                {
                    rohc_encode_rsf_field(p_rohc_pkt, p_csc,
                            &to_bit_pos);
                }
                else if(ROHC_TCP_SEQ_IND_FID == field_id)
                {
                    tmp = rohc_c_prof6_calc_bits(
                            p_csc->field_len[ROHC_SN_FID-ROHC_VAR_FID_BASE]);
                    p_csc->ctrl_field_val_p6[field_id - 
                        ROHC_PROF6_VAR_CTRL_FID_BASE] = tmp;
                    rohc_setbits(p_rohc_pkt,&to_bit_pos,
                            tmp,0,no_of_bits);
                }
                else if(ROHC_TCP_ACK_IND_FID == field_id)
                {
                    tmp = rohc_c_prof6_calc_bits(
                            p_csc->field_len[ROHC_TCP_ACKNO_FID -
                            ROHC_VAR_FID_BASE]);
                    p_csc->ctrl_field_val_p6[field_id -
                        ROHC_PROF6_VAR_CTRL_FID_BASE] = tmp;
                    rohc_setbits(p_rohc_pkt,&to_bit_pos,
                            tmp,0,no_of_bits);
                }
                else
                {
                    rohc_setbits(p_rohc_pkt,&to_bit_pos,
                            p_csc->ctrl_field_val_p6[field_id-
                            ROHC_PROF6_VAR_CTRL_FID_BASE],0,no_of_bits);
                }
            }
            else
            {
                ROHC_LOG(LOG_INFO,"Field_id shall not be b/w 28-39 ");
                return ROHC_FAILURE;
            }

        }/* end if fixed no of bits encoding */
        else if(ROHC_ENCTYPE_VAR_NO_BITS == no_of_bits)
        {
            if((field_id != ROHC_CID_FID)||
                    (ROHC_TRUE == p_entity->large_cids))
            {
                *p_rohc_pkt += rohc_c_encode_sdvl(*p_rohc_pkt,
                        p_csc->field_value[field_id-ROHC_VAR_FID_BASE]);
            }
        }/* end if variable length encoding like large context-id */
        else if(ROHC_ENCTYPE_SPLIT_FIELD == no_of_bits)
        { /* split field always in base headers */
            rohc_U32bit_t bit_offset;
            rohc_U32bit_t   len;
            len=rohc_c_pkt_type[pkt_type][i].no_of_bits_occ;
            bit_offset=p_csc->field_len[field_id-
                ROHC_VAR_FID_BASE]-len;
            rohc_setbits(p_rohc_pkt,&to_bit_pos,
                    p_csc->field_value[field_id-ROHC_VAR_FID_BASE],
                    (rohc_U8bit_t)bit_offset,(rohc_U8bit_t)len);
            p_csc->field_len[field_id-ROHC_VAR_FID_BASE]-=len;
        }/*end if split field found*/
        else if(ROHC_ENCTYPE_VAR_32_BITS == no_of_bits)
        {
            rohc_p6_variable_length_32_enc(p_rohc_pkt,
                    &to_bit_pos,field_id,p_csc);
        }
        else if(ROHC_ENCTYPE_STATIC_OR_IRREG == no_of_bits)
        {
            rohc_p6_static_or_irregular_enc(p_rohc_pkt,
                    &to_bit_pos,field_id,p_csc);
        }
        else if(ROHC_ENCTYPE_OPT_IP_ID_LSB == no_of_bits)
        {
            rohc_p6_opt_ip_id_lsb_enc();
            /* +- SPR 17777 */
        }
        else if(ROHC_ENCTYPE_TCP_OPTIONS_LIST == no_of_bits)
        {
            if(p_csc->ctrl_field_val_p6[ROHC_TCP_LIST_PRESENT_FID-ROHC_PROF6_VAR_CTRL_FID_BASE])
            {
                rohc_U32bit_t len = 0;
                if(ROHC_FAILURE == 
                        rohc_tcp_opt_list_compressed_pkt(p_entity,*p_rohc_pkt,
                            p_csc,&len))
                {
                    return ROHC_FAILURE;
                }
                *p_rohc_pkt += len;
            }
        }
    }/* end for traversal field type in packet type array */
    return ROHC_SUCCESS;
}

/******Feedback Packet Fromation***************************************/
/*****************************************************************************
**      FUNCTION:
**              rohc_d_make_feedback_packet
******************************************************************************
**
**      DESCRIPTION:
**          Compose Feedback packet 
**      ARGUMENTS:
**          Pointer to memory location where length is to be stored     
**          Pointer to entity     
**          Pointer to global context     
**      RETURN VALUE:
**          ROHC_SUCCESS or ROHC_FAILURE
**      NOTES:
**      This function composes feedback buffer from Feedback structure
        when invoked by CPM . D-PPM queues the received feedback buffers    
*****************************************************************************/
rohc_return_t 
rohc_d_make_feedback_packet 
ROHC_PARAMS((p_entity,p_feedback,p_ecode),
        struct rohc_entity_t * p_entity _AND_
        struct rohc_feedback_t * p_feedback _AND_   
        rohc_error_code_t   *p_ecode)
{
    rohc_U8bit_t *p_feedback_pkt=NULL,*p_out_pkt;
    rohc_U32bit_t len=0;
    rohc_U8bit_t  i=0, append_crc=ROHC_FALSE;
    ROHC_ENTER_FN("rohc_d_make_feedback_packet  "); 
    p_out_pkt = 
        p_feedback_pkt=(rohc_U8bit_t *)ROHC_MALLOC(ROHC_MAX_FEEDBACK_PKT_SIZE);
    if(p_feedback_pkt == NULL)
    {
        ROHC_LOG(LOG_INFO,"CPBM :EID[%u:%u]:CID[%u]: Unable"\
                " to allocate memory for feedback packet",
                p_entity->rohc_entity_id.ueIndex,p_entity->rohc_entity_id.lcId,
                p_entity->context_id);
        *p_ecode = ROHC_MEMORY_FAILURE;
        return ROHC_FAILURE;
    }
    p_feedback_pkt[0]=0xf0;
    p_feedback_pkt[1]=0;
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

    if(p_feedback->feedback_type == ROHC_FB_1)
    {
        p_feedback_pkt[len]=(rohc_U8bit_t)p_feedback->sn;
        len += 1;

        p_out_pkt[ROHC_MAX_API_ELMS_LEN + ROHC_MAX_ELEM_LEN] |= len;
        ROHC_MEMMOVE(
                (rohc_U8bit_t*)(p_out_pkt +( ROHC_MAX_API_ELMS_LEN+
                        ROHC_MAX_ELEM_LEN + 1)),
                (rohc_U8bit_t*)(p_out_pkt +( ROHC_MAX_API_ELMS_LEN+
                        ROHC_MAX_ELEM_LEN + 2)),
                len);

        /* START_COVERITY_25502_FIX*/
        p_entity->fb_info.p_buff = p_out_pkt; 
        p_entity->fb_info.len = len;
        /* END_COVERITY_25502_FIX*/
        return ROHC_SUCCESS;
    } 
    else /* feedback-2 */
    {
        p_feedback_pkt[len] = 
            (((p_feedback->ack_type<<6)|(p_feedback->mode<<4)));
        /*  SN TBV */
        if(p_feedback->sn <= 0x00000fff)
        {
            p_feedback_pkt[len+0] |= p_feedback->sn >>8;
            p_feedback_pkt[len+1] = (rohc_U8bit_t)p_feedback->sn;
            len += 2;
        }
        else if(p_feedback->sn <= 0x0000fffff)
        {
            p_feedback_pkt[len+0] |= p_feedback->sn >>16;
            p_feedback_pkt[len+1] = p_feedback->sn>>8;
            p_feedback_pkt[len+2] = 0x41;
            p_feedback_pkt[len+3] = (rohc_U8bit_t)p_feedback->sn;
            len += 4;

        }else if(p_feedback->sn <= 0x0fffffff)
        {
            p_feedback_pkt[len+0] |= p_feedback->sn >>24;
            p_feedback_pkt[len+1] = p_feedback->sn>>16;
            p_feedback_pkt[len+2] = 0x41;
            p_feedback_pkt[len+3] = p_feedback->sn>>8;
            p_feedback_pkt[len+4] = 0x41;
            p_feedback_pkt[len+5] = (rohc_U8bit_t)p_feedback->sn;
            len += 6;
        }else
        {
            p_feedback_pkt[len+0] |= 0;
            p_feedback_pkt[len+1] = p_feedback->sn>>24;
            p_feedback_pkt[len+2] = 0x41;
            p_feedback_pkt[len+3] = p_feedback->sn>>16;
            p_feedback_pkt[len+4] = 0x41;
            p_feedback_pkt[len+5] = p_feedback->sn>>8;
            p_feedback_pkt[len+6] = 0x41;
            p_feedback_pkt[len+7] = (rohc_U8bit_t)p_feedback->sn;
            len += 8;
        } 
    }   

    while(p_feedback->opt_type[i] !=  ROHC_INVALID_OPT_TYPE)
    {
        switch(p_feedback->opt_type[i])
        {
            case ROHC_FBO_CRC : 
            p_feedback_pkt[len+0]=0x11U;
            p_feedback_pkt[len+1]=0x00U;
            len+=2;
            append_crc=ROHC_TRUE;
            break;

            case ROHC_FBO_REJECT : 
            p_feedback_pkt[len+0]=0x20U;
            len+=1;
            break;

            case ROHC_FBO_SN_NOT_VALID : 
            p_feedback_pkt[len+0]=0x30U;
            len+=1;
            break;

            case ROHC_FBO_SN :
            /* len+=2;
               p_feedback_pkt[0]=0x41U;*/
            /*              SN TRV */
            /*              p_feedback_pkt[1]=p_feedback->sn&(0xff000000);
                            p_feedback->sn<<=8;
                            p_feedback+=2;*/
            break;

            case ROHC_FBO_CLOCK : 
            p_feedback_pkt[len+0]=0x51U;
            p_feedback_pkt[len+1]=p_feedback->opt_val[i];
            len+=2;
            break;

            case ROHC_FBO_JITTER : 
            p_feedback_pkt[len+0]=0x61U;
            p_feedback_pkt[len+1]=p_feedback->opt_val[i];
            len+=2;
            break;

            case ROHC_FBO_LOSS : 
            p_feedback_pkt[len+0]=0x71U;
            p_feedback_pkt[len+1]=p_feedback->opt_val[i];
            len+=2;
            break;
        }   /* end switch*/
        i++;
    }   /* end while */

    if( len <= 7) 
    {
        ROHC_MEMMOVE((p_out_pkt + 1),
                (p_out_pkt + 2),
                len);
        if(append_crc == ROHC_TRUE)
        { 
            p_out_pkt[len]=rohc_compute_crc(ROHC_CRC_TYPE_8,(p_out_pkt+1),len,0xff);
        }
        len+=1;
    }
    else
    {
        p_out_pkt[1]=len;
        if(append_crc == ROHC_TRUE)
        { 
            p_out_pkt[len+1]=rohc_compute_crc(ROHC_CRC_TYPE_8,(p_out_pkt+ 1),len,0xff);
        }   
        len+=2;
    }

    p_entity->fb_info.p_buff = p_out_pkt; 
    p_entity->fb_info.len = len;
    ROHC_EXIT_FN("rohc_d_make_feedback_packet");
    return ROHC_SUCCESS;
}/* end function rohc_d_make_feedback_packet */

/* Start  SPR# 6563 Fix*/
/*****************************************************************************
 ** Function Name  : rohc_c_encode_sdvl_ts 
 ** Inputs         : encoded Octests, Value to be encoded
 ** Outputs        : 
 ** Returns        : No of Octets
 ** Description    : it does the sdvl encoding
 ******************************************************************************/
rohc_U8bit_t rohc_c_encode_sdvl_ts
        ROHC_PARAMS((dest,value , length),           
        rohc_U8bit_t *dest _AND_
        rohc_U32bit_t value _AND_ 
        rohc_U32bit_t length)
{   
    rohc_U8bit_t tmp;
    if( length > 21 ){
        *dest++ = ((7 << 5) ^ ((value >> 24) & 31)) & 255;  /* 7 = bit pattern 111 */
        *dest++ = (value >> 16) & 255;
        *dest++ = (value >> 8) & 255;
        *dest = value & 255;
        tmp = 4; 
    } else if( length > 14 ){      
        *dest++ = ((6 << 5) ^ ((value >> 16) & 31)) & 255;  /* 6 = bit pattern 110 */
        *dest++ = (value >> 8) & 255;
        *dest = value & 255;
        tmp = 3; 
    } else if( length > 7 ){
        *dest++ = ((2 << 6) ^ ((value >> 8)& 63)) & 255;    /* 2 = bit pattern 10 */
        *dest = value & 255;
        tmp = 2; 
    } else if( length <= 7 ){
        *dest = value & 255;
        tmp = 1; 
    }
    return tmp;
}
/* End  SPR# 6563 Fix*/
