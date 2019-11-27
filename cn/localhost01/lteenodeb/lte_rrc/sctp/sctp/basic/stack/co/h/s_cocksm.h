/***************************************************************************
 *  FILE NAME:
 *      s_cocksm.h
 *
 *  DESCRIPTION:
 *      checksum calculation function
 *
 *  DATE     NAME               REF#   REASON
 *  -------  ----------------  -----  ----------------
 *  19Aug01  gsheoran            -     Rel 3.0
 *  26july02  dipak bash         -     Rel 4.0, crc32 checksum
 *  30Sep,02  Manish Rajpal      -     init time checksum selection
 *  26Aug 03  Manish Rajpal      -     Rel 5.0, per association checksum
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ******************************************************************/



#ifndef _S_CO_CKSM_H_
#define _S_CO_CKSM_H_
#include <s_cotyps.h>
#ifdef __cplusplus
extern "C" {
#endif

#define compute_cksm sctp_compute_cksm

    typedef unsigned int
    ( *sctp_compute_cksm_fn_t )
    ( unsigned char *buffer,
      unsigned int length );

    extern sctp_compute_cksm_fn_t sctp_compute_cksm[];

    typedef unsigned int
    ( *sctp_compute_cksm_iovec_fn_t )
    ( sctp_iovec_st           *p_iovec,
      unsigned int           iov_len );

    extern sctp_compute_cksm_iovec_fn_t sctp_compute_cksm_iovec[];

    void
    sctp_init_crc_table( void );

    unsigned int
    generate_crc32c(
        unsigned char *buffer,
        unsigned int length );


    unsigned int
    compute_adler32_cksm(
        unsigned char *buf,
        unsigned int  len );

    unsigned int
    sctp_compute_crc32_iovec(
        sctp_iovec_st           *p_iovec,
        unsigned int           iov_len );

    unsigned int
    sctp_compute_adler32_iovec(
        sctp_iovec_st           *p_iovec,
        unsigned int           iov_len );

#ifdef __cplusplus
}
#endif

#endif /* _S_CO_CKSM_H_ */
