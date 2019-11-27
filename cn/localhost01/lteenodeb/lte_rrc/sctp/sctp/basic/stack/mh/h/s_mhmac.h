/*****************************************************************************
 *    FILE NAME:
 *          s_mhmac.h
 *
 *    DESCRIPTION:
 *           message parsing functions
 *
 *    DATE                  NAME         REFERENCE       REASON
 *    -----------------------------------------------------
 *    02June 2000     Sandeep Mahajan   -       Original Creation
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ****************************************************************************/
#ifndef       __FILE_sctp_mesg_mac_SEEN__
#define       __FILE_sctp_mesg_mac_SEEN__

#ifdef __cplusplus
extern "C" {
#endif

    typedef struct
    {
        sctp_U8bit   current_mac_key[SCTP_KEYSIZE];
        sctp_U8bit   old_mac_key[SCTP_KEYSIZE];
        sctp_time_t  switch_over_time;
    } sctp_mac_info_st;

    extern sctp_mac_info_st sctp_mac_info;

    extern sctp_timer_t mac_timer_id;

    void
    sctp_init_mac_keys(
        void );

    void
    sctp_process_mac_timeout(
        void );

    /* SPR 20568 Starts for CSR 1-6658486 */
    sctp_return_t
    sctp_is_cookie_valid(
        sctp_chunk_cookie_st  *cookie_chunk,
        sctp_U16bit          cookie_len );
    /* SPR 20568 Ends for CSR 1-6658486  */

    void
    sctp_init_chunk_cookie(
        sctp_chunk_cookie_st  *cookie_chunk );


#ifdef __cplusplus
}
#endif

#endif /* __FILE_sctp_mesg_mac_SEEN__ */



