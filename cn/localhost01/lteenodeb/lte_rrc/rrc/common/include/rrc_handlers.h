/******************************************************************************
*
*   FILE NAME:
*       rrc_handlers.h
*
*   DESCRIPTION:
*       This header file contains all external handlers provided by RRC layer
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 Apr 2009     VasylN      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_HANDLERS_H_
#define _RRC_HANDLERS_H_

#include "rrc_defines.h"
#include "rrc_oam_intf.h"

/* CSR_00040799 Fix Start */
#define RRC_MAX_COMM_INFO   8
/* CSR_00040799 Fix End */

typedef struct _sector_comm_info_t
{
    U8                  cell_index;
    S8                  comm_info_lower_layer[RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][MAX_LEN_COMM_INFO];
}sector_comm_info_t;

typedef struct _sector_comm_info_list_t
{
    U8                    count;
/* CSR_00040799 Fix Start */
    sector_comm_info_t    sector_comm_info[RRC_MAX_COMM_INFO];
/* CSR_00040799 Fix End */
}sector_comm_info_list_t;

typedef struct _comm_info
{
    S8      comm_info_upper_layer[RRC_OAM_MAX_EXT_SON_UPPER_LAYER_MODULES][MAX_LEN_COMM_INFO];
    /* SPR 20908 Start */
    sector_comm_info_list_t    sector_comm_info_list;
    /* SPR 20908 End */
}comm_info_t;
/* For license validation */
typedef struct comm_info_init_param
{
    U32     expiry_duration;
    comm_info_t    comm_info;
}comm_info_init_param_t;

typedef struct thread_arg
{
    QDRIVER driver_id;
    QWAIT * p_qwait;
    U8      logical_id;
    pthread_t tid;
}thread_arg_t;

rrc_return_et rrc_create_uecc_instances
(
    U8              num_of_uecc_instances, /* Number of UECC instances to be created */
    U8              num_of_cores,          /* Number of Cores */
    U8              *core_number           /* Array of Core Numbers */
);

rrc_return_et rrc_destroy_uecc_instances
(
    U8              num_of_uecc_instances /* Number of UECC instances to be created */
);

rrc_return_et rrc_create_logger_thread
(
    rrc_shared_mem_config_t *p_shared_mem_cfg,
    U16                     health_monitor_duration
);
                        
/******************************************************************************
*   LLIM external handlers
******************************************************************************/

void*
rrc_llim_init
(
    void *p_rrc_llim_init_data
);

S32
rrc_llmd_process_msg
(
    void *p_api,
    void *p_gl_ctx
);

void
rrc_llmd_process_timer_msg
(
    rrc_timer_t timer_id,
    void        *p_timer_buf,
    void        *p_gl_ctx
);

/******************************************************************************
*   CSC external handlers
******************************************************************************/

void*
rrc_csc_init
(
    void *p_rrc_csc_init_data
);

S32
rrc_csc_process_msg
(
    void *p_api,
    void *p_gl_ctx
);

void
rrc_csc_process_timer_msg
(
    rrc_timer_t timer_id,
    void        *p_timer_buf,
    void        *p_gl_ctx
);

/******************************************************************************
*   UECC external handlers
******************************************************************************/
void*
rrc_uecc_init
(
    void *p_rrc_uecc_init_data
);

S32
rrc_uecc_process_msg
(
    void *p_api,
    void *p_gl_ctx
);

void
rrc_uecc_process_timer_msg
(
    rrc_timer_t timer_id,
    void        *p_timer_buf,
    void        *p_gl_ctx
);

/******************************************************************************
*   OAMH external handlers
******************************************************************************/
void*
rrc_oamh_init
(
    void *
);

S32
rrc_oamh_process_msg
(
    void *,
    void *
);

void
rrc_oamh_process_timer_msg
(
    rrc_timer_t,
    void *,
    void *
);

/******************************************************************************
*   UECCMD external handlers
******************************************************************************/

void*
rrc_ueccmd_init
(
    void *p_rrc_ueccmd_init_data
);

S32
rrc_ueccmd_process_msg
(
    void *p_api,
    void *p_gl_ctx
);

void
rrc_ueccmd_process_timer_msg
(
    rrc_timer_t timer_id,
    void        *p_timer_buf,
    void        *p_gl_ctx
);

/******************************************************************************
 *    LOGGING external handlers
 *****************************************************************************/
void* l3_logger_init
(
    void *p_reader_config
);

void  l3_logger_process_timer_msg(
        rrc_timer_t timer_id, 
        void        *p_timer_buf,
        void        *p_gl_ctx);

S32 l3_logger_process_msg
(
    void *p_api,
    void *p_gl_ctx
);

#endif /* _RRC_HANDLERS_H_ */

