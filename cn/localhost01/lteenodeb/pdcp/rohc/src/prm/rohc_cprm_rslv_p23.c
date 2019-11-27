/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_cprm_rslv_p23.c,v 1.1.1.1.16.1 2010/11/03 07:05:36 gur22059 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the function definition for determining
 *                     the packet type for profile p2(IP/UDP) and p3(IP/ESP).
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_cprm_rslv_p23.c,v $
 * Revision 1.1.1.1.16.1  2010/11/03 07:05:36  gur22059
 * ROHC Merge for Profile-0 and 2
 *
 * Revision 1.3.36.3  2010/10/13 03:57:46  gur22059
 * Comment Incorporation after reduction in context block
 *
 * Revision 1.3.36.2  2010/09/23 05:20:13  gur22059
 * Reduction in the size of context block w.r.t profile2
 *
 * Revision 1.3.36.1  2010/09/10 10:17:00  gur22059
 * Locality of reference is used for better performamce
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
#include "rohc_ccom_mcr.h"
#include "rohc_cprm_mcr.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/
/* + SPR 17439 */
 STATIC rohc_U32bit_t rohc_cprm_delta_inner_ip4 ROHC_PROTO
      ((
       rohc_csc_udp_t          *p_csc      ,
       rohc_encoded_value_t    *en_ip_id ));

        /* +- SPR 17777 */
 STATIC rohc_U32bit_t rohc_cprm_delta_inner_ip6
     ( rohc_csc_udp_t          *p_csc);

 STATIC rohc_U32bit_t rohc_cprm_delta_outer_ip4 ROHC_PROTO
      ((
       rohc_csc_udp_t          *p_csc      ,
       rohc_encoded_value_t    *en_ip_id ));
/* - SPR 17439 */
 STATIC rohc_U32bit_t rohc_cprm_delta_outer_ip6
       (rohc_csc_udp_t          *p_csc);
        /* +- SPR 17777 */

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
extern  rohc_U8bit_t rohc_p23_uo_fo_pkt_typ[2];
extern  rohc_U8bit_t rohc_p23_uo_crc_3[2];
extern  rohc_U8bit_t rohc_p23_uo_crc_7[2];                        
extern  rohc_U8bit_t rohc_p23_uo_both_ipid[2];
extern  rohc_U8bit_t rohc_p23_uo_one_ipid[2];
extern  rohc_U8bit_t rohc_p23_uo_ext_3[2] ;
extern  rohc_U8bit_t rohc_p23_uo_mt_pending[2];
extern  rohc_U8bit_t rohc_p23_r_fo_pkt_typ_1[2];
extern  rohc_U8bit_t rohc_p23_r_fo_pkt_typ_2[2] ;
extern  rohc_U8bit_t rohc_p23_r_both_ipid[2];
extern  rohc_U8bit_t rohc_p23_r_one_ipid[2];
extern  rohc_U8bit_t rohc_p23_r_ext_3[2];
extern  rohc_U8bit_t rohc_p23_r_mt_pending[2];

extern rohc_p23_pkt_typ_info_t 
        rohc_p23_pkt_typ_info_uo[ROHC_P23_UO_MAX_PACKETS];

extern rohc_p23_pkt_typ_info_t 
        rohc_p23_pkt_typ_info_r[ROHC_P23_R_MAX_PACKETS];

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/



/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_resolve_pkt_typ_p2_uo
******************************************************************************
**
**      DESCRIPTION:
**              This function determines the packet type for profile 2 & 3,
**              when compressor is operated in U/O mode
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**              delta_bit_map[IN] Delta bit map for changing fields
**              en_sn   [IN]      Encoded value of sn
**              en_ip_id[IN]      Encoded value of ip_id
**              en_ip_id[IN]      Encoded value of ip_id2
**              no_crc_bit[IN}    Number of crc bits    
**      RETURN VALUE:
**              Rohc packet type
**      NOTES:
**               
*****************************************************************************/
        /* +- SPR 17777 */
