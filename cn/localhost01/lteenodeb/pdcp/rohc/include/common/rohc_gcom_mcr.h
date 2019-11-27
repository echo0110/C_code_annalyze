/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_gcom_mcr.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains GET/SET macro for rohc entity
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_gcom_mcr.h,v $
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

#ifndef _ROHC_GCOM_MCR_H_
#define _ROHC_GCOM_MCR_H_


#define ROHC_NTOHS(val_m) (ntohs_wrapper((val_m)))
#define ROHC_NTOHL(val_m) (ntohl_wrapper((val_m)))
#define ROHC_HTONS(val_m) (htons_wrapper((val_m)))
#define ROHC_HTONL(val_m) (htonl_wrapper((val_m)))

 #define ROHC_ABS(x) 	abs_wrapper(x)


#define ROHC_GET_UPPER_NIBBLE(byte) (((rohc_U8bit_t)(byte)&0xf0U)>>4U)
#define ROHC_GET_LOWER_NIBBLE(byte) ((rohc_U8bit_t)(byte)&0x0fU)
#define ROHC_GET_2MSB(byte)         (((rohc_U8bit_t)(byte)&0xc0U)>>6U)
#define ROHC_GET_5MSB(byte)         (((rohc_U8bit_t)(byte)&0xf8U)>>3U)
#define ROHC_GET_3LSB(byte)         ((rohc_U8bit_t)(byte)&0x07U)

#define ROHC_GET_NBITS(value,from_bit_pos,no_of_bits,result) do{\
                rohc_U32bit_t tmp; \
                tmp=value>>(from_bit_pos - 1);\
                (result)=(((1<<(no_of_bits))-1)&tmp);\
                }while(0)

#define ROHC_SET_LOWER_NIBBLE(x, n) ((x & 0xf0) | n)
#define ROHC_SET_UPPER_NIBBLE(x, n) ((x & 0x0f) | (n << 4U))

#define ROHC_SET_32BIT(p_to, value) do{ \
                *(p_to + 3) = value & 0xFF; \
                *(p_to + 2) = (value & 0xFF00) >> 8; \
                *(p_to + 1) = (value & 0xFF0000) >> 16; \
                *(p_to) = (value & 0xFF000000) >> 24; \
                }while(0)

#define ROHC_SET_16BIT(p_to, value) do{ \
                *(p_to + 1)= value & 0xFF; \
                *(p_to) = (value & 0xFF00) >> 8; \
                }while(0)

#define ROHC_GET_4BYTES(ptr) \
                *(ptr+3) |            \
                (*(ptr+2)<<8) |     \
                (*(ptr+1)<<16) |    \
                (*(ptr)<<24)

#define ROHC_GET_2BYTES(ptr) \
                 (*(ptr+1) |          \
                (*(ptr)<<8))

#define ROHC_GET_3BYTES(ptr) \
                *(ptr+2) |            \
                (*(ptr+1)<<8) |     \
                (*(ptr)<<16)     \

                
#define ROHC_ASSIGN_2_OCTS(in,out) \
                out[0] = in[0];\
                out[1] = in[1]

#define ROHC_ASSIGN_4_OCTS(in,out) \
                out[0] = in[0];\
                out[1] = in[1];\
                out[2] = in[2];\
                out[3] = in[3]

#define ROHC_SET_4_OCTS(p_to, value) \
                *(p_to + 3) = (rohc_U8bit_t)(value); \
                *(p_to + 2) = (rohc_U8bit_t)(value >> 8); \
                *(p_to + 1) = (rohc_U8bit_t)(value >> 16); \
                *(p_to) = (rohc_U8bit_t)(value >> 24)

#define ROHC_SET_3_OCTS(p_to, value) \
                *(p_to + 2) = (rohc_U8bit_t)(value); \
                *(p_to + 1) = (rohc_U8bit_t)(value >> 8); \
                *(p_to) = (rohc_U8bit_t)(value >> 16)
                
