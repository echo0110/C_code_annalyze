/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_com_def.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_com_def.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:55  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/28 04:41:36  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_COM_DEF_H_
#define _ROHC_COM_DEF_H_

#define IN
#define OUT
#define INOUT

/* + SPR 17439 */
#define ROHC_PROTO(X)            X
/* - SPR 17439 */
#define ROHC_PARAMS(X,Y)         X Y;
/* + SPR 17439 */
#define ROHC_NO_PARAMS()         (void)
/* - SPR 17439 */
#define _AND_                    ;



#define ROHC_DIR_TX 0
#define ROHC_DIR_RX 1
#define ROHC_DIR_BOTH 2
#define ROHC_IPV4_MIN_HDR_LEN  20
/* SPR 9155 fix start */
#define ROHC_IPV6_MIN_HDR_LEN  40
#define ROHC_UDP_HDR_LEN 8
/* SPR 9155 fix stop */

/* Supported ROHC profiles */
#define ROHC_UNCOMP     (0U)
#define ROHC_IP_UDP_RTP (1U)
#define ROHC_IP_UDP     (2U)
#define ROHC_IP_ESP     (4U) 
#define ROHC_IP_TCP     (6U)
#define ROHC_ALL_PROFILE    (7U)
#define ROHC_INVALID_PROFILE    (0xFFU)

#define ROHC_INNERIP    (1U)
#define ROHC_OUTERIP    (2U)
/* to check if profile 6 is supported in ics buffer macro is used*/

#define ROHC_IP_TCP_SUPPORTED     (0x10U)

#define ROHC_MAX_FIELD_TYPES       (120U)
#define ROHC_PROF6_MAX_CTRL_FLDS   (20U)


#define MAX_TCP_OPT_SIZE        (40U)
#define ROHC_MAX_ENTRIES_TCP_OPT_IDX     (16U)
#define MAX_OPT_WITH_VAR_LEN     (4U)
#define ROHC_MAX_TCP_GENERIC_OPT    (5U) /* Note: Currently only 2 generic tcp options are known apart from
                                            those which have a reserved index in  rfc 4996 sec 6.3.4 . These
                                            are alternate cksum req and alternate cksum data.*/

#define EOL_LEN_ID            (0U)
#define SACK_LEN_ID           (1U)
#define ALT_CKSUM_DATA_ID     (2U)

/* Standard kinds of TCP options */
#define TCP_EOL_OPT             (0U)
#define TCP_NOP_OPT             (1U)
#define TCP_MSS_OPT             (2U)
#define TCP_WS_OPT              (3U)
#define TCP_SACK_PERMTD_OPT     (4U)
#define TCP_SACK_OPT            (5U)
#define TCP_TS_OPT              (8U)
#define TCP_ALT_CKSUM_REQ_OPT   (14U)
#define TCP_ALT_CKSUM_DATA_OPT  (15U)
/* Macros for length field of standard tcp options */
#define TCP_SACK_PERMTD_OPT_LEN       (2U)
#define TCP_WS_OPT_LEN                (3U)
#define TCP_MSS_OPT_LEN               (4U)
#define TCP_TS_OPT_LEN                (10U)
#define TCP_ALT_CKSUM_REQ_OPT_LEN     (3U)

/* bit masks to check Ext 3 flag in extension 3 to
determine presence absence of extension 3 fields 
based on bits in ext3 flag octet*/
#define ROHC_ENC_EXT3_PKTTYPE                   (0xc0U)                 

#define ROHC_ENC_SN_EXT3FLAGP1_MASK             (0x20U)
#define ROHC_ENC_RTS_EXT3FLAGP1_MASK            (0x10U)
#define ROHC_ENC_TSC_EXT3FLAGP1_MASK            (0x08U)
#define ROHC_ENC_I_EXT3FLAGP1_MASK              (0x04U)
#define ROHC_ENC_INNERIPFLAG_EXT3FLAGP1_MASK    (0x02U)
#define ROHC_ENC_RTPFLAG_EXT3FLAGP1_MASK        (0x01U)
/* for p23 */
#define ROHC_ENC_OUTERIPFLAG_EXT3FLAGP1_MASK    (0x01U)