rohc_U8bit_t rohc_cprm_resolve_pkt_typ_p2_uo ROHC_PARAMS(
            (p_csc, delta_bit_map, en_sn,
                 en_ip_id, en_ip_id2, no_crc_bit),
                    rohc_csc_udp_t *p_csc _AND_
                    rohc_U32bit_t delta_bit_map _AND_
                    rohc_encoded_value_t *en_sn _AND_
                    rohc_encoded_value_t *en_ip_id _AND_
                    rohc_encoded_value_t *en_ip_id2 _AND_
                    rohc_U8bit_t no_crc_bit)
{ 
    
    rohc_U8bit_t pkt_bit_map[ROHC_BIT_MAP_LEN2];
    rohc_U8bit_t pkt_bit_map_no_ext3[ROHC_BIT_MAP_LEN2];
    
    rohc_U8bit_t i = ROHC_PRM_CRC_BITS0;

       /* SPR 3444 changes starts*/ 
    rohc_U8bit_t    cond = (8*ROHC_BIT_MAP_LEN2) - 1;
       /* SPR 3444 changes ends*/ 
    rohc_U8bit_t    ip4_count;
    
    ROHC_ENTER_FN("rohc_cprm_resolve_pkt_typ_p2_uo");
    ROHC_CPRM_BIT_INIT(pkt_bit_map, rohc_p23_uo_fo_pkt_typ,ROHC_BIT_MAP_LEN2);
    
    if (no_crc_bit == ROHC_PRM_CRC_BITS3)
    {
        ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p23_uo_crc_3,ROHC_BIT_MAP_LEN2);
    }
    else if (no_crc_bit  == ROHC_PRM_CRC_BITS7)
    {
        ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p23_uo_crc_7, ROHC_BIT_MAP_LEN2);
    }
    else
    {
        /*en_sn->bits = ROHC_P23_SN_BITS;
        en_ip_id->bits = ROHC_P123_IPID_BITS;
        en_ip_id2->bits = ROHC_P123_IPID_BITS ;*/
        ROHC_EXIT_FN("rohc_cprm_resolve_pkt_typ_p2_uo");
        return ROHC_UNKNOWN_PKT;
    }   
    
    ip4_count = ROHC_CPIB_GET_IP4_COUNT(p_csc->p_cpib);
    if ( ip4_count == 2 )
    {
      ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p23_uo_both_ipid, ROHC_BIT_MAP_LEN2);
    }

    else if ( ip4_count == 1 )
    {
      ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p23_uo_one_ipid, ROHC_BIT_MAP_LEN2);
    }

    if(ROHC_CSC_GET_TRANS_STATE(p_csc) == ROHC_C_TST_PENDING )
    {
      ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p23_uo_mt_pending, ROHC_BIT_MAP_LEN2);
    }   
    
