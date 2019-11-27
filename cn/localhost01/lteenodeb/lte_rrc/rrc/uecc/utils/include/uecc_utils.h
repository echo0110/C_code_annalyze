/******************************************************************
*
*  ARICENT -
*
*  Copyright (c) 2009 Aricent.
* 
****************************************************************/
/*****************************************************************
*
* FILE NAME  :uecc_utils.h
*
* DESCRIPTION :This file is header file for uecc_utils.c
*
*      DATE       NAME          REFERENCE       REASON
*    18 May 2009  Pradeep S                    initial
*
*
*
******************************************************************/

#ifndef UECC_UTILS_H
#define UECC_UTILS_H

#include "rrc_defines.h"
/* CDMA2000_CSFB_HO start */
#define SETNBIT(p_buf, bitPos, src, width, p_uecc_ue_context) \
    setNBitVal(&p_buf, &bitPos, src, width, p_uecc_ue_context)
/* CDMA2000_CSFB_HO stops */
struct _uecc_gb_context_t;
struct _uecc_ue_context_t;

/* Interface module check message validity function */
typedef rrc_return_et (*p_uecc_intf_check_func_t) (
    struct _uecc_gb_context_t*  p_uecc_gb_context   /* UECC global context */
);

/* Interface module event handler */
typedef void (*p_uecc_intf_handler_t) (
    void*                       p_api,             /* Input API buffer */
    struct _uecc_gb_context_t*  p_uecc_gb_context  /* UECC global context */
);

/* Universal ID structure */
typedef struct
{
    U16     id;
} uecc_id_t;

typedef struct
{
    uecc_id_t               api_id;
    p_uecc_intf_handler_t   p_uecc_intf_handler;
} uecc_intf_handlers_table_t;

typedef struct
{
    uecc_id_t                   module_id;
    p_uecc_intf_check_func_t    p_uecc_intf_check_func;
    uecc_intf_handlers_table_t* p_uecc_handlers_table;
} uecc_intf_modules_table_t;

rrc_return_et uecc_rrc_send_internal_msg
(
    rrc_module_id_t     dst_module_id,  /* Destination module identifier */
    U16                 api_id,         /* API Id */
    U16                 msg_size,       /* Size of message (payload) */
    void                *p_msg          /* Message specific info (payload) */
);

void* uecc_alloc_intrl_msg
(
    rrc_module_id_t         dst_module_id,  /* Destination module identifier */
    U16                     api_id,         /* API Id */
    U32                     msg_size        /* Size of message (payload) */
);

U16 uecc_find_index_by_id
(
    U16         id,
    uecc_id_t*  p_table,
    U16         element_size
);

void rrc_generate_key(
    U8 p0,
    U8 algorithm_id,
    U8 *key_dest,
    U8 *k_enb
);

void rrc_generate_kenb_star(
    U16 p0,
    U16 dl_earfcn,
    U8 *key_dest,
    U8 *k_enb
);


#ifdef ENDC_ENABLED
/*NR_DC Code Change Start*/
void rrc_generate_skgnb
(
    U16 p0,
    U8 *key_dest,
    U8 *k_enb
);

U8
rrc_uecc_count_scg_bearer
(
 struct _uecc_ue_context_t *p_ue_context
);
/*NR_DC Code Change Stop*/
#endif

void rrc_pack_U16
(
 void    *p_dest,
 void    *p_src
);

void
rrc_unpack_U16
(
 void    *p_dest,
 void    *p_src
 );


void
rrc_pack_U32
(
 void    *p_dest,
 void    *p_src
 );

void
rrc_unpack_U32
(
 void    *p_dest,
 void    *p_src
 );

rrc_return_t rrc_uecc_check_is_cell_valid_and_active
(
     struct _uecc_gb_context_t*    p_uecc_gb_context,  /* UECC global context */
     rrc_cell_index_t              cell_index
);
void rrc_uecc_set_cell_index_in_rrm_api
(
    struct _uecc_gb_context_t*   p_uecc_gb_context,  /* UECC global context */
    void             *   p_api,
    U16                  api_id,
    rrc_cell_index_t     cell_index
);
void rrc_uecc_get_cell_index_from_rrm_api
(
 struct _uecc_ue_context_t   *p_ue_context,
 void                *p_api,
 rrc_cell_index_t    *p_cell_index
);
/* CDMA2000_CSFB_HO start */
/* Bug 12190 Fix Start */
void setNBitVal(
    U8*** p_buf, 
    U8**  bitPosition,
   U32 src, 
   U8 width,
   struct  _uecc_ue_context_t   *p_ue_context
);
/* Bug 12190 Fix Stop */
/* CDMA2000_CSFB_HO stops */

rrc_return_et uecc_rrc_send_inter_cell_internal_msg
(
    rrc_module_id_t     dst_module_id,  /* Destination module identifier */
    U16                 api_id,         /* API Id */
    U16                 msg_size,       /* Size of message (payload) */
    void                *p_msg          /* Message specific info (payload) */
);
/*SPR 15896 Fix Start*/
U8 set_minimum_bitmask(void);
U8 set_medium_bitmask(void);
U8 set_maximum_bitmask(void);
U8 set_all_trace_bitmask(void); 
U8 set_minimum_medium_both_bitmask(void);
/*SPR 15896 Fix Stop*/
char* change_data_to_str(U16 len, U8*data);
#endif
