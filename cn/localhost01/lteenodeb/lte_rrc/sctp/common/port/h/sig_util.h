/*****************************************************************************
 *    FILE NAME:
 *        sig_util.h
 *
 *    DESCRIPTION:
 *       This file has prototype of gen util functions
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    12NOV03     Vishavdeep Sharma   -            Original Creation
 *
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/

/* Basic Sigtran Mutex Type needs to be ported */

#ifndef  __SIGTRAN_UTIL_H__
#define __SIGTRAN_UTIL_H__

#include <sig_typs.h>
#include <sig_defs.h>

#ifdef __cplusplus
extern "C" {
#endif
#define SIGTRAN_NTOHL(x)      ntohl(x)
#define SIGTRAN_NTOHS(x)      ntohs(x)
#define SIGTRAN_HTONL(x)      htonl(x)
#define SIGTRAN_HTONS(x)      htons(x)

#define SIGTRAN_SET_ECODE(p_ecode, err) \
    if (p_ecode != 0) \
        *p_ecode = err;

    sigtran_S32bit sigtran_strcmp(
        sigtran_pvoid_t         p_str1,
        sigtran_pvoid_t         p_str2 );

    sigtran_S32bit sigtran_strncmp(
        sigtran_pvoid_t         p_str1,
        sigtran_pvoid_t         p_str2,
        sigtran_U32bit          size );

    sigtran_U32bit sigtran_strlen(
        sigtran_pvoid_t         p_str );

    sigtran_pvoid_t sigtran_strcpy(
        sigtran_pvoid_t         p_dst,
        sigtran_pvoid_t         p_src );

    sigtran_pvoid_t sigtran_strrchr(
        sigtran_pvoid_t         p_str,
        sigtran_U32bit          chr );

    /* Utility functions */
    void sig_util_extract_1byte
    ( sigtran_buffer_t  *p_buf,
      sigtran_U8bit     *p_value );

    void sig_util_extract_2bytes
    ( sigtran_buffer_t   *p_buf,
      sigtran_U16bit    *p_value );

    void sig_util_extract_4bytes
    ( sigtran_buffer_t   *p_buf,
      sigtran_U32bit    *p_value );

    void sig_util_set_1byte
    ( sigtran_buffer_t  *p_buf,
      sigtran_U8bit     value );

    void sig_util_set_2bytes
    ( sigtran_buffer_t   *p_buf,
      sigtran_U16bit     val );

    void sig_util_set_4bytes
    ( sigtran_buffer_t   *p_buf,
      sigtran_U32bit     val );

    sigtran_return_t sig_util_ext_ip_port_frm_ipc_info(
        sig_ip_addr_st   *p_ipc_info,
        sigtran_U32bit  *p_ip_addr,
        sigtran_U16bit  *p_port,
        sigtran_U32bit  *p_flow_info,
        sigtran_U32bit  *p_scope_id,
        sigtran_error_t *p_ecode );

    sigtran_return_t sig_genrate_public_name( sig_stack_type_et stack_type,
                                              sig_dest_type_et  dest_type,
                                              sigtran_U16bit    handle,
                                              sigtran_U32bit    *p_ext_mod_id );

    sigtran_return_t sigtran_memcpy(
        sigtran_pvoid_t         p_mem1,
        sigtran_pvoid_t         p_mem2,
        sigtran_U32bit           size );

    sigtran_return_t sigtran_memset(
        sigtran_pvoid_t         p_mem1,
        sigtran_U32bit           val,
        sigtran_U32bit           size );

    sigtran_S32bit sigtran_memcmp(
        sigtran_pvoid_t         p_mem1,
        sigtran_pvoid_t         p_mem2,
        sigtran_U32bit          size );

    sigtran_return_t sig_get_ipc_info_frm_service_name
    ( sigtran_UL32bit  service_name,
      sig_stack_type_et *p_stack_type,
      sig_dest_type_et  *p_dest_type,
      sigtran_U16bit    *p_handle,
      sigtran_error_t   *p_ecode );

#ifdef __cplusplus
}
#endif
#endif
