/***************************************************************************
 *  FILE NAME  : s_comac.h
 *
 *  DESCRIPTION: contains general macros used by the stack
 *
 *  DATE     NAME              REF#   REASON
 *  -------  ----------------  -----  -------------------------------------
 *  15Sep01  gsheoran          -----  Original
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ***************************************************************************/
#ifndef _S_CO_MAC_H_SEEN
#define _S_CO_MAC_H_SEEN_

#include <s_cogbl.h>

/*
 * Changes for CSR 1-2457756. The following two macros return an appropriate value of maximum fragment size.
 *
 * If  X is a multiple of 4 then X is returned otherwise (X -X%4) is returned.
 *
 * Here X is (pmtu - SCTP_MAX_IP_HEADER_SIZE - SCTP_UDP_HDR_SIZE -
 * SCTP_OFFSET_BODY - SCTP_MIN_DATACHUNK_LEN) for macro SCTP_UDP_MAX_FRAG_SIZE
 *
 * and  (pmtu-SCTP_MAX_IP_HEADER_SIZE-SCTP_OFFSET_BODY-SCTP_MIN_DATACHUNK_LEN)
 * for macro SCTP_IP_MAX_FRAG_SIZE
 */

#define SCTP_UDP_MAX_FRAG_SIZE(pmtu) \
    (pmtu - SCTP_MAX_IP_HEADER_SIZE - SCTP_UDP_HDR_SIZE - \
     SCTP_OFFSET_BODY - SCTP_MIN_DATACHUNK_LEN - \
     ( pmtu - SCTP_MAX_IP_HEADER_SIZE - SCTP_UDP_HDR_SIZE - \
       SCTP_OFFSET_BODY - SCTP_MIN_DATACHUNK_LEN ) % 4 )

#define SCTP_IP_MAX_FRAG_SIZE(pmtu) \
    (pmtu - SCTP_MAX_IP_HEADER_SIZE - SCTP_OFFSET_BODY -\
     SCTP_MIN_DATACHUNK_LEN - \
     ( pmtu - SCTP_MAX_IP_HEADER_SIZE -  SCTP_OFFSET_BODY -\
       SCTP_MIN_DATACHUNK_LEN) % 4  )

#define SCTP_UDP_MAX_HB_INFOLEN(pmtu) \
    (pmtu - SCTP_MAX_IP_HEADER_SIZE - SCTP_UDP_HDR_SIZE - \
     SCTP_OFFSET_BODY - SCTP_MIN_HBCHUNK_LEN - 4)

#define SCTP_IP_MAX_HB_INFOLEN(pmtu) \
    (pmtu - SCTP_MAX_IP_HEADER_SIZE - SCTP_OFFSET_BODY - \
     SCTP_MIN_HBCHUNK_LEN - 4)


/* Macro to set the error code. */
#define SCTP_SET_ECODE(p_ecode, err) \
    if (1) \
    {\
        if (p_ecode != SCTP_NULL) \
            *p_ecode = (sctp_error_t) err;\
    }


#define ECODE(p_ecode) ((p_ecode == SCTP_NULL)? 0: *p_ecode)


#endif /* _S_CO_MAC_H_SEEN_ */

