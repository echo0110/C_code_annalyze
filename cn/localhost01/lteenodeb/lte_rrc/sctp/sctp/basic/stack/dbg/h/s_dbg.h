/***************************************************************************
 *  FILE NAME  : s_dbg.h
 *
 *  DESCRIPTION: Contains the Debug Functionality.
 *
 *  DATE       NAME         REF#         REASON
 *  -------------------------------------------
 *  02Jun00    Samahajan    -            .Create original
 *  16Aug01    gsheoran     -            REL 3.0 Update
 *  09May08    Raza         REL 6.2      SPR 18527
 *
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef  __FILE_sctp_debg_SEEN__
#define  __FILE_sctp_debg_SEEN__

#ifdef __cplusplus
extern "C" {
#endif


#define SCTP_ENABLE_INTF_TRC(flag)  flag |= SCTP_INTF_TRC;
#define SCTP_DISABLE_INTF_TRC(flag) flag &= (0xFE);

#define SCTP_ENABLE_ERR_TRC(flag)   flag |= SCTP_ERR_TRC;
#define SCTP_DISABLE_ERR_TRC(flag)  flag &= (0xFD);

#define SCTP_ENABLE_DBG_TRC(flag)   flag |= SCTP_DBG_TRC;
#define SCTP_DISABLE_DBG_TRC(flag)  flag &= (0xFB);

#define SCTP_ENABLE_ALL_TRC(flag)   flag |= SCTP_ALL_TRC;
#define SCTP_DISABLE_ALL_TRC(flag)  flag &= (0x00);

#ifndef SCTP_OS_PATH_DELIM
#define SCTP_OS_PATH_DELIM                  '/'
#endif

    /*
     * This data struture contains the current global trace level for
     * each trace flags.
     */
    extern sctp_U8bit  sctp_gbl_trace;
    extern sctp_U32bit sctp_log_text( const char *, ... );
    extern char *sctp_get_filename( const char * );