/* bit masks to check Inner ip flag in extension 3 to 
determine presence absence of fields 
based on bits in inner ip flag octet*/
#define ROHC_ENC_TOS_INNERIPFLAGP1_MASK     (0x80U)
#define ROHC_ENC_TTL_INNERIPFLAGP1_MASK     (0x40U)
#define ROHC_ENC_PROTO_INNERIPFLAGP1_MASK   (0x10U)
#define ROHC_ENC_IPX_INNERIPFLAGP1_MASK     (0x08U)
#define ROHC_ENC_NBO_INNERIPFLAGP1_MASK     (0x04U)
#define ROHC_ENC_RND_INNERIPFLAGP1_MASK     (0x02U)
#define ROHC_ENC_IP2_INNERIPFLAGP1_MASK     (0x01U)

/* mask to check Outer ip flag in extension 3 to
determine presence absence of fields 
based on bits in outer ip flag octet*/
#define ROHC_ENC_TOS_OUTERIPFLAGP1_MASK     (0x80U)
#define ROHC_ENC_TTL_OUTERIPFLAGP1_MASK     (0x40U)
#define ROHC_ENC_PROTO_OUTERIPFLAGP1_MASK   (0x10U)
#define ROHC_ENC_IPX_OUTERIPFLAGP1_MASK     (0x08U)
#define ROHC_ENC_NBO_OUTERIPFLAGP1_MASK     (0x04U)
#define ROHC_ENC_RND_OUTERIPFLAGP1_MASK     (0x02U)
#define ROHC_ENC_I_OUTERIPFLAGP1_MASK       (0x01U)

/* mask to check RTP flag in extension 3 to
determine presence absence of fields 
based on bits in rtp flag octet*/
#define ROHC_ENC_M_RTPFLAGP1_MASK           (0x10U)
#define ROHC_ENC_R_X_RTPFLAGP1_MASK         (0x08U)
#define ROHC_ENC_R_PT_RTPFLAGP1_MASK        (0x20U)
#define ROHC_ENC_CSRC_RTPFLAGP1_MASK        (0x04U)
#define ROHC_ENC_TSS_RTPFLAGP1_MASK         (0x02U)
#define ROHC_ENC_TIS_RTPFLAGP1_MASK         (0x01U)
/****************************************************************/

/*  Mask for RSF change field for profile 6*/
#define ROHC_ENC_RST_FLAG_TCP_FLAGP6_MASK   (0x01U)
#define ROHC_ENC_SYN_FLAG_TCP_FLAGP6_MASK   (0x02U)
#define ROHC_ENC_FIN_FLAG_TCP_FLAGP6_MASK   (0x04U)
/****************************************************************/
/* packet type passed by PRM and used 
as first dimension in rohc_pkt_type array  */
#define ROHC_R0P1_PKTTYPE       1U
#define ROHC_R0CRCP1_PKTTYPE    2U
#define ROHC_UO0P1_PKTTYPE      3U
#define ROHC_R1P1_PKTTYPE       4U
#define ROHC_R1E0P1_PKTTYPE     5U
#define ROHC_R1E1P1_PKTTYPE     6U
#define ROHC_R1E2P1_PKTTYPE     7U
#define ROHC_R1E3P1_PKTTYPE     8U
#define ROHC_R1IDP1_PKTTYPE     9U
#define ROHC_R1IDE0P1_PKTTYPE   10U 
#define ROHC_R1IDE1P1_PKTTYPE   11U
#define ROHC_R1IDE2P1_PKTTYPE   12U
#define ROHC_R1IDE3P1_PKTTYPE   13U
#define ROHC_R1TSP1_PKTTYPE     14U
#define ROHC_R1TSE0P1_PKTTYPE   15U 
#define ROHC_R1TSE1P1_PKTTYPE   16U
#define ROHC_R1TSE2P1_PKTTYPE   17U
#define ROHC_R1TSE3P1_PKTTYPE   18U
#define ROHC_UO1P1_PKTTYPE      19U
#define ROHC_UO1IDP1_PKTTYPE    20U 
#define ROHC_UO1IDE0P1_PKTTYPE  21U
#define ROHC_UO1IDE1P1_PKTTYPE  22U
#define ROHC_UO1IDE2P1_PKTTYPE  23U
#define ROHC_UO1IDE3P1_PKTTYPE  24U
#define ROHC_UO1TSP1_PKTTYPE    25U
#define ROHC_UOR2P1_PKTTYPE     26U
#define ROHC_UOR2E0P1_PKTTYPE   27U
#define ROHC_UOR2E1P1_PKTTYPE   28U
#define ROHC_UOR2E2P1_PKTTYPE   29U
#define ROHC_UOR2E3P1_PKTTYPE   30U
#define ROHC_UOR2IDP1_PKTTYPE   31U
#define ROHC_UOR2IDE0P1_PKTTYPE 32U
#define ROHC_UOR2IDE1P1_PKTTYPE 33U
#define ROHC_UOR2IDE2P1_PKTTYPE 34U
#define ROHC_UOR2IDE3P1_PKTTYPE 35U
#define ROHC_UOR2TSP1_PKTTYPE   36U
#define ROHC_UOR2TSE0P1_PKTTYPE 37U
#define ROHC_UOR2TSE1P1_PKTTYPE 38U
#define ROHC_UOR2TSE2P1_PKTTYPE 39U
#define ROHC_UOR2TSE3P1_PKTTYPE 40U

