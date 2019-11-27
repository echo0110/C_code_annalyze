/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_cprm_rslv_p1.c,v 1.3.36.1 2010/11/09 05:24:38 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the function definition for determining
 *                     the packet type for profile 1 (IP/UDP/RTP).
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_cprm_rslv_p1.c,v $
 * Revision 1.3.36.1  2010/11/09 05:24:38  gur22059
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
#include "rohc_cprm_proto.h"
#include "rohc_cprm_mcr.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/
/* + SPR 17439 */
 static rohc_U32bit_t rohc_cprm_delta_inner_ip4 ROHC_PROTO
      ((
       rohc_csc_rtp_t          *p_csc      ,
       rohc_encoded_value_t    *en_ip_id ));

/* +- SPR 17777 */
 static rohc_U32bit_t rohc_cprm_delta_inner_ip6
      ( rohc_csc_rtp_t          *p_csc);

 static rohc_U32bit_t rohc_cprm_delta_outer_ip4(
       rohc_csc_rtp_t          *p_csc);

 static rohc_U32bit_t rohc_cprm_delta_outer_ip6
       (rohc_csc_rtp_t          *p_csc);
/* +- SPR 17777 */
/* - SPR 17439 */
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
extern rohc_U8bit_t rohc_p1_r_fo_pkt_typ_1[9];
extern rohc_U8bit_t rohc_p1_r_fo_pkt_typ_2[9];
extern rohc_U8bit_t rohc_p1_r_bothrand1_or_both6[9];
extern rohc_U8bit_t rohc_p1_r_bothrand1_or_both6_else[9];
extern rohc_U8bit_t rohc_p1_r_crc_0[9];
extern rohc_U8bit_t rohc_p1_r_crc_7[9] ;
extern rohc_U8bit_t rohc_p1_r_ext_3[9] ;
extern rohc_U8bit_t rohc_p1_r_mt_pending[9];
extern rohc_U8bit_t rohc_p1_uo_fo_pkt_typ[7];
extern rohc_U8bit_t rohc_p1_uo_bothrand1_or_both6[7];
extern rohc_U8bit_t rohc_p1_uo_bothrand1_or_both6_else[7];
extern rohc_U8bit_t rohc_p1_uo_ext_3[7];
extern rohc_U8bit_t rohc_p1_uo_crc_7[7] ;
extern rohc_U8bit_t rohc_p1_uo_crc_3[7] ;                               
extern rohc_U8bit_t rohc_p1_uo_mt_pending[7] ;


extern rohc_p1_pkt_typ_info_t 
rohc_p1_pkt_typ_info_uo[ROHC_P1_UO_MAX_PACKETS];

extern rohc_p1_pkt_typ_info_t
rohc_p1_pkt_typ_info_r[ROHC_P1_R_MAX_PACKETS];

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/



/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_p1_resolve_pkt_typ_uo
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

