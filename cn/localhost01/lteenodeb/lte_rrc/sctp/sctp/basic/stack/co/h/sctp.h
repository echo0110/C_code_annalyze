/*****************************************************************************
 *    FILE NAME:
 *      sctp.h
 *
 *    DESCRIPTION:
 *          This file has contains the SCTP preprocessor directives as
 *          recommende by IETF draft for socket APIs.
 *
 *    DATE        NAME       REFERENCE       REASON
 *    -----------------------------------------------------
 *    28Aug 2003  Manish Rajpal  -       .Original Creation
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ****************************************************************************/
#ifndef __FILE_sctp_h_SEEN__
#define __FILE_sctp_h_SEEN__


#define HAVE_SCTP                           1
#define HAVE_KERNEL_SCTP                    0
#define HAVE_SCTP_PRSCTP                    0

#ifdef SCTP_DYNAMIC_IP_SUPPORT
#define HAVE_SCTP_ADDIP                     1
#else
#define HAVE_SCTP_ADDIP                     0
#endif

#define HAVE_SCTP_CANSETPRIMARY             HAVE_SCTP_ADDIP
#define HAVE_SCTP_SAT_NETWORK_CAPABILITY    0

/*For indicating support for separate buffers for each association*/
#define HAVE_SCTP_MULTIBUF                  1/*Setting it to 0 has no significance*/
#define HAVE_SCTP_NOCONNECT                 1

#endif
