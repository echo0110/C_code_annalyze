/*****************************************************************************
 *    FILE NAME:
 *        sig_mem.h
 *
 *    DESCRIPTION:
 *       This file has common porting functions for memory.
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    12NOV03     Vishavdeep Sharma   -            Original Creation
 *
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/

#ifndef __SIGTRAN_MEM_H__
#define __SIGTRAN_MEM_H__

#include <sig_typs.h>
#ifdef __cplusplus
extern "C" {
#endif

    typedef struct
    {
        sigtran_S32bit      num_buf;
        sigtran_S32bit      buf_size;
    } sigtran_pool_info_st;


    sigtran_return_t sigtran_mem_init(
        sigtran_U32bit              task_id,
        sigtran_U32bit              num_common_pools,
        sigtran_pool_info_st        *p_comm_pool_info,
        sigtran_pool_data_st        *p_common_pool_data, /*CMN Pool Info,OUT*/
        sigtran_U32bit              num_task_pools,
        sigtran_pool_info_st        *p_list_pool_info,
        sigtran_pool_id_t           *p_pool_id_list, /* List of Pool Ids,OUT*/
        sigtran_boolean_t           b_init_mesg_pool,
        sigtran_error_t             *p_ecode );

    sigtran_return_t sigtran_mem_deinit(
        sigtran_U32bit              task_id,
        sigtran_U32bit              num_common_pools,
        sigtran_pool_info_st        *p_comm_pool_info,
        sigtran_pool_data_st        *p_common_pool_data,
        sigtran_U32bit              num_task_pools,
        sigtran_pool_info_st        *p_list_pool_info,
        sigtran_pool_id_t           *p_pool_id_list,
        sigtran_boolean_t           b_deinit_mesg_pool,
        sigtran_error_t             *p_ecode );


    sigtran_buffer_t sigtran_get_buffer(
        sigtran_U32bit        task_id,
        sigtran_boolean_t     is_cmn_pool,
        sigtran_void_t        *p_pool_info,
        sigtran_S32bit        buf_size );

    sigtran_return_t sigtran_free_buffer(
        sigtran_U32bit        task_id,
        sigtran_boolean_t     is_cmn_pool,
        sigtran_pool_id_t     pool_id,
        sigtran_buffer_t      *pp_buffer );

    sigtran_void_t  sigtran_incr_mem_ref_count(
        sigtran_buffer_t      p_buffer );

    sigtran_buffer_t  sigtran_mesg_get_buffer(
        sigtran_U32bit              task_id,
        sigtran_void_t              *p_pool_info,
        sigtran_S32bit              buf_size );


    sigtran_return_t sigtran_mesg_free_buffer(
        sigtran_U32bit              task_id,
        sigtran_buffer_t           *pp_buffer );


    void  sigtran_destroy_all_cmn_pools( void );

    sigtran_buffer_t sigtran_malloc(
        sigtran_U32bit         size );

    sigtran_void_t  sigtran_free(
        sigtran_buffer_t      p_buffer );

    sigtran_void_t *sigtran_os_malloc(
        sigtran_U32bit         size );

    sigtran_void_t  sigtran_os_free(
        sigtran_void_t       *p_buffer );

#ifdef SIG_COMMON_WITH_CSPL

    sigtran_return_t sigtran_inst_ext_buff_in_mesg_buffer(
        sigtran_U32bit              task_id,
        sigtran_buffer_t            *p_mesg_buffer,
        sigtran_S32bit              insert_position,
        sigtran_buffer_t            *p_ext_buffer,
        sig_free_func_ptr_t         free_fn,
        sigtran_pvoid_t             p_free_fn_args,
        sigtran_S32bit              buf_size,
        sigtran_error_t           *p_ecode );

    sigtran_return_t sigtran_extract_buffer_to_iovec(
        sigtran_U32bit              task_id,
        sigtran_buffer_t            *p_buffer,
        sigtran_iovec_st            *p_iovec,
        sigtran_U32bit              *p_num_iovec );
#endif /* SIG_COMMON_WITH_CSPL */
#ifdef __cplusplus
}
#endif
#endif