#define ROHC_R1P23_PKTTYPE              41U
#define ROHC_R1E0P23_PKTTYPE            42U
#define ROHC_R1E1P23_PKTTYPE            43U
#define ROHC_R1E2P23_PKTTYPE            44U
#define ROHC_R1E3P23_PKTTYPE            45U

#define ROHC_UO1P23_PKTTYPE             46U

#define ROHC_UOR2P23_PKTTYPE            47U 
#define ROHC_UOR2E0P23_PKTTYPE          48U
#define ROHC_UOR2E1P23_PKTTYPE          49U
#define ROHC_UOR2E2P23_PKTTYPE          50U
#define ROHC_UOR2E3P23_PKTTYPE          51U

#define ROHC_CO_P6_COMMON_PKTYPE        52U
#define ROHC_CO_P6_RND1_PKTYPE          53U
#define ROHC_CO_P6_RND2_PKTYPE          54U
#define ROHC_CO_P6_RND3_PKTYPE          55U
#define ROHC_CO_P6_RND4_PKTYPE          56U
#define ROHC_CO_P6_RND5_PKTYPE          57U
#define ROHC_CO_P6_RND6_PKTYPE          58U
#define ROHC_CO_P6_RND7_PKTYPE          59U
#define ROHC_CO_P6_RND8_PKTYPE          60U
#define ROHC_CO_P6_SEQ1_PKTYPE          61U
#define ROHC_CO_P6_SEQ2_PKTYPE          62U
#define ROHC_CO_P6_SEQ3_PKTYPE          63U
#define ROHC_CO_P6_SEQ4_PKTYPE          64U
#define ROHC_CO_P6_SEQ5_PKTYPE          65U
#define ROHC_CO_P6_SEQ6_PKTYPE          66U
#define ROHC_CO_P6_SEQ7_PKTYPE          67U
#define ROHC_CO_P6_SEQ8_PKTYPE          68U

#define ROHC_IRP0_PKTYPE                69U
#define ROHC_NORMALP0_PKTYPE            70U


#define ROHC_IR_WITHOUT_DYN_PKTTYPE     71U 
#define ROHC_IR_WITH_DYN_PKTTYPE        72U 
#define ROHC_IRDYN_PKTTYPE              73U
#define ROHC_IRP6CR_PKTYPE              74U


#define ROHC_MAX_PKT_TYPES              (ROHC_IRP6CR_PKTYPE + 1)