/* +- SPR 17777 */
rohc_U8bit_t rohc_cprm_p1_resolve_pkt_typ_uo ROHC_PARAMS(
            (p_csc, delta_bit_map, en_sn,
                 en_ts, en_ip_id, no_crc_bit),
                    rohc_csc_rtp_t          *p_csc          _AND_
                    rohc_U32bit_t           delta_bit_map   _AND_
                    rohc_encoded_value_t    *en_sn          _AND_
                    rohc_encoded_value_t    *en_ts          _AND_
                    rohc_encoded_value_t    *en_ip_id       _AND_
                    rohc_U8bit_t            no_crc_bit)
{ 
    rohc_U8bit_t pkt_bit_map[ROHC_BIT_MAP_LEN7];
    rohc_U8bit_t pkt_bit_map_no_ext3[ROHC_BIT_MAP_LEN7];
    rohc_U8bit_t pkt_type = ROHC_UNKNOWN_PKT;

    ROHC_ENTER_FN("rohc_cprm_p1_resolve_pkt_typ_uo");

    ROHC_CPRM_BIT_INIT(pkt_bit_map,rohc_p1_uo_fo_pkt_typ,ROHC_BIT_MAP_LEN7);

    if (no_crc_bit == ROHC_PRM_CRC_BITS3)
    {
        ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p1_uo_crc_3,ROHC_BIT_MAP_LEN7);
    }
    else if (no_crc_bit  == ROHC_BIT_MAP_LEN7)
    {
        ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p1_uo_crc_7, ROHC_BIT_MAP_LEN7);
    }
    else
    {
        /*en_sn->bits = ROHC_P1_SN_BITS;*/
        /*en_ts->bits = ROHC_P1_TS_BITS ;*/
        /*en_ip_id->bits = ROHC_P1_SN_BITS ;*/
        ROHC_EXIT_FN("rohc_cprm_p1_resolve_pkt_typ_uo");
        return ROHC_UNKNOWN_PKT;
    }   

    if( ((p_csc->p_cpib->inner_ip4) && !(ROHC_CSC_GET_NEW_INNER_RND(p_csc))) ||
            ((p_csc->p_cpib->outer_ip4) && !(ROHC_CSC_GET_NEW_OUTER_RND(p_csc))) )
    {
        ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p1_uo_bothrand1_or_both6_else, 
                ROHC_BIT_MAP_LEN7);
    }   
    else
    {
        ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p1_uo_bothrand1_or_both6, 
                ROHC_BIT_MAP_LEN7);
    }

    if(ROHC_CSC_GET_TRANS_STATE(p_csc) == ROHC_C_TST_PENDING )
    {
        ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p1_uo_mt_pending, ROHC_BIT_MAP_LEN7);
    }   

    /* ROHC_CPRM_BIT_SEL_PKT(pkt_bit_map, ROHC_BIT_MAP_LEN7 , sel_pkt_typ , num_pkt);*/

    /* if any other bit except marker is on , ext-3 is required */
    if(((delta_bit_map & 
                    (~(ROHC_CPRM_CH_R_M| ROHC_CPRM_CH_I | ROHC_CPRM_CH_I2)) )!= 0 )||
            (((p_csc->p_cpib->inner_ip4) && !(ROHC_CSC_GET_NEW_INNER_RND(p_csc))) &&
             ((p_csc->p_cpib->outer_ip4) && !(ROHC_CSC_GET_NEW_OUTER_RND(p_csc)))))


    {
        ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p1_uo_ext_3 , ROHC_BIT_MAP_LEN7);
        /* SPR 3532 Klocwork fix */
        /* Cyclomatic changes Start */
        pkt_type = rohc_cprm_p1_resolve_pkt_bmp_uo (en_sn, en_ts, en_ip_id, pkt_bit_map);
        if (ROHC_UNKNOWN_PKT != pkt_type)
        {
            p_csc->field_value[ROHC_EXT_FID -ROHC_VAR_FID_BASE]=1;

            ROHC_EXIT_FN("rohc_cprm_p1_resolve_pkt_typ_uo");
            return pkt_type;
        }
        /* Cyclomatic changes End */
    } 
    else
    {
        ROHC_CPRM_KEEP_REMOVE(pkt_bit_map_no_ext3,pkt_bit_map,
                rohc_p1_uo_ext_3,ROHC_BIT_MAP_LEN7);
        /* SPR 3532 klocwork fix */ 
        /* Cyclomatic changes Start */
        pkt_type = rohc_cprm_p1_resolve_pkt_bmp_uo (en_sn, en_ts, en_ip_id, pkt_bit_map_no_ext3);
        if (ROHC_UNKNOWN_PKT != pkt_type)
        {
            p_csc->field_value[ROHC_EXT_FID -ROHC_VAR_FID_BASE]=0;
            ROHC_EXIT_FN("rohc_cprm_p1_resolve_pkt_typ_uo");
            return pkt_type;
        }
        pkt_type = rohc_cprm_p1_resolve_pkt_bmp_uo (en_sn, en_ts, en_ip_id, pkt_bit_map);
        if (ROHC_UNKNOWN_PKT != pkt_type)
        {
            p_csc->field_value[ROHC_EXT_FID -ROHC_VAR_FID_BASE]=1;
            ROHC_EXIT_FN("rohc_cprm_p1_resolve_pkt_typ_uo");
            return pkt_type;
        }
        /* Cyclomatic changes End */

    }
    /* en_sn->bits = ROHC_P1_SN_BITS;*/
    /*en_ts->bits = ROHC_P1_TS_BITS ;*/
    /*en_ip_id->bits = ROHC_P1_SN_BITS ;*/
    ROHC_EXIT_FN("rohc_cprm_p1_resolve_pkt_typ_uo");
    return ROHC_UNKNOWN_PKT;     
}

/*****************************************************************************/