#define ROHC_SET_2_OCTS(p_to, value) \
                *(p_to + 1)= (rohc_U8bit_t)value; \
                *(p_to) = (rohc_U8bit_t)(value >> 8)
                

#define ROHC_ENT_GET_FB_RCV_LIST(p_entity_m)\
    (p_entity_m->feedback_recv)

#define ROHC_ENT_GET_FB_BE_SENT_LIST(p_entity_m)\
    (p_entity_m->feedback_to_be_sent)

#define ROHC_ENT_GET_K_1(p_entity_m)\
    (p_entity_m->k_1)

#define ROHC_ENT_SET_K_1(p_entity_m,k_1_m)\
    (p_entity_m->k_1 = k_1_m)

#define ROHC_ENT_GET_K_2(p_entity_m)\
    (p_entity_m->k_2)

#define ROHC_ENT_SET_K_2(p_entity_m,k_2_m)\
    (p_entity_m->k_2 = k_2_m)

#define ROHC_ENT_GET_N_1(p_entity_m)\
    (p_entity_m->n_1)

#define ROHC_ENT_SET_N_1(p_entity_m,n_1_m)\
    (p_entity_m->n_1 = n_1_m)

#define ROHC_ENT_GET_N_2(p_entity_m)\
    (p_entity_m->n_2)

#define ROHC_ENT_SET_N_2(p_entity_m,n_2_m)\
    (p_entity_m->n_2 = n_2_m)

#define ROHC_ENT_GET_M_1(p_entity_m)\
    (p_entity_m->m_1)

#define ROHC_ENT_SET_M_1(p_entity_m,m_1_m)\
    (p_entity_m->m_1 = m_1_m)

#define ROHC_ENT_GET_M_2(p_entity_m)\
    (p_entity_m->m_2)

#define ROHC_ENT_SET_M_2(p_entity_m,m_2_m)\
    (p_entity_m->m_2 = m_2_m)

#define ROHC_ENT_GET_N(p_entity_m)\
    (p_entity_m->n)

#define ROHC_ENT_SET_N(p_entity_m,n_m)\
    (p_entity_m->n = n_m)

#define ROHC_ENT_GET_MAX_IR_RETURN_TIME(p_entity_m)\
    (p_entity_m->max_ir_return_time)

#define ROHC_ENT_SET_MAX_IR_RETURN_TIME(p_entity_m,max_ir_return_time_m)\
    (p_entity_m->max_ir_return_time = max_ir_return_time_m)

#define ROHC_ENT_GET_MAX_FO_RETURN_TIME(p_entity_m)\
    (p_entity_m->max_fo_return_time)

#define ROHC_ENT_SET_MAX_FO_RETURN_TIME(p_entity_m,max_fo_return_time_m)\
    (p_entity_m->max_fo_return_time = max_fo_return_time_m)

#define ROHC_ENT_GET_MAX_STATIC_INFO_UPDATE_COUNTER(p_entity_m)\
    (p_entity_m->max_static_info_update_counter)

#define ROHC_ENT_SET_MAX_STATIC_INFO_UPDATE_COUNTER\
(p_entity_m,max_static_info_update_counter)\
    (p_entity_m->max_static_info_update_counter=\
     max_static_info_update_counter_m)

#define ROHC_ENT_GET_MAX_DYNAMIC_INFO_UPDATE_COUNTER(p_entity_m)\
    (p_entity_m->max_dynamic_info_update_counter)

#define ROHC_ENT_SET_MAX_DYNAMIC_INFO_UPDATE_COUNTER\
(p_entity_m,max_dynamic_info_update_counter)\
    (p_entity_m->max_dynamic_info_update_counter=\
     max_dynamic_info_update_counter_m)

#define ROHC_DCPM_INIT_OPT(optType, optVal) \
                optType =0; \
                optVal =0

#define ROHC_C_INIT_OPT_KIND(optType, val) \
                optType =val


#endif