/*IRREGULAR CHAIN TYPE IDENTIFIERS */
#define ROHC_P6_NO_IRREG_CHAIN                       0U
#define ROHC_P6_IPV6_OUTER_WITHOUT_TTL_IRREG_CHAIN   1U
#define ROHC_P6_IPV6_OUTER_WITH_TTL_IRREG_CHAIN      2U
#define ROHC_P6_IPV6_INNERMOST_IRREG_CHAIN           3U
#define ROHC_P6_IPV4_OUTER_WITHOUT_TTL_IRREG_CHAIN   4U
#define ROHC_P6_IPV4_OUTER_WITH_TTL_IRREG_CHAIN      5U
#define ROHC_P6_IPV4_INNERMOST_IRREG_CHAIN           6U
#define ROHC_P6_TCP_IRREG_CHAIN                      7U
#define ROHC_P6_TCP_OPTION_IRREG_CHAIN               8U
#define ROHC_MAX_IRREG_CHAINS                        (ROHC_P6_TCP_OPTION_IRREG_CHAIN+1)

#define ROHC_MAX_NO_OF_FIELDS_IN_A_IRREG_CHAIN       20U


/* MASK for ROHC Profile 6 packet types */
/* First Nibble of discriminator for RND6 and SEQ1 is 1010 */
#define ROHC_P6_RND6_SEQ1_MASK          0xA0U   
/* First Nibble of discriminator for RND1, RND7, RND8 and SEQ8 is 1011 */
#define ROHC_P6_RND_1_7_8_SEQ8_MASK     0xB0U   
/* First Nibble of discriminator for RND2 and SEQ7 is 1100 */
#define ROHC_P6_RND2_SEQ7_MASK          0xC0U   
/* First Nibble of discriminator for RND4, SEQ2 and SEQ6  is 1101 */
#define ROHC_P6_RND4_SEQ_2_6_MASK       0xD0U   
/* First 3 bits of discriminator for RND5, SEQ3 and SEQ5 is 100 */
#define ROHC_P6_RND5_SEQ_5_MASK         0x80U 
#define ROHC_P6_RND5_SEQ_3_MASK         0x90U 
#define ROHC_TS_VAL_7_DESC_MASK		    0x80U   
#define ROHC_TS_VAL_14_DESC_MASK	    0xC0U

#define ROHC_TS_7_NUM_BITS		0x7fU
#define ROHC_TS_14_NUM_BITS		0x3fU
#define ROHC_TS_21_29_NUM_BITS		0x1fU

#define ROHC_SACK_UNCHANGED_DESC	0x00U
#define ROHC_SACK_0_DISC		 0x00U
#define ROHC_SACK_1_DISC		 0x01U
#define ROHC_SACK_2_DISC		 0x02U
#define ROHC_SACK_3_DISC		 0x03U
#define ROHC_SACK_4_DISC	     0x04U

#define ROHC_TS_VAL_7_DISC		0x00U
#define ROHC_TS_VAL_14_DISC		0x80U
#define ROHC_TS_VAL_21_DISC		0xC0U
#define ROHC_TS_VAL_21_29_DISC		0xE0U

#define ROHC_P6_8_BITS           8
#define ROHC_P6_16_BITS         16
#define ROHC_P6_32_BITS         32




/****************************************************************/
/* field id returned by rohc_pkt_type array 
used to index field_value , field_length arrays in 
global and  local context */

/*Field ID #Value Constant for all packets in all streams : 
read value of field from global array */
#define ROHC_R0_FID     0U
#define ROHC_R0CRC_FID  1U
#define ROHC_UO0_FID    2U
#define ROHC_R1_FID     3U
#define ROHC_R1ID_FID   4U
#define ROHC_R1TS_FID   5U
#define ROHC_UO1_FID    6U
#define ROHC_UO1ID_FID  7U  
#define ROHC_UO1TS_FID  8U
#define ROHC_UOR2_FID   9U
#define ROHC_UOR2ID_FID 10U
#define ROHC_UOR2T1_FID 11U
#define ROHC_T0_FID     12U
#define ROHC_T1_FID     13U
#define ROHC_EXT0_FID   14U
#define ROHC_EXT1_FID   15U
#define ROHC_EXT2_FID   16U
#define ROHC_EXT3_FID   17U
#define ROHC_TCP_DISC_FID   18U
#define ROHC_RND1_FID   19U
#define ROHC_RND2_FID   20U
#define ROHC_RND3_FID   21U
#define ROHC_RND4_FID   22U
#define ROHC_RND5_FID   23U
#define ROHC_RND6_FID   24U
#define ROHC_RND7_FID   25U
#define ROHC_RND8_FID   26U
#define ROHC_COMMON_FID   27U