/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_p1_resolve_pkt_typ_r
******************************************************************************
**
**      DESCRIPTION:
**              This function determines the packet type for profile 1,
**              when compressor is operated in R mode
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
/* +- SPR 17777 */
rohc_U8bit_t rohc_cprm_p1_resolve_pkt_typ_r ROHC_PARAMS(
            (p_csc, delta_bit_map, en_sn,
                 en_ts, en_ip_id, no_crc_bit),
                    rohc_csc_rtp_t          *p_csc          _AND_
                    rohc_U32bit_t           delta_bit_map   _AND_
                    rohc_encoded_value_t    *en_sn          _AND_
                    rohc_encoded_value_t    *en_ts          _AND_
                    rohc_encoded_value_t    *en_ip_id       _AND_
                    rohc_U8bit_t            no_crc_bit)
{
    rohc_U8bit_t pkt_bit_map[ROHC_BIT_MAP_LEN9];
    rohc_U8bit_t pkt_bit_map_no_ext3[ROHC_BIT_MAP_LEN9];

    rohc_U8bit_t pkt_type = ROHC_UNKNOWN_PKT;


    ROHC_ENTER_FN("rohc_cprm_p1_resolve_pkt_typ_r");
    if (no_crc_bit == ROHC_PRM_CRC_BITS7)
    {
        ROHC_CPRM_BIT_INIT(pkt_bit_map, rohc_p1_r_fo_pkt_typ_2, 
                ROHC_BIT_MAP_LEN9);
        ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p1_r_crc_7, 
                ROHC_BIT_MAP_LEN9);
    }
    else if(no_crc_bit == ROHC_PRM_CRC_BITS0) 
    {
        ROHC_CPRM_BIT_INIT(pkt_bit_map, rohc_p1_r_fo_pkt_typ_1,
                ROHC_BIT_MAP_LEN9);
        ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p1_r_crc_0, 
                ROHC_BIT_MAP_LEN9);
    }
    else
    {

        ROHC_EXIT_FN("rohc_cprm_p1_resolve_pkt_typ_r");
        return ROHC_UNKNOWN_PKT;
    }

    if( ((p_csc->p_cpib->inner_ip4) && !(ROHC_CSC_GET_NEW_INNER_RND(p_csc))) ||
            ((p_csc->p_cpib->outer_ip4) && !(ROHC_CSC_GET_NEW_OUTER_RND(p_csc))) )
    {
        ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p1_r_bothrand1_or_both6_else, 
                ROHC_BIT_MAP_LEN9);
    }   
    else
    {
        ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p1_r_bothrand1_or_both6, 
                ROHC_BIT_MAP_LEN9);
    }

    if(ROHC_CSC_GET_TRANS_STATE(p_csc) == ROHC_C_TST_PENDING )
    {
        ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p1_r_mt_pending, ROHC_BIT_MAP_LEN9);
    }   

    /*ROHC_CPRM_BIT_SEL_PKT(pkt_bit_map, ROHC_BIT_MAP_LEN7,
      sel_pkt_typ, num_pkt);*/

    /* if any other bit except marker is on , ext-3 is required */
    if((delta_bit_map & 
                (~(ROHC_CPRM_CH_R_M| ROHC_CPRM_CH_I | ROHC_CPRM_CH_I2)) )!= 0 )
    {
        ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p1_r_ext_3 , ROHC_BIT_MAP_LEN9);
        /* SPR 3532 - klocwork fix */
        /* Cyclomatic changes Start */
        pkt_type = rohc_cprm_p1_resolve_pkt_bmp_r (en_sn, en_ts, en_ip_id, pkt_bit_map);
        if (ROHC_UNKNOWN_PKT != pkt_type)
        {
            p_csc->field_value[ROHC_EXT_FID -ROHC_VAR_FID_BASE]=1;
            ROHC_EXIT_FN("rohc_cprm_p1_resolve_pkt_typ_r");
            return pkt_type;
        }
        /* Cyclomatic changes End */
    }
    else
    {
        ROHC_CPRM_KEEP_REMOVE(pkt_bit_map_no_ext3,pkt_bit_map,
                rohc_p1_r_ext_3,ROHC_BIT_MAP_LEN9);
        /* SPR 3532 - klocwork fix */
        /* Cyclomatic changes Start */
        pkt_type = rohc_cprm_p1_resolve_pkt_bmp_r (en_sn, en_ts, en_ip_id, pkt_bit_map_no_ext3);
        if (ROHC_UNKNOWN_PKT != pkt_type)
        {
            p_csc->field_value[ROHC_EXT_FID -ROHC_VAR_FID_BASE]=0;

            ROHC_EXIT_FN("rohc_cprm_p1_resolve_pkt_typ_r");
            return pkt_type;
        }
        pkt_type = rohc_cprm_p1_resolve_pkt_bmp_r (en_sn, en_ts, en_ip_id, pkt_bit_map);
        if (ROHC_UNKNOWN_PKT != pkt_type)
        {
            p_csc->field_value[ROHC_EXT_FID -ROHC_VAR_FID_BASE]=1;

            ROHC_EXIT_FN("rohc_cprm_p1_resolve_pkt_typ_r");
            return pkt_type;
        }
        /* Cyclomatic changes End */
    }
    ROHC_EXIT_FN("rohc_cprm_p1_resolve_pkt_typ_r");
    return ROHC_UNKNOWN_PKT;     
}