/*  ROHC_CPRM_BIT_SEL_PKT(pkt_bit_map, 2 , sel_pkt_typ , num_pkt);*/
    /* if any other bit except marker is on , ext-3 is required */
    if( (delta_bit_map &
        (~(ROHC_CPRM_CH_R_M| ROHC_CPRM_CH_I | ROHC_CPRM_CH_I2)) )!= 0 )
    {
        ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p23_uo_ext_3 , ROHC_BIT_MAP_LEN2);
      
        for(i = 0 ; i <cond; i++)
        {
            if((ROHC_IS_ITH_BIT_SET(pkt_bit_map,i)) &&
               rohc_p23_pkt_typ_info_uo[i].sn_bits >=  en_sn->bits && 
               rohc_p23_pkt_typ_info_uo[i].in_ip_id_bits >= en_ip_id->bits && 
               rohc_p23_pkt_typ_info_uo[i].out_ip_id_bits >= en_ip_id2->bits)
            {   
/*SPR #1392*/
                en_sn->bits = rohc_p23_pkt_typ_info_uo[i].sn_bits;
                en_ip_id->bits = rohc_p23_pkt_typ_info_uo[i].in_ip_id_bits;
                en_ip_id2->bits = rohc_p23_pkt_typ_info_uo[i].out_ip_id_bits;
                p_csc->field_value[ROHC_EXT_FID -ROHC_VAR_FID_BASE]=1;

                ROHC_EXIT_FN("rohc_cprm_resolve_pkt_typ_p2_uo");
                return rohc_p23_pkt_typ_info_uo[i].pbm_pkt_typ;
            }   
        
        } 
    }else
    {
        ROHC_CPRM_KEEP_REMOVE(pkt_bit_map_no_ext3,pkt_bit_map,
                            rohc_p23_uo_ext_3,ROHC_BIT_MAP_LEN2);
        for(i = 0 ; i <cond; i++)
        {
            if((ROHC_IS_ITH_BIT_SET(pkt_bit_map_no_ext3,i)) &&
               rohc_p23_pkt_typ_info_uo[i].sn_bits >=  en_sn->bits && 
               rohc_p23_pkt_typ_info_uo[i].in_ip_id_bits >= en_ip_id->bits && 
               rohc_p23_pkt_typ_info_uo[i].out_ip_id_bits >= en_ip_id2->bits)
            {   
/*SPR #1392*/
                en_sn->bits = rohc_p23_pkt_typ_info_uo[i].sn_bits;
                en_ip_id->bits = rohc_p23_pkt_typ_info_uo[i].in_ip_id_bits;
                en_ip_id2->bits = rohc_p23_pkt_typ_info_uo[i].out_ip_id_bits;
                p_csc->field_value[ROHC_EXT_FID -ROHC_VAR_FID_BASE]=0;

                ROHC_EXIT_FN("rohc_cprm_resolve_pkt_typ_p2_uo");
                return rohc_p23_pkt_typ_info_uo[i].pbm_pkt_typ;
            }   
        
        }       
      
        for(i = 0 ; i <cond; i++)
        {
            if((ROHC_IS_ITH_BIT_SET(pkt_bit_map,i)) &&
               rohc_p23_pkt_typ_info_uo[i].sn_bits >=  en_sn->bits && 
               rohc_p23_pkt_typ_info_uo[i].in_ip_id_bits >= en_ip_id->bits && 
               rohc_p23_pkt_typ_info_uo[i].out_ip_id_bits >= en_ip_id2->bits)
            {   
/*SPR #1392*/
                en_sn->bits = rohc_p23_pkt_typ_info_uo[i].sn_bits;
                en_ip_id->bits = rohc_p23_pkt_typ_info_uo[i].in_ip_id_bits;
                en_ip_id2->bits = rohc_p23_pkt_typ_info_uo[i].out_ip_id_bits;
                p_csc->field_value[ROHC_EXT_FID -ROHC_VAR_FID_BASE]=1;

                ROHC_EXIT_FN("rohc_cprm_resolve_pkt_typ_p2_uo");
                return rohc_p23_pkt_typ_info_uo[i].pbm_pkt_typ;
            }   
        
        }                               
    }  
    
   /* en_sn->bits = ROHC_P23_SN_BITS;
    en_ip_id->bits = ROHC_P123_IPID_BITS ;
    en_ip_id2->bits = ROHC_P123_IPID_BITS ;*/
    ROHC_EXIT_FN("rohc_cprm_resolve_pkt_typ_p2_uo");
    return ROHC_UNKNOWN_PKT;
        
}

/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_resolve_pkt_typ_p2_r
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
**              en_ip_id[IN]      Encoded value of ip_id
**              en_ip_id2[IN]      Encoded value of ip_id2
**              no_crc_bit[IN}    Number of crc bits    
**      RETURN VALUE:
**              Rohc packet type
**      NOTES:
**               
*****************************************************************************/
        /* +- SPR 17777 */
