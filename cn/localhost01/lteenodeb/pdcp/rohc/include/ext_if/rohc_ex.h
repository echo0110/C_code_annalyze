/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_ex.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
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
 * $Log: rohc_ex.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/28 05:18:04  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_EX_H_
#define _ROHC_EX_H_

#include "rohc_g_typ.h"
#include "rohc_typ.h"
#include "rohc_def.h"
typedef enum enodeb_status {
    ENODEB_RELOC_PROC_NOT_IN_PROGRESS = 0,
    ENODEB_RELOC_IN_PROGRESS ,
    ENODEB_RELOC_COMPLETED, 
    ENODEB_RELOC_CLOSED    
}rohc_enodeb_status_t;

typedef struct rohc_initdb_req {
    rohc_U32bit_t    max_size_entity_pool;
    rohc_U32bit_t    max_size_csc_pool;
    rohc_U32bit_t    max_size_dsc_pool;
}rohc_initdb_req_t;
                                


/* Parameters from RRC in the API ROHC_CONFIG_REQ */

typedef struct rohc_config_req {

    rohc_U16bit_t    max_cid;            /* maximum CID value that can be used. */
    rohc_U16bit_t    profiles;          /* number of profiles supported */       
    rohc_U16bit_t    direction;          /*  indicates the direction field */
}rohc_config_req_t;

typedef struct rohc_data_req {
    rohc_U32bit_t           in_buff_len;        /* input buffer length */
    rohc_U32bit_t           out_buff_len;       /* output buffer length */
    rohc_U8bit_t            feedback_flag;      /* feedback information */
}rohc_data_req_t;
 
typedef struct rohc_api_info {
    rohc_timestamp_t        curr_time_stamp;    /* time of arrival of packet at ROHC */
 union {
        rohc_initdb_req_t   initdb_req;         /* pool sizes for INIT DB REQ */
        rohc_config_req_t   config_req;         /* CONFIG REQ from RRC */
        rohc_data_req_t     rohc_data_req;      /* buffer lengths for rohc DATA REQ */
    } rohc_api_union;

    rohc_U32bit_t       in_buff_len;        /* input buffer length for DATA IND */
    rohc_U16bit_t           api_id;             /* api_id */
    rohc_entity_id_t           entity_id;          /* entity_id */
    rohc_U8bit_t    c_flag;
#define ROHC_DATA_IND_FB 0x01   
#define ROHC_DATA_IND_CMP_PKT 0x02
} rohc_api_info_t;

/* + SPR 17439 */
rohc_void_t 
    rohc_timer_handler ROHC_PROTO(( rohc_void_t *td));

/* Initializes the ROHC moudules*/  
rohc_return_t  rohc_main_init ROHC_NO_PARAMS();    

    /* Coverity 54622/54623*/
rohc_void_t rohcFreeFromPool(void);
/* - SPR 17439 */    
    /* Coverity 54622/54623*/
#endif /* _ROHC_EX_H_ */