/* Cyclomatic changes Start */
/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_p1_resolve_pkt_bmp_uo
******************************************************************************
**
**      DESCRIPTION:
**              This function determines the packet type for profile 1,
**              when compressor is operated in UO mode from packet bitmap
**      ARGUMENTS:
**              en_sn   [IN]      Encoded value of sn
**              en_ts   [IN]      Encoded value of ts
**              en_ip_id[IN]      Encoded value of ip_id
**              pkt_bit_map[IN}   Packet bitmap    
**      RETURN VALUE:
**              Rohc packet type
**      NOTES:
**               
*****************************************************************************/
rohc_U8bit_t rohc_cprm_p1_resolve_pkt_bmp_uo (
                    rohc_encoded_value_t    *en_sn,
                    rohc_encoded_value_t    *en_ts,
                    rohc_encoded_value_t    *en_ip_id,
                    rohc_U8bit_t            pkt_bit_map[])
{
    rohc_U8bit_t i = 0;

    ROHC_ENTER_FN("rohc_cprm_p1_resolve_pkt_bmp_uo");

    for(i = 0 ; ((i <8*ROHC_BIT_MAP_LEN7)&&(i<ROHC_P1_UO_MAX_PACKETS)); i++)
    {
        if((ROHC_IS_ITH_BIT_SET(pkt_bit_map,i)) &&
                (rohc_p1_pkt_typ_info_uo[i].sn_bits >=  en_sn->bits) &&
                (rohc_p1_pkt_typ_info_uo[i].ts_bits >= en_ts->bits) &&
                (rohc_p1_pkt_typ_info_uo[i].ip_id_bits >= en_ip_id->bits))
        {
            en_sn->bits = rohc_p1_pkt_typ_info_uo[i].sn_bits;
            en_ts->bits = rohc_p1_pkt_typ_info_uo[i].ts_bits;
            en_ip_id->bits = rohc_p1_pkt_typ_info_uo[i].ip_id_bits;

            ROHC_EXIT_FN("rohc_cprm_p1_resolve_pkt_typ_uo");
            return rohc_p1_pkt_typ_info_uo[i].pbm_pkt_typ;
        }
    }
    ROHC_EXIT_FN("rohc_cprm_p1_resolve_pkt_bmp_uo");
    return ROHC_UNKNOWN_PKT;
}

/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_p1_resolve_pkt_bmp_r
******************************************************************************
**
**      DESCRIPTION:
**              This function determines the packet type for profile 1,
**              when compressor is operated in R mode from packet bitmap
**      ARGUMENTS:
**              en_sn   [IN]      Encoded value of sn
**              en_ts   [IN]      Encoded value of ts
**              en_ip_id[IN]      Encoded value of ip_id
**              pkt_bit_map[IN}   Packet bitmap    
**      RETURN VALUE:
**              Rohc packet type
**      NOTES:
**               
*****************************************************************************/
rohc_U8bit_t rohc_cprm_p1_resolve_pkt_bmp_r (
                    rohc_encoded_value_t    *en_sn,
                    rohc_encoded_value_t    *en_ts,
                    rohc_encoded_value_t    *en_ip_id,
                    rohc_U8bit_t            pkt_bit_map[])
{
    rohc_U8bit_t i = 0;

    ROHC_ENTER_FN("rohc_cprm_p1_resolve_pkt_bmp_r");

    for(i = 0 ; ((i<8*ROHC_BIT_MAP_LEN9) && (i<ROHC_P1_R_MAX_PACKETS)); i++)
    {
        if((ROHC_IS_ITH_BIT_SET(pkt_bit_map,i)) &&
                rohc_p1_pkt_typ_info_r[i].sn_bits >=  en_sn->bits && 
                rohc_p1_pkt_typ_info_r[i].ts_bits >= en_ts->bits && 
                rohc_p1_pkt_typ_info_r[i].ip_id_bits >= en_ip_id->bits)
        {
            en_sn->bits = rohc_p1_pkt_typ_info_r[i].sn_bits;
            en_ts->bits = rohc_p1_pkt_typ_info_r[i].ts_bits;
            en_ip_id->bits = rohc_p1_pkt_typ_info_r[i].ip_id_bits;

            ROHC_EXIT_FN("rohc_cprm_p1_resolve_pkt_bmp_r");
            return rohc_p1_pkt_typ_info_r[i].pbm_pkt_typ;
        }
    }
    ROHC_EXIT_FN("rohc_cprm_p1_resolve_pkt_bmp_r");
    return ROHC_UNKNOWN_PKT;
}

/* Cyclomatic changes End */