rohc_U8bit_t rohc_cprm_resolve_pkt_typ_p2_r ROHC_PARAMS(
                (p_csc, delta_bit_map, en_sn,
                 en_ip_id, en_ip_id2, no_crc_bit),
                    rohc_csc_udp_t *p_csc _AND_
                    rohc_U32bit_t delta_bit_map _AND_
                    rohc_encoded_value_t *en_sn _AND_
                    rohc_encoded_value_t *en_ip_id _AND_
                    rohc_encoded_value_t *en_ip_id2 _AND_
                    rohc_U8bit_t no_crc_bit)
{
    rohc_U8bit_t pkt_bit_map[ROHC_BIT_MAP_LEN2];
    rohc_U8bit_t pkt_bit_map_no_ext3[ROHC_BIT_MAP_LEN2];
    
    rohc_U8bit_t i = 0;

    rohc_U8bit_t    ip4_count;
    rohc_U8bit_t    cond = 8*ROHC_BIT_MAP_LEN2;

    ROHC_ENTER_FN("rohc_cprm_resolve_pkt_typ_p2_r");
    if (no_crc_bit == 7)
    {
        ROHC_CPRM_BIT_INIT(pkt_bit_map, rohc_p23_r_fo_pkt_typ_2, 
                                                ROHC_BIT_MAP_LEN2);
        
    }
    else if(no_crc_bit  == 0) 
    {
        ROHC_CPRM_BIT_INIT(pkt_bit_map, rohc_p23_r_fo_pkt_typ_1, 
                                                ROHC_BIT_MAP_LEN2);
        
    }
    else
    {
        /*en_sn->bits = ROHC_P23_SN_BITS;
        en_ip_id->bits = ROHC_P123_IPID_BITS ;
        en_ip_id2->bits = ROHC_P123_IPID_BITS ;*/
        ROHC_EXIT_FN("rohc_cprm_resolve_pkt_typ_p2_r");
        return ROHC_UNKNOWN_PKT;
    }
    
    ip4_count = ROHC_CPIB_GET_IP4_COUNT(p_csc->p_cpib);
 
    if ( ip4_count == ROHC_BIT_MAP_LEN2 )
    {
      ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p23_r_both_ipid, ROHC_BIT_MAP_LEN2);
    } 
    else if ( ip4_count == 1 )
    {
      ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p23_r_one_ipid, ROHC_BIT_MAP_LEN2);
    }

    if(ROHC_CSC_GET_TRANS_STATE(p_csc) == ROHC_C_TST_PENDING )
    {
      ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p23_r_mt_pending, ROHC_BIT_MAP_LEN2);
    }   
    
    /*ROHC_CPRM_BIT_SEL_PKT(pkt_bit_map, 2 , sel_pkt_typ , num_pkt);*/
     
    /* if any other bit except marker is on , ext-3 is required */
    if((delta_bit_map &
         (~(ROHC_CPRM_CH_R_M| ROHC_CPRM_CH_I | ROHC_CPRM_CH_I2)) )!= 0 )
    {
        ROHC_CPRM_BIT_AND(pkt_bit_map, rohc_p23_r_ext_3 , ROHC_BIT_MAP_LEN2);
     
        for(i = 0 ; i <cond; i++)
        {
            if((ROHC_IS_ITH_BIT_SET(pkt_bit_map,i)) &&
               rohc_p23_pkt_typ_info_r[i].sn_bits >=  en_sn->bits && 
               rohc_p23_pkt_typ_info_r[i].in_ip_id_bits >= en_ip_id->bits && 
               rohc_p23_pkt_typ_info_r[i].out_ip_id_bits >= en_ip_id2->bits)
            {   
/*SPR #1392*/
                en_sn->bits = rohc_p23_pkt_typ_info_r[i].sn_bits;
                en_ip_id->bits = rohc_p23_pkt_typ_info_r[i].in_ip_id_bits;
                en_ip_id2->bits = rohc_p23_pkt_typ_info_r[i].out_ip_id_bits;
                p_csc->field_value[ROHC_EXT_FID -ROHC_VAR_FID_BASE]=1;
                
                ROHC_EXIT_FN("rohc_cprm_resolve_pkt_typ_p2_r");
                return rohc_p23_pkt_typ_info_r[i].pbm_pkt_typ;
            }   
        
        }  
    }
    else
    {
        
        ROHC_CPRM_KEEP_REMOVE(pkt_bit_map_no_ext3,pkt_bit_map,
                            rohc_p23_r_ext_3,ROHC_BIT_MAP_LEN2);

        for(i = 0 ; i <cond; i++)
        {
             if((ROHC_IS_ITH_BIT_SET(pkt_bit_map_no_ext3,i)) &&
               rohc_p23_pkt_typ_info_r[i].sn_bits >=  en_sn->bits && 
               rohc_p23_pkt_typ_info_r[i].in_ip_id_bits >= en_ip_id->bits && 
               rohc_p23_pkt_typ_info_r[i].out_ip_id_bits >= en_ip_id2->bits)
            {   
/*SPR #1392*/
                en_sn->bits = rohc_p23_pkt_typ_info_r[i].sn_bits;
                en_ip_id->bits = rohc_p23_pkt_typ_info_r[i].in_ip_id_bits;
                en_ip_id2->bits = rohc_p23_pkt_typ_info_r[i].out_ip_id_bits;
                p_csc->field_value[ROHC_EXT_FID -ROHC_VAR_FID_BASE]=0;
                    
                ROHC_EXIT_FN("rohc_cprm_resolve_pkt_typ_p2_r");
                return rohc_p23_pkt_typ_info_r[i].pbm_pkt_typ;
            }
        }
        for(i = 0 ; i <cond; i++)
        { 
             if((ROHC_IS_ITH_BIT_SET(pkt_bit_map,i)) &&
               rohc_p23_pkt_typ_info_r[i].sn_bits >=  en_sn->bits && 
               rohc_p23_pkt_typ_info_r[i].in_ip_id_bits >= en_ip_id->bits && 
               rohc_p23_pkt_typ_info_r[i].out_ip_id_bits >= en_ip_id2->bits)
            {   
/*SPR #1392*/
                en_sn->bits = rohc_p23_pkt_typ_info_r[i].sn_bits;
                en_ip_id->bits = rohc_p23_pkt_typ_info_r[i].in_ip_id_bits;
                en_ip_id2->bits = rohc_p23_pkt_typ_info_r[i].out_ip_id_bits;
                p_csc->field_value[ROHC_EXT_FID -ROHC_VAR_FID_BASE]=1;

                ROHC_EXIT_FN("rohc_cprm_resolve_pkt_typ_p2_r");
                return rohc_p23_pkt_typ_info_r[i].pbm_pkt_typ;
        }   
          } 
    }                           
    
        /*en_sn->bits = ROHC_P23_SN_BITS;
        en_ip_id->bits = ROHC_P123_IPID_BITS ;
        en_ip_id2->bits = ROHC_P123_IPID_BITS ;*/
        ROHC_EXIT_FN("rohc_cprm_resolve_pkt_typ_p2_r");
        return ROHC_UNKNOWN_PKT;
}