#define ROHC_MAX_CONSTANT_FID 33U

/*Field ID #Value variable for packets read value 
of field from local array in packet stream context*/
/* For Prof6, decreasing ROHC_VAR_FID_BASE from 50 to 40 to increase size of
 * field array*/
#define ROHC_VAR_FID_BASE       40U
#define ROHC_INNERIP_ID_FID     40U
#define ROHC_EXT_FID            41U
#define ROHC_SN_FID             42U
#define ROHC_CRC_FID            43U
#define ROHC_EXT3_FLAGS_FID     44U 
#define ROHC_INNERIP_FLAGS_FID  45U
#define ROHC_OUTERIP_FLAGS_FID  46U
#define ROHC_TS_FID             47U
#define ROHC_INNERIP4_TOS_FID   48U
#define ROHC_INNERIP6_TCLASS_FID    ROHC_INNERIP4_TOS_FID   
#define ROHC_INNERIP4_TTL_FID       49U
#define ROHC_INNERIP6_HLIM_FID      ROHC_INNERIP4_TTL_FID
#define ROHC_INNERIP4_PROTO_FID     50U
#define ROHC_INNERIP6_NHEADER_FID   ROHC_INNERIP4_PROTO_FID 
    
#define ROHC_INNERIP_EXTHEADERS_LIST_FID    51U
#define ROHC_OUTERIP4_TOS_FID               52U
#define ROHC_OUTERIP6_TCLASS_FID     ROHC_OUTERIP4_TOS_FID
#define ROHC_OUTERIP4_TTL_FID               53U
#define ROHC_OUTERIP6_HLIM_FID  ROHC_OUTERIP4_TTL_FID
#define ROHC_OUTERIP4_PROTO_FID             54U
#define ROHC_OUTERIP6_NHEADER_FID    ROHC_OUTERIP4_PROTO_FID
#define ROHC_OUTERIP_EXTHEADERS_LIST_FID    55U
#define ROHC_OUTERIP_ID_FID                 56U
#define ROHC_RTP_FLAGS_FID                  57U
#define ROHC_RTP_R_P_FID                    58U
#define ROHC_RTP_PT_FID                     59U
#define ROHC_RTP_CSRC_LIST_FID              60U
#define ROHC_RTP_TS_STRIDE_FID              61U
#define ROHC_RTP_TIME_STRIDE_FID            62U
#define ROHC_CID_FID                        63U
#define ROHC_INNERIP4_SRC_FID               64U
#define ROHC_INNERIP4_DST_FID               65U
#define ROHC_OUTERIP4_SRC_FID               66U
#define ROHC_OUTERIP4_DST_FID               67U
#define ROHC_INNERIP6_FLOWLABEL_FID         68U
#define ROHC_OUTERIP6_FLOWLABEL_FID         69U
#define ROHC_UDP_SRC_FID                    70U 
#define ROHC_UDP_DST_FID                    71U
#define ROHC_RTP_SSRC_FID                   72U
#define ROHC_ESP_SPI_FID                    73U
#define ROHC_INNERIP_DF_FID                 74U 
#define ROHC_OUTERIP_DF_FID                 75U 
#define ROHC_UDP_CKSUM_FID                  76U
#define ROHC_RTP_CC_FID                     77U
#define ROHC_RTP_M_FID                      78U
#define ROHC_RTP_X_FID                      79U
#define ROHC_MODE_FID                       80U
#define ROHC_RTP_TSS_FID                    81U
#define ROHC_RTP_TIS_FID                    82U
/* Reusing indexes 48,52 as these fields are not present as such for Prof6*/
#define ROHC_INNERIP4_DSCP_FID              48U
#define ROHC_INNERIP4_ECN_FID               52U
/* FID 57 - 62  is specific to prof1 so reusing this index for Prof6*/
#define ROHC_TCP_SRC_PORT_FID               57U
#define ROHC_TCP_DST_PORT_FID               58U
#define ROHC_TCP_WINDOW_FID                 59U
#define ROHC_TCP_CKSUM_FID                  60U
#define ROHC_TCP_MSN_FID                    61U
#define ROHC_TCP_ECN_USED_FID               62U
/* FID 70 - 72  is specific to prof1 so reusing this index for Prof6*/
#define ROHC_TCP_ACK_STRIDE_FLAG_FID        70U
#define ROHC_TCP_ACK_ZERO_FID               71U
#define ROHC_TCP_URP_ZERO_FID               72U
/* FID 76 - 82  is specific to prof1 so reusing this index for Prof6*/
#define ROHC_TCP_RES_FLAG_FID               76U
#define ROHC_TCP_ECN_FLAG_FID               77U
#define ROHC_TCP_URG_FLAG_FID               78U
#define ROHC_TCP_ACK_FLAG_FID               79U
#define ROHC_TCP_PSH_FLAG_FID               80U
#define ROHC_TCP_RST_FLAG_FID               81U
#define ROHC_TCP_SYN_FLAG_FID               82U
#define ROHC_TCP_FIN_FLAG_FID               83U
#define ROHC_TCP_URG_PTR_FID                84U
#define ROHC_TCP_ACK_STRIDE_FID             85U
#define ROHC_TCP_OPTIONS_FID                86U
#define ROHC_INNERIP_BEH_FID                87U 
#define ROHC_TCP_SN_STRIDE_FID              88U
#define ROHC_TCP_SCALED_ACKNO_FID           89U
#define ROHC_TCP_SCALED_SN_FID              90U
#define ROHC_TCP_TS_STRIDE_FID              91U
#define ROHC_TCP_TS_ECHO_FID                92U
#define ROHC_TCP_OPT_LIST_FID               93U
/* Added FID to Decode SACK Block*/
#define ROHC_TCP_SACK_BLOCK_START_1         94U
#define ROHC_TCP_SACK_BLOCK_START_2         95U
#define ROHC_TCP_SACK_BLOCK_START_3         96U
#define ROHC_TCP_SACK_BLOCK_START_4         97U
#define ROHC_TCP_SACK_BLOCK_END_1           98U
#define ROHC_TCP_SACK_BLOCK_END_2           99U
#define ROHC_TCP_SACK_BLOCK_END_3           100U
#define ROHC_TCP_SACK_BLOCK_END_4           101U
#define ROHC_TCP_ACKNO_FID                  102U