/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_p1_compute_delta
******************************************************************************
**
**      DESCRIPTION:
**              Computes the delta bit field for profile p1
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**              en_sn   [IN]      Encoded value of sn
**              en_ts   [IN]      Encoded value of ts
**              en_ip_id[IN]      Encoded value of ip_id
**      RETURN VALUE:
**              Delta bit map field
**      NOTES:
**               
*****************************************************************************/
/* +- SPR 17777 */
rohc_U32bit_t rohc_cprm_p1_compute_delta ROHC_PARAMS 
    ((p_csc,en_sn,en_ip_id) ,
    rohc_csc_rtp_t          *p_csc      _AND_
    rohc_encoded_value_t    *en_sn      _AND_
    rohc_encoded_value_t    *en_ip_id )
{
    rohc_U32bit_t ret_value = 0;
    rohc_U32bit_t return_bitmap = 0;
    ROHC_ENTER_FN("rohc_cprm_p1_compute_delta");
    p_csc->field_value[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE]=0;
    p_csc->field_value[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE]=0;
    p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE]=0;
    p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE]=0;

    p_csc->field_value[ROHC_SN_FID-ROHC_VAR_FID_BASE] = en_sn->value;

    /* Fix for SPR#6563 */ 
    /* if(ROHC_CPRM_IS_CH_MODE(p_csc)) */
    //{
    ret_value |= ROHC_CPRM_CH_MODE;
    p_csc->field_value[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE] |= 
        (p_csc->c_mode<<6);
    /*} */
    /* Fix for SPR#6563 */ 
    /*  if(ROHC_CPRM_IS_CH_TS(p_csc)) */
    /* en_ts->bits != 0*/
    /*  {
        ret_value |= ROHC_CPRM_CH_TS;
        p_csc->field_value[ROHC_TS_FID-ROHC_VAR_FID_BASE] = en_ts->value;
        p_csc->field_len[ROHC_TS_FID-ROHC_VAR_FID_BASE] = en_ts->bits;
        }
     */
    if(ROHC_CPRM_IS_CH_R_PT(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_R_PT;
        p_csc->field_value[ROHC_RTP_PT_FID-ROHC_VAR_FID_BASE] =  
            ( ((rohc_rtp_t*)p_csc->p_cpib->rtp)->m_pt & ROHC_PT_MASK )  ;
        p_csc->field_value[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_R_PT_RTPFLAGP1_MASK ;
    }
    /*Start SPR 6197 Fix*/
    /*Need to check if M bit is set in the uncompressed header*/
    if  (((rohc_rtp_t*)p_csc->p_cpib->rtp)->m_pt & ROHC_M_MASK)
    {
        ret_value |= ROHC_CPRM_CH_R_M;
        p_csc->field_value[ROHC_RTP_M_FID-ROHC_VAR_FID_BASE] = 
            (((rohc_rtp_t*)p_csc->p_cpib->rtp)->m_pt & ROHC_M_MASK )>>7 ;
        /* Flag is set whenever M bit is present in the uncompressed header */
        p_csc->field_value[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_M_RTPFLAGP1_MASK;
    }
    /*End SPR 6197 Fix*/
    if(ROHC_CPRM_IS_CH_R_X(p_csc) )
    {
        ret_value |= ROHC_CPRM_CH_R_X;
        p_csc->field_value[ROHC_RTP_X_FID-ROHC_VAR_FID_BASE] = 
            (((rohc_rtp_t*)p_csc->p_cpib->rtp)->v_p_x_cc & ROHC_X_MASK)>>4;
    }
    if(ROHC_CPRM_IS_CH_R_P(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_R_P;
        p_csc->field_value[ROHC_RTP_R_P_FID-ROHC_VAR_FID_BASE] = 
            (((rohc_rtp_t*)p_csc->p_cpib->rtp)->v_p_x_cc & ROHC_PAD_MASK)>>5;
        p_csc->field_value[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_R_PT_RTPFLAGP1_MASK;
    }
    if(ROHC_CPRM_IS_CH_CSRC(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_CSRC;
        p_csc->field_value[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_CSRC_RTPFLAGP1_MASK;
        p_csc->field_value[ROHC_RTP_CC_FID-ROHC_VAR_FID_BASE]
            = (((rohc_rtp_t*)p_csc->p_cpib->rtp)->v_p_x_cc &ROHC_PAD_MASK);  
    }
    if(ROHC_CPRM_IS_CH_TSS(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_TSS;
        p_csc->field_value[ROHC_RTP_TSS_FID-ROHC_VAR_FID_BASE] = 1;
        p_csc->field_value[ROHC_RTP_TS_STRIDE_FID-ROHC_VAR_FID_BASE] =
            p_csc->ts_stride;
        p_csc->field_value[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_TSS_RTPFLAGP1_MASK ;
    }
    if(ROHC_CPRM_IS_CH_TIS(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_TIS;
        p_csc->field_value[ROHC_RTP_TIS_FID-ROHC_VAR_FID_BASE] = 1;
        p_csc->field_value[ROHC_RTP_TIME_STRIDE_FID-ROHC_VAR_FID_BASE] =
            p_csc->time_stride;
        p_csc->field_value[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_TIS_RTPFLAGP1_MASK ;
    }
    if(p_csc->field_value[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE])
    {
        p_csc->field_value[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_RTPFLAG_EXT3FLAGP1_MASK;
    }
    /* IP - inner */    
    if(ROHC_CPRM_IS_CH_I(p_csc))
    {
        return_bitmap = rohc_cprm_delta_inner_ip4(p_csc, en_ip_id);
    }
    else
    {
        /* +- SPR 17777 */
        return_bitmap = rohc_cprm_delta_inner_ip6(p_csc);
    }   
    ret_value |= return_bitmap;

    if(p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE])
    {
        p_csc->field_value[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE] |=  
            ROHC_ENC_INNERIPFLAG_EXT3FLAGP1_MASK;
    } 
    /* outer ip */

    if(ROHC_CPRM_IS_CH_I2(p_csc))
    {
        /* +- SPR 17777 */
        return_bitmap = rohc_cprm_delta_outer_ip4(p_csc);
    }
    else if(p_csc->p_cpib->outer_ip6 != ROHC_NULL)
    {
        /* +- SPR 17777 */
        return_bitmap = rohc_cprm_delta_outer_ip6(p_csc);
    }
    ret_value |= return_bitmap;

    /* for udp */
    /* SPR 7406 FIX Start */
    if( ((rohc_udp_t*)(p_csc->p_cpib->udp))->cksum ) 
    {
        p_csc->static_delta |= ROHC_UDP_CHECK_SUM_PRESENT;
        ret_value |= ROHC_CPRM_CH_CHK_SUM;
     /* SPR 7406 FIX End */
        p_csc->field_value[ROHC_UDP_CKSUM_FID-ROHC_VAR_FID_BASE] = 
            ROHC_NTOHS(((rohc_udp_t*)(p_csc->p_cpib->udp))->cksum);
    }
    ROHC_EXIT_FN("rohc_cprm_p1_compute_delta");
    return ret_value;
}


/*****************************************************************************
**      FUNCTION:
**            rohc_cprm_delta_inner_ip4 
******************************************************************************
**
**      DESCRIPTION:
**              Computes the delta bit field for profile p1
**      ARGUMENTS:
**              p_csc   [IN]      Pointer to the CSC block
**              en_ip_id[IN]      Encoded value of ip_id
**      RETURN VALUE:
**              Delta bit map field
**      NOTES:
**               
*****************************************************************************/
static rohc_U32bit_t rohc_cprm_delta_inner_ip4 ROHC_PARAMS 
    ((p_csc,en_ip_id) ,
    rohc_csc_rtp_t          *p_csc      _AND_
    rohc_encoded_value_t    *en_ip_id )
{
    rohc_U32bit_t ret_value = 0;

    p_csc->field_value[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE] = 
        en_ip_id->value;
    /* IPv4 and IP-ID */
    /*p_csc->field_value[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE] = 
      en_ip_id->value;
      p_csc->field_len[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE] = 
      en_ip_id->bits;*/
    if(p_csc->new_inner_rnd == 0 && en_ip_id->bits != 0)
    {
        ret_value |= ROHC_CPRM_CH_I;
        /* p_csc->field_value[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE] |=
           ROHC_ENC_I_EXT3FLAGP1_MASK;*/
    }   

    if(ROHC_CPRM_IS_CH_TOS(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_TOS_TC;
        p_csc->field_value[ROHC_INNERIP4_TOS_FID-ROHC_VAR_FID_BASE] = 
            ((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->tos;
        p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_TOS_INNERIPFLAGP1_MASK;
    }
    if(ROHC_CPRM_IS_CH_TTL(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_TTL_HL;
        p_csc->field_value[ROHC_INNERIP4_TTL_FID-ROHC_VAR_FID_BASE] = 
            ((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->ttl;
        p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_TTL_INNERIPFLAGP1_MASK;                            
    }

    if(ROHC_CPRM_IS_CH_PR(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_PR_NH;
        p_csc->field_value[ROHC_INNERIP4_PROTO_FID-ROHC_VAR_FID_BASE] = 
            ((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->proto;
        p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_PROTO_INNERIPFLAGP1_MASK;                    
    }

    if(ROHC_CPRM_IS_CH_DF(p_csc))
    {   
        ret_value |= ROHC_CPRM_CH_DF;
        p_csc->field_value[ROHC_INNERIP_DF_FID-ROHC_VAR_FID_BASE ] = 
            (ROHC_NTOHS(((rohc_ip4_t*)p_csc->p_cpib->inner_ip4)->off) & 
             ROHC_IP4_DF)>>14;
        p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            (p_csc->field_value[ROHC_INNERIP_DF_FID-
             ROHC_VAR_FID_BASE])<<5;                    

    }
    /* Start SPR 6012 Fix*/
    /* End SPR 6012 Fix*/
    if(ROHC_CPRM_IS_CH_IPX(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_IPX;
        p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_IPX_INNERIPFLAGP1_MASK;  
    }
    /* Start SPR 6012 Fix*/
    if(p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE])
    {
        /* When InnerIP_Flags are set,RND bit indicates whether hdr(IP identifier) is not
         * to be compressed but instead sent as-is in compressed headers*/
        p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |= 
            (ROHC_ENC_RND_INNERIPFLAGP1_MASK & (p_csc->new_inner_rnd<<1));
        /* NBO bit indicates whether the octets of hdr(IP identifier) of this IP
         * header are swapped before compression and after decompression*/
        p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |= 
            (ROHC_ENC_NBO_INNERIPFLAGP1_MASK & (p_csc->new_inner_nbo<<2));            
    }
    /* End SPR 6012 Fix*/
    return ret_value;
}


/*****************************************************************************
**      FUNCTION:
**            rohc_cprm_delta_inner_ip6 
******************************************************************************
**
**      DESCRIPTION:
**              Computes the delta bit field for profile p1
**      ARGUMENTS:
**              p_csc   [IN]      Pointer to the CSC block
**              en_ip_id[IN]      Encoded value of ip_id
**      RETURN VALUE:
**              Delta bit map field
**      NOTES:
**               
*****************************************************************************/
        /* +- SPR 17777 */
static rohc_U32bit_t rohc_cprm_delta_inner_ip6(
    rohc_csc_rtp_t          *p_csc)
{
    rohc_U32bit_t ret_value = 0;

    if(ROHC_CPRM_IS_CH_TC(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_TOS_TC;
        p_csc->field_value[ROHC_INNERIP6_TCLASS_FID-ROHC_VAR_FID_BASE ] = 
            (ROHC_NTOHL(((rohc_ip6_t *)p_csc->p_cpib->inner_ip6)->
                        ip6_ctlun.ip6_un1.un1_flow) &
             ROHC_IP6_TRAFFICCLASS_MASK)>>20;
        p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_TOS_INNERIPFLAGP1_MASK; 
    }
    if(ROHC_CPRM_IS_CH_NH(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_PR_NH;

        p_csc->field_value[ROHC_INNERIP6_NHEADER_FID-ROHC_VAR_FID_BASE] = 
            ((rohc_ip6_t *)p_csc->p_cpib->inner_ip6)->ip6_ctlun.
            ip6_un1.un1_nxt;
        /* Start SPR 6108 fix */
        p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_PROTO_INNERIPFLAGP1_MASK;                       
        /* End SPR 6108 fix */

    }
    if(ROHC_CPRM_IS_CH_HL(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_TTL_HL;
        p_csc->field_value[ROHC_INNERIP6_HLIM_FID-ROHC_VAR_FID_BASE   ] = 
            ((rohc_ip6_t *)p_csc->p_cpib->inner_ip6)->ip6_ctlun.
            ip6_un1.un1_hlim;
        /* Start SPR 6108 fix */
        p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_TTL_INNERIPFLAGP1_MASK;          
        /* End SPR 6108 fix */

    }
    if(ROHC_CPRM_IS_CH_IPX(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_IPX;
        p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_IPX_INNERIPFLAGP1_MASK;  
    }
    return ret_value;
}


/*****************************************************************************
**      FUNCTION:
**            rohc_cprm_delta_outer_ip4 
******************************************************************************
**
**      DESCRIPTION:
**              Computes the delta bit field for profile p1
**      ARGUMENTS:
**              p_csc   [IN]      Pointer to the CSC block
**              en_ip_id[IN]      Encoded value of ip_id
**      RETURN VALUE:
**              Delta bit map field
**      NOTES:
**               
*****************************************************************************/
        /* +- SPR 17777 */
static rohc_U32bit_t rohc_cprm_delta_outer_ip4(
    rohc_csc_rtp_t          *p_csc)
{
    rohc_U32bit_t ret_value = 0;

    /* encoding of ip-id 2 is not required for p1 */
    if(p_csc->new_outer_rnd== 0)
    {   
        ret_value |= ROHC_CPRM_CH_I2;
        p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_IP2_INNERIPFLAGP1_MASK;
    }   
    p_csc->field_value[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE] = 
        ((rohc_ip4_t *)p_csc->p_cpib->outer_ip4)->id;
    p_csc->field_len[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE] = 16;

    if(ROHC_CPRM_IS_CH_TOS2(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_TOS_TC2;
        p_csc->field_value[ROHC_OUTERIP4_TOS_FID-ROHC_VAR_FID_BASE] = 
            ((rohc_ip4_t *)p_csc->p_cpib->outer_ip4)->tos;
        p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_TOS_OUTERIPFLAGP1_MASK;
    }



    if(ROHC_CPRM_IS_CH_TTL2(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_TTL_HL2;
        p_csc->field_value[ROHC_OUTERIP4_TTL_FID-ROHC_VAR_FID_BASE] = 
            ((rohc_ip4_t *)p_csc->p_cpib->outer_ip4)->ttl;
        p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_TTL_OUTERIPFLAGP1_MASK;                                
    }
    if(ROHC_CPRM_IS_CH_PR2(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_PR_NH2;
        p_csc->field_value[ROHC_OUTERIP4_PROTO_FID-ROHC_VAR_FID_BASE] = 
            ((rohc_ip4_t *)p_csc->p_cpib->outer_ip4)->proto;
        p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_PROTO_OUTERIPFLAGP1_MASK;                            
    }
    if(ROHC_CPRM_IS_CH_DF2(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_DF2;
        p_csc->field_value[ROHC_OUTERIP_DF_FID-ROHC_VAR_FID_BASE ] = 
            (((rohc_ip4_t *)p_csc->p_cpib->outer_ip4)->off & 
             ROHC_IP4_DF)>>14;
        p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            (p_csc->field_value[ROHC_OUTERIP_DF_FID-
             ROHC_VAR_FID_BASE ])<<5;
    }
    /* Start SPR 6012 Fix*/
    /* End SPR 6012 Fix*/
    if(ROHC_CPRM_IS_CH_IPX2(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_IPX2;
        p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_IPX_OUTERIPFLAGP1_MASK;
    }
    /* Start SPR 6012 Fix*/
    if(p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE])
    {
        /* When OuterIP_Flags are set,RND bit indicates whether hdr(IP identifier) is not
         * to be compressed but instead sent as-is in compressed headers*/
        p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            (ROHC_ENC_RND_OUTERIPFLAGP1_MASK & (p_csc->new_outer_rnd<<1));
        /* NBO bit indicates whether the octets of hdr(IP identifier) of this IP
         * header are swapped before compression and after decompression*/
        p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            (ROHC_ENC_NBO_OUTERIPFLAGP1_MASK & (p_csc->new_outer_nbo<<2));
    }
    /* End SPR 6012 Fix*/
    return ret_value;
}


/*****************************************************************************
**      FUNCTION:
**            rohc_cprm_delta_outer_ip6 
******************************************************************************
**
**      DESCRIPTION:
**              Computes the delta bit field for profile p1
**      ARGUMENTS:
**              p_csc   [IN]      Pointer to the CSC block
**              en_ip_id[IN]      Encoded value of ip_id
**      RETURN VALUE:
**              Delta bit map field
**      NOTES:
**               
*****************************************************************************/
        /* +- SPR 17777 */
static rohc_U32bit_t rohc_cprm_delta_outer_ip6(
    rohc_csc_rtp_t          *p_csc)
{
    rohc_U32bit_t ret_value = 0;

    if(ROHC_CPRM_IS_CH_HL2(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_TTL_HL2;
        p_csc->field_value[ROHC_OUTERIP6_HLIM_FID-ROHC_VAR_FID_BASE   ] = 
            ((rohc_ip6_t *)p_csc->p_cpib->outer_ip6)->ip6_ctlun.
            ip6_un1.un1_hlim;
        p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_TTL_OUTERIPFLAGP1_MASK;
    }
    if(ROHC_CPRM_IS_CH_NH2(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_PR_NH2;
        p_csc->field_value[ROHC_OUTERIP6_NHEADER_FID-ROHC_VAR_FID_BASE] = 
            ((rohc_ip6_t *)p_csc->p_cpib->outer_ip6)->ip6_ctlun.
            ip6_un1.un1_nxt;
        p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_PROTO_OUTERIPFLAGP1_MASK;
    }
    if(ROHC_CPRM_IS_CH_IPX2(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_IPX2;
        p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_IPX_OUTERIPFLAGP1_MASK;
    }
    if(ROHC_CPRM_IS_CH_TC2(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_TOS_TC2;
        p_csc->field_value[ROHC_OUTERIP6_TCLASS_FID-ROHC_VAR_FID_BASE ] = 
            ROHC_NTOHL((((rohc_ip6_t *)p_csc->p_cpib->outer_ip6)->
                        ip6_ctlun.ip6_un1.un1_flow) &
                    ROHC_IP6_TRAFFICCLASS_MASK)>>20;
        p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_TOS_OUTERIPFLAGP1_MASK;
    }

    return ret_value;
}