/*****************************************************************************
**      FUNCTION:
**              rohc_cprm_compute_p2_delta
******************************************************************************
**
**      DESCRIPTION:
**              Computes the delta bit field for profile p2 and p3
**      ARGUMENTS:
**              p_entity[IN]      Pointer to the rohc entity block
**              p_csc   [IN]      Pointer to the CSC block
**      RETURN VALUE:
**              Delta bit map field
**      NOTES:
**               
*****************************************************************************/
        /* +- SPR 17777 */
rohc_U32bit_t rohc_cprm_compute_p2_delta ROHC_PARAMS(
                            ( p_csc,  en_sn,
                            en_ip_id, en_ip_id2), 
                            rohc_csc_udp_t *p_csc _AND_
                            rohc_encoded_value_t *en_sn _AND_
                            rohc_encoded_value_t *en_ip_id _AND_
                            rohc_encoded_value_t *en_ip_id2 
                            )
{
    rohc_U32bit_t ret_value = 0;
    rohc_U32bit_t return_bitmap = 0;
    ROHC_ENTER_FN("rohc_cprm_compute_p2_delta");
     
    p_csc->field_value[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE]=0;
    p_csc->field_value[ROHC_RTP_FLAGS_FID-ROHC_VAR_FID_BASE]=0;
    p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE]=0;
    p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE]=0;
    
    p_csc->field_value[ROHC_SN_FID-ROHC_VAR_FID_BASE] = en_sn->value;
    
    /* Fix for SPR#6563 */
    //if(ROHC_CPRM_IS_CH_MODE(p_csc))
    //{
        ret_value |= ROHC_CPRM_CH_MODE;
        p_csc->field_value[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE] |=
                        (p_csc->c_mode <<3);
    //}
    /* Fix for SPR#6563 */
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
        return_bitmap = rohc_cprm_delta_outer_ip4(p_csc, en_ip_id2);
    }
    else if(p_csc->p_cpib->outer_ip6 != ROHC_NULL)
    {
        return_bitmap = rohc_cprm_delta_outer_ip6(p_csc);
    }
    ret_value |= return_bitmap;
    /* for udp */
        /*if(ROHC_CPRM_IS_CH_CHK_SUM(p_csc))*/
    if( p_csc->static_delta & ROHC_UDP_CHECK_SUM_PRESENT ) 
    {
       /* ret_value |= ROHC_CPRM_CH_CHK_SUM;*/
        p_csc->field_value[ROHC_UDP_CKSUM_FID-ROHC_VAR_FID_BASE] = 
                 ROHC_NTOHS(((rohc_udp_t *)p_csc->p_cpib->udp)->cksum);
    }
    
    ROHC_EXIT_FN("rohc_cprm_compute_p2_delta");
    return ret_value;
}