/************FID's Created For Common Compressed Packet***********/
/* upto 99 indexes are allowed here*/
#define ROHC_INVALID_FID                    0xffU

/****************************************************************/
#define ROHC_MAX_NO_OF_FIELDS_IN_A_PACKET   (69U)

#define ROHC_MAX_PKT_SIZE                   (300U)
/****************************************************************/

/* Follwing are the Prof6 specific Control FIDs */
/* Note- These FIDs are part of array ctrl_field_val_p6 in csc for prof6 */
#define ROHC_PROF6_VAR_CTRL_FID_BASE       201U
#define ROHC_TCP_RSF_FLAGS_FID             201U
#define ROHC_TCP_URG_PTR_PRESENT_FID       202U
#define ROHC_INNERIP4_DSCP_PRESENT_FID     203U
#define ROHC_INNERIP4_TTL_HPL_PRESENT_FID  204U
#define ROHC_TTL_HOPL_OUTER_FLAG_FID       205U
#define ROHC_TCP_SEQ_IND_FID               206U
#define ROHC_TCP_ACK_IND_FID               207U
#define ROHC_TCP_IPID_IND_FID              208U
#define ROHC_TCP_WINDOW_IND_FID            209U
#define ROHC_TCP_RESERVED_FID              210U
#define ROHC_TCP_LIST_PRESENT_FID          211U
#define ROHC_PROF6_MAX_CTRL_FID            221U

