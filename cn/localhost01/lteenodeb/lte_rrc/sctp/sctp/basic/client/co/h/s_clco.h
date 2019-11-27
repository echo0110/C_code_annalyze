/******************************************************************************
 *    FILE NAME:    s_clco.h
 *
 *    DESCRIPTION:
 *        This file is part of common part of both SCTP SU and SM clients.
 *
 *
 *    DATE          NAME           REFERENCE       REASON
 *    -----------------------------------------------------
 *
 *    23March 2000     Vakul Garg  -           .Original Creation
 *    12Sep   2001     Yogesh Gahlaut           Rel 3.0 Changes
 *
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/
#ifndef    __FILE_sctp_clp_co_SEEN__
#define    __FILE_sctp_clp_co_SEEN__

#ifdef __cplusplus
extern "C" {
#endif

#include <s_cotyps.h>
#include <s_cobuf.h>

    /* Done for vxworks */
#ifdef SCTP_RED_SUPPORT
    /* This huge size is needed to transfer the huge data of associations to
     * support redundancy
     */
#define SCTP_MAX_API_SIZE 65536 /*64K, maximum size of UDP datagram*/
#else
#define SCTP_MAX_API_SIZE 3000
    /* this is more than 1500 just because to support data chunk with size
     * more than 1500 in msg based interface
     */
#endif


#define    E_INVALID_API_POINTER   1
#define    SCTP_INVALID_API_ID     1
#define    APIBUF_LENGTH_OFFSET    12
#define    SCTP_API_HEADER_LEN     16


    /*** other parameters ****************************/

#define    SCTP_MAX_HOSTNAME_LEN           50   /* Maximum hostname length */
#define    SCP_MAX_PERCENTAGE_VALUE        100  /* Max percentage */
#define    SCTP_IPADDRTYPE_IPv4            0    /* address type for ip ver 4 */
#define    SCTP_HB_ENABLE                  0
#define    SCTP_HB_DISABLE                 1
#define    SCTP_HB_REQUEST                 2


#ifdef SCTP_DEBUG_ENABLED

#define    SCTP_CL_REPORT(x)              printf x


#ifndef SCTP_OS_PATH_DELIM
#define SCTP_OS_PATH_DELIM                  '/'
#endif

#define SCTP_CLIENT_TRACE_LEN_MAX 200

#if defined (SIGTRAN_PORT_WINNT) || defined (SIGTRAN_PORT_VXWORKS) || defined (SIGTRAN_PORT_VX_PNE)

#define SCTP_CLIENT_TRACE(format_str) \
    do \
    { \
        char       dbgstr[SCTP_CLIENT_TRACE_LEN_MAX]  = {0}; \
        sprintf(dbgstr,"[SCTP:%4s %10s:%04d] ", SCTP_MODULE, strrchr(__FILE__, SCTP_OS_PATH_DELIM) + 1, __LINE__);\
        SCTP_CL_REPORT((dbgstr));\
        SCTP_CL_REPORT(format_str);\
        SCTP_CL_REPORT(("\r\n")); \
    } while(0) \
         
#else

#define SCTP_CLIENT_TRACE(format_str) \
    do \
    { \
        char       dbgstr[SCTP_CLIENT_TRACE_LEN_MAX]  = {0}; \
        snprintf(dbgstr,SCTP_CLIENT_TRACE_LEN_MAX,"[SCTP:%4s %10s:%04d] ", SCTP_MODULE, strrchr(__FILE__, SCTP_OS_PATH_DELIM) + 1, __LINE__);\
        SCTP_CL_REPORT((dbgstr));\
        SCTP_CL_REPORT(format_str);\
        SCTP_CL_REPORT(("\r\n")); \
    } while(0) \
         
#endif


#define SCTP_CLIENT_TRACE_ADDR(p_addr) \
    do \
    {\
        sctp_sockaddr_st *p_debug_addr = p_addr;\
        if (p_debug_addr->family == SCTP_AF_INET) \
        {\
            struct in_addr in; \
            in.s_addr =  SCTP_HTONL(p_debug_addr->ip.v4.addr); \
            SCTP_CL_REPORT(("\nAF_INET:"));\
            SCTP_CL_REPORT(("%x (%s) ", p_debug_addr->ip.v4.addr, inet_ntoa(in)));\
        }\
        else if (p_debug_addr->family == SCTP_AF_INET6)\
        {\
            SCTP_CL_REPORT(("\nAF_INET6:"));\
            SCTP_CL_REPORT(("%x %x %x %x",\
                            p_debug_addr->ip.v6.addr.s_u6_addr32[0],\
                            p_debug_addr->ip.v6.addr.s_u6_addr32[1],\
                            p_debug_addr->ip.v6.addr.s_u6_addr32[2],\
                            p_debug_addr->ip.v6.addr.s_u6_addr32[3]));\
        }\
        SCTP_CL_REPORT(("\n"));\
    }while(0)


#define SCTP_CL_PRINT_HEX(p_buffer, noctets) \
    do \
    {\
        sctp_U32bit i = 0; \
        sctp_U32bit count1 = 0; \
        for (; i < (sctp_U32bit) noctets; i+=4) \
        {\
            SCTP_CL_REPORT(("%x%x %x%x %x%x %x%x ", \
                            (unsigned int) (p_buffer[i])/16, \
                            (unsigned int) (p_buffer[i])%16, \
                            (unsigned int) (p_buffer[i+1])/16, \
                            (unsigned int) (p_buffer[i+1])%16, \
                            (unsigned int) (p_buffer[i+2])/16, \
                            (unsigned int) (p_buffer[i+2])%16, \
                            (unsigned int) (p_buffer[i+3])/16, \
                            (unsigned int) (p_buffer[i+3])%16));\
            SCTP_CL_REPORT(("   "));\
            count1++; \
            /* Now have to give a new line after 4 of these */ \
            if (count1 == 4)\
            {\
                SCTP_CL_REPORT(("\n"));\
                count1 = 0;\
            }\
        }\
        SCTP_CL_REPORT(("\n"));\
    } while (0)\
         

#else

#define    SCTP_CL_REPORT(x)

#define SCTP_CLIENT_TRACE(format_str) \
    do \
    { \
    } while(0)


#define SCTP_CLIENT_TRACE_ADDR(p_addr) \
    do \
    {\
    }while(0)
#define SCTP_CL_PRINT_HEX(p_buffer, noctets) \
    do \
    {\
    } while (0)\
         
#endif /* SCTP_DEBUG_ENABLED */


    typedef    sctp_U16bit sctp_api_id_t;
    typedef    sctp_U16bit sctp_src_id_t;
    typedef    sctp_U16bit sctp_dest_id_t;
    typedef    sctp_U32bit sctp_trans_id_t;
    typedef    sctp_U32bit sctp_len_t;


    typedef    struct
    {
        sctp_api_id_t      api_id;
        sctp_U16bit        api_version_no;
        sctp_src_id_t      src_id;
        sctp_dest_id_t     dest_id;
        sctp_trans_id_t    transaction_id;
        sctp_len_t         buffer_len;
    } sctp_api_header_t;


    extern sctp_U8bit *
    sctp_build_api_header(
        sctp_U8bit          *p_buffer,
        sctp_api_id_t       api_id,
        sctp_trans_id_t     transaction_id,
        sctp_src_id_t       src_id,
        sctp_dest_id_t      dest_id );


#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_clp_co_SEEN__   */