/*****************************************************************************
**      FUNCTION:
**            rohc_cprm_delta_inner_ip4 
******************************************************************************
**
**      DESCRIPTION:
**              Computes the delta bit field for profile p2
**      ARGUMENTS:
**              p_csc   [IN]      Pointer to the CSC block
**              en_ip_id[IN]      Encoded value of ip_id
**      RETURN VALUE:
**              Delta bit map field
**      NOTES:
**               
*****************************************************************************/
STATIC rohc_U32bit_t rohc_cprm_delta_inner_ip4 ROHC_PARAMS 
    ((p_csc,en_ip_id) ,
    rohc_csc_udp_t          *p_csc      _AND_
    rohc_encoded_value_t    *en_ip_id )
{
    rohc_U32bit_t ret_value = 0;
    /* IPv4 and IP-ID */
    p_csc->field_value[ROHC_INNERIP_ID_FID-ROHC_VAR_FID_BASE] = 
        en_ip_id->value;
    if(p_csc->new_inner_rnd== 0 && en_ip_id->bits != 0)
    {
        ret_value |= ROHC_CPRM_CH_I;
        /* p_csc->field_value[ROHC_EXT3_FLAGS_FID-ROHC_VAR_FID_BASE] |=
           ROHC_ENC_I_EXT3FLAGP1_MASK;*/
    }

    if(ROHC_CPRM_IS_CH_TOS(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_TOS_TC;
        p_csc->field_value[ROHC_INNERIP4_TOS_FID-ROHC_VAR_FID_BASE] = 
            ((rohc_ip4_t *)p_csc->p_cpib->inner_ip4)->tos;
        p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_TOS_INNERIPFLAGP1_MASK;
    }
    if(ROHC_CPRM_IS_CH_TTL(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_TOS_TC;
        p_csc->field_value[ROHC_INNERIP4_TTL_FID-ROHC_VAR_FID_BASE] = 
            ((rohc_ip4_t *)p_csc->p_cpib->inner_ip4)->ttl;
        p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_TTL_INNERIPFLAGP1_MASK;
    }
    if(ROHC_CPRM_IS_CH_PR(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_PR_NH;
        p_csc->field_value[ROHC_INNERIP4_PROTO_FID-ROHC_VAR_FID_BASE] = 
            ((rohc_ip4_t *)p_csc->p_cpib->inner_ip4)->proto;
        p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_PROTO_INNERIPFLAGP1_MASK;
    }
    if(ROHC_CPRM_IS_CH_DF(p_csc))
    {   
        ret_value |= ROHC_CPRM_CH_DF;
        p_csc->field_value[ROHC_INNERIP_DF_FID-ROHC_VAR_FID_BASE ] = 
            ( ROHC_NTOHS(((rohc_ip4_t *)p_csc->p_cpib->inner_ip4)->off) &
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
    return ret_value;
}

/*****************************************************************************
**      FUNCTION:
**            rohc_cprm_delta_inner_ip6 
******************************************************************************
**
**      DESCRIPTION:
**              Computes the delta bit field for profile p2
**      ARGUMENTS:
**              p_csc   [IN]      Pointer to the CSC block
**              en_ip_id[IN]      Encoded value of ip_id
**      RETURN VALUE:
**              Delta bit map field
**      NOTES:
**               
*****************************************************************************/
        /* +- SPR 17777 */
STATIC rohc_U32bit_t rohc_cprm_delta_inner_ip6(
    rohc_csc_udp_t          *p_csc)
{
    rohc_U32bit_t ret_value = 0;
    if(ROHC_CPRM_IS_CH_TC(p_csc))
    {
        ret_value |= ROHC_CPRM_CH_TOS_TC;
        p_csc->field_value[ROHC_INNERIP6_TCLASS_FID-ROHC_VAR_FID_BASE ] = 
            (ROHC_NTOHL(((rohc_ip6_t *)p_csc->p_cpib->inner_ip6)->
                        ip6_ctlun.ip6_un1.un1_flow) &ROHC_IP6_TRAFFICCLASS_MASK)>>20;
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
        p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_TTL_INNERIPFLAGP1_MASK;          

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
**              Computes the delta bit field for profile p2
**      ARGUMENTS:
**              p_csc   [IN]      Pointer to the CSC block
**              en_ip_id[IN]      Encoded value of ip_id
**      RETURN VALUE:
**              Delta bit map field
**      NOTES:
**               
*****************************************************************************/
STATIC rohc_U32bit_t rohc_cprm_delta_outer_ip4 ROHC_PARAMS 
    ((p_csc,en_ip_id2) ,
    rohc_csc_udp_t          *p_csc      _AND_
    rohc_encoded_value_t    *en_ip_id2 )
{
    rohc_U32bit_t ret_value = 0;
    p_csc->field_value[ROHC_OUTERIP_ID_FID-ROHC_VAR_FID_BASE] = 
        en_ip_id2->value;
    if(p_csc->new_outer_rnd== 0 && en_ip_id2->bits == 0 )
    {
        ret_value |= ROHC_CPRM_CH_I2;
        p_csc->field_value[ROHC_INNERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_IP2_INNERIPFLAGP1_MASK;

    }
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
            ( ROHC_NTOHS(((rohc_ip4_t *)p_csc->p_cpib->outer_ip4)->off) & 
              ROHC_IP4_DF)>>14;
        p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=(
                p_csc->field_value[ROHC_OUTERIP_DF_FID-
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
**              Computes the delta bit field for profile p2
**      ARGUMENTS:
**              p_csc   [IN]      Pointer to the CSC block
**              en_ip_id[IN]      Encoded value of ip_id
**      RETURN VALUE:
**              Delta bit map field
**      NOTES:
**               
*****************************************************************************/
        /* +- SPR 17777 */
STATIC rohc_U32bit_t rohc_cprm_delta_outer_ip6
    (rohc_csc_udp_t          *p_csc )
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
            ( ROHC_NTOHL(((rohc_ip6_t *)p_csc->p_cpib->outer_ip6)->ip6_ctlun.
                         ip6_un1.un1_flow)&ROHC_IP6_TRAFFICCLASS_MASK)>>20;
        p_csc->field_value[ROHC_OUTERIP_FLAGS_FID-ROHC_VAR_FID_BASE] |=
            ROHC_ENC_TOS_OUTERIPFLAGP1_MASK;
    }
    return ret_value;
}