/****************************************************************/
/* Compressor states for profile 1,2 and 3 */
#define ROHC_C_ST_IR                    1U
#define ROHC_C_ST_FO                    2U
#define ROHC_C_ST_SO                    3U

/* Decompressor states for profile 1,2 and 3 */
#define ROHC_D_ST_NC                    4U
#define ROHC_D_ST_SC                    5U
#define ROHC_D_ST_FC                    6U

/* Compressor states for uncompressed profile */
#define ROHC_CUN_ST_IR                  7U
#define ROHC_CUN_ST_NORMAL              8U

/* decompressor states for uncompressed profile */
#define ROHC_DUN_ST_NC                  9U
#define ROHC_DUN_ST_FC                  10U

/* Compressor transition states for profile 1,2 and 3**/
#define ROHC_C_TST_DONE                 11U
#define ROHC_C_TST_PENDING              12U

/* Decompressor transition states for profile 1,2 and 3*/
#define ROHC_D_TST_DONE                 13U
#define ROHC_D_TST_TCP                  ROHC_D_TST_DONE
#define ROHC_D_TST_INITIATED            14U
#define ROHC_D_TST_PENDING              15U
/* ROHC modes */
#define ROHC_MODE_U                     1U
#define ROHC_MODE_TCP                   ROHC_MODE_U
#define ROHC_MODE_O                     2U
#define ROHC_MODE_R                     3U

#define ROHC_MAX_MODE_TYPES         (ROHC_MODE_R + 1)

/* Initial value for static info update counter */
#define DEFAULT_STATIC_INFO_UPDATE_COUNTER (00x0)

/* Initial value for dynamic info update counter */
#define DEFAULT_DYNAMIC_INFO_UPDATE_COUNTER (00x0)

/* Feedback option type */

#define ROHC_FBO_CRC            1U
#define ROHC_FBO_REJECT         2U
#define ROHC_FBO_SN_NOT_VALID   3U
#define ROHC_FBO_SN             4U
#define ROHC_FBO_CLOCK          5U
#define ROHC_FBO_JITTER         6U
#define ROHC_FBO_LOSS           7U
#define ROHC_FBO_CONTEXT_MEM    9U
#define ROHC_ACK                0U
#define ROHC_NACK               1U
#define ROHC_STATIC_NACK        2U
#define ROHC_FB_1               1U
#define ROHC_FB_2               2U
#define ROHC_FBO_MSN_NOT_VALID   ROHC_FBO_SN_NOT_VALID
#define ROHC_FBO_MSN             ROHC_FBO_SN


/* The max cid value for small cids for the entity */
#define ROHC_MAX_CID    (0xF)


#define ROHC_MT_NOT_REQ 0x00
#define ROHC_CLK_RES_PRESENT 0x01
#define ROHC_JIT_PRESENT 0x02

/* for RND */
#define ROHC_RND_ABSENT     0U
#define ROHC_RND_PRESENT    1U
#define ROHC_RND_INVALID    2U

#define ROHC_MAX_RND_TYPES      (ROHC_RND_INVALID +1)

#define ROHC_IHL_VAL    (5U)
#define IPV4            (4U)
#define IPV6            (6U)
#define IP_ID_BEHAVIOR_SEQUENTIAL         0U
#define IP_ID_BEHAVIOR_SEQUENTIAL_SWAPPED 1U
#define IP_ID_BEHAVIOR_RANDOM             2U
#define IP_ID_BEHAVIOR_ZERO               3U
#define ROHC_TOTAL_FID_LEN (ROHC_MAX_FIELD_TYPES - ROHC_VAR_FID_BASE)
#define ROHC_TCP_CTRL_FID_BASE (ROHC_PROF6_VAR_CTRL_FID_BASE -ROHC_TOTAL_FID_LEN )
/*****************************************************/
#endif /* _ROHC_COM_DEF_H_ */