#define SCTP_PRINT_HEX(p_buffer, noctets) \
    do \
    {\
        sctp_U32bit i = 0; \
        sctp_U32bit count1 = 0; \
        for (; i < (sctp_U32bit) noctets; i += 4) \
        {\
            if (count1 == 0) \
            {\
                sctp_log_text("%06x ", i); \
            }\
            sctp_log_text("%x%x %x%x %x%x %x%x ", (unsigned int) (p_buffer[i])/16, \
                          (unsigned int) (p_buffer[i])%16, \
                          (unsigned int) (p_buffer[i+1])/16, \
                          (unsigned int) (p_buffer[i+1])%16, \
                          (unsigned int) (p_buffer[i+2])/16, \
                          (unsigned int) (p_buffer[i+2])%16, \
                          (unsigned int) (p_buffer[i+3])/16, \
                          (unsigned int) (p_buffer[i+3])%16);\
            sctp_log_text("   ");\
            count1++; \
            /* Now have to give a new line after 4 of these */ \
            if (count1 == 4)\
            {\
                sctp_log_text("\n");\
                count1 = 0;\
            }\
        }\
        sctp_log_text("\n");\
    } while (0)\
         
    sctp_return_t
    sctp_api_enable_assoc_trc(
        sctp_U32bit        ep_id,
        sctp_trc_et        trace_type,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_api_disable_assoc_trc(
        sctp_U32bit        ep_id,
        sctp_trc_et        ttype,
        sctp_error_t       *p_ecode );

    sctp_return_t
    sctp_api_get_trace_info(
        sctp_U32bit        ep_id,
        sctp_U8bit         *trace_info,
        sctp_error_t       *p_ecode );


    sctp_return_t
    sctp_is_trace_enabled(
        sctp_ep_st         *p_ep,
        sctp_trc_et        trace_type );

#ifdef SCTP_DEBUG_ENABLED

    sctp_return_t
    sctp_init_debg_module(
        sctp_error_t       *p_ecode );

    void
    sctp_hex_dump(
        sctp_ep_st         *p_ep,
        sctp_trc_et        trace_type,
        sctp_U8bit         *p_buffer,
        sctp_U32bit        noctets );

    void
    givetime( void );


#define SCTP_REPORT(x)   sctp_log_text x

#ifndef SCTP_MODULE
#define SCTP_MODULE           "Undef"
#endif

#define SCTP_TRACE_LEN_MAX 200

#if defined (SIGTRAN_PORT_WINNT) || defined (SIGTRAN_PORT_VXWORKS) || defined (SIGTRAN_PORT_VX_PNE)

#define SCTP_TRACE(p_ep, ttype, format_str) \
    do \
    { \
        if (sctp_is_trace_enabled(p_ep, ttype) == SCTP_SUCCESS) \
        {\
            sctp_ep_st *p_tmp_ep1 = p_ep; \
            char       dbgstr[SCTP_TRACE_LEN_MAX]  = {0}; \
            givetime();\
            sprintf(dbgstr, "[SCTP:%4s %10s:%04d] ", SCTP_MODULE, \
                    sctp_get_filename(__FILE__), __LINE__);\
            SCTP_REPORT((dbgstr));\
            SCTP_REPORT(format_str);\
            if (p_tmp_ep1)\
            {\
                SCTP_REPORT((" Socket/Endpoint Id = %u\r\n", (unsigned int)p_tmp_ep1->ep_id));\
            }\
            else\
            {\
                SCTP_REPORT(("\r\n")); \
            }\
        }\
    } while(0) \
         
#else

#define SCTP_TRACE(p_ep, ttype, format_str) \
    do \
    { \
        if (sctp_is_trace_enabled(p_ep, ttype) == SCTP_SUCCESS) \
        {\
            sctp_ep_st *p_tmp_ep1 = p_ep; \
            char       dbgstr[SCTP_TRACE_LEN_MAX]  = {0}; \
            givetime();\
            snprintf(dbgstr,SCTP_TRACE_LEN_MAX, "[SCTP:%4s %10s:%04d] ", SCTP_MODULE, \
                     sctp_get_filename(__FILE__), __LINE__);\
            SCTP_REPORT((dbgstr));\
            SCTP_REPORT(format_str);\
            if (p_tmp_ep1)\
            {\
                SCTP_REPORT((" Socket/Endpoint Id = %u\r\n", (unsigned int)p_tmp_ep1->ep_id));\
            }\
            else\
            {\
                SCTP_REPORT(("\r\n")); \
            }\
        }\
    } while(0) \
         
#endif


#define SCTP_HEX_DUMP(p_ep, ttype, p_buffer, noctets) \
    sctp_hex_dump(p_ep, ttype, p_buffer, noctets)

#define SCTP_HEX_DUMP_IOV(p_ep, ttype, p_iovec, iovec_len) \
    do \
    { \
        unsigned int i = 0; \
        for (i =0; i < iovec_len; i++) \
        { \
            sctp_hex_dump(p_ep, ttype, (sctp_U8bit*)p_iovec[i].iov_base, \
                          p_iovec[i].iov_len); \
        } \
    }while(0)


#define SCTP_TRACE_ADDR(p_ep, ttype, p_addr) \
    do \
    {\
        if (sctp_is_trace_enabled(p_ep, ttype) == SCTP_SUCCESS) \
        {\
            sctp_sockaddr_st *p_debug_addr = p_addr;\
            if (p_debug_addr->family == SCTP_AF_INET) \
            {\
                struct in_addr in; \
                in.s_addr = SCTP_HTONL(p_debug_addr->ip.v4.addr); \
                SCTP_REPORT(("AF_INET:"));\
                SCTP_REPORT(("%X %s", p_debug_addr->ip.v4.addr, inet_ntoa(in)));\
            }\
            else if (p_debug_addr->family == SCTP_AF_INET6)\
            {\
                SCTP_REPORT(("AF_INET6:"));\
                SCTP_REPORT(("%X %X %X %X",\
                             p_debug_addr->ip.v6.addr.s_u6_addr32[0],\
                             p_debug_addr->ip.v6.addr.s_u6_addr32[1],\
                             p_debug_addr->ip.v6.addr.s_u6_addr32[2],\
                             p_debug_addr->ip.v6.addr.s_u6_addr32[3]));\
            }\
            SCTP_REPORT(("\n"));\
        }\
    }while(0)
#else

#define    SCTP_TRACE(p_ep, ttype, format_str)

#define    SCTP_HEX_DUMP(p_ep, ttype, p_buffer, noctets)

#define    SCTP_HEX_DUMP_IOV(p_ep, ttype, p_iovec, iov_len)

#define    SCTP_TRACE_ADDR(p_ep, ttype, p_addr)

#endif /* SCTP_DEBUG_ENABLED */

#ifdef __cplusplus
}
#endif


#endif /* __FILE_sctp_debg_SEEN__ */

